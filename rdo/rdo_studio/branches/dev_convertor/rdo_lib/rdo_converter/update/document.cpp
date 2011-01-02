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

Document::Document(CREF(tstring) filePath, CREF(tstring) modelName)
	: m_filePath (filePath )
	, m_modelName(modelName)
{}

Document::~Document()
{
	close();
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

void Document::close()
{
	STL_FOR_ALL_CONST(m_fileList, it)
	{
		it->second.m_pMemoryStream->write(*it->second.m_pFileStream.get());
		it->second.m_pFileStream->close();
	}
	m_fileList.clear();
}

void Document::MemoryStream::write(CPTR(char) buffer, ruint size)
{
	for (ruint i = 0; i < size; ++i)
	{
		m_buffer.push_back(buffer[i]);
	}
}

void Document::MemoryStream::write(REF(std::ofstream) stream) const
{
	stream.write(&m_buffer[0], m_buffer.size());
}

void Document::write(Type type, CPTR(char) buffer, ruint size)
{
	LPMemoryStream streamOut = getMemoryStream(type);
	streamOut->write(buffer, size);
}

CLOSE_RDO_CONVERTER_NAMESPACE
