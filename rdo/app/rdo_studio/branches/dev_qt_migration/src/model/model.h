/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio_mfc/src/model/model.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MFC_MODEL_MODEL_H_
#define _RDO_STUDIO_MFC_MODEL_MODEL_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/signal.hpp>
#include <QtCore/qobject.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
#include "kernel/rdothread.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_studio_mfc/src/frame/manager.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_docview.h"
#include "app/rdo_studio_mfc/rdo_process/rdoprocess_project.h"
#include "app/rdo_studio_mfc/rdo_process/proc2rdo/rdoprocess_method_proc2rdo.h"
#include "app/rdo_studio_mfc/src/application.h"
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
// -------------------- RDOStudioModel
// --------------------------------------------------------------------------------
namespace rdoEditor {
	class RDOEditorTabCtrl;
}

class RDOStudioModelView;

class RDOStudioModel
	: public QObject
	, public RDOThreadGUI
	, public IInit
{
Q_OBJECT

friend class RDOStudioFrameTreeCtrl;
friend class RDOStudioApp;
friend class RDOThreadStudioGUI;
friend class RDOStudioPlugins;

private:
	enum BuildState
	{
		BS_UNDEFINED,
		BS_COMPLETE,
		BS_ERROR
	};

	RDOStudioFrameManager                  m_frameManager;

	int                                    m_useTemplate;
	rbool                                  m_autoDeleteDoc;
	rbool                                  m_showCanNotCloseModelMessage;

	rbool                                  m_GUI_HAS_MODEL;
	rbool                                  m_GUI_CAN_RUN;
	rbool                                  m_GUI_IS_RUNING;

	SYSTEMTIME                             m_timeStart;
	BuildState                             m_buildState;

	mutable rbool                          m_openError;
	mutable rbool                          m_smrEmptyError;
	mutable rbool                          m_modelClosed;

	rbool                                  m_frmDescribed;
	double                                 m_timeNow;
	double                                 m_speed;
	double                                 m_showRate;
	rbool                                  m_tempPause;
	rdo::runtime::RunTimeMode              m_runtimeMode;
	rdo::runtime::RunTimeMode              m_runtimeModePrev;
	rdo::simulation::report::RDOExitCode   m_exitCode;
	mutable rbool                          m_modify;
	RDOStudioModelView*                    m_pModelView;
	RPViewQt*                              m_pModelProcView;
	tstring                                m_name;

	boost::signal<void (float)>            m_timeNowSignal;

	void  updateFrmDescribed      ();
	void  newModelFromRepository  ();
	void  openModelFromRepository ();
	void  saveModelToRepository   ();
	void  closeModelFromRepository();
	rbool canCloseModel           ();
	void  afterModelStart         ();

	PTR(RPMethodProc2RDO) getProc2rdo() const;

	struct ModelTemplateItem
	{
		ruint m_resID;
		int   m_position;

		ModelTemplateItem();
		ModelTemplateItem(CREF(ModelTemplateItem) copy);
		ModelTemplateItem(ruint resID, int position);
	};
	typedef  std::map<rdoModelObjects::RDOFileType, ModelTemplateItem>  ModelTemplate;
	typedef  std::map<int, ModelTemplate>                               ModelTemplateList;
	ModelTemplateList m_modelTemplates;

	void show_result();

	void createView    ();
	void createProcView();

	DECLARE_IInit;

protected:
	virtual void proc(REF(RDOThread::RDOMessageInfo) msg);

public:
	RDOStudioModel();
	virtual ~RDOStudioModel();

	rbool newModel      (tstring _model_name = _T(""), tstring _model_path = _T(""), const int _useTemplate = -1);
	rbool openModel     (CREF(tstring) modelName = _T(""));
	rbool saveModel     () const;
	void  saveAsModel   () const;
	void  saveToXML     ();
	void  loadFromXML   ();
	rbool closeModel    ();
	rbool buildModel    ();
	rbool runModel      ();
	rbool stopModel     () const;
	void  update        ();
	void  setGUIPause   ();
	void  setGUIContinue();

	void resetView();

	CREF(tstring) getName    () const;
	void          setName    (CREF(tstring) str);
	tstring       getFullName() const;

	rbool  isModify      () const;
	rbool  canNew        () const;
	rbool  canOpen       () const;
	rbool  canSave       () const;
	rbool  canClose      () const;
	rbool  canBuild      () const;
	rbool  canRun        () const;
	rbool  isRunning     () const;
	rbool  isFrmDescribed() const;
	double getTimeNow    () const;

	rdo::simulation::report::RDOExitCode getExitCode   () const;
	rdo::runtime::RunTimeMode            getRuntimeMode() const;
	void    setRuntimeMode       (const rdo::runtime::RunTimeMode value);
	tstring getLastBreakPointName();
	double  getSpeed             () const;
	void    setSpeed             (double persent);
	double  getShowRate          () const;
	void    setShowRate          (double value);

	void       showNextFrame   ();
	void       showPrevFrame   ();
	rbool      canShowNextFrame() const;
	rbool      canShowPrevFrame() const;
	int        getFrameCount   () const;
	CPTR(char) getFrameName    (int index) const;
	void       showFrame       (int index);
	void       closeAllFrame   ();
	rbool      hasModel        () const;

	 PTR(rdoEditor::RDOEditorTabCtrl) getTab();
	CPTR(rdoEditor::RDOEditorTabCtrl) getTab() const;

	void  updateStyleOfAllModel() const;
	rbool isPrevModelClosed    () const;

	rbool saveModified();

	REF(RDOStudioFrameManager) getFrameManager();

	PTR(RPViewQt) getProcView();

private slots:
	void onModelBuild();
	void onModelRun  ();
	void onModelStop ();
};

// --------------------------------------------------------------------------------
extern PTR(RDOStudioModel) model;

#endif // _RDO_STUDIO_MFC_MODEL_MODEL_H_
