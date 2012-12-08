/*!
  \copyright (c) RDO-Team, 2012
  \file      interface_db.h
  \authors   Клеванец Игорь (cerevra@gmail.com)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      15.11.2012
  \brief     Работа с запросами к базе данных
  \indent    4T
*/

#ifndef _SIMULATOR_RUNTIME_SQL_INTERFACE_DB_H_
#define _SIMULATOR_RUNTIME_SQL_INTERFACE_DB_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtSql\QtSql>
#include <map>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator\runtime\test\sql\lib\general_db.h"
// --------------------------------------------------------------------------------

class InterfaceDB: GeneralDB
{
public:
	void insertRow           (const QString& tableName, const QString& qRow);
	void insertRtpRow        (const QString& r_t_name, const QString& r_t_perm);
	void insertIntRow        ();
	void insertEnumRow       (const QString& defaultValue);
	void insertParamOfTypeRow(const QString& rtp_name, const QString& param_name, const int type_id);

	typedef std::map <QString,int> enumContainer;

	int  getEnumTypeID       (enumContainer& container, const QString defaultValue = "NULL");

private:
};

#endif // _SIMULATOR_RUNTIME_SQL_INTERFACE_DB_H_