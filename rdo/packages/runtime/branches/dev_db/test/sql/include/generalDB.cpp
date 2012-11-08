
#include <QtSql\QtSql>
#include <iostream>
#include "simulator\runtime\test\sql\include\generalDB.h"

generalDB::generalDB():
	m_qserv ("localhost"),
	m_quName("postgres"),
	m_qpass ("rdo"),
	m_port  (5432),
	m_qdbase("rdo")
{
	generalDB(m_qserv,m_qdbase,m_quName,m_qpass,m_port);
}

generalDB::generalDB(const QString& qserv, const QString& qdbase, const QString& quName, const QString& qpass, const int& port)
{
	if (!m_db.contains())
		m_db = QSqlDatabase::addDatabase("QPSQL");
	else if (m_db.isOpen()) 
		m_db.close();

	m_db.setHostName     (qserv);
	m_db.setDatabaseName (qdbase);
	m_db.setUserName     (quName);
	m_db.setPassword     (qpass);
	m_db.setPort         (port);

	if (!(m_db.open()))
		std::cout << "Connection to datebase failed! :(" << std::endl;
	QSqlError a = m_db.lastError ();
	QString b = m_db.driverName ();
}
