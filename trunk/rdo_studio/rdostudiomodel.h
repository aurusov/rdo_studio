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

class RDOStudioModel
{
friend class RDOStudioModelDoc;

private:
	CMultiDocTemplate* modelDocTemplate;
	CMultiDocTemplate* frameDocTemplate;

	std::string name;
	bool useTemplate;
	bool closeWithDocDelete;
	bool canNotCloseByModel;
	bool running;

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

	bool canCloseDocument();

	void parseFrame();

public:
	RDOStudioModel();
	~RDOStudioModel();

	CEvent addNewFrameEvent;
	void addNewFrame() const;

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
	bool isRunning() const { return running; };
	double getModelTime() const;

	RDOStudioModelDoc* getModelDoc() const;
	RDOStudioFrameDoc* getFrameDoc() const;
	void updateModify() const;

	rdoEditor::RDOEditorTabCtrl* getTab() const;
};

// ----------------------------------------------------------------------------
extern RDOStudioModel* model;

#endif // RDOSTUDIOMODEL_H
