# Cocos2d-x 4.0 Android Build Guide — 999xcoco

## Toolchain Configuration

- **Cocos2d-x Version**: `4.0`
- **NDK Version**: `28.2.13676358` (Verified)
- **Android SDK**: `~/Library/Android/sdk` (Compile SDK: `34`, Target SDK: `34`, Min SDK: `21`)
- **Java**: `OpenJDK 21.0.8 LTS`
- **Gradle**: `8.5` (Android Gradle Plugin `8.2.2`)
- **CMake**: `4.4.3` / `3.10+` (via CMakeLists.txt)
- **ABIs**: `arm64-v8a`, `armeabi-v7a`

## Build Commands

### Clean & Assemble Debug APK:
```bash
cd proj.android
./gradlew assembleDebug
```

### Install APK to connected device/emulator:
```bash
adb install -r app/build/outputs/apk/debug/app-debug.apk
```

### Run on Device:
```bash
adb shell am start -n org.cocos2dx.hellocpp/org.cocos2dx.cpp.AppActivity
```
