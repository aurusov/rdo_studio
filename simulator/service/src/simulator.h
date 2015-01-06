#pragma once

// ----------------------------------------------------------------------- PLATFORM
#include "utils/src/common/platform.h"
// ----------------------------------------------------------------------- INCLUDES
#include <string>
#include <vector>
#include <sstream>

#include <boost/noncopyable.hpp>

#ifdef COMPILER_VISUAL_STUDIO
    #pragma warning(disable : 4786)
    #ifdef RDO_MT
        #include <afxwin.h>
    #else
        #include <windows.h>
    #endif
#endif // COMPILER_VISUAL_STUDIO
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "kernel/rdokernel.h"
#include "kernel/rdothread.h"
#include "simulator/runtime/thread_proxy_i.h"
#include "simulator/runtime/rdo_runtime.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "converter/smr2rdox/rdo_common/model_objects_convertor.h"
// --------------------------------------------------------------------------------

//#define DISABLE_CORBA
//#ifndef DISABLE_CORBA
namespace rdo {
namespace animation {

struct Frame;

}} // namespace rdo::animation

#define DISABLE_CORBA

//#define CORBA_ENABLE
#ifdef CORBA_ENABLE

#ifndef __WIN32__
#define __WIN32__
#endif

#ifndef __x86__
#define __x86__
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif

#ifndef __NT__
#define __NT__
#endif

#ifndef __OSVERSION__
#define __OSVERSION__ 4
#endif

#pragma warning(disable: 4996)
#include <omniORB4/CORBA.h>
#include "simulator/corba/RDOCorba.hh"
#pragma warning(default: 4996)

namespace rdoCorba {

// --------------------------------------------------------------------------------
// -------------------- RDOThreadCorba
// --------------------------------------------------------------------------------
class RDOThreadCorba: public RDOThreadMT
{
public:
    RDOThreadCorba();

private:
    virtual ~RDOThreadCorba()
    {}
    virtual void proc(RDOMessageInfo& msg);
    virtual void idle();
    virtual void start();
    virtual void stop();

    CWinThread* thread_corbaRunThreadFun;
    static std::size_t corbaRunThreadFun(void* pParam);
};

} // namespace rdoCorba

#endif // CORBA_ENABLE

namespace rdo {
namespace service {
namespace simulation {

class RDOThreadSimulator;

}}} // namespace rdo::service::simulation

OPEN_RDO_RUNTIME_NAMESPACE

class RDOResult;

// --------------------------------------------------------------------------------
// -------------------- RDOThreadRunTime
// --------------------------------------------------------------------------------
PREDECLARE_POINTER(RDOThreadRunTime);
class RDOThreadRunTime
    : public rdo::counter_reference
    , public RDOThreadMT
    , public IThreadProxy
{
DECLARE_FACTORY(RDOThreadRunTime);
public:
    bool runtimeError() const;

    struct GetFrame
    {
        rdo::animation::Frame* m_pFrame;
        std::size_t m_number;

        GetFrame(rdo::animation::Frame* pFrame, std::size_t number)
            : m_pFrame(pFrame)
            , m_number(number)
        {}
    };

    struct FrameAreaDown
    {
        std::size_t m_number;
        std::string m_name;

        FrameAreaDown(std::size_t number, const std::string& name)
            : m_number(number)
            , m_name  (name  )
        {}
    };

private:
    rdo::service::simulation::RDOThreadSimulator* m_pSimulator;
    bool m_runtimeError;
    uint64_t m_timeStart;

    RDOThreadRunTime();
    virtual ~RDOThreadRunTime()
    {};

    virtual void proc   (RDOMessageInfo& msg);
    virtual void idle   ();
    virtual void start  ();
    virtual void stop   ();
    virtual void destroy();

    void writeResultsInfo();

    void sendMessage(ThreadID threadID, std::size_t messageID, void* pParam);
};

CLOSE_RDO_RUNTIME_NAMESPACE

class RDOTrace;

namespace rdo {
namespace service {
namespace simulation {

// --------------------------------------------------------------------------------
// -------------------- RDOThreadSimulator
// --------------------------------------------------------------------------------
class RDOThreadSimulator: public RDOThreadMT
{
friend class rdo::runtime::RDOThreadRunTime;
friend class RDORuntimeTracer;
friend class RDOSimResulter;

public:
    struct RTP
    {
        struct Param
        {
            std::string m_name;
        };
        typedef std::vector<Param> ParamList;

        std::string m_name;
        ParamList m_params;
    };

    struct RSS
    {
        std::string m_name;
    };

    struct GetRTP: public std::vector<RTP>
    {};
    struct GetRSS: public std::vector<RSS>
    {};

private:
    rdo::compiler::parser::LPRDOParser m_pParser;
    rdo::runtime::LPRDORuntime m_pRuntime;
    bool m_canTrace;
    rdo::runtime::LPRDOThreadRunTime m_pThreadRuntime;
    rdo::simulation::report::ExitCode m_exitCode;

    void terminateModel();
    void closeModel    ();

    ShowMode m_showMode;
    double   m_showRate;

    std::stringstream m_resultString;
    std::stringstream m_resultInfoString;


#ifdef CORBA_ENABLE

//    void corbaGetRTPcount(::CORBA::Long& rtp_count);
//    void corbaGetRTPParamscount(rdo::compiler::parser::RDOCorba::PARAM_count& params_count);
    void corbaGetRTP(rdo::compiler::parser::RDOCorba::GetRTP_var& my_rtpList);
    void corbaGetRSS(rdo::compiler::parser::RDOCorba::GetRSS_var& my_rssList);

#endif // CORBA_ENABLE

protected:
    virtual ~RDOThreadSimulator();

    virtual void proc(RDOMessageInfo& msg);

    bool parseModel();
    void runModel();
    void stopModel();

    typedef std::vector<rdo::simulation::report::FileMessage> SyntaxMessageList;
    SyntaxMessageList getErrors();

    void codeCompletion();

    void corbaGetRTP(GetRTP* RTPList);
    void corbaGetRSS(GetRSS* RSSList);

public:
    RDOThreadSimulator();

    void parseSMRFileInfo(rdo::converter::smr2rdox::RDOSMRFileInfo& info);

    ShowMode getInitialShowMode   () const;
    int      getInitialFrameNumber() const;
    double   getInitialShowRate   () const;

    struct GetList
    {
        enum class Type
        {
            FRAMES,
            BITMAPS
        };

        typedef std::list<std::string> StringList;

        Type m_type;
        StringList* m_list;

        GetList(Type type, StringList* list)
            : m_type(type)
            , m_list(list)
        {}
    };
};

// --------------------------------------------------------------------------------
// -------------------- RDOThreadCodeComp
// --------------------------------------------------------------------------------
class RDOThreadCodeComp: public RDOThreadMT
{
protected:
    rdo::compiler::parser::LPRDOParser m_pParser;

    virtual ~RDOThreadCodeComp();
    virtual void proc(RDOMessageInfo& msg);

public:
    RDOThreadCodeComp();

    struct GetCodeComp: public boost::noncopyable
    {
        rdo::FileType m_file;
        int m_pos_x;
        int m_pos_y;
        std::string& m_result;

        GetCodeComp(rdo::FileType file, int pos_x, int pos_y, std::string& result)
            : m_file  (file  )
            , m_pos_x (pos_x )
            , m_pos_y (pos_y )
            , m_result(result)
        {}
    };
};

}}} // namespace rdo::service::simulation
