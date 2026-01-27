// located Desktop/Main/GameProject and there is an Main exe
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <winsock2.h>
#include <windows.h>
#include <thread>
#include <chrono>
#include <deque>
#include <numeric>

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>
const int SERVER_STARTUP_DELAY = 5000;
const int MAX_CMD_LENGTH = 512;
const int MAX_CONNECTION_RETRIES = 10;
const int RETRY_DELAY = 2000;
const char *SERVER_URL = "http://127.0.0.1:8080";

static HWND hConsoleWindow = NULL;
static bool consoleVisible = false;
static bool serverConnected = false;
static std::string serverResponse = "";

class FPSCounter
{
private:
    std::deque<float> frameTimes;
    const size_t maxSamples = 90;
    float lastTime = 0.0f;

public:
    void update()
    {
        float currentTime = (float)glfwGetTime();
        if (lastTime > 0.0f)
        {
            float deltaTime = currentTime - lastTime;
            frameTimes.push_back(deltaTime);

            if (frameTimes.size() > maxSamples)
                frameTimes.pop_front();
        }
        lastTime = currentTime;
    }

    float getAverageFPS() const
    {
        if (frameTimes.empty())
            return 0.0f;

        float avgDelta = std::accumulate(frameTimes.begin(), frameTimes.end(), 0.0f) / frameTimes.size();
        return avgDelta > 0.0f ? 1.0f / avgDelta : 0.0f;
    }

    float getCurrentDeltaTime() const
    {
        return frameTimes.empty() ? 0.0f : frameTimes.back();
    }
};

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, std::string *response)
{
    size_t totalSize = size * nmemb;
    response->append((char *)contents, totalSize);
    return totalSize;
}

void ToggleConsole()
{
    if (!hConsoleWindow)
        hConsoleWindow = GetConsoleWindow();

    if (hConsoleWindow)
    {
        consoleVisible = !consoleVisible;
        ShowWindow(hConsoleWindow, consoleVisible ? SW_SHOW : SW_HIDE);
    }
}
bool StartServer()
{
    std::string cmd = R"(D:\Appdata\Local\Programs\Python\Python311\python.exe -m uvicorn Server:app --port 8080)";
    std::string workDir = R"(C:\Users\Francy\Desktop\Main)";

    char mutableCmd[MAX_CMD_LENGTH];
    if (strncpy_s(mutableCmd, sizeof(mutableCmd), cmd.c_str(), cmd.size()) != 0)
    {
        std::cerr << "Error: Command string too long" << std::endl;
        return false;
    }

    char mutableWorkDir[MAX_CMD_LENGTH];
    if (strncpy_s(mutableWorkDir, sizeof(mutableWorkDir), workDir.c_str(), workDir.size()) != 0)
    {
        std::cerr << "Error: Work directory string too long" << std::endl;
        return false;
    }

    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcessA(NULL, mutableCmd, NULL, NULL, FALSE,
                        CREATE_NEW_CONSOLE, NULL, mutableWorkDir, &si, &pi))
    {
        std::cerr << "Failed to start server. Error code: " << GetLastError() << std::endl;
        return false;
    }

    std::cout << "Server process started (PID: " << pi.dwProcessId << ")" << std::endl;
    std::cout << "Waiting " << (SERVER_STARTUP_DELAY / 1000) << " seconds for server to initialize..." << std::endl;
    Sleep(SERVER_STARTUP_DELAY);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}

bool ConnectToServer()
{
    curl_global_init(CURL_GLOBAL_ALL);

    serverResponse.clear();
    CURLcode res = CURLE_COULDNT_CONNECT;
    int retry_count = 0;

    while (retry_count < MAX_CONNECTION_RETRIES && res != CURLE_OK)
    {
        CURL *curl = curl_easy_init();
        if (!curl)
        {
            std::cerr << "Failed to initialize CURL" << std::endl;
            curl_global_cleanup();
            return false;
        }

        serverResponse.clear();
        curl_easy_setopt(curl, CURLOPT_URL, SERVER_URL);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &serverResponse);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 2L);

        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            retry_count++;
            if (retry_count < MAX_CONNECTION_RETRIES)
            {
                if (consoleVisible)
                    std::cout << "Waiting for server... (attempt " << retry_count << "/" << MAX_CONNECTION_RETRIES << ")" << std::endl;
                Sleep(RETRY_DELAY);
            }
        }

        curl_easy_cleanup(curl);
    }

    if (res != CURLE_OK)
    {
        std::cerr << "Failed to connect to server after " << MAX_CONNECTION_RETRIES << " attempts: " << curl_easy_strerror(res) << std::endl;
        curl_global_cleanup();
        return false;
    }

    if (consoleVisible)
        std::cout << "\n[Server Response]: " << serverResponse << std::endl;

    curl_global_cleanup();
    return true;
}

void ShowDebugOverlay(FPSCounter &fpsCounter, bool *p_open = NULL)
{
    const float DISTANCE = 10.0f;
    const ImVec2 window_pos = ImVec2(DISTANCE, DISTANCE);
    const ImVec2 window_pos_pivot = ImVec2(0.0f, 0.0f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_FirstUseEver, window_pos_pivot);
    ImGui::SetNextWindowBgAlpha(0.35f);

    ImGui::Begin("Debug Overlay", p_open,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                     ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

    ImGui::Text("FPS: %.1f", fpsCounter.getAverageFPS());
    ImGui::Text("Delta: %.3f ms", fpsCounter.getCurrentDeltaTime() * 1000.0f);
    ImGui::Separator();
    ImGui::Text("Server Status: %s", serverConnected ? "Connected" : "Disconnected");
    ImGui::Text("T - Toggle Console");
    ImGui::Text("Esc - Exit");

    ImGui::End();
}

int main()
{
    hConsoleWindow = GetConsoleWindow();
    ShowWindow(hConsoleWindow, SW_HIDE);
    consoleVisible = false;

    if (!StartServer())
    {
        ShowWindow(hConsoleWindow, SW_SHOW);
        std::cerr << "Failed to initialize server" << std::endl;
        return 1;
    }

    if (!ConnectToServer())
    {
        ShowWindow(hConsoleWindow, SW_SHOW);
        std::cerr << "Failed to connect to server" << std::endl;
        return 1;
    }

    serverConnected = true;
    ShowWindow(hConsoleWindow, SW_SHOW);
    std::cout << "Server connected! Launching GUI..." << std::endl;

    // Initialize GLFW
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return 1;
    }

    const char *glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(1280, 720, "AI Game Dev Simulator", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    FPSCounter fpsCounter;

    // Main loop
    while (!glfwWindowShouldClose(window))
    {
        fpsCounter.update();

        glfwPollEvents();

        // T key to toggle console
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        {
            static bool tKeyPressed = false;
            if (!tKeyPressed)
            {
                ToggleConsole();
                tKeyPressed = true;
            }
        }
        else
        {
            static bool tKeyPressed = false;
            tKeyPressed = false;
        }

        // ESC key to exit
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true);
        }

        // Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Show debug overlay
        ShowDebugOverlay(fpsCounter);

        // Example main content window
        ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
        ImGui::Begin("Main Window");

        ImGui::Text("AI Game Dev Simulator");
        ImGui::Separator();

        ImGui::TextWrapped("Server Response:");
        ImGui::TextWrapped("%s", serverResponse.c_str());

        if (ImGui::Button("Refresh Server Status", ImVec2(200, 0)))
        {
            // TODO: Make another server request
        }

        ImGui::End();

        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
