/*!
  \copyright (c) RDO-Team, 2013
  \file      init_struct_db.cpp
  \authors   Клеванец Игорь (cerevra@gmail.com)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      25.01.2013
  \brief     Создание структуры БД
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator\compiler\parser\src\db\init_struct_db.h"
// --------------------------------------------------------------------------------

InitSructDB::InitSructDB()
{
	GeneralDB dbTemp("localhost", "postgres", "postgres", "rdo", 5432);
	dbTemp.queryExec("DROP DATABASE IF EXISTS rdo;");
	dbTemp.queryExec("CREATE DATABASE rdo;");

	GeneralDB db;
	db.queryExec(generateCreateDBQuery());
}

GeneralDB::QueryList InitSructDB::generateCreateDBQuery()
{
	GeneralDB::QueryList queryList;

	queryList.push_back(
		"CREATE TABLE rtp("
		"id       serial,"
		"rtp_name VARCHAR(40) NOT NULL,"
		"rtp_perm boolean NOT NULL,"
		"PRIMARY KEY (id)"
		");");

	queryList.push_back(
		"CREATE TABLE list_of_types_of_params("
		"type_id  integer,"
		"table_id integer NOT NULL,"
		"PRIMARY KEY (type_id)"
		");");

	queryList.push_back(
		"CREATE TABLE param_of_type("
		"id          serial,"
		"param_name  VARCHAR(40) NOT NULL,"
		"rtp_id      integer,"
		"type_id     integer NOT NULL,"
		"PRIMARY KEY (id,rtp_id),"
		"FOREIGN KEY (rtp_id) REFERENCES rtp(id),"
		"FOREIGN KEY (type_id) REFERENCES list_of_types_of_params"
		");");
//------------------------

//------------------------
	queryList.push_back("CREATE SEQUENCE type_of_param_seq;");

	queryList.push_back(
		"CREATE FUNCTION copy_type_id() RETURNS TRIGGER AS $trig$ "
		"BEGIN "
		"INSERT INTO list_of_types_of_params VALUES "
		"(NEW.id, NEW.tableoid); "
		"RETURN NULL; "
		"END; "
		"$trig$ LANGUAGE plpgsql; ");
//------------------------





//------------------------
	queryList.push_back(
		"CREATE TABLE real("
		"id          integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
		"def_val     real,"
		"min         real,"
		"max         real,"
		"PRIMARY KEY (id)"
		");");

	queryList.push_back(
		"CREATE TRIGGER real_trig "
		"AFTER INSERT ON real "
		"FOR EACH ROW "
		"EXECUTE PROCEDURE copy_type_id();");
//------------------------

//------------------------
	queryList.push_back(
		"CREATE TABLE enum("
		"id          integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
		"def_val     VARCHAR(40),"
		"PRIMARY KEY (id)"
		");");

	queryList.push_back(
		"CREATE TRIGGER enum_trig "
		"AFTER INSERT ON enum "
		"FOR EACH ROW "
		"EXECUTE PROCEDURE copy_type_id();");

	queryList.push_back(
		"CREATE TABLE enum_valid_value("
		"enum_id     integer,"
		"vv_id       serial,"
		"vv_str      VARCHAR(40) NOT NULL,"
		"vv_int      integer NOT NULL,"
		"PRIMARY KEY (enum_id, vv_id),"
		"FOREIGN KEY (enum_id) REFERENCES enum"
		");");
//------------------------

//------------------------
	queryList.push_back(
		"CREATE TABLE int("
		"id          integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
		"def_val     integer,"
		"min         integer,"
		"max         integer,"
		"PRIMARY KEY (id)"
		");");

	queryList.push_back(
		"CREATE TRIGGER int_trig "
		"AFTER INSERT ON int "
		"FOR EACH ROW "
		"EXECUTE PROCEDURE copy_type_id();");
//------------------------

//------------------------
	queryList.push_back(
		"CREATE TABLE identificator("
		"id      integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
		"PRIMARY KEY (id)"
		");");

	queryList.push_back(
		"CREATE TRIGGER identificator_trig "
		"AFTER INSERT ON identificator "
		"FOR EACH ROW "
		"EXECUTE PROCEDURE copy_type_id();");
//------------------------

//------------------------
	queryList.push_back(
		"CREATE TABLE bool("
		"id      integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
		"def_val boolean,"
		"PRIMARY KEY (id)"
		");");

	queryList.push_back(
		"CREATE TRIGGER bool_trig "
		"AFTER INSERT ON bool "
		"FOR EACH ROW "
		"EXECUTE PROCEDURE copy_type_id();");
//------------------------

//------------------------
	queryList.push_back(
		"CREATE TABLE string("
		"id      integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
		"def_val VARCHAR(40) NOT NULL,"
		"PRIMARY KEY (id)"
		");");

	queryList.push_back(
		"CREATE TRIGGER string_trig "
		"AFTER INSERT ON string "
		"FOR EACH ROW "
		"EXECUTE PROCEDURE copy_type_id();");
//------------------------

//------------------------
	queryList.push_back(
		"CREATE TABLE void("
		"id      integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
		"PRIMARY KEY (id)"
		");");

	queryList.push_back(
		"CREATE TRIGGER void_trig "
		"AFTER INSERT ON void "
		"FOR EACH ROW "
		"EXECUTE PROCEDURE copy_type_id();");


	return queryList;
}

