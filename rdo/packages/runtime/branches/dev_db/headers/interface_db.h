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
#include "simulator\compiler\parser\type\type_i_db.h"
#include "simulator\runtime\headers\general_db.h"
// --------------------------------------------------------------------------------

class InterfaceDB: public GeneralDB
{
public:
	void insertRow      (const QString& tableName, const QString& qRow);
	void insertRTPHeader(const QString& rtp_name, bool permanent);
	void insertRTPParam (const QString& rtp_name, const QString& param_name, rdo::compiler::parser::ITypeDB type);
	void insertIntRow   ();
	void insertEnumRow  (const QString& defaultValue);

	typedef std::map <QString,int> enumContainer;

	int  getEnumTypeID       (enumContainer& container, const QString defaultValue = "NULL");

private:
};

#endif // _SIMULATOR_RUNTIME_SQL_INTERFACE_DB_H_