#ifndef RDO_PROCESS_MESSAGES_H
#define RDO_PROCESS_MESSAGES_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_object.h"
#include "utils/rdodebug.h"

// --------------------------------------------------------------------------------
// -------------------- RPMessages
// --------------------------------------------------------------------------------
class RPProject;

namespace rp {

class msg: public RPObject
{
friend class RPProject;

public:
	enum Messages {
		RP_OBJ_BEFOREDELETE = 0,   // from - удаляемый объект                        / param не используется
		RP_OBJ_NAMECHANGED,        // from - объект, у которого сменилось имя        / param не используется
		RP_OBJ_SELCHANGED,         // from - объект, у которого изменилось выделение / param = bool*
		RP_FLOWSTATE_CHANGED       // from - указатель но проект                     / param = RPProject::FlowState*
	};

protected:
	msg();
	virtual ~msg();

	typedef std::multimap< UINT, RPObject* > Connected;
	Connected connected;

	virtual void notify( RPObject* from, UINT message, void* param = NULL );

	void sendMessage( RPObject* from, UINT message, void* param = NULL );

public:
	virtual rp::string getClassName() const { return "msg"; }

	void connect( RPObject* to, UINT message );
	void disconnect( RPObject* to );
	void disconnect( RPObject* to, UINT message );

	// Блокируем вхождение в данный класс функции saveToXML и loadFromXML
	// (из-за проектной ошибки - класс msg не должен наследоваться от RPObject).
	virtual void saveToXML  (REF (pugi::xml_node) parentNode) const;
	virtual void loadFromXML(CREF(pugi::xml_node) node);
};

} // namespace rp

#endif // RDO_PROCESS_MESSAGES_H
