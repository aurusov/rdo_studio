/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      app/rdo_studio/src/model/model.h
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

#ifndef _RDO_STUDIO_MODEL_MODEL_H_
#define _RDO_STUDIO_MODEL_MODEL_H_

// ----------------------------------------------------------------------- INCLUDES
#include <boost/optional.hpp>
#include <QtCore/qobject.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdointerface.h"
#include "kernel/rdothread.h"
#include "simulator/service/rdosimwin.h"
#include "app/rdo_studio/src/frame/frame_manager.h"
#include "app/rdo_studio/src/application.h"
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

friend class RDOThreadStudioGUI;

public:
	RDOStudioModel();
	virtual ~RDOStudioModel();

	rbool openModel (CREF(QString) modelName = QString());
	rbool runModel  ();
	rbool closeModel();
	void  update    ();

	void resetView();

	CREF(QString) getName    () const;
	void          setName    (CREF(QString) str);
	QString       getFullName() const;

	rbool  isRunning     () const;
	rbool  isFrmDescribed() const;
	double getTimeNow    () const;

	rdo::simulation::report::RDOExitCode getExitCode   () const;
	rdo::runtime::RunTimeMode            getRuntimeMode() const;
	void    setRuntimeMode       (const rdo::runtime::RunTimeMode value);
	QString getLastBreakPointName();
	double  getShowRate          () const;
	void    setShowRate          (double value);

	int           getFrameCount   () const;
	CREF(QString) getFrameName    (int index) const;
	void          showFrame       (int index);
	void          closeAllFrame   ();
	rbool         hasModel        () const;

	 PTR(rdoEditor::RDOEditorTabCtrl) getTab();
	CPTR(rdoEditor::RDOEditorTabCtrl) getTab() const;

	void  updateStyleOfAllModel() const;
	rbool isPrevModelClosed    () const;

	REF(RDOStudioFrameManager) getFrameManager();
	void onChangeFrame(ruint index);

protected:
	virtual void proc(REF(RDOThread::RDOMessageInfo) msg);

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
	QString                                m_name;

	rbool newModel  (CREF(QString) modelName, CREF(QString) modelPath, ruint templateIndex);
	rbool saveModel () const;
	rbool buildModel();
	rbool stopModel () const;

	void  updateFrmDescribed      ();
	void  newModelFromRepository  ();
	void  openModelFromRepository ();
	void  saveModelToRepository   ();
	void  closeModelFromRepository();
	rbool canCloseModel           ();
	void  afterModelStart         ();

	double  getSpeed() const;
	void    setSpeed(double persent);

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

	void createView();

	DECLARE_IInit;

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
extern PTR(RDOStudioModel) g_pModel;

#endif // _RDO_STUDIO_MODEL_MODEL_H_
