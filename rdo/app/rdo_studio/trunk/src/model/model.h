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
#include <boost/optional.hpp>
#include <QtCore/qobject.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
#include "kernel/rdothread.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_studio_mfc/src/frame/manager.h"
#include "app/rdo_studio_mfc/src/model/view.h"
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

class RDOStudioModel
	: public QObject
	, public RDOThreadGUI
	, public IInit
{
Q_OBJECT

friend class RDOThreadStudioGUI;

private:
	enum BuildState
	{
		BS_UNDEFINED,
		BS_COMPLETE,
		BS_ERROR
	};

	RDOStudioFrameManager                  m_frameManager;
	boost::optional<ruint>                 m_templateIndex;
	rbool                                  m_GUI_HAS_MODEL;
	rbool                                  m_GUI_CAN_RUN;
	rbool                                  m_GUI_IS_RUNNING;

	void setHasModel  (rbool value);
	void setCanRun    (rbool value);
	void setIsRunning (rbool value);
	void updateActions();

	rbool  isModify  () const;
	rbool  canNew    () const;
	rbool  canOpen   () const;
	rbool  canSave   () const;
	rbool  canClose  () const;
	rbool  canBuild  () const;
	rbool  canRun    () const;

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
	rdo::simulation::report::RDOExitCode   m_exitCode;
	mutable rbool                          m_modify;
	RDOStudioModelView*                    m_pModelView;
	RPViewQt*                              m_pModelProcView;
	tstring                                m_name;

	rbool newModel      (CREF(tstring) modelName, CREF(tstring) modelPath, ruint templateIndex);
	rbool saveModel     () const;
	void  saveToXML     ();
	void  loadFromXML   ();
	rbool buildModel    ();
	rbool stopModel     () const;

	void  updateFrmDescribed      ();
	void  newModelFromRepository  ();
	void  openModelFromRepository ();
	void  saveModelToRepository   ();
	void  closeModelFromRepository();
	rbool canCloseModel           ();
	void  afterModelStart         ();

	double  getSpeed() const;
	void    setSpeed(double persent);

	PTR(RPMethodProc2RDO) getProc2rdo() const;

	struct ModelTemplateItem
	{
		QString                resName;
		boost::optional<ruint> position;

		ModelTemplateItem();
		ModelTemplateItem(CREF(ModelTemplateItem) copy);
		ModelTemplateItem(CREF(QString) resName);
		ModelTemplateItem(CREF(QString) resName, ruint position);
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

	rbool openModel (CREF(tstring) modelName = _T(""));
	rbool runModel  ();
	rbool closeModel();
	void  update    ();

	CREF(tstring) getName    () const;
	void          setName    (CREF(tstring) str);
	tstring       getFullName() const;

	rbool  isRunning     () const;
	rbool  isFrmDescribed() const;
	double getTimeNow    () const;

	rdo::simulation::report::RDOExitCode getExitCode   () const;
	rdo::runtime::RunTimeMode            getRuntimeMode() const;
	void    setRuntimeMode       (const rdo::runtime::RunTimeMode value);
	tstring getLastBreakPointName();
	double  getShowRate          () const;
	void    setShowRate          (double value);

	int        getFrameCount   () const;
	CPTR(char) getFrameName    (int index) const;
	void       showFrame       (int index);
	void       closeAllFrame   ();
	rbool      hasModel        () const;

	PTR(rdoEditor::RDOEditorTabCtrl) getTab()
	{
		if (!m_pModelView)
			return NULL;

		return &m_pModelView->getTab();
	}

	CPTR(rdoEditor::RDOEditorTabCtrl) getTab() const
	{
		if (!m_pModelView)
			return NULL;

		return &m_pModelView->getTab();
	}

	void  updateStyleOfAllModel() const;
	rbool isPrevModelClosed    () const;

	REF(RDOStudioFrameManager) getFrameManager();
	void onChangeFrame(ruint index);

	PTR(RPViewQt) getProcView();

private slots:
	void onFileNew    ();
	void onFileOpen   ();
	void onFileClose  ();
	void onFileSaveAll();

	void onModelBuild();
	void onModelRun  ();
	void onModelStop ();

	void onModelRuntimeMaxSpeed();
	void onModelRuntimeJump    ();
	void onModelRuntimeSync    ();
	void onModelRuntimePause   ();

	void onModelShowRateInc    ();
	void onModelShowRateIncFour();
	void onModelShowRateDecFour();
	void onModelShowRateDec    ();

	void onShowNextFrame();
	void onShowPrevFrame();

	void onModelSpeedValueChanged(int value);

	void onEditModifyChanged(bool value);
};

// --------------------------------------------------------------------------------
extern PTR(RDOStudioModel) model;

#endif // _RDO_STUDIO_MFC_MODEL_MODEL_H_
