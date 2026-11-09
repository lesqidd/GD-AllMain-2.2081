# All Main Completer — Geometry Dash 2.2081 (Windows)

A **Geode C++ source project** that adds a **Main 100%** button to the Geometry Dash main menu.

When you click the button and confirm, it attempts to:

- Mark **all 22 original classic main levels** (Stereo Madness to Dash) completed in Normal mode.
- Set **Normal** and **Practice** progress to 100%.
- Award the **three secret coins per level (66 total)** using the game's native coin keys.
- Update the associated local completion, star, and demon records where applicable.
- Save the change using GD's own save managers.

**Scope:** Does not touch online levels, map packs, or the four Tower platformer levels. It is not a bot and does not actually play the levels. Doesn't guarantee server-side profile/leaderboard or platform achievements.

**Important:** This is a source-only, **uncompiled and untested** project. It was written using Geode's published 2.2081 binding signatures; the `.geode` binary must be built and tested. A successful build is not proof the runtime save changes work correctly.

## Back up your save first

1. Close Geometry Dash.
2. Press `Win + R`, paste `%LOCALAPPDATA%\GeometryDash`, press Enter.
3. Copy the **entire folder**, especially `CCGameManager.dat` and `CCLocalLevels.dat`, somewhere safe.
4. If you use GD's **Account > Save** feature, record whether your cloud copy predates this mod. **Do not cloud-load after applying** unless you intend to overwrite local progress.

## Build automatically through GitHub (no local C++ setup)

1. Create an empty **private GitHub repository**.
2. Upload **all the contents of this zip** to its root, including the hidden `.github/workflows/build.yml` file.
3. Open **Actions** → **Build GD 2.2081 mod** → **Run workflow** (if it did not automatically run from your push).
4. Open the finished run → **Artifacts** → **GD-AllMain-2.2081-Win64**.
5. Download the artifact zip; extract the `.geode` file from it.
6. Put the `.geode` file in `<Steam>/steamapps/common/Geometry Dash/geode/mods/` and restart GD 2.2081 with Geode 5.x installed.
7. On the main menu, press **Main 100%** → read the warning → **Apply**.
8. Reopen the official levels to verify normal progress, practice progress, and coins. If anything is incorrect, exit GD and restore your local save backup.

GitHub Actions uses the official `geode-sdk/build-geode-mod` build action and Geode SDK v5.11.2. It will need access to download the SDK and build dependencies. If the workflow fails, share the build log; the project has not been built here.

## Local build (optional)

Requires Visual Studio 2022 C++ Build Tools, CMake 3.21+, Geode CLI, and SDK v5.11.2 configured as `%GEODE_SDK%`:

```cmd
cmake -S . -B build -A x64
cmake --build build --config Release
```

The Geode CMake packaging step should produce the installable `.geode` file in the build output.

## What is deliberately not included

No network requests, no telemetry, no DLL injector, no account login, no external scripts. The mod activates only after an explicit two-button confirmation. It modifies only game local progress and invokes the game's own save interfaces.
