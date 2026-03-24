set(ELIX_LIBSRC
    elix.hpp
    elix-object.cpp
    elix-builtins.cpp
    elix-expr.cpp
    elix-scanner.cpp
    elix-math.cpp
    elix.cpp)

set(ELIX_SRC ${ELIX_LIBSRC} elix-main.cpp)

#set(ELIX_EXECNAME ELix)

#add_library(elix STATIC ${ELIX_LIBSRC})
#add_library(elix SHARED ${ELIX_LIBSRC})
add_executable(ELix ${ELIX_SRC})