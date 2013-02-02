/*!
  \copyright (c) RDO-Team, 2012
  \file      general_db.cpp
  \authors   Клеванец Игорь (cerevra@gmail.com)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      15.11.2012
  \brief     Базовая работа с базой данных
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <QtSql\QtSql>
#include <QtSql\QSqlError>
#include <iostream>
#include <boost\foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator\compiler\parser\src\db\general_db.h"
#include "utils\rdodebug.h"
// --------------------------------------------------------------------------------

GeneralDB::GeneralDB(const QString& hostName, const QString& databaseName, const QString& userName, const QString& password, const int& port)
	: m_hostName    (hostName    )
	, m_databaseName(databaseName)
	, m_userName    (userName    )
	, m_password    (password    )
	, m_port        (port        )
{
	initDB();
}

GeneralDB::GeneralDB()
{
	setDefParams();
	initDB      ();
}

GeneralDB::~GeneralDB()
{
	m_db.close();
}

void GeneralDB::insertRow(const QString& tableName, const QString& qRow)
{
	queryExec("INSERT INTO " + tableName + " VALUES(" + qRow + ");");
}

void GeneralDB::queryExec(const QueryList& query)
{
	BOOST_FOREACH(const QString& queryItem, query)
	{
		queryExec(queryItem);
	}
}

void GeneralDB::queryExec(const QString& query)
{
	std::cout << query.toStdString() << "..." << std::endl;

	m_db.exec(query);

	QSqlError lastError = m_db.lastError();
	switch (lastError.type())
	{
	case QSqlError::NoError:
		std::cout << query.toStdString() << "... ok" << std::endl;
		break;

	default:
		std::cerr << query.toStdString() << "... failed: " << lastError.text().toStdString() << std::endl;
		break;
	}
}

int GeneralDB::queryExecIndex(const QString& table)
{
	QSqlQuery query;
	query.exec(QString("select max(id) as alt from %1;")
		.arg(table ));
	query.next();
	int index = query.value(query.record().indexOf("alt")).toInt();
	m_contextParrentID = index;
	return index;
}

void GeneralDB::pushContext(int context)
{
	m_contextParrentID = context;
}

int GeneralDB::popContext()
{
	ASSERT(m_contextParrentID.is_initialized());
	int i = m_contextParrentID.get();
	m_contextParrentID.reset();
	return i;
}

void GeneralDB::initDB()
{
	m_db = QSqlDatabase::addDatabase("QPSQL");

	m_db.setHostName    (m_hostName);
	m_db.setDatabaseName(m_databaseName);
	m_db.setUserName    (m_userName);
	m_db.setPassword    (m_password);
	m_db.setPort        (m_port);

	if (!m_db.open())
	{
		std::cout << "Connection to datebase failed! :(" << std::endl;
	}
}

void GeneralDB::setDefParams()
{
	m_hostName     = "localhost";
	m_databaseName = "rdo";
	m_userName     = "postgres";
	m_password     = "rdo";
	m_port         = 5432;
}
