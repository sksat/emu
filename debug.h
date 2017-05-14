#ifndef DEBUG_H_
#define DEBUG_H_

#ifndef NO_DEBUG

#include <stdio.h>
#define DEBUG_PUTS(msg) puts(msg);

#else

#define DEBUG_PUTS

#endif

#endif
