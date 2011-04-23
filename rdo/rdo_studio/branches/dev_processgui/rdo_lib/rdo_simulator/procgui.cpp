/*
 * copyright: (c) RDO-Team, 2011
 * filename : procgui.cpp
 * author   : Шувалов Андрей
 * date     : 22.04.2011
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_simulator/ProcGUI.h"
#include "rdo_lib/rdo_mbuilder/rdo_resources.h"
// ===============================================================================

// --------------------------------------------------------------------
// ---------- ProcGUIBlock
// --------------------------------------------------------------------
ProcGUIBlock::ProcGUIBlock(PTR(rdoParse::RDOParser) pParser, PTR(rdoRuntime::RDORuntime) pRuntime)	
	: m_pParser (pParser )
	, m_pRuntime(pRuntime)
{
	//! Создает процесс
	m_pProcess = F(rdoRuntime::RDOPROCProcess)::create(_T("GuiProcess"), m_pRuntime);
	ASSERT(m_pProcess);
	m_pProcess.query_cast<ILogic>()->init(m_pRuntime);
}

ProcGUIBlock::~ProcGUIBlock()
{}

void ProcGUIBlock::Create()
{
	tstring rtp_name       = _T("Транзакты");
	tstring rtp_param_name = _T("Время_создания");

	//! Получили список всех типов ресурсов
	rdoMBuilder::RDOResTypeList rtpList(m_pParser);
	//! Найти тип ресурса, если его нет, то создать
	if (!rtpList[rtp_name].exist())
	{
		//! Создадим тип ресурса
		rdoMBuilder::RDOResType rtp(rtp_name);
		//! Добавим параметр Время_создания
		rtp.m_params.append(rdoMBuilder::RDOResType::Param(rtp_param_name, rdo::Factory<rdoParse::RDOType__real>::create()));
		//! Добавим тип ресурса
		rdoRuntime::RDOPROCTransact::typeID = rtp.id();
	}
	else
	{
		CREF(rdoMBuilder::RDOResType) rtp = rtpList[rtp_name];
		rdoRuntime::RDOPROCTransact::typeID = rtp.id();
	}

	//! GENERATE
	rdoRuntime::LPRDOCalcConst pCalc  = rdo::Factory<rdoRuntime::RDOCalcConst>::create(4);
	LPIPROCBlock pBlock = F(rdoRuntime::RDOPROCGenerate)::create(m_pProcess, pCalc);
	ASSERT(pBlock);

	//! TERMINATE
	pBlock = F(rdoRuntime::RDOPROCTerminate)::create(m_pProcess, 1);
	ASSERT(pBlock);
}
