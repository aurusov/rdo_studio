/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_resources.cpp
  \author    ”русов јндрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/mbuilder/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "simulator/compiler/mbuilder/rdo_resources.h"

#include "simulator/compiler/parser/rdortp.h"
#include "simulator/compiler/parser/type/range.h"
// --------------------------------------------------------------------------------

OPEN_MBUILDER_NAMESPACE

// --------------------------------------------------------------------------------
// --------------------  онструкторы копий и операторы присваивани€
// --------------------------------------------------------------------------------
RDOResType::RDOResType(CREF(RDOResType) obj)
{
	operator=(obj);
}

RDOResType::Param::Param(CREF(RDOResType::Param) obj)
{
	operator=(obj);
}

RDOResource::RDOResource(CREF(RDOResource) obj)
{
	operator=(obj);
}

void RDOResType::operator= (CREF(RDOResType) obj)
{
	m_name   = obj.m_name;
	m_exist  = obj.m_exist;
	m_type   = obj.m_type;
	m_id     = obj.m_id;
	m_params = obj.m_params;
}

void RDOResType::Param::operator= (CREF(RDOResType::Param) obj)
{
	m_name     = obj.m_name;
	m_exist    = obj.m_exist;
	m_pType    = obj.m_pType;
	m_id       = obj.m_id;
	m_pMin     = obj.m_pMin;
	m_pMax     = obj.m_pMax;
	m_pDefault = obj.m_pDefault;
}

void RDOResource::operator= (CREF(RDOResource) obj)
{
	m_name  = obj.m_name;
	m_exist = obj.m_exist;
	m_rtp   = obj.m_rtp;
	m_id    = obj.m_id;
	m_params.clear();
	STL_FOR_ALL_CONST(obj.m_params, it)
	{
		m_params[it->first] = it->second;
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOResType
// --------------------------------------------------------------------------------
// ---- »нициализаци€ типа ресурса по существующему в пам€ти
// ---- —обирает все параметры существующего в пам€ти типа ресурса
// --------------------------------------------------------------------------------
RDOResType::RDOResType(CREF(rdoParser::LPRDORTPResType) rtp)
	: rdoParser::RDOParserSrcInfo(rtp->src_info()              )
	, m_name (rtp->name()                                     )
	, m_type (rtp->isPermanent() ? rt_permanent : rt_temporary)
	, m_exist(true                                            )
	, m_id   (rtp->getNumber()                                )
{
	STL_FOR_ALL_CONST(rtp->getParams(), param_it)
	{
		Param param(*param_it);
		param.m_id = m_params.size();
		m_params.append(param);
	}
}

RDOResType::Param::Param(CREF(rdoParser::LPRDORTPParam) param)
	: rdoParser::RDOParserSrcInfo(param->src_info())
	, m_name    (param->name()                    )
	, m_pType   (param->getTypeInfo()             )
	, m_exist   (true                             )
	, m_id      (-1                               )
	, m_pDefault(param->getDefault()              )
{
	switch (typeID())
	{
		case rdo::runtime::RDOType::t_int:
		{
			rdoParser::LPRDOTypeIntRange pRange = param->getTypeInfo()->type().object_dynamic_cast<rdoParser::RDOTypeIntRange>();
			if (pRange)
			{
				m_pMin = pRange->range()->getMin();
				m_pMax = pRange->range()->getMax();
			}
			break;
		}
		case rdo::runtime::RDOType::t_real:
		{
			rdoParser::LPRDOTypeRealRange pRange = param->getTypeInfo()->type().object_dynamic_cast<rdoParser::RDOTypeRealRange>();
			if (pRange)
			{
				m_pMin = pRange->range()->getMin();
				m_pMax = pRange->range()->getMax();
			}
			break;
		}
	}
}

// --------------------------------------------------------------------------------
// ---- »нициализаци€ *нового* типа ресурса
// --------------------------------------------------------------------------------
RDOResType::RDOResType(CREF(tstring) name, Type type)
	: m_name (name )
	, m_type (type )
	, m_exist(false)
	, m_id   (-1   )
{}

rbool RDOResType::ParamList::append(REF(Param) param)
{
	if (std::find_if(begin(), end(), rdoParser::compareNameRef<Param>(param.name())) != end())
	{
		return false;
	}

	m_list.push_back(param);
	return true;
}

RDOResType::Param::Param(CREF(tstring) name, CREF(rdoParser::LPTypeInfo) pType, CREF(rdoParser::LPRDOValue) pDefault)
	: m_name    (name    )
	, m_pType   (pType   )
	, m_exist   (true    )
	, m_id      (-1      )
	, m_pDefault(pDefault)
{}

RDOResType::Param::Param(CREF(tstring) name, CREF(rdo::intrusive_ptr<rdoParser::RDOType__int>) pType, CREF(rdoParser::LPRDOValue) pDefault)
	: m_name    (name    )
	, m_exist   (true    )
	, m_id      (-1      )
	, m_pDefault(pDefault)
{
	initType(pType);
}

RDOResType::Param::Param(CREF(tstring) name, CREF(rdo::intrusive_ptr<rdoParser::RDOType__real>) pType, CREF(rdoParser::LPRDOValue) pDefault)
	: m_name    (name    )
	, m_exist   (true    )
	, m_id      (-1      )
	, m_pDefault(pDefault)
{
	initType(pType);
}

RDOResType::Param::Param(CREF(tstring) name, CREF(rdo::runtime::RDOEnumType::Enums) enums, CREF(rdoParser::LPRDOValue) pDefault)
	: m_name    (name    )
	, m_exist   (true    )
	, m_id      (-1      )
	, m_pDefault(pDefault)
{
	initType(enums);
}

void RDOResType::Param::setRange(CREF(rdoParser::LPRDOValue) pMin, CREF(rdoParser::LPRDOValue) pMax)
{
	ASSERT(pMin);
	ASSERT(pMax);

	m_pMin = pMin;
	m_pMax = pMax;
}

void RDOResType::Param::setDefault(CREF(rdoParser::LPRDOValue) pDefault)
{
	ASSERT(pDefault);

	m_pDefault = pDefault;
}

rbool RDOResType::Param::operator== (CREF(Param) param) const
{
	UNUSED(param);
	NEVER_REACH_HERE;
	return true;
}

// --------------------------------------------------------------------------------
// -------------------- RDOResTypeList
// --------------------------------------------------------------------------------
// ---- —обирает все типы ресурсов, которые есть у парсера
// --------------------------------------------------------------------------------
RDOResTypeList::RDOResTypeList(CREF(rdoParser::LPRDOParser) pParser)
	: RDOList<RDOResType>(pParser)
{
	STL_FOR_ALL_CONST(m_pParser->getRTPResTypes(), rtp_it)
	{
		RDOResType rtp(*rtp_it);
		m_list.push_back(rtp);
	}
}

// --------------------------------------------------------------------------------
// ---- ƒобавление *нового* типа ресурса
// --------------------------------------------------------------------------------
rdoParser::LPRDORTPResType RDOResTypeList::appendBefore(REF(RDOResType) rtp)
{
	if (std::find_if(begin(), end(), rdoParser::compareNameRef<RDOResType>(rtp.name())) != end())
	{
		return rdoParser::LPRDORTPResType(NULL);
	}

	rdoParser::LPRDORTPResType pResourceType = rdo::Factory<rdoParser::RDORTPResType>::create(m_pParser, rdoParser::RDOParserSrcInfo(rtp.name()), rtp.isPermanent());
	ASSERT(pResourceType);
	return pResourceType;
}

rbool RDOResTypeList::appendAfter(REF(RDOResType) rtp, CREF(rdoParser::LPRDORTPResType) pResourceType)
{
	ASSERT(pResourceType);

	STL_FOR_ALL_CONST(rtp.m_params, param)
	{
		rdoParser::LPTypeInfo pParamType;
		rdoParser::LPRDOValue pDefault = param->hasDefault() ? param->getDefault() : rdo::Factory<rdoParser::RDOValue>::create();
		ASSERT(pDefault);
		switch (param->typeID())
		{
			case rdo::runtime::RDOType::t_int:
			{
				if (param->hasRange())
				{
					rdoParser::LPRDOTypeRangeRange pRange    = rdo::Factory<rdoParser::RDOTypeRangeRange>::create(param->getMin(), param->getMax(), rdoParser::RDOParserSrcInfo());
					rdoParser::LPRDOTypeIntRange   pIntRange = rdo::Factory<rdoParser::RDOTypeIntRange>::create(pRange);
					pParamType = rdo::Factory<rdoParser::TypeInfo>::create(pIntRange, rdoParser::RDOParserSrcInfo());
				}
				else
				{
					pParamType = param->type();
				}
				break;
			}
			case rdo::runtime::RDOType::t_real:
			{
				if (param->hasRange())
				{
					rdoParser::LPRDOTypeRangeRange pRange     = rdo::Factory<rdoParser::RDOTypeRangeRange>::create(param->getMin(), param->getMax(), rdoParser::RDOParserSrcInfo());
					rdoParser::LPRDOTypeRealRange  pRealRange = rdo::Factory<rdoParser::RDOTypeRealRange>::create(pRange);
					pParamType = rdo::Factory<rdoParser::TypeInfo>::create(pRealRange, rdoParser::RDOParserSrcInfo());
				}
				else
				{
					pParamType = param->type();
				}
				break;
			}
			case rdo::runtime::RDOType::t_enum:
			{
				pParamType = param->type();
				break;
			}
			default:
			{
				m_pParser->removeRTPResType(pResourceType);
				return false;
			}
		}
		rdoParser::LPRDORTPParam pParam = rdo::Factory<rdoParser::RDORTPParam>::create(pParamType, pDefault, rdoParser::RDOParserSrcInfo(param->name()));
		ASSERT(pParam);
		pResourceType->addParam(pParam);
	}
	rtp.m_exist = true;
	rtp.m_id    = pResourceType->getNumber();
	m_list.push_back(rtp);
	return true;
}

// --------------------------------------------------------------------------------
// -------------------- RDOResource
// --------------------------------------------------------------------------------
// ---- »нициализаци€ ресурса по существующему в пам€ти
// ---- —обирает все параметры существующего в пам€ти ресурса
// --------------------------------------------------------------------------------
RDOResource::RDOResource(CREF(rdoParser::LPRDORSSResource) rss)
	: rdoParser::RDOParserSrcInfo(rss->src_info())
	, m_name (rss->name()                       )
	, m_rtp  (rss->getType()                    )
	, m_exist(true                              )
	, m_id   (rss->getID()                      )
{
	if (m_rtp.m_params.size() == rss->params().size())
	{
		ruint index = 0;
		STL_FOR_ALL_CONST(m_rtp.m_params, param_it)
		{
			m_params[param_it->name()] = rss->params()[index].param();
			index++;
		}
	}
}

RDOResource::Params::const_iterator RDOResource::operator[] (CREF(tstring) param) const
{
	return m_params.find(param);
}

REF(RDOResource::Params::mapped_type) RDOResource::operator[] (CREF(tstring) param)
{
	RDOResource::Params::iterator param_it = m_params.find(param);
	if (param_it != m_params.end())
	{
		return param_it->second;
	}
	else
	{
		static Params::mapped_type tmpValue;
		return tmpValue;
	}
}

rdoParser::LPRDORSSResource RDOResource::getParserResource(CREF(rdoParser::LPRDOParser) pParser) const
{
	if (!exist())
	{
		return rdoParser::LPRDORSSResource(NULL);
	}

	return pParser->findRSSResource(name());
}

rbool RDOResource::fillParserResourceParams(REF(rdoParser::LPRDORSSResource) pToParserRSS) const
{
	STL_FOR_ALL_CONST(getType().m_params, param_it)
	{
		RDOResource::Params::const_iterator value_it = operator[](param_it->name());
		if (value_it == end())
			return false;

		rdoParser::LPRDOValue pValue = rdo::Factory<rdoParser::RDOValue>::create(value_it->second);
		ASSERT(pValue);
		pValue = param_it->type()->value_cast(pValue);
		/// @todo а почему тут toParserRSS->src_info(), а не value_it->src_info() ?
		pValue->setSrcInfo(pToParserRSS->src_info());
		pToParserRSS->addParam(pValue);
	}
	return true;
}

// --------------------------------------------------------------------------------
// ---- »нициализаци€ *нового* ресурса
// --------------------------------------------------------------------------------
RDOResource::RDOResource(CREF(RDOResType) rtp, CREF(tstring) name)
	: m_name (name                                  )
	, m_rtp  (rtp                                   )
	, m_exist(false                                 )
	, m_id   (rdoParser::RDORSSResource::UNDEFINED_ID)
{
	STL_FOR_ALL_CONST(m_rtp.m_params, param_it)
	{
		rdoParser::LPRDOValue pValue = rdo::Factory<rdoParser::RDOValue>::create(param_it->type(), param_it->src_info());
		ASSERT(pValue);
		if (param_it->hasDefault())
		{
			pValue = param_it->getDefault();
		}
		else if (param_it->hasRange())
		{
			pValue = param_it->getMin();
		}
		m_params[param_it->name()] = pValue;
	}
}

// --------------------------------------------------------------------------------
// -------------------- RDOResourceList
// --------------------------------------------------------------------------------
// ---- —обирает все ресурсы, которые есть у парсера
// --------------------------------------------------------------------------------
RDOResourceList::RDOResourceList(CREF(rdoParser::LPRDOParser) pParser)
	: RDOList<RDOResource>(pParser)
{
	STL_FOR_ALL_CONST(m_pParser->getRSSResources(), rss_it)
	{
		RDOResource rss(*rss_it);
		m_list.push_back(rss);
	}
}

CLOSE_MBUILDER_NAMESPACE
