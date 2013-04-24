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
#include <boost/algorithm/string/trim.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "repository/rdorepository.h"
#include "utils/rdofile.h"
#include "utils/rdotime.h"
#include "utils/rdolocale.h"
#include "kernel/rdokernel.h"
#include "simulator/service/rdosimwin.h"
#include "thirdparty/pugixml/src/pugixml.hpp"
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

	m_files[rdoModelObjects::RDOX].m_extention = ".rdox";
	m_files[rdoModelObjects::RTP ].m_extention = ".rtp";
	m_files[rdoModelObjects::RSS ].m_extention = ".rss";
	m_files[rdoModelObjects::EVN ].m_extention = ".evn";
	m_files[rdoModelObjects::PAT ].m_extention = ".pat";
	m_files[rdoModelObjects::DPT ].m_extention = ".dpt";
	m_files[rdoModelObjects::PRC ].m_extention = ".prc";
	m_files[rdoModelObjects::PRCX].m_extention = ".prcx";
	m_files[rdoModelObjects::FRM ].m_extention = ".frm";
	m_files[rdoModelObjects::FUN ].m_extention = ".fun";
	m_files[rdoModelObjects::SMR ].m_extention = ".smr";
	m_files[rdoModelObjects::PMD ].m_extention = ".pmd";
	m_files[rdoModelObjects::PMV ].m_extention = ".pmv";
	m_files[rdoModelObjects::TRC ].m_extention = ".trc";

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
			data->m_extention = getExtention   (data->m_type);
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
			extractName((path / boost::filesystem::path(data->m_name + m_files[rdoModelObjects::RDOX].m_extention)).string());
			path = rdo::locale::convertToWStr(path.string());
			if (!rdo::File::exist(path))
			{
				boost::filesystem::create_directory(path);
			}
			createRDOX();
		}
		else
		{
			m_modelName = "noname";
			m_modelPath = "";
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

rbool RDOThreadRepository::openModel(CREF(tstring) modelFileName)
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

			tstring rdoxFileName = m_modelPath + m_modelName + m_files[rdoModelObjects::RDOX].m_extention;
			tstring smrFileName  = m_modelPath + m_modelName + m_files[rdoModelObjects::SMR ].m_extention;

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
				setName("");
				broadcastMessage(RT_REPOSITORY_MODEL_OPEN_ERROR, const_cast<PTR(tstring)>(&modelFileName));
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
		m_modelName = "";
		m_modelPath = "";
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

void RDOThreadRepository::extractName(CREF(tstring) fullName)
{
	m_modelPath = rdo::File::extractFilePath(fullName);

	boost::filesystem::path path(fullName);
	tstring name = path.filename().stem().string();
	setName(name);
}

void RDOThreadRepository::setName(CREF(tstring) name)
{
	m_modelName = name;
	boost::algorithm::trim(m_modelName);
	if (m_modelName.empty())
	{
		m_modelPath = "";
		resetModelNames();
	}
}

void RDOThreadRepository::loadFile(CREF(tstring) fileName, REF(rdo::stream) stream, rbool described, rbool mustExist, REF(rbool) reanOnly) const
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
			if (stream.isBinary())
			{
				boost::filesystem::ifstream file(rdo::locale::convertToWStr(fileName), std::ios::in | std::ios::binary);
				stream << file.rdbuf();
				file.close();
			}
			else
			{
				boost::filesystem::ifstream file(rdo::locale::convertToWStr(fileName));
				stream << file.rdbuf();
				file.close();
			}
/*
			if (stream.getOpenMode() & std::ios::binary*)
			{
				boost::filesystem::ifstream file(rdo::locale::convertToWStr(fileName), std::ios::in | std::ios::binary);
				file.seekg(0, std::ios::end);
				int len = file.tellg();
				file.seekg(0, std::ios::beg);
				stream.resize(len);
				file.read(stream.data(), len);
				file.close();
			}
			else
			{
				boost::filesystem::ifstream file(rdo::locale::convertToWStr(fileName));
				stream << file.rdbuf();
				file.close();
			}
*/
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

void RDOThreadRepository::saveFile(CREF(tstring) fileName, REF(rdo::stream) stream, rbool deleteIfEmpty) const
{
	if (!fileName.empty())
	{
		rbool file_exist = rdo::File::exist(fileName);
		if (!stream.str().empty() || (file_exist && !deleteIfEmpty))
		{
			if (stream.isBinary())
			{
				boost::filesystem::ofstream file(rdo::locale::convertToWStr(fileName), std::ios::out | std::ios::binary);
				file << stream.rdbuf();
				file.close();
			}
			else
			{
				boost::filesystem::ofstream file(rdo::locale::convertToWStr(fileName));
				file << stream.rdbuf();
				file.close();
			}
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

void RDOThreadRepository::createRDOX()
{
	BOOST_AUTO(it, m_files.find(rdoModelObjects::RDOX));
	ASSERT(it != m_files.end());
	boost::filesystem::path rdoxFileName = rdo::locale::convertToWStr(m_modelPath + m_modelName + it->second.m_extention);
	if (!rdo::File::exist(rdoxFileName))
	{
		pugi::xml_document doc;
		pugi::xml_node      rootNode           = doc.append_child("Settings");
		pugi::xml_node      versionNode        = rootNode.append_child("Version");
		pugi::xml_attribute projectVersionAttr = versionNode.append_attribute("ProjectVersion");
		pugi::xml_attribute smrVersionAttr     = versionNode.append_attribute("SMRVersion");
		projectVersionAttr.set_value("2");
		smrVersionAttr    .set_value("2");

		boost::filesystem::ofstream ofs(rdoxFileName);
		if (ofs.good())
		{
			doc.save(ofs);
			m_projectName.m_fullFileName = rdo::locale::convertFromWStr(rdoxFileName.wstring());
			m_projectName.m_rdox         = true;
		}
	}
}

void RDOThreadRepository::load(rdoModelObjects::RDOFileType type, REF(rdo::stream) stream)
{
	loadFile(getFullFileName(type), stream, m_files[type].m_described, m_files[type].m_mustExist, m_files[type].m_readOnly);
}

void RDOThreadRepository::save(rdoModelObjects::RDOFileType type, REF(rdo::stream) stream) const
{
	saveFile(getFullFileName(type), stream, isDeleteIfEmpty(type));
	if (type == rdoModelObjects::SMR)
	{
		const_cast<PTR(RDOThreadRepository)>(this)->updateModelNames();
	}
}

void RDOThreadRepository::loadBMP(REF(tstring) name, REF(rdo::stream) stream) const
{
	tstring fileName = m_modelPath + name + ".bmp";
	if (rdo::File::exist(fileName))
	{
		boost::filesystem::ifstream file(rdo::locale::convertToWStr(fileName), std::ios::in | std::ios::binary);
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
	stream << "Results_file   = " << getFileExtName(rdoModelObjects::PMV) << "    " << rdo::Time::local().asString() << std::endl;
	stream << "Run_file       = " << getFileExtName(rdoModelObjects::SMR) << std::endl;
	stream << "Model_name     = " << getFileName(rdoModelObjects::SMR) << std::endl;
	stream << "Resource_file  = " << getFileName(rdoModelObjects::RSS) << getExtention(rdoModelObjects::RSS) << std::endl;
}

rbool RDOThreadRepository::createFile(CREF(boost::filesystem::path) name, REF(boost::filesystem::ofstream) stream) const
{
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
		m_traceFile.open(rdo::locale::convertToWStr(getFullFileName(rdoModelObjects::TRC)), std::ios::out | std::ios::binary);
		if (m_traceFile.is_open())
		{
			writeModelFilesInfo(m_traceFile);
			rdo::textstream model_structure;
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
		results_file.open(rdo::locale::convertToWStr(getFullFileName(rdoModelObjects::PMV)), std::ios::out | std::ios::binary);
		if (results_file.is_open())
		{
			writeModelFilesInfo(results_file);
			rdo::textstream stream;
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

tstring RDOThreadRepository::getFileName(rdoModelObjects::RDOFileType type) const
{
	BOOST_AUTO(it, m_files.find(type));
	if (it == m_files.end())
	{
		NEVER_REACH_HERE;
	}

	return it->second.m_fileName;
}

tstring RDOThreadRepository::getExtention(rdoModelObjects::RDOFileType type) const
{
	BOOST_AUTO(it, m_files.find(type));
	if (it == m_files.end())
	{
		NEVER_REACH_HERE;
	}

	return it->second.m_extention;
}

tstring RDOThreadRepository::getFileExtName(rdoModelObjects::RDOFileType type) const
{
	BOOST_AUTO(it, m_files.find(type));
	if (it == m_files.end())
	{
		NEVER_REACH_HERE;
	}
	return it->second.m_fileName + it->second.m_extention;
}

tstring RDOThreadRepository::getFullFileName(rdoModelObjects::RDOFileType type) const
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
