#include <cstdlib>
#include <cstdio>
#include <cstring>

#include <string>
#include <mysql.h>

#include "mytap.h"
#include "command_line.h"

int show_variable(MYSQL *mysql, const std::string& var_name, std::string& var_value) {
	char query[128];

	snprintf(query, sizeof(query),"show variables like '%s'", var_name.c_str());
	if (mysql_query(mysql, query)) {
		fprintf(stderr, "Failed to execute SHOW VARIABLES LIKE : no %d, %s\n",
				mysql_errno(mysql), mysql_error(mysql));
		return -1;
	}

	MYSQL_RES *result;
	MYSQL_ROW row;
	result = mysql_store_result(mysql);

	int num_fields = mysql_num_fields(result);

	row = mysql_fetch_row(result);
	var_value = row[1];
	mysql_free_result(result);
}

int main(int argc, char** argv) {
	CommandLine cl;

	if(cl.parse(argc, argv))
		return exit_status();

	plan(12);
	diag("Testing SET CHARACTER SET");

	MYSQL* mysql = mysql_init(NULL);
	if (!mysql)
		return exit_status();
	
	if (!mysql_real_connect(mysql, cl.host, cl.username, cl.password, NULL, cl.port, NULL, 0)) {
	    fprintf(stderr, "Failed to connect to database: Error: %s\n",
	              mysql_error(mysql));
		return exit_status();
	}

	if (mysql_query(mysql, "drop database if exists t1")) {
	    fprintf(stderr, "File %s, line %d, Error: %s\n",
	              __FILE__, __LINE__, mysql_error(mysql));
		return exit_status();
	}

	if (mysql_query(mysql, "create database t1 charset utf8")) {
	    fprintf(stderr, "File %s, line %d, Error: %s\n",
	              __FILE__, __LINE__, mysql_error(mysql));
		return exit_status();
	}

	if (mysql_query(mysql, "use t1")) {
	    fprintf(stderr, "File %s, line %d, Error: %s\n",
	              __FILE__, __LINE__, mysql_error(mysql));
		return exit_status();
	}

	if (mysql_query(mysql, "set names 'utf8'")) {
	    fprintf(stderr, "SET CHARACTER SET 'utf8': Error: %s\n",
	              mysql_error(mysql));
		return exit_status();
	}

	std::string var_charset_client = "character_set_client";
	std::string var_charset_connection = "character_set_connection";
	std::string var_charset_results = "character_set_results";
	std::string var_charset_database = "character_set_database";
	std::string var_value;

	show_variable(mysql, var_charset_client, var_value);
	ok(var_value.compare("utf8") == 0, "Initial client character set");

	show_variable(mysql, var_charset_connection, var_value);
	ok(var_value.compare("utf8") == 0, "Initial connection character set");

	show_variable(mysql, var_charset_results, var_value);
	ok(var_value.compare("utf8") == 0, "Initial results character set");

	show_variable(mysql, var_charset_database, var_value);
	ok(var_value.compare("utf8") == 0, "Initial results character set");

	if (mysql_query(mysql, "set character set latin1")) {
	    fprintf(stderr, "SET CHARACTER SET : Error: %s\n",
	              mysql_error(mysql));
		return exit_status();
	}

	show_variable(mysql, var_charset_client, var_value);
	ok(var_value.compare("latin1") == 0, "Client character set is changed");

	std::string db_charset_value;
	show_variable(mysql, var_charset_database, db_charset_value);

	show_variable(mysql, var_charset_database, var_value);
	ok(var_value.compare("utf8") == 0, "Database character set is not changed");

	show_variable(mysql, var_charset_connection, var_value);
	ok(var_value.compare(db_charset_value) == 0, "Connection character set same as database charset");

	show_variable(mysql, var_charset_results, var_value);
	ok(var_value.compare("latin1") == 0, "Results character set is changed");

	if (mysql_query(mysql, "set names 'latin1'")) {
		fprintf(stderr, "SET CHARACTER SET : Error: %s\n",
				mysql_error(mysql));
		return exit_status();
	}

	show_variable(mysql, var_charset_client, var_value);
	ok(var_value.compare("latin1") == 0, "Client character set is correct");

	show_variable(mysql, var_charset_connection, var_value);
	ok(var_value.compare("latin1") == 0, "Set names changed connection character set");

	show_variable(mysql, var_charset_results, var_value);
	ok(var_value.compare("latin1") == 0, "Results character set is correct");

	show_variable(mysql, var_charset_database, var_value);
	ok(var_value.compare("utf8") == 0, "Database character set is not changed by set names");

	mysql_close(mysql);

	return exit_status();
}

