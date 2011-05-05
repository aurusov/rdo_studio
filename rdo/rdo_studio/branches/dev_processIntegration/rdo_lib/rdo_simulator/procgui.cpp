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
// ---------- ProcGUIBlock
// --------------------------------------------------------------------
ProcGUIBlock::ProcGUIBlock(CREF(rdoParse::LPRDOParser) pParser, PTR(rdoRuntime::RDORuntime) pRuntime)
	: m_pParser (pParser )
	, m_pRuntime(pRuntime)
{
	ASSERT(m_pParser );
	ASSERT(m_pRuntime);

	//! Создаёт процесс
	m_pProcess = F(rdoRuntime::RDOPROCProcess)::create(_T("GuiProcess"), m_pRuntime);
	ASSERT(m_pProcess);
	m_pProcess.query_cast<ILogic>()->init(m_pRuntime);
}

ProcGUIBlock::~ProcGUIBlock()
{}

void ProcGUIBlock::Create(std::vector <double>  pParams)
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
	}
	else
	{
		CREF(rdoMBuilder::RDOResType) rtp = rtpList[rtp_name];
	}

	//! GENERATE
	int m_pGtype=static_cast<int>(pParams[0]);
	int m_pGbase=static_cast<int>(pParams[1]);
	switch(m_pGtype) // определяем активные окна исходя из закона
	{
		case 0: // константа 
		{
				
				LPIPROCBlock pBlock = F(rdoRuntime::RDOPROCGenerate)::create(m_pProcess, getConstCalc(pParams[4]));
				ASSERT(pBlock);
				break;
		}	
		case 1: // нормальный
		{
				LPIPROCBlock pBlock = F(rdoRuntime::RDOPROCGenerate)::create(m_pProcess, getNormalCalc(m_pGbase,pParams[4],pParams[5]));
				ASSERT(pBlock);
				break;
		}
		case 2: // равномерный закон
		{
				LPIPROCBlock pBlock = F(rdoRuntime::RDOPROCGenerate)::create(m_pProcess, getUniformCalc(m_pGbase,pParams[4],pParams[5]));
				ASSERT(pBlock);
				break;
		}
		case 3: // экспоненциальный
		{
				LPIPROCBlock pBlock = F(rdoRuntime::RDOPROCGenerate)::create(m_pProcess, getExpCalc(m_pGbase,pParams[4]));
				ASSERT(pBlock);
				break;
		}		
	}
}

void ProcGUIBlock::Process(std::vector <double>  pParams)
{
	int m_pActType=static_cast<int>(pParams[4]);
	switch(m_pActType)
	{
		case 0:
		{
			Advance(pParams);
			break;
		}
		case 1:
		{
			break;
		}
		case 2:
		{
			break;
		}
		case 3:
		{
			break;
		}
	}
}

void ProcGUIBlock::Advance(std::vector <double>  pParams)
{
	int m_pGtype=static_cast<int>(pParams[0]);
	int m_pGbase=static_cast<int>(pParams[1]);
	switch(m_pGtype)
	{
		case 0:
		{
			LPIPROCBlock pBlock = F(rdoRuntime::RDOPROCAdvance)::create(m_pProcess, getConstCalc(pParams[2]));
			ASSERT(pBlock);
			break;
		}
		case 1:
		{
			LPIPROCBlock pBlock = F(rdoRuntime::RDOPROCAdvance)::create(m_pProcess, getNormalCalc(m_pGbase,pParams[2],pParams[3]));
			ASSERT(pBlock);
			break;
		}
		case 2:
		{
			LPIPROCBlock pBlock = F(rdoRuntime::RDOPROCAdvance)::create(m_pProcess, getUniformCalc(m_pGbase,pParams[2],pParams[3]));
			ASSERT(pBlock);
			break;
		}
		case 3:
		{
			LPIPROCBlock pBlock = F(rdoRuntime::RDOPROCAdvance)::create(m_pProcess,  getExpCalc(m_pGbase,pParams[2]));
			ASSERT(pBlock);
			break;
		}
	}
}

void ProcGUIBlock::Terminate(std::vector <double>  pParams)
{
	//! TERMINATE
	LPIPROCBlock pBlock = F(rdoRuntime::RDOPROCTerminate)::create(m_pProcess, static_cast<int>(pParams[2]));
	ASSERT(pBlock);
}

rdoRuntime::LPRDOCalcConst ProcGUIBlock::getConstCalc(double m_pArg1)
{
	rdoRuntime::LPRDOCalcConst pCalc = rdo::Factory<rdoRuntime::RDOCalcConst>::create(m_pArg1);
	ASSERT(pCalc);
	return pCalc;
}

rdoRuntime::LPRDOCalcFunctionCall ProcGUIBlock::getNormalCalc(int m_pBase,double m_pArg1,double m_pArg2)
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

rdoRuntime::LPRDOCalcFunctionCall ProcGUIBlock::getUniformCalc(int m_pBase,double m_pArg1,double m_pArg2)
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

rdoRuntime::LPRDOCalcFunctionCall ProcGUIBlock::getExpCalc(int m_pBase,double m_pArg1)
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

CLOSE_RDO_SIMULATOR_NAMESPACE
