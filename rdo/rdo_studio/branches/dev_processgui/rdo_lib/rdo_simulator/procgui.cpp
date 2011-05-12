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
#include "rdo_lib/rdo_simulator/procgui.h"
#include "rdo_lib/rdo_mbuilder/rdo_resources.h"
// ===============================================================================

OPEN_RDO_SIMULATOR_NAMESPACE

// --------------------------------------------------------------------
// ---------- ProcGUICalc
// --------------------------------------------------------------------
ProcGUICalc::ProcGUICalc(PTR(rdoRuntime::RDORuntime) pRuntime)
	: m_pRuntime(pRuntime)
{
	ASSERT(m_pRuntime);
}

ProcGUICalc::~ProcGUICalc()
{}

rdoRuntime::LPRDOCalcConst ProcGUICalc::getConstCalc(double m_pArg1)
{
	rdoRuntime::LPRDOCalcConst pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pArg1);
	ASSERT(pCalc);
	return pCalc;
}

rdoRuntime::LPRDOCalcFunctionCall ProcGUICalc::getNormalCalc(int m_pBase,double m_pArg1,double m_pArg2)
{
	PTR(rdoRuntime::RandGeneratorNormal) pGenerator = new rdoRuntime::RandGeneratorNormal();
	rdoRuntime::LPRDOCalcSeqInit m_pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(m_pBase, pGenerator);
	ASSERT(m_pInitCalc);
	m_pRuntime->addInitCalc(m_pInitCalc);
	rdoRuntime::LPRDOCalcSeqNext m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextNormal>::create(pGenerator);
	ASSERT(m_pNextCalc);
	rdoRuntime::LPRDOCalcFunctionCall pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(m_pNextCalc);
	ASSERT(pFuctionCall);
	rdoRuntime::LPRDOCalcConst pArg1 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pArg1);
	rdoRuntime::LPRDOCalcConst pArg2 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pArg2);
	pFuctionCall->addParameter(pArg1);
	pFuctionCall->addParameter(pArg2);
	return pFuctionCall;
}

rdoRuntime::LPRDOCalcFunctionCall ProcGUICalc::getUniformCalc(int m_pBase,double m_pArg1,double m_pArg2)
{
	PTR(rdoRuntime::RandGeneratorUniform) pGenerator = new rdoRuntime::RandGeneratorUniform();
	rdoRuntime::LPRDOCalcSeqInit m_pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(m_pBase, pGenerator);
	ASSERT(m_pInitCalc);
	m_pRuntime->addInitCalc(m_pInitCalc);
	rdoRuntime::LPRDOCalcSeqNext m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextUniform>::create(pGenerator);
	ASSERT(m_pNextCalc);
	rdoRuntime::LPRDOCalcFunctionCall pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(m_pNextCalc);
	ASSERT(pFuctionCall);
	rdoRuntime::LPRDOCalcConst pArg1 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pArg1);
	rdoRuntime::LPRDOCalcConst pArg2 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pArg2);
	pFuctionCall->addParameter(pArg1);
	pFuctionCall->addParameter(pArg2);
	return pFuctionCall;
}

rdoRuntime::LPRDOCalcFunctionCall ProcGUICalc::getExpCalc(int m_pBase,double m_pArg1)
{
	PTR(rdoRuntime::RandGeneratorExponential) pGenerator = new rdoRuntime::RandGeneratorExponential();
	rdoRuntime::LPRDOCalcSeqInit m_pInitCalc = rdo::Factory<rdoRuntime::RDOCalcSeqInit>::create(m_pBase, pGenerator);
	ASSERT(m_pInitCalc);
	m_pRuntime->addInitCalc(m_pInitCalc);
	rdoRuntime::LPRDOCalcSeqNext m_pNextCalc = rdo::Factory<rdoRuntime::RDOCalcSeqNextExponential>::create(pGenerator);
	ASSERT(m_pNextCalc);
	rdoRuntime::LPRDOCalcFunctionCall pFuctionCall = rdo::Factory<rdoRuntime::RDOCalcFunctionCall>::create(m_pNextCalc);
	ASSERT(pFuctionCall);
	rdoRuntime::LPRDOCalcConst pArg1 = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pArg1);
	pFuctionCall->addParameter(pArg1);
	return pFuctionCall;
}

// --------------------------------------------------------------------
// ---------- ProcGUIProcess
// --------------------------------------------------------------------
ProcGUIProcess::ProcGUIProcess(PTR(rdoRuntime::RDORuntime) pRuntime)
	: m_pRuntime(pRuntime)
{
	ASSERT(m_pRuntime);
	m_pProcess = F(rdoRuntime::RDOPROCProcess)::create(_T("GuiProcess"), m_pRuntime);
	ASSERT(m_pProcess);
	m_pProcess.query_cast<ILogic>()->init(m_pRuntime);
}

ProcGUIProcess::~ProcGUIProcess()
{}

void ProcGUIProcess::insertBlock(CREF(LPProcGUIBlock) pBlock)
{
	ASSERT(pBlock);
	m_blockList.push_back(pBlock);
}

// ----------------------------------------------------------------------------
// ---------- ProcGUIBlock
// ----------------------------------------------------------------------------
ProcGUIBlock::ProcGUIBlock(CREF(LPProcGUIProcess) pProcess, CREF(tstring) pName)
	: m_name    (pName    )
	, m_pProcess(pProcess)
{
	ASSERT(m_pProcess);
	m_pProcess->insertBlock(this);
}

ProcGUIBlock::~ProcGUIBlock()
{}

// ----------------------------------------------------------------------------
// ---------- ProcGUIBlockGenerate
// ----------------------------------------------------------------------------
ProcGUIBlockGenerate::ProcGUIBlockGenerate(CREF(LPProcGUIProcess) pProcess, PTR(rdoRuntime::RDORuntime) pRuntime, CREF(rdoParse::LPRDOParser) pParser, CREF(RPShapeDataBlockCreate) pParams/* CREF(tstring) name, CREF(rdoRuntime::LPRDOCalc) pTimeCalc*/)
	: ProcGUIBlock(pProcess, pParams.getName())
	, ProcGUICalc    (pRuntime      )
	, m_pParser      (pParser       )
	, m_pParams      (pParams       )
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
		if (!rtpList.append(rtp))
		{
			m_pParser->error().error(rdoParse::RDOParserSrcInfo(), rdo::format(_T("Ошибка создания типа ресурса: %s"), rtp_name.c_str()));
		}
		rdoRuntime::RDOPROCTransact::s_typeID = rtp.id();
	}
	else
	{
		CREF(rdoMBuilder::RDOResType) rtp = rtpList[rtp_name];
		rdoRuntime::RDOPROCTransact::s_typeID = rtp.id();
	}

	//! GENERATE
	switch(m_pParams.getZakon()) // определяем активные окна исходя из закона
	{
	case RPShapeDataBlock::Const: // константа 
		{
			m_pBlock = F(rdoRuntime::RDOPROCGenerate)::create(m_pProcess->getProcess(), getConstCalc(m_pParams.getExp()), m_pParams.getAmount());
			ASSERT(m_pBlock);
			break;
		}	
	case RPShapeDataBlock::Normal: // нормальный
		{
			m_pBlock = F(rdoRuntime::RDOPROCGenerate)::create(m_pProcess->getProcess(), getNormalCalc(m_pParams.getBase(), m_pParams.getExp(), m_pParams.getDisp()), m_pParams.getAmount());
			ASSERT(m_pBlock);
			break;
		}
	case RPShapeDataBlock::Uniform: // равномерный закон
		{
			m_pBlock = F(rdoRuntime::RDOPROCGenerate)::create(m_pProcess->getProcess(), getUniformCalc(m_pParams.getBase(), m_pParams.getExp(), m_pParams.getDisp()), m_pParams.getAmount());
			ASSERT(m_pBlock);
			break;
		}
	case RPShapeDataBlock::Exp: // экспоненциальный
		{
			m_pBlock = F(rdoRuntime::RDOPROCGenerate)::create(m_pProcess->getProcess(), getExpCalc(m_pParams.getBase(), m_pParams.getExp()), m_pParams.getAmount());
			ASSERT(m_pBlock);
			break;
		}		
	}
}

// ----------------------------------------------------------------------------
// ---------- ProcGUIBlockTerminate
// ----------------------------------------------------------------------------
ProcGUIBlockTerminate::ProcGUIBlockTerminate(CREF(LPProcGUIProcess) pProcess, CREF(RPShapeDataBlockTerminate) pParams)
	: ProcGUIBlock(pProcess, pParams.getName())
	, m_pParams      (pParams                    )
{
	m_pBlock = F(rdoRuntime::RDOPROCTerminate)::create(m_pProcess->getProcess(), static_cast<ruint>(m_pParams.getTermInc()));
	ASSERT(m_pBlock);
}

// ----------------------------------------------------------------------------
// ---------- ProcGUIBlockProcess
// ----------------------------------------------------------------------------
ProcGUIBlockProcess::ProcGUIBlockProcess(CREF(LPProcGUIProcess) pProcess, PTR(rdoRuntime::RDORuntime) pRuntime, CREF(RPShapeDataBlockProcess) pParams)
	: ProcGUIBlock(pProcess, pParams.getName())
	, m_pParams      (pParams                    )
{
	std::list <RPShapeDataBlockProcess::resAction> action = m_pParams.getAction();
	std::list <RPShapeDataBlockProcess::resAction>::iterator it = action.begin();
	while(it != action.end())
	{
		switch(*it)
		{
			case RPShapeDataBlockProcess::Seize:
			{
				break;
			}
			case RPShapeDataBlockProcess::Advance:
			{
				LPProcGUIBlock m_pBlock;
				m_pBlock = rdo::Factory<ProcGUIAdvance>::create(pProcess, pRuntime, m_pParams);
				break;
			}
			case RPShapeDataBlockProcess::Release:
			{
				break;
			}
		}
		it++;
	}
}

// ----------------------------------------------------------------------------
// ---------- ProcGUIAdvance
// ----------------------------------------------------------------------------
ProcGUIAdvance::ProcGUIAdvance(CREF(LPProcGUIProcess) pProcess, PTR(rdoRuntime::RDORuntime) pRuntime, CREF(RPShapeDataBlockProcess) pParams)
	: ProcGUIBlock(pProcess, pParams.getName()+" "+"Advance")
	, ProcGUICalc    (pRuntime      )
	, m_pParams      (pParams       )
{
	switch(m_pParams.getZakon()) // определяем активные окна исходя из закона
	{
	case RPShapeDataBlock::Const: // константа 
		{
			m_pBlock = F(rdoRuntime::RDOPROCAdvance)::create(m_pProcess->getProcess(), getConstCalc(m_pParams.getExp()));
			ASSERT(m_pBlock);
			break;
		}	
	case RPShapeDataBlock::Normal: // нормальный
		{
			m_pBlock = F(rdoRuntime::RDOPROCAdvance)::create(m_pProcess->getProcess(), getNormalCalc(m_pParams.getBase(), m_pParams.getExp(), m_pParams.getDisp()));
			ASSERT(m_pBlock);
			break;
		}
	case RPShapeDataBlock::Uniform: // равномерный закон
		{
			m_pBlock = F(rdoRuntime::RDOPROCAdvance)::create(m_pProcess->getProcess(), getUniformCalc(m_pParams.getBase(), m_pParams.getExp(), m_pParams.getDisp()));
			ASSERT(m_pBlock);
			break;
		}
	case RPShapeDataBlock::Exp: // экспоненциальный
		{
			m_pBlock = F(rdoRuntime::RDOPROCAdvance)::create(m_pProcess->getProcess(), getExpCalc(m_pParams.getBase(), m_pParams.getExp()));
			ASSERT(m_pBlock);
			break;
		}		
	}
}

CLOSE_RDO_SIMULATOR_NAMESPACE
