/*!
  \copyright (c) RDO-Team, 2011
  \file      document.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      01.11.2010
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/filesystem/fstream.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/update/document.h"
#include "utils/src/locale/rdolocale.h"
// --------------------------------------------------------------------------------

OPEN_RDO_CONVERTER_SMR2RDOX_NAMESPACE

//#define DUMP_DOCUMENT

// --------------------------------------------------------------------------------
// -------------------- Document
// --------------------------------------------------------------------------------
Document::Document()
{}

Document::~Document()
{
	close();
}

void Document::create(CREF(boost::filesystem::path) filePath, CREF(boost::filesystem::path) modelName)
{
	m_filePath  = filePath;
	m_modelName = modelName;
}

void Document::init(rdo::converter::smr2rdox::RDOFileTypeIn type, REF(std::ifstream) stream)
{
	Type typeOut;
	switch (type)
	{
	case rdo::converter::smr2rdox::PAT_IN: typeOut = PAT; break;
	case rdo::converter::smr2rdox::RTP_IN: typeOut = RTP; break;
	case rdo::converter::smr2rdox::RSS_IN: typeOut = RSS; break;
	case rdo::converter::smr2rdox::OPR_IN: typeOut = OPR; break;
	case rdo::converter::smr2rdox::FRM_IN: typeOut = FRM; break;
	case rdo::converter::smr2rdox::FUN_IN: typeOut = FUN; break;
	case rdo::converter::smr2rdox::DPT_IN: typeOut = DPT; break;
	case rdo::converter::smr2rdox::SMR_IN: typeOut = SMR; break;
	case rdo::converter::smr2rdox::PMD_IN: typeOut = PMD; break;
	case rdo::converter::smr2rdox::PMV_IN: typeOut = PMV; break;
	default                              : typeOut = UNDEFINED; NEVER_REACH_HERE; break;
	}
	LPMemoryStream streamOut = getMemoryStream(typeOut);
	streamOut->init(stream);
}

void Document::insertUpdate(CREF(LPDocUpdate) pUpdate)
{
	ASSERT(pUpdate);

	m_updateContainer.push_back(Update(pUpdate, false));
}

void Document::convert()
{
	LPIDocument pDocument = LPDocument(this).interface_cast<IDocument>();

#ifdef DUMP_DOCUMENT
	{
		STL_FOR_ALL_CONST(m_updateContainer, it)
		{
			LPDocUpdate pUpdate = it->first;
			ASSERT(pUpdate);
			pUpdate->dump(pDocument);
		}
	}
#endif

	STL_FOR_ALL(m_updateContainer, it)
	{
		it->second = true;
		LPDocUpdate pUpdate = it->first;
		ASSERT(pUpdate);
		pUpdate->apply(pDocument);

#ifdef DUMP_DOCUMENT
		{
			TRACE("=================\n");
			STL_FOR_ALL_CONST(m_updateContainer, it)
			{
				if (!it->second)
				{
					LPDocUpdate pUpdate = it->first;
					ASSERT(pUpdate);
					pUpdate->dump(pDocument);
				}
			}
		}
#endif
	}
}

Document::TypeOut Document::typeToOut(CREF(Type) typeIn) const
{
	switch (typeIn)
	{
	case PAT: return rdo::converter::smr2rdox::PAT_OUT;
	case RTP: return rdo::converter::smr2rdox::RTP_OUT;
	case RSS: return rdo::converter::smr2rdox::RSS_OUT;
	case FRM: return rdo::converter::smr2rdox::FRM_OUT;
	case FUN: return rdo::converter::smr2rdox::FUN_OUT;
	case DPT: return rdo::converter::smr2rdox::DPT_OUT;
	case SMR: return rdo::converter::smr2rdox::SMR_OUT;
	case PMD: return rdo::converter::smr2rdox::PMD_OUT;
	case PMV: return rdo::converter::smr2rdox::PMV_OUT;
	case TRC: return rdo::converter::smr2rdox::TRC_OUT;
	case EVN: return rdo::converter::smr2rdox::EVN_OUT;
	case PRC: return rdo::converter::smr2rdox::PRC_OUT;
	default : break;
	}
	return rdo::converter::smr2rdox::UNDEFINED_OUT;
}

void Document::close()
{
	STL_FOR_ALL_CONST(m_memoryFileList, memoryIt)
	{
		TypeOut typeOut = typeToOut(memoryIt->first);
		if (typeOut != rdo::converter::smr2rdox::UNDEFINED_OUT)
		{
			LPFileStream pFileStream = getFileStream(typeOut);
			ASSERT(pFileStream);
			memoryIt->second->get(*pFileStream.get());
		}
	}
	m_memoryFileList.clear();

	STL_FOR_ALL_CONST(m_streamFileList, fileIt)
	{
		fileIt->second->close();
	}
	m_streamFileList.clear();
}

boost::filesystem::path Document::getName(TypeOut typeOut) const
{
	tstring extention;
	switch (typeOut)
	{
	case rdo::converter::smr2rdox::PAT_OUT: extention = "pat"; break;
	case rdo::converter::smr2rdox::RTP_OUT: extention = "rtp"; break;
	case rdo::converter::smr2rdox::RSS_OUT: extention = "rss"; break;
	case rdo::converter::smr2rdox::FRM_OUT: extention = "frm"; break;
	case rdo::converter::smr2rdox::FUN_OUT: extention = "fun"; break;
	case rdo::converter::smr2rdox::DPT_OUT: extention = "dpt"; break;
	case rdo::converter::smr2rdox::SMR_OUT: extention = "smr"; break;
	case rdo::converter::smr2rdox::PMD_OUT: extention = "pmd"; break;
	case rdo::converter::smr2rdox::PMV_OUT: extention = "pmv"; break;
	case rdo::converter::smr2rdox::TRC_OUT: extention = "trc"; break;
	case rdo::converter::smr2rdox::EVN_OUT: extention = "evn"; break;
	case rdo::converter::smr2rdox::PRC_OUT: extention = "prc"; break;
	default: NEVER_REACH_HERE;
	}

	boost::filesystem::path fileName(m_filePath / m_modelName);
	fileName.replace_extension(rdo::format(".%s", extention.c_str()));
	return fileName;
}

Document::LPMemoryStream Document::getMemoryStream(Type type)
{
	BOOST_AUTO(it, m_memoryFileList.find(type));
	if (it == m_memoryFileList.end())
	{
		LPMemoryStream pMemoryStream = LPMemoryStream(new MemoryStream());
		std::pair<MemoryFileList::iterator, rbool> result = m_memoryFileList.insert(MemoryFileList::value_type(type, pMemoryStream));
		ASSERT(result.second);
		it = result.first;
	}
	return it->second;
}

Document::LPFileStream Document::getFileStream(TypeOut type)
{
	BOOST_AUTO(it, m_streamFileList.find(type));
	if (it == m_streamFileList.end())
	{
		LPFileStream pFileStream = LPFileStream(new boost::filesystem::ofstream(getName(type), std::ios::trunc | std::ios::binary));
		std::pair<StreamFileList::iterator, rbool> result = m_streamFileList.insert(StreamFileList::value_type(type, pFileStream));
		ASSERT(result.second);
		it = result.first;
	}
	return it->second;
}

void Document::insert(Type type, ruint to, CREF(tstring) value)
{
	LPMemoryStream streamOut = getMemoryStream(type);
	streamOut->insert(to, value);

	STL_FOR_ALL(m_updateContainer, it)
	{
		if (!it->second)
		{
			LPDocUpdate pUpdate = it->first;
			ASSERT(pUpdate);
			pUpdate->insert(type, to, value.length());
		}
	}
}

void Document::remove(Type type, ruint from, ruint to)
{
	LPMemoryStream streamOut = getMemoryStream(type);
	streamOut->remove(from, to);

	STL_FOR_ALL(m_updateContainer, it)
	{
		if (!it->second)
		{
			LPDocUpdate pUpdate = it->first;
			ASSERT(pUpdate);
			pUpdate->remove(type, from, to);
		}
	}
}

tstring Document::get(Type type, ruint from, ruint to)
{
	return getMemoryStream(type)->get(from, to);
}

// --------------------------------------------------------------------------------
// -------------------- MemoryStream
// --------------------------------------------------------------------------------
void Document::MemoryStream::init(REF(std::ifstream) stream)
{
	if (!m_buffer.empty())
	{
		return;
	}

	for (;;)
	{
		char byte;
		stream.get(byte);
		if (stream.eof())
		{
			break;
		}
		m_buffer.push_back(byte);
	}
}

void Document::MemoryStream::get(REF(std::ofstream) stream) const
{
	std::string result = rdo::locale::convertFromCLocale(std::string(&m_buffer[0], m_buffer.size()));
	stream << result;
}

void Document::MemoryStream::insert(ruint to, CREF(tstring) value)
{
	Buffer::iterator itTo;
	switch (to)
	{
	case IDocUpdate::Position::POSITION_BEGIN: itTo = m_buffer.begin();      break;
	case IDocUpdate::Position::POSITION_END  : itTo = m_buffer.end();        break;
	default                                  : itTo = m_buffer.begin() + to; break;
	}

	for (ruint i = 0; i < value.length(); ++i)
	{
		itTo = m_buffer.insert(itTo, value[i]);
		++itTo;
	}
}

void Document::MemoryStream::remove(ruint from, ruint to)
{
	Buffer::iterator itFrom;
	switch (from)
	{
	case IDocUpdate::Position::POSITION_BEGIN: itFrom = m_buffer.begin();        break;
	case IDocUpdate::Position::POSITION_END  : itFrom = m_buffer.end();          break;
	default                                  : itFrom = m_buffer.begin() + from; break;
	}

	Buffer::iterator itTo;
	switch (to)
	{
	case IDocUpdate::Position::POSITION_BEGIN: itTo = m_buffer.begin();      break;
	case IDocUpdate::Position::POSITION_END  : itTo = m_buffer.end();        break;
	default                                  : itTo = m_buffer.begin() + to; break;
	}

	m_buffer.erase(itFrom, itTo);
}

tstring Document::MemoryStream::get(ruint from, ruint to)
{
	Buffer::iterator itFrom = m_buffer.begin() + from;
	Buffer::iterator itTo   = m_buffer.begin() + to;
	tstring result;
	result.resize(to - from);
	std::copy(itFrom, itTo, result.begin());
	return result;
}

CLOSE_RDO_CONVERTER_SMR2RDOX_NAMESPACE
