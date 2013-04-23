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
#include "utils\rdodebug.h"
// --------------------------------------------------------------------------------

//#define SERIALIZE_IN_DB_RTP_DETAILS

class IDB
{
public:
	typedef std::list<QString>   QueryList;
	typedef boost::any           bany;

	virtual void         insertRow        (const QString& tableName, const QString& qRow) = 0;
	virtual int          insertRowInd     (const QString& tableName, const QString& qRow) = 0;

	virtual void         queryExec        (const QString&   query) = 0;
	virtual void         queryExec        (const QueryList& query) = 0;
	virtual void         queryListPushBack(const QString&   query) = 0;
	virtual void         queryListExec    () = 0;
	virtual int          queryListExecInd () = 0;

	virtual void         pushContxt       (CREF(bany) context) = 0;
	virtual bany         popContxt        () = 0;
	virtual bool         isEmptyContext   () = 0;

	virtual QSqlDatabase getQtDB          () = 0;

	template <class T> void pushContext   (T context);
	template <class T> T popContext       ();
};

#include "simulator\runtime\src\db\interface_db.inl"

#endif // _SIMULATOR_RUNTIME_SQL_INTERFACE_DB_H_
