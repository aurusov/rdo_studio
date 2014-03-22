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
#include "utils/src/common/warning_disable.h"
#include <boost/optional.hpp>
#include <boost/chrono.hpp>
#include <QObject>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/interface/rdointerface.h"
#include "kernel/rdothread.h"
#include "simulator/service/src/simulator.h"
#include "app/rdo_studio/src/frame/frame_manager.h"
#include "app/rdo_studio/src/application.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace model {

class TabCtrl;
class View;

class Model
	: public QObject
	, public RDOThreadGUI
	, public IInit
{
Q_OBJECT

friend class ThreadStudioGUI;

public:
	Model();
	virtual ~Model();

	bool  openModel (CREF(QString) modelName = QString());
	bool  runModel  ();
	bool  closeModel();
	void  update    ();

	void resetView();

	CREF(QString) getName    () const;
	void          setName    (CREF(QString) str);
	QString       getFullName() const;

	bool   isRunning     () const;
	bool   isFrmDescribed() const;
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
	bool          hasModel        () const;
	bool          isModify        () const;
	bool          isEmpty         () const;

	 PTR(TabCtrl) getTab();
	CPTR(TabCtrl) getTab() const;

	void  updateStyleOfAllModel() const;
	bool  isPrevModelClosed    () const;

	REF(rdo::gui::frame::Manager) getFrameManager();
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

	rdo::gui::frame::Manager  m_frameManager;
	boost::optional<ruint>    m_templateIndex;
	bool                      m_GUI_HAS_MODEL;
	bool                      m_GUI_CAN_RUN;
	bool                      m_GUI_IS_RUNNING;

	void setHasModel  (bool value);
	void setCanRun    (bool value);
	void setIsRunning (bool value);
	void updateActions();

	bool  canNew    () const;
	bool  canOpen   () const;
	bool  canSave   () const;
	bool  canClose  () const;
	bool  canBuild  () const;
	bool  canRun    () const;

	boost::chrono::system_clock::time_point m_timeStart;
	BuildState                              m_buildState;

	mutable bool                            m_openError;
	mutable bool                            m_smrEmptyError;
	mutable bool                            m_modelClosed;

	bool                                    m_frmDescribed;
	double                                  m_timeNow;
	double                                  m_speed;
	double                                  m_showRate;
	rdo::runtime::RunTimeMode               m_runtimeMode;
	rdo::simulation::report::RDOExitCode    m_exitCode;
	mutable bool                            m_modify;
	View*                                   m_pView;
	QString                                 m_name;

	bool newModel  (CREF(QString) modelName, CREF(QString) modelPath, ruint templateIndex);
	bool saveModel () const;
	bool buildModel();
	bool stopModel () const;

	void  updateFrmDescribed      ();
	void  newModelFromRepository  ();
	void  openModelFromRepository ();
	void  saveModelToRepository   ();
	void  closeModelFromRepository();
	bool  canCloseModel           ();
	void  afterModelStart         ();

	double  getSpeed() const;
	void    setSpeed(double persent);

	void updateTimeNow();

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

}}} // namespace rdo::gui::model

// --------------------------------------------------------------------------------
extern rdo::gui::model::Model* g_pModel;

#endif // _RDO_STUDIO_MODEL_MODEL_H_
