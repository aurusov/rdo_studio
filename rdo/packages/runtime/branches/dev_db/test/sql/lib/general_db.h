/*!
  \copyright (c) RDO-Team, 2012
  \file      general_db.h
  \authors   Клеванец Игорь (cerevra@gmail.com)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      15.11.2012
  \brief     Базовая работа с базой данных
  \indent    4T
*/

#ifndef _LIB_RUNTIME_SQL_GENERAL_H_
#define _LIB_RUNTIME_SQL_GENERAL_H_

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <QtSql\QtSql>
// ----------------------------------------------------------------------- SYNOPSIS
// --------------------------------------------------------------------------------

class GeneralDB
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

private:
	void initDB();
	void setDefParams();

	QString			m_hostName;
	QString			m_databaseName;
	QString			m_userName;
	QString			m_password;
	int				m_port;
	QSqlDatabase	m_db;

};

#endif // _LIB_RUNTIME_SQL_GENERAL_H_