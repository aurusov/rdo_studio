#ifndef RDO_PROCESS_PROJECT_RDO_PROC_MJ_H
#define RDO_PROCESS_PROJECT_RDO_PROC_MJ_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../rdoprocess_project.h"
#include "rdoprocess_creation_RDO_files_MJ.h"
#include "rdoprocess_shape_MJ.h"

class RPProject_RDO_Proc_MJ: public RPProject, public RPObject_MJ
{
protected:
	void blank_rdo_MJ();

public:
	RPProject_RDO_Proc_MJ();
	virtual ~RPProject_RDO_Proc_MJ();

	virtual rp::string getType() { return "RPProject_RDO_Proc_MJ"; }

	RPCreationRDOFilesMJ* RDOfiles;
	std::list< CString > list_pattern_names; // MJ 7.04.06 хранятся имена всех паттернов для записи в файл *.opr generate() заполняет его

	virtual void generate();
};

#endif // RDO_PROCESS_PROJECT_RDO_PROC_MJ_H
