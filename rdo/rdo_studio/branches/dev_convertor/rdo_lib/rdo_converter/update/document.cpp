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
	STL_FOR_ALL_CONST(m_fileList, it)
	{
		it->second->close();
		delete it->second;
	}
}

REF(std::ofstream) Document::getStream(rdoModelObjectsConvertor::RDOFileType type)
{
	BOOST_AUTO(it, m_fileList.find(type));
	if (it == m_fileList.end())
	{
		tstring extention;
		switch (type)
		{
		case rdoModelObjectsConvertor::PAT: extention = _T("pat"); break;
		case rdoModelObjectsConvertor::RTP: extention = _T("rtp"); break;
		case rdoModelObjectsConvertor::RSS: extention = _T("rss"); break;
		case rdoModelObjectsConvertor::OPR: extention = _T("opr"); break;
		case rdoModelObjectsConvertor::FRM: extention = _T("frm"); break;
		case rdoModelObjectsConvertor::FUN: extention = _T("fun"); break;
		case rdoModelObjectsConvertor::DPT: extention = _T("dpt"); break;
		case rdoModelObjectsConvertor::SMR: extention = _T("smr"); break;
		case rdoModelObjectsConvertor::PMD: extention = _T("pmd"); break;
		case rdoModelObjectsConvertor::PMV: extention = _T("pmv"); break;
		case rdoModelObjectsConvertor::TRC: extention = _T("trc"); break;
		case rdoModelObjectsConvertor::ENV: extention = _T("env"); break;
		case rdoModelObjectsConvertor::PRC: extention = _T("prc"); break;
		default: NEVER_REACH_HERE;
		}

		tstring fileName = rdo::format(_T("%s%s.%s"), m_filePath.c_str(), m_modelName.c_str(), extention.c_str());
		PTR(std::ofstream) pStream = new std::ofstream(fileName.c_str(), std::ios::trunc | std::ios::binary);
		std::pair<FileList::iterator, rbool> result = m_fileList.insert(FileList::value_type(type, pStream));
		ASSERT(result.second);
		it = result.first;
	}
	return *it->second;
}

CLOSE_RDO_CONVERTER_NAMESPACE
