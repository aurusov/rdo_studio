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
	generateCreateDBQuery();
	queryExec(m_queryList);
}

void InitSructDB::dropDB()
{
	{
		GeneralDB dbTemp("localhost", "postgres", "postgres", "rdo", 5432);
		dbTemp.queryExec("DROP DATABASE IF EXISTS rdo;");
	}
	QSqlDatabase::removeDatabase("qt_sql_default_connection");
}

void InitSructDB::createDB()
{
	{
		GeneralDB dbTemp("localhost", "postgres", "postgres", "rdo", 5432);
		dbTemp.queryExec("CREATE DATABASE rdo;");
	}
	QSqlDatabase::removeDatabase("qt_sql_default_connection");
}

void InitSructDB::rdoValueTable(QString tableName, QString dataType)
{
	m_queryList.push_back(QString(
		"CREATE TABLE %1("
		"id integer NOT NULL DEFAULT nextval('rdo_value_seq'),"
		"vv %2 NOT NULL,"
		"PRIMARY KEY (id)"
		");")
		.arg(tableName)
		.arg(dataType));

	m_queryList.push_back(QString(
		"CREATE TRIGGER %1_trig "
		"AFTER INSERT ON %1 "
		"FOR EACH ROW "
		"EXECUTE PROCEDURE copy_rdo_value_id();")
		.arg(tableName));
}

void InitSructDB::dataTypeTable(QString tableName, QString dataType)
{
	//------------------------
	m_queryList.push_back(QString(
		"CREATE TABLE %1("
		"id      integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
		"def_val %2,"
		"PRIMARY KEY (id)"
		");")
		.arg(tableName)
		.arg(dataType));

	m_queryList.push_back(QString(
		"CREATE TRIGGER %1_trig "
		"AFTER INSERT ON %1 "
		"FOR EACH ROW "
		"EXECUTE PROCEDURE copy_type_id();")
		.arg(tableName));
//------------------------
}

void InitSructDB::generateCreateDBQuery()
{
	m_queryList.push_back(
		"CREATE TABLE rtp("
		"id       serial,"
		"rtp_name VARCHAR(40) NOT NULL,"
		"rtp_perm boolean NOT NULL,"
		"PRIMARY KEY (id)"
		");");

	m_queryList.push_back(
		"CREATE TABLE list_of_types_of_params("
		"type_id  integer,"
		"table_id integer NOT NULL,"
		"PRIMARY KEY (type_id)"
		");");

	m_queryList.push_back(
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
	m_queryList.push_back("CREATE SEQUENCE type_of_param_seq;");

	m_queryList.push_back(
		"CREATE FUNCTION copy_type_id() RETURNS TRIGGER AS $trig$ "
		"BEGIN "
		"INSERT INTO list_of_types_of_params VALUES "
		"(NEW.id, NEW.tableoid); "
		"RETURN NULL; "
		"END; "
		"$trig$ LANGUAGE plpgsql; ");
//------------------------


//------------------------
	m_queryList.push_back(
		"CREATE TABLE real("
		"id          integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
		"def_val     real,"
		"min         real,"
		"max         real,"
		"PRIMARY KEY (id)"
		");");

	m_queryList.push_back(
		"CREATE TRIGGER real_trig "
		"AFTER INSERT ON real "
		"FOR EACH ROW "
		"EXECUTE PROCEDURE copy_type_id();");
//------------------------

//------------------------
	m_queryList.push_back(
		"CREATE TABLE enum("
		"id          integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
		"def_val     VARCHAR(40),"
		"PRIMARY KEY (id)"
		");");

	m_queryList.push_back(
		"CREATE TRIGGER enum_trig "
		"AFTER INSERT ON enum "
		"FOR EACH ROW "
		"EXECUTE PROCEDURE copy_type_id();");

	m_queryList.push_back(
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
	m_queryList.push_back(
		"CREATE TABLE int("
		"id          integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
		"def_val     integer,"
		"min         integer,"
		"max         integer,"
		"PRIMARY KEY (id)"
		");");

	m_queryList.push_back(
		"CREATE TRIGGER int_trig "
		"AFTER INSERT ON int "
		"FOR EACH ROW "
		"EXECUTE PROCEDURE copy_type_id();");
//------------------------

//------------------------
	m_queryList.push_back(
		"CREATE TABLE void("
		"id      integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
		"PRIMARY KEY (id)"
		");");

	m_queryList.push_back(
		"CREATE TRIGGER void_trig "
		"AFTER INSERT ON void "
		"FOR EACH ROW "
		"EXECUTE PROCEDURE copy_type_id();");
//------------------------

	dataTypeTable("identificator","VARCHAR(40)");
	dataTypeTable("bool","boolean");
	dataTypeTable("string","VARCHAR(40)");



//------------------------
	m_queryList.push_back(
		"CREATE TABLE rdo_value("
		"value_id  integer,"
		"table_id integer NOT NULL,"
		"PRIMARY KEY (value_id)"
		");");


	m_queryList.push_back("CREATE SEQUENCE rdo_value_seq;");

	m_queryList.push_back(
		"CREATE FUNCTION copy_rdo_value_id() RETURNS TRIGGER AS $trig$ "
		"BEGIN "
		"INSERT INTO rdo_value VALUES "
		"(NEW.id, NEW.tableoid); "
		"RETURN NULL; "
		"END; "
		"$trig$ LANGUAGE plpgsql; ");
//------------------------

	rdoValueTable("real_rv","real");
	rdoValueTable("int_rv","integer");
	rdoValueTable("identificator_rv","VARCHAR(40)");
	rdoValueTable("bool_rv","boolean");
	rdoValueTable("string_rv","VARCHAR(40)");
	rdoValueTable("enum_rv","VARCHAR(40)");

//------------------------
	m_queryList.push_back(
		"CREATE TABLE array_rv("
		"id  integer NOT NULL DEFAULT nextval('rdo_value_seq'),"
		"PRIMARY KEY (id)"
		");");

	m_queryList.push_back(
		"CREATE TRIGGER array_rv_trig "
		"AFTER INSERT ON array_rv "
		"FOR EACH ROW "
		"EXECUTE PROCEDURE copy_rdo_value_id();");

	m_queryList.push_back(
		"CREATE TABLE array_value("
		"array_id      integer,"
		"array_elem_id serial,"
		"vv_id         integer NOT NULL,"
		"PRIMARY KEY   (array_id, vv_id),"
		"FOREIGN KEY   (array_id) REFERENCES array_rv"
		");");
//------------------------
}
