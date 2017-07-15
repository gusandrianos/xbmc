set(SOURCES GUIPlayerPanel.cpp
            GUIPlayerWindow.cpp
            GUIVirtualPortPanel.cpp
)

set(HEADERS GUIPlayerPanel.h
            GUIPlayerWindow.h
            GUIPlayerWindowDefines.h
            GUIVirtualPortPanel.h
            IPlayerWindow.h
)

core_add_library(games_players_windows)
