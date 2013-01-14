/*!
  \copyright (c) RDO-Team, 2012
  \file      general_db.h
  \authors   Клеванец Игорь (cerevra@gmail.com)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      15.11.2012
  \brief     Базовая работа с базой данных
  \indent    4T
*/

#ifndef _SIMULATOR_RUNTIME_SQL_GENERAL_DB_H_
#define _SIMULATOR_RUNTIME_SQL_GENERAL_DB_H_

// ----------------------------------------------------------------------- INCLUDES
#include <QtSql\QtSql>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator\compiler\parser\headers\interface_db.h"
// --------------------------------------------------------------------------------

class GeneralDB: public InterfaceDB
{
public:
	GeneralDB();
	GeneralDB(
		const QString& hostName,
		const QString& databaseName,
		const QString& userName,
		const QString& password,
		const int& port
	);
	~GeneralDB();

	typedef  std::list<QString>  QueryList;

	void queryExec(const QString&   query);
	void queryExec(const QueryList& query);

	virtual void insertRow      (const QString& tableName, const QString& qRow);

private:
	void initDB();
	void setDefParams();

	QString      m_hostName;
	QString      m_databaseName;
	QString      m_userName;
	QString      m_password;
	int          m_port;
	QSqlDatabase m_db;

};

#endif // _SIMULATOR_RUNTIME_SQL_GENERAL_DB_H_