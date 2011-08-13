/******************************************************************************//**
 * @copyright (c) RDO-Team, 2011
 * @file      rdobase.inl
 * @authors   Ћущан ƒмитрий
 * @date      06.08.2011
 * @brief     ”правление симул€тором
 * @indent    4T
 *********************************************************************************/

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** RDOSimulatorBase
// ********************************************************************************
inline RDOSimulatorBase::~RDOSimulatorBase()
{}

inline void RDOSimulatorBase::setStartTime(double value)
{
	m_startTime = value;
}

inline double RDOSimulatorBase::getCurrentTime() const
{
	return m_currentTime;
}

inline RunTimeMode RDOSimulatorBase::getMode() const
{
	return m_mode;
}

inline double RDOSimulatorBase::getSpeed() const
{
	return m_speed;
}

inline double RDOSimulatorBase::getShowRate() const
{
	return m_showRate;
}

inline void RDOSimulatorBase::inc_cnt_events()
{
	++m_cnt_events;
}

inline void RDOSimulatorBase::inc_cnt_choice_from()
{
	++m_cnt_choice_from;
}

inline ruint RDOSimulatorBase::get_cnt_events()
{
	return m_cnt_events;
}

inline ruint RDOSimulatorBase::get_cnt_choice_from()
{
	return m_cnt_choice_from;
}

inline ruint RDOSimulatorBase::getMSec(CREF(SYSTEMTIME) systime)
{
	return systime.wMilliseconds + systime.wSecond * 1000 + systime.wMinute * 1000 * 60 + systime.wHour * 1000 * 60 * 60;
}

inline void RDOSimulatorBase::setCurrentTime(double value)
{
	m_currentTime = value;
}

inline void RDOSimulatorBase::onNewTimeNow()
{}

// ********************************************************************************
// ******************** RDOSimulatorBase::BOPlanned
// ********************************************************************************
inline RDOSimulatorBase::BOPlanned::BOPlanned()
	: m_opr  (NULL)
	, m_param(NULL)
{}

inline RDOSimulatorBase::BOPlanned::BOPlanned(CREF(BOPlanned) copy)
	: m_opr  (copy.m_opr  )
	, m_param(copy.m_param)
{}

inline RDOSimulatorBase::BOPlanned::BOPlanned(LPIBaseOperation opr, PTR(void) param)
	: m_opr  (opr  )
	, m_param(param)
{}

CLOSE_RDO_RUNTIME_NAMESPACE
