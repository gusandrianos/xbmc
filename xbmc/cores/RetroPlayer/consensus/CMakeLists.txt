set(SOURCES RaftEntry.cpp
            RaftLog.cpp
            RaftPeer.cpp
            RaftServer.cpp
)

set(HEADERS RaftEntry.h
            RaftLog.h
            RaftPeer.h
            RaftServer.h
)

core_add_library(retroplayer_consensus)
