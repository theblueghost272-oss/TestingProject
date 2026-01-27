import os
import re
import json
import time
from watchdog.observers import Observer
from watchdog.events import FileSystemEventHandler
from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
from google.oauth2 import service_account
from googleapiclient.discovery import build
from typing import Dict, List, Optional
from contextlib import asynccontextmanager

PROJECT_ROOT = os.path.expanduser("~/Desktop/Main/GameProject")
GOOGLE_CREDENTIALS_PATH = "google_credentials.json"
SPREADSHEET_ID = "192QPumURDoJ91vPDdqRV0fxR6NJ-GMRahVZ_byCaXe0"
TASK_RANGE = "Tasks!A2:D"

@asynccontextmanager
async def lifespan(app: FastAPI):
    event_handler = TaskCommentHandler()
    observer = Observer()
    observer.schedule(event_handler, PROJECT_ROOT, recursive=True)
    observer.start()
    
    verifier["needs_review"] = []
    fixer["auto_fix_queue"] = []
    
    print("AI Dev Simulator started!")
    print(f"Watching project directory: {PROJECT_ROOT}")
    
    yield
    
    observer.stop()
    observer.join()

app = FastAPI(title="AI Game Dev Simulator", version="1.0", lifespan=lifespan)

@app.get("/")
async def root():
    return {"message": "AI Game Dev Simulator Server Active", "time": time.ctime()}

@app.get("/alexa_status")
async def alexa_status():
    return {"status": "Alexa is ready and listening"}

class GoogleSheets:
    def __init__(self):
        self.creds = service_account.Credentials.from_service_account_file(
            GOOGLE_CREDENTIALS_PATH,
            scopes=["https://www.googleapis.com/auth/spreadsheets"]
        )
        self.service = build("sheets", "v4", credentials=self.creds)
    
    def update_task_status(self, task_id: str, status: str):
        try:
            sheet = self.service.spreadsheets()
            result = sheet.values().get(
                spreadsheetId=SPREADSHEET_ID,
                range=TASK_RANGE
            ).execute()
            
            values = result.get("values", [])
            for i, row in enumerate(values):
                if row and row[0] == task_id:
                    update_range = f"Tasks!D{i+2}"
                    body = {"values": [[status]]}
                    sheet.values().update(
                        spreadsheetId=SPREADSHEET_ID,
                        range=update_range,
                        valueInputOption="RAW",
                        body=body
                    ).execute()
                    return True
            return False
        except Exception as e:
            print(f"Google Sheets error: {str(e)}")
            return False

class TaskCommentHandler(FileSystemEventHandler):
    TASK_PATTERNS = [
        r"#task\s+(completed|done)",
        r"//\s*task\s+(completed|done)",
        r"/\*\s*task\s+(completed|done)\s*\*/"
    ]
    
    def __init__(self):
        self.sheets = GoogleSheets()
        self.last_processed = {}
    
    def on_modified(self, event):
        if event.is_directory:
            return
        
        filepath = event.src_path
        current_time = os.path.getmtime(filepath)
        
        if filepath in self.last_processed and current_time <= self.last_processed[filepath]:
            return
        
        self.last_processed[filepath] = current_time
        
        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                content = f.read()
            
            for pattern in self.TASK_PATTERNS:
                matches = re.findall(pattern, content, re.IGNORECASE)
                if matches:
                    task_id = os.path.basename(filepath).split('.')[0]
                    self.sheets.update_task_status(task_id, "Completed")
                    print(f"Task {task_id} marked completed from {filepath}")
                    break
        except Exception as e:
            print(f"File processing error: {str(e)}")

class CodeRequest(BaseModel):
    file_path: str
    requirements: str

class VerificationResult(BaseModel):
    file_path: str
    issues: List[Dict[str, str]]
    severity: str

class FixRequest(BaseModel):
    file_path: str
    issue_id: str

code_generator = {}
verifier = {}
fixer = {}

@app.post("/generate_code")
async def generate_code(request: CodeRequest):
    try:
        new_code = f"// Generated code for {request.requirements}\nvoid newFeature() {{}}"
        
        with open(request.file_path, 'w') as f:
            f.write(new_code)
        
        verifier["needs_review"].append(request.file_path)
        return {"status": "success", "file": request.file_path}
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

@app.post("/verify_code")
async def verify_code(file_path: str):
    if not os.path.exists(file_path):
        raise HTTPException(status_code=404, detail="File not found")
    
    issues = []
    if "buggy" in file_path:
        issues.append({
            "type": "logic_error",
            "description": "Potential null pointer dereference",
            "line": 42
        })
    
    status = "critical" if issues else "clean"
    result = VerificationResult(
        file_path=file_path,
        issues=issues,
        severity=status
    )
    
    task_id = os.path.basename(file_path).split('.')[0]
    sheets = GoogleSheets()
    sheets.update_task_status(task_id, "Needs Review" if issues else "Verified")
    
    return result

@app.post("/apply_fix")
async def apply_fix(request: FixRequest):
    with open(request.file_path, 'a') as f:
        f.write(f"\n// Fixed issue: {request.issue_id}")
    
    verifier["needs_review"].append(request.file_path)
    return {"status": "fixed", "issue": request.issue_id}

NEW_AI_ROLES = {}

def register_ai_module(name: str, capabilities: List[str], handler):
    NEW_AI_ROLES[name] = {
        "capabilities": capabilities,
        "handler": handler
    }
    print(f"Registered new AI module: {name}")

@app.post("/new_ai_module/{module_name}")
async def trigger_new_module(module_name: str, params: Dict):
    if module_name not in NEW_AI_ROLES:
        raise HTTPException(status_code=404, detail="Module not registered")
    
    module = NEW_AI_ROLES[module_name]
    return module["handler"](params)

if __name__ == "__main__":
    import uvicorn
    uvicorn.run(app, host="127.0.0.1", port=8080)


