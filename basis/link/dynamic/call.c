#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main()
{
	const char* dep = "libutil";

	void *handle = dlopen(dep, RTLD_LAZY);
	if (!handle)
	{
		perror("handle");
		return -1;
	}

	sleep(5);

	dlclose(handle);


}
