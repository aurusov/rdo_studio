/*
 * copyright: (c) RDO-Team, 2010
 * filename : document.cpp
 * author   : Урусов Андрей
 * date     : 01.11.2010
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
#include "rdo_lib/rdo_converter/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_converter/update/document.h"
// ===============================================================================

OPEN_RDO_CONVERTER_NAMESPACE

// ----------------------------------------------------------------------------
// ---------- Document
// ----------------------------------------------------------------------------
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
	case rdoModelObjectsConvertor::PAT_IN: typeOut = rdoModelObjectsConvertor::PAT_OUT; break;
	case rdoModelObjectsConvertor::RTP_IN: typeOut = rdoModelObjectsConvertor::RTP_OUT; break;
	case rdoModelObjectsConvertor::RSS_IN: typeOut = rdoModelObjectsConvertor::RSS_OUT; break;
	case rdoModelObjectsConvertor::OPR_IN: return;
	case rdoModelObjectsConvertor::FRM_IN: typeOut = rdoModelObjectsConvertor::FRM_OUT; break;
	case rdoModelObjectsConvertor::FUN_IN: typeOut = rdoModelObjectsConvertor::FUN_OUT; break;
	case rdoModelObjectsConvertor::DPT_IN: typeOut = rdoModelObjectsConvertor::DPT_OUT; break;
	case rdoModelObjectsConvertor::SMR_IN: typeOut = rdoModelObjectsConvertor::SMR_OUT; break;
	case rdoModelObjectsConvertor::PMD_IN: typeOut = rdoModelObjectsConvertor::PMD_OUT; break;
	case rdoModelObjectsConvertor::PMV_IN: typeOut = rdoModelObjectsConvertor::PMV_OUT; break;
	default: NEVER_REACH_HERE;
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

	STL_FOR_ALL(m_updateContainer, it)
	{
		it->second = true;
		LPDocUpdate pUpdate = it->first;
		ASSERT(pUpdate);
		pUpdate->apply(pDocument);
	}
}

void Document::close()
{
	STL_FOR_ALL_CONST(m_fileList, it)
	{
		it->second.m_pMemoryStream->get(*it->second.m_pFileStream.get());
		it->second.m_pFileStream->close();
	}
	m_fileList.clear();
}

tstring Document::getName(Type type) const
{
	tstring extention;
	switch (type)
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
	case rdoModelObjectsConvertor::ENV_OUT: extention = _T("env"); break;
	case rdoModelObjectsConvertor::PRC_OUT: extention = _T("prc"); break;
	default: NEVER_REACH_HERE;
	}

	return rdo::format(_T("%s%s.%s"), m_filePath.c_str(), m_modelName.c_str(), extention.c_str());
}

REF(Document::TypeItem) Document::getItem(Type type)
{
	BOOST_AUTO(it, m_fileList.find(type));
	if (it == m_fileList.end())
	{
		TypeItem item;
		item.m_pFileStream   = LPFileStream  (new std::ofstream(getName(type).c_str(), std::ios::trunc | std::ios::binary));
		item.m_pMemoryStream = LPMemoryStream(new MemoryStream());
		std::pair<FileList::iterator, rbool> result = m_fileList.insert(FileList::value_type(type, item));
		ASSERT(result.second);
		it = result.first;
	}
	return it->second;
}

Document::LPFileStream Document::getFileStream(Type type)
{
	return getItem(type).m_pFileStream;
}

Document::LPMemoryStream Document::getMemoryStream(Type type)
{
	return getItem(type).m_pMemoryStream;
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

// ----------------------------------------------------------------------------
// ---------- MemoryStream
// ----------------------------------------------------------------------------
void Document::MemoryStream::init(REF(std::ifstream) stream)
{
	if (!m_buffer.empty())
	{
		return;
	}

	while (!stream.eof())
	{
		char byte;
		stream.get(byte);
		m_buffer.push_back(byte);
	}
}

void Document::MemoryStream::get(REF(std::ofstream) stream) const
{
	stream.write(&m_buffer[0], m_buffer.size());
}

void Document::MemoryStream::insert(ruint to, CREF(tstring) value)
{
	Buffer::iterator itTo = m_buffer.begin() + to;
	for (ruint i = 0; i < value.length(); ++i)
	{
		itTo = m_buffer.insert(itTo, value[i]);
		++itTo;
	}
}

void Document::MemoryStream::remove(ruint from, ruint to)
{
	Buffer::iterator itFrom = m_buffer.begin() + from;
	Buffer::iterator itTo   = m_buffer.begin() + to;
	m_buffer.erase(itFrom, itTo);
}

CLOSE_RDO_CONVERTER_NAMESPACE
