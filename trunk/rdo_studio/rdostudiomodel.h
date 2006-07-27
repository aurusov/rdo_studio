#ifndef RDOSTUDIOMODEL_H
#define RDOSTUDIOMODEL_H

#if _MSC_VER > 1000
#pragma once
#endif

#include "rdostudioframemanager.h"
#include "rdostudiomodeldoc.h"
#include "rdostudiomodelview.h"
#include "rdostudioplugins.h"
#include <rdosimwin.h>
#include <rdothread.h>

// ----------------------------------------------------------------------------
// ---------- RDOStudioModel
// ----------------------------------------------------------------------------
namespace rdoEditor {
	class RDOEditorTabCtrl;
}

class RDOStudioModel: public RDOThreadGUI
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
	bool autoDeleteDoc;
	bool showCanNotCloseModelMessage;

	bool GUI_HAS_MODEL;
	bool GUI_IS_RUNING;

	SYSTEMTIME time_start;

	mutable bool openError;
	mutable bool modelClosed;
	mutable bool saveAsFlag;

	bool frmDescribed;
	double timeNow;
	rdoSimulator::ShowMode showMode;
	void updateFrmDescribed();

	bool prevModify;

	static void modelExitCallback( int exitCode );

	void newModelFromRepository();
	void openModelFromRepository();
	void saveModelToRepository();
	void closeModelFromRepository();

	bool canCloseModel();

	void beforeModelStart();

	RDOStudioModelDoc* getModelDoc() const {
		POSITION pos = modelDocTemplate->GetFirstDocPosition();
		return pos ? static_cast<RDOStudioModelDoc*>(modelDocTemplate->GetNextDoc( pos )) : NULL;
	}

protected:
	virtual void proc( RDOThread::RDOMessageInfo& msg );

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

	void update();

	std::string getName() const {
		RDOStudioModelDoc* doc = getModelDoc();
		return doc ? doc->getName() : "";
	}
	void setName( const std::string& str );

	bool isModify() {
		RDOStudioModelDoc* doc = getModelDoc();
		bool result = doc ? doc->isModify() : false;
		if ( prevModify != result ) {
			prevModify = result;
			plugins->pluginProc( rdoPlugin::PM_MODEL_MODIFY );
		}
		return result;
	}
	bool isRunning() const                            { return GUI_IS_RUNING; }
	double getTimeNow() const                         { return timeNow;       }

	bool isFrmDescribed() const                       { return frmDescribed;  }
	rdoSimulator::ShowMode getShowMode() const        { return showMode;      }
	void setShowMode( const rdoSimulator::ShowMode value );
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

	bool hasModel() const                       { return GUI_HAS_MODEL;                              }

	rdoEditor::RDOEditorTabCtrl* getTab() const {
		RDOStudioModelDoc* doc = getModelDoc();
		if ( doc ) {
			RDOStudioModelView* view = doc->getView();
			if ( view ) {
				return view->tab;
			}
		}
		return NULL;
	}

	void updateStyleOfAllModel() const;

	bool isPrevModelClosed() const { return modelClosed; }
};

// ----------------------------------------------------------------------------
extern RDOStudioModel* model;

#endif // RDOSTUDIOMODEL_H
