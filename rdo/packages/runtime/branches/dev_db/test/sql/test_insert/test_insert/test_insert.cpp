
#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include <map>
#include <windows.h>

#include <QtSql\QtSql>
#include <QtCore\QCoreApplication>

bool insertRow (QString tableName, QString qRow);
QString rtpRow (QString r_t_name, QString r_t_perm);
QString param_of_type_Row (QString param_name, int r_t_id, int type_id);
QString int_Row();
QString enum_vV_Row(int enum_id, QString vv_str, int vv_int);
int newEnum(std::map <QString,int> cont, QString def_val);
void newParam(QString client, QString param, int rtpIndex);
void eraseMap();
QString enum_Row(QString def_val);
int chkEnum(std::map <QString,int> cont, QString def_val);

std::map <QString,int> enumCont;


int main(int argc, char *argv[])
{
	system("..\\..\\test\\Debug\\test.exe");//-----------------------------удалить к чертям
	Sleep(5000);


	QCoreApplication  app(argc, argv);

	QString qserv     = "localhost";
	QString qdbase    = "rdo";
	QString quName    = "postgres";
	QString qpass     = "rdo";
	int     port      = 5432;

	QSqlDatabase   db = QSqlDatabase::addDatabase("QPSQL");
	db.setHostName     (qserv);
	db.setDatabaseName (qdbase);
	db.setUserName     (quName);
	db.setPassword     (qpass);
	db.setPort         (port);

	if (!(db.open()))
		std::cout << "Connection to datebase failed! :(" << std::endl;

	QTextCodec* codec = QTextCodec::codecForName("CP1251");
	QTextCodec::setCodecForCStrings(codec);

	QSqlQuery query;

//------------------------
	insertRow("rtp",rtpRow("Парикмахерские","true" ));

	insertRow("int",int_Row());

	newParam("Парикмахерские","количество_в_очереди",1);
//------------------------

//------------------------
	insertRow("rtp",rtpRow("Клиенты","false"));

	if(!enumCont.empty()) eraseMap();
	enumCont.insert(enumCont.begin(), std::pair<QString,int> ("Мальчик",0));
	enumCont.insert(enumCont.begin(), std::pair<QString,int> ("Девочка",1));
	newParam("Клиенты","тип",chkEnum(enumCont,"NULL"));

	if(!enumCont.empty()) eraseMap();
	enumCont.insert(enumCont.begin(), std::pair<QString,int> ("Пришел",0));
	enumCont.insert(enumCont.begin(), std::pair<QString,int> ("Начал_стрижку",1));
	newParam("Клиенты","состояние",chkEnum(enumCont,"NULL"));
//------------------------

//------------------------
	insertRow("rtp",rtpRow("Парикмахеры","true"));

	if(!enumCont.empty()) eraseMap();
	enumCont.insert(enumCont.begin(), std::pair<QString,int> ("Свободен",0));
	enumCont.insert(enumCont.begin(), std::pair<QString,int> ("Занят",1));
	newParam("Парикмахеры","состояние_парикмахера",chkEnum(enumCont,"Свободен"));

	newParam("Парикмахеры","количество_обслуженных",1);

	newParam("Парикмахеры","длительность_min",1);

	newParam("Парикмахеры","длительность_max",1);

	if(!enumCont.empty()) eraseMap();
	newParam("Парикмахеры","тип_клиента",chkEnum(enumCont,"2"));
//------------------------




	db.close();
	std::cout << "to be continued...";
	getch();
	return 0;
}


bool insertRow (QString tableName, QString qRow)
{
	QSqlQuery query;
	QString insertRow = "INSERT INTO " + tableName + " VALUES(" + qRow + ");";
	bool insertResult = query.exec(insertRow);

	if (!(insertResult))
	{
		QString message = "Can't insert! row '" + qRow + "' into table '" + tableName + "' :(";
		std::cout << std::string(message.toUtf8().data()) << std::endl;
	}
	return insertResult;
}


QString rtpRow (QString r_t_name, QString r_t_perm)
{
	return "DEFAULT,'" + r_t_name + "'," + r_t_perm;
}

QString param_of_type_Row (QString param_name, int r_t_id, int type_id)
{
	return "DEFAULT,'" + param_name + "'," + QString::number(r_t_id) + "," + QString::number(type_id);
}

QString int_Row()
{
	return "DEFAULT,NULL,NULL,NULL";
}

QString enum_Row(QString def_val)
{
	return "DEFAULT,'" + def_val + "'";
}

QString enum_vV_Row(int enum_id, QString vv_str, int vv_int)
{
	return QString::number(enum_id) + ",DEFAULT,'" + vv_str + "'," + QString::number(vv_int);
}

int newEnum(std::map <QString,int> cont, QString def_val = "NULL")
{
	QSqlQuery query;
	insertRow("enum",enum_Row(def_val));

	query.exec("select max(type_id) as alt from enum;");
	query.next();
	int enum_id = query.value(query.record().indexOf("alt")).toInt();

	std::map <QString,int>::iterator it;
	for ( it=cont.begin(); it != cont.end(); ++it )
	{
		insertRow("enum_valid_value",enum_vV_Row(enum_id,(*it).first,(*it).second));
	}

	return enum_id;
}

void newParam(QString client, QString param, int rtpIndex)
{
	QSqlQuery query;
	query.exec("select r_t_id from rtp where r_t_name='" + client + "';");
	query.next();
	insertRow("param_of_type",
				param_of_type_Row(param,query.value(query.record().indexOf("r_t_id")).toInt(),
				rtpIndex));
}

void eraseMap()
{
	while(!enumCont.empty())
	{
		enumCont.erase(enumCont.begin());
	}
}

int chkEnum(std::map <QString,int> cont, QString def_val = "NULL")
{
	if (enumCont.empty()) return def_val.toInt();
	else return newEnum(cont,def_val);
}
