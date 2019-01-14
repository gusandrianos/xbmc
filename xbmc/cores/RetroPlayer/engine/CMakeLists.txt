set(SOURCES Core.cpp
            Demands.cpp
            Predicates.cpp
            Subjects.cpp
            Supplies.cpp
            TaskQueues.cpp
            Tasks.cpp
            ThreadPools.cpp
)

set(HEADERS Classes.h
            Core.h
            Demands.h
            Predicates.h
            Subjects.h
            Supplies.h
            TaskQueues.h
            Tasks.h
            ThreadPools.h
            Types.h
)

core_add_library(retroplayer_engine)
