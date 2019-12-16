#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ifaddrs.h>


int main() {
#if defined(MACRO_IP)
	struct ifaddrs *ifas, *ifp;
#  if defined(MACRO_IFS)
	int ret = getifaddrs(&ifas);
	if (ret < 0) {
		printf("getifaddrs error[%d]\n", errno);
		return 0;
	}
	for (ifp = ifas; ifp != NULL; ifp = ifp->ifa_next) {
		printf("\t%s\n", ifp->ifa_name);
	}
#  endif
	freeifaddrs(ifas);
#endif
	return 1;
}
