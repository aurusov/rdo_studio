/*!
  \copyright (c) RDO-Team, 2012
  \file      general.cpp
  \authors   Клеванец Игорь (cerevra@gmail.com)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      15.11.2012
  \brief     Работа с подключением к базе данных
  \indent    4T
*/

#include <QtSql\QtSql>
#include <iostream>
#include "simulator\runtime\test\sql\include\general_db.h"

generalDB::generalDB()
{
	setDefParams();
	initDB();
}

generalDB::generalDB(const QString& qserv, const QString& qdbase, const QString& quName, const QString& qpass, const int& port):
	m_qserv (qserv ),
	m_quName(quName),
	m_qpass (qpass ),
	m_port  (port  ),
	m_qdbase(qdbase)
{
	initDB();
}

generalDB::~generalDB()
{
	m_db.close();
}

void generalDB::initDB()
{
	if (!m_db.contains())
		m_db = QSqlDatabase::addDatabase("QPSQL");
	else if (m_db.isOpen()) 
		m_db.close();

	m_db.setHostName     (m_qserv);
	m_db.setDatabaseName (m_qdbase);
	m_db.setUserName     (m_quName);
	m_db.setPassword     (m_qpass);
	m_db.setPort         (m_port);

	if (!(m_db.open()))
		std::cout << "Connection to datebase failed! :(" << std::endl;
}

void generalDB::setDefParams()
{
	m_qserv  = "localhost";
	m_quName = "postgres";
	m_qpass  = "rdo";
	m_port   = 5432;
	m_qdbase = "rdo";
}

void generalDB::queryExec(const QString& quer)
{
	QString str = m_db.exec(quer).lastError().text();
	if (str != "")
		std::cout << str.toStdString()<< "\n" << std::endl;
	else 
		std::cout << "OK! : " << quer.toStdString()<< "\n" << std::endl;	
}