/*!
  \copyright (c) RDO-Team, 2012
  \file      interface_db.h
  \authors   Клеванец Игорь (cerevra@gmail.com)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      15.11.2012
  \brief     Интерфейс работы с БД
  \indent    4T
*/

#ifndef _SIMULATOR_RUNTIME_SQL_INTERFACE_DB_H_
#define _SIMULATOR_RUNTIME_SQL_INTERFACE_DB_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtSql\QtSql>
#include <map>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class IDB
{
public:
	typedef std::list<QString>   QueryList;

	virtual void insertRow     (const QString& tableName, const QString& qRow) = 0;
	virtual void queryExec     (const QString&   query                       ) = 0;
	virtual void queryExec     (const QueryList& query                       ) = 0;

	virtual int  queryExecIndex(const QString& table                         ) = 0;
	virtual void setContext    (int context                                  ) = 0;
	virtual int  getContext    (                                             ) = 0;

	//void insertRTPHeader(const QString& rtp_name, bool permanent);
	//void insertIntRow   ();
	//void insertEnumRow  (const QString& defaultValue);

	//typedef std::map <QString,int> enumContainer;

	//int  getEnumTypeID       (enumContainer& container, const QString defaultValue = "NULL");
};

#endif // _SIMULATOR_RUNTIME_SQL_INTERFACE_DB_H_
