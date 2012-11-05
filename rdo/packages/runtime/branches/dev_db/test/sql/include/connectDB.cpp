
#include <QtSql\QtSql>
#include <iostream>

QSqlDatabase connectDB(QSqlDatabase db, QString qdbase = "rdo")
{
	if (db.isOpen()) db.close();

	QString qserv     = "localhost";
	QString quName    = "postgres";
	QString qpass     = "rdo";
	int     port      = 5432;

	db.setHostName     (qserv);
	db.setDatabaseName (qdbase);
	db.setUserName     (quName);
	db.setPassword     (qpass);
	db.setPort         (port);

	if (!(db.open()))
		std::cout << "Connection to datebase failed! :(" << std::endl;
	return db;
}
