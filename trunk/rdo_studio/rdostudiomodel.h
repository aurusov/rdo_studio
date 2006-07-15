#ifndef RDOSTUDIOMODEL_H
#define RDOSTUDIOMODEL_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdostudioframemanager.h"
#include <rdosimwin.h>
#include <rdothread.h>

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
friend class RDOStudioFrameView;
friend class RDOStudioFrameTreeCtrl;
friend class RDOStudioApp;
friend class RDOThreadStudioGUI;

private:
	CMultiDocTemplate* modelDocTemplate;
	RDOStudioFrameManager frameManager;

	bool useTemplate;
	bool closeWithDocDelete;
	bool showCanNotCloseModelMessage;

	mutable bool openError;
	mutable bool modelClosed;
	mutable bool saveAsFlag;

	bool frmDescribed;
	double modelTime;
	rdosim::ShowMode showMode;
	void updateFrmDescribed();

	bool prevModify;

	static void modelExitCallback( int exitCode );

	void newModelFromRepository();
	void openModelFromRepository();
	void saveModelToRepository();
	void closeModelFromRepository();

	bool canCloseModel();

	void beforeModelStart();
	void showFrame();
	void stopModelFromSimulator();

	RDOStudioModelDoc* getModelDoc() const;

protected:
	virtual void procGUI( RDOThread::RDOMessageInfo& msg );

public:
	RDOStudioModel();
	virtual ~RDOStudioModel();

	void newModel( const bool useTemplate = false );
	bool openModel( const std::string& modelName = "" ) const;
	bool saveModel() const;
	void saveAsModel() const;
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
	rdosim::ShowMode getShowMode() const { return showMode;     };
	void setShowMode( const rdosim::ShowMode value );
	double getShowRate() const;
	void setShowRate( const double value ) const;

	void showNextFrame()                        { frameManager.showNextFrame();                      }
	void showPrevFrame()                        { frameManager.showPrevFrame();                      }
	bool canShowNextFrame() const               { return frameManager.canShowNextFrame();            }
	bool canShowPrevFrame() const               { return frameManager.canShowPrevFrame();            }
	int  getFrameCount() const                  { return frameManager.count();                       }
	const char* getFrameName( int index ) const { return frameManager.getFrameName( index ).c_str(); }
	void showFrame( int index )                 { frameManager.showFrame( index );                   }
	void closeAllFrame()                        { frameManager.closeAll();                           }

	bool hasModel() const { return getModelDoc() ? true : false; }

	void updateModify() const;

	rdoEditor::RDOEditorTabCtrl* getTab() const;

	void updateStyleOfAllModel() const;

	bool isPrevModelClosed() const { return modelClosed; }
};

// ----------------------------------------------------------------------------
extern RDOStudioModel* model;

#endif // RDOSTUDIOMODEL_H
