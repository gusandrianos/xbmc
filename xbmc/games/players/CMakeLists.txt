set(SOURCES InputSink.cpp
            Player.cpp
            PlayerManager.cpp
)

set(HEADERS InputSink.h
            IPlayerHandler.h
            Player.h
            PlayerManager.h
            PlayerTypes.h
)

core_add_library(games_players)
