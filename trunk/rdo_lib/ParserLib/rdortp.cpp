#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdortp.h"
#include "rdoparser.h"
#include "rdoDefines.h"
#include "rdoparser_lexer.h"

namespace rdoParse 
{

int rtplex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOFlexLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOFlexLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOFlexLexer*>(lexer)->yylex();
}
void rtperror( char* mes )
{
	rdoParse::currParser->error( mes );
}

void RDORTPEnum::add(const std::string *const next) 
{ 
	if(std::find_if(enumVals.begin(), enumVals.end(), comparePointers<std::string>(next)) != enumVals.end())
		currParser->error(("Second appearance of the same value name: " + *next).c_str());

	enumVals.push_back(next); 
}

int RDORTPEnum::findValue(const std::string *const val) const
{
	std::vector<const std::string *>::const_iterator it = 
		std::find_if(enumVals.begin(), enumVals.end(), comparePointers<std::string>(val));

	if(it == enumVals.end())
		currParser->error("Wrong parameter value: " + *val);

//	return (it - enumVals.begin()) + 1;
	return it - enumVals.begin();
}

void RDORTPResType::add(const RDORTPParamDesc *const _param)
{
	if(findRTPParam(_param->getName()))
		currParser->error("Second appearance of the same parameter name: " + *(_param->getName()));

	params.push_back(_param); 

}
const RDORTPParamDesc *RDORTPResType::findRTPParam(const std::string *const param) const
{
	std::vector<const RDORTPParamDesc *>::const_iterator it = std::find_if(params.begin(), 
		params.end(), 
		compareName<RDORTPParamDesc>(param));
	if(it != params.end())
		return (*it);

	return NULL;
}

int RDORTPResType::getRTPParamNumber(const std::string *const param) const
{
	std::vector<const RDORTPParamDesc *>::const_iterator it = std::find_if(params.begin(), 
		params.end(), 
		compareName<RDORTPParamDesc>(param));
	if(it != params.end())
		return it - params.begin();

	return -1;
}

int RDORTPResType::writeModelStructure() const
{
	int s1 = getType();
	std::string s2 = *getName();
	int s3 = getParams().size();
	currParser->modelStructure << getType() << " " << *getName() << " " << getParams().size() << std::endl;
	for(int i = 0; i < getParams().size(); i++)
	{
		currParser->modelStructure << "  " << (i+1) << " ";
		getParams().at(i)->writeModelStructure();
	}
	return 0;
}

int RDORTPParamDesc::writeModelStructure() const
{
	currParser->modelStructure << *getName() << " ";
	getType()->writeModelStructure();
//	currParser->modelStructure << " ";
	return 0;
}

int RDORTPIntResParam::writeModelStructure() const
{
	currParser->modelStructure << "I" << std::endl;
	return 0;
}

int RDORTPRealResParam::writeModelStructure() const
{
	currParser->modelStructure << "R" << std::endl;
	return 0;
}

int RDORTPEnumResParam::writeModelStructure() const
{
	currParser->modelStructure << "E " << enu->enumVals.size() << std::endl;
	for(int i = 0; i < enu->enumVals.size(); i++)
		currParser->modelStructure << "    " << i << " " << *enu->enumVals.at(i) << std::endl;
	return 0;
}

const RDORTPResParam *RDORTPResParam::constructSuchAs() const 
{ 
	return this; 
}
const RDORTPResParam *RDORTPResParam::constructSuchAs(const int defVal) const
{
	std::ostringstream str;
	str << "Invalid default value: " << defVal;
	currParser->error(str.str().c_str());
	return NULL;	// unreachable code...
}
const RDORTPResParam *RDORTPResParam::constructSuchAs(const double *const defVal) const
{
	std::ostringstream str;
	str << "Invalid default value: " << *defVal;
	currParser->error(str.str().c_str());
	return NULL;	// unreachable code...
}
const RDORTPResParam *RDORTPResParam::constructSuchAs(const std::string *const defVal) const 
{
	currParser->error(("Invalid default value: " + *defVal).c_str());
	return NULL;	// unreachable code...
}

RDOValue RDORTPResParam::getRSSEnumValue(const std::string *const val) const 
{
	std::ostringstream str;
	str << "Unexpected parameter value: " << *val;
	currParser->error(str.str().c_str());
	return NULL;	// unreachable code...
}
RDOValue RDORTPResParam::getRSSIntValue(const int val) const 
{
	std::ostringstream str;
	str << "Unexpected parameter value: " << val;
	currParser->error(str.str().c_str());
	return NULL;	// unreachable code...
}
RDOValue RDORTPResParam::getRSSRealValue(const double *const val) const
{
	std::ostringstream str;
	str << "Unexpected parameter value: " << *val;
	currParser->error(str.str().c_str());
	return NULL;	// unreachable code...
}

RDOValue RDORTPIntResParam::getRSSDefaultValue()const 
{
	if(!dv->exist)
		currParser->error("No default value");

	return RDOValue(dv->GetIntValue());
}

RDOValue RDORTPIntResParam::getRSSIntValue(const int val) const 
{
	if(diap->exist)
	{
		if((val < diap->minVal) || (val > diap->maxVal))
			currParser->error(("integer value " + toString(val) + " out of range[" + toString(diap->minVal) + ", " + toString(diap->maxVal) + "]").c_str());
	}
	return RDOValue(val);
}

RDOValue RDORTPRealResParam::getRSSDefaultValue() const 
{
	if(!dv->exist)
		currParser->error("No default value");

	return RDOValue(dv->GetRealValue());
}

RDOValue RDORTPRealResParam::getRSSRealValue(const double *const val) const 
{
	if(diap->exist)
	{
		if((*val < diap->minVal) || (*val > diap->maxVal))
			currParser->error(("real value " + toString(*val) + " out of range[" + toString(diap->minVal) + ", " + toString(diap->maxVal) + "]").c_str());
	}
	return RDOValue(*val);
}

RDOValue RDORTPRealResParam::getRSSIntValue(const int val) const
{
	if(diap->exist)
	{
		if((val < diap->minVal) || (val > diap->maxVal))
			currParser->error(("real value " + toString(val) + " out of range[" + toString(diap->minVal) + ", " + toString(diap->maxVal) + "]").c_str());
	}
	return RDOValue(val);
}

RDOValue RDORTPEnumResParam::getRSSDefaultValue() const 
{
	if(!dv->exist)
		currParser->error("No default value");

	return RDOValue(enu->findValue(dv->GetEnumValue()));
}
RDOValue RDORTPEnumResParam::getRSSEnumValue(const std::string *const val) const
{
	return enu->findValue(val);
}

RDORTPIntResParam::RDORTPIntResParam(RDORTPIntDiap *_diap, RDORTPIntDefVal *_dv): 
	diap(_diap), 
	RDORTPResParam(_dv) 
{
	diap->check(_dv);
}

void RDORTPIntDiap::check(const RDORTPIntDefVal *dv) const
{
	if(exist && dv->exist)
		if((minVal > dv->GetIntValue()) || (maxVal < dv->GetIntValue()))
			currParser->error(("Integer value " + toString(dv->GetIntValue()) + 
					" out of range[" + toString(minVal) + ", " + toString(maxVal) + "]").c_str());
}

RDORTPRealResParam::RDORTPRealResParam(RDORTPRealDiap *_diap, RDORTPRealDefVal *_dv): 
	diap(_diap), 
	RDORTPResParam(_dv) 
{
	diap->check(_dv);
}

void RDORTPRealDiap::check(const RDORTPRealDefVal *dv) const
{
	if(exist && dv->exist)
		if((minVal > dv->GetRealValue()) || (maxVal < dv->GetRealValue()))
			currParser->error(("Integer value " + toString(dv->GetRealValue()) + 
					" out of range[" + toString(minVal) + ", " + toString(maxVal) + "]").c_str());
}

const RDORTPResParam *RDORTPIntResParam::constructSuchAs(const int defVal) const 
{
	RDORTPIntDefVal *newDV = new RDORTPIntDefVal(defVal);
	RDORTPIntResParam *rp = new RDORTPIntResParam(diap, newDV);
	return rp;
}

const RDORTPResParam *RDORTPRealResParam::constructSuchAs(const double *const defVal) const 
{
	RDORTPRealDefVal *newDV = new RDORTPRealDefVal(*defVal);
	RDORTPRealResParam *rp = new RDORTPRealResParam(diap, newDV);
	return rp;
}

const RDORTPResParam *RDORTPEnumResParam::constructSuchAs(const std::string *const defVal) const 
{
	enu->findValue(defVal);	 // if no value - Syntax exception will be thrown
	RDORTPEnumDefVal *newDV = new RDORTPEnumDefVal(defVal);
	RDORTPEnumResParam *rp = new RDORTPEnumResParam(enu, newDV);
	return rp;
}

int RDORTPDefVal::GetIntValue() const
{
	currParser->error("Invalid default value");
	return 0;	// unreachable code...
}

double RDORTPDefVal::GetRealValue() const
{
	currParser->error("Invalid default value");
	return 0.;	// unreachable code...
}

const std::string *RDORTPDefVal::GetEnumValue() const
{
	currParser->error("Invalid default value");
	return NULL;	// unreachable code...
}

int RDORTPIntResParam::getDiapTableFunc() const 
{
	if(!diap->exist)
		currParser->error("integer table function parameter must have range");

	if(diap->minVal != 1)
		currParser->error("integer table function parameter must have minimum value = 1");

	return diap->maxVal - diap->minVal + 1;
}

int RDORTPRealResParam::getDiapTableFunc() const
{
	currParser->error("unexpected real table function parameter");
	return 0;		// unreachable code...
}

int RDORTPEnumResParam::getDiapTableFunc() const 
{
	return enu->enumVals.size();
}

RDORTPParamDesc::RDORTPParamDesc(const std::string *const _name, const RDORTPResParam *const _parType)
	: name(_name), parType(_parType) 
{
}

}		// namespace rdoParse 
