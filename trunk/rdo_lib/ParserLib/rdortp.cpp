#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdortp.h"
#include "rdoparser.h"
#include "rdoDefines.h"

namespace rdoParse 
{

void RDORTPEnum::add(const string *const next) 
{ 
	if(find_if(enumVals.begin(), enumVals.end(), comparePointers<string>(next)) != enumVals.end())
		currParser->error(("Second appearance of the same value name: " + *next).c_str());

	enumVals.push_back(next); 
}

int RDORTPEnum::findValue(const string *const val) const
{
	vector<const string *>::const_iterator it = 
		find_if(enumVals.begin(), enumVals.end(), comparePointers<string>(val));

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
const RDORTPParamDesc *RDORTPResType::findRTPParam(const string *const param) const
{
	vector<const RDORTPParamDesc *>::const_iterator it = find_if(params.begin(), 
		params.end(), 
		compareName<RDORTPParamDesc>(param));
	if(it != params.end())
		return (*it);

	return NULL;
}

int RDORTPResType::getRTPParamNumber(const string *const param) const
{
	vector<const RDORTPParamDesc *>::const_iterator it = find_if(params.begin(), 
		params.end(), 
		compareName<RDORTPParamDesc>(param));
	if(it != params.end())
		return it - params.begin();

	return -1;
}

const RDORTPResParam *RDORTPResParam::constructSuchAs() const 
{ 
	return this; 
}
const RDORTPResParam *RDORTPResParam::constructSuchAs(const int defVal) const
{
	ostringstream str;
	str << "Invalid default value: " << defVal;
	currParser->error(str.str().c_str());
	return NULL;	// unreachable code...
}
const RDORTPResParam *RDORTPResParam::constructSuchAs(const double *const defVal) const
{
	ostringstream str;
	str << "Invalid default value: " << *defVal;
	currParser->error(str.str().c_str());
	return NULL;	// unreachable code...
}
const RDORTPResParam *RDORTPResParam::constructSuchAs(const string *const defVal) const 
{
	currParser->error(("Invalid default value: " + *defVal).c_str());
	return NULL;	// unreachable code...
}

RDOValue RDORTPResParam::getRSSEnumValue(const string *const val) const 
{
	ostringstream str;
	str << "Unexpected parameter value: " << *val;
	currParser->error(str.str().c_str());
	return NULL;	// unreachable code...
}
RDOValue RDORTPResParam::getRSSIntValue(const int val) const 
{
	ostringstream str;
	str << "Unexpected parameter value: " << val;
	currParser->error(str.str().c_str());
	return NULL;	// unreachable code...
}
RDOValue RDORTPResParam::getRSSRealValue(const double *const val) const
{
	ostringstream str;
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
RDOValue RDORTPEnumResParam::getRSSEnumValue(const string *const val) const
{
	return enu->findValue(val);
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

const RDORTPResParam *RDORTPEnumResParam::constructSuchAs(const string *const defVal) const 
{
	enu->findValue(defVal);	 // if no value - Syntax exception will be thrown
	RDORTPEnumDefVal *newDV = new RDORTPEnumDefVal(defVal);
	RDORTPEnumResParam *rp = new RDORTPEnumResParam(enu, newDV);
	return rp;
}

int RDORTPDefVal::GetIntValue()
{
	currParser->error("Invalid default value");
	return 0;	// unreachable code...
}

double RDORTPDefVal::GetRealValue()
{
	currParser->error("Invalid default value");
	return 0.;	// unreachable code...
}

const string *RDORTPDefVal::GetEnumValue()
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

RDORTPParamDesc::RDORTPParamDesc(const string *const _name, const RDORTPResParam *const _parType)
	: name(_name), parType(_parType) 
{
}

}		// namespace rdoParse 
