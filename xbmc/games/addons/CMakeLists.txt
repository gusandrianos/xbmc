set(SOURCES GameClient.cpp
            GameClientHardware.cpp
            GameClientInGameSaves.cpp
            GameClientJoystick.cpp
            GameClientKeyboard.cpp
            GameClientMouse.cpp
            GameClientPort.cpp
            GameClientProperties.cpp
            GameClientTiming.cpp
            GameClientTranslator.cpp)

set(HEADERS GameClient.h
            GameClientCallbacks.h
            GameClientHardware.h
            GameClientInGameSaves.h
            GameClientJoystick.h
            GameClientKeyboard.h
            GameClientMouse.h
            GameClientPort.h
            GameClientProperties.h
            GameClientTiming.h
            GameClientTranslator.h)

core_add_library(gameaddons)
