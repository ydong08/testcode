#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>




int check_mac_format(const char *mac_arg)
{
	int z;
	char mac[24]={0};
	char * pattern;
	regex_t reg;
	regmatch_t pm[10];
	const size_t nmatch = 10;

	strcpy(mac, mac_arg);
	fprintf(stderr,"mac=%s\n", mac);

	pattern ="([0-9a-fA-F]{2}:){5}[0-9a-fA-F]{2}";
	z = regcomp(&reg, pattern, REG_EXTENDED);

	if (z != 0){
		fprintf(stderr, "Failed to open regcomp.\n");
		regfree(&reg);
		return 0;
	}

	z = regexec(&reg, mac, nmatch, pm, 0);
	if (z == REG_NOMATCH) {
		fprintf(stderr, "MAC format is wrong.\n");
		regfree(&reg);
		return 0;
	}

	regfree(&reg);

	return 1;
}



int main()
{
	const char *mac = "44-44-44-44-44-66";
	if (!check_mac_format(mac))
		fprintf(stderr, "check mac fail.\n");
	return 0;
}
