# 999x Game — Cocos2d-x 4.0 Android Native Build Guide

## 1. Environment & Verified Toolchain
- **OS**: macOS (Apple Silicon / Intel) / Linux
- **JDK**: Java 17+ (Java 17 / OpenJDK 21 LTS)
- **Android SDK**: API Level 34 (Android 14)
- **Android NDK**: `28.2.13676358`
- **Gradle**: `8.5` (Android Gradle Plugin `8.2.2`)
- **CMake**: `3.22.1+`
- **Target ABIs**: `arm64-v8a` (64-bit modern devices) & `armeabi-v7a` (32-bit compatibility)
- **App Package ID**: `org.cocos2dx.hellocpp`
- **App Name**: `999x Game`

---

## 2. Cocos2d-x Engine Setup
The build system dynamically discovers Cocos2d-x 4.0 in one of the following locations:
1. `cocos2d/` (Symlink or Git submodule in project root)
2. `COCOS2DX_ROOT_PATH` environment variable:
   ```bash
   export COCOS2DX_ROOT_PATH="/path/to/cocos2d-x-4.0"
   ```
3. Local `/Users/satyamkumar/Desktop/cocos2d-x-4.0`

### To setup a symlink or submodule on fresh clone:
```bash
# Option A: Symlink existing Cocos2d-x 4.0
ln -s /path/to/cocos2d-x-4.0 cocos2d

# Option B: Or export environment variable
export COCOS2DX_ROOT_PATH=/path/to/cocos2d-x-4.0
```

---

## 3. Clean Build Command
To execute a clean, verified build from scratch:
```bash
cd proj.android
./gradlew clean assembleDebug
```

Output APK will be generated at:
`proj.android/app/build/outputs/apk/debug/HelloCpp-debug.apk`

---

## 4. Install & Launch on Android Device / Emulator
```bash
# 1. Install APK
adb install -r proj.android/app/build/outputs/apk/debug/HelloCpp-debug.apk

# 2. Launch Main Activity
adb shell am start -n org.cocos2dx.hellocpp/org.cocos2dx.cpp.AppActivity

# 3. View Logcat Debug Output
adb logcat -s Cocos2dx MyGame:V
```

---

## 5. Security & Persistence
- **Token Storage**: `TokenManager` utilizes salted XOR obfuscation & hex encoding for JWT tokens and user session persistence.
- **Financial Architecture**: Strict integer paise precision (`MoneyPaise`) preventing floating-point arithmetic errors.
- **Network Handshake**: `ApiClient` with crash-safe response handlers and `ApiService` with type-safe RapidJSON parsers.
