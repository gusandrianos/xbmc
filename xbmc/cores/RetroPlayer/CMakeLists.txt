set(SOURCES RetroPlayer.cpp
            RetroPlayerAgent.cpp
            RetroPlayerAudio.cpp
            RetroPlayerAutoSave.cpp
            RetroPlayerInput.cpp
            RetroPlayerVideo.cpp)

set(HEADERS RetroPlayer.h
            RetroPlayerAgent.h
            RetroPlayerAudio.h
            RetroPlayerAutoSave.h
            RetroPlayerDefines.h
            RetroPlayerInput.h
            RetroPlayerVideo.h)

core_add_library(retroplayer)
