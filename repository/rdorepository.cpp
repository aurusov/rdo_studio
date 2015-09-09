// ---------------------------------------------------------------------------- PCH
// ----------------------------------------------------------------------- INCLUDES
#include <boost/format.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/property_tree/xml_parser.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "repository/rdorepository.h"
#include "utils/src/file/rdofile.h"
#include "utils/src/time/rdotime.h"
#include "utils/src/locale/rdolocale.h"
#include "kernel/rdokernel.h"
#include "simulator/service/src/simulator.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace repository {

// --------------------------------------------------------------------------------
// -------------------- RDOThreadRepository
// --------------------------------------------------------------------------------
RDOThreadRepository::RDOThreadRepository()
    : RDOThreadMT    ("RDOThreadRepository")
    , m_hasModel     (false                )
    , m_realOnlyInDlg(false                )
{
    notifies.push_back(Message::STUDIO_MODEL_NEW                  );
    notifies.push_back(Message::STUDIO_MODEL_OPEN                 );
    notifies.push_back(Message::STUDIO_MODEL_CLOSE                );
    notifies.push_back(Message::STUDIO_MODEL_SAVE                 );
    notifies.push_back(Message::REPOSITORY_MODEL_GET_FILEINFO     );
    notifies.push_back(Message::REPOSITORY_LOAD                   );
    notifies.push_back(Message::REPOSITORY_SAVE                   );
    notifies.push_back(Message::REPOSITORY_LOAD_BINARY            );
    notifies.push_back(Message::REPOSITORY_CREATE_FILE            );
    notifies.push_back(Message::SIMULATOR_MODEL_STOP_OK           );
    notifies.push_back(Message::SIMULATOR_MODEL_STOP_BY_USER      );
    notifies.push_back(Message::SIMULATOR_MODEL_STOP_RUNTIME_ERROR);
    notifies.push_back(Message::RUNTIME_MODEL_START_BEFORE        );
    notifies.push_back(Message::RUNTIME_TRACE_STRING              );

    m_files[rdo::FileType::RDOX].m_extension = "rdox";
    m_files[rdo::FileType::RTP ].m_extension = "rtp";
    m_files[rdo::FileType::RSS ].m_extension = "rss";
    m_files[rdo::FileType::EVN ].m_extension = "evn";
    m_files[rdo::FileType::PAT ].m_extension = "pat";
    m_files[rdo::FileType::DPT ].m_extension = "dpt";
    m_files[rdo::FileType::PRC ].m_extension = "prc";
    m_files[rdo::FileType::PRCX].m_extension = "prcx";
    m_files[rdo::FileType::FRM ].m_extension = "frm";
    m_files[rdo::FileType::FUN ].m_extension = "fun";
    m_files[rdo::FileType::SMR ].m_extension = "smr";
    m_files[rdo::FileType::PMD ].m_extension = "pmd";
    m_files[rdo::FileType::PMV ].m_extension = "pmv";
    m_files[rdo::FileType::TRC ].m_extension = "trc";

//    m_files[rdo::OPR].deleteifempty = true;
//    m_files[rdo::DPT].deleteifempty = true;

    resetModelNames();

    after_constructor();
}

RDOThreadRepository::~RDOThreadRepository()
{
    m_traceFile.close();
}

void RDOThreadRepository::proc(RDOMessageInfo& msg)
{
    switch (msg.message)
    {
        case Message::STUDIO_MODEL_NEW:
        {
            msg.lock();
            NewModel* data = static_cast<NewModel*>(msg.param);
            newModel(data);
            msg.unlock();
            break;
        }
        case Message::STUDIO_MODEL_OPEN:
        {
            msg.lock();
            OpenFile* data = static_cast<OpenFile*>(msg.param);
            data->m_result = openModel(data->m_name);
            msg.unlock();
            break;
        }
        case Message::STUDIO_MODEL_CLOSE:
        {
            closeModel();
            break;
        }
        case Message::STUDIO_MODEL_SAVE:
        {
            bool res = saveModel();
            msg.lock();
            if (msg.param) *static_cast<bool*>(msg.param) = res;
            msg.unlock();
            break;
        }
        case Message::RUNTIME_MODEL_START_BEFORE:
        {
            beforeModelStart();
            break;
        }
        case Message::SIMULATOR_MODEL_STOP_OK           :
        case Message::SIMULATOR_MODEL_STOP_BY_USER      :
        case Message::SIMULATOR_MODEL_STOP_RUNTIME_ERROR:
        {
            stopModel();
            break;
        }
        case Message::RUNTIME_TRACE_STRING:
        {
            msg.lock();
            trace(*static_cast<std::string*>(msg.param));
            msg.unlock();
            break;
        }
        case Message::REPOSITORY_MODEL_GET_FILEINFO:
        {
            msg.lock();
            FileInfo* data = static_cast<FileInfo*>(msg.param);
            data->m_name      = getFileName    (data->m_type);
            data->m_fullName  = getFullFileName(data->m_type);
            data->m_extension = getExtension   (data->m_type);
            data->m_readOnly  = isReadOnly     (data->m_type);
            msg.unlock();
            break;
        }
        case Message::REPOSITORY_LOAD:
        {
            msg.lock();
            FileData* fdata = static_cast<FileData*>(msg.param);
            load(fdata->m_type, fdata->m_stream);
            msg.unlock();
            break;
        }
        case Message::REPOSITORY_SAVE:
        {
            msg.lock();
            FileData* fdata = static_cast<FileData*>(msg.param);
            save(fdata->m_type, fdata->m_stream);
            msg.unlock();
            break;
        }
        case Message::REPOSITORY_LOAD_BINARY:
        {
            msg.lock();
            BinaryFile* data = static_cast<BinaryFile*>(msg.param);
            loadBMP(data->m_name, data->m_stream);
            msg.unlock();
            break;
        }
        case Message::REPOSITORY_CREATE_FILE:
        {
            msg.lock();
            CreateFileInfo* data = static_cast<CreateFileInfo*>(msg.param);
            createFile(data->m_name, data->m_stream);
            msg.unlock();
            break;
        }
        default: break;
    }
}

void RDOThreadRepository::resetModelNames()
{
    for (auto& file: m_files)
        file.second.resetname();
}

RDOThreadRepository::FindModel RDOThreadRepository::updateModelNames()
{
    if (!m_projectName.m_rdox)
    {
        rdo::converter::smr2rdox::RDOSMRFileInfo fileInfo;
        kernel->simulator()->parseSMRFileInfo(fileInfo);
        if (fileInfo.m_error)
        {
            if (fileInfo.m_modelName.empty())
            {
                return FindModel::SMR_EMPTY;
            }
            return FindModel::SMR_ERROR;
        }
    }

    for (auto& file: m_files)
    {
        file.second.m_fileName      = m_modelName;
        file.second.m_mustExist     = false;
        file.second.m_deleteIfEmpty = true;
    }
    return FindModel::OK;
}

void RDOThreadRepository::newModel(const NewModel* const data)
{
    if (canCloseModel())
    {
        realCloseModel();
        if (data)
        {
            boost::filesystem::path path = data->m_path;
            extractName((path / data->m_name).replace_extension(m_files[rdo::FileType::RDOX].m_extension));
            if (!rdo::File::exist(path))
                boost::filesystem::create_directory(path);

            createRDOX();
        }
        else
        {
            m_modelName = "noname";
            m_modelPath = boost::filesystem::path();
        }
        for (auto& file: m_files)
        {
            file.second.m_fileName = m_modelName;
        }
        m_hasModel = true;
        broadcastMessage(Message::REPOSITORY_MODEL_NEW);
    }
    else
    {
        broadcastMessage(Message::REPOSITORY_MODEL_CLOSE_ERROR);
    }
}

bool RDOThreadRepository::openModel(const boost::filesystem::path& modelFileName)
{
    if (canCloseModel())
    {
        realCloseModel();

        bool canOpen = true;
        m_realOnlyInDlg = false;
        if (modelFileName.empty())
        {
            OpenFile data;
            broadcastMessage(Message::REPOSITORY_MODEL_OPEN_GET_NAME, &data, true);
            if (data.m_result)
            {
                m_realOnlyInDlg = data.m_readOnly;
                extractName(data.m_name);
            }
            else
            {
                canOpen = false;
            }
        }
        else
        {
            extractName(modelFileName);
            canOpen = !m_modelName.empty();
        }

        if (canOpen)
        {
            for (auto& file: m_files)
                file.second.m_readOnly = m_realOnlyInDlg;

            boost::filesystem::path rdoxFileName = (m_modelPath / m_modelName).replace_extension(m_files[rdo::FileType::RDOX].m_extension);
            boost::filesystem::path smrFileName  = (m_modelPath / m_modelName).replace_extension(m_files[rdo::FileType::SMR ].m_extension);

            if (rdo::File::exist(rdoxFileName))
            {
                m_projectName.m_fullFileName = rdoxFileName;
                m_projectName.m_rdox = true;
                m_hasModel = true;
            }
            else if (rdo::File::exist(smrFileName))
            {
                m_projectName.m_fullFileName = smrFileName;
                m_projectName.m_rdox = false;
                m_files[rdo::FileType::SMR].m_fileName = m_modelName;
                m_hasModel = true;
            }

            if (m_hasModel)
            {
                switch (updateModelNames())
                {
                case FindModel::OK       : broadcastMessage(Message::REPOSITORY_MODEL_OPEN); return true;
                case FindModel::SMR_ERROR: broadcastMessage(Message::REPOSITORY_MODEL_OPEN); return false;
                case FindModel::SMR_EMPTY: return false;
                }
            }
            else
            {
                setName(boost::filesystem::path());
                broadcastMessage(Message::REPOSITORY_MODEL_OPEN_ERROR, const_cast<boost::filesystem::path*>(&modelFileName));
            }
        }
    }
    else
    {
        broadcastMessage(Message::REPOSITORY_MODEL_CLOSE_ERROR);
    }

    return false;
}

bool RDOThreadRepository::saveModel()
{
    ASSERT(!m_modelPath.empty());
    broadcastMessage(Message::REPOSITORY_MODEL_SAVE);
    return true;
}

bool RDOThreadRepository::canCloseModel()
{
    if (m_hasModel)
    {
        bool res = true;
        broadcastMessage(Message::REPOSITORY_MODEL_CLOSE_CAN_CLOSE, &res, true);
        return res;
    }
    else
    {
        return true;
    }
}

void RDOThreadRepository::realCloseModel()
{
    if (m_hasModel)
    {
        m_hasModel = false;
        broadcastMessage(Message::REPOSITORY_MODEL_CLOSE);
        m_modelName = boost::filesystem::path();
        m_modelPath = boost::filesystem::path();
        resetModelNames();
    }
}

void RDOThreadRepository::closeModel()
{
    if (canCloseModel())
    {
        realCloseModel();
    }
    else
    {
        broadcastMessage(Message::REPOSITORY_MODEL_CLOSE_ERROR);
    }
}

void RDOThreadRepository::extractName(const boost::filesystem::path& fullName)
{
    m_modelPath = rdo::File::extractFilePath(fullName);

    boost::filesystem::path path(fullName);
    setName(path.filename().stem());
}

void RDOThreadRepository::setName(const boost::filesystem::path& name)
{
    m_modelName = name;
    if (m_modelName.empty())
    {
        m_modelPath = boost::filesystem::path();
        resetModelNames();
    }
}

void RDOThreadRepository::loadFile(const boost::filesystem::path& fileName, std::ostream& stream, bool mustExist, bool& reanOnly) const
{
    if (rdo::File::exist(fileName))
    {
        if (!m_realOnlyInDlg)
        {
            reanOnly = rdo::File::read_only(fileName);
        }
        else
        {
            reanOnly = true;
        }
        boost::filesystem::ifstream file(fileName, std::ios::in | std::ios::binary);
        stream << file.rdbuf();
        file.close();
    }
    else
    {
        stream.setstate(std::ios_base::badbit);
        if (mustExist)
        {
            stream.setstate(stream.rdstate() | std::ios_base::failbit);
        }
    }
}

void RDOThreadRepository::saveFile(const boost::filesystem::path& fileName, const std::stringstream& stream, bool deleteIfEmpty) const
{
    if (!fileName.empty())
    {
        const bool file_exist = rdo::File::exist(fileName);
        if (!stream.str().empty() || (file_exist && !deleteIfEmpty))
        {
            boost::filesystem::ofstream file(fileName, std::ios::out | std::ios::binary);
            file << stream.rdbuf();
            file.close();
        }
        else
        {
            if (file_exist && deleteIfEmpty)
            {
                rdo::File::unlink(fileName);
            }
        }
    }
}

bool RDOThreadRepository::createRDOX(const boost::filesystem::path& fileName)
{
    boost::property_tree::ptree pt;
    boost::property_tree::ptree& version = pt.add("Settings.Version", "");
    version.put("<xmlattr>.ProjectVersion", "2");
    version.put("<xmlattr>.SMRVersion", "2");

    boost::filesystem::ofstream ofs(fileName);
    if (!ofs.good())
        return false;

    boost::property_tree::xml_writer_settings<std::string> settings('\t', 1);
    boost::property_tree::write_xml(ofs, pt, settings);
    return true;
}

void RDOThreadRepository::createRDOX()
{
    const auto it = m_files.find(rdo::FileType::RDOX);
    ASSERT(it != m_files.end());
    boost::filesystem::path rdoxFileName = (m_modelPath / m_modelName).replace_extension(it->second.m_extension);
    if (!rdo::File::exist(rdoxFileName))
    {
        if (createRDOX(rdoxFileName))
        {
            m_projectName.m_fullFileName = rdoxFileName;
            m_projectName.m_rdox         = true;
        }
    }
}

void RDOThreadRepository::load(rdo::FileType type, std::ostream& stream)
{
    loadFile(getFullFileName(type), stream, m_files[type].m_mustExist, m_files[type].m_readOnly);
}

void RDOThreadRepository::save(rdo::FileType type, const std::stringstream& stream) const
{
    saveFile(getFullFileName(type), stream, isDeleteIfEmpty(type));
    if (type == rdo::FileType::SMR)
    {
        const_cast<RDOThreadRepository*>(this)->updateModelNames();
    }
}

void RDOThreadRepository::loadBMP(boost::filesystem::path& name, std::ostream& stream) const
{
    boost::filesystem::path fileName = (m_modelPath / name).replace_extension("bmp");
    if (rdo::File::exist(fileName))
    {
        boost::filesystem::ifstream file(fileName, std::ios::in | std::ios::binary);
        stream << file.rdbuf();
        file.close();
        name = fileName;
    }
    else
    {
        stream.setstate(std::ios_base::badbit);
    }
}

void RDOThreadRepository::writeModelFilesInfo(boost::filesystem::ofstream& stream) const
{
    stream << "Results_file   = " << rdo::locale::convertFromWStr(getFileExtName(rdo::FileType::PMV).wstring()) << "    " << rdo::Time::local().asString() << std::endl;
    stream << "Run_file       = " << rdo::locale::convertFromWStr(getFileExtName(rdo::FileType::SMR).wstring()) << std::endl;
    stream << "Model_name     = " << rdo::locale::convertFromWStr(getFileName(rdo::FileType::SMR).wstring()) << std::endl;
    stream << "Resource_file  = " << rdo::locale::convertFromWStr(getFileName(rdo::FileType::RSS).replace_extension(getExtension(rdo::FileType::RSS)).wstring()) << std::endl;
}

bool RDOThreadRepository::createFile(const boost::filesystem::path& name, boost::filesystem::ofstream& stream) const
{
    // TODO: проверить name с русскими буквами
    std::stringstream backupDirName;
    backupDirName << m_modelPath
                  << boost::format("%1$04d-%2$02d-%3$02d %4$02d-%5$02d-%6$02d %7$s.%8$s")
                     % m_systemTime.date().year ()
                     % m_systemTime.date().month()
                     % m_systemTime.date().day  ()
                     % m_systemTime.time_of_day().hours  ()
                     % m_systemTime.time_of_day().minutes()
                     % m_systemTime.time_of_day().seconds()
                     % name.filename()
                     % name.extension()
                     ;

    const std::string fullFileName = backupDirName.str();
    stream.open(rdo::locale::convertToWStr(fullFileName), std::ios::out | std::ios::binary);
    return stream.is_open();
}

void RDOThreadRepository::beforeModelStart()
{
    m_systemTime = boost::posix_time::second_clock::local_time();

    if (m_traceFile.is_open())
        m_traceFile.close();

    m_traceFile.open(getFullFileName(rdo::FileType::TRC), std::ios::out | std::ios::binary);
    if (m_traceFile.is_open())
    {
        writeModelFilesInfo(m_traceFile);
        std::stringstream model_structure;
        sendMessage(kernel->simulator(), Message::SIMULATOR_GET_MODEL_STRUCTURE, &model_structure);
        m_traceFile << std::endl << model_structure.str() << std::endl;
        m_traceFile << "$Tracing" << std::endl;
    }
}

void RDOThreadRepository::stopModel()
{
    if (m_traceFile.is_open())
        m_traceFile.close();

    boost::filesystem::ofstream results_file;
    results_file.open(getFullFileName(rdo::FileType::PMV), std::ios::out | std::ios::binary);
    if (results_file.is_open())
    {
        writeModelFilesInfo(results_file);
        std::stringstream stream;
        sendMessage(kernel->simulator(), Message::SIMULATOR_GET_MODEL_RESULTS_INFO, &stream);
        results_file << std::endl << stream.str() << std::endl;
        stream.str("");
        stream.clear();
        sendMessage(kernel->simulator(), Message::SIMULATOR_GET_MODEL_RESULTS, &stream);
        results_file << std::endl << stream.str() << std::endl;
    }
}

void RDOThreadRepository::trace(const std::string& message)
{
    if (m_traceFile.is_open())
    {
        m_traceFile.write(message.c_str(), message.length());
        m_traceFile.write("\n", 1);
        m_traceFile.flush();
    }
}

boost::filesystem::path RDOThreadRepository::getFileName(rdo::FileType type) const
{
    const auto it = m_files.find(type);
    if (it == m_files.end())
    {
        NEVER_REACH_HERE;
    }

    return it->second.m_fileName;
}

boost::filesystem::path RDOThreadRepository::getExtension(rdo::FileType type) const
{
    const auto it = m_files.find(type);
    if (it == m_files.end())
    {
        NEVER_REACH_HERE;
    }

    return it->second.m_extension;
}

boost::filesystem::path RDOThreadRepository::getFileExtName(rdo::FileType type) const
{
    const auto it = m_files.find(type);
    if (it == m_files.end())
    {
        NEVER_REACH_HERE;
    }

    return boost::filesystem::path(it->second.m_fileName).replace_extension(it->second.m_extension);
}

boost::filesystem::path RDOThreadRepository::getFullFileName(rdo::FileType type) const
{
    return m_modelPath / getFileExtName(type);
}

bool RDOThreadRepository::isReadOnly(rdo::FileType type) const
{
    const auto it = m_files.find(type);
    if (it == m_files.end())
    {
        NEVER_REACH_HERE;
    }

    return it->second.m_readOnly;
}

bool RDOThreadRepository::isMustExist(rdo::FileType type) const
{
    const auto it = m_files.find(type);
    if (it == m_files.end())
    {
        NEVER_REACH_HERE;
    }

    return it->second.m_mustExist;
}

bool RDOThreadRepository::isDeleteIfEmpty(rdo::FileType type) const
{
    const auto it = m_files.find(type);
    if (it == m_files.end())
    {
        NEVER_REACH_HERE;
    }

    return it->second.m_deleteIfEmpty;
}

}} // namespace rdo::repository
