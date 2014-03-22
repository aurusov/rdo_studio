/*!
  \copyright (c) RDO-Team, 2011
  \file      rdorepository.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date
  \brief
  \indent    4T
*/

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
	notifies.push_back(RT_STUDIO_MODEL_NEW                  );
	notifies.push_back(RT_STUDIO_MODEL_OPEN                 );
	notifies.push_back(RT_STUDIO_MODEL_CLOSE                );
	notifies.push_back(RT_STUDIO_MODEL_SAVE                 );
	notifies.push_back(RT_REPOSITORY_MODEL_GET_FILEINFO     );
	notifies.push_back(RT_REPOSITORY_LOAD                   );
	notifies.push_back(RT_REPOSITORY_SAVE                   );
	notifies.push_back(RT_REPOSITORY_LOAD_BINARY            );
	notifies.push_back(RT_REPOSITORY_CREATE_FILE            );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_OK           );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_BY_USER      );
	notifies.push_back(RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR);
	notifies.push_back(RT_RUNTIME_MODEL_START_BEFORE        );
	notifies.push_back(RT_RUNTIME_TRACE_STRING              );

	m_files[rdoModelObjects::RDOX].m_extension = "rdox";
	m_files[rdoModelObjects::RTP ].m_extension = "rtp";
	m_files[rdoModelObjects::RSS ].m_extension = "rss";
	m_files[rdoModelObjects::EVN ].m_extension = "evn";
	m_files[rdoModelObjects::PAT ].m_extension = "pat";
	m_files[rdoModelObjects::DPT ].m_extension = "dpt";
	m_files[rdoModelObjects::PRC ].m_extension = "prc";
	m_files[rdoModelObjects::PRCX].m_extension = "prcx";
	m_files[rdoModelObjects::FRM ].m_extension = "frm";
	m_files[rdoModelObjects::FUN ].m_extension = "fun";
	m_files[rdoModelObjects::SMR ].m_extension = "smr";
	m_files[rdoModelObjects::PMD ].m_extension = "pmd";
	m_files[rdoModelObjects::PMV ].m_extension = "pmv";
	m_files[rdoModelObjects::TRC ].m_extension = "trc";

//	m_files[rdoModelObjects::OPR].deleteifempty = true;
//	m_files[rdoModelObjects::DPT].deleteifempty = true;

	resetModelNames();

	after_constructor();
}

RDOThreadRepository::~RDOThreadRepository()
{
	m_traceFile.close();
}

void RDOThreadRepository::proc(REF(RDOMessageInfo) msg)
{
	switch (msg.message)
	{
		case RT_STUDIO_MODEL_NEW:
		{
			msg.lock();
			PTR(NewModel) data = static_cast<PTR(NewModel)>(msg.param);
			newModel(data);
			msg.unlock();
			break;
		}
		case RT_STUDIO_MODEL_OPEN:
		{
			msg.lock();
			PTR(OpenFile) data = static_cast<PTR(OpenFile)>(msg.param);
			data->m_result = openModel(data->m_name);
			msg.unlock();
			break;
		}
		case RT_STUDIO_MODEL_CLOSE:
		{
			closeModel();
			break;
		}
		case RT_STUDIO_MODEL_SAVE:
		{
			rbool res = saveModel();
			msg.lock();
			if (msg.param) *static_cast<PTR(rbool)>(msg.param) = res;
			msg.unlock();
			break;
		}
		case RT_RUNTIME_MODEL_START_BEFORE:
		{
			beforeModelStart();
			break;
		}
		case RT_SIMULATOR_MODEL_STOP_OK           :
		case RT_SIMULATOR_MODEL_STOP_BY_USER      :
		case RT_SIMULATOR_MODEL_STOP_RUNTIME_ERROR:
		{
			stopModel();
			break;
		}
		case RT_RUNTIME_TRACE_STRING:
		{
			msg.lock();
			trace(*static_cast<PTR(tstring)>(msg.param));
			msg.unlock();
			break;
		}
		case RT_REPOSITORY_MODEL_GET_FILEINFO:
		{
			msg.lock();
			PTR(FileInfo) data = static_cast<PTR(FileInfo)>(msg.param);
			data->m_name      = getFileName    (data->m_type);
			data->m_fullName  = getFullFileName(data->m_type);
			data->m_extension = getExtension   (data->m_type);
			data->m_described = isDescribed    (data->m_type);
			data->m_readOnly  = isReadOnly     (data->m_type);
			msg.unlock();
			break;
		}
		case RT_REPOSITORY_LOAD:
		{
			msg.lock();
			PTR(FileData) fdata = static_cast<PTR(FileData)>(msg.param);
			load(fdata->m_type, fdata->m_stream);
			msg.unlock();
			break;
		}
		case RT_REPOSITORY_SAVE:
		{
			msg.lock();
			PTR(FileData) fdata = static_cast<PTR(FileData)>(msg.param);
			save(fdata->m_type, fdata->m_stream);
			msg.unlock();
			break;
		}
		case RT_REPOSITORY_LOAD_BINARY:
		{
			msg.lock();
			PTR(BinaryFile) data = static_cast<PTR(BinaryFile)>(msg.param);
			loadBMP(data->m_name, data->m_stream);
			msg.unlock();
			break;
		}
		case RT_REPOSITORY_CREATE_FILE:
		{
			msg.lock();
			PTR(CreateFileInfo) data = static_cast<PTR(CreateFileInfo)>(msg.param);
			createFile(data->m_name, data->m_stream);
			msg.unlock();
			break;
		}
		default: break;
	}
}

void RDOThreadRepository::resetModelNames()
{
	STL_FOR_ALL(m_files, it)
	{
		it->second.resetname();
	}
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
				return fm_smr_empty;
			}
			return fm_smr_error;
		}
	}

	STL_FOR_ALL(m_files, it)
	{
		it->second.m_fileName      = m_modelName;
		it->second.m_described     = true;
		it->second.m_mustExist     = false;
		it->second.m_deleteIfEmpty = true;
	}
	return fm_ok;
}

void RDOThreadRepository::newModel(CPTRC(NewModel) data)
{
	if (canCloseModel())
	{
		realCloseModel();
		if (data)
		{
			boost::filesystem::path path = data->m_path;
			extractName((path / data->m_name).replace_extension(m_files[rdoModelObjects::RDOX].m_extension));
			if (!rdo::File::exist(path))
			{
				boost::filesystem::create_directory(path);
			}
			createRDOX();
		}
		else
		{
			m_modelName = "noname";
			m_modelPath = boost::filesystem::path();
		}
		STL_FOR_ALL(m_files, it)
		{
			it->second.m_fileName = m_modelName;
		}
		m_hasModel = true;
		broadcastMessage(RT_REPOSITORY_MODEL_NEW);
	}
	else
	{
		broadcastMessage(RT_REPOSITORY_MODEL_CLOSE_ERROR);
	}
}

rbool RDOThreadRepository::openModel(CREF(boost::filesystem::path) modelFileName)
{
	if (canCloseModel())
	{
		realCloseModel();

		rbool canOpen   = true;
		m_realOnlyInDlg = false;
		if (modelFileName.empty())
		{
			OpenFile data;
			broadcastMessage(RT_REPOSITORY_MODEL_OPEN_GET_NAME, &data, true);
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
			STL_FOR_ALL(m_files, it)
			{
				it->second.m_readOnly = m_realOnlyInDlg;
			}

			boost::filesystem::path rdoxFileName = (m_modelPath / m_modelName).replace_extension(m_files[rdoModelObjects::RDOX].m_extension);
			boost::filesystem::path smrFileName  = (m_modelPath / m_modelName).replace_extension(m_files[rdoModelObjects::SMR ].m_extension);

			if (rdo::File::exist(rdoxFileName))
			{
				m_projectName.m_fullFileName = rdoxFileName;
				m_projectName.m_rdox         = true;
				m_hasModel                   = true;
			}
			else if (rdo::File::exist(smrFileName))
			{
				m_projectName.m_fullFileName             = smrFileName;
				m_projectName.m_rdox                     = false;
				m_files[rdoModelObjects::SMR].m_fileName = m_modelName;
				m_hasModel                               = true;
			}

			if (m_hasModel)
			{
				switch (updateModelNames())
				{
				case fm_ok       : broadcastMessage(RT_REPOSITORY_MODEL_OPEN); return true;
				case fm_smr_error: broadcastMessage(RT_REPOSITORY_MODEL_OPEN); return false;
				case fm_smr_empty: return false;
				}
			}
			else
			{
				setName(boost::filesystem::path());
				broadcastMessage(RT_REPOSITORY_MODEL_OPEN_ERROR, const_cast<PTR(boost::filesystem::path)>(&modelFileName));
			}
		}
	}
	else
	{
		broadcastMessage(RT_REPOSITORY_MODEL_CLOSE_ERROR);
	}

	return false;
}

rbool RDOThreadRepository::saveModel()
{
	ASSERT(!m_modelPath.empty());
	broadcastMessage(RT_REPOSITORY_MODEL_SAVE);
	return true;
}

rbool RDOThreadRepository::canCloseModel()
{
	if (m_hasModel)
	{
		rbool res = true;
		broadcastMessage(RT_REPOSITORY_MODEL_CLOSE_CAN_CLOSE, &res, true);
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
		broadcastMessage(RT_REPOSITORY_MODEL_CLOSE);
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
		broadcastMessage(RT_REPOSITORY_MODEL_CLOSE_ERROR);
	}
}

void RDOThreadRepository::extractName(CREF(boost::filesystem::path) fullName)
{
	m_modelPath = rdo::File::extractFilePath(fullName);

	boost::filesystem::path path(fullName);
	setName(path.filename().stem());
}

void RDOThreadRepository::setName(CREF(boost::filesystem::path) name)
{
	m_modelName = name;
	if (m_modelName.empty())
	{
		m_modelPath = boost::filesystem::path();
		resetModelNames();
	}
}

void RDOThreadRepository::loadFile(CREF(boost::filesystem::path) fileName, std::ostream& stream, rbool described, rbool mustExist, REF(rbool) reanOnly) const
{
	if (described)
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
	else
	{
		stream.setstate(std::ios_base::badbit);
	}
}

void RDOThreadRepository::saveFile(CREF(boost::filesystem::path) fileName, const std::stringstream& stream, rbool deleteIfEmpty) const
{
	if (!fileName.empty())
	{
		rbool file_exist = rdo::File::exist(fileName);
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

	boost::property_tree::xml_writer_settings<char> settings('\t', 1);
	boost::property_tree::write_xml(ofs, pt, settings);
	return true;
}

void RDOThreadRepository::createRDOX()
{
	BOOST_AUTO(it, m_files.find(rdoModelObjects::RDOX));
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

void RDOThreadRepository::load(rdoModelObjects::RDOFileType type, std::ostream& stream)
{
	loadFile(getFullFileName(type), stream, m_files[type].m_described, m_files[type].m_mustExist, m_files[type].m_readOnly);
}

void RDOThreadRepository::save(rdoModelObjects::RDOFileType type, const std::stringstream& stream) const
{
	saveFile(getFullFileName(type), stream, isDeleteIfEmpty(type));
	if (type == rdoModelObjects::SMR)
	{
		const_cast<PTR(RDOThreadRepository)>(this)->updateModelNames();
	}
}

void RDOThreadRepository::loadBMP(REF(boost::filesystem::path) name, std::ostream& stream) const
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

void RDOThreadRepository::writeModelFilesInfo(REF(boost::filesystem::ofstream) stream) const
{
	stream << "Results_file   = " << rdo::locale::convertFromWStr(getFileExtName(rdoModelObjects::PMV).wstring()) << "    " << rdo::Time::local().asString() << std::endl;
	stream << "Run_file       = " << rdo::locale::convertFromWStr(getFileExtName(rdoModelObjects::SMR).wstring()) << std::endl;
	stream << "Model_name     = " << rdo::locale::convertFromWStr(getFileName(rdoModelObjects::SMR).wstring()) << std::endl;
	stream << "Resource_file  = " << rdo::locale::convertFromWStr(getFileName(rdoModelObjects::RSS).replace_extension(getExtension(rdoModelObjects::RSS)).wstring()) << std::endl;
}

rbool RDOThreadRepository::createFile(CREF(boost::filesystem::path) name, REF(boost::filesystem::ofstream) stream) const
{
	//! TODO: проверить name с русскими буквами
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

	tstring fullFileName = backupDirName.str();
	stream.open(rdo::locale::convertToWStr(fullFileName), std::ios::out | std::ios::binary);
	return stream.is_open();
}

void RDOThreadRepository::beforeModelStart()
{
	m_systemTime = boost::posix_time::second_clock::local_time();

	if (m_traceFile.is_open())
	{
		m_traceFile.close();
	}
	if (m_files[rdoModelObjects::TRC].m_described)
	{
		m_traceFile.open(getFullFileName(rdoModelObjects::TRC), std::ios::out | std::ios::binary);
		if (m_traceFile.is_open())
		{
			writeModelFilesInfo(m_traceFile);
			std::stringstream model_structure;
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_STRUCTURE, &model_structure);
			m_traceFile << std::endl << model_structure.str() << std::endl;
			m_traceFile << "$Tracing" << std::endl;
		}
	}
}

void RDOThreadRepository::stopModel()
{
	if (m_traceFile.is_open())
	{
		m_traceFile.close();
	}
	if (m_files[rdoModelObjects::PMV].m_described)
	{
		boost::filesystem::ofstream results_file;
		results_file.open(getFullFileName(rdoModelObjects::PMV), std::ios::out | std::ios::binary);
		if (results_file.is_open())
		{
			writeModelFilesInfo(results_file);
			std::stringstream stream;
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_RESULTS_INFO, &stream);
			results_file << std::endl << stream.str() << std::endl;
			stream.str("");
			stream.clear();
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_RESULTS, &stream);
			results_file << std::endl << stream.str() << std::endl;
		}
	}
}

void RDOThreadRepository::trace(CREF(tstring) message)
{
	if (m_traceFile.is_open())
	{
		m_traceFile.write(message.c_str(), message.length());
		m_traceFile.write("\n", 1);
		m_traceFile.flush();
	}
}

boost::filesystem::path RDOThreadRepository::getFileName(rdoModelObjects::RDOFileType type) const
{
	BOOST_AUTO(it, m_files.find(type));
	if (it == m_files.end())
	{
		NEVER_REACH_HERE;
	}

	return it->second.m_fileName;
}

boost::filesystem::path RDOThreadRepository::getExtension(rdoModelObjects::RDOFileType type) const
{
	BOOST_AUTO(it, m_files.find(type));
	if (it == m_files.end())
	{
		NEVER_REACH_HERE;
	}

	return it->second.m_extension;
}

boost::filesystem::path RDOThreadRepository::getFileExtName(rdoModelObjects::RDOFileType type) const
{
	BOOST_AUTO(it, m_files.find(type));
	if (it == m_files.end())
	{
		NEVER_REACH_HERE;
	}
	return boost::filesystem::path(it->second.m_fileName).replace_extension(it->second.m_extension);
}

boost::filesystem::path RDOThreadRepository::getFullFileName(rdoModelObjects::RDOFileType type) const
{
	return (type == rdoModelObjects::PMV || type == rdoModelObjects::TRC)
		? getFullOutputFileName(type)
		: getFullInputFileName (type);
}

tstring RDOThreadRepository::getFullInputFileName(rdoModelObjects::RDOFileType type) const
{
	ASSERT(type != rdoModelObjects::PMV && type != rdoModelObjects::TRC);
	return m_modelPath + getFileExtName(type);
}

tstring RDOThreadRepository::getFullOutputFileName(rdoModelObjects::RDOFileType type) const
{
	ASSERT(type == rdoModelObjects::PMV || type == rdoModelObjects::TRC);

	ruint runNumber = (kernel->simulator()->getSeriesCapacity());
	tstring runNumberStr = runNumber
		? rdo::format("-%i", kernel->simulator()->getCurrentRunNumber())
		: "";
	return m_modelPath + getFileName(type)+ runNumberStr + getExtention(type);
}

rbool RDOThreadRepository::isReadOnly(rdoModelObjects::RDOFileType type) const
{
	BOOST_AUTO(it, m_files.find(type));
	if (it == m_files.end())
	{
		NEVER_REACH_HERE;
	}

	return it->second.m_readOnly;
}

rbool RDOThreadRepository::isDescribed(rdoModelObjects::RDOFileType type) const
{
	BOOST_AUTO(it, m_files.find(type));
	if (it == m_files.end())
	{
		NEVER_REACH_HERE;
	}

	return it->second.m_described;
}

rbool RDOThreadRepository::isMustExist(rdoModelObjects::RDOFileType type) const
{
	BOOST_AUTO(it, m_files.find(type));
	if (it == m_files.end())
	{
		NEVER_REACH_HERE;
	}

	return it->second.m_mustExist;
}

rbool RDOThreadRepository::isDeleteIfEmpty(rdoModelObjects::RDOFileType type) const
{
	BOOST_AUTO(it, m_files.find(type));
	if (it == m_files.end())
	{
		NEVER_REACH_HERE;
	}

	return it->second.m_deleteIfEmpty;
}

}} // namespace rdo::repository
