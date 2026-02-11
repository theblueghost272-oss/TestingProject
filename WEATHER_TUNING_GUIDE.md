# WeatherChange Lua Script Tuning Guide

This guide provides practical values to make your weather/day-night cycle feel smooth, less frequent, and immersive for gameplay.

## Key Issues in Current Script

1. **Day progresses too quickly**
   - Total `daytimeState` length is currently about **840 seconds (14 minutes)**.
   - Sunset and night transitions are compressed into short windows (`length = 120`, `60`, `30`) which feels abrupt.

2. **Weather changes too often**
   - Some weather states are as short as **30–60 seconds** (`snowstorm`, `storm`, `fog`).
   - That can feel noisy and unrealistic unless your game is intentionally chaotic.

3. **Tween completion is not awaited**
   - Tweens are started and promises resolve immediately, so visual interpolation may be cut by the next state if timings are short.

4. **`pairs()` on weather/season can be non-deterministic**
   - Using `pairs()` means weather order can vary run-to-run.
   - Not necessarily wrong, but can feel inconsistent during testing.

## Recommended Gameplay Targets

For a smooth multiplayer experience:

- **1 full in-game day = 45 to 60 real minutes**.
- **One weather event = 6 to 12 real minutes** on average.
- **Sunset and sunrise should each last 6 to 10 minutes**.
- **Tween speed** should be long enough to hide color jumps: **45–120 seconds** depending on state.

Good starting point:

- `TweenSpeed` attribute: **75**
- Day length target: **3600 seconds (60 min)**
- Weather event target: **480–900 seconds (8–15 min)**

## Suggested `daytimeState` (Smooth Sunset)

Use longer evening segments and one extra transition block before full night.

```lua
local daytimeState = {
    [1] = { length = 360, preset = "Dawn",   lighting = { properties = { Brightness = 1.1, ClockTime = 6.50, Ambient = Color3.new(0.78, 0.78, 0.80) } } },
    [2] = { length = 420, preset = "Dawn",   lighting = { properties = { Brightness = 1.2, ClockTime = 8.50, Ambient = Color3.new(0.88, 0.88, 0.90) } } },
    [3] = { length = 900, preset = "Midday", lighting = { properties = { Brightness = 1.3, ClockTime = 12.00, Ambient = Color3.new(1.00, 1.00, 1.00) } } },
    [4] = { length = 780, preset = "Midday", lighting = { properties = { Brightness = 1.2, ClockTime = 15.50, Ambient = Color3.new(0.95, 0.95, 0.95) } } },
    [5] = { length = 480, preset = "Dusk",   lighting = { properties = { Brightness = 1.0, ClockTime = 18.00, Ambient = Color3.new(0.85, 0.82, 0.80) } } },
    [6] = { length = 360, preset = "Dusk",   lighting = { properties = { Brightness = 0.8, ClockTime = 19.25, Ambient = Color3.new(0.70, 0.68, 0.70) } } },
    [7] = { length = 300, preset = "Night",  lighting = { properties = { Brightness = 0.6, ClockTime = 21.00, Ambient = Color3.new(0.58, 0.58, 0.62) } } },
}
```

### Why this feels better

- Sunset has **two long dusk stages**.
- Night onset is delayed and softened.
- Ambient color shifts happen over many minutes instead of sudden steps.

## Suggested `weatherState` Lengths

Keep your existing visual profiles, but extend durations.

```lua
sun       = 900,  -- 15 min
clear     = 840,  -- 14 min
rain      = 600,  -- 10 min
fog       = 540,  -- 9 min
snow      = 720,  -- 12 min
storm     = 420,  -- 7 min (rarer heavy weather)
snowstorm = 360,  -- 6 min (rarer heavy weather)
```

## Suggested Atmosphere Tweaks (Smoother Contrast)

These are safe baseline values to avoid harsh jumps:

- **Clear/Sun**
  - `Density`: `0.15–0.22`
  - `Haze`: `0.00–0.05`
  - `Glare`: `0.00–0.03`
- **Rain**
  - `Density`: `0.25–0.35` (raise from `0.1`)
  - `Haze`: `0.10–0.18`
  - `Glare`: `0.00`
- **Fog**
  - `Density`: `0.45–0.60` (reduce from `0.8` to avoid white wall)
  - `Offset`: `0.20–0.30`
  - `Haze`: `0.20–0.30`
- **Storm/Snowstorm**
  - `Density`: `0.35–0.50`
  - `Glare`: `0.05–0.12`
  - `Haze`: `0.15–0.25`

## Practical Implementation Notes

1. **Increase `TweenSpeed`**
   - Try `TweenSpeed = 75` first.
   - If transitions are still noticeable, raise toward `90`.

2. **Prevent overly frequent weather**
   - Keep extreme events (`storm`, `snowstorm`) in season includes, but longer base durations naturally reduce churn.

3. **Optionally add a hold buffer**
   - Add a short random hold between weather changes:
     - `task.wait(math.random(60, 180))`
   - This avoids mechanical cadence.

4. **Use deterministic ordering when needed**
   - For testing smoothness, iterate with a list (ipairs) instead of `pairs()` to keep reproducible order.

5. **Smooth color changes every few in-game hours**
   - Yes—this is exactly the right approach.
   - Keep each major lighting color target separated by long durations (6–15 minutes each in real-time), then tween over most of that segment.

## Recommended Starter Profile (If You Want One Setting)

- Day cycle: **60 minutes**
- Weather cycle: **8–15 minutes per event**
- Tween speed: **75 seconds**
- Dusk total duration: **14 minutes** (two phases)
- Night total duration: **5–8 minutes**

This gives a calm, cinematic progression that still feels dynamic during normal play sessions.
