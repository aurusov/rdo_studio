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

	queryListExec();
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
	queryListPushBack(QString(
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
	queryListPushBack(QString(
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
	queryListPushBack(QString(
		"CREATE TRIGGER %1_trig "
		"AFTER INSERT ON %1 "
		"FOR EACH ROW "
		"EXECUTE PROCEDURE %2();")
		.arg(tableName)
		.arg(functionName));
}

void InitStructDB::generateCreateDBQuery()
{
	queryListPushBack(
		"CREATE TABLE rtp("
		"id       serial,"
		"rtp_name VARCHAR(40) NOT NULL,"
		"rtp_perm boolean NOT NULL,"
		"PRIMARY KEY (id)"
		");");

#ifdef SERIALIZE_IN_DB_RTP_DETAILS
	queryListPushBack(
		"CREATE TABLE list_of_types_of_params("
		"type_id  integer,"
		"table_id integer NOT NULL,"
		"PRIMARY KEY (type_id)"
		");");

	queryListPushBack(
		"CREATE TABLE param_of_type("
		"id          integer NOT NULL,"
		"param_name  VARCHAR(40) NOT NULL,"
		"rtp_id      integer,"
		"type_id     integer NOT NULL,"
		"PRIMARY KEY (id,rtp_id),"
		"FOREIGN KEY (rtp_id) REFERENCES rtp(id),"
		"FOREIGN KEY (type_id) REFERENCES list_of_types_of_params"
		");");

	queryListPushBack("CREATE SEQUENCE type_of_param_seq;");

	queryListPushBack(
		"CREATE FUNCTION copy_type_id() RETURNS TRIGGER AS $trig$ "
		"BEGIN "
		"INSERT INTO list_of_types_of_params VALUES "
		"(NEW.id, NEW.tableoid); "
		"RETURN NULL; "
		"END; "
		"$trig$ LANGUAGE plpgsql; ");

	queryListPushBack(
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

	queryListPushBack(
		"CREATE TABLE enum("
		"id          integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
		"def_val     integer,"
		"PRIMARY KEY (id),"
		"FOREIGN KEY (def_val) REFERENCES enum_rv(id)"
		");");

	trigger("enum","copy_type_id");

	queryListPushBack(
		"CREATE TABLE enum_valid_value("
		"enum_id     integer,"
		"vv_id       serial,"
		"vv_str      VARCHAR(40) NOT NULL,"
		"vv_int      integer NOT NULL,"
		"PRIMARY KEY (enum_id, vv_id),"
		"FOREIGN KEY (enum_id) REFERENCES enum"
		");");

	queryListPushBack(
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

	queryListPushBack(
		"CREATE TABLE array_t("
		"id          integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
		"type_id     integer NOT NULL,"
		"def_val     integer,"
		"PRIMARY KEY (id),"
		"FOREIGN KEY (type_id) REFERENCES list_of_types_of_params(type_id),"
		"FOREIGN KEY (def_val) REFERENCES array_rv(id)"
		");");

	trigger("array_t","copy_type_id");

	queryListPushBack(
		"CREATE TABLE void("
		"id      integer NOT NULL DEFAULT nextval('type_of_param_seq'),"
		"PRIMARY KEY (id)"
		");");

	trigger("void","copy_type_id");

	dataTypeTable("identificator");
	dataTypeTable("bool");
	dataTypeTable("string");
#endif

	queryListPushBack(
		"CREATE TABLE rdo_value("
		"value_id  integer,"
		"table_id integer NOT NULL,"
		"PRIMARY KEY (value_id)"
		");");

	queryListPushBack("CREATE SEQUENCE rdo_value_seq;");

	queryListPushBack(
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

	queryListPushBack(
		"CREATE TABLE array_rv("
		"id  integer NOT NULL DEFAULT nextval('rdo_value_seq'),"
		"PRIMARY KEY (id)"
		");");

	trigger("array_rv","copy_rdo_value_id");

	queryListPushBack(
		"CREATE TABLE array_value("
		"array_id      integer,"
		"array_elem_id serial,"
		"vv_id         integer NOT NULL,"
		"PRIMARY KEY   (array_id, vv_id),"
		"FOREIGN KEY   (array_id) REFERENCES array_rv"
		");");

	queryListPushBack(
		"CREATE TABLE rss("
		"id      integer NOT NULL,"
		"rtp_id  integer NOT NULL,"
		"trace   boolean NOT NULL,"
		"PRIMARY KEY (id),"
		"FOREIGN KEY (rtp_id) REFERENCES rtp(id)"
		");");

	queryListPushBack(
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
	queryListPushBack(
		"CREATE TABLE trc_resource_type("
		"rtp_id       integer NOT NULL,"
		"name         varchar(40) NOT NULL,"
		"param_count  integer,"					//!!!!!
		"PRIMARY KEY (rtp_id)"
		");");

	queryListPushBack(
		"CREATE TABLE trc_param("
		"param_id    integer NOT NULL,"
		"rtp_id      integer NOT NULL,"
		"name        varchar(40) NOT NULL,"
		"param_type  char(1) NOT NULL,"
		"count       integer,"					//!!!!!
		"PRIMARY KEY (param_id,rtp_id),"
		"FOREIGN KEY (rtp_id) REFERENCES trc_resource_type(rtp_id)"
		");");

	queryListPushBack(
		"CREATE TABLE trc_vv("
		"param_id     integer NOT NULL,"
		"rtp_id       integer NOT NULL,"
		"string       varchar(40) NOT NULL,"
		"int          integer NOT NULL,"
		"PRIMARY KEY (param_id,rtp_id),"
		"FOREIGN KEY (param_id,rtp_id) REFERENCES trc_param(param_id,rtp_id)"
		");");

	queryListPushBack(
		"CREATE TABLE trc_resources("
		"res_id integer NOT NULL,"
		"name   varchar(40) NOT NULL,"
		"rtp_id integer NOT NULL,"
		"PRIMARY KEY (res_id),"
		"FOREIGN KEY (rtp_id) REFERENCES trc_resource_type(rtp_id)"
		");");

	queryListPushBack(
		"CREATE TABLE trc_patterns("
		"pat_id       integer NOT NULL,"
		"name         varchar(40) NOT NULL,"
		"type         char(1) NOT NULL,"
		"count_res    integer,"				//!!!!!
		"PRIMARY KEY (pat_id)"
		");");

	queryListPushBack(
		"CREATE TABLE trc_relres("
		"id           serial NOT NULL,"
		"rtp_id       integer NOT NULL,"
		"pat_id       integer NOT NULL,"
		"PRIMARY KEY (id),"
		"FOREIGN KEY (rtp_id) REFERENCES trc_resource_type(rtp_id),"
		"FOREIGN KEY (pat_id) REFERENCES trc_patterns(pat_id)"
		");");

	queryListPushBack(
		"CREATE TABLE trc_activities("
		"act_id       integer NOT NULL,"
		"name         varchar(40) NOT NULL,"
		"pat_id       integer NOT NULL,"
		"PRIMARY KEY (act_id),"
		"FOREIGN KEY (pat_id) REFERENCES trc_patterns(pat_id)"
		");");

	queryListPushBack(
		"CREATE TABLE trc_time("
		"time         real NOT NULL,"
		"PRIMARY KEY (time)"
		");");

	queryListPushBack("CREATE SEQUENCE trc_row_id;");

	queryListPushBack(
		"CREATE TABLE trc_rows("
		"id           integer NOT NULL,"
		"tableid      integer NOT NULL,"
		"type         varchar(5) NOT NULL,"
		"PRIMARY KEY (id)"
		");");

	queryListPushBack(
		"CREATE FUNCTION trc_copy_row() RETURNS TRIGGER AS $trig$ "
		"BEGIN "
		"INSERT INTO trc_rows VALUES "
		"(NEW.id, NEW.tableoid, NEW.type); "
		"RETURN NULL; "
		"END; "
		"$trig$ LANGUAGE plpgsql;");

	queryListPushBack(
		"CREATE FUNCTION trc_copy_es_row() RETURNS TRIGGER AS $trig$ "
		"BEGIN "
		"INSERT INTO trc_rows VALUES "
		"(NEW.id, NEW.tableoid, 'ES'); "
		"RETURN NULL; "
		"END; "
		"$trig$ LANGUAGE plpgsql;");

	queryListPushBack(
		"CREATE FUNCTION trc_copy_er_row() RETURNS TRIGGER AS $trig$ "
		"BEGIN "
		"INSERT INTO trc_rows VALUES "
		"(NEW.id, NEW.tableoid, 'ER'); "
		"RETURN NULL; "
		"END; "
		"$trig$ LANGUAGE plpgsql;");

	queryListPushBack(
		"CREATE TABLE trc_es("
		"id		integer NOT NULL DEFAULT nextval('trc_row_id'),"
		"time	real NOT NULL,"
		"sno		integer NOT NULL,"
		"PRIMARY KEY (id),"
		"FOREIGN KEY (time) REFERENCES trc_time(time)"
		");");

	queryListPushBack(
		"CREATE TRIGGER trc_es_trig "
		"AFTER INSERT ON trc_es "
		"FOR EACH ROW "
		"EXECUTE PROCEDURE trc_copy_es_row();");

	queryListPushBack(
		"CREATE TABLE trc_eb_ef("
		"id           integer NOT NULL DEFAULT nextval('trc_row_id'),"
		"time         real NOT NULL,"
		"type         varchar(5) NOT NULL,"
		"internal_id  integer NOT NULL,"
		"act_id       integer NOT NULL,"
		"pat_id       integer NOT NULL,"					//!!!!!
		"res_count    integer NOT NULL,"					//!!!!!
		"PRIMARY KEY (id),"
		"FOREIGN KEY (time) REFERENCES trc_time(time),"
		"FOREIGN KEY (act_id) REFERENCES trc_activities(act_id),"
		"FOREIGN KEY (pat_id) REFERENCES trc_patterns(pat_id)"
		");");

	queryListPushBack(
		"CREATE TABLE trc_e_res("
		"id           serial,"
		"e_id         integer NOT NULL,"
		"res_id       integer NOT NULL,"
		"PRIMARY KEY (id),"
		"FOREIGN KEY (res_id) REFERENCES trc_resources(res_id)"
		");");

	queryListPushBack(
		"CREATE TRIGGER trc_eb_ef_trig "
		"AFTER INSERT ON trc_eb_ef "
		"FOR EACH ROW "
		"EXECUTE PROCEDURE trc_copy_row();");

	queryListPushBack(
		"CREATE TABLE trc_ei_ee("
		"id           integer NOT NULL DEFAULT nextval('trc_row_id'),"
		"time         real NOT NULL,"
		"type         varchar(5) NOT NULL,"
		"evnt_id      integer NOT NULL,"
		"pat_id       integer NOT NULL,"			//!!!!!
		"res_count    integer NOT NULL,"			//!!!!!
		"PRIMARY KEY (id),"
		"FOREIGN KEY (time) REFERENCES trc_time(time),"
		"FOREIGN KEY (pat_id) REFERENCES trc_patterns(pat_id)"
		");");

	queryListPushBack(
		"CREATE TRIGGER trc_ei_ee_trig "
		"AFTER INSERT ON trc_ei_ee "
		"FOR EACH ROW "
		"EXECUTE PROCEDURE trc_copy_row();");

	queryListPushBack(
		"CREATE TABLE trc_er("
		"id           integer NOT NULL DEFAULT nextval('trc_row_id'),"
		"time         real NOT NULL,"
		"type         varchar(5) NOT NULL,"
		"internal_id  integer NOT NULL,"
		"rule_id      integer NOT NULL,"
		"pat_id       integer NOT NULL,"			//!!!!!
		"res_count    integer NOT NULL,"			//!!!!!
		"PRIMARY KEY (id),"
		"FOREIGN KEY (time) REFERENCES trc_time(time),"
		"FOREIGN KEY (pat_id) REFERENCES trc_patterns(pat_id)"
		");");

	queryListPushBack(
		"CREATE TRIGGER trc_er_trig "
		"AFTER INSERT ON trc_er "
		"FOR EACH ROW "
		"EXECUTE PROCEDURE trc_copy_er_row();");
}
