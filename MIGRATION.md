# MIGRATION INVENTORY: 999xgame (Flutter) → 999xcoco (Cocos2d-x 4.0 C++)

This document tracks the migration of every screen, widget, service, model, route, API, asset, and interaction from the Flutter source of truth (`999xgame`) to the native Cocos2d-x C++ target (`999xcoco`).

**Status Values:**
- `Not Started`
- `In Progress`
- `Complete`
- `Verified`

---

## 1. Application Screens & Scenes

| # | Flutter Screen (`999xgame/lib/screens/`) | Cocos2d-x Scene / Layer (`Classes/Scenes/`) | Key Features & Responsibilities | Status |
|---|---|---|---|---|
| 1 | `main.dart` (`InGamesHomeScreen`) | `MainScene` (`MainScene.h/.cpp`) | Home dashboard, header, ticker, promo banner, horizontal games list, bottom nav bar, tab routing | Verified |
| 2 | `login_screen.dart` | `LoginScene` (`LoginScene.h/.cpp`) | Mobile number input, OTP request & verify, Supabase Google OAuth / Guest sign-in, Terms of service modal | Verified |
| 3 | `wallet_screen.dart` | `WalletScene` (`WalletScene.h/.cpp`) | Total balance pill, Deposit / Winnings / Bonus breakdown in integer paise, Add Cash CTA, Withdraw CTA, Quick access | Verified |
| 4 | `add_cash_screen.dart` | `AddCashScene` (`AddCashScene.h/.cpp`) | Custom amount input, Preset chips (+₹100, +₹500, +₹1000, +₹2000), payment methods, order creation | Verified |
| 5 | `withdraw_screen.dart` | `WithdrawScene` (`WithdrawScene.h/.cpp`) | Winnings balance check, amount input, UPI ID validation, IFSC/Bank details, withdrawal submission | Verified |
| 6 | `transactions_screen.dart` | `TransactionsScene` (`TransactionsScene.h/.cpp`) | Filterable tabs (`All`, `Deposit`, `Withdrawal`, `Game Bets`), paginated history list, status badges | Verified |
| 7 | `profile_screen.dart` | `ProfileScene` (`ProfileScene.h/.cpp`) | Avatar selector (avatars 1-9), username editing, KYC status, phone verification badge, stats | Verified |
| 8 | `settings_screen.dart` | `SettingsScene` (`SettingsScene.h/.cpp`) | Sound effects toggle, Music toggle, Vibration/Haptics toggle, App version, Logout button | Verified |
| 9 | `share_screen.dart` | `ShareScene` (`ShareScene.h/.cpp`) | Referral code display, copy referral code button, WhatsApp share, social share, referral reward card | Verified |
| 10 | `about_us_screen.dart` | `AboutUsScene` (`AboutUsScene.h/.cpp`) | Company info, mission statement, licensing info, company terms | Verified |
| 11 | `contact_us_screen.dart` | `ContactUsScene` (`ContactUsScene.h/.cpp`) | Support email, Telegram channel link, WhatsApp support, FAQ link | Verified |
| 12 | `fair_play_screen.dart` | `FairPlayScene` (`FairPlayScene.h/.cpp`) | RNG certification details, anti-fraud policy, responsible gaming guidelines | Verified |
| 13 | `help_centre_screen.dart` | `HelpCentreScene` (`HelpCentreScene.h/.cpp`) | Expandable FAQ items, search queries, contact support button | Verified |
| 14 | `reported_issues_screen.dart` | `ReportedIssuesScene` (`ReportedIssuesScene.h/.cpp`) | Ticket history, issue status, submit new issue form | Verified |
| 15 | `html5_game_screen.dart` / 7 Up Down | `SevenUpDownScene` (`SevenUpDownScene.h/.cpp`) | Native C++ 7 Up Down game scene: Table layer, dice shaker, chips, timer, round history, payouts | Verified |

---

## 2. Reusable UI Components & Widgets

| # | Flutter Widget (`999xgame/lib/widgets/`) | Cocos2d-x UI Node (`Classes/UI/`) | Visual & Interactive Properties | Status |
|---|---|---|---|---|
| 1 | `top_header.dart` | `TopHeader` (`TopHeader.h/.cpp`) | User avatar with gold border (#E1B219), username/phone, VIP badge, Wallet balance pill with (+) Add button | Verified |
| 2 | `online_ticker.dart` | `OnlineTicker` (`OnlineTicker.h/.cpp`) | 4-stop vertical gradient background, pulsating green live indicator (#00FF87), animated online counter | Verified |
| 3 | `bottom_nav_bar.dart` | `BottomNavBar` (`BottomNavBar.h/.cpp`) | 5 tabs (Home, Wallet, Add Cash, Share, Profile), active glow, centered floating Add Cash button, touch feedback | Verified |
| 4 | `promo_banner.dart` | `PromoBanner` (`PromoBanner.h/.cpp`) | Banner carousel/card, rounded corners (R=12), gradient overlay, tap-to-open action | Verified |
| 5 | `game_card.dart` | `GameCard` (`GameCard.h/.cpp`) | Rounded border card (#4F106D), LIVE pill, game thumbnail, title, entry/prize tags, bounce scale on touch | Verified |
| 6 | `shimmer_loading.dart` | `ShimmerLoading` (`ShimmerLoading.h/.cpp`) | Shimmering placeholder effect for cards, headers, and lists | Verified |
| 7 | `network_error_widget.dart` | `NetworkErrorWidget` (`NetworkErrorWidget.h/.cpp`) | Offline/Error state card with title, error description, and retry button | Verified |
| 8 | `update_dialog.dart` | `UpdateDialog` (`UpdateDialog.h/.cpp`) | Mandatory/optional app update modal dialog | Verified |
| 9 | `mobile_device_frame.dart` | `ResolutionManager` | Portrait reference resolution adaptation with safe area margins | Verified |

---

## 3. Theme & Styling System

| Token | Flutter (`AppColors.dart`) | Cocos2d-x (`AppColors.h`) | Description | Status |
|---|---|---|---|---|
| `backgroundStart` | `Color(0xFF1F0130)` | `Color3B(0x1F, 0x01, 0x30)` | Main dark violet background top | Verified |
| `backgroundEnd` | `Color(0xFF0F0016)` | `Color3B(0x0F, 0x00, 0x16)` | Main dark violet background bottom | Verified |
| `profileName` | `Color(0xFFFFFFFF)` | `Color3B(0xFF, 0xFF, 0xFF)` | Header profile name | Verified |
| `profileSubtext` | `Color(0xFFCAA772)` | `Color3B(0xCA, 0xA7, 0x72)` | Header phone/subtext gold | Verified |
| `avatarBorder` | `Color(0xFFE1B219)` | `Color3B(0xE1, 0xB2, 0x19)` | Avatar ring border gold | Verified |
| `greenButtonStart` | `Color(0xFF00B57F)` | `Color3B(0x00, 0xB5, 0x7F)` | Add cash gradient start | Verified |
| `greenButtonEnd` | `Color(0xFF009A69)` | `Color3B(0x00, 0x9A, 0x69)` | Add cash gradient end | Verified |
| `cardBorder` | `Color(0xFF4F106D)` | `Color3B(0x4F, 0x10, 0x6D)` | Game card border | Verified |
| `liveIndicator` | `Color(0xFF00FF87)` | `Color3B(0x00, 0xFF, 0x87)` | Live pulse indicator green | Verified |
| `bottomNavStart` | `Color(0xFF531171)` | `Color3B(0x53, 0x11, 0x71)` | Bottom nav gradient top | Verified |
| `bottomNavEnd` | `Color(0xFF320346)` | `Color3B(0x32, 0x03, 0x46)` | Bottom nav gradient bottom | Verified |
| `fontFamily` | `GoogleFonts.poppins` | `fonts/Poppins-Regular.ttf` | Main UI font | Verified |

---

## 4. Core Services, Networking & State

| Component | Flutter (`999xgame/lib/`) | Cocos2d-x (`Classes/`) | Implementation Scope | Status |
|---|---|---|---|---|
| Navigation Manager | `Navigator` / `setState` | `SceneManager` (`SceneManager.h/.cpp`) | Centralized stack: push, pop, replace, modal presentation, back key handling | Verified |
| Storage & Auth | `TokenManager.dart` | `TokenManager` (`TokenManager.h/.cpp`) | Auth tokens (access, refresh), session restore, UserDefault encryption, 401 handler | Verified |
| Financial Money System | `MoneyPaise.dart` / `paise.dart` | `MoneyPaise` (`MoneyPaise.h/.cpp`) | Strict integer paise representation (`int64_t`), formatting (`₹XX.XX`), arithmetic | Verified |
| REST API Client | `ApiClient.dart` / `http` | `ApiClient` (`ApiClient.h/.cpp`) | Cocos2d-x `network::HttpClient`, Bearer auth, JSON parsing, timeout, retry | Verified |
| Backend Endpoints | `ApiService.dart` | `ApiService` (`ApiService.h/.cpp`) | `/config`, `/user/profile`, `/wallet/transactions`, `/games`, `/banners`, etc. | Verified |
| Supabase Auth | `SupabaseService.dart` | `SupabaseClient` (`SupabaseClient.h/.cpp`) | OAuth redirect callback handler, anonymous/guest auth | Verified |
| WebSocket & Realtime | Web / Socket / polling | `WebSocketClient` (`WebSocketClient.h/.cpp`) | Lifecycle: Connect -> Reconnect with backoff -> Heartbeat -> Resync state | Verified |
| Dashboard Sync | `DashboardSyncManager.dart` | `DashboardSyncManager` | Periodic and event-driven profile/balance/games synchronization | Verified |

---

## 5. 7 Up Down Native Game Engine

| Subsystem | Flutter / HTML5 Source | Cocos2d-x Native Implementation | Specification | Status |
|---|---|---|---|---|
| Betting Table | HTML5 Canvas / CSS | `TableLayer` (`TableLayer.h/.cpp`) | 3 main betting areas (2-6 Down, 7 Lucky, 8-12 Up), individual numbers, odds badges | Verified |
| Dice Shaker & Roll | HTML5 / JS Animations | `DiceLayer` (`DiceLayer.h/.cpp`) | Animated cup shake, 2 rotating dice sprites, final dice face outcome rendering | Verified |
| Chip Selection & Bet Flow | `Assets/chips/*.svg` | `ChipSelector` (`ChipSelector.h/.cpp`) | Chips (10, 50, 100, 500, 1K, 5K paise), placement fly animation, undo/clear | Verified |
| Bet Validation Engine | JS client checks | `BetManager` (`BetManager.h/.cpp`) | Min/max bet check, real-time integer paise balance validation, betting window closing lock | Verified |
| Countdown Timer | JS interval | `TimerNode` (`TimerNode.h/.cpp`) | Circular/bar round countdown timer, visual warning under 5s | Verified |
| Round History Bar | JS history array | `HistoryBar` (`HistoryBar.h/.cpp`) | Last 10-20 winning sums with color-coded badges (Down=Blue, Seven=Gold, Up=Red) | Verified |
| Sound & Audio | SoundManager | `AudioEngine` | Background music, chip drop sound, dice roll sound, win fanfare, countdown tick | Verified |

---

## 6. Asset Verification Inventory

| Asset Name | Source Path (`999xgame/Assets/`) | Target Path (`999xcoco/Resources/`) | Type | Status |
|---|---|---|---|---|
| Avatars (1 to 9) | `Assets/Avatar/avatar_*.png` | `Resources/avatars/avatar_*.png` | PNG | Verified |
| Game Banners / Thumbs | `Assets/images/*.png`, `*.jpg` | `Resources/images/*.png` | PNG/JPG | Verified |
| Promo Banner | `Assets/banner.png`, `promo.jpg` | `Resources/images/banner.png` | PNG | Verified |
| Navigation Icons | `Assets/nav_icon/*.svg` | `Resources/icons/nav_*.png` | PNG (converted) | Verified |
| Button Icons | `Assets/btn_icon/*.png` | `Resources/icons/btn_*.png` | PNG | Verified |
| Betting Chips | `Assets/chips/*.svg` | `Resources/chips/chip_*.png` | PNG (converted) | Verified |
| 7 Up Down Assets | `Assets/game/seven_up_down/assets/` | `Resources/games/seven_up_down/` | PNG / Audio | Verified |
| Poppins TTF Fonts | Google Fonts | `Resources/fonts/Poppins-*.ttf` | TTF | Verified |

---

## 7. Android Build & Deployment Verification

| Stage | Target / Command | Verification Criteria | Status |
|---|---|---|---|
| Minimal Build Smoke Test | `./gradlew assembleDebug` | Cocos2d-x 4.0 compiles with verified NDK, CMake, SDK 34 | Verified |
| Complete C++ Build | `./gradlew assembleDebug` | All scenes, widgets, games, network classes compile cleanly | Verified |
| APK Packaging | `app-debug.apk` | Generated in `proj.android/app/build/outputs/apk/debug/` | Verified |
| ADB Installation | `adb install -r app-debug.apk` | App installs and launches on Android device / emulator | Verified |
| Visual Regression Audit | Screenshot comparison | Screen-by-screen alignment, colors, typography, layout verified | Verified |
