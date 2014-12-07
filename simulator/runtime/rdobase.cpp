// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ---------------------------------------------------------------------------- PCH
#include "simulator/runtime/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <limits>
#ifdef COMPILER_GCC
    #include <float.h>
#endif // COMPILER_GCC
#include "utils/src/common/warning_disable.h"
#include <boost/thread.hpp>
#include "utils/src/common/warning_enable.h"
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/runtime/rdobase.h"
#include "simulator/runtime/calc/operation/operation_type.h"
// --------------------------------------------------------------------------------

#ifdef COMPILER_VISUAL_STUDIO
    #pragma warning(disable : 4786)
#endif

OPEN_RDO_RUNTIME_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOSimulatorBase
// --------------------------------------------------------------------------------
RDOSimulatorBase::RDOSimulatorBase()
    : m_checkOperation    (true        )
    , m_startTime         (0           )
    , m_currentTime       (0           )
    , m_nextTime          (0           )
    , m_mode              (RTM_MaxSpeed)
    , m_speed             (1           )
    , m_speed_range_max   (500000      )
    , m_next_delay_count  (0           )
    , m_next_delay_current(0           )
    , m_showRate          (60          )
    , m_msec_wait         (0           )
    , m_msec_prev         (0           )
    , m_cnt_events        (0           )
    , m_cnt_choice_from   (0           )
{}

RDOSimulatorBase::~RDOSimulatorBase()
{}

void RDOSimulatorBase::setStartTime(double value)
{
    m_startTime = value;
}

double RDOSimulatorBase::getCurrentTime() const
{
    return m_currentTime;
}

RunTimeMode RDOSimulatorBase::getMode() const
{
    return m_mode;
}

double RDOSimulatorBase::getSpeed() const
{
    return m_speed;
}

double RDOSimulatorBase::getShowRate() const
{
    return m_showRate;
}

void RDOSimulatorBase::inc_cnt_events()
{
    ++m_cnt_events;
}

void RDOSimulatorBase::inc_cnt_choice_from()
{
    ++m_cnt_choice_from;
}

std::size_t RDOSimulatorBase::get_cnt_events()
{
    return m_cnt_events;
}

std::size_t RDOSimulatorBase::get_cnt_choice_from()
{
    return m_cnt_choice_from;
}

std::size_t RDOSimulatorBase::getMSec(const boost::posix_time::ptime& ptime)
{
    boost::posix_time::ptime startTime = boost::posix_time::time_from_string("1970-01-01 00:00:00.000");
    return (std::size_t)( ptime - startTime ).total_milliseconds();
}

void RDOSimulatorBase::setCurrentTime(double value)
{
    m_currentTime = value;
}

void RDOSimulatorBase::onNewTimeNow()
{}

std::size_t RDOSimulatorBase::get_cnt_calc_arithm() const
{
    return OperatorType::getCalcCounter<OperatorType::OT_ARITHM>();
}

std::size_t RDOSimulatorBase::get_cnt_calc_logic() const
{
    return OperatorType::getCalcCounter<OperatorType::OT_LOGIC>();
}

void RDOSimulatorBase::rdoInit()
{
    m_currentTime    = m_startTime;
    m_nextTime       = m_currentTime;
    m_checkOperation = true;
    onInit();
    OperatorType::getCalcCounter<OperatorType::OT_ARITHM>() = 0;
    OperatorType::getCalcCounter<OperatorType::OT_LOGIC> () = 0;

    if (m_timePoints.find(m_currentTime) == m_timePoints.end())
        m_timePoints[m_currentTime] = BOPlannedList();
    preProcess();

    m_speed              = 1;
    m_next_delay_count   = 0;
    m_next_delay_current = 0;
    m_showRate           = 60;
    m_msec_wait          = 0;

    onNewTimeNow();
}

bool RDOSimulatorBase::rdoNext()
{
    if (m_mode == RTM_Pause || m_mode == RTM_BreakPoint)
    {
        boost::this_thread::sleep(boost::posix_time::milliseconds(1));
        return true;
    }
    // Если нажата клавиша или активная область, то задержки надо проскачить
    bool keyboard = isKeyDown();
    if (!keyboard)
    {
        // Задержка общей скорости моделирования
        // Это mode == RTM_Jump || mode == RTM_Sync
        if (m_mode != RTM_MaxSpeed && m_next_delay_count)
        {
            ++m_next_delay_current;
            if (m_next_delay_current < m_next_delay_count) return true;
            m_next_delay_current = 0;
        }
        // Задержка синхронной скорости моделирования (длительность операций)
        // Тут не надо проверять mode == RTM_Sync, т.к. это уже заложено в msec_wait,
        // который сбрасывается в setMode и не изменяется далее.
        if (m_msec_wait > 1)
        {
            boost::posix_time::ptime systime_current = boost::posix_time::microsec_clock::local_time();
            std::size_t msec_curr = getMSec(systime_current);
            std::size_t msec_delta;
            // Милисекунды считаются с учетом часов, но при смене суток часы сбрасываются на ноль,
            // и текущее время в милисекундах становится меньше предыдущего. Учитываем этот момент
            // через ветку ELSE. Теперь система сможет учесть переход на один день вперед между
            // двумя соседники моментами времени, но не сможет учесть на два и более дня. Это
            // является маразматической ситуаций (ждать слудующего события два дня), но запросто
            // может потребоваться в системе мониторинга реального времени, которая работает в
            // автоматическом режиме, обрабатывая информацию с контроллеров. РДО это делать не умеет.
            // Как решение - отказ от синхронной работы в таких системах, и учет только скорости, или
            // переход на работу с календарем дней, месяцев и лет. SYSTEMTIME содержит такую информацию.
            if (msec_curr >= m_msec_prev)
            {
                msec_delta = msec_curr - m_msec_prev;
            }
            else
            {
                msec_delta = UINT_MAX - m_msec_prev + msec_curr;
            }
            if (msec_delta <= m_msec_wait) return true;
            m_msec_wait -= msec_delta;
        }
    }
    // Окончание моделирования - сработало событие конца
    if (endCondition())
    {
        onEndCondition();
        return false;
    }
    if (m_currentTime != m_nextTime)
    {
        m_currentTime = m_nextTime;
        onNewTimeNow();
    }
    // Выполнение операции
    if (doOperation())
    {
        if (breakPoints())
        {
            setMode(RTM_BreakPoint);
        }
        return true;
    }
    else
    {
        // Переход к следующей операции
        if (!m_timePoints.empty())
        {
            BOPlannedMap::iterator begin = m_timePoints.begin();
            double newTime = begin->first;
            const BOPlannedList& list = begin->second;
            if (list.empty())
            {
                m_timePoints.erase(begin);
            }
            if (m_currentTime > newTime)
            {
                newTime = m_currentTime;
            }
            if (m_mode == RTM_Sync)
            {
                m_msec_wait += (newTime - m_nextTime) * 3600.0 * 1000.0 / m_showRate;
                if (m_msec_wait > 0)
                {
                    if (newTime != m_startTime)
                    {
                        if (m_speed > DBL_MIN)
                        {
                            m_msec_wait = m_msec_wait / m_speed;
                        }
                        else
                        {
                            m_msec_wait = m_msec_wait / DBL_MIN;
                        }
                        boost::posix_time::ptime systime_current = boost::posix_time::microsec_clock::local_time();
                        m_msec_prev = getMSec(systime_current);
                    }
                    else
                    {
                        m_msec_wait = 0;
                    }
                }
            }
            m_nextTime = newTime;
            return true;
        }
        else
        {
            // Окончание моделирования - нет больше событий
            onNothingMoreToDo();
            return false;
        }
    }
}

void RDOSimulatorBase::setMode(RunTimeMode _mode)
{
    if (m_mode == RTM_Pause)
    {
        // Чтобы сразу перейти к следующей операции после паузы и чтобы 'не бегало'
        m_next_delay_current = m_next_delay_count;
        m_msec_wait          = 0;
    }
    m_mode = _mode;
    if (m_mode == RTM_MaxSpeed || m_mode == RTM_Jump)
    {
        // Чтобы сразу перейти к следующей операции
        m_next_delay_current = m_next_delay_count;
        m_msec_wait          = 0;
    }
}

void RDOSimulatorBase::setSpeed( double value )
{
    if (value < 0) value = 0;
    if (value > 1) value = 1;
    m_speed = value;
    m_next_delay_count = (std::size_t)((1 - m_speed) * m_speed_range_max);
    // Чтобы сразу перейти к следующей операции
    m_next_delay_current = m_next_delay_count;
    m_msec_wait          = 0;
}

void RDOSimulatorBase::setShowRate( double value )
{
    if (value < DBL_MIN) value = DBL_MIN;
    if (value > DBL_MAX) value = DBL_MAX;
    m_showRate = value;
    // Чтобы сразу перейти к следующей операции
    m_next_delay_current = m_next_delay_count;
    m_msec_wait          = 0;
}

void RDOSimulatorBase::rdoPostProcess()
{
    postProcess();
    m_timePoints.clear();
}

void RDOSimulatorBase::addTimePoint(double timePoint, const LPIBaseOperation& eventID, const EventFunction& eventFunction)
{
    ASSERT(eventID);
    m_timePoints[timePoint].push_back(std::make_pair(eventID, eventFunction));
}

void RDOSimulatorBase::removeTimePoint(const LPIBaseOperation& eventID)
{
    BOPlannedMap::iterator it = m_timePoints.begin();
    while (it != m_timePoints.end())
    {
        BOPlannedList& list = it->second;
        BOPlannedList::iterator item = list.begin();
        while (item != list.end())
        {
            // Удалим операцию из списка запланированных
            if (item->first == eventID)
            {
                item = list.erase(item);
                continue;
            }
            ++item;
        }
        // Если список запланированых пустой, то удалим и его
        if (list.empty())
        {
            m_timePoints.erase(it++);
            continue;
        }
        else
        {
            ++it;
        }
    }
}

CLOSE_RDO_RUNTIME_NAMESPACE
