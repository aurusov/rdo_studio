// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/procgui/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <algorithm>
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/procgui/procgui_datablock.h"
// --------------------------------------------------------------------------------

OPEN_COMPILER_GUI_NAMESPACE

// --------------------------------------------------------------------------------
// -------------------- RPShapeDataBlock
// --------------------------------------------------------------------------------
RPShapeDataBlock::RPShapeDataBlock(RPShapeDataBlock::zakonRaspr zakon, const std::string& name)
	: m_zakon(zakon)
	, m_name (name )
{}

void RPShapeDataBlock::setDisp(double disp)
{
	m_disp=disp;
}

void RPShapeDataBlock::setExp(double exp)
{
	m_exp=exp;
}

void RPShapeDataBlock::setBase(int base)
{
	m_base=base;
}

void RPShapeDataBlock::setMax(double max)
{
	m_max=max;
}

// --------------------------------------------------------------------------------
// -------------------- RPShapeDataBlockCreate
// --------------------------------------------------------------------------------
RPShapeDataBlockCreate::RPShapeDataBlockCreate(RPShapeDataBlock::zakonRaspr zakon, const std::string& name)
	: RPShapeDataBlock(zakon, name)
{}

RPShapeDataBlockCreate::~RPShapeDataBlockCreate()
{}

void RPShapeDataBlockCreate::setAmount(boost::optional<std::size_t> amount)
{
	m_amount = amount;
}

void RPShapeDataBlockCreate::setStatistics(const rdo::runtime::LPIInternalStatistics& pStatistics)
{
	m_pStatistics = pStatistics;
}

// --------------------------------------------------------------------------------
// -------------------- RPShapeDataBlockTerminate
// --------------------------------------------------------------------------------
RPShapeDataBlockTerminate::RPShapeDataBlockTerminate(const std::string& name)
	:m_name(name)
{}

RPShapeDataBlockTerminate::~RPShapeDataBlockTerminate()
{}

void RPShapeDataBlockTerminate::setTermInc(int term_inc)
{
	m_term_inc = term_inc;
}

void RPShapeDataBlockTerminate::setStatistics(const rdo::runtime::LPIInternalStatistics& pStatistics)
{
	m_pStatistics = pStatistics;
}

// --------------------------------------------------------------------------------
// -------------------- RPShapeDataBlockProcess
// --------------------------------------------------------------------------------
RPShapeDataBlockProcess::RPShapeDataBlockProcess(RPShapeDataBlock::zakonRaspr zakon, const std::string& name)
	: RPShapeDataBlock(zakon,name)
{}

RPShapeDataBlockProcess::~RPShapeDataBlockProcess()
{}

void RPShapeDataBlockProcess::addAction(RPShapeDataBlockProcess::Action action)
{
	m_actionList.push_back(action);
}

void RPShapeDataBlockProcess::addRes(const std::string& resName)
{
	ASSERT(std::find(m_resNameList.begin(), m_resNameList.end(), resName) == m_resNameList.end());

	m_resNameList.push_back(resName);
}

const RPShapeDataBlockProcess::ActionList& RPShapeDataBlockProcess::getActionList() const
{
	return m_actionList;
}

const RPShapeDataBlockProcess::ResNameList& RPShapeDataBlockProcess::getResNameList() const
{
	return m_resNameList;
}

void RPShapeDataBlockProcess::setStatistics(const rdo::runtime::LPIInternalStatistics& pStatistics)
{
	m_pStatistics = pStatistics;
}

CLOSE_COMPILER_GUI_NAMESPACE
