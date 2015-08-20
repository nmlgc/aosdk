//
// Audio Overload SDK
//
// Portability wrappers.
//

#ifdef WIN32
#include <Windows.h>
#else
#include <sys/stat.h>
#endif

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
