#ifndef RDOSTUDIOMODEL_H
#define RDOSTUDIOMODEL_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdostudioframemanager.h"
#include <rdosimwin.h>

// ----------------------------------------------------------------------------
// ---------- RDOStudioModel
// ----------------------------------------------------------------------------
class RDOStudioModelDoc;
namespace rdoEditor {
	class RDOEditorTabCtrl;
}

class RDOStudioModel
{
friend class RDOStudioModelDoc;
friend class RDOStudioMainFrame;
friend class RDOStudioFrameView;
friend class RDOStudioFrameTreeCtrl;

private:
	CMultiDocTemplate* modelDocTemplate;
	RDOStudioFrameManager frameManager;

	bool useTemplate;
	bool closeWithDocDelete;
	bool showCanNotCloseModelMessage;

	bool openError;
	bool modelClosed;

	bool frmDescribed;
	double modelTime;
	RDOSimulatorNS::ShowMode showMode;
	void updateFrmDescribed();

	static void newModelNotify();
	static void openModelNotify();
	static void saveModelNotify();
	static bool canCloseModelNotify();
	static void closeModelNotify();
	static void canNotCloseModelNotify();

	static void beforeModelStartNotify();
	static void afterModelStartNotify();
	static void endExecuteModelNotify();
	static void stopModelNotify();
	static void parseSuccessNotify();
	static void parseErrorNotify();
	static void executeErrorNotify();
	static void showFrameNotify();

	static void buildNotify( std::string str );
	static void debugNotify( std::string str );

	void newModelFromRepository();
	void openModelFromRepository();
	void saveModelToRepository();
	void closeModelFromRepository();
	void canNotCloseModelFromRepository() const;

	void beforeModelStart();
	void showFrame();
	void stopModelFromSimulator();

	RDOStudioModelDoc* getModelDoc() const;

public:
	RDOStudioModel();
	~RDOStudioModel();

	void newModel( const bool useTemplate = false );
	bool openModel( const std::string& modelName = "" ) const;
	bool saveModel() const;
	void saveAsModel() const;
	bool canCloseModel();
	void closeModel() const;

	void buildModel() const;
	void runModel() const;
	void stopModel() const;

	std::string getName() const;
	void setName( const std::string& str );

	bool isModify() const;
	bool isRunning() const;
	double getModelTime() const { return modelTime; };

	bool isFrmDescribed() const                  { return frmDescribed; };
	RDOSimulatorNS::ShowMode getShowMode() const { return showMode;     };
	void setShowMode( const RDOSimulatorNS::ShowMode value );
	double getShowRate() const;
	void setShowRate( const double value ) const;

	bool hasModel() const { return getModelDoc() ? true : false; }

	void updateModify() const;

	rdoEditor::RDOEditorTabCtrl* getTab() const;

	void updateStyleOfAllModel() const;

	bool isPrevModelClosed() const { return modelClosed; }
};

// ----------------------------------------------------------------------------
extern RDOStudioModel* model;

#endif // RDOSTUDIOMODEL_H
