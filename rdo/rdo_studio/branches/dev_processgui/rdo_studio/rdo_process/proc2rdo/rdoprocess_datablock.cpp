/*
 * copyright: (c) RDO-Team, 2011
 * filename : rdoprocess_datablock.cpp
 * author   : Шувалов Андрей
 * date     : 04.05.2011
 * bref     : 
 * indent   : 4T
 */

// ====================================================================== PCH
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_studio/rdo_process/proc2rdo/stdafx.h"
#include "rdo_studio/rdo_process/proc2rdo/rdoprocess_datablock.h"
// ===============================================================================

// ----------------------------------------------------------------------------
// ---------- RPShapeDataBlock
// ----------------------------------------------------------------------------
RPShapeDataBlock::RPShapeDataBlock(RPShapeDataBlock::zakonRaspr zakon, tstring name)
{
	m_zakon = zakon;
	m_name  = name;
}

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

// ----------------------------------------------------------------------------
// ---------- RPShapeDataBlockCreate
// ----------------------------------------------------------------------------
RPShapeDataBlockCreate::RPShapeDataBlockCreate(RPShapeDataBlock::zakonRaspr zakon, tstring name)
	: RPShapeDataBlock(zakon,name)
{}

RPShapeDataBlockCreate::~RPShapeDataBlockCreate()
{}

void RPShapeDataBlockCreate::setAmount(int amount)
{
	m_amount=amount;
}

// ----------------------------------------------------------------------------
// ---------- RPShapeDataBlockTerminate
// ----------------------------------------------------------------------------
RPShapeDataBlockTerminate::RPShapeDataBlockTerminate(tstring name)
	:m_name(name)
{}

RPShapeDataBlockTerminate::~RPShapeDataBlockTerminate()
{}

void RPShapeDataBlockTerminate::setTermInc(int term_inc)
{
	m_term_inc = term_inc;
}

// ----------------------------------------------------------------------------
// ---------- RPShapeDataBlockProcess
// ----------------------------------------------------------------------------
RPShapeDataBlockProcess::RPShapeDataBlockProcess(RPShapeDataBlock::zakonRaspr zakon, tstring name)
	: RPShapeDataBlock(zakon,name)
{}

RPShapeDataBlockProcess::~RPShapeDataBlockProcess()
{}

void RPShapeDataBlockProcess::addAction(RPShapeDataBlockProcess::resAction action)
{
	m_actions.push_back(action);
}

void RPShapeDataBlockProcess::addRes(tstring res)
{
	m_res.push_back(res);
}

std::list <RPShapeDataBlockProcess::resAction> RPShapeDataBlockProcess::getAction()
{
	return m_actions;
}

std::list <tstring> RPShapeDataBlockProcess::getRes()
{
	return m_res;
}