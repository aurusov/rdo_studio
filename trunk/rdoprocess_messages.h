#ifndef RDO_PROCESS_MESSAGES_H
#define RDO_PROCESS_MESSAGES_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdoprocess_object.h"

// ----------------------------------------------------------------------------
// ---------- RPMessages
// ----------------------------------------------------------------------------
class RPApp;

namespace rp {

class msg: public RPObject
{
friend class RPApp;

protected:
	msg();
	virtual ~msg();

	typedef std::multimap< UINT, RPObject* > Connected;
	Connected connected;

	virtual void notify( RPObject* from, UINT message, WPARAM wParam, LPARAM lParam );

	void sendMessage( RPObject* from, UINT message, WPARAM wParam = 0, LPARAM lParam = 0 );

public:
	virtual rp::string getType() { return "msg"; }

	void connect( RPObject* to, UINT message );
	void disconnect( RPObject* to );
	void disconnect( RPObject* to, UINT message );

	static const UINT RP_OBJ_BEFOREDELETE;
	static const UINT RP_OBJ_NAMECHANGED;
	static const UINT RP_OBJ_SELCHANGED;
	static const UINT RP_FLOWSTATE_CHANGED;

/*
	Параметры сообщений:
	RP_OBJ_BEFOREDELETE:
		from    - удаляемый объект,
		wParam  - не используется,
		lParam  - не используется
	RP_OBJ_NAMECHANGED:
		from    - объект, у которого сменилось имя,
		wParam  - не используется,
		lParam  - не используется
	RP_OBJ_SELCHANGED:
		from    - объект, у которого изменилось выделение,
		wParam  - новое значение (true/false),
		lParam  - не используется
	RP_FLOWSTATE_CHANGED:
		from    - указатель но проект,
		wParam  - новое состояние проекта,
		lParam  - не используется
*/

};

}

#endif // RDO_PROCESS_MESSAGES_H
