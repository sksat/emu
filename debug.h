#ifndef DEBUG_H_
#define DEBUG_H_

namespace debug {
	extern bool out_flg;
}

#ifndef NO_DEBUG

#include <iostream>

#define DEBUG_PUTS(msg) puts(msg);
#define DOUT(stream) {if(debug::out_flg){std::cout<<stream;}}

#else

#define DEBUG_PUTS(msg)
#define DOUT(stream)
#endif

#endif
