#ifndef DEBUG_H_
#define DEBUG_H_

inline bool debug_out_flg = true;

#ifndef NO_DEBUG

#include <iostream>
#define DEBUG_PUTS(msg) puts(msg);
#define DOUT(stream) {if(debug_out_flg){std::cout<<stream;}}

#else

#define DEBUG_PUTS(msg)
#define DOUT(stream)
#endif

#endif
