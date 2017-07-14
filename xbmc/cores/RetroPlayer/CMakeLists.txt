set(SOURCES PixelConverter.cpp
            RetroPlayer.cpp
            RetroPlayerAudio.cpp
            RetroPlayerAutoSave.cpp
            RetroPlayerClock.cpp
            RetroPlayerInput.cpp
            RetroPlayerVideo.cpp)

set(HEADERS IPixelConverter.h
            PixelConverter.h
            RetroPlayer.h
            RetroPlayerAudio.h
            RetroPlayerAutoSave.h
            RetroPlayerClock.h
            RetroPlayerDefines.h
            RetroPlayerInput.h
            RetroPlayerVideo.h)

if(CORE_PLATFORM_NAME_LC STREQUAL rbpi)
  list(APPEND SOURCES PixelConverterRBP.cpp)
  list(APPEND HEADERS PixelConverterRBP.h)
endif()

core_add_library(retroplayer)
