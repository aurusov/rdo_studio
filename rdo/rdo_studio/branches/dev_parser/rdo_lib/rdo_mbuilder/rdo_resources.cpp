#include "rdo_lib/rdo_mbuilder/pch.h"
#include "rdo_lib/rdo_mbuilder/rdo_resources.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdo_type_range.h"

namespace rdoMBuilder
{

// --------------------------------------------------------------------
// ----------  онструкторы копий и операторы присваивани€
// --------------------------------------------------------------------
RDOResType::RDOResType(CREF(RDOResType) obj)
{
	operator=( obj );
}

RDOResType::Param::Param(CREF(RDOResType::Param) obj)
{
	operator=( obj );
}

RDOResource::RDOResource(CREF(RDOResource) obj)
{
	operator=( obj );
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
	m_name  = obj.m_name;
	m_exist = obj.m_exist;
	m_type  = obj.m_type;
	m_id    = obj.m_id;
	m_min   = obj.m_min;
	m_max   = obj.m_max;
}

void RDOResource::operator= (CREF(RDOResource) obj)
{
	m_name  = obj.m_name;
	m_exist = obj.m_exist;
	m_rtp   = obj.m_rtp;
	m_id    = obj.m_id;
	m_params.clear();
	for (Params::const_iterator it = obj.m_params.begin(); it != obj.m_params.end(); ++it)
		m_params[it->first] = it->second;
}

// --------------------------------------------------------------------
// ---------- RDOResType
// --------------------------------------------------------------------
// ---- »нициализаци€ типа ресурса по существующему в пам€ти
// ---- —обирает все параметры существующего в пам€ти типа ресурса
// --------------------------------------------------------------------
RDOResType::RDOResType(CREF(rdoParse::LPRDORTPResType) rtp)
	: rdoParse::RDOParserSrcInfo(rtp->src_info()              )
	, m_name (rtp->name()                                     )
	, m_type (rtp->isPermanent() ? rt_permanent : rt_temporary)
	, m_exist(true                                            )
	, m_id   (rtp->getNumber()                                )
{
	STL_FOR_ALL_CONST(rdoParse::RDORTPResType::ParamList, rtp->getParams(), param_it)
	{
		Param param(*param_it);
		param.m_id = m_params.size();
		m_params.append(param);
	}
}

RDOResType::Param::Param(CREF(rdoParse::LPRDORTPParam) param)
	: rdoParse::RDOParserSrcInfo(param->src_info())
	, m_name (param->name()                       )
	, m_type (param->getParamType()               )
	, m_exist(true                                )
	, m_id   (-1                                  )
{
	switch ( typeID() )
	{
		case rdoRuntime::RDOType::t_int:
		{
			if (dynamic_cast<PTR(rdoParse::RDOTypeIntRange)>(param->getParamType()->type().get()))
			{
				rdoParse::LPRDOTypeIntRange pRange = param->getParamType()->type().lp_cast<rdoParse::LPRDOTypeIntRange>();
				m_min = pRange->range()->getMin();
				m_max = pRange->range()->getMax();
			}
			break;
		}
		case rdoRuntime::RDOType::t_real:
		{
			if (dynamic_cast<PTR(rdoParse::RDOTypeRealRange)>(param->getParamType()->type().get()))
			{
				rdoParse::LPRDOTypeRealRange pRange = param->getParamType()->type().lp_cast<rdoParse::LPRDOTypeRealRange>();
				m_min = pRange->range()->getMin();
				m_max = pRange->range()->getMax();
			}
			break;
		}
	}
}

// --------------------------------------------------------------------
// ---- »нициализаци€ *нового* типа ресурса
// --------------------------------------------------------------------
RDOResType::RDOResType(CREF(tstring) name, Type type)
	: m_name (name )
	, m_type (type )
	, m_exist(false)
	, m_id   (-1   )
{}

rbool RDOResType::ParamList::append(REF(Param) param)
{
	if (std::find_if(begin(), end(), rdoParse::compareNameRef<Param>(param.name())) != end())
		return false;

	m_list.push_back( param );
	return true;
}

RDOResType::Param::Param(CREF(tstring) name, CREF(rdoParse::LPRDOTypeParam) type)
	: m_name (name)
	, m_type (type)
	, m_exist(true)
	, m_id   (-1  )
{}

RDOResType::Param::Param(CREF(tstring) name, CREF(rdo::intrusive_ptr<rdoParse::RDOType__int>) type, CREF(rdoParse::RDOValue) default)
	: m_name (name)
	, m_exist(true)
	, m_id   (-1  )
{
	initType  (type   );
	setDefault(default);
}

RDOResType::Param::Param(CREF(tstring) name, CREF(rdo::intrusive_ptr<rdoParse::RDOType__real>) type, CREF(rdoParse::RDOValue) default)
	: m_name (name)
	, m_exist(true)
	, m_id   (-1  )
{
	initType  (type   );
	setDefault(default);
}

RDOResType::Param::Param(CREF(tstring) name, CREF(rdoRuntime::RDOEnumType::Enums) enums, CREF(rdoParse::RDOValue) default)
	: m_name (name)
	, m_exist(true)
	, m_id   (-1  )
{
	initType  (enums  );
	setDefault(default);
}

void RDOResType::Param::setDiap(CREF(rdoParse::RDOValue) min, CREF(rdoParse::RDOValue) max)
{
	m_min = min;
	m_max = max;
}

void RDOResType::Param::setDefault(CREF(rdoParse::RDOValue) default)
{
	ASSERT(m_type);
	m_type = rdo::Factory<rdoParse::RDOTypeParam>::create(m_type->type(), default, rdoParse::RDOParserSrcInfo());
	ASSERT(m_type);
}

rbool RDOResType::Param::operator== (CREF(Param) param) const
{
	NEVER_REACH_HERE;
	return true;
}

// --------------------------------------------------------------------
// ---------- RDOResTypeList
// --------------------------------------------------------------------
// ---- —обирает все типы ресурсов, которые есть у парсера
// --------------------------------------------------------------------
RDOResTypeList::RDOResTypeList(PTR(rdoParse::RDOParser) parser)
	: RDOList<RDOResType>(parser)
{
	STL_FOR_ALL_CONST(rdoParse::RDOParser::RTPResTypeList, m_parser->getRTPResTypes(), rtp_it)
	{
		RDOResType rtp(*rtp_it);
		m_list.push_back(rtp);
	}
}

// --------------------------------------------------------------------
// ---- ƒобавление *нового* типа ресурса
// --------------------------------------------------------------------
rbool RDOResTypeList::append(REF(RDOResType) rtp)
{
	if (std::find_if(begin(), end(), rdoParse::compareNameRef<RDOResType>(rtp.name())) != end())
		return false;

	rdoParse::LPRDORTPResType pResourceType = rdo::Factory<rdoParse::RDORTPResType>::create(m_parser, rdoParse::RDOParserSrcInfo(rtp.name()), rtp.isPermanent());
	STL_FOR_ALL_CONST(RDOResType::ParamList::List, rtp.m_params, param)
	{
		rdoParse::LPRDOTypeParam pParamType;
		switch (param->typeID())
		{
			case rdoRuntime::RDOType::t_int:
			{
				rdoParse::RDOValue default = param->hasDefault() ? param->getDefault() : rdoParse::RDOValue();
				if (param->hasDiap())
				{
					rdoParse::LPRDOTypeRangeRange pRange    = rdo::Factory<rdoParse::RDOTypeRangeRange>::create(param->getMin(), param->getMax(), rdoParse::RDOParserSrcInfo());
					rdoParse::LPRDOTypeIntRange   pIntRange = rdo::Factory<rdoParse::RDOTypeIntRange>::create(pRange);
					pParamType = rdo::Factory<rdoParse::RDOTypeParam>::create(pIntRange, default, rdoParse::RDOParserSrcInfo());
				}
				else
				{
					pParamType = param->type();
				}
				break;
			}
			case rdoRuntime::RDOType::t_real:
			{
				rdoParse::RDOValue default = param->hasDefault() ? param->getDefault() : rdoParse::RDOValue();
				if (param->hasDiap())
				{
					rdoParse::LPRDOTypeRangeRange pRange     = rdo::Factory<rdoParse::RDOTypeRangeRange>::create(param->getMin(), param->getMax(), rdoParse::RDOParserSrcInfo());
					rdoParse::LPRDOTypeRealRange  pRealRange = rdo::Factory<rdoParse::RDOTypeRealRange>::create(pRange);
					pParamType = rdo::Factory<rdoParse::RDOTypeParam>::create(pRealRange, default, rdoParse::RDOParserSrcInfo());
				}
				else
				{
					pParamType = param->type();
				}
				break;
			}
			case rdoRuntime::RDOType::t_enum:
			{
				pParamType = param->type();
				break;
			}
			default:
			{
				m_parser->removeRTPResType(pResourceType);
				return false;
			}
		}
		rdoParse::LPRDORTPParam pParam = rdo::Factory<rdoParse::RDORTPParam>::create(pResourceType, pParamType, rdoParse::RDOParserSrcInfo(param->name()));
		ASSERT(pParam);
		pResourceType->addParam(pParam);
	}
	rtp.m_exist = true;
	rtp.m_id    = pResourceType->getNumber();
	m_list.push_back(rtp);
	return true;
}

// --------------------------------------------------------------------
// ---------- RDOResource
// --------------------------------------------------------------------
// ---- »нициализаци€ ресурса по существующему в пам€ти
// ---- —обирает все параметры существующего в пам€ти ресурса
// --------------------------------------------------------------------
RDOResource::RDOResource(CREF(rdoParse::RDORSSResource) rss)
	: rdoParse::RDOParserSrcInfo(rss.src_info())
	, m_name (rss.name()                       )
	, m_rtp  (rss.getType()                    )
	, m_exist(true                             )
	, m_id   (rss.getID()                      )
{
	if (m_rtp.m_params.size() == rss.params().size())
	{
		ruint index = 0;
		STL_FOR_ALL_CONST(RDOResType::ParamList::List, m_rtp.m_params, param_it)
		{
			m_params[param_it->name()] = rss.params()[index].param();
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

CPTR(rdoParse::RDORSSResource) RDOResource::getParserResource(CREF(rdoParse::RDOParser) parser) const
{
	if (!exist())
		return NULL;

	return parser.findRSSResource(name());
}

rbool RDOResource::fillParserResourceParams(PTR(rdoParse::RDORSSResource) toParserRSS) const
{
	STL_FOR_ALL_CONST(RDOResType::ParamList::List, getType().m_params, param_it)
	{
		RDOResource::Params::const_iterator value_it = operator[](param_it->name());
		if (value_it == end())
			return false;

		rdoParse::RDOValue value(value_it->second);
		value = param_it->type()->value_cast(value);
		//! TODO: а почему тут toParserRSS->src_info(), а не value_it->src_info() ?
		value.setSrcInfo(toParserRSS->src_info());
		toParserRSS->addParam(value);
	}
	return true;
}

// --------------------------------------------------------------------
// ---- »нициализаци€ *нового* ресурса
// --------------------------------------------------------------------
RDOResource::RDOResource(CREF(RDOResType) rtp, CREF(tstring) name)
	: m_name (name                                  )
	, m_rtp  (rtp                                   )
	, m_exist(false                                 )
	, m_id   (rdoParse::RDORSSResource::UNDEFINED_ID)
{
	STL_FOR_ALL_CONST(RDOResType::ParamList::List, m_rtp.m_params, param_it)
	{
		rdoParse::RDOValue value(param_it->type()->type());
		if (param_it->hasDefault())
		{
			value = param_it->getDefault();
		}
		else if (param_it->hasDiap())
		{
			value = param_it->getMin();
		}
		m_params[param_it->name()] = value;
	}
}

// --------------------------------------------------------------------
// ---------- RDOResourceList
// --------------------------------------------------------------------
// ---- —обирает все ресурсы, которые есть у парсера
// --------------------------------------------------------------------
RDOResourceList::RDOResourceList(PTR(rdoParse::RDOParser) parser)
	: RDOList<RDOResource>(parser)
{
	STL_FOR_ALL_CONST(rdoParse::RDOParser::RSSResourceList, m_parser->getRSSResources(), rss_it)
	{
		RDOResource rss(**rss_it);
		m_list.push_back(rss);
	}
}

} // rdoMBuilder
