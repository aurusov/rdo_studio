#ifndef RDOSTUDIOMODEL_H
#define RDOSTUDIOMODEL_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdostudioframemanager.h"

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
friend class RDOStudioApp;
friend class RDOStudioFramesTreeCtrl;

private:
	CMultiDocTemplate* modelDocTemplate;
	RDOStudioFrameManager frameManager;

	bool useTemplate;
	bool closeWithDocDelete;
	bool showCanNotCloseModelMessage;

	static void newModelNotify();
	static void openModelNotify();
	static void saveModelNotify();
	static bool canCloseModelNotify();
	static void closeModelNotify();
	static void canNotCloseModelNotify();

	static void runModelNotify();
	static void stopModelNotify();
	static void parseErrorModelNotify();
	static void buildNotify( std::string str );
	static void debugNotify( std::string str );

	static void parseSuccessNotify();
	static void showFrameNotify();

	void newModelFromRepository();
	void openModelFromRepository();
	void saveModelToRepository();
	void closeModelFromRepository();
	void canNotCloseModelFromRepository() const;

	void parseSuccess();
	void parseFrame();

public:
	RDOStudioModel();
	~RDOStudioModel();

	void newModel( const bool _useTemplate = false );
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
	double getModelTime() const;

	RDOStudioModelDoc* getModelDoc() const;
	void updateModify() const;

	rdoEditor::RDOEditorTabCtrl* getTab() const;

	void updateStyleOfAllModel() const;
};

// ----------------------------------------------------------------------------
extern RDOStudioModel* model;

#endif // RDOSTUDIOMODEL_H
