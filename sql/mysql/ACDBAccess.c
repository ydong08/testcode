

#include <string.h>
#include <stdarg.h>
#include "ACDBAccess.h"
#include <m_string.h>


#define MYSQL_MAX_SQL_LEN        (512)

#define SQL_SELECT_CONTROLLER_NETWORK  \
"select * from ac_controller_network where controller_mac='%s';"

#define SQL_SELECT_CONTROLLER_RADIO    \
"select * from ac_controller_radio where"  \
" controller_mac='%s' AND radio_type=%d AND radio_index=%d;"

#define SQL_SELECT_CONTROLLER_ROAM     \
"select * from ac_controller_roam where controller_mac='%s';"

#define SQL_SELECT_COMMON_INFO         \
 "select * from ac_common_info where ac_mac='%s';"


#if 1
#ifndef strmov
char *strmov(register char *dst, register const char *src)
{
  while ((*dst++ = *src++)) ;
  return dst-1;
}
#endif
#endif

/*
 * connet mysql database
 */
int ACDBConnect(const char *host , short port, const char *user, 
			const char *passwd, const char *db_name)
{
	if (host == NULL || user == NULL || passwd == NULL)
	{
		printf("[Error]invalid parameter, connect mysql fail.\n");
		return MYSQL_RETURN_ERROR;
	}

	if (db_name == NULL)
	{
		printf("[Warn]invalid db name, will use default db.\n");
	}

	if (gMysql)
	{
		printf("[Info]mysql already initialized.\n");
		return MYSQL_RETURN_SUCCESS;
	}

	/* in multithreaded environment, 
	 * library init must be called first to avoid thread-safe issue 
	 */
	if (mysql_library_init(0, NULL, NULL))
	{
		printf("[Error]could not initialize mysql client library.\n");
		return MYSQL_RETURN_ERROR;
	}

	gMysql = mysql_init(gMysql);
	if (!gMysql)
	{
		printf("[Error]mysql init fail:insufficient memory.\n");
		return MYSQL_RETURN_ERROR;
	}

	//mysql_options(gMysql, MYSQL_READ_DEFAULT_GROUP, "acmeshcli");
	//mysql_options(gMysql, MYSQL_READ_DEFAULT_FILE, "my.cnf");

	mysql_options(gMysql, MYSQL_READ_DEFAULT_GROUP, "acmeshcli");

	if (!mysql_real_connect(gMysql, host, user, passwd, db_name, 
						port, NULL, 0/*CLIENT_MULTI_STATEMENTS*/))
	{
		printf("[Error]connect mysql fail:%s.\n", mysql_error(gMysql));
		return MYSQL_RETURN_ERROR;
	}

	return MYSQL_RETURN_SUCCESS;
}

/*
 * disconnect database
 */
int ACDBDisconnect(void)
{
	if (gMysql)
	{
		mysql_close(gMysql);
		printf("[Info]close mysql ok.\n");

		mysql_library_end();
		printf("[Info]release mysql library ok.\n");
	}

	return MYSQL_RETURN_SUCCESS;
}

/*
 * use database
 */
int ACDBUseDb(const char *db_name)
{
	if (!db_name)
	{
		printf("[Error]invalid db name.\n");
		return MYSQL_RETURN_ERROR;
	}

	if (!gMysql)
	{
		printf("[Error]mysql client not yet initialized.\n");
		return MYSQL_RETURN_ERROR;
	}

	if (!mysql_select_db(gMysql, db_name))
	{
		printf("[Error]use %s databases fail:%s.\n", 
			db_name, mysql_error(gMysql));
		return MYSQL_RETURN_ERROR;
	}

	printf("[Info]use %s databases ok.\n", db_name);
	return MYSQL_RETURN_SUCCESS;
}


/*
 * sql execute
 */
int ACDBExecSql(const char *sql, MYSQL_RES** res)
{
	MYSQL_RES*   result        = NULL; // store result set
	char         *end          = NULL;
	unsigned int fields_num    = 0;    //the number of the filed
    my_ulonglong rows_num      = 0;    // the number of the row
    int          status        = 0;
	MYSQL_ROW    row;
	char         query[MYSQL_MAX_SQL_LEN] = {0};

	*res = NULL;
	if (sql == NULL)
	{
		printf("[Error]invalid sql statement.\n");
		return MYSQL_RETURN_ERROR;
	}

	if (gMysql == NULL)
	{
		printf("[Error]mysql database is disconnect.\n");
		return MYSQL_RETURN_ERROR;
	}

	printf("[Debug]SQL:%s\n", sql);

	end = strmov(query, sql);
	end += mysql_real_escape_string(gMysql, end, ";", 1);

	status = mysql_real_query(gMysql, query, (unsigned int)(end - query));
	//status = mysql_real_query(gMysql, sql, strlen(sql));
	if (status != 0)
	{
		printf("[Error]execute %s fail:%s.\n", 
			sql, mysql_error(gMysql));
		return MYSQL_RETURN_ERROR;
	}

	// query succeeded,process any data returned by it
	status = mysql_field_count(gMysql);
	if(status == 0)
	{
		rows_num =  mysql_affected_rows(gMysql);
		printf("[Debug]Operation affected rows %d\n.",rows_num);
		return MYSQL_RETURN_SUCCESS;
	}


	result = mysql_store_result(gMysql);
	if (result == NULL) // no result or error
	{
		printf("[Error] mysql_store_result() fail:%s.\n", mysql_error(gMysql));
		return MYSQL_RETURN_ERROR;
	}
	
	// there are rows
	fields_num = mysql_num_fields(result);
	rows_num = mysql_num_rows(result);

	printf("[Info]current SQL get row(%d), field(%d).\n", rows_num, fields_num);
	*res = result;
	return MYSQL_RETURN_SUCCESS;
}

#if 0
/*
 * sql execute
 */
int ACDBExecSql2Row(const char *sql, RowCallback rowcb, void* result)
{
	MYSQL_RES*   res           = NULL; // store result set
	char         *end          = NULL;
	unsigned int fields_num    = 0;    //the number of the filed
    my_ulonglong rows_num      = 0;    // the number of the row
    int          status        = 0;
	MYSQL_ROW    row;
	char         query[MYSQL_MAX_SQL_LEN] = {0};

	if (sql == NULL)
	{
		printf("[Error]invalid sql statement.\n");
		return MYSQL_RETURN_ERROR;
	}

	printf("[Debug]SQL:(%s)\n", sql);

	end = strmov(query, sql);
	end = mysql_real_escape_string(gMysql, end, "", strlen(""));

	status = mysql_real_query(gMysql, query, (unsigned int)(end - query));
	if (status != 0)
	{
		printf("[Error]database query (%s) fail:%s.\n", 
			sql, mysql_error(gMysql));
		return MYSQL_RETURN_ERROR;
	}

	// query succeeded,process any data returned by it
	status = mysql_field_count(gMysql);
	if(status == 0)
	{
		rows_num =  mysql_affected_rows(gMysql);
		printf("[Debug]Operation affected rows %d\n.",rows_num);
		return MYSQL_RETURN_SUCCESS;
	}

	do
	{
		res = mysql_store_result(gMysql);
		if (res == NULL) // no result or error
		{
			printf("[Error] mysql_store_result() fail:%s.\n", mysql_error(gMysql));
			return MYSQL_RETURN_ERROR;
		}
		
		// there are rows
		fields_num = mysql_num_fields(res);
		rows_num = mysql_num_rows(res);

		while((row = mysql_fetch_row(res)) != NULL)
		{
			if (rowcb)
			{
				rowcb(result, row, fields_num);
			}

		}
		mysql_free_result(res);

		/* 0: Successful and there are more results
		 * -1: Successful and there are no more results
		 * >0: An error occurred
		 */
		status = mysql_next_result(gMysql);
		if (status == -1)
		{
			printf("[Info]deal all");
		}
		else if (0 < status)
		{
			printf("[Error]get query statement result fail:%s.\n", 
				mysql_error(gMysql));
			return MYSQL_RETURN_ERROR;
		}

	} while(status == 0);

	return MYSQL_RETURN_SUCCESS;
}
#endif


#if 0
/*
 * sql execute
 */
int ACDBExecSql2Res(const char *sql, ResCallback rescb, void* result)
{
	MYSQL_RES    *res          = NULL; // store result set
	char         *end          = NULL;
	unsigned int fields_num    = 0;//the number of the filed
    my_ulonglong rows_num      = 0;// the number of the row
    int          status        = 0;
	MYSQL_ROW    row;
	char         query[MYSQL_MAX_SQL_LEN] = {0};

	if (sql == NULL)
	{
		printf("[Error]invalid sql statement.\n");
		return MYSQL_RETURN_ERROR;
	}

	printf("[Debug]SQL:(%s)\n", sql);

	end = strmov(query, sql);
	end = mysql_real_escape_string(gMysql, end, "", strlen(""));

	status = mysql_real_query(gMysql, query, (unsigned int)(end - query));
	if (status != 0)
	{
		printf("[Error]database query (%s) fail:%s.\n", 
			sql, mysql_error(gMysql));
		return MYSQL_RETURN_ERROR;
	}

	// query succeeded,process any data returned by it
	status = mysql_field_count(gMysql);
	if(status == 0)
	{
		rows_num =  mysql_affected_rows(gMysql);
		printf("[Debug]Operation affected rows %d\n.",rows_num);
		return MYSQL_RETURN_SUCCESS;
	}

	do
	{
		res = mysql_store_result(gMysql);
		if (res == NULL) // no result or error
		{
			printf("[Error] mysql_store_result() fail:%s.\n", 
				mysql_error(gMysql));
			return MYSQL_RETURN_ERROR;
		}
		
		// there are rows
		fields_num = mysql_num_fields(res);
		rows_num = mysql_num_rows(res);
		
		if (rescb)
		{
			rescb(result, res, fields_num, rows_num);
		}

		mysql_free_result(res);

		/* 0: Successful and there are more results
		 * -1: Successful and there are no more results
		 * >0: An error occurred
		 */
		status = mysql_next_result(gMysql);
		if (status == -1)
		{
			printf("[Info]deal all result.\n");
		}
		else if (0 < status)
		{
			printf("[Error]get query statement result fail:%s.\n", 
				mysql_error(gMysql));
			return MYSQL_RETURN_ERROR;
		}
	} while(status == 0);

	return MYSQL_RETURN_SUCCESS;
}
#endif

#if 0
int hasNextRes(void)
{
	int status        = 0;

	/* 0: Successful and there are more results
	 * -1: Successful and there are no more results
	 * >0: An error occurred
	 */
	status = mysql_next_result(gMysql);
	if (status == -1)
	{
		printf("[Info]deal all result.\n");
		return 0;

	}
	else if (0 < status)
	{
		printf("[Error]get query statement result fail:%s.\n", 
			mysql_error(gMysql));
		return 0;
	}
	else if (status == 0)
	{
		printf("[Info]next result exist.\n");
		return 1;
	}
}
#endif

/* */
int ACDBGetFieldValue(MYSQL_RES** res, const char* fieldname, char* value)
{
	MYSQL_FIELD   *field     = NULL;
	unsigned long *lengths   = NULL;
	MYSQL_ROW     row;
	unsigned int i           = 0;
	unsigned int fields_num  = 0;
	char         buf[MYSQL_MAX_SQL_LEN] = {0};  
	
	if (res == NULL || fieldname == NULL || value == NULL)
	{
		printf("[Error]invalid parameter.\n");
		return MYSQL_RETURN_ERROR;
	}

	fields_num = mysql_num_fields(*res);
	field = mysql_fetch_fields(*res);

	while((row = mysql_fetch_row(*res)) != NULL)
	{
		lengths = mysql_fetch_lengths(*res);
		for (i = 0; i < fields_num; ++i)
		{
			if (0 == strncmp(fieldname, field[i].name, strlen(fieldname)))
			{
				memcpy(value, row[i], lengths[i]);
				return MYSQL_RETURN_SUCCESS;
			}
		}
	}
	return MYSQL_RETURN_ERROR;
}


int ACDBFreeMysqlRes(MYSQL_RES **res)
{
	if (res != NULL)
	{
		mysql_free_result(*res);
	}

	return MYSQL_RETURN_SUCCESS;
}


int ACDBExecStatement(MYSQL_RES **res, const char* format, ...)
{
	int  ret                    = 0;
	char sql[MYSQL_MAX_SQL_LEN] = {0};  
	
	if (format == NULL || res == NULL)
	{
		printf("[Warn]invalid parameter.\n");
		return MYSQL_RETURN_ERROR;
	}

	va_list args;
	va_start(args, format);
	vsnprintf(sql, sizeof(sql), format, args);
	va_end(args);

	ret == ACDBExecSql(sql, res);
	if (ret == MYSQL_RETURN_ERROR)
	{
		if (*res)
			ACDBFreeMysqlRes(res);

		return MYSQL_RETURN_ERROR;
	}

	return MYSQL_RETURN_SUCCESS;
}

int get_info_controller_network(MYSQL_RES **res, const char *controller_mac)
{
	int  ret                    = 0;
	char sql[MYSQL_MAX_SQL_LEN] = {0};  
	
	if (controller_mac == NULL || res == NULL)
	{
		printf("[Warn]invalid parameter.\n");
		return MYSQL_RETURN_ERROR;
	}

	snprintf(sql, sizeof(sql), SQL_SELECT_CONTROLLER_NETWORK, controller_mac);
	ret == ACDBExecSql(sql, res);
	if (ret == MYSQL_RETURN_ERROR)
	{
		if (*res)
			ACDBFreeMysqlRes(res);

		return MYSQL_RETURN_ERROR;
	}

	return MYSQL_RETURN_SUCCESS;
}

int get_info_controller_radio(MYSQL_RES **res, const char *controller_mac, int radio_type, int radio_index)
{
	int  ret                    = 0;
	char sql[MYSQL_MAX_SQL_LEN] = {0};  
	
	if (controller_mac == NULL || res == NULL)
	{
		printf("[Warn]invalid parameter.\n");
		return MYSQL_RETURN_ERROR;
	}

	snprintf(sql, sizeof(sql), SQL_SELECT_CONTROLLER_RADIO, 
		controller_mac, radio_type, radio_index);

	ret == ACDBExecSql(sql, res);
	if (ret == MYSQL_RETURN_ERROR)
	{
		if (*res)
			ACDBFreeMysqlRes(res);
		
		return MYSQL_RETURN_ERROR;
	}

	return MYSQL_RETURN_SUCCESS;

}

int get_info_controller_roam(MYSQL_RES **res, const char *controller_mac)
{
	int  ret                    = 0;
	char sql[MYSQL_MAX_SQL_LEN] = {0};  
	
	if (controller_mac == NULL || res == NULL)
	{
		printf("[Warn]invalid parameter.\n");
		return MYSQL_RETURN_ERROR;
	}

	snprintf(sql, sizeof(sql), SQL_SELECT_CONTROLLER_ROAM, controller_mac);
	ret == ACDBExecSql(sql, res);
	if (ret == MYSQL_RETURN_ERROR)
	{
		if (*res)
			ACDBFreeMysqlRes(res);

		return MYSQL_RETURN_ERROR;
	}

	return MYSQL_RETURN_SUCCESS;

}

int get_info_common_info(MYSQL_RES **res, const char *ac_mac)
{
	int  ret                    = 0;
	char sql[MYSQL_MAX_SQL_LEN] = {0};  
	
	if (ac_mac == NULL || res == NULL)
	{
		printf("[Warn]invalid parameter.\n");
		return MYSQL_RETURN_ERROR;
	}

	snprintf(sql, sizeof(sql), SQL_SELECT_COMMON_INFO, ac_mac);
	ret == ACDBExecSql(sql, res);
	if (ret == MYSQL_RETURN_ERROR)
	{
		if (*res)
			ACDBFreeMysqlRes(res);

		return MYSQL_RETURN_ERROR;
	}

	return MYSQL_RETURN_SUCCESS;
}

