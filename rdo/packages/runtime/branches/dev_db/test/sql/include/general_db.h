/*!
  \copyright (c) RDO-Team, 2012
  \file      general.h
  \authors   Клеванец Игорь (cerevra@gmail.com)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      15.11.2012
  \brief     Работа с подключением к базе данных
  \indent    4T
*/

#include <QtSql\QtSql>

class generalDB
{
public:
	generalDB();
	generalDB(const QString& qserv, const QString& qdbase, const QString&	quName, const	QString&	qpass, const int& port);
	~generalDB();

	void initDB();
	void setDefParams();
	void queryExec(const QString& quer);

	QString		 m_qserv ;
	QString		 m_quName;
	QString		 m_qpass ;
	int    		 m_port  ;
	QString		 m_qdbase;
	QSqlDatabase m_db    ;
};
