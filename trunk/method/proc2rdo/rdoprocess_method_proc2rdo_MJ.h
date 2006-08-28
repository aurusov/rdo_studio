#ifndef RDO_PROCESS_METHOD_PROC2RDO_H
#define RDO_PROCESS_METHOD_PROC2RDO_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <rdoprocess_method.h>
#include "rdoprocess_creation_RDO_files_MJ.h"
#include "rdoprocess_shape_MJ.h"

class RPMethodProc2RDO_MJ: public rpMethod::RPMethod, public RPObject_MJ
{
private:
	virtual ~RPMethodProc2RDO_MJ();

protected:
	CToolBar toolbar;
	void blank_rdo_MJ();
	
	int btn_generate;
	int btn_generate_setup;
	virtual void buttonCommand( int button_id );
	virtual void buttonUpdate( RPCtrlToolbar::ButtonUpdate& button_update );

public:
	RPMethodProc2RDO_MJ( RPObject* _parent );
	virtual rp::string getClassName() const { return "RPMethodProc2RDO_MJ"; }
	void registerObject();
	virtual rp::string getVersionDesc() const { return "альфа"; }
	virtual rp::string getDescription() const { return "Переводит квадратики в паттерны"; }

	virtual RPObjectFlowChart* makeFlowChart( RPObject* parent );

	double generate_time_MJ;
	RPCreationRDOFilesMJ* RDOfiles;
	std::list< CString > list_pattern_names; // MJ 7.04.06 хранятся имена всех паттернов для записи в файл *.opr generate() заполняет его

	virtual void generate();
};

extern RPMethodProc2RDO_MJ* proc2rdo;

extern "C" {
	RP_METHOD_DLL rpMethod::RPMethod* registerMethod( RPObjectFactory* _factory, RPProject* _project );
}

#endif // RDO_PROCESS_METHOD_PROC2RDO_H
