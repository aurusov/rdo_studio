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
#include "simulator\runtime\src\db\init_struct_db.h"
// --------------------------------------------------------------------------------

InitStructDB::InitStructDB(QString nameDB)
	: GeneralDB("localhost", nameDB, "postgres", "rdo", 5432)
{
	if(nameDB == QString("rdo"))
		generateCreateDBQuery   ();
	if(nameDB == QString("trc"))
		generateCreateTrcDBQuery();

	queryExec(m_queryList);
	m_queryList.clear();
}

void InitStructDB::dropDB(QString db)
{
	{
		GeneralDB dbTemp("localhost", "postgres", "postgres", "rdo", 5432);
		dbTemp.queryExec(QString("DROP DATABASE IF EXISTS %1;")
			.arg(db));
	}
	QSqlDatabase::removeDatabase("postgres");
}

void InitStructDB::createDB(QString db)
{
	{
		GeneralDB dbTemp("localhost", "postgres", "postgres", "rdo", 5432);
		dbTemp.queryExec(QString("CREATE DATABASE %1;")
			.arg(db));
	}
	QSqlDatabase::removeDatabase("postgres");
}

void InitStructDB::rdoValueTable(QString tableName, QString dataType)
{
	m_queryList.push_back(QString(
		"CREATE TABLE %1("
		"id integer NOT NULL DEFAULT nextval('rdo_value_seq'),"
		"vv %2 NOT NULL,"
		"PRIMARY KEY (id)"
		");")
		.arg(tableName)
		.arg(dataType));

	trigger(tableName,"copy_rdo_value_id");
}

#ifdef SERIALIZE_IN_DB_RTP_DETAILS
void InitStructDB::dataTypeTable(QString tableName)
{
	m_queryList.push_back(QString(
		"CREATE TABLE %1("
		"id      integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
		"def_val integer,"
		"PRIMARY KEY (id),"
		"FOREIGN KEY (def_val) REFERENCES %1_rv(id)"
		");")
		.arg(tableName));

	trigger(tableName,"copy_type_id");
}
#endif

void InitStructDB::trigger(QString tableName, QString functionName)
{
	m_queryList.push_back(QString(
		"CREATE TRIGGER %1_trig "
		"AFTER INSERT ON %1 "
		"FOR EACH ROW "
		"EXECUTE PROCEDURE %2();")
		.arg(tableName)
		.arg(functionName));
}

void InitStructDB::generateCreateDBQuery()
{
	m_queryList.push_back(
		"CREATE TABLE rtp("
		"id       serial,"
		"rtp_name VARCHAR(40) NOT NULL,"
		"rtp_perm boolean NOT NULL,"
		"PRIMARY KEY (id)"
		");");

#ifdef SERIALIZE_IN_DB_RTP_DETAILS
	m_queryList.push_back(
		"CREATE TABLE list_of_types_of_params("
		"type_id  integer,"
		"table_id integer NOT NULL,"
		"PRIMARY KEY (type_id)"
		");");

	m_queryList.push_back(
		"CREATE TABLE param_of_type("
		"id          integer NOT NULL,"
		"param_name  VARCHAR(40) NOT NULL,"
		"rtp_id      integer,"
		"type_id     integer NOT NULL,"
		"PRIMARY KEY (id,rtp_id),"
		"FOREIGN KEY (rtp_id) REFERENCES rtp(id),"
		"FOREIGN KEY (type_id) REFERENCES list_of_types_of_params"
		");");

	m_queryList.push_back("CREATE SEQUENCE type_of_param_seq;");

	m_queryList.push_back(
		"CREATE FUNCTION copy_type_id() RETURNS TRIGGER AS $trig$ "
		"BEGIN "
		"INSERT INTO list_of_types_of_params VALUES "
		"(NEW.id, NEW.tableoid); "
		"RETURN NULL; "
		"END; "
		"$trig$ LANGUAGE plpgsql; ");

	m_queryList.push_back(
		"CREATE TABLE real("
		"id          integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
		"def_val     integer,"
		"min         integer,"
		"max         integer,"
		"PRIMARY KEY (id),"
		"FOREIGN KEY (def_val) REFERENCES real_rv(id),"
		"FOREIGN KEY (min) REFERENCES real_rv(id),"
		"FOREIGN KEY (max) REFERENCES real_rv(id)"
		");");

	trigger("real","copy_type_id");

	m_queryList.push_back(
		"CREATE TABLE enum("
		"id          integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
		"def_val     integer,"
		"PRIMARY KEY (id),"
		"FOREIGN KEY (def_val) REFERENCES enum_rv(id)"
		");");

	trigger("enum","copy_type_id");

	m_queryList.push_back(
		"CREATE TABLE enum_valid_value("
		"enum_id     integer,"
		"vv_id       serial,"
		"vv_str      VARCHAR(40) NOT NULL,"
		"vv_int      integer NOT NULL,"
		"PRIMARY KEY (enum_id, vv_id),"
		"FOREIGN KEY (enum_id) REFERENCES enum"
		");");

	m_queryList.push_back(
		"CREATE TABLE int("
		"id          integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
		"def_val     integer,"
		"min         integer,"
		"max         integer,"
		"PRIMARY KEY (id),"
		"FOREIGN KEY (def_val) REFERENCES int_rv(id),"
		"FOREIGN KEY (min) REFERENCES int_rv(id),"
		"FOREIGN KEY (max) REFERENCES int_rv(id)"
		");");

	trigger("int","copy_type_id");

	m_queryList.push_back(
		"CREATE TABLE array_t("
		"id          integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
		"type_id     integer NOT NULL,"
		"def_val     integer,"
		"PRIMARY KEY (id),"
		"FOREIGN KEY (type_id) REFERENCES list_of_types_of_params(type_id),"
		"FOREIGN KEY (def_val) REFERENCES array_rv(id)"
		");");

	trigger("array_t","copy_type_id");

	m_queryList.push_back(
		"CREATE TABLE void("
		"id      integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
		"PRIMARY KEY (id)"
		");");

	trigger("void","copy_type_id");

	dataTypeTable("identificator");
	dataTypeTable("bool");
	dataTypeTable("string");
#endif

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

	rdoValueTable("real_rv","real");
	rdoValueTable("int_rv","integer");
	rdoValueTable("identificator_rv","VARCHAR(40)");
	rdoValueTable("bool_rv","boolean");
	rdoValueTable("string_rv","VARCHAR(40)");
	rdoValueTable("enum_rv","VARCHAR(40)");

	m_queryList.push_back(
		"CREATE TABLE array_rv("
		"id  integer NOT NULL DEFAULT nextval('rdo_value_seq'),"
		"PRIMARY KEY (id)"
		");");

	trigger("array_rv","copy_rdo_value_id");

	m_queryList.push_back(
		"CREATE TABLE array_value("
		"array_id      integer,"
		"array_elem_id serial,"
		"vv_id         integer NOT NULL,"
		"PRIMARY KEY   (array_id, vv_id),"
		"FOREIGN KEY   (array_id) REFERENCES array_rv"
		");");

	m_queryList.push_back(
		"CREATE TABLE rss("
		"id      integer NOT NULL,"
		"rtp_id  integer NOT NULL,"
		"trace   boolean NOT NULL,"
		"PRIMARY KEY (id),"
		"FOREIGN KEY (rtp_id) REFERENCES rtp(id)"
		");");

	m_queryList.push_back(
		"CREATE TABLE rss_param("
		"rss_id   integer NOT NULL,"
		"id       integer NOT NULL,"
		"value    integer NOT NULL,"
		"PRIMARY KEY (id,rss_id),"
		"FOREIGN KEY (rss_id) REFERENCES rss(id),"
		"FOREIGN KEY (value) REFERENCES rdo_value(value_id)"
		");");
}

void InitStructDB::generateCreateTrcDBQuery()
{
	m_queryList.push_back(
		"CREATE TABLE trc_resource_type("
		"rtp_id			integer NOT NULL,"
		"name			varchar(40) NOT NULL,"
		"param_count	integer,"				//!!!!!
		"PRIMARY KEY (rtp_id)"
		");");
}
