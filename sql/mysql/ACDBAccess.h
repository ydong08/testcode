


#ifndef ACMySqlAccess_H
#define ACMySqlAccess_H

#include <stdio.h>
#include <stdlib.h>
#include "mysql.h"
#include "errmsg.h"

#define MYSQL_RETURN_ERROR       (1)
#define MYSQL_RETURN_SUCCESS     (0)

static MYSQL *gMysql;  

int ACDBConnect(const char *host , short port, const char *user, 
			const char *passwd, const char *db_name);
int ACDBDisconnect(void);

int ACDBUseDb(const char *db_name);
int ACDBExecSql(const char *sql, MYSQL_RES** res);

/* get field value by field name */
int ACDBGetFieldValue(MYSQL_RES **res, const char *fieldname, char *value);

/* this api must be called after get field info */
int ACDBFreeMysqlRes(MYSQL_RES **res);

int ACDBExecStatement(MYSQL_RES **res, const char* format, ...);
int get_info_controller_network(MYSQL_RES **res, const char *controller_mac);
int get_info_controller_radio(MYSQL_RES **res, const char *controller_mac, int radio_type, int radio_index);
int get_info_controller_roam(MYSQL_RES **res, const char *controller_mac);
int get_info_common_info(MYSQL_RES **res, const char *ac_mac);

#endif

