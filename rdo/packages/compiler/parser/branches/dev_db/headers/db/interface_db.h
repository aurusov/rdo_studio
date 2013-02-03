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
#include <list>
#include <boost\any.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class IDB
{
public:
	typedef std::list<QString>   QueryList;
	typedef boost::any           bany;

	virtual void insertRow     (const QString& tableName, const QString& qRow) = 0;

	virtual void queryExec     (const QString&   query                       ) = 0;
	virtual void queryExec     (const QueryList& query                       ) = 0;
	virtual int  queryExecIndex(const QString& table                         ) = 0;

	virtual void pushContext   (int context                                  ) = 0;
	virtual bany popContext    (                                             ) = 0;
};

#endif // _SIMULATOR_RUNTIME_SQL_INTERFACE_DB_H_
