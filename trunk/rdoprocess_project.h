#ifndef RDO_PROCESS_PROJECT_MFC_H
#define RDO_PROCESS_PROJECT_MFC_H

#include <rdoprocess_project.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ----------------------------------------------------------------------------
// ---------- RPProjectMFC
// ----------------------------------------------------------------------------
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
	virtual RPCtrlToolbar* createToolBar( const rp::string& caption );
	virtual RPPixmap*      createBitmap( char* xpm[] );
	virtual bool lockResource( rpMethod::RPMethod* method );
	virtual void unlockResource();
	virtual HWND getMainWnd();
	virtual void makeFlowChartWnd( RPObjectFlowChart* flowobj );

	void open();
	void save();
	virtual void load( rp::RPXMLNode* node );
};

#endif // RDO_PROCESS_PROJECT_MFC_H
