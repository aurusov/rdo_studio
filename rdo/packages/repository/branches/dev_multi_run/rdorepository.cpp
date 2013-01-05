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
#include <boost/filesystem.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "repository/rdorepository.h"
#include "utils/rdofile.h"
#include "utils/rdotime.h"
#include "kernel/rdokernel.h"
#include "simulator/service/rdosimwin.h"
#include "thirdparty/pugixml/src/pugixml.hpp"
// --------------------------------------------------------------------------------

OPEN_RDO_REPOSITORY_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RDOThreadRepository
// --------------------------------------------------------------------------------
RDOThreadRepository::RDOThreadRepository()
	: RDOThreadMT    (_T("RDOThreadRepository"))
	, m_modelName    (_T("")                   )
	, m_modelPath    (_T("")                   )
	, m_hasModel     (false                    )
	, m_realOnlyInDlg(false                    )
	, m_firstStart   (0                        )
	, m_count        (0                        )
{
	notifies.push_back(RT_STUDIO_MODEL_NEW                  );
	notifies.push_back(RT_STUDIO_MODEL_OPEN                 );
	notifies.push_back(RT_STUDIO_MODEL_CLOSE                );
	notifies.push_back(RT_STUDIO_MODEL_SAVE                 );
	notifies.push_back(RT_STUDIO_MODEL_SAVE_AS              );
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

	m_files[rdoModelObjects::RDOX].m_extention = _T(".rdox");
	m_files[rdoModelObjects::RTP ].m_extention = _T(".rtp");
	m_files[rdoModelObjects::RSS ].m_extention = _T(".rss");
	m_files[rdoModelObjects::EVN ].m_extention = _T(".evn");
	m_files[rdoModelObjects::PAT ].m_extention = _T(".pat");
	m_files[rdoModelObjects::DPT ].m_extention = _T(".dpt");
	m_files[rdoModelObjects::PRC ].m_extention = _T(".prc");
	m_files[rdoModelObjects::PRCX].m_extention = _T(".prcx");
	m_files[rdoModelObjects::FRM ].m_extention = _T(".frm");
	m_files[rdoModelObjects::FUN ].m_extention = _T(".fun");
	m_files[rdoModelObjects::SMR ].m_extention = _T(".smr");
	m_files[rdoModelObjects::PMD ].m_extention = _T(".pmd");
	m_files[rdoModelObjects::PMV ].m_extention = _T(".pmv");
	m_files[rdoModelObjects::TRC ].m_extention = _T(".trc");

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
		case RT_STUDIO_MODEL_SAVE_AS:
		{
			saveAsModel();
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
			createFile(data->m_name, data->m_ext, data->m_stream);
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
		rdo::textstream smrStream;
		loadFile(getFullFileName(rdoModelObjects::SMR), smrStream, true, true, m_files[rdoModelObjects::SMR].m_readOnly);
		rdo::converter::smr2rdox::RDOSMRFileInfo fileInfo;
		kernel->simulator()->parseSMRFileInfo(smrStream, fileInfo);
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
			tstring path = data->m_path;
			tstring::size_type pos = path.find_last_of(_T('\\'));
			if (pos == tstring::npos)
			{
				pos = path.find_last_of(_T('/'));
			}
			if (pos != path.length() - 1)
			{
				path += _T('\\');
			}
			extractName(path + data->m_name + m_files[rdoModelObjects::RDOX].m_extention);
			if (!rdo::File::exist(path))
			{
				boost::filesystem::create_directory(path.c_str());
			}
			createRDOX();
		}
		else
		{
			m_modelName = _T("noname");
			m_modelPath = _T("");
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
				setName(_T(""));
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
	rbool flag = true;
	if (m_modelPath.empty())
	{
		flag = saveAsDlg();
	}
	if (flag)
	{
		broadcastMessage(RT_REPOSITORY_MODEL_SAVE);
	}
	return flag;
}

void RDOThreadRepository::saveAsModel()
{
	if (saveAsDlg())
	{
		broadcastMessage(RT_REPOSITORY_MODEL_SAVE);
	}
}

rbool RDOThreadRepository::saveAsDlg()
{
	OpenFile data;
	broadcastMessage(RT_REPOSITORY_MODEL_SAVE_GET_NAME, &data, true);
	if (data.m_result)
	{
		extractName(data.m_name);
		if (m_modelName.empty())
		{
			return false;
		}
		STL_FOR_ALL(m_files, it)
		{
			it->second.m_fileName = m_modelName;
		}
		return true;
	}
	else
	{
		return false;
	}
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
		m_modelName = _T("");
		m_modelPath = _T("");
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
	m_modelPath = rdo::extractFilePath(fullName);

	tstring name = fullName;
	tstring::size_type pos = name.find_last_of(_T('.'));
	if (pos != tstring::npos)
	{
		tstring s;
		s.assign(&name[0], pos);
		name = s;
	}
	static tchar szDelims[] = _T(" \t\n\r");
	name.erase(0, name.find_first_not_of(szDelims));
	name.erase(name.find_last_not_of(szDelims) + 1, tstring::npos);
	pos = name.find_last_of(_T('\\'));
	if (pos == tstring::npos)
	{
		pos = name.find_last_of(_T('/'));
	}
	if (pos != tstring::npos)
	{
		tstring s(name, pos + 1, name.length() - pos);
		setName(s);
	}
	else
	{
		setName(_T(""));
	}
}

void RDOThreadRepository::setName(CREF(tstring) name)
{
	m_modelName = name;
	static tchar szDelims[] = _T(" \t\n\r");
	m_modelName.erase(0, m_modelName.find_first_not_of(szDelims));
	m_modelName.erase(m_modelName.find_last_not_of(szDelims) + 1, tstring::npos);
	if (m_modelName.empty())
	{
		m_modelPath = _T("");
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
				std::ifstream file(fileName.c_str(), std::ios::in | std::ios::binary);
				stream << file.rdbuf();
				file.close();
			}
			else
			{
				std::ifstream file(fileName.c_str());
				stream << file.rdbuf();
				file.close();
			}
/*
			if (stream.getOpenMode() & std::ios::binary*)
			{
				std::ifstream file(fileName.c_str(), std::ios::in | std::ios::binary);
				file.seekg(0, std::ios::end);
				int len = file.tellg();
				file.seekg(0, std::ios::beg);
				stream.resize(len);
				file.read(stream.data(), len);
				file.close();
			}
			else
			{
				std::ifstream file(fileName.c_str());
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
				rdo::ofstream file(fileName.c_str(), std::ios::out | std::ios::binary);
				file << stream.rdbuf();
				file.close();
			}
			else
			{
				rdo::ofstream file(fileName.c_str());
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
	tstring rdoxFileName = m_modelPath + m_modelName + it->second.m_extention;
	if (!rdo::File::exist(rdoxFileName))
	{
		pugi::xml_document doc;
		pugi::xml_node      rootNode           = doc.append_child(_T("Settings"));
		pugi::xml_node      versionNode        = rootNode.append_child(_T("Version"));
		pugi::xml_attribute projectVersionAttr = versionNode.append_attribute(_T("ProjectVersion"));
		pugi::xml_attribute smrVersionAttr     = versionNode.append_attribute(_T("SMRVersion"));
		projectVersionAttr.set_value(_T("2"));
		smrVersionAttr    .set_value(_T("2"));

		rdo::ofstream ofs(rdoxFileName.c_str());
		if (ofs.good())
		{
			doc.save(ofs);
			m_projectName.m_fullFileName = rdoxFileName;
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
	tstring file_name = m_modelPath + name + _T(".bmp");
	if (rdo::File::exist(file_name))
	{
		std::ifstream file(file_name.c_str(), std::ios::in | std::ios::binary);
		stream << file.rdbuf();
		file.close();
		name = file_name;
	}
	else
	{
		stream.setstate(std::ios_base::badbit);
	}
}

void RDOThreadRepository::writeModelFilesInfo(REF(rdo::ofstream) stream) const
{
	stream << _T("Results_file   = ") << getFileExtName(rdoModelObjects::PMV) << _T("    ") << rdo::Time::local().asString() << std::endl;
	stream << _T("Run_file       = ") << getFileExtName(rdoModelObjects::SMR) << std::endl;
	stream << _T("Model_name     = ") << getFileName(rdoModelObjects::SMR) << std::endl;
	stream << _T("Resource_file  = ") << getFileName(rdoModelObjects::RSS) << getExtention(rdoModelObjects::RSS) << std::endl;
}

rbool RDOThreadRepository::createFile(CREF(tstring) name, CREF(tstring) ext, REF(rdo::ofstream) stream) const
{
	stringstream backupDirName;
	backupDirName << m_modelPath
	              << boost::basic_format<tchar>(_T("%1$04d-%2$02d-%3$02d %4$02d-%5$02d-%6$02d %7$s.%8$s"))
	                 % m_systemTime.date().year ()
	                 % m_systemTime.date().month()
	                 % m_systemTime.date().day  ()
	                 % m_systemTime.time_of_day().hours  ()
	                 % m_systemTime.time_of_day().minutes()
	                 % m_systemTime.time_of_day().seconds()
	                 % name
	                 % ext
	                 ;

	tstring fullFileName = backupDirName.str();
	stream.open(fullFileName.c_str(), std::ios::out | std::ios::binary);
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
		++m_firstStart;
		tstring m_buffer = rdo::format("%i", m_firstStart);
		m_traceFile.open((m_modelPath + m_buffer + getFileExtName(rdoModelObjects::TRC)).c_str(), std::ios::out | std::ios::binary);
		if (m_traceFile.is_open())
		{
			writeModelFilesInfo(m_traceFile);
			rdo::textstream model_structure;
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_STRUCTURE, &model_structure);
			m_traceFile << std::endl << model_structure.str() << std::endl;
			m_traceFile << _T("$Tracing") << std::endl;
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
		m_count++;
		//char buffer[100];
		//_itoa (m_count, buffer, 10);
		tstring buffer = rdo::format("%i", m_count);
		rdo::ofstream results_file;
		results_file.open((m_modelPath + buffer + getFileExtName(rdoModelObjects::PMV)).c_str(), std::ios::out | std::ios::binary );
		if (results_file.is_open())
		{
			writeModelFilesInfo(results_file);
			rdo::textstream stream;
			sendMessage(kernel->simulator(), RT_SIMULATOR_GET_MODEL_RESULTS_INFO, &stream);
			results_file << std::endl << stream.str() << std::endl;
			stream.str(_T(""));
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
		m_traceFile.write(_T("\n"), 1);
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
	return m_modelPath + getFileExtName(type);
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

CLOSE_RDO_REPOSITORY_NAMESPACE
