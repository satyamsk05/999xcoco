cmake_minimum_required(VERSION 3.6)

set(APP_NAME MyGame)

project(${APP_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT DEFINED BUILD_ENGINE_DONE)
    if(NOT DEFINED COCOS2DX_ROOT_PATH)
        if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/cocos2d/cocos/cocos2d.h")
            set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
        elseif(EXISTS "$ENV{COCOS2DX_ROOT_PATH}")
            set(COCOS2DX_ROOT_PATH $ENV{COCOS2DX_ROOT_PATH})
        elseif(EXISTS "/Users/satyamkumar/Desktop/cocos2d-x-4.0")
            set(COCOS2DX_ROOT_PATH "/Users/satyamkumar/Desktop/cocos2d-x-4.0")
        else()
            set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
        endif()
    endif()
    set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

    include(CocosBuildSet)
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)
endif()

# record sources, headers, resources...
set(GAME_SOURCE)
set(GAME_HEADER)

set(GAME_RES_FOLDER
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
    )

# add cross-platform source and header files
list(APPEND GAME_SOURCE
     Classes/AppDelegate.cpp
     Classes/Theme/AppColors.cpp
     Classes/Core/Storage/TokenManager.cpp
     Classes/App/SceneManager.cpp
     Classes/Network/ApiClient.cpp
     Classes/Network/ApiService.cpp
     Classes/Network/WebSocketClient.cpp
     Classes/UI/TopHeader.cpp
     Classes/UI/OnlineTicker.cpp
     Classes/UI/BottomNavBar.cpp
     Classes/UI/PromoBanner.cpp
     Classes/UI/GameCard.cpp
     Classes/Games/SevenUpDown/BetManager.cpp
     Classes/Games/SevenUpDown/TableLayer.cpp
     Classes/Games/SevenUpDown/DiceLayer.cpp
     Classes/Games/SevenUpDown/ChipSelector.cpp
     Classes/Games/SevenUpDown/HistoryBar.cpp
     Classes/Games/SevenUpDown/SevenUpDownScene.cpp
     Classes/Scenes/MainScene.cpp
     Classes/Scenes/WalletScene.cpp
     Classes/Scenes/AddCashScene.cpp
     Classes/Scenes/WithdrawScene.cpp
     Classes/Scenes/TransactionsScene.cpp
     Classes/Scenes/ProfileScene.cpp
     Classes/Scenes/SettingsScene.cpp
     Classes/Scenes/ShareScene.cpp
     Classes/Scenes/InfoScene.cpp
     Classes/Scenes/LoginScene.cpp
     )

list(APPEND GAME_HEADER
     Classes/AppDelegate.h
     Classes/Theme/AppColors.h
     Classes/Core/MoneyPaise.h
     Classes/Core/Storage/TokenManager.h
     Classes/App/SceneManager.h
     Classes/Network/ApiClient.h
     Classes/Network/ApiService.h
     Classes/Network/WebSocketClient.h
     Classes/UI/TopHeader.h
     Classes/UI/OnlineTicker.h
     Classes/UI/BottomNavBar.h
     Classes/UI/PromoBanner.h
     Classes/UI/GameCard.h
     Classes/Games/SevenUpDown/BetManager.h
     Classes/Games/SevenUpDown/TableLayer.h
     Classes/Games/SevenUpDown/DiceLayer.h
     Classes/Games/SevenUpDown/ChipSelector.h
     Classes/Games/SevenUpDown/HistoryBar.h
     Classes/Games/SevenUpDown/SevenUpDownScene.h
     Classes/Scenes/MainScene.h
     Classes/Scenes/WalletScene.h
     Classes/Scenes/AddCashScene.h
     Classes/Scenes/WithdrawScene.h
     Classes/Scenes/TransactionsScene.h
     Classes/Scenes/ProfileScene.h
     Classes/Scenes/SettingsScene.h
     Classes/Scenes/ShareScene.h
     Classes/Scenes/InfoScene.h
     Classes/Scenes/LoginScene.h
     )

if(ANDROID)
    set(APP_NAME MyGame)
    list(APPEND GAME_SOURCE
         proj.android/app/jni/hellocpp/main.cpp
         )
endif()

set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
    )

if(NOT ANDROID)
    add_executable(${APP_NAME} ${all_code_files})
else()
    add_library(${APP_NAME} SHARED ${all_code_files})
    add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos/platform/android ${ENGINE_BINARY_PATH}/cocos/platform)
    target_link_libraries(${APP_NAME} -Wl,--whole-archive cpp_android_spec -Wl,--no-whole-archive -Wl,-Bsymbolic -Wl,--exclude-libs,libcrypto.a -Wl,--exclude-libs,libssl.a)
endif()

target_link_libraries(${APP_NAME} cocos2d -Wl,-Bsymbolic -Wl,--exclude-libs,libcrypto.a -Wl,--exclude-libs,libssl.a)
target_include_directories(${APP_NAME}
        PRIVATE Classes
        PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
)

setup_cocos_app_config(${APP_NAME})
