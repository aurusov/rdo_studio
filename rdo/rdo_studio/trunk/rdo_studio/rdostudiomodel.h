/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdostudiomodel.h
 * author   : Урусов Андрей
 * date     : 
 * bref     : 
 * indent   : 4T
 */

#ifndef _RDOSTUDIOMODEL_H_
#define _RDOSTUDIOMODEL_H_

#if _MSC_VER > 1000
#pragma once
#endif

// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_studio/rdostudioframemanager.h"
#include "rdo_studio/rdostudiomodeldoc.h"
#include "rdo_studio/rdostudiomodelview.h"
#include "rdo_studio/rdostudioplugins.h"
#include "rdo_lib/rdo_simulator/rdosimwin.h"
#include "rdo_kernel/rdothread.h"
// ===============================================================================

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
friend class RDOStudioPlugins;

private:
	PTR(CMultiDocTemplate) modelDocTemplate;
	RDOStudioFrameManager frameManager;

	int                       useTemplate;
	rbool                     autoDeleteDoc;
	rbool                     showCanNotCloseModelMessage;

	rbool                     GUI_HAS_MODEL;
	rbool                     GUI_CAN_RUN;
	rbool                     GUI_IS_RUNING;
	rbool                     GUI_ACTION_NEW;
	rbool                     GUI_ACTION_OPEN;
	rbool                     GUI_ACTION_SAVE;
	rbool                     GUI_ACTION_CLOSE;
	rbool                     GUI_ACTION_BUILD;
	rbool                     GUI_ACTION_RUN;

	SYSTEMTIME                time_start;

	mutable rbool             openError;
	mutable rbool             smrEmptyError;
	mutable rbool             modelClosed;

	rbool                     frmDescribed;
	double                    timeNow;
	double                    speed;
	double                    showRate;
	rbool                     tempPause;
	rdoRuntime::RunTimeMode   runtimeMode;
	rdoRuntime::RunTimeMode   runtimeMode_prev;
	rdoSimulator::RDOExitCode exitCode;
	mutable rbool             prevModify;

	void  updateFrmDescribed      ();
	void  newModelFromRepository  ();
	void  openModelFromRepository ();
	void  saveModelToRepository   ();
	void  closeModelFromRepository();
	rbool canCloseModel           ();
	void  afterModelStart         ();

	PTR(RDOStudioModelDoc) getModelDoc() const
	{
		POSITION pos = modelDocTemplate->GetFirstDocPosition();
		return pos ? static_cast<PTR(RDOStudioModelDoc)>(modelDocTemplate->GetNextDoc(pos)) : NULL;
	}

	struct TemplateData
	{
		ruint m_resID;
		int   m_position;

		TemplateData()
			: m_resID   (~0)
			, m_position(~0)
		{}
		TemplateData(CREF(TemplateData) copy)
			: m_resID   (copy.m_resID   )
			, m_position(copy.m_position)
		{}
		TemplateData(ruint resID, int position)
			: m_resID   (resID   )
			, m_position(position)
		{}
	};
	typedef  std::map<rdoModelObjects::RDOFileType, TemplateData>  TemplateDataList;
	typedef  std::map<int, TemplateDataList>                       TemplateList;
	TemplateList m_modelTemplates;

	void show_result();

protected:
	virtual void proc(REF(RDOThread::RDOMessageInfo) msg);

public:
	RDOStudioModel();
	virtual ~RDOStudioModel();

	rbool newModel      (tstring _model_name = _T(""), tstring _model_path = _T(""), const int _useTemplate = -1);
	rbool openModel     (CREF(tstring) modelName = _T("")) const;
	rbool saveModel     () const;
	void  saveAsModel   () const;
	rbool closeModel    () const;
	rbool buildModel    () const;
	rbool runModel      ();
	rbool stopModel     () const;
	void  update        ();
	void  setGUIPause   ();
	void  setGUIContinue();

	tstring getName() const
	{
		PTR(RDOStudioModelDoc) pDoc = getModelDoc();
		return pDoc ? pDoc->getName() : _T("");
	}
	void    setName    (CREF(tstring) str);
	tstring getFullName() const;

	rbool   isModify   () const
	{
		PTR(RDOStudioModelDoc) pDoc = getModelDoc();
		rbool result = pDoc ? pDoc->isModify() : false;
		if (prevModify != result)
		{
			prevModify = result;
			if (plugins)
			{
				plugins->pluginProc(rdoPlugin::PM_MODEL_MODIFY);
			}
		}
		return result;
	}
	rbool  canNew        () const { return ((hasModel() && GUI_CAN_RUN) || !hasModel()) && GUI_ACTION_NEW;   }
	rbool  canOpen       () const { return ((hasModel() && GUI_CAN_RUN) || !hasModel()) && GUI_ACTION_OPEN;  }
	rbool  canSave       () const { return isModify()                                   && GUI_ACTION_SAVE;  }
	rbool  canClose      () const { return hasModel() && !isRunning()                   && GUI_ACTION_CLOSE; }
	rbool  canBuild      () const { return hasModel() && GUI_CAN_RUN                    && GUI_ACTION_BUILD; }
	rbool  canRun        () const { return hasModel() && GUI_CAN_RUN                    && GUI_ACTION_RUN;   }
	rbool  isRunning     () const { return GUI_IS_RUNING;                                                    }
	rbool  isFrmDescribed() const { return frmDescribed;                                                     }
	double getTimeNow    () const { return timeNow;                                                          }

	rdoSimulator::RDOExitCode getExitCode   () const { return exitCode;    }
	rdoRuntime::RunTimeMode   getRuntimeMode() const { return runtimeMode; }
	void    setRuntimeMode       (const rdoRuntime::RunTimeMode value);
	tstring getLastBreakPointName();
	double  getSpeed             () const            { return speed;       }
	void    setSpeed             (double persent);
	double  getShowRate          ()                  { return showRate;    }
	void    setShowRate          (double value);

	void       showNextFrame   ()                { frameManager.showNextFrame();                    }
	void       showPrevFrame   ()                { frameManager.showPrevFrame();                    }
	rbool      canShowNextFrame() const          { return frameManager.canShowNextFrame();          }
	rbool      canShowPrevFrame() const          { return frameManager.canShowPrevFrame();          }
	int        getFrameCount   () const          { return frameManager.count();                     }
	CPTR(char) getFrameName    (int index) const { return frameManager.getFrameName(index).c_str(); }
	void       showFrame       (int index)       { frameManager.showFrame(index);                   }
	void       closeAllFrame   ()                { frameManager.closeAll();                         }
	rbool      hasModel        () const          { return GUI_HAS_MODEL;                            }

	PTR(rdoEditor::RDOEditorTabCtrl) getTab() const
	{
		PTR(RDOStudioModelDoc) pDoc = getModelDoc();
		if (pDoc)
		{
			PTR(RDOStudioModelView) pView = pDoc->getView();
			if (pView)
			{
				return pView->tab;
			}
		}
		return NULL;
	}

	void  updateStyleOfAllModel() const;
	rbool isPrevModelClosed    () const { return modelClosed; }
};

// ----------------------------------------------------------------------------
extern PTR(RDOStudioModel) model;

#endif //! _RDOSTUDIOMODEL_H_
