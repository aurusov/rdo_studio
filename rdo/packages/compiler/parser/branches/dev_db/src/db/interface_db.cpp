/*!
  \copyright (c) RDO-Team, 2012
  \file      interface_db.cpp
  \authors   Клеванец Игорь (cerevra@gmail.com)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      15.11.2012
  \brief     Интерфейс работы с БД
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <QtSql\QtSql>
#include <QtSql\QSqlError>
#include <iostream>
#include <boost/foreach.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator\compiler\parser\headers\general_db.h"
#include "simulator\compiler\parser\headers\db\interface_db.h"
// --------------------------------------------------------------------------------



//void InterfaceDB::insertRTPHeader(const QString& rtp_name, bool permanent)
//{
//	insertRow("rtp", "DEFAULT,'" + rtp_name + "'," + (permanent ? "true" : "false"));
//}
//
//void InterfaceDB::insertIntRow()
//{
//	insertRow("int","DEFAULT,NULL,NULL,NULL");
//}
//
//void InterfaceDB::insertEnumRow(const QString& defaultValue)
//{
//	insertRow("enum","DEFAULT,'" + defaultValue + "'");
//}

//void InterfaceDB::insertRTPParam(const QString& rtp_name, const QString& param_name, rdo::compiler::parser::RuntimeWrapperType type)
//{
//	QSqlQuery query;
//	query.exec("select r_t_id from rtp where r_t_name='" + rtp_name + "';");
//	query.next();
//	insertRow("param_of_type","DEFAULT,'" + param_name + "'," + QString::number(query.value(query.record().indexOf("r_t_id")).toInt()) + "," + QString::number(type.selfInsertToDB(*this)));
//}

//int InterfaceDB::getEnumTypeID(enumContainer& container, const QString defaultValue)
//{
//	QSqlQuery query;
//	insertEnumRow(defaultValue);
//
//	query.exec("select max(type_id) as alt from enum;");
//	query.next();
//	int enum_id = query.value(query.record().indexOf("alt")).toInt();
//
//	std::map <QString,int>::const_iterator it;
//	for ( it=container.begin(); it != container.end(); ++it )
//	{
//		insertRow("enum_valid_value",QString::number(enum_id) + ",DEFAULT,'" + (*it).first+ "'," + QString::number((*it).second));
//	}
//
//	return enum_id;
//}
