#ifndef DEBUG_H_
#define DEBUG_H_

#ifndef NO_DEBUG

#include <iostream>
#define DEBUG_PUTS(msg) puts(msg);
#define DOUT(stream) std::cout<<stream;

#else

#define DEBUG_PUTS(msg)
#define DOUT(stream)
#endif

#endif
