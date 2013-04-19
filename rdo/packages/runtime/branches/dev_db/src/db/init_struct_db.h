/*!
  \copyright (c) RDO-Team, 2013
  \file      init_struct_db.h
  \authors   Клеванец Игорь (cerevra@gmail.com)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      25.01.2013
  \brief     Создание структуры БД
  \indent    4T
*/

#ifndef _SIMULATOR_PARSER_DB_INIT_STRUCT_H_
#define _SIMULATOR_PARSER_DB_INIT_STRUCT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator\runtime\src\db\general_db.h"
// --------------------------------------------------------------------------------

class InitStructDB : public GeneralDB
{
public:
	InitStructDB(QString nameDB);
	static void dropDB(QString db);
	static void createDB(QString db);

private:
	void rdoValueTable        (QString tableName, QString dataType);
#ifdef SERIALIZE_IN_DB_RTP_DETAILS
	void dataTypeTable        (QString tableName);
#endif
	void trigger              (QString tableName, QString functionName);
	void generateCreateDBQuery();
	void generateCreateTrcDBQuery();

	GeneralDB::QueryList m_queryList;
};

#endif // _SIMULATOR_PARSER_DB_INIT_STRUCT_H_
