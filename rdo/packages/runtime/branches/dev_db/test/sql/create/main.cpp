/*!
  \copyright (c) RDO-Team, 2012
  \file      main.cpp
  \authors   Клеванец Игорь (cerevra@gmail.com)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      05.11.2012
  \brief     Тест создания структры базы данных
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <iostream>
#include <conio.h>
#include <QtSql\QtSql>
#include <QtCore\QCoreApplication>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator\runtime\test\sql\include\general_db.h"
// --------------------------------------------------------------------------------

void dropDB()
{
	generalDB db("localhost", "postgres", "postgres", "rdo", 5432);
	db.queryExec("DROP DATABASE IF EXISTS rdo;");
	db.queryExec("CREATE DATABASE rdo;");
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	dropDB();

	generalDB db;

	QSqlQuery query;
//------------------------
	QString create_rtp = "CREATE TABLE rtp"
						"("
						"	r_t_id               serial,"
						"	r_t_name             VARCHAR(40) NOT NULL,"
						"	r_t_perm             boolean NOT NULL,"
						"	PRIMARY KEY (r_t_id)"
						");";
	db.queryExec(create_rtp);

	QString create_list = "CREATE TABLE list_of_types_of_params"
						"("
						"	type_id       integer,"
						"	table_id      integer NOT NULL,"
						"	PRIMARY KEY (type_id)"
						");";
	db.queryExec(create_list);

	QString create_param = "CREATE TABLE param_of_type"
						"("
						"	param_id      serial,"
						"	param_name    VARCHAR(40) NOT NULL,"
						"	r_t_id        integer,"
						"	type_id       integer NOT NULL,"
						"	PRIMARY KEY (param_id,r_t_id),"
						"	FOREIGN KEY (r_t_id) REFERENCES rtp,"
						"	FOREIGN KEY (type_id) REFERENCES list_of_types_of_params"
						");";
	db.queryExec(create_param);
//------------------------

//------------------------
	QString create_seq = "CREATE SEQUENCE type_of_param_seq;";
	db.queryExec(create_seq);

	QString create_function = "CREATE FUNCTION copy_type_id() RETURNS TRIGGER AS $trig$"
							"	BEGIN"
							"		INSERT INTO list_of_types_of_params VALUES"
							"		(NEW.type_id, NEW.tableoid);"
							"		RETURN NULL;"
							"	END;"
							"	$trig$ LANGUAGE plpgsql;";
	db.queryExec(create_function);
//------------------------





//------------------------
	QString create_real = "CREATE TABLE real"
						"("
						"	type_id      integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
						"	def_val      real,"
						"	min          real,"
						"	max          real,"
						"	PRIMARY KEY (type_id)"
						");";
	db.queryExec(create_real);

	QString create_realTr = "CREATE TRIGGER real_trig"
						"	AFTER INSERT ON real"
						"	FOR EACH ROW"
						"	EXECUTE PROCEDURE copy_type_id();";
	db.queryExec(create_realTr);
//------------------------

//------------------------
	QString create_enum = "CREATE TABLE enum"
						"("
						"	type_id      integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
						"	def_val      VARCHAR(40),"
						"	PRIMARY KEY (type_id)"
						");";
	db.queryExec(create_enum);

	QString create_enumTr = "CREATE TRIGGER enum_trig"
						"	AFTER INSERT ON enum"
						"	FOR EACH ROW"
						"	EXECUTE PROCEDURE copy_type_id();";
	db.queryExec(create_enumTr);

	QString create_enumVv = "CREATE TABLE enum_valid_value"
						"("
						"	enum_id              integer,"
						"	vv_id                serial,"
						"	vv_str               VARCHAR(40) NOT NULL,"
						"	vv_int               integer NOT NULL,"
						"	PRIMARY KEY (enum_id, vv_id),"
						"	FOREIGN KEY (enum_id) REFERENCES enum"
						");";
	db.queryExec(create_enumVv);
//------------------------

//------------------------
	QString create_int = "CREATE TABLE int"
						"("
						"	type_id      integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
						"	def_val      integer,"
						"	min          integer,"
						"	max          integer,"
						"	PRIMARY KEY (type_id)"
						");";
	db.queryExec(create_int);

	QString create_intTr = "CREATE TRIGGER int_trig"
						"	AFTER INSERT ON int"
						"	FOR EACH ROW"
						"	EXECUTE PROCEDURE copy_type_id();";
	db.queryExec(create_intTr);
//------------------------

	std::cout << "to be continued...\n";
	getch();
	return 0;
}
