#ifndef RDO_PROCESS_METHOD_PROC2RDO_H
#define RDO_PROCESS_METHOD_PROC2RDO_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "rdo_studio/rdo_process/rp_method/rdoprocess_method.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_creation_RDO_files_MJ.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_shape_MJ.h"
#include "rdo_common/rdotypes.h"

class RPMethodProc2RDO_MJ: public rpMethod::RPMethod, public RPObject_MJ
{
private:
	virtual ~RPMethodProc2RDO_MJ();

protected:
	void registerObject();

	CToolBar toolbar;
	int btn_generate;
	int btn_generate_setup;
	virtual void buttonCommand( int button_id );
	virtual void buttonUpdate( RPCtrlToolbar::ButtonUpdate& button_update );

public:
	RPMethodProc2RDO_MJ( RPObject* _parent );
	static rpMethod::RPMethod* registerMethod();
	bool compile();
	virtual rp::string getVersionDesc() const { return "альфа"; }
	virtual rp::string getDescription() const { return "Переводит квадратики в паттерны"; }
	virtual rp::string getClassName() const { return "RPMethodProc2RDO_MJ"; }
	virtual RPObjectFlowChart* makeFlowChart( RPObject* parent );
	virtual void generate();
	double generate_time_MJ;
	RPCreationRDOFilesMJ* RDOfiles;
	std::list< CString > list_pattern_names; // MJ 7.04.06 хранятся имена всех паттернов для записи в файл *.opr generate() заполняет его

	
};

extern RPMethodProc2RDO_MJ* proc2rdo;

#endif // RDO_PROCESS_METHOD_PROC2RDO_H
