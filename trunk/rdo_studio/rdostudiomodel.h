#ifndef RDOSTUDIOMODEL_H
#define RDOSTUDIOMODEL_H

#if _MSC_VER > 1000
#pragma once
#endif

// ----------------------------------------------------------------------------
// ---------- RDOStudioModel
// ----------------------------------------------------------------------------
class RDOStudioModelDoc;
class RDOStudioFrameDoc;
namespace rdoEditor {
	class RDOEditorTabCtrl;
}

static const UINT RDO_ADDNEWFRAME_MSG = WM_USER + 111;

class RDOStudioModel
{
friend class RDOStudioModelDoc;
friend class RDOStudioApp;

private:
	CMultiDocTemplate* modelDocTemplate;
	CMultiDocTemplate* frameDocTemplate;

	bool useTemplate;
	bool closeWithDocDelete;
	bool showCanNotCloseModelMessage;

	CEvent addNewFrameEvent;
	void addNewFrame();

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

	static void showFrameNotify();

	void newModelFromRepository();
	void openModelFromRepository();
	void saveModelToRepository();
	void closeModelFromRepository();
	void canNotCloseModelFromRepository() const;

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

	void runModel() const;
	void stopModel() const;

	std::string getName() const;
	void setName( const std::string& str );

	bool isModify() const;
	bool isRunning() const;
	double getModelTime() const;

	RDOStudioModelDoc* getModelDoc() const;
	RDOStudioFrameDoc* getFrameDoc() const;
	void updateModify() const;

	rdoEditor::RDOEditorTabCtrl* getTab() const;
};

// ----------------------------------------------------------------------------
extern RDOStudioModel* model;

#endif // RDOSTUDIOMODEL_H
