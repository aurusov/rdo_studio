/*
 * @copyright (c) RDO-Team, 2010
 * @file      equaltype.h
 * @authors   Чирков Михаил
 * @date      03.12.10
 * @brief     
 * @indent    4T
 *********************************************************************************/

#ifndef _EQUALTYPE_H_
#define _EQUALTYPE_H_

// *********************************************************************** INCLUDES
// *********************************************************************** SYNOPSIS
// ********************************************************************************

OPEN_RDO_RUNTIME_NAMESPACE

// ********************************************************************************
// ******************** EqualType
// ********************************************************************************
enum EqualType
{
	ET_UNDEFINED,
	ET_NOCHANGE,
	ET_EQUAL,
	ET_PLUS,
	ET_MINUS,
	ET_MULTIPLY,
	ET_DIVIDE,
	ET_INCR,
	ET_DECR
};

CLOSE_RDO_RUNTIME_NAMESPACE

#endif // _EQUALTYPE_H_
