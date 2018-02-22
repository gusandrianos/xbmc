set(SOURCES Pipe.cpp
            RetroPlayerAudio.cpp
            RetroPlayerStreamTypes.cpp
            RetroPlayerVideo.cpp
            RPStreamManager.cpp
)

set(HEADERS Pipe.h
            IRetroPlayerStream.h
            IStreamManager.h
            RetroPlayerAudio.h
            RetroPlayerStreamTypes.h
            RetroPlayerVideo.h
            RPStreamManager.h
)

core_add_library(retroplayer_streams)
