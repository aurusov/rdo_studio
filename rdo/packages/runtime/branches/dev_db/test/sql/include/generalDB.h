#include <QtSql\QtSql>

class generalDB
{
public:
	generalDB();
	generalDB(const QString& qserv, const QString& qdbase, const QString&	quName, const	QString&	qpass, const int& port);

	QString		 m_qserv ;
	QString		 m_quName;
	QString		 m_qpass ;
	int    		 m_port  ;
	QString		 m_qdbase;
	QSqlDatabase m_db;
};