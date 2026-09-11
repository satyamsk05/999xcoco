MIGRATE 999xgame → 999xcoco

SOURCE REPOSITORY:
https://github.com/satyamsk05/999xgame

TARGET REPOSITORY:
https://github.com/satyamsk05/999xcoco

OBJECTIVE:
Create a Cocos2d-x 4.0 C++ Android version of the existing 999x Flutter application.

VERY IMPORTANT:
The existing Flutter app is the SOURCE OF TRUTH.
Do NOT invent a new UI.
Do NOT redesign anything.
Do NOT remove existing functionality.
Do NOT create unnecessary branches.
Do NOT modify the source 999xgame repository.
All new implementation goes into 999xcoco.

TARGET:
Android ONLY.
Do NOT install or require Xcode.
Do NOT use Flutter in the new application.
Do NOT use React Native.
Do NOT use HTML/CSS as the main UI.
Use Cocos2d-x 4.0 + C++.

==================================================
PHASE 1 — FULL SOURCE AUDIT
==================================================

First inspect the complete 999xgame repository.

Pay special attention to:

lib/main.dart
lib/screens/
lib/features/
lib/widgets/
lib/services/
lib/core/
lib/shared/
lib/theme/
Assets/
pubspec.yaml

Identify:

1. Every application screen
2. Every navigation route
3. Every reusable widget
4. Every theme/color/font definition
5. Every image/icon/asset
6. Every game-related component
7. Every API service
8. Authentication flow
9. Supabase usage
10. WebSocket / Socket.IO usage
11. Local storage
12. WebView usage
13. Game state management
14. Loading/error states
15. Bottom navigation
16. Header/navigation UI
17. Wallet/balance UI
18. Profile UI
19. 7 Up Down UI and game assets

Create:
MIGRATION.md

containing a Flutter → Cocos2d-x mapping for every important component.

==================================================
PHASE 2 — COCOSED2D-X PROJECT
==================================================

Create a proper Cocos2d-x 4.0 C++ Android project in:

999xcoco/

Use a clean production structure such as:

Classes/
    App/
    Core/
    Scenes/
    UI/
    Widgets/
    Games/
        SevenUpDown/
    Services/
    Network/
    Models/
    Storage/
    Theme/
    Utils/

Resources/
    images/
    icons/
    fonts/
    games/
        seven_up_down/

proj.android/
CMakeLists.txt
README.md

Do NOT copy Flutter source files into the target.
Do NOT create fake .cpp files containing Flutter/Dart code.

==================================================
PHASE 3 — UI MIGRATION
==================================================

Recreate the existing Flutter UI in Cocos2d-x.

Flutter → Cocos2d-x mapping:

Scaffold
→ Scene / Layer hierarchy

Container
→ Node / Layer

Row / Column
→ Node hierarchy with explicit layout

Stack
→ layered Nodes

Image
→ Sprite

Text
→ Label

Icon
→ Sprite / custom icon node

Button
→ Button/custom touch-enabled Node

GestureDetector
→ Cocos touch listener

AnimationController
→ Cocos2d-x Action / Animation / Scheduler

Theme
→ centralized C++ Theme constants

Do NOT approximate the design unnecessarily.

Preserve:
- dimensions
- spacing
- typography
- colors
- borders
- radius
- shadows
- icons
- images
- alignment
- navigation behavior
- responsive behavior

The resulting Android UI should visually match the Flutter source as closely as possible.

==================================================
PHASE 4 — ASSETS
==================================================

Inspect all existing Flutter assets.

Reuse existing assets wherever licensing/project ownership permits.

Important asset directories include:

Assets/
Assets/images/
Assets/Avatar/
Assets/nav_icon/
Assets/btn_icon/
Assets/chips/
Assets/game/seven_up_down/
Assets/game/seven_up_down/assets/
Assets/game/seven_up_down/assets/chips/

Do not replace existing assets with random/generated alternatives.

Preserve asset names where practical.

Create a Cocos ResourceManager if useful.

==================================================
PHASE 5 — APPLICATION SCREENS
==================================================

Migrate all relevant Flutter screens to Cocos2d-x.

Examples already visible in the source include:

Login
Profile
Settings
Transactions
Add Cash
About Us
Contact Us
Fair Play
Help Centre
Share
Reported Issues
HTML5/Game screen

Do not skip screens simply because they are not part of the first game screen.

Use separate Cocos scenes/layers where appropriate.

==================================================
PHASE 6 — 7 UP DOWN
==================================================

The existing 7 Up Down implementation/assets are the highest priority game feature.

Recreate the existing design and behavior.

Preserve the current concepts:

- dice
- betting table
- 2–6
- 7
- 8–12
- individual numbers
- odds
- chips
- countdown
- round history
- player/bet display
- balance
- betting controls
- animations
- sounds where existing assets/behavior exist

Do NOT invent new betting options.
Do NOT redesign the table.

Use Cocos2d-x rendering and animation.

The client must NOT independently determine authoritative game results.

==================================================
PHASE 7 — BACKEND / NETWORK
==================================================

Inspect existing:

lib/services/
lib/core/
lib/features/

Determine exactly how the Flutter app communicates with the backend.

Document:

- API base URLs/configuration
- endpoints
- authentication
- token handling
- Supabase
- Socket.IO/WebSocket
- request/response models
- wallet operations
- game-round communication

Implement equivalent C++ networking in Cocos2d-x.

Do not hard-code secrets.

Do not copy service credentials into source code.

If a backend service cannot be directly ported, document the adapter required rather than silently removing functionality.

==================================================
PHASE 8 — ANDROID BUILD
==================================================

Target Android only.

Use the existing local environment:

Cocos2d-x:
4.0

Android SDK:
~/Library/Android/sdk

ANDROID_HOME:
~/Library/Android/sdk

NDK currently installed:
~/Library/Android/sdk/ndk/28.2.13676358

Java:
OpenJDK 21.0.8

CMake:
4.4.3

IMPORTANT:
Before building, inspect Cocos2d-x 4.0's actual Android requirements.

Do NOT assume NDK 28 is compatible.

If another NDK version is required:
- identify it
- install it if necessary
- configure the project explicitly
- keep the existing NDK unless removal is necessary

Build a DEBUG APK.

==================================================
PHASE 9 — VISUAL VERIFICATION
==================================================

After the first successful build:

Run the application on an Android emulator/device if available.

Verify:

1. App launches
2. Login screen
3. Navigation
4. Main UI
5. Assets
6. Fonts
7. Buttons/touch
8. Profile
9. Wallet-related screens
10. 7 Up Down screen
11. Dice/table rendering
12. Countdown
13. Betting UI
14. Round history
15. Screen scaling

Fix visual/layout issues found during testing.

==================================================
PHASE 10 — QUALITY
==================================================

Do not produce a toy/demo implementation.

Use:

- clean C++
- clear ownership/lifecycle
- reusable components
- centralized theme
- centralized asset paths
- proper scene transitions
- error handling
- network abstraction
- model classes
- configuration files
- no hard-coded secrets
- no unnecessary dependencies

Do not rewrite the backend unless required.

==================================================
GIT RULES
==================================================

Target repository only:

999xcoco

Do NOT push changes to:

999xgame

Do NOT create unnecessary branches.

Work on the existing default branch of 999xcoco unless there is a specific technical reason otherwise.

Commit logically grouped changes.

==================================================
FINAL REQUIREMENT
==================================================

Do not tell me that the migration is complete merely because files were generated.

Actually build the Android project.

At the end report:

1. What was migrated
2. What remains
3. Cocos2d-x version
4. NDK version used
5. Java version
6. CMake version
7. Android compile SDK
8. Build tools
9. APK path
10. Build command
11. Whether APK build succeeded
12. Any remaining blockers

Most important:
The visual design of the existing Flutter 999x application is the source of truth.
The new Cocos2d-x application must reproduce that design, not create a different design.