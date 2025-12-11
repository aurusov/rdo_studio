// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/format.hpp>
#include <boost/property_tree/xml_parser.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/common/rdocommon.h"
#include "utils/src/file/rdofile.h"
#include "utils/src/locale/rdolocale.h"

#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdoparser_rdo.h"
#include "converter/smr2rdox/rdofun.h"
#include "converter/smr2rdox/rdorss.h"
#include "converter/smr2rdox/rdo_common/model_objects_convertor.h"
#include "converter/smr2rdox/update/update_i.h"
#include "converter/smr2rdox/update/update.h"
#include "repository/rdorepository.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- Converter
// --------------------------------------------------------------------------------
Converter::ParserList Converter::s_parserStack;

#define DECLARE_PARSER_OBJECT_CONTAINER_NONAME(NAME) \
void Converter::insert##NAME(LPRDO##NAME value) \
{ \
    howIsIt<LPRDO##NAME>(); \
    m_all##NAME.push_back(value); \
}

#define DECLARE_PARSER_OBJECT_CONTAINER(NAME) \
DECLARE_PARSER_OBJECT_CONTAINER_NONAME(NAME) \
const LPRDO##NAME Converter::find##NAME(const std::string& name) const \
{ \
    NAME##List::const_iterator it = std::find_if(m_all##NAME.begin(), m_all##NAME.end(), compareName<RDO##NAME>(name)); \
    return it != m_all##NAME.end() ? *it : LPRDO##NAME(NULL); \
} \
bool Converter::remove##NAME(const LPRDO##NAME item) \
{ \
    NAME##List::iterator it = std::find(m_all##NAME.begin(), m_all##NAME.end(), item); \
    if (it == m_all##NAME.end()) \
        return false; \
    m_all##NAME.erase(it); \
    return true; \
}

DECLARE_PARSER_OBJECT_CONTAINER(PATPattern     );
DECLARE_PARSER_OBJECT_CONTAINER(RTPResType     );
DECLARE_PARSER_OBJECT_CONTAINER(RSSResource    );
DECLARE_PARSER_OBJECT_CONTAINER(OPROperation   );
DECLARE_PARSER_OBJECT_CONTAINER(FRMFrame       );
DECLARE_PARSER_OBJECT_CONTAINER(FUNConstant    );
DECLARE_PARSER_OBJECT_CONTAINER(FUNFunction    );
DECLARE_PARSER_OBJECT_CONTAINER(FUNSequence    );
DECLARE_PARSER_OBJECT_CONTAINER(DPTSearch      );
DECLARE_PARSER_OBJECT_CONTAINER(DPTSome        );
DECLARE_PARSER_OBJECT_CONTAINER(DPTPrior       );
DECLARE_PARSER_OBJECT_CONTAINER(DPTFreeActivity);
DECLARE_PARSER_OBJECT_CONTAINER(PMDResult      );

DECLARE_PARSER_OBJECT_CONTAINER_NONAME(FUNGroup   );
DECLARE_PARSER_OBJECT_CONTAINER_NONAME(DPTFree    );
DECLARE_PARSER_OBJECT_CONTAINER_NONAME(PROCProcess);
DECLARE_PARSER_OBJECT_CONTAINER_NONAME(Operations );

rdo::converter::smr2rdox::FileTypeIn Converter::getFileToParse()
{
    return !s_parserStack.empty() && s_parserStack.back()->m_pParserItem
            ? s_parserStack.back()->m_pParserItem->m_type
            : rdo::converter::smr2rdox::FileTypeIn::PAT;
}

std::size_t Converter::lexer_loc_line()
{
    return !s_parserStack.empty() && s_parserStack.back()->m_pParserItem
            ? s_parserStack.back()->m_pParserItem->lexer_loc_line()
            : ~0;
}

std::size_t Converter::lexer_loc_pos()
{
    return !s_parserStack.empty() && s_parserStack.back()->m_pParserItem
            ? s_parserStack.back()->m_pParserItem->lexer_loc_pos()
            : 0;
}

Converter* Converter::s_converter()
{
    return !s_parserStack.empty()
            ? s_parserStack.back()
            : NULL;
}

Converter::Converter()
    : m_pParserItem         (NULL )
    , m_have_kw_Resources   (false)
    , m_have_kw_ResourcesEnd(false)
    , m_pattern             (false)
{
    s_parserStack.push_back(this);

    m_pRuntime = rdo::Factory<rdo::runtime::RDORuntime>::create(new rdo::runtime::Error());
    ASSERT(m_pRuntime);
    m_pRuntime->memory_insert(sizeof(Converter));
    m_pRuntime->init();

    m_pDocument = rdo::Factory<Document>::create();
    ASSERT(m_pDocument);
}

Converter::~Converter()
{
    m_pRuntime->deinit();
    m_pRuntime = NULL;

    m_movementObjectList.clear();
    s_parserStack.remove(this);
}

void Converter::insertDocUpdate(const LPDocUpdate& pDocUpdate)
{
    ASSERT(m_pParserItem);
    if (m_pParserItem->m_parser_fun != cnv_smr_file_parse)
    {
        m_pDocument->insertUpdate(pDocUpdate);
    }
}

bool Converter::isCurrentDPTSearch()
{
    return getLastDPTSearch() && !getLastDPTSearch()->closed() ? true : false;
}

bool Converter::isCurrentDPTPrior()
{
    return getLastDPTPrior() ? true : false;
}

void Converter::insertChanges(const std::string& name, const std::string& value)
{
    m_changes.push_back(ChangesData(name, value));
}

std::string Converter::getChanges() const
{
    std::stringstream stream;
    stream << "$Changes" << std::endl;
    std::size_t changes_max_length = 0;
    for (const auto& change: m_changes)
    {
        if (change.m_name.length() > changes_max_length)
        {
            changes_max_length = change.m_name.length();
        }
    }
    for (const auto& change: m_changes)
    {
        stream << "  " << change.m_name;
        for (std::size_t i = change.m_name.length(); i < changes_max_length; i++)
        {
            stream << " ";
        }
        stream << "  = " << change.m_value << std::endl;
    }
    return stream.str();
}

std::string Converter::getModelStructure()
{
    std::stringstream modelStructure;

    // $Changes
    modelStructure << getChanges();

    // RTP
    modelStructure << std::endl << std::endl << "$Resource_type" << std::endl;
    for (const auto& rtp: m_allRTPResType)
        rtp->writeModelStructure(modelStructure);

    // RSS
    modelStructure << std::endl << "$Resources" << std::endl;
    for (const auto& rss: m_allRSSResource)
        rss->writeModelStructure(modelStructure);

    // PAT
    modelStructure << std::endl << "$Pattern" << std::endl;
    for (const auto& pat: m_allPATPattern)
        pat->writeModelStructure(modelStructure);

    // OPR/DPT
    std::size_t counter = 1;
    modelStructure << std::endl << "$Activities" << std::endl;
    modelStructure << m_pRuntime->writeActivitiesStructure(counter);

    // DPT only
    for (std::size_t i = 0; i < m_allDPTSearch.size(); i++)
    {
        for (std::size_t j = 0; j < m_allDPTSearch.at(i)->getActivities().size(); j++)
        {
            LPRDODPTSearchActivity pSearchActivity = m_allDPTSearch.at(i)->getActivities().at(j);
            ASSERT(pSearchActivity);
            modelStructure << counter++ << " " << pSearchActivity->name() << " " << pSearchActivity->pattern()->getPatternId() << std::endl;
        }
    }

    // PMD
    modelStructure << std::endl << "$Watching" << std::endl;
    std::size_t watching_max_length = 0;
    for (const auto& watching: m_pRuntime->getResult())
    {
        LPITrace          trace     = watching.object_dynamic_cast<ITrace>();
        LPIName           name      = trace.object_dynamic_cast<IName>();
        LPIModelStructure structure = trace.object_dynamic_cast<IModelStructure>();
        if (trace && name && structure)
        {
            if (trace->traceable() && name->name().length() > watching_max_length)
            {
                watching_max_length = name->name().length();
            }
        }
    }
    for (const auto& watching: m_pRuntime->getResult())
    {
        LPITrace          trace     = watching.object_dynamic_cast<ITrace>();
        LPIName           name      = trace.object_dynamic_cast<IName>();
        LPIModelStructure structure = trace.object_dynamic_cast<IModelStructure>();
        if (trace && name && structure)
        {
            if (trace->traceable())
            {
                modelStructure << "  " << name->name();
                for (std::size_t i = name->name().length(); i < watching_max_length + 2; i++)
                    modelStructure << " ";

                structure->writeModelStructure(modelStructure);
            }
        }
    }

    return modelStructure.str();
}

const RDOParserSMRInfo::FileList& RDOParserSMRInfo::getFileList() const
{
    return m_fileList;
}

void RDOParserSMRInfo::insertFileName(rdo::converter::smr2rdox::FileTypeIn type,
                                      const boost::filesystem::path&          modelPath,
                                      const boost::filesystem::path&          modelName,
                                      const boost::filesystem::path&          smrFileName,
                                      const boost::filesystem::path&          nameFromSMR,
                                      const boost::filesystem::path&          fileExt
)
{
    const boost::filesystem::path& fileName = !nameFromSMR.empty() ? nameFromSMR : (!modelName.empty() ? modelName : smrFileName);
    if (fileName.empty())
        return;

    boost::filesystem::path fullFileName = modelPath / fileName;
    fullFileName.replace_extension(fileExt);

    if (rdo::File::exist(fullFileName))
    {
        m_fileList[type] = fullFileName;
    }
}

bool RDOParserSMRInfo::parseSMR(const boost::filesystem::path& smrFullFileName, boost::filesystem::path& modelName)
{
    boost::filesystem::ifstream stream(smrFullFileName);
    if (!stream.is_open())
        return false;

    RDOParserContainer::Iterator it = begin();
    ASSERT(it != end());

    m_pParserItem = it->second;
    it->second->parse(this, stream);
    m_pParserItem = NULL;

#ifdef _DEBUG
    ++it;
    ASSERT(it == end());
#endif

    if (!hasSMR())
        return false;

    boost::filesystem::path smrFilePath, smrFileName, smrFileExt;
    if (!rdo::File::splitpath(smrFullFileName, smrFilePath, smrFileName, smrFileExt))
        return false;

    modelName = getSMR()->getFile("Model_name");

    insertFileName(rdo::converter::smr2rdox::FileTypeIn::PAT, smrFilePath, modelName, smrFileName, modelName,                           "pat");
    insertFileName(rdo::converter::smr2rdox::FileTypeIn::RTP, smrFilePath, modelName, smrFileName, modelName,                           "rtp");
    insertFileName(rdo::converter::smr2rdox::FileTypeIn::RSS, smrFilePath, modelName, smrFileName, getSMR()->getFile("Resource_file"),  "rss");
    insertFileName(rdo::converter::smr2rdox::FileTypeIn::FRM, smrFilePath, modelName, smrFileName, getSMR()->getFile("Frame_file"   ),  "frm");
    insertFileName(rdo::converter::smr2rdox::FileTypeIn::FUN, smrFilePath, modelName, smrFileName, modelName,                           "fun");
    insertFileName(rdo::converter::smr2rdox::FileTypeIn::PMD, smrFilePath, modelName, smrFileName, getSMR()->getFile("Statistic_file"), "pmd");
    insertFileName(rdo::converter::smr2rdox::FileTypeIn::SMR, smrFilePath, modelName, smrFileName, smrFileName,                         "smr");
    insertFileName(rdo::converter::smr2rdox::FileTypeIn::PMV, smrFilePath, modelName, smrFileName, getSMR()->getFile("Results_file"  ), "pmv");
    insertFileName(rdo::converter::smr2rdox::FileTypeIn::TRC, smrFilePath, modelName, smrFileName, getSMR()->getFile("Trace_file"    ), "trc");

    if (!getSMR()->getFile("OprIev_file").empty())
        insertFileName(rdo::converter::smr2rdox::FileTypeIn::OPR, smrFilePath, modelName, smrFileName, getSMR()->getFile("OprIev_file"), "opr");
    else
        insertFileName(rdo::converter::smr2rdox::FileTypeIn::DPT, smrFilePath, modelName, smrFileName, modelName, "dpt");

    return true;
}

RDOParserModel::Result RDOParserModel::convert(const boost::filesystem::path& smrFullFileName, rdo::converter::smr2rdox::RDOSMRFileInfo& info)
{
    info.m_error = true;

    RDOParserSMRInfo::FileList fileList;
    boost::filesystem::path    modelName;
    {
        std::unique_ptr<RDOParserSMRInfo> pSMRParser(new RDOParserSMRInfo());

        try
        {
            if (!pSMRParser->parseSMR(smrFullFileName, modelName))
                return Result::NONE;

            info.m_modelName     = pSMRParser->getSMR()->getFile("Model_name"    );
            info.m_resourceFile  = pSMRParser->getSMR()->getFile("Resource_file" );
            info.m_frameFile     = pSMRParser->getSMR()->getFile("Frame_file"    );
            info.m_statisticFile = pSMRParser->getSMR()->getFile("Statistic_file");
            info.m_resultsFile   = pSMRParser->getSMR()->getFile("Results_file"  );
            info.m_traceFile     = pSMRParser->getSMR()->getFile("Trace_file"    );
            info.m_error         = false;
        }
        catch (const RDOSyntaxException&)
        {
            return Result::NONE;
        }
        catch (const rdo::runtime::RDORuntimeException&)
        {
            return Result::NONE;
        }
        fileList = pSMRParser->getFileList();
        if (fileList.empty())
        {
            return Result::NONE;
        }
    }

    try
    {
        RDOParserContainer::Iterator it = begin();
        while (it != end())
        {
            m_pParserItem = it->second;
            if (m_pParserItem->needStream())
            {
                const auto it = fileList.find(m_pParserItem->m_type);
                if (it != fileList.end())
                {
                    boost::filesystem::ifstream stream(it->second, std::ios::binary);
                    m_pParserItem->parse(this, stream);
                }
            }
            else
            {
                m_pParserItem->parse(this);
            }
            m_pParserItem = NULL;
            ++it;
        }
    }
    catch (const rdo::converter::smr2rdox::RDOSyntaxException&)
    {
        return Result::NONE;
    }
    catch (const rdo::runtime::RDORuntimeException&)
    {
        return Result::NONE;
    }
    catch (...)
    {
        return Result::NONE;
    }

    error().clear();

    boost::filesystem::path fullPath = boost::filesystem::path(fileList.begin()->second).parent_path();

    try
    {
        boost::posix_time::ptime time(boost::posix_time::second_clock::local_time());
        std::stringstream backupDirName;
        backupDirName << boost::format("backup %1$04d-%2$02d-%3$02d %4$02d-%5$02d-%6$02d")
                         % time.date().year ()
                         % time.date().month()
                         % time.date().day  ()
                         % time.time_of_day().hours  ()
                         % time.time_of_day().minutes()
                         % time.time_of_day().seconds();

        boost::filesystem::path backupPath = fullPath / backupDirName.str();

        try
        {
            if (!boost::filesystem::create_directory(backupPath))
            {
                YYLTYPE pos;
                pos.m_last_line = 0;
                pos.m_last_pos  = 0;
                error().error(RDOParserSrcInfo(pos), rdo::format("Ошибка создания backup-директории '%s': уже существует\n", backupPath.string().c_str()));
            }
        }
        catch (const boost::system::error_code& ex)
        {
            std::string message = ex.message();
            if (message.find("boost") == 0)
            {
                const auto pos = message.find(' ');
                if (pos != std::string::npos)
                {
                    message = message.substr(pos + 1);
                }
            }
            YYLTYPE pos;
            pos.m_last_line = 0;
            pos.m_last_pos  = 0;
            error().error(RDOParserSrcInfo(pos), rdo::format("Ошибка создания backup-директории '%s': %s\n", backupPath.string().c_str(), message.c_str()));
        }

        for (auto& path: fileList)
        {
            boost::filesystem::path from(path.second);
            boost::filesystem::path to  (backupPath / from.filename());
            boost::filesystem::rename(from, to);
            path.second = to;
        }
    }
    catch (const rdo::converter::smr2rdox::RDOSyntaxException&)
    {
        return Result::CONVERTER_ERROR;
    }
    catch (const rdo::runtime::RDORuntimeException&)
    {
        return Result::CONVERTER_ERROR;
    }
    catch (...)
    {
        return Result::CONVERTER_ERROR;
    }

    m_pDocument->create(fullPath, modelName);
    RDOParserContainer::Iterator it = begin();
    while (it != end())
    {
        LPRDOParserItem pParserItem = it->second;
        ASSERT(pParserItem);
        if (pParserItem->needStream())
        {
            const auto fileIt = fileList.find(pParserItem->m_type);
            if (fileIt != fileList.end())
            {
                boost::filesystem::ifstream streamIn(fileIt->second, std::ios::binary);
                ASSERT(streamIn.good());

                m_pDocument->init(pParserItem->m_type, streamIn);
            }
        }
        ++it;
    }
    m_pDocument->convert();
    m_pDocument->close();

    boost::filesystem::path rdoxFile(fullPath / modelName);
    rdoxFile.replace_extension(".rdox");
    if (!repository::RDOThreadRepository::createRDOX(rdoxFile))
    {
        return Result::CONVERTER_ERROR;
    }

    if (!rdo::File::trimLeft(m_pDocument->getName(rdo::converter::smr2rdox::FileTypeOut::SMR)))
    {
        return Result::CONVERTER_ERROR;
    }

    return Result::OK;
}

void Converter::checkFunctionName(const RDOParserSrcInfo& src_info)
{
    LPRDOFUNConstant pConstant = findFUNConstant(src_info.src_text());
    if (pConstant)
    {
        error().push_only(src_info, rdo::format("Константа '%s' уже существует", src_info.src_text().c_str()));
//        parser->error("Second appearance of the same constant name: " + *(_cons->getName()));
        error().push_only(pConstant->src_info(), "См. первое определение");
        error().push_done();
    }
    LPRDOFUNSequence pSequence = findFUNSequence(src_info.src_text());
    if (pSequence)
    {
        error().push_only(src_info, rdo::format("Последовательность '%s' уже существует", src_info.src_text().c_str()));
        error().push_only(pSequence->src_info(), "См. первое определение");
        error().push_done();
    }
    LPRDOFUNFunction pFunction = findFUNFunction(src_info.src_text());
    if (pFunction)
    {
        error().push_only(src_info, rdo::format("Функция '%s' уже существует", src_info.src_text().c_str()));
        error().push_only(pFunction->src_info(), "См. первое определение");
        error().push_done();
    }
}

void Converter::checkActivityName(const RDOParserSrcInfo& src_info)
{
    for (const auto& search: getDPTSearchs())
    {
        RDODPTSearch::ActivityList::const_iterator it_search_act = std::find_if(search->getActivities().begin(), search->getActivities().end(), compareName<RDODPTSearchActivity>(src_info.src_text()));
        if (it_search_act != search->getActivities().end())
        {
            error().push_only(src_info, rdo::format("Активность '%s' уже существует", src_info.src_text().c_str()));
            error().push_only((*it_search_act)->src_info(), "См. первое определение");
            error().push_done();
//            error("Activity name: " + *_name + " already defined");
        }
    }
    for (const auto& some: getDPTSomes())
    {
        RDODPTSome::ActivityList::const_iterator it_some_act = std::find_if(some->getActivities().begin(), some->getActivities().end(), compareName<RDODPTSomeActivity>(src_info.src_text()));
        if (it_some_act != some->getActivities().end())
        {
            error().push_only(src_info, rdo::format("Активность '%s' уже существует", src_info.src_text().c_str()));
            error().push_only((*it_some_act)->src_info(), "См. первое определение");
            error().push_done();
        }
    }
    for (const auto& prior: getDPTPriors())
    {
        RDODPTPrior::ActivityList::const_iterator it_prior_act = std::find_if(prior->getActivities().begin(), prior->getActivities().end(), compareName<RDODPTPriorActivity>(src_info.src_text()));
        if (it_prior_act != prior->getActivities().end())
        {
            error().push_only(src_info, rdo::format("Активность '%s' уже существует", src_info.src_text().c_str()));
            error().push_only((*it_prior_act)->src_info(), "См. первое определение");
            error().push_done();
        }
    }
    LPRDODPTFreeActivity pFreeActivity = findDPTFreeActivity(src_info.src_text());
    if (pFreeActivity)
    {
        error().push_only(src_info, rdo::format("Активность '%s' уже существует", src_info.src_text().c_str()));
        error().push_only(pFreeActivity->src_info(), "См. первое определение");
        error().push_done();
    }
    LPRDOOPROperation pOperation = findOPROperation(src_info.src_text());
    if (pOperation)
    {
        error().push_only(src_info, rdo::format("Операция '%s' уже существует", src_info.src_text().c_str()));
        error().push_only(pOperation->src_info(), "См. первое определение");
        error().push_done();
    }
}

void Converter::checkDPTName(const RDOParserSrcInfo& src_info)
{
    if (src_info.src_text().empty())
    {
        // Актуально для операций и свободных блоков активностей
        return;
    }
    DPTSearchList::const_iterator search_it = std::find_if(getDPTSearchs().begin(), getDPTSearchs().end(), compareName<RDODPTSearch>(src_info.src_text()));
    if (search_it != getDPTSearchs().end())
    {
        error().push_only(src_info, rdo::format("Точка '%s' уже существует", src_info.src_text().c_str()));
        error().push_only((*search_it)->src_info(), "См. первое определение");
        error().push_done();
//        error(src_info, "DPT name: " + src_info.src_text() + " already defined");
    }
    DPTSomeList::const_iterator some_it = std::find_if(getDPTSomes().begin(), getDPTSomes().end(), compareName<RDODPTSome>(src_info.src_text()));
    if (some_it != getDPTSomes().end())
    {
        error().push_only(src_info, rdo::format("Точка '%s' уже существует", src_info.src_text().c_str()));
        error().push_only((*some_it)->src_info(), "См. первое определение");
        error().push_done();
    }
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
