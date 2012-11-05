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
#include "simulator\runtime\test\sql\include\connectDB.h"
// --------------------------------------------------------------------------------

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
	db = connectDB(db, "postgres");

	QSqlQuery query;
	QString create_dropDb = "DROP DATABASE IF EXISTS rdo;";
	QString create_db     = "CREATE DATABASE rdo;";

	bool dbB_drop = query.exec(create_dropDb);
	bool cr_dbB   = query.exec(create_db);

	if (!(dbB_drop*cr_dbB))
		std::cout << "Creation of datebase failed! :(" << std::endl;

	db = connectDB(db);

//------------------------
	QString create_rtp = "CREATE TABLE rtp"
						"("
						"	r_t_id               serial,"
						"	r_t_name             VARCHAR(40) NOT NULL,"
						"	r_t_perm             boolean NOT NULL,"
						"	PRIMARY KEY (r_t_id)"
						");";
	bool rtp = query.exec(create_rtp);

	QString create_list = "CREATE TABLE list_of_types_of_params"
						"("
						"	type_id       integer,"
						"	table_id      integer NOT NULL,"
						"	PRIMARY KEY (type_id)"
						");";
	bool list = query.exec(create_list);

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
	bool param = query.exec(create_param);
//------------------------

//------------------------
	QString create_seq = "CREATE SEQUENCE type_of_param_seq;";
	bool seq = query.exec(create_seq);

	QString create_function = "CREATE FUNCTION copy_type_id() RETURNS TRIGGER AS $trig$"
							"	BEGIN"
							"		INSERT INTO list_of_types_of_params VALUES"
							"		(NEW.type_id, NEW.tableoid);"
							"		RETURN NULL;"
							"	END;"
							"	$trig$ LANGUAGE plpgsql;";
	bool function = query.exec(create_function);
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
	bool real = query.exec(create_real);

	QString create_realTr = "CREATE TRIGGER real_trig"
						"	AFTER INSERT ON real"
						"	FOR EACH ROW"
						"	EXECUTE PROCEDURE copy_type_id();";
	bool realTr = query.exec(create_realTr);
//------------------------

//------------------------
	QString create_enum = "CREATE TABLE enum"
						"("
						"	type_id      integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
						"	def_val      VARCHAR(40),"
						"	PRIMARY KEY (type_id)"
						");";
	bool enumB = query.exec(create_enum);

	QString create_enumTr = "CREATE TRIGGER enum_trig"
						"	AFTER INSERT ON enum"
						"	FOR EACH ROW"
						"	EXECUTE PROCEDURE copy_type_id();";
	bool enumTr = query.exec(create_enumTr);

	QString create_enumVv = "CREATE TABLE enum_valid_value"
						"("
						"	enum_id              integer,"
						"	vv_id                serial,"
						"	vv_str               VARCHAR(40) NOT NULL,"
						"	vv_int               integer NOT NULL,"
						"	PRIMARY KEY (enum_id, vv_id),"
						"	FOREIGN KEY (enum_id) REFERENCES enum"
						");";
	bool enumVv = query.exec(create_enumVv);
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
	bool intB = query.exec(create_int);

	QString create_intTr = "CREATE TRIGGER int_trig"
						"	AFTER INSERT ON int"
						"	FOR EACH ROW"
						"	EXECUTE PROCEDURE copy_type_id();";
	bool intTr = query.exec(create_intTr);
//------------------------


	if (!(rtp*seq*list*param*real*function*realTr*enumB*enumTr*enumVv*intB*intTr))
		std::cout << "Creation of structure of datebase failed! :(" << std::endl;

	db.close();
	std::cout << "to be continued...\n";
//	getch();
	return 0;
}
