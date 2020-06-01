#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

char *endwith(const char *pszStr, const char *pszEnd)
{
    if (pszStr == NULL || pszEnd == NULL ||
        strlen(pszStr) < strlen(pszEnd))
    {
        return NULL;
    }
    char *pszTmp = (char *)(pszStr + (strlen(pszStr) - strlen(pszEnd)));
    return strstr(pszTmp, pszEnd);
}

void rm_zip_file()
{
    char szTmpCmd[256] = {0};
    // delete old zip files
    memcpy(szTmpCmd, "rm -f ./logDir/\*_txn.zip", sizeof("rm -f *_txn.zip"));
    // FILE *fp = popen(szTmpCmd, "r");
    // if (fp)
    // {
    //     pclose(fp);
    // }
    system("rm -f ./logDir/*_txn.zip");

}


void create_new_log_files(const char* pfile)
{
    char szFilePath[1024] = {0};
    char szTmp[256] = {0};
    strncpy(szTmp, pfile, strlen(pfile)+1);
    if (endwith(szTmp, "_txn.zip"))
    {
        char szTmpCmd[256] = {0};
        // delete old zip files
        system("rm -f ./logDir/*_txn.zip");
        
        //package new zip log file
        memcpy(szTmp+strlen(szTmp)-3, "log", strlen("log")+1);
        printf("tmp file name: %s\n", szTmp);
        memset(szTmpCmd, 0x00, sizeof(szTmpCmd));
        snprintf(szTmpCmd, sizeof(szTmpCmd), "zip -qrj ./logDir/%s ./logDir/%s", pfile, szTmp);
        //printf("cmd: %s\n", szTmpCmd);
        FILE* fp = popen(szTmpCmd, "r");
        if (fp)
        {
            //printf("popen success\n");
            pclose(fp);
            struct stat st;
            memset(&st, 0x00, sizeof(st));
            if (-1 != stat(szFilePath, &st))
            {
                printf("Log File Tatol Size: %ld\n", st.st_size);
            }
            snprintf(szFilePath, sizeof(szFilePath), "./logDir/%s", pfile);
        }
        else
        {
            printf("create log zip file fail");
            memset(szFilePath, 0x00, sizeof(szFilePath));
        }
        printf("file Path: %s\n", szFilePath);
    }
}

int plain_test() {
  const char* cmd = "zip -ur cc.zip *.cc";
  FILE* fp = popen(cmd, "w");
  if (!fp) {
    perror("popen");
    exit(1);
  }
  pclose(fp);

  const char* cmd_ls = "ls -la *zip";
  (void)popen(cmd_ls, "r");

  return 0;
}


int main()
{
    //rm_zip_file();
    const char* pfile = "20180530_txn.zip";
    create_new_log_files(pfile);

    return 0;
    
}