#ifndef SKSAT_COMMON_HPP_
#define SKSAT_COMMON_HPP_

#include "platform.hpp"

#include <cstdio>

namespace sksat
{
	using ::FILE;
	using ::fpos_t;

	using ::clearerr;
	using ::fclose;
	using ::feof;
	using ::ferror;
	using ::fflush;
	using ::fgetc;
	using ::fgetpos;
	using ::fgets;
	using ::fopen;
	using ::fprintf;
	using ::fputc;
	using ::fputs;
	using ::fread;
	using ::freopen;
	using ::fscanf;
	using ::fseek;
	using ::fsetpos;
	using ::ftell;
	using ::fwrite;
	using ::getc;
	using ::getchar;

#ifdef OS_WIN
	#warning popen,pclose has not been declared (why?)
#else
	using ::popen;
	using ::pclose;
#endif
}

#ifndef NO_STL
	#include <vector>
	#include <string>
	#include <map>
	#include <functional>
	#include <sstream>
	namespace sksat {
		using std::vector;
		using std::string;
		using std::map;
		using std::stringstream;
	}
#endif

#endif
