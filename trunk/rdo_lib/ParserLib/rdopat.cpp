#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdopat.h"
#include "rdoparser.h"
#include "rdofun.h"
#include "rdorss.h"
#include "rdortp.h"
#include "rdopatrtime.h"
#include "rdoruntime.h"

namespace rdoParse 
{

RDOPATPattern::RDOPATPattern(const string *const _name, const bool _trace):
	name(_name), trace(_trace), useCommonChoice(false)
{
	if(currParser->findPattern(name))
		currParser->error("Pattern with name: " + *name + " already exist");

	currParser->allPATPatterns.push_back(this);
	currParser->lastPATPattern = this;
}

void RDOPATPattern::testGoodForSearchActivity() const
{
	currParser->error("Only RULEs can be used in search activity");
}

void RDOPATPattern::testGoodForSomeActivity() const
{
	currParser->error("Only RULEs amd OPERATIONs can be used in some activity");
}

void RDOPATPattern::testGoodForFreeActivity() const
{
	currParser->error("Only IEs amd KEYBOARD OPERATIONs can be used in free activity");
}

int RDOPATPattern::writeModelStructure()
{
	return 0;
}

void RDOPATPatternRule::testGoodForSearchActivity() const
{
	for(vector<RDORelevantResource *>::const_iterator i = relRes.begin();
			i != relRes.end(); i++)
	{
		if(((*i)->begin == CS_Create) ||
			((*i)->begin == CS_Erase))
		{
			currParser->error("Rule: " + *getName() + " Cannot be used in search activity because of bad converter status");
		}
	}
}

const RDOFUNFunctionParam *RDOPATPattern::findPATPatternParam(const string *const paramName) const
{
	vector<RDOFUNFunctionParam *>::const_iterator it = 
		find_if(params.begin(), params.end(), compareName<RDOFUNFunctionParam>(paramName));

	if(it == params.end())
		return NULL;

	return (*it);
}

const RDORelevantResource *RDOPATPattern::findRelevantResource(const string *const resName) const
{
	vector<RDORelevantResource *>::const_iterator it = 
		find_if(relRes.begin(), relRes.end(), compareName<RDORelevantResource>(resName));

	if(it == relRes.end())
		return NULL;

	return (*it);
}

int RDOPATPattern::findPATPatternParamNum(const string *const paramName)	const
{
	vector<RDOFUNFunctionParam *>::const_iterator it = 
		find_if(params.begin(), params.end(), compareName<RDOFUNFunctionParam>(paramName));

	if(it == params.end())
		return -1;

	return it - params.begin();
}

int RDOPATPattern::findRelevantResourceNum(const string *const resName) const
{
	vector<RDORelevantResource *>::const_iterator it = 
		find_if(relRes.begin(), relRes.end(), compareName<RDORelevantResource>(resName));

	if(it == relRes.end())
		return -1;

	return it - relRes.begin();
}

void RDOPATPattern::add(RDOFUNFunctionParam *const _param) 
{ 
	if(findPATPatternParam(_param->getName()))
		currParser->error("Second appearance of the same parameter name: " + *(_param->getName()));

	params.push_back(_param); 
}

void RDOPATPattern::addRelRes(string *relName, string *res, ConvertStatus beg, ConvertStatus end)
{
	currParser->error("Needed 1 converter status for this pattern type");
}
void RDOPATPattern::addRelRes(string *relName, string *res, ConvertStatus beg)
{
	currParser->error("Needed 2 converter statuses for this pattern type");
}
void RDOPATPattern::addRelRes(string *relName, string *resName, string *convBeg)
{
	currParser->error("Needed 1 converter status for this pattern type");
}

void RDOPATPatternOperation::addRelRes(string *relName, string *resName, string *convBeg)
{
	ConvertStatus beg;
	if(!convBeg->compare("Keep"))
		beg = CS_Keep;
	else if(!convBeg->compare("Create"))
		beg = CS_Create;
	else if(!convBeg->compare("Erase"))
		beg = CS_Erase;
	else if(!convBeg->compare("NonExist"))
		beg = CS_NonExist;
	else if(!convBeg->compare("NoChange"))
		beg = CS_NoChange;
	else
		currParser->error("Wrong converter status: " + *convBeg);

	addRelRes(relName, resName, beg, CS_NoChange);
}

void RDOPATPatternOperation::addRelRes(string *relName, string *resName, ConvertStatus beg, ConvertStatus end)
{
	switch(beg)
	{
	case CS_Keep:
		if(end != CS_Keep && end != CS_Erase && end != CS_NoChange)
			currParser->error("Wrong second converter status");
		break;
	case CS_Create:
		if(end != CS_Keep && end != CS_Erase && end != CS_NoChange)
			currParser->error("Wrong second converter status");
		break;
	case CS_Erase:
		if(end != CS_NonExist)
			currParser->error("Wrong second converter status");
		break;
	case CS_NonExist:
		if(end != CS_Create)
			currParser->error("Wrong second converter status");
		break;
	case CS_NoChange:
		if(end != CS_Keep && end != CS_Erase && end != CS_NoChange)
			currParser->error("Wrong second converter status");
		break;
	};

	const RDORSSResource *const res = currParser->findRSSResource(resName);
	if(res)
	{
		if(beg == CS_Create)
			currParser->error("Cannot use Create status for resource, only for resource type");
		if(end == CS_Create)
			currParser->error("Cannot use Create status for resource, only for resource type");

		if(res->getType()->isPerm())
		{
			if(beg == CS_Create || beg == CS_Erase || beg == CS_NonExist)
				currParser->error("Wrong first converter status for resource of permanent type");
			if(end == CS_Create || end == CS_Erase || end == CS_NonExist)
				currParser->error("Wrong second converter status for resource of permanent type");
		}

		relRes.push_back(new RDORelevantResourceDirect(relName, relRes.size(), res, beg, end));
		return;
	}

	const RDORTPResType *const type = currParser->findRTPResType(resName);
	if(!type)
		currParser->error("Unknown resource name or type: " + *resName);

	relRes.push_back(new RDORelevantResourceByType(relName, relRes.size(), type, beg, end));
}

void RDOPATPatternRule::addRelRes(string *relName, string *resName, ConvertStatus beg)
{
	if(beg == CS_NonExist)
		currParser->error("Cannot use NonExist status in rule");

	const RDORSSResource *const res = currParser->findRSSResource(resName);
	if(res)
	{
		if(beg == CS_Create)
			currParser->error("Cannot use Create status for resource, only for resource type");

		if(res->getType()->isPerm())
		{
			if(beg == CS_Create || beg == CS_Erase || beg == CS_NonExist)
				currParser->error("Wrong converter status for resource of permanent type");
		}

		relRes.push_back(new RDORelevantResourceDirect(relName, relRes.size(), res, beg));
		return;
	}

	const RDORTPResType *const type = currParser->findRTPResType(resName);
	if(!type)
		currParser->error("Unknown resource name or type: " + *resName);

	relRes.push_back(new RDORelevantResourceByType(relName, relRes.size(), type, beg));
}

void RDOPATPatternEvent::addRelRes(string *relName, string *resName, ConvertStatus beg)
{
	const RDORSSResource *const res = currParser->findRSSResource(resName);
	if(res)
	{
		if(beg == CS_Create)
			currParser->error("Cannot use Create status for resource, only for resource type");

		if(res->getType()->isPerm())
		{
			if(beg == CS_Create || beg == CS_Erase || beg == CS_NonExist)
				currParser->error("Wrong converter status for resource of permanent type");
		}

		RDORelevantResourceDirect *rd = new RDORelevantResourceDirect(relName, relRes.size(), res, beg);
		relRes.push_back(rd);
		patRuntime->addChoiceFromCalc(rd->createSelectFirstResourceCalc());

		return;
	}

	const RDORTPResType *const type = currParser->findRTPResType(resName);
	if(!type)
		currParser->error("Unknown resource name or type: " + *resName);

	if(beg == CS_Keep || beg == CS_Erase)
		currParser->error("Can use Keep and Erase status with resource name only (not with resource type) in irregular event");

	relRes.push_back(new RDORelevantResourceByType(relName, relRes.size(), type, beg));
}

void RDOPATPattern::setCommonChoiceFirst() {	useCommonChoice = true; commonChoice = NULL; }
void RDOPATPattern::setCommonChoiceWithMin(RDOFUNArithm *arithm) { useCommonChoice = true; useCommonWithMax = false; commonChoice = arithm; }
void RDOPATPattern::setCommonChoiceWithMax(RDOFUNArithm *arithm) { useCommonChoice = true; useCommonWithMax = false; commonChoice = arithm; }
void RDOPATPatternRule::setTime(RDOFUNArithm *arithm) 
{ 
	currParser->error("Rule must have no $Time field"); 
}

void RDOPATPattern::setTime(RDOFUNArithm *arithm) 
{ 
	patRuntime->setTime(arithm->createCalc(NULL));
//	time = arithm; 
}

void RDOPATPattern::addRelResBody(string *resName) 
{ 
	vector<RDORelevantResource *>::const_iterator it = 
		find_if(relRes.begin(), relRes.end(), compareName<RDORelevantResource>(resName));

	if(it == relRes.end())
		currParser->error("Name of relevant resource expected instead of: " + *resName);

	currRelRes = (*it);
	if(currRelRes->alreadyHaveConverter)
		currParser->error("\"" + *resName + "\" relevant resource has converter block already");

	currRelRes->alreadyHaveConverter = true;
}

void RDOPATPattern::addRelResUsage(RDOPATChoice *choice, RDOPATFirst *first)
{
	if(useCommonChoice && !first->isEmpty)
		currParser->error("Cannot use both common choice and choice for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(!useCommonChoice && first->isEmpty)
	{
		if((currRelRes->begin != CS_Create) && (currRelRes->end != CS_Create))
			currParser->error("Must use either common choice either choice for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");
	}

	if(!first->isEmpty || !choice->isEmpty)
	{
		if((currRelRes->begin == CS_Create) || (currRelRes->end == CS_Create))
			currParser->error("Cannot use choice when create \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");
	}

	currRelRes->choice = choice;
	currRelRes->first = first;
}

void RDOPATPatternEvent::addRelResUsage(RDOPATChoice *choice, RDOPATFirst *first)
{
	if(!first->isEmpty || !choice->isEmpty)
		currParser->error("Unexpected choice for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	currRelRes->choice = choice;
	currRelRes->first = first;
}

void RDOPATPattern::end()
{
	int size = relRes.size();
	for(int i = 0; i < size; i++)
	{
		RDORelevantResource *currRelRes = relRes.at(i);
		RDOCalc *calc = currRelRes->createSelectFirstResourceCalc();
		patRuntime->addChoiceFromCalc(calc);
	}

	if(useCommonChoice)
	{
		if(commonChoice == NULL)	// first
		{
			int size = relRes.size();
			for(int i = 0; i < size; i++)
			{
				RDOCalc *calc = relRes.at(i)->createSelectFirstResourceChoiceCalc();
				patRuntime->addChoiceFromCalc(calc);
			}
			return;
		}

//		RDOCalc *calc = new RDOSelectResourceCommon(relRes, useCommonWithMax, commonChoice);
		currParser->error("RDOPATPattern::end not implemented yet for not \"first\" conditions in common choice");
	}
	else
	{
		int size = relRes.size();
		for(int i = 0; i < size; i++)
		{
			RDOCalc *calc = relRes.at(i)->createSelectResourceChoiceCalc();
			patRuntime->addChoiceFromCalc(calc);
		}
		return;
	}
}

void RDOPATPattern::addRelResConvert()
{
	if(currRelRes->begin != CS_NoChange && currRelRes->begin != CS_Erase && currRelRes->begin != CS_NonExist)
		currParser->error("Converter needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->end != CS_NoChange && currRelRes->end != CS_Erase && currRelRes->end != CS_NonExist)
		currParser->error("Converter end needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->begin == CS_Erase)
		patRuntime->addBeginCalc(new RDOCalcEraseRes(currRelRes->numberOfResource));

	if(currRelRes->end == CS_Erase)
		patRuntime->addEndCalc(new RDOCalcEraseRes(currRelRes->numberOfResource));
}

RDOPATPatternOperation::RDOPATPatternOperation(string *_name, bool _trace, int patternCounter):
	RDOPATPattern(_name, _trace) 
{ 
//	currParser->runTime->addRuntimeOperation((RDOOperationRuntime *)(patRuntime = new RDOOperationRuntime(currParser->runTime, _trace))); 
	patRuntime = new RDOOperationRuntime(currParser->runTime, _trace); 
	patRuntime->setPatternId(patternCounter);
}

RDOPATPatternOperation::RDOPATPatternOperation(string *_name, bool _trace):
	RDOPATPattern(_name, _trace) 
{}

RDOPATPatternKeyboard::RDOPATPatternKeyboard(string *_name, bool _trace, int patternCounter)
	: RDOPATPatternOperation(_name, _trace)
{
	patRuntime = new RDOKeyboardRuntime(currParser->runTime, _trace); 
	patRuntime->setPatternId(patternCounter);
}
 
RDOPATPatternEvent::RDOPATPatternEvent(string *_name, bool _trace, int patternCounter):
	RDOPATPattern(_name, _trace) 
{ 
//	currParser->runTime->addRuntimeIE((RDOIERuntime *)(patRuntime = new RDOIERuntime(currParser->runTime, _trace))); 
	patRuntime = new RDOIERuntime(currParser->runTime, _trace); 
	patRuntime->setPatternId(patternCounter);
}

RDOPATPatternRule::RDOPATPatternRule(string *_name, bool _trace, int patternCounter):
	RDOPATPattern(_name, _trace) 
{ 
//	currParser->runTime->addRuntimeRule((RDORuleRuntime *)(patRuntime = new RDORuleRuntime(currParser->runTime, _trace))); 
	patRuntime = new RDORuleRuntime(currParser->runTime, _trace); 
	patRuntime->setPatternId(patternCounter);
}

RDOCalc *RDORelevantResourceDirect::createSelectFirstResourceCalc()
{
	return new RDOSelectResourceDirectCalc(numberOfResource, res->getNumber(), NULL, NULL);
}

RDOCalc *RDORelevantResourceDirect::createSelectFirstResourceChoiceCalc()
{
	return new RDOSelectResourceDirectCalc(numberOfResource, res->getNumber(), NULL, choice);
}

RDOCalc *RDORelevantResourceDirect::createSelectResourceChoiceCalc()
{
	return new RDOSelectResourceDirectCalc(numberOfResource, res->getNumber(), first, choice);
}

RDOCalc *RDORelevantResourceByType::createSelectFirstResourceCalc()
{
	if((begin != CS_Create) && (end != CS_Create))
		return new RDOSelectResourceByTypeCalc(numberOfResource, type->getType(), NULL, NULL);
	else
		return new RDOCalcConst(1);
}

RDOCalc *RDORelevantResourceByType::createSelectFirstResourceChoiceCalc()
{
	if((begin != CS_Create) && (end != CS_Create))
		return new RDOSelectResourceByTypeCalc(numberOfResource, type->getType(), NULL, choice);
	else
		return new RDOCalcConst(1);
}

RDOCalc *RDORelevantResourceByType::createSelectResourceChoiceCalc()
{
	if((begin != CS_Create) && (end != CS_Create))
		return new RDOSelectResourceByTypeCalc(numberOfResource, type->getType(), first, choice);
	else
		return new RDOCalcConst(1);
}

void RDOPATParamsSet::checkParamsNumbers(RDORelevantResource *currRelRes)
{
	int size = paramArithms.size();
	for(int i = 0; i < size; i++)
	{
		int parNumb = currRelRes->getType()->getRTPParamNumber(paramNames.at(i));
		if(parNumb == -1)
			currParser->error("Wrong resource parameter name: " + *paramNames.at(i));

		if(find(paramNumbs.begin(), paramNumbs.end(), parNumb) != paramNumbs.end())
			currParser->error("Second appearence of the same resource parameter name: " + *paramNames.at(i));

		paramNumbs.push_back(parNumb);
	}
}

void RDOPATParamsSet::addIdentif(string *paramName, RDOFUNArithm *paramArithm)
{
	paramNames.push_back(paramName);
	paramArithms.push_back(paramArithm);
}

void RDOPATParamsSet::addIdentif(string *paramName)
{
	paramNames.push_back(paramName);
	paramArithms.push_back(NULL);
}

void RDOPATPatternOperation::addRelResConvertBegin(bool trace, RDOPATParamsSet *parSet)
{
	parSet->checkParamsNumbers(currRelRes);
	if(currRelRes->begin == CS_NoChange || currRelRes->begin == CS_Erase || currRelRes->begin == CS_NonExist)
		currParser->error("Converter Begin not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->end != CS_NoChange && currRelRes->end != CS_Erase && currRelRes->end != CS_NonExist)
		currParser->error("Converter end needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->end == CS_Erase)
		patRuntime->addEndCalc(new RDOCalcEraseRes(currRelRes->numberOfResource));

	if(currRelRes->begin == CS_Create)
	{
		if(currRelRes->getType()->getParams().size() != parSet->paramNumbs.size())
			currParser->error("Must define all parameters when create new resource: \"" + *currRelRes->getName() + "\" in pattern \"" + *getName() + "\" in convert begin");

		RDOCalc *calc = new RDOCalcCreateEmptyResource(currRelRes->getType()->getType(), trace, currRelRes->numberOfResource);
		patRuntime->addBeginCalc(calc);
	}

	int size = parSet->paramArithms.size();
	for(int i = 0; i < size; i++)
	{
		int parNumb = parSet->paramNumbs.at(i);
		RDOCalc *rightValue = parSet->paramArithms.at(i)->createCalc(currRelRes->getType()->getParams().at(parNumb)->getType());
		RDOCalc *calc = new RDOSetRelParamCalc(currRelRes->numberOfResource, parNumb, rightValue);
		patRuntime->addBeginCalc(calc);
	}
}

void RDOPATPatternOperation::addRelResConvertEnd(bool trace, RDOPATParamsSet *parSet)
{
	parSet->checkParamsNumbers(currRelRes);
	if(currRelRes->end == CS_NoChange || currRelRes->end == CS_Erase || currRelRes->end == CS_NonExist || currRelRes->begin == CS_Erase)
		currParser->error("Converter end not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->begin != CS_NoChange && currRelRes->begin != CS_Erase && currRelRes->begin != CS_NonExist)
		currParser->error("Converter begin needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->end == CS_Create)
	{
		if(currRelRes->getType()->getParams().size() != parSet->paramNumbs.size())
			currParser->error("Must define all parameters when create new resource: \"" + *currRelRes->getName() + "\" in pattern \"" + *getName() + "\" in convert end");

		RDOCalc *calc = new RDOCalcCreateEmptyResource(currRelRes->getType()->getType(), trace, currRelRes->numberOfResource);
		patRuntime->addEndCalc(calc);
	}

	int size = parSet->paramArithms.size();
	for(int i = 0; i < size; i++)
	{
		int parNumb = parSet->paramNumbs.at(i);
		RDOCalc *rightValue = parSet->paramArithms.at(i)->createCalc(currRelRes->getType()->getParams().at(parNumb)->getType());
		RDOCalc *calc = new RDOSetRelParamCalc(currRelRes->numberOfResource, parNumb, rightValue);
		patRuntime->addEndCalc(calc);
	}
}

void RDOPATPatternOperation::addRelResConvertBeginEnd(bool trace, RDOPATParamsSet *parSet, bool trace2, RDOPATParamsSet *parSet2)
{
	parSet->checkParamsNumbers(currRelRes);
	parSet2->checkParamsNumbers(currRelRes);
	if(currRelRes->begin == CS_NoChange || currRelRes->begin == CS_Erase || currRelRes->begin == CS_NonExist)
		currParser->error("Converter begin not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->end == CS_NoChange || currRelRes->end == CS_Erase || currRelRes->end == CS_NonExist)
		currParser->error("Converter end not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->begin == CS_Create)
	{
		if(currRelRes->getType()->getParams().size() != parSet->paramNumbs.size())
			currParser->error("Must define all parameters when create new resource: \"" + *currRelRes->getName() + "\" in pattern \"" + *getName() + "\" in convert begin");

		RDOCalc *calc = new RDOCalcCreateEmptyResource(currRelRes->getType()->getType(), trace, currRelRes->numberOfResource);
		patRuntime->addBeginCalc(calc);
	}

	int size = parSet->paramArithms.size();
	for(int i = 0; i < size; i++)
	{
		int parNumb = parSet->paramNumbs.at(i);
		RDOCalc *rightValue = parSet->paramArithms.at(i)->createCalc(currRelRes->getType()->getParams().at(parNumb)->getType());
		RDOCalc *calc = new RDOSetRelParamCalc(currRelRes->numberOfResource, parNumb, rightValue);
		patRuntime->addBeginCalc(calc);
	}

	if(currRelRes->end == CS_Create)
	{
		if(currRelRes->getType()->getParams().size() != parSet2->paramNumbs.size())
			currParser->error("Must define all parameters when create new resource: \"" + *currRelRes->getName() + "\" in pattern \"" + *getName() + "\" in convert end");

		RDOCalc *calc = new RDOCalcCreateEmptyResource(currRelRes->getType()->getType(), trace2, currRelRes->numberOfResource);
		patRuntime->addEndCalc(calc);
	}

	size = parSet2->paramArithms.size();
	for(i = 0; i < size; i++)
	{
		int parNumb = parSet2->paramNumbs.at(i);
		RDOCalc *rightValue = parSet2->paramArithms.at(i)->createCalc(currRelRes->getType()->getParams().at(parNumb)->getType());
		RDOCalc *calc = new RDOSetRelParamCalc(currRelRes->numberOfResource, parNumb, rightValue);
		patRuntime->addEndCalc(calc);
	}
}

void RDOPATPatternRule::addRelResConvertRule(bool trace, RDOPATParamsSet *parSet)
{
	parSet->checkParamsNumbers(currRelRes);
	if(currRelRes->begin == CS_NoChange || currRelRes->begin == CS_Erase || currRelRes->begin == CS_NonExist)
		currParser->error("Converter not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->begin == CS_Create)
	{
		if(currRelRes->getType()->getParams().size() != parSet->paramNumbs.size())
			currParser->error("Must define all parameters when create new resource: \"" + *currRelRes->getName() + "\" in pattern \"" + *getName() + "\" in converter");

		RDOCalc *calc = new RDOCalcCreateEmptyResource(currRelRes->getType()->getType(), trace, currRelRes->numberOfResource);
		patRuntime->addBeginCalc(calc);
	}

	int size = parSet->paramArithms.size();
	for(int i = 0; i < size; i++)
	{
		int parNumb = parSet->paramNumbs.at(i);
		RDOCalc *rightValue = parSet->paramArithms.at(i)->createCalc(currRelRes->getType()->getParams().at(parNumb)->getType());
		RDOCalc *calc = new RDOSetRelParamCalc(currRelRes->numberOfResource, parNumb, rightValue);
		patRuntime->addBeginCalc(calc);
	}
}

void RDOPATPatternEvent::addRelResConvertEvent(bool trace, RDOPATParamsSet *parSet)
{
	parSet->checkParamsNumbers(currRelRes);
	if(currRelRes->begin == CS_NoChange || currRelRes->begin == CS_Erase || currRelRes->begin == CS_NonExist)
		currParser->error("Converter not needed for \"" + *currRelRes->getName() + "\" relevant resource in pattern \"" + *getName() + "\"");

	if(currRelRes->begin == CS_Create)
	{
		if(currRelRes->getType()->getParams().size() != parSet->paramNumbs.size())
			currParser->error("Must define all parameters when create new resource: \"" + *currRelRes->getName() + "\" in pattern \"" + *getName() + "\" in converter");

		RDOCalc *calc = new RDOCalcCreateEmptyResource(currRelRes->getType()->getType(), trace, currRelRes->numberOfResource);
		patRuntime->addBeginCalc(calc);
	}

	int size = parSet->paramArithms.size();
	for(int i = 0; i < size; i++)
	{
		int parNumb = parSet->paramNumbs.at(i);
		RDOCalc *rightValue = parSet->paramArithms.at(i)->createCalc(currRelRes->getType()->getParams().at(parNumb)->getType());
		RDOCalc *calc = new RDOSetRelParamCalc(currRelRes->numberOfResource, parNumb, rightValue);
		patRuntime->addBeginCalc(calc);
	}
}

void RDOPATPattern::addRelResConvertBegin(bool trace, RDOPATParamsSet *parSet)
{
	currParser->error("Unexoected lexem ConvertBegin in pattern \"" + *getName() + "\"");
}

void RDOPATPattern::addRelResConvertEnd(bool trace, RDOPATParamsSet *parSet)
{
	currParser->error("Unexoected lexem ConvertEnd in pattern \"" + *getName() + "\"");
}

void RDOPATPattern::addRelResConvertBeginEnd(bool trace, RDOPATParamsSet *parSet,bool trace2, RDOPATParamsSet *parSet2)
{
	currParser->error("Unexoected lexem ConvertBegin in pattern \"" + *getName() + "\"");
}

void RDOPATPattern::addRelResConvertRule(bool trace, RDOPATParamsSet *parSet)
{
	currParser->error("Unexoected lexem ConvertRule in pattern \"" + *getName() + "\"");
}

void RDOPATPattern::addRelResConvertEvent(bool trace, RDOPATParamsSet *parSet)
{
	currParser->error("Unexoected lexem ConvertEvent in pattern \"" + *getName() + "\"");
}

const RDORTPResType *const RDORelevantResourceDirect::getType() const 
{ 
	return res->getType(); 
}

}		// namespace rdoParse 
