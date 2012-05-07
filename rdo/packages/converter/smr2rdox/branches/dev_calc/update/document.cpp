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
#include <boost/filesystem.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/update/document.h"
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

void Document::create(CREF(tstring) filePath, CREF(tstring) modelName)
{
	m_filePath  = filePath;
	m_modelName = modelName;
}

void Document::init(rdoModelObjectsConvertor::RDOFileTypeIn type, REF(std::ifstream) stream)
{
	Type typeOut;
	switch (type)
	{
	case rdoModelObjectsConvertor::PAT_IN: typeOut = PAT; break;
	case rdoModelObjectsConvertor::RTP_IN: typeOut = RTP; break;
	case rdoModelObjectsConvertor::RSS_IN: typeOut = RSS; break;
	case rdoModelObjectsConvertor::OPR_IN: typeOut = OPR; break;
	case rdoModelObjectsConvertor::FRM_IN: typeOut = FRM; break;
	case rdoModelObjectsConvertor::FUN_IN: typeOut = FUN; break;
	case rdoModelObjectsConvertor::DPT_IN: typeOut = DPT; break;
	case rdoModelObjectsConvertor::SMR_IN: typeOut = SMR; break;
	case rdoModelObjectsConvertor::PMD_IN: typeOut = PMD; break;
	case rdoModelObjectsConvertor::PMV_IN: typeOut = PMV; break;
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
			TRACE(_T("=================\n"));
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
	case PAT: return rdoModelObjectsConvertor::PAT_OUT;
	case RTP: return rdoModelObjectsConvertor::RTP_OUT;
	case RSS: return rdoModelObjectsConvertor::RSS_OUT;
	case FRM: return rdoModelObjectsConvertor::FRM_OUT;
	case FUN: return rdoModelObjectsConvertor::FUN_OUT;
	case DPT: return rdoModelObjectsConvertor::DPT_OUT;
	case SMR: return rdoModelObjectsConvertor::SMR_OUT;
	case PMD: return rdoModelObjectsConvertor::PMD_OUT;
	case PMV: return rdoModelObjectsConvertor::PMV_OUT;
	case TRC: return rdoModelObjectsConvertor::TRC_OUT;
	case EVN: return rdoModelObjectsConvertor::EVN_OUT;
	case PRC: return rdoModelObjectsConvertor::PRC_OUT;
	}
	return rdoModelObjectsConvertor::UNDEFINED_OUT;
}

void Document::close()
{
	STL_FOR_ALL_CONST(m_memoryFileList, memoryIt)
	{
		TypeOut typeOut = typeToOut(memoryIt->first);
		if (typeOut != rdoModelObjectsConvertor::UNDEFINED_OUT)
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

tstring Document::getName(TypeOut typeOut) const
{
	tstring extention;
	switch (typeOut)
	{
	case rdoModelObjectsConvertor::PAT_OUT: extention = _T("pat"); break;
	case rdoModelObjectsConvertor::RTP_OUT: extention = _T("rtp"); break;
	case rdoModelObjectsConvertor::RSS_OUT: extention = _T("rss"); break;
	case rdoModelObjectsConvertor::FRM_OUT: extention = _T("frm"); break;
	case rdoModelObjectsConvertor::FUN_OUT: extention = _T("fun"); break;
	case rdoModelObjectsConvertor::DPT_OUT: extention = _T("dpt"); break;
	case rdoModelObjectsConvertor::SMR_OUT: extention = _T("smr"); break;
	case rdoModelObjectsConvertor::PMD_OUT: extention = _T("pmd"); break;
	case rdoModelObjectsConvertor::PMV_OUT: extention = _T("pmv"); break;
	case rdoModelObjectsConvertor::TRC_OUT: extention = _T("trc"); break;
	case rdoModelObjectsConvertor::EVN_OUT: extention = _T("evn"); break;
	case rdoModelObjectsConvertor::PRC_OUT: extention = _T("prc"); break;
	default: NEVER_REACH_HERE;
	}

	boost::filesystem::path fileName(boost::filesystem::path(m_filePath) / m_modelName);
	fileName.replace_extension(rdo::format(_T(".%s"), extention.c_str()));
	return fileName.string();
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
		LPFileStream pFileStream = LPFileStream(new std::ofstream(getName(type).c_str(), std::ios::trunc | std::ios::binary));
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
	stream.write(&m_buffer[0], m_buffer.size());
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
