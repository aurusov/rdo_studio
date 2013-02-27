/*!
  \copyright (c) RDO-Team, 2012
  \file      interface_db.h
  \authors   Клеванец Игорь (cerevra@gmail.com)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      15.11.2012
  \brief     Интерфейс работы с БД
  \indent    4T
*/

template <class T>
void IDB::pushContext           (T context)
{
	pushContxt(boost::any(context));
}

template <class T>
T IDB::popContext            ()
{
	return boost::any_cast<T>(popContxt());
}
