#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>

/* 进程的最初环境变量来自系统变量 
 * 进程内可以通过setenv()设置进程私有的环境变量，
 * 该环境变量可以被其子进程继承 */
extern char **environ;

/*
 struct passwd {
    char *pw_name; // Username
    char *pw_passwd; // Password
    __uid_t pw_uid; // User ID. 
    __gid_t pw_gid; // Group ID.
    char *pw_gecos; // Real name. 
    char *pw_dir; // Home directory 
    char *pw_shell; // Shell program
};
*/
void print_pw(struct passwd *pw)
{
	if (!pw)
	{
		printf("pw info point is null.\n");
		return ;		
	}
	
	printf("pw->pw_name = %s\n", pw->pw_name);
    printf("pw->pw_passwd = %s\n", pw->pw_passwd);
    printf("pw->pw_uid = %d\n", pw->pw_uid);
    printf("pw->pw_gid = %d\n", pw->pw_gid);
    printf("pw->pw_gecos = %s\n", pw->pw_gecos);
    printf("pw->pw_dir = %s\n", pw->pw_dir);
    printf("pw->pw_shell = %s\n", pw->pw_shell);
    
    return ;
}

int main()
{
	char *buf = NULL;
	int i;
	int rc = -1;
	ssize_t buflen = 0;
	char user[12] = "winter";
	uid_t uid = 0;
	struct passwd pwd, *pwd_p;
	for (i = 0; environ[i]; ++i)
	{
		printf("environ[%d]: %s.\n", i, environ[i]);
	}
	
	printf("\n\n************************\n");
	uid = getuid();
	
	do
	{
		if (!buflen)
		{
			buflen = sysconf(_SC_GETPW_R_SIZE_MAX);	
			if (-1 == buflen)
				buflen = 2048;	
		} 
		else 
		{
			buflen = +2048;	
		}
		
		buf = realloc(buf, buflen);
		if (!buf)
		{
			perror("realloc");
			return -1;
		}
			
		/* user -> UID */
        rc = getpwnam_r(user, &pwd, buf, buflen, &pwd_p);
        if (!rc)
        {
        	printf("getpwnam_r result:\n");
        	print_pw(pwd_p);	
        }
    
        /* UID -> user */
        rc = getpwuid_r(uid, &pwd, buf, buflen, &pwd_p);
        if (!rc)
        {
        	printf("getpwuid_r result:\n");
        	print_pw(pwd_p);	
        }
		
	} while(rc && rc == ERANGE);

	return 0;
}
