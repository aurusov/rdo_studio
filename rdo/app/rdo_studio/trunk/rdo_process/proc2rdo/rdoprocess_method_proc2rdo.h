#ifndef RDO_PROCESS_METHOD_PROC2RDO_H
#define RDO_PROCESS_METHOD_PROC2RDO_H

#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_method.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_creation_RDO_files.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_shape.h"
#include "utils/rdotypes.h"

class RPMethodProc2RDO: public rpMethod::RPMethod, public RPProcessObject
{
private:
	virtual ~RPMethodProc2RDO();

protected:
	void registerObject();

	CToolBar toolbar;
	int btn_generate;
	int btn_generate_setup;
	virtual void buttonCommand( int button_id );
	virtual void buttonUpdate( RPCtrlToolbar::ButtonUpdate& button_update );

public:
	RPMethodProc2RDO( RPObject* _parent );
	static rpMethod::RPMethod* registerMethod();
	rbool checkModelStructure();
	virtual rp::string getVersionDesc() const { return "альфа"; }
	virtual rp::string getDescription() const { return "Переводит квадратики в паттерны"; }
	virtual rp::string getClassName() const { return "RPMethodProc2RDO"; }
	virtual RPObjectFlowChart* makeFlowChart( RPObject* parent );
	virtual void generate();
	double m_generateTime;
	RPCreationRDOFilesMJ* RDOfiles;
	std::list< CString > list_pattern_names; // MJ 7.04.06 хранятся имена всех паттернов для записи в файл *.opr generate() заполняет его
	
	// Блокируем вхождение функции saveToXML и loadFromXML в RPMethod
	// (из-за проектной ошибки - класс RPMethod не должен наследоваться от RPObject)
	virtual void saveToXML  (REF (pugi::xml_node) parentNode) const;
	virtual void loadFromXML(CREF(pugi::xml_node) node);
};

extern RPMethodProc2RDO* proc2rdo;

#endif // RDO_PROCESS_METHOD_PROC2RDO_H
