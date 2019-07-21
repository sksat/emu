#ifndef SKSAT_PLATFORM_HPP
#define SKSAT_PLATFORM_HPP


// default define list: `CXX -dM -E -x c++ /dev/null`
// MEMO: http://herumi.in.coocan.jp/prog/gcc-and-vc.html

// OS detect
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
	// Windows
	#define OS_WIN 32
	#ifdef _WIN64
		// Windows 64bit
		#undef  OS_WIN
		#define OS_WIN 64
		#define OS_WIN64
        #else
		// Windows 32bit
		#define OS_WIN32
	#endif
#elif __APPLE__
	// Apple
	#define OS_APPLE
#elif defined(linux) || defined(__linux) || defined(__linux__)
	// Linux
	#define OS_LINUX 32
	#ifdef __x86_64__
		#undef  OS_LINUX
		#define OS_LINUX 64
		#define OS_LINUX64
	#else
		#define OS_LINUX32
	#endif
#elif __unix
	// UNIX
	#define OS_UNIX 1
#elif __posix
	// POSIX
	#define OS_POSIX 1
#else
	#error unknown operating system
#endif

// compiler detect
#if defined(__clang__)
	// clang
	#define COMPILER "clang"
	#define COMPILER_CLANG
	#define COMPILER_VERSION __clang_version__
#elif defined(__GNUC__)
	// gcc
	#define COMPILER "gcc"
	#define COMPILER_GCC
	#define COMPILER_VERSION __VERSION__
#elif defined(_MSC_VER)
	// Microsoft Visual C/C++
	#define COMPILER "Microsoft Visual C/C++"
	#define COMPILER_MSC
	#define COMPILER_VC
	#define COMPILER_VERSION #_MSC_VER
#else
	#warning unknown compiler
#endif

#endif
