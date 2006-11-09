#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdofun.h"
#include "rdoparser.h"
#include "rdortp.h"
#include "rdorss.h"
#include "rdopat.h"
#include "rdoruntime.h"
#include "rdofunc.h"
#include "rdodpt.h"
#include "rdoparser_lexer.h"

namespace rdoParse 
{

int funlex( YYSTYPE* lpval, YYLTYPE* llocp, void* lexer )
{
	reinterpret_cast<RDOLexer*>(lexer)->m_lpval = lpval;
	reinterpret_cast<RDOLexer*>(lexer)->m_lploc = llocp;
	return reinterpret_cast<RDOLexer*>(lexer)->yylex();
}
void funerror( char* mes )
{
}

void RDOParser::addConstant(RDORTPParamDesc *const _cons)
{
	if(findFUNConst(_cons->getName()))
		parser->error("Second appearance of the same constant name: " + *(_cons->getName()));

	RDOFUNConstant *newConst = new RDOFUNConstant( _cons );
	runTime->setConstValue(newConst->number, newConst->descr->getType()->getRSSDefaultValue());
}

RDOFUNConstant::RDOFUNConstant( RDORTPParamDesc* _descr ):
	descr( _descr ),
	number( parser->getFUNCONST_id() )
{
	parser->insertFUNConstant( this );
}

const RDOFUNConstant *RDOParser::findFUNConst(const std::string *const _cons) const
{
	std::vector<RDOFUNConstant *>::const_iterator it = 
		std::find_if(allFUNConstant.begin(), allFUNConstant.end(), compareName<RDOFUNConstant>(_cons));

	if(it == allFUNConstant.end())
		return NULL;

	return (*it);
}

const RDOFUNFunctionParam *const RDOFUNFunction::findFUNFunctionParam(const std::string *const paramName) const 
{
	std::vector<const RDOFUNFunctionParam *>::const_iterator it = 
		std::find_if(params.begin(), params.end(), compareName<RDOFUNFunctionParam>(paramName));

	if(it == params.end())
		return NULL;

	return (*it);
}

RDOFUNFunction::RDOFUNFunction( const std::string* const _name, const RDORTPResParam* const _retType):
	name( _name ),
	retType( _retType )
{
	parser->insertFUNFunction( this );
}

int RDOFUNFunction::findFUNFunctionParamNum(const std::string *const paramName) const
{
	std::vector<const RDOFUNFunctionParam *>::const_iterator it = 
		std::find_if(params.begin(), params.end(), compareName<RDOFUNFunctionParam>(paramName));

	if(it == params.end())
		return -1;

	return it - params.begin();
}

void RDOFUNFunction::add(const RDOFUNFunctionParam *const _param) 
{ 
	if ( findFUNFunctionParam(_param->getName()) ) {
		parser->error( rdo::format("Параметр уже существует: %s", _param->getName()->c_str()) );
//		parser->error("Second appearance of the same parameter name: " + *(_param->getName()));
	}
	params.push_back( _param ); 
}

void RDOFUNFunction::add(const RDOFUNFunctionListElement *const _param)
{
	listElems.push_back(_param); 
}

void RDOFUNFunction::add(const RDOFUNCalculateIf *const _calculateIf)
{
	calculateIf.push_back(_calculateIf); 
}
 
void RDOFUNFunction::createListCalc()
{
	int numParams = params.size();
	int elements = listElems.size();
	int currElement = 0;

	if ( !retType->dv->exist ) {
		parser->error( rdo::format("Функция '%s' должна иметь значение по-умолчанию", name->c_str()) );
//		parser->error(("list function " + *name + " must have default result value").c_str());
	}

	rdoRuntime::RDOCalcConst *defaultValue = new rdoRuntime::RDOCalcConst(retType->getRSSDefaultValue()); 
	rdoRuntime::RDOFunListCalc *funCalc = new rdoRuntime::RDOFunListCalc(defaultValue);
	try
	{
		for(;;)	// for all cases in list
		{
			if(currElement >= elements)
				break;

			rdoRuntime::RDOCalc *caseCalc = new rdoRuntime::RDOCalcConst(true);
			for(int currParam = 0; currParam < numParams; currParam++)
			{
				const RDOFUNFunctionListElement *const arg = listElems.at(currElement++);
				const RDOFUNFunctionParam *const param = params.at(currParam);
				rdoRuntime::RDOCalcFuncParam *funcParam = new rdoRuntime::RDOCalcFuncParam(currParam);
				rdoRuntime::RDOCalcIsEqual *compareCalc = arg->createIsEqualCalc(param, funcParam);
				rdoRuntime::RDOCalc *andCalc = new rdoRuntime::RDOCalcAnd(caseCalc, compareCalc);
				caseCalc = andCalc;
			}

			const RDOFUNFunctionListElement *const eq = listElems.at(currElement++);
			if(!eq->isEquivalence())
				parser->error("\"=\" expected");

			const RDOFUNFunctionListElement *const res = listElems.at(currElement++);
			rdoRuntime::RDOCalcConst *resultCalc = res->createResultCalc(retType);

			funCalc->addCase(caseCalc, resultCalc);
		}
		functionCalc = funCalc;
	}
	catch(std::out_of_range ex)
	{
		parser->error(("Wrong element number in list function " + *name).c_str());
	}
}

void RDOFUNFunction::createTableCalc()
{
	int numParams = params.size();
	try
	{
		rdoRuntime::RDOCalc *calc = new rdoRuntime::RDOCalcConst(0);
		int d = 1;
		for(int currParam = 0; currParam < numParams; currParam++)
		{
			const RDOFUNFunctionParam *const param = params.at(currParam);
			rdoRuntime::RDOCalcFuncParam *funcParam = new rdoRuntime::RDOCalcFuncParam(currParam);
			rdoRuntime::RDOCalc *val2 = funcParam;
			if(param->getType()->getType() != 2)
			{
				rdoRuntime::RDOCalcConst *const1 = new rdoRuntime::RDOCalcConst(1);
				val2 = new rdoRuntime::RDOCalcMinus(val2, const1);
			}
			rdoRuntime::RDOCalcConst *const2 = new rdoRuntime::RDOCalcConst(d);
			rdoRuntime::RDOCalcMult *mult = new rdoRuntime::RDOCalcMult(const2, val2);
			rdoRuntime::RDOCalcPlus *add = new rdoRuntime::RDOCalcPlus(mult, calc);
			d *= param->getType()->getDiapTableFunc();
			calc = add;
		}

		if(listElems.size() != d)
			parser->error(("wrong number of value in table function " + *name).c_str());

		rdoRuntime::RDOFuncTableCalc *funCalc = new rdoRuntime::RDOFuncTableCalc(calc);
		for(int currElem = 0; currElem < d; currElem++)
		{
			const RDOFUNFunctionListElement *const el = listElems.at(currElem);
			if(el->isEquivalence())
				parser->error("\"=\" unexpected in table function");

			rdoRuntime::RDOCalcConst *resultCalc = el->createResultCalc(retType);
			funCalc->addResultCalc(resultCalc);
		}
		functionCalc = funCalc;
	}
	catch(std::out_of_range ex)
	{
		parser->error(("Wrong element number in list function " + *name).c_str());
	}
}

rdoRuntime::RDOCalcIsEqual *RDOFUNFunctionListElement::createIsEqualCalc(const RDOFUNFunctionParam *const param, const rdoRuntime::RDOCalcFuncParam *const funcParam) const
{
	rdoRuntime::RDOCalcConst *constCalc = createResultCalc(param->getType());
	rdoRuntime::RDOCalcIsEqual *res = new rdoRuntime::RDOCalcIsEqual(constCalc, funcParam);
	return res;
}

rdoRuntime::RDOCalcConst *RDOFUNFunctionListElementItentif::createResultCalc(const RDORTPResParam *const retType) const
{
	return new rdoRuntime::RDOCalcConst(retType->getRSSEnumValue(value));
}

rdoRuntime::RDOCalcConst *RDOFUNFunctionListElementReal::createResultCalc(const RDORTPResParam *const retType) const 
{
	return new rdoRuntime::RDOCalcConst(retType->getRSSRealValue(value));
}

rdoRuntime::RDOCalcConst *RDOFUNFunctionListElementInt::createResultCalc(const RDORTPResParam *const retType) const 
{
	return new rdoRuntime::RDOCalcConst(retType->getRSSIntValue(value));
}

rdoRuntime::RDOCalcConst *RDOFUNFunctionListElementEq::createResultCalc(const RDORTPResParam *const retType) const
{
	parser->error("Internal parser error, incorrect state");
	return NULL;	// unreachable code
}

RDOFUNArithm::RDOFUNArithm( std::string* resName, std::string* parName )
{
	const RDORSSResource* const res = parser->findRSSResource( resName ); 
	if ( !res ) {
		if(parser->getFUNGroupStack().empty() || 
			*parser->getFUNGroupStack().back()->resType->getName() != *resName)
		{
			if((parser->getFileToParse() != rdoModelObjects::PAT) ||
				!parser->getLastPATPattern()->findRelevantResource(resName))
			{
				if((parser->getFileToParse() == rdoModelObjects::DPT) && 
					(parser->getLastDPTSearch() != NULL) && 
					(parser->getLastDPTSearch()->lastActivity->getRule()->findRelevantResource(resName) != NULL))
				{
					const RDORelevantResource *const rel = parser->getLastDPTSearch()->lastActivity->getRule()->findRelevantResource(resName);
					int relResNumb = parser->getLastDPTSearch()->lastActivity->getRule()->findRelevantResourceNum(resName);
					int parNumb = rel->getType()->getRTPParamNumber(parName);
					if ( parNumb == -1 ) {
						parser->error( rdo::format("Неизвестный параметр ресурса: %s", parName->c_str()) );
//						parser->error( "Unknown resource parameter: " + *parName );
					}

					calc = new rdoRuntime::RDOCalcGetRelevantResParam(relResNumb, parNumb);
					type = rel->getType()->findRTPParam(parName)->getType()->getType();
					if(type == 2)
						enu = ((RDORTPEnumResParam *)rel->getType()->findRTPParam(parName)->getType())->enu;

					return;
				} else {
					parser->error( rdo::format("Неизвестный параметр ресурса: %s", parName->c_str()) );
//					parser->error(("Unknown resource name: " + *resName).c_str());
				}
			} else {
				// Релевантные ресурсы в паттерне (with_min-common-choice, $Time, $Body)
				RDOPATPattern* pat = parser->getLastPATPattern();
				const RDORelevantResource* const rel = pat->findRelevantResource( resName );
				if ( !pat->currRelRes ) {
					// Внутри with_min-common-choice или $Time
					if ( rel->begin == RDOPATPattern::CS_NonExist ) {
						parser->error( rdo::format("Релевантный ресурс не может быть использован, т.к. его статус NonExist: %s", rel->getName()->c_str()) );
					}
				} else {
					// Внутри $Body
					// Проверяем использование неинициализированного рел.ресурса в Choice from другом рел.ресурсе
					if ( pat->currRelRes->isChoiceFromState() ) {
						if ( !rel->alreadyHaveConverter && !rel->isDirect() ) {
							parser->error( rdo::format("Релевантный ресурс неопределен: %s. Его нельзя использовать в условиях выбора других ресурсов до его собственного Choice from", rel->getName()->c_str()) );
						}
						if ( rel->begin == RDOPATPattern::CS_NonExist ) {
							parser->error( rdo::format("Релевантный ресурс не существует в начале операции (NonExist): %s", rel->getName()->c_str()) );
						}
						if ( rel->begin == RDOPATPattern::CS_Create ) {
							parser->error( rdo::format("Сразу после создания (Create) релевантный ресурс можно использовать только в конверторах: %s", rel->getName()->c_str()) );
						}
					}
					// Проверяем использование временного рел.ресурса внутри конвертора другого рел.ресурса
					if ( rel->getType()->isTemporary() ) {
						// В конверторе начала
						if ( pat->currRelRes->currentState == RDORelevantResource::convertBegin ) {
							if ( rel->begin == RDOPATPattern::CS_Create && !rel->alreadyHaveConverter ) {
								parser->error( rdo::format("Релевантный ресурс нельзя использовать до его инициализации (Create): %s", rel->getName()->c_str()) );
							}
							if ( rel->begin == RDOPATPattern::CS_Erase && rel->alreadyHaveConverter ) {
								parser->error( rdo::format("Релевантный ресурс нельзя использовать полсле удаления (Erase): %s", rel->getName()->c_str()) );
							}
							if ( rel->begin == RDOPATPattern::CS_NonExist ) {
								parser->error( rdo::format("Релевантный ресурс не существует в этом конверторе (NonExist): %s", rel->getName()->c_str()) );
							}
						}
						// В конверторе конца
						if ( pat->currRelRes->currentState == RDORelevantResource::convertEnd ) {
							if ( rel->end == RDOPATPattern::CS_Create && !rel->alreadyHaveConverter ) {
								parser->error( rdo::format("Релевантный ресурс нельзя использовать до его инициализации (Create): %s", rel->getName()->c_str()) );
							}
							if ( rel->end == RDOPATPattern::CS_Erase && rel->alreadyHaveConverter ) {
								parser->error( rdo::format("Релевантный ресурс нельзя использовать полсле удаления (Erase): %s", rel->getName()->c_str()) );
							}
							if ( rel->end == RDOPATPattern::CS_NonExist ) {
								parser->error( rdo::format("Релевантный ресурс не существует в этом конверторе (NonExist): %s", rel->getName()->c_str()) );
							}
						}
					}
				}
				int relResNumb = pat->findRelevantResourceNum( resName );
				int parNumb = rel->getType()->getRTPParamNumber( parName );
				if ( parNumb == -1 ) {
					parser->error( rdo::format("Неизвестный параметр ресурса: %s", parName->c_str()) );
//					parser->error( "Unknown resource parameter: " + *parName );
				}
				calc = new rdoRuntime::RDOCalcGetRelevantResParam( relResNumb, parNumb );
				type = rel->getType()->findRTPParam( parName )->getType()->getType();
				if ( type == 2 ) {
					enu = ((RDORTPEnumResParam*)rel->getType()->findRTPParam(parName)->getType())->enu;
				}
				return;
			}
		}

		RDOFUNGroup *currGroup = parser->getFUNGroupStack().back();
//		if(*currGroup->resType->name != *resName)
//			parser->error(("Unknown resource name: " + *resName).c_str());

		int parNumb = currGroup->resType->getRTPParamNumber(parName);
		if ( parNumb == -1 ) {
			parser->error( rdo::format("Неизвестный параметр ресурса: %s", parName->c_str()) );
//			parser->error(("Unknown resource parameter: " + *parName).c_str());
		}

		calc = new rdoRuntime::RDOCalcGetGroupResParam(parNumb);
		type = currGroup->resType->findRTPParam(parName)->getType()->getType();
		if(type == 2)
			enu = ((RDORTPEnumResParam *)currGroup->resType->findRTPParam(parName)->getType())->enu;

		return;
	}
	if ( res->getType()->isTemporary() ) {
		parser->error(("Cannot use temporary resource in function: " + *resName).c_str());
	}
	int resNumb = res->getNumber();
	int parNumb = res->getType()->getRTPParamNumber(parName);
	if ( parNumb == -1 ) {
		parser->error( rdo::format("Неизвестный параметр ресурса: %s", parName->c_str()) );
//		parser->error(("Unknown resource parameter: " + *parName).c_str());
	}

	calc = new rdoRuntime::RDOCalcGetResParam(resNumb, parNumb);
	type = res->getType()->findRTPParam(parName)->getType()->getType();
	if(type == 2)
		enu = ((RDORTPEnumResParam *)res->getType()->findRTPParam(parName)->getType())->enu;
}

RDODeletable::RDODeletable() 
{ 
	parser->insertDeletables( this );
}

RDODeletable::~RDODeletable() 
{ 
	parser->removeDeletables( this );
}

RDOFUNArithm *RDOFUNArithm::operator +(RDOFUNArithm &second)
{
	int newType;
	rdoRuntime::RDOCalc *newCalc;

	if((type == 0) && (second.type == 0))
		newType = 0;
	else if((type >= 2) || (second.type >= 2))
		parser->error("cannot add enumerative types");
	else
		newType = 1;

	newCalc = new rdoRuntime::RDOCalcPlus(calc, second.calc);
	return new RDOFUNArithm(newType, newCalc);
}

RDOFUNArithm *RDOFUNArithm::operator -(RDOFUNArithm &second)
{
	int newType;
	rdoRuntime::RDOCalc *newCalc;

	if((type == 0) && (second.type == 0))
		newType = 0;
	else if((type >= 2) || (second.type >= 2))
		parser->error("cannot subtract enumerative types");
	else
		newType = 1;

	newCalc = new rdoRuntime::RDOCalcMinus(calc, second.calc);
	return new RDOFUNArithm(newType, newCalc);
}

RDOFUNArithm *RDOFUNArithm::operator *(RDOFUNArithm &second)
{
	int newType;
	rdoRuntime::RDOCalc *newCalc;

	if((type == 0) && (second.type == 0))
		newType = 0;
	else if((type >= 2) || (second.type >= 2))
		parser->error("cannot multiply enumerative types");
	else
		newType = 1;

	newCalc = new rdoRuntime::RDOCalcMult(calc, second.calc);
	return new RDOFUNArithm(newType, newCalc);
}

RDOFUNArithm *RDOFUNArithm::operator /(RDOFUNArithm &second)
{
	int newType;
	rdoRuntime::RDOCalc *newCalc;

	if((type == 0) && (second.type == 0))
		newType = 0;
	else if((type >= 2) || (second.type >= 2))
		parser->error("cannot divide enumerative types");
	else
		newType = 1;

	newCalc = new rdoRuntime::RDOCalcDiv(calc, second.calc);
	if(newType == 0)
		newCalc = new rdoRuntime::RDOCalcDoubleToInt(newCalc);

	return new RDOFUNArithm(newType, newCalc);
}

RDOFUNLogic *RDOFUNArithm::operator <(RDOFUNArithm &second)
{
	rdoRuntime::RDOCalc *newCalc;
	if((type >= 2) || (second.type >= 2))
		parser->error("cannot compare enumerative types");

	newCalc = new rdoRuntime::RDOCalcIsLess(calc, second.calc);
	return new RDOFUNLogic(newCalc);
}

RDOFUNLogic *RDOFUNArithm::operator >(RDOFUNArithm &second)
{
	rdoRuntime::RDOCalc *newCalc;
	if((type >= 2) || (second.type >= 2))
		parser->error("cannot compare enumerative types");

	newCalc = new rdoRuntime::RDOCalcIsGreater(calc, second.calc);
	return new RDOFUNLogic(newCalc);
}

RDOFUNLogic *RDOFUNArithm::operator <=(RDOFUNArithm &second)
{
	rdoRuntime::RDOCalc *newCalc;
	if((type >= 2) || (second.type >= 2))
		parser->error("cannot compare enumerative types");

	newCalc = new rdoRuntime::RDOCalcIsLEQ(calc, second.calc);
	return new RDOFUNLogic(newCalc);
}

RDOFUNLogic *RDOFUNArithm::operator >=(RDOFUNArithm &second)
{
	rdoRuntime::RDOCalc *newCalc;
	if((type >= 2) || (second.type >= 2))
		parser->error("cannot compare enumerative types");

	newCalc = new rdoRuntime::RDOCalcIsGEQ(calc, second.calc);
	return new RDOFUNLogic(newCalc);
}

RDOFUNLogic *RDOFUNArithm::operator ==(RDOFUNArithm &second)
{
	rdoRuntime::RDOCalc *newCalc;
	if((type == 2) && (second.type == 2))
	{
		if(enu != second.enu)
			parser->error("cannot compare different enumerative types");
	}
	else if((type == 2) && (second.type == 3))
	{
		second.calc = new rdoRuntime::RDOCalcConst(enu->findValue(second.str));
	}
	else if((type >= 2) || (second.type >= 2))
		parser->error("cannot compare enumerative type with nonenumerative type");

	newCalc = new rdoRuntime::RDOCalcIsEqual(calc, second.calc);
	return new RDOFUNLogic(newCalc);
}

RDOFUNLogic *RDOFUNArithm::operator !=(RDOFUNArithm &second)
{
	rdoRuntime::RDOCalc *newCalc;
	if((type == 2) && (second.type == 2))
	{
		if(enu != second.enu)
			parser->error("cannot compare different enumerative types");
	}
	else if((type == 2) && (second.type == 3))
	{
		second.calc = new rdoRuntime::RDOCalcConst(enu->findValue(second.str));
	}
	else if((type >= 2) || (second.type >= 2))
		parser->error("cannot compare enumerative type with nonenumerative type");

	newCalc = new rdoRuntime::RDOCalcIsNotEqual(calc, second.calc);
	return new RDOFUNLogic(newCalc);
}

RDOFUNLogic* RDOFUNLogic::operator &&( const RDOFUNLogic& second )
{
	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcAnd( calc, second.calc );
	return new RDOFUNLogic( newCalc );
}

RDOFUNLogic* RDOFUNLogic::operator ||( const RDOFUNLogic& second )
{
	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcOr( calc, second.calc );
	return new RDOFUNLogic( newCalc );
}

RDOFUNArithm::RDOFUNArithm( int n )
{
	type = 0;
	calc = new rdoRuntime::RDOCalcConst( n );
}

RDOFUNArithm::RDOFUNArithm( double d )
{
	type = 1;
	calc = new rdoRuntime::RDOCalcConst( d );
}

RDOFUNArithm::RDOFUNArithm( double* d )
{
	type = 1;
	calc = new rdoRuntime::RDOCalcConst( *d );
}

RDOFUNArithm::RDOFUNArithm( std::string* s )
{
	if ( (*s == "Time_now") || (*s == "time_now") || (*s == "Системное_время") || (*s == "системное_время") ) {
		type = 1;
		calc = new rdoRuntime::RDOCalcGetTimeNow();
		return;
	}

	if(*s == "Seconds")
	{
		type = 1;
		calc = new rdoRuntime::RDOCalcGetSeconds();
		return;
	}

	const RDOFUNFunctionParam *param = NULL;
	if(parser->getFileToParse() == rdoModelObjects::FUN)
		param = parser->getLastFUNFunction()->findFUNFunctionParam(s);
	else if(parser->getFileToParse() == rdoModelObjects::PAT)
		param = parser->getLastPATPattern()->findPATPatternParam(s);

	const RDOFUNConstant *cons = parser->findFUNConst(s);
	if(cons != NULL && param != NULL)
		parser->error("Ambiguity: constant or parameter usage: " + *s + " ?");

	if(cons != NULL)
	{
		type = cons->descr->getType()->getType();
		if(type == 2)
			enu = ((RDORTPEnumResParam *)cons->descr->getType())->enu;

		calc = new rdoRuntime::RDOCalcGetConst(cons->number);
		return;
	}

	const RDOFUNSequence *seq = parser->findSequence(s);
	if(seq != NULL && param != NULL)
		parser->error("Ambiguity: sequence or parameter usage: " + *s + " ?");

	if(seq != NULL)
	{
		RDOFUNParams tmp;
		const RDOFUNArithm *ar = tmp.createSeqCall(s);
		type = ar->getType();
		if(type == 2)
			enu = ar->enu;

		calc = ar->calc;
		return;
	}

	if(!param)
	{
		type = 3;
		str = s;
		return;
	}

	type = param->getType()->getType();
	if(type == 2)
		enu = ((RDORTPEnumResParam *)param->getType())->enu;

	if(parser->getFileToParse() == rdoModelObjects::FUN)
		calc = new rdoRuntime::RDOCalcFuncParam(parser->getLastFUNFunction()->findFUNFunctionParamNum(s));
	else if(parser->getFileToParse() == rdoModelObjects::PAT)
		calc = new rdoRuntime::RDOCalcPatParam(parser->getLastPATPattern()->findPATPatternParamNum(s));
}

const RDOFUNArithm* RDOFUNParams::createSeqCall( const std::string* const seqName ) const
{
	const RDOFUNSequence* const seq = parser->findSequence( seqName );
//	if ( !seq ) parser->error( "Undefined function or sequence: " + *seqName );
	if ( !seq ) parser->error( rdo::format("Неопределенная функция или последовательность: %s", seqName->c_str()) );
	return seq->createCallCalc( this );
}

const RDOFUNArithm *RDOFUNSequenceUniform::createCallCalc(const RDOFUNParams *const param) const
{
	if(param->params.size() != 2)
		parser->error("Wrong parameters number in uniform sequence call: " + *header->name);

	rdoRuntime::RDOCalcFunctionCall *funcCall = new rdoRuntime::RDOCalcFunctionCall(next);
	RDOFUNArithm *arithm1 = param->params[0];
	rdoRuntime::RDOCalc *arg1 = arithm1->createCalc(NULL);
	RDOFUNArithm *arithm2 = param->params[1];
	rdoRuntime::RDOCalc *arg2 = arithm2->createCalc(NULL);

	funcCall->addParameter(arg1);
	funcCall->addParameter(arg2);

	RDOFUNArithm *res = new RDOFUNArithm(0, funcCall);
	res->type = header->type->getType();
	if(res->type == 2)
		parser->error("Internal parser error");
	return res;
}

const RDOFUNArithm *RDOFUNSequenceExponential::createCallCalc(const RDOFUNParams *const param) const
{
	if(param->params.size() != 1)
		parser->error("Wrong parameters number in exponential sequence call: " + *header->name);

	rdoRuntime::RDOCalcFunctionCall *funcCall = new rdoRuntime::RDOCalcFunctionCall(next);
	RDOFUNArithm *arithm1 = param->params[0];
	rdoRuntime::RDOCalc *arg1 = arithm1->createCalc(NULL);

	funcCall->addParameter(arg1);

	RDOFUNArithm *res = new RDOFUNArithm(0, funcCall);
	res->type = header->type->getType();
	if(res->type == 2)
		parser->error("Internal parser error");
	return res;
}

const RDOFUNArithm *RDOFUNSequenceNormal::createCallCalc(const RDOFUNParams *const param) const
{
	if(param->params.size() != 2)
		parser->error("Wrong parameters number in normal sequence call: " + *header->name);

	rdoRuntime::RDOCalcFunctionCall *funcCall = new rdoRuntime::RDOCalcFunctionCall(next);
	RDOFUNArithm *arithm1 = param->params[0];
	rdoRuntime::RDOCalc *arg1 = arithm1->createCalc(NULL);
	RDOFUNArithm *arithm2 = param->params[1];
	rdoRuntime::RDOCalc *arg2 = arithm2->createCalc(NULL);

	funcCall->addParameter(arg1);
	funcCall->addParameter(arg2);

	RDOFUNArithm *res = new RDOFUNArithm(0, funcCall);
	res->type = header->type->getType();
	if(res->type == 2)
		parser->error("Internal parser error");
	return res;
}

const RDOFUNArithm *RDOFUNSequenceByHist::createCallCalc(const RDOFUNParams *const param) const
{
	if(param->params.size() != 0)
		parser->error("Wrong parameters number in by_hist sequence call: " + *header->name);

	rdoRuntime::RDOCalcFunctionCall *funcCall = new rdoRuntime::RDOCalcFunctionCall(next);

	RDOFUNArithm *res = new RDOFUNArithm(0, funcCall);
	res->type = header->type->getType();
	if(res->type == 2)
		res->enu = ((RDORTPEnumResParam *)header->type)->enu;

	return res;
}

const RDOFUNArithm *RDOFUNSequenceEnumerative::createCallCalc(const RDOFUNParams *const param) const
{
	if(param->params.size() != 0)
		parser->error("Wrong parameters number in enumerative sequence call: " + *header->name);

	rdoRuntime::RDOCalcFunctionCall *funcCall = new rdoRuntime::RDOCalcFunctionCall(next);

	RDOFUNArithm *res = new RDOFUNArithm(0, funcCall);
	res->type = header->type->getType();
	if(res->type == 2)
		res->enu = ((RDORTPEnumResParam *)header->type)->enu;

	return res;
}

const RDOFUNArithm *RDOFUNParams::createCall(const std::string *const funName) const 
{
	const RDOFUNFunction *const func = parser->findFunction(funName);
	if(!func)
		return createSeqCall(funName);

	int nParams = func->getParams().size();
	if(nParams != params.size())
		parser->error(("Wrong parameters number in function call: " + *funName).c_str());

	rdoRuntime::RDOCalcFunctionCall *funcCall = new rdoRuntime::RDOCalcFunctionCall(func->getFunctionCalc());
	for(int i = 0; i < nParams; i++)
	{
		const RDORTPResParam *const funcParam = func->getParams()[i]->getType();
		RDOFUNArithm *arithm = params[i];
		if(funcParam->getType() == 0)
		{
			rdoRuntime::RDOCalc *arg = arithm->createCalc(NULL);
			if(arithm->type == 1)
				arg = new rdoRuntime::RDOCalcDoubleToInt(arg);

			if(((RDORTPIntResParam *)funcParam)->diap->exist)
				arg = new rdoRuntime::RDOCalcCheckDiap(((RDORTPIntResParam *)funcParam)->diap->minVal, 
					((RDORTPIntResParam *)funcParam)->diap->maxVal, arg);

			funcCall->addParameter(arg);
		}
		else if(funcParam->getType() == 1)
		{
			rdoRuntime::RDOCalc *arg = arithm->createCalc(NULL);

			if(((RDORTPRealResParam *)funcParam)->diap->exist)
				arg = new rdoRuntime::RDOCalcCheckDiap(((RDORTPRealResParam *)funcParam)->diap->minVal, 
					((RDORTPRealResParam *)funcParam)->diap->maxVal, arg);

			funcCall->addParameter(arg);
		}
		else
		{
			rdoRuntime::RDOCalc *arg;
			if(arithm->type < 2)
				parser->error("wrong parameter type");

			if(arithm->type == 2)
			{
				if(arithm->enu != ((RDORTPEnumResParam *)funcParam)->enu)
					parser->error("wrong enumerative parameter type");

				arg = arithm->createCalc(NULL);
			}
			else
				arg = new rdoRuntime::RDOCalcConst(((RDORTPEnumResParam *)funcParam)->enu->findValue(arithm->str));	

			funcCall->addParameter(arg);
		}
	}

	RDOFUNArithm *res = new RDOFUNArithm(0, funcCall);
	res->type = func->getType()->getType();
	if(func->getType()->getType() == 2)
		res->enu = ((RDORTPEnumResParam *)func->getType())->enu;

	return res;
}

void RDOFUNFunction::createAlgorithmicCalc()
{
	rdoRuntime::RDOFunAlgorithmicCalc *funcCalc = new rdoRuntime::RDOFunAlgorithmicCalc();
	int size = calculateIf.size();
	if ( !size ) {
		parser->warning( "Отсутствует тело функции" );
	}
	for ( int i = 0; i < size; i++ ) {
		funcCalc->addCalcIf(calculateIf[i]->condition->calc, calculateIf[i]->action->createCalc(getType()));
	}
	functionCalc = funcCalc;
}

RDOFUNCalculateIf::RDOFUNCalculateIf(RDOFUNLogic *_condition, std::string *_funName, RDOFUNArithm *_action):
	condition (_condition),
	funName	 (_funName),
	action	 (_action)
{
	if(*funName != *(parser->getLastFUNFunction()->getName()))
		parser->error("function name expected");
}

RDOFUNGroup::RDOFUNGroup(int _funType, const std::string *const _resType):
	funType(_funType)
{
	resType = parser->findRTPResType(_resType);
	if(!resType)
		parser->error(("Unknown resource type: " + *_resType).c_str());

//	if(resType->isPerm())
//		parser->error(("Temporary resource type expected: " + *_resType).c_str());

	parser->insertFUNGroup( this );
}

RDOFUNLogic *RDOFUNGroup::createFunLogin()
{
	RDOFUNLogic *trueLogic = new RDOFUNLogic(new rdoRuntime::RDOCalcConst(1));
	return createFunLogin(trueLogic);
}

RDOFUNLogic *RDOFUNGroup::createFunLogin(RDOFUNLogic *cond)
{
	rdoRuntime::RDOFunCalcGroup *calc;
	switch(funType)
	{
	case 1:	calc = new rdoRuntime::RDOFunCalcExist(resType->getNumber(), cond->calc);		break;
	case 2:	calc = new rdoRuntime::RDOFunCalcNotExist(resType->getNumber(), cond->calc);	break;
	case 3:	calc = new rdoRuntime::RDOFunCalcForAll(resType->getNumber(), cond->calc);		break;
	case 4:	calc = new rdoRuntime::RDOFunCalcNotForAll(resType->getNumber(), cond->calc);	break;
	default:
		parser->error("Internal compiler error");
	}

	parser->getFUNGroupStack().pop_back();
	return new RDOFUNLogic(calc);
}


//////////////////////////// Sequences	///////////////////////////////

RDOFUNSequenceUniform::RDOFUNSequenceUniform(RDOFUNSequenceHeader *_header, int _base): 
	RDOFUNSequence(_header, _base)
{
	if(header->type->getType() == 2)
		parser->error("Uniform sequence cannot be enumerative type");

	createCalcs();
}

void RDOFUNSequenceUniform::createCalcs()
{
	RandGeneratorUniform *gen = new RandGeneratorUniform();
	initSeq = new rdoRuntime::RDOCalcSeqInit(base, gen);
	parser->runTime->addInitCalc(initSeq);
	next = new rdoRuntime::RDOCalcSeqNextUniform(gen);
}

RDOFUNSequenceExponential::RDOFUNSequenceExponential(RDOFUNSequenceHeader *_header, int _base): 
	RDOFUNSequence(_header, _base)
{
	if(header->type->getType() == 2)
		parser->error("Exponential sequence cannot be enumerative type");

	createCalcs();
}

void RDOFUNSequenceExponential::createCalcs()
{
	RandGeneratorExponential *gen = new RandGeneratorExponential();
	initSeq = new rdoRuntime::RDOCalcSeqInit(base, gen);
	parser->runTime->addInitCalc(initSeq);
	next = new rdoRuntime::RDOCalcSeqNextExponential(gen);
}

RDOFUNSequenceNormal::RDOFUNSequenceNormal(RDOFUNSequenceHeader *_header, int _base): 
	RDOFUNSequence(_header, _base)
{
	if(header->type->getType() == 2)
		parser->error("Normal sequence cannot be enumerative type");

	createCalcs();
}

void RDOFUNSequenceNormal::createCalcs()
{
	RandGeneratorNormal *gen = new RandGeneratorNormal();
	initSeq = new rdoRuntime::RDOCalcSeqInit(base, gen);
	parser->runTime->addInitCalc(initSeq);
	next = new rdoRuntime::RDOCalcSeqNextNormal(gen);
}

void RDOFUNSequenceByHistInt::addInt(int _from, int _to, double *_freq)
{
	from.push_back(_from);
	to.push_back(_to);
	freq.push_back(*_freq);
}

void RDOFUNSequenceByHistInt::createCalcs()
{
	RandGeneratorByHist *gen = new RandGeneratorByHist();
	int size = from.size();
	for(int i = 0; i < size; i++)
		gen->addValues(from[i], to[i], freq[i]);

	initSeq = new rdoRuntime::RDOCalcSeqInit(base, gen);
	parser->runTime->addInitCalc(initSeq);
	next = new rdoRuntime::RDOCalcSeqNextByHist(gen);
}

void RDOFUNSequenceByHistReal::addReal(double * _from, double * _to, double *_freq)
{
	from.push_back(*_from);
	to.push_back(*_to);
	freq.push_back(*_freq);
}

void RDOFUNSequenceByHistReal::createCalcs()
{
	RandGeneratorByHist *gen = new RandGeneratorByHist();
	int size = from.size();
	for(int i = 0; i < size; i++)
		gen->addValues(from[i], to[i], freq[i]);

	initSeq = new rdoRuntime::RDOCalcSeqInit(base, gen);
	parser->runTime->addInitCalc(initSeq);
	next = new rdoRuntime::RDOCalcSeqNextByHist(gen);
}

void RDOFUNSequenceByHistEnum::addEnum(std::string *_val, double *_freq)
{
	val.push_back(header->type->getRSSEnumValue(_val));
	freq.push_back(*_freq);
}

void RDOFUNSequenceByHistEnum::createCalcs()
{
	RandGeneratorByHistEnum *gen = new RandGeneratorByHistEnum();
	int size = val.size();
	for(int i = 0; i < size; i++)
		gen->addValues(val[i], freq[i]);

	initSeq = new rdoRuntime::RDOCalcSeqInit(base, gen);
	parser->runTime->addInitCalc(initSeq);
	next = new rdoRuntime::RDOCalcSeqNextByHist(gen);
}


void RDOFUNSequenceEnumerativeInt::addInt(int _val)
{
	val.push_back(_val);
}

void RDOFUNSequenceEnumerativeInt::createCalcs()
{
	RandGeneratorEnumerative *gen = new RandGeneratorEnumerative();
	int size = val.size();
	for(int i = 0; i < size; i++)
		gen->addValue(val[i]);

	initSeq = new rdoRuntime::RDOCalcSeqInit(base, gen);
	parser->runTime->addInitCalc(initSeq);
	next = new rdoRuntime::RDOCalcSeqNextByHist(gen);
}

void RDOFUNSequenceEnumerativeReal::addReal(double * _val)
{
	val.push_back(*_val);
}

void RDOFUNSequenceEnumerativeReal::createCalcs()
{
	RandGeneratorEnumerative *gen = new RandGeneratorEnumerative();
	int size = val.size();
	for(int i = 0; i < size; i++)
		gen->addValue(val[i]);

	initSeq = new rdoRuntime::RDOCalcSeqInit(base, gen);
	parser->runTime->addInitCalc(initSeq);
	next = new rdoRuntime::RDOCalcSeqNextByHist(gen);
}

void RDOFUNSequenceEnumerativeEnum::addEnum(std::string *_val)
{
	val.push_back(header->type->getRSSEnumValue(_val));
}

void RDOFUNSequenceEnumerativeEnum::createCalcs()
{
	RandGeneratorEnumerative *gen = new RandGeneratorEnumerative();
	int size = val.size();
	for(int i = 0; i < size; i++)
		gen->addValue(val[i]);

	initSeq = new rdoRuntime::RDOCalcSeqInit(base, gen);
	parser->runTime->addInitCalc(initSeq);
	next = new rdoRuntime::RDOCalcSeqNextByHist(gen);
}

RDOFUNSequence::RDOFUNSequence(RDOFUNSequenceHeader *_header, int _base): 
	header(_header), base(_base)
{
	parser->insertFUNSequences( this );
}

rdoRuntime::RDOCalc *RDOFUNArithm::createCalc(const RDORTPResParam *const forType)
{
	if(type != 3)
	{
		if(forType == NULL)
			return calc;
		if(forType->getType() != 0)	// not for integer
			return calc;

		return new rdoRuntime::RDOCalcInt(calc);
	}

	if(forType == NULL)
		parser->error("Wrong parameter type");

	return new rdoRuntime::RDOCalcConst(forType->getRSSEnumValue(str));
}

} // namespace rdoParse
