# choice of C++ compiler
CC 			= g++

# Option explanation:
# -Wall -Wextra -Werror -pedantic  => be strict, force clean and robust code
# -std=c++11  => rely on language features from c++11
# -fPIC  => produce position-independent code, enable use of shared objects
# -fmax-error=3  => stop after 3 errors were detected
# -Wl,--export-dynamic  => add all symbols to dynamic symbol table, needed to access from dlopened-algorithm-library to global variables (memRegistryPtr)
# -D MEMTRACE_DISABLED => memtrace functionality will be disabled, for exact chronograph-results
MEMDISABLE	= -D MEMTRACE_DISABLED
CFLAGS 		= -O3 -Wall -Wextra -Werror -pedantic -std=c++11 -fPIC -fmax-errors=3 -Wl,--export-dynamic

# -shared => produce a shared object, which can be linked to other objects
CSOFLAGS	= -shared

APPNAME		= cate
BUILD		= ./build/
SRCDIR		= ./src/
INCLUDE		= ./include
EXPORT		= ./exec/

# general command for creating a directory for files
MKDIR		= @mkdir -p $(@D)

