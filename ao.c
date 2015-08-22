//
// Audio Overload SDK
//
// Portability wrappers.
//

#ifdef WIN32
#include "win32_utf8/src/win32_utf8.h"
#else
#include <stdio.h>
#include <sys/stat.h>
#endif

FILE* ao_fopen(const char *fn, const char *mode)
{
	return fopen(fn, mode);
}

int ao_mkdir(const char *dirname)
{
#ifdef WIN32
	// Let's avoid the entire problem of certain runtimes omitting the
	// mode parameter altogether by just using the kernel function.
	return CreateDirectoryA(dirname, NULL);
#else
	return mkdir(dirname, 0777);
#endif
}
