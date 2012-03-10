/*!
  \copyright (c) RDO-Team, 2006-2012
  \file      rdoprocess_project.h
  \authors   Манжай Игорь (daynin@gmail.com)
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \authors   Шувалов Андрей (dronbas@gmail.com)
  \date      10.02.2006
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_RDO_PROCESS_PROJECT_H_
#define _RDO_STUDIO_MFC_RDO_PROCESS_PROJECT_H_

// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdodebug.h"
#include "app/rdo_studio_mfc/rdo_process/rp_method/rdoprocess_project.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// ---------- RPProjectMFC
// --------------------------------------------------------------------------------
class RPCtrlToolbarMFC;

class RPProjectMFC: public RPProject
{
private:
	std::list< RPCtrlToolbarMFC* > toolbars;

public:
	RPProjectMFC();
	virtual ~RPProjectMFC();
	virtual rp::string getClassName() const { return "RPProjectMFC"; }
	virtual std::ofstream& log() const;
	virtual RPPixmap*      createBitmap( char* xpm[] );
	virtual void makeFlowChartWnd( RPObjectFlowChart* flowobj );

	void open();
	void save();
	virtual void load( rp::RPXMLNode* node );
	// Переопределенная виртуальная функция saveToXML и loadFromXML для RPProjectMFC:
	void saveToXML  (REF (pugi::xml_node) parentNode) const;
	void loadFromXML(CREF(pugi::xml_node) node);
};

#endif // _RDO_STUDIO_MFC_RDO_PROCESS_PROJECT_H_
