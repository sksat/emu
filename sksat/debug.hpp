#ifndef SKSAT_DEBUG_HPP_
#define SKSAT_DEBUG_HPP_

#include <sstream>

#define ASSERT(flg,msg) { \
if(flg){ \
	std::stringstream ss;\
	ss<<"error: "<<__FILE__<<": "<<__LINE__<<"\n\r"<<msg<<std::endl;\
	throw ss.str();\
}}

#endif
