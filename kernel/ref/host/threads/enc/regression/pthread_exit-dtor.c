#include "test.h"
#include <unistd.h>
#define main pthread_exit_dtor_main
#include "../../../../../3rdparty/libc/libc-test/src/regression/pthread_exit-dtor.c"
