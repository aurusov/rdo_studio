#include "pch.h"
#include "rdofun.h"
#include "rdoparser.h"
#include "rdortp.h"
#include "rdorss.h"
#include "rdopat.h"
#include "rdofunc.h"
#include "rdodpt.h"
#include "rdoparser_lexer.h"
#include <rdo_runtime.h>
#include <rdocalc.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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

// ----------------------------------------------------------------------------
// ---------- RDOFUNLogic
// ----------------------------------------------------------------------------
RDOFUNLogic::RDOFUNLogic( rdoRuntime::RDOCalc* _calc, bool hide_warning ):
	calc( _calc )
{
	if ( calc ) {
		calc->setSrcFileType( src_filetype() );
	}
	if ( !hide_warning ) {
		const rdoRuntime::RDOCalcConst* calc_const = dynamic_cast<const rdoRuntime::RDOCalcConst*>(calc);
		if ( calc_const ) {
			if ( calc_const->calcValueBase( parser->runtime ) ) {
				parser->warning( calc_const->src_info(), rdo::format("Логическое выражение всегда истинно: %s", calc_const->src_text().c_str()) );
			} else {
				parser->warning( calc_const->src_info(), rdo::format("Логическое выражение всегда ложно: %s", calc_const->src_text().c_str()) );
			}
		}
	}
}

RDOFUNLogic* RDOFUNLogic::operator &&( const RDOFUNLogic& second )
{
	const rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(calc);
	const rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(second.calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser->runtime, calc1->calcValueBase( parser->runtime ) && calc2->calcValueBase( parser->runtime ) );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcAnd::getStaticSrcInfo( calc1, calc2 ) );
	} else {
		newCalc = new rdoRuntime::RDOCalcAnd( parser->runtime, calc, second.calc );
	}
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	return logic;
}

RDOFUNLogic* RDOFUNLogic::operator ||( const RDOFUNLogic& second )
{
	const rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(calc);
	const rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(second.calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser->runtime, calc1->calcValueBase( parser->runtime ) || calc2->calcValueBase( parser->runtime ) );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcOr::getStaticSrcInfo( calc1, calc2 ) );
	} else {
		newCalc = new rdoRuntime::RDOCalcOr( parser->runtime, calc, second.calc );
	}
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	return logic;
}

RDOFUNLogic* RDOFUNLogic::operator_not()
{
	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcNot( parser->runtime, calc );
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	return logic;
}

void RDOFUNLogic::setSrcInfo( const RDOParserSrcInfo& src_info )
{
	RDOParserSrcInfo::setSrcInfo( src_info );
	if ( calc ) calc->setSrcInfo( src_info );
}

void RDOFUNLogic::setSrcInfo( const RDOParserSrcInfo& begin, const std::string& delim, const RDOParserSrcInfo& end )
{
	RDOParserSrcInfo::setSrcInfo( begin, delim, end );
	if ( calc ) calc->setSrcInfo( begin, delim, end );
}

void RDOFUNLogic::setSrcPos( const YYLTYPE& _error_pos )
{
	RDOParserSrcInfo::setSrcPos( _error_pos );
	if ( calc ) calc->setSrcPos( src_info().src_pos() );
}

void RDOFUNLogic::setSrcPos( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end )
{
	RDOParserSrcInfo::setSrcPos( _pos_begin, _pos_end );
	if ( calc ) calc->setSrcPos( src_info().src_pos() );
}

void RDOFUNLogic::setSrcPos( int first_line, int first_pos, int last_line, int last_pos )
{
	RDOParserSrcInfo::setSrcPos( first_line, first_pos, last_line, last_pos );
	if ( calc ) calc->setSrcPos( first_line, first_pos, last_line, last_pos );
}

void RDOFUNLogic::setSrcText( const std::string& value )
{
	RDOParserSrcInfo::setSrcText( value );
	if ( calc ) calc->setSrcText( value );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNArithm
// ----------------------------------------------------------------------------
RDOFUNArithm::RDOFUNArithm( RDOParser* _parser, RDORTPParamType::ParamType _type, rdoRuntime::RDOCalc* _calc, const RDOParserSrcInfo& src_info ):
	RDOParserObject( _parser ),
	type( _type ),
	enu( NULL ),
	str( "" ),
	calc( _calc )
{
	setSrcInfo( src_info );
}

RDOFUNArithm::RDOFUNArithm( const RDOParserObject* _parent, RDORTPParamType::ParamType _type, rdoRuntime::RDOCalc* _calc, const RDOParserSrcInfo& src_info ):
	RDOParserObject( _parent ),
	type( _type ),
	enu( NULL ),
	str( "" ),
	calc( _calc )
{
	setSrcInfo( src_info );
}

RDOFUNArithm::RDOFUNArithm( RDOParser* _parser, const RDOParserSrcInfo& res_name_src_info, const RDOParserSrcInfo& par_name_src_info ):
	RDOParserObject( _parser ),
	type( RDORTPParamType::pt_int ),
	enu( NULL ),
	str( "" ),
	calc( NULL )
{
	init( res_name_src_info, par_name_src_info );
}

RDOFUNArithm::RDOFUNArithm( const RDOParserObject* _parent, const RDOParserSrcInfo& res_name_src_info, const RDOParserSrcInfo& par_name_src_info ):
	RDOParserObject( _parent ),
	type( RDORTPParamType::pt_int ),
	enu( NULL ),
	str( "" ),
	calc( NULL )
{
	init( res_name_src_info, par_name_src_info );
}

void RDOFUNArithm::init( const RDOParserSrcInfo& res_name_src_info, const RDOParserSrcInfo& par_name_src_info )
{
	setSrcInfo( res_name_src_info, ".", par_name_src_info );
	const RDORSSResource* const res = parser->findRSSResource( res_name_src_info.src_text() ); 
	if ( res ) {
		// Это ресурс с закладки RSS
		if ( res->getType()->isTemporary() ) {
			parser->error( res_name_src_info, rdo::format("Нельзя использовать временный ресурс: %s", res_name_src_info.src_text().c_str()) );
//			parser->error(("Cannot use temporary resource in function: " + *res_name_src_info.src_text()).c_str());
		}
		int resNumb = res->getNumber();
		int parNumb = res->getType()->getRTPParamNumber( par_name_src_info.src_text() );
		if ( parNumb == -1 ) {
			parser->lexer_loc_set( par_name_src_info.src_pos().last_line, par_name_src_info.src_pos().last_pos );
			parser->error( rdo::format("Неизвестный параметр ресурса: %s", par_name_src_info.src_text().c_str()) );
		}
		calc = new rdoRuntime::RDOCalcGetResParam( parser->runtime, resNumb, parNumb );
		calc->setSrcInfo( src_info() );
		type = res->getType()->findRTPParam( par_name_src_info.src_text() )->getType()->getType();
		if ( type == RDORTPParamType::pt_enum ) {
			enu = ((RDORTPEnumParamType *)res->getType()->findRTPParam( par_name_src_info.src_text() )->getType())->enu;
		}
		return;
	} else {
		// Это не ресурс, но возможно релевантный ресурс или ресурс, внутри групповой функции
		if ( !parser->getFUNGroupStack().empty() && parser->getFUNGroupStack().back()->resType->getName() == res_name_src_info.src_text() ) {
			// Это ресурс внутри групповой функции
			RDOFUNGroup* currGroup = parser->getFUNGroupStack().back();
			int parNumb = currGroup->resType->getRTPParamNumber( par_name_src_info.src_text() );
			if ( parNumb == -1 ) {
				parser->error( par_name_src_info, rdo::format("Неизвестный параметр ресурса: %s", par_name_src_info.src_text().c_str()) );
			}
			calc = new rdoRuntime::RDOCalcGetGroupResParam( parser->runtime, parNumb );
			calc->setSrcInfo( src_info() );
			type = currGroup->resType->findRTPParam( par_name_src_info.src_text() )->getType()->getType();
			if ( type == RDORTPParamType::pt_enum ) {
				enu = ((RDORTPEnumParamType *)currGroup->resType->findRTPParam( par_name_src_info.src_text() )->getType())->enu;
			}
			return;
		} else if ( parser->getFileToParse() == rdoModelObjects::PAT && parser->getLastPATPattern() && parser->getLastPATPattern()->findRelevantResource( res_name_src_info.src_text() ) ) {
			// Это релевантный ресурс где-то в паттерне (with_min-common-choice, $Time, $Body)
			RDOPATPattern* pat = parser->getLastPATPattern();
			const RDORelevantResource* const rel = pat->findRelevantResource( res_name_src_info.src_text() );
			if ( !pat->currRelRes ) {
				// Внутри with_min-common-choice или $Time
				if ( rel->begin == rdoRuntime::RDOResourceTrace::CS_NonExist || rel->begin == rdoRuntime::RDOResourceTrace::CS_Create ) {
					parser->error( res_name_src_info, rdo::format("Релевантный ресурс не может быть использован, т.к. он еще не существует: %s", rel->getName().c_str()) );
				}
			} else {
				// Внутри $Body
				// Проверяем использование неинициализированного рел.ресурса (rel) в Choice from другом рел.ресурсе (pat->currRelRes)
				if ( pat->currRelRes->isChoiceFromState() ) {
					if ( !rel->alreadyHaveConverter && !rel->isDirect() ) {
						parser->error( res_name_src_info, rdo::format("Релевантный ресурс неопределен: %s. Его нельзя использовать в условиях выбора других ресурсов до его собственного Choice from", rel->getName().c_str()) );
					}
					if ( rel->begin == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
						parser->error( res_name_src_info, rdo::format("Релевантный ресурс в начале операции не существует (NonExist): %s", rel->getName().c_str()) );
					}
					if ( rel->begin == rdoRuntime::RDOResourceTrace::CS_Create ) {
						parser->error( res_name_src_info, rdo::format("Сразу после создания (Create) релевантный ресурс '%s' можно использовать только в конверторах, но не в условии выбора", rel->getName().c_str()) );
					}
				}
				// Проверяем использование временного рел.ресурса внутри конвертора другого рел.ресурса
				if ( rel->getType()->isTemporary() ) {
					// В конверторе начала
					if ( pat->currRelRes->currentState == RDORelevantResource::convertBegin ) {
						if ( rel->begin == rdoRuntime::RDOResourceTrace::CS_Create && !rel->alreadyHaveConverter ) {
							parser->error( res_name_src_info, rdo::format("Релевантный ресурс нельзя использовать до его создания (Create): %s", rel->getName().c_str()) );
						}
						if ( rel->begin == rdoRuntime::RDOResourceTrace::CS_Erase && rel->alreadyHaveConverter ) {
							parser->error( res_name_src_info, rdo::format("Релевантный ресурс нельзя использовать после удаления (Erase): %s", rel->getName().c_str()) );
						}
						if ( rel->begin == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
							parser->error( res_name_src_info, rdo::format("Релевантный ресурс не существует в этом конверторе (NonExist): %s", rel->getName().c_str()) );
						}
					}
					// В конверторе конца
					if ( pat->currRelRes->currentState == RDORelevantResource::convertEnd ) {
						if ( rel->end == rdoRuntime::RDOResourceTrace::CS_Create && !rel->alreadyHaveConverter ) {
							parser->error( res_name_src_info, rdo::format("Релевантный ресурс нельзя использовать до его создания (Create): %s", rel->getName().c_str()) );
						}
						if ( rel->end == rdoRuntime::RDOResourceTrace::CS_Erase && rel->alreadyHaveConverter ) {
							parser->error( res_name_src_info, rdo::format("Релевантный ресурс нельзя использовать после удаления (Erase): %s", rel->getName().c_str()) );
						}
						if ( rel->end == rdoRuntime::RDOResourceTrace::CS_NonExist ) {
							parser->error( res_name_src_info, rdo::format("Релевантный ресурс не существует в этом конверторе (NonExist): %s", rel->getName().c_str()) );
						}
					}
				}
				// Проверяем использование еще не инициализированного (только для Create) параметра рел. ресурса в его же конверторе
				const RDORTPParam* param = pat->currRelRes->getType()->findRTPParam( par_name_src_info.src_text() );
				if ( param && pat->currRelRes->getName() == res_name_src_info.src_text() ) {
					// В конверторе начала
					if ( pat->currRelRes->currentState == RDORelevantResource::convertBegin && pat->currRelRes->begin == rdoRuntime::RDOResourceTrace::CS_Create) {
						if ( !pat->currRelRes->getParamSetBegin()->isExist( par_name_src_info.src_text() ) ) {
							if ( !param->getType()->dv->isExist() ) {
								parser->error( par_name_src_info, rdo::format("Перед использованием параметра '%s' ему необходимо присвоить значение в текущем конверторе или указать значение по-умолчанию в типе ресурса", par_name_src_info.src_text().c_str()) );
							}
						}
					}
					// В конверторе начала
					if ( pat->currRelRes->currentState == RDORelevantResource::convertEnd && pat->currRelRes->end == rdoRuntime::RDOResourceTrace::CS_Create) {
						if ( !pat->currRelRes->getParamSetEnd()->isExist( par_name_src_info.src_text() ) ) {
							if ( !param->getType()->dv->isExist() ) {
								parser->error( par_name_src_info, rdo::format("Перед использованием параметра '%s' ему необходимо присвоить значение в текущем конверторе или указать значение по-умолчанию в типе ресурса", par_name_src_info.src_text().c_str()) );
							}
						}
					}
				}
			}
			int relResNumb = pat->findRelevantResourceNum( res_name_src_info.src_text() );
			int parNumb    = rel->getType()->getRTPParamNumber( par_name_src_info.src_text() );
			if ( parNumb == -1 ) {
				parser->error( par_name_src_info, rdo::format("Неизвестный параметр ресурса: %s", par_name_src_info.src_text().c_str()) );
//				parser->error( "Unknown resource parameter: " + *par_name_src_info.src_text() );
			}
			calc = new rdoRuntime::RDOCalcGetRelevantResParam( parser->runtime, relResNumb, parNumb );
			calc->setSrcInfo( src_info() );
			type = rel->getType()->findRTPParam( par_name_src_info.src_text() )->getType()->getType();
			if ( type == RDORTPParamType::pt_enum ) {
				enu = ((RDORTPEnumParamType*)rel->getType()->findRTPParam( par_name_src_info.src_text() )->getType())->enu;
			}
			return;
		} else if ( parser->getFileToParse() == rdoModelObjects::DPT && parser->getLastDPTSearch() && parser->getLastDPTSearch()->lastActivity && parser->getLastDPTSearch()->lastActivity->getRule() && parser->getLastDPTSearch()->lastActivity->getRule()->findRelevantResource( res_name_src_info.src_text() ) ) {
			// Это ресурс, который используется в DPT (condition, term_condition, evaluate_by, value before, value after)
			const RDORelevantResource* const rel = parser->getLastDPTSearch()->lastActivity->getRule()->findRelevantResource( res_name_src_info.src_text() );
			int relResNumb = parser->getLastDPTSearch()->lastActivity->getRule()->findRelevantResourceNum( res_name_src_info.src_text() );
			int parNumb    = rel->getType()->getRTPParamNumber( par_name_src_info.src_text() );
			if ( parNumb == -1 ) {
				parser->error( par_name_src_info, rdo::format("Неизвестный параметр ресурса: %s", par_name_src_info.src_text().c_str()) );
			}

			calc = new rdoRuntime::RDOCalcGetRelevantResParam( parser->runtime, relResNumb, parNumb );
			calc->setSrcInfo( src_info() );
			type = rel->getType()->findRTPParam( par_name_src_info.src_text() )->getType()->getType();
			if ( type == RDORTPParamType::pt_enum ) {
				enu = ((RDORTPEnumParamType *)rel->getType()->findRTPParam( par_name_src_info.src_text() )->getType())->enu;
			}
			return;
		}
	}
	parser->error( res_name_src_info, rdo::format("Неизвестный ресурс: %s", res_name_src_info.src_text().c_str()) );
//	parser->error(("Unknown resource name: " + *res_name_src_info.src_text()).c_str());
}

RDOFUNArithm::RDOFUNArithm( RDOParser* _parser, int value, const RDOParserSrcInfo& src_info ):
	RDOParserObject( _parser ),
	type( RDORTPParamType::pt_int ),
	enu( NULL ),
	str( "" ),
	calc( NULL )
{
	calc = new rdoRuntime::RDOCalcConst( parser->runtime, value );
	setSrcInfo( src_info );
}

RDOFUNArithm::RDOFUNArithm( const RDOParserObject* _parent, int value, const RDOParserSrcInfo& src_info ):
	RDOParserObject( _parent ),
	type( RDORTPParamType::pt_int ),
	enu( NULL ),
	str( "" ),
	calc( NULL )
{
	calc = new rdoRuntime::RDOCalcConst( parser->runtime, value );
	setSrcInfo( src_info );
}

RDOFUNArithm::RDOFUNArithm( RDOParser* _parser, double* value, const RDOParserSrcInfo& src_info ):
	RDOParserObject( _parser ),
	type( RDORTPParamType::pt_real ),
	enu( NULL ),
	str( "" ),
	calc( NULL )
{
	calc = new rdoRuntime::RDOCalcConst( parser->runtime, *value );
	setSrcInfo( src_info );
}

RDOFUNArithm::RDOFUNArithm( const RDOParserObject* _parent, double* value, const RDOParserSrcInfo& src_info ):
	RDOParserObject( _parent ),
	type( RDORTPParamType::pt_real ),
	enu( NULL ),
	str( "" ),
	calc( NULL )
{
	calc = new rdoRuntime::RDOCalcConst( parser->runtime, *value );
	setSrcInfo( src_info );
}

RDOFUNArithm::RDOFUNArithm( RDOParser* _parser, const std::string& value, const YYLTYPE& _pos ):
	RDOParserObject( _parser ),
	type( RDORTPParamType::pt_int ),
	enu( NULL ),
	str( "" ),
	calc( NULL )
{
	init( value, _pos );
}

RDOFUNArithm::RDOFUNArithm( const RDOParserObject* _parent, const std::string& value, const YYLTYPE& _pos ):
	RDOParserObject( _parent ),
	type( RDORTPParamType::pt_int ),
	enu( NULL ),
	str( "" ),
	calc( NULL )
{
	init( value, _pos );
}

void RDOFUNArithm::init( const std::string& value, const YYLTYPE& _pos )
{
	setSrcPos( _pos );
	setSrcText( value );

	if ( value == "Time_now" || value == "time_now" || value == "Системное_время" || value == "системное_время" ) {
		type = RDORTPParamType::pt_real;
		calc = new rdoRuntime::RDOCalcGetTimeNow( parser->runtime );
		calc->setSrcInfo( src_info() );
		return;
	}

	if ( value == "Seconds" || value == "seconds" ) {
		type = RDORTPParamType::pt_real;
		calc = new rdoRuntime::RDOCalcGetSeconds( parser->runtime );
		calc->setSrcInfo( src_info() );
		return;
	}

	// Ищем параметры паттерна или функции по имени
	const RDOFUNFunctionParam* param = NULL;
	switch ( parser->getFileToParse() ) {
		case rdoModelObjects::PAT: param = parser->getLastPATPattern()->findPATPatternParam( value ); break;
		case rdoModelObjects::FUN: param = parser->getLastFUNFunction()->findFUNFunctionParam( value ); break;
	}

	// Ищем константы по имени
	const RDOFUNConstant* cons = parser->findFUNConst( value );

	if ( cons && param ) {
		parser->lexer_loc_set( src_info().src_pos().last_line, src_info().src_pos().last_pos );
		parser->error( rdo::format("Имя параметра образца совпадает с именем константы: %s", value.c_str()) );
//		parser->error( "Ambiguity: constant or parameter usage: " + *value + " ?" );
	}

	if ( cons ) {
		type = cons->getType()->getType();
		if ( type == RDORTPParamType::pt_enum ) {
			enu = ((RDORTPEnumParamType *)cons->getType())->enu;
		}
		calc = new rdoRuntime::RDOCalcGetConst( parser->runtime, cons->number );
		calc->setSrcInfo( src_info() );
		return;
	}

	// Ищем последовательность по имени
	const RDOFUNSequence* seq = parser->findSequence( value );
	if ( seq && param ) {
		parser->lexer_loc_set( src_info().src_pos().last_line, src_info().src_pos().last_pos );
		parser->error( rdo::format("Имя параметра образца совпадает с именем последовательности: %s", value.c_str()) );
//		parser->error("Ambiguity: sequence or parameter usage: " + *value + " ?");
	}

	if ( seq ) {
		RDOFUNParams tmp( seq );
		RDOFUNArithm* arithm = tmp.createSeqCall( value );
		arithm->setSrcInfo( src_info() );
		type = arithm->getType();
		if ( type == RDORTPParamType::pt_enum ) {
			enu = arithm->enu;
		}
		calc = arithm->calc;
		return;
	}

	if ( param ) {
		// Это параметр
		type = param->getType()->getType();
		if ( type == RDORTPParamType::pt_enum ) {
			enu = ((RDORTPEnumParamType *)param->getType())->enu;
		}
		switch ( parser->getFileToParse() ) {
			case rdoModelObjects::PAT: calc = new rdoRuntime::RDOCalcPatParam( parser->runtime, parser->getLastPATPattern()->findPATPatternParamNum( value ) ); break;
			case rdoModelObjects::FUN: calc = new rdoRuntime::RDOCalcFuncParam( parser->runtime, parser->getLastFUNFunction()->findFUNFunctionParamNum( value ), param->src_info() ); break;
		}
		if ( calc ) calc->setSrcInfo( src_info() );
		return;
	} else {
		// Возможно, что это значение перечислимого типа
		std::vector< RDORTPEnumParamType* >::const_iterator it = parser->getEnums().begin();
		while ( it != parser->getEnums().end() ) {
			if ( (*it)->enu->findEnumValueWithoutThrow( value ) != -1 ) {
				// Да, это перечислимый тип, только одно и тоже значение может встречаться в разных
				// перечислимых типах, поэтому какой именно из них выбрать - вопрос
				type = RDORTPParamType::pt_unknow;
				str  = value;
				setSrcText( value );
				return;
			}
			it++;
		}
		parser->lexer_loc_set( _pos.last_line, _pos.last_column );
		parser->error( rdo::format("Неизвестный идентификатор: %s", value.c_str()) );
//		type = RDORTPParamType::pt_unknow;
//		str  = value;
//		return;
	}
}

RDOFUNArithm* RDOFUNArithm::operator +( RDOFUNArithm& second )
{
	RDORTPParamType::ParamType newType;

	if ( type == RDORTPParamType::pt_int && second.type == RDORTPParamType::pt_int ) {
		newType = RDORTPParamType::pt_int;
	} else if ( type == RDORTPParamType::pt_enum || second.type == RDORTPParamType::pt_enum ) {
		if ( type == RDORTPParamType::pt_enum ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
		}
		parser->error( "Нельзя складывать перечислимые данные" );
//		parser->error("cannot add enumerative types");
	} else if ( type == RDORTPParamType::pt_unknow || second.type == RDORTPParamType::pt_unknow ) {
		if ( type == RDORTPParamType::pt_unknow ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	} else {
		newType = RDORTPParamType::pt_real;
	}
	const rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(calc);
	const rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(second.calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser->runtime, calc1->calcValueBase( parser->runtime ) + calc2->calcValueBase( parser->runtime ) );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcPlus::getStaticSrcInfo(calc1, calc2) );
	} else {
		newCalc = new rdoRuntime::RDOCalcPlus( parser->runtime, calc, second.calc );
	}
	RDOFUNArithm* arithm = new RDOFUNArithm( this, newType, newCalc, newCalc->src_info() );
	return arithm;
}

RDOFUNArithm* RDOFUNArithm::operator -( RDOFUNArithm& second )
{
	RDORTPParamType::ParamType newType;

	if ( type == RDORTPParamType::pt_int && second.type == RDORTPParamType::pt_int ) {
		newType = RDORTPParamType::pt_int;
	} else if ( type == RDORTPParamType::pt_enum || second.type == RDORTPParamType::pt_enum ) {
		if ( type == RDORTPParamType::pt_enum ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
		}
		parser->error( "Нельзя вычитать перечислимые данные" );
//		parser->error("cannot subtract enumerative types");
	} else if ( type == RDORTPParamType::pt_unknow || second.type == RDORTPParamType::pt_unknow ) {
		if ( type == RDORTPParamType::pt_unknow ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	} else {
		newType = RDORTPParamType::pt_real;
	}
	const rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(calc);
	const rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(second.calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser->runtime, calc1->calcValueBase( parser->runtime ) - calc2->calcValueBase( parser->runtime ) );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcMinus::getStaticSrcInfo(calc1, calc2) );
	} else {
		newCalc = new rdoRuntime::RDOCalcMinus( parser->runtime, calc, second.calc );
	}
	RDOFUNArithm* arithm = new RDOFUNArithm( this, newType, newCalc, newCalc->src_info() );
	return arithm;
}

RDOFUNArithm* RDOFUNArithm::operator *( RDOFUNArithm& second )
{
	RDORTPParamType::ParamType newType;

	if ( type == RDORTPParamType::pt_int && second.type == RDORTPParamType::pt_int ) {
		newType = RDORTPParamType::pt_int;
	} else if ( type == RDORTPParamType::pt_enum || second.type == RDORTPParamType::pt_enum ) {
		if ( type == RDORTPParamType::pt_enum ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
		}
		parser->error( "Нельзя перемножать перечислимые данные" );
//		parser->error("cannot multiply enumerative types");
	} else if ( type == RDORTPParamType::pt_unknow || second.type == RDORTPParamType::pt_unknow ) {
		if ( type == RDORTPParamType::pt_unknow ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	} else {
		newType = RDORTPParamType::pt_real;
	}
/*
	const rdoRuntime::RDOCalcConst* calc1 = NULL;
	const rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(second.calc);
	bool  right = false;
	if ( calc2 ) {
		calc1 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(calc);
		if ( !calc1 && dynamic_cast<const rdoRuntime::RDOCalcBinary*>(calc) ) {
			calc1 = dynamic_cast<const rdoRuntime::RDOCalcBinary*>(calc)->getRightAsConst();
			right = calc1 ? true : false;
		}
	}
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser->runtime, calc1->calcValueBase( parser->runtime ) * calc2->calcValueBase( parser->runtime ) );
		if ( right ) {
			dynamic_cast<rdoRuntime::RDOCalcBinary*>(calc)->setRight( newCalc );
			return this;
		}
	} else {
		newCalc = new rdoRuntime::RDOCalcMult( parser->runtime, calc, second.calc );
	}
*/
	const rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(calc);
	const rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(second.calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser->runtime, calc1->calcValueBase( parser->runtime ) * calc2->calcValueBase( parser->runtime ) );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcMult::getStaticSrcInfo(calc1, calc2) );
	} else {
		newCalc = new rdoRuntime::RDOCalcMult( parser->runtime, calc, second.calc );
	}
	RDOFUNArithm* arithm = new RDOFUNArithm( this, newType, newCalc, newCalc->src_info() );
	return arithm;
}

RDOFUNArithm* RDOFUNArithm::operator /( RDOFUNArithm& second )
{
	RDORTPParamType::ParamType newType;

	if ( type == RDORTPParamType::pt_int && second.type == RDORTPParamType::pt_int ) {
		newType = RDORTPParamType::pt_int;
	} else if ( type == RDORTPParamType::pt_enum || second.type == RDORTPParamType::pt_enum ) {
		if ( type == RDORTPParamType::pt_enum ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
		}
		parser->error( "Нельзя разделить перечислимые данные" );
//		parser->error("cannot divide enumerative types");
	} else if ( type == RDORTPParamType::pt_unknow || second.type == RDORTPParamType::pt_unknow ) {
		if ( type == RDORTPParamType::pt_unknow ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	} else {
		newType = RDORTPParamType::pt_real;
	}
	const rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(calc);
	const rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(second.calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser->runtime, (double)calc1->calcValueBase( parser->runtime ) / (double)calc2->calcValueBase( parser->runtime ) );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcDiv::getStaticSrcInfo(calc1, calc2) );
	} else {
		newCalc = new rdoRuntime::RDOCalcDiv( parser->runtime, calc, second.calc );
	}
	rdoRuntime::RDOCalc* newCalc_div = newCalc;
	// TODO: перевод вещественного в целое при делении. А что делать с умножением и т.д. ?
	if ( newType == RDORTPParamType::pt_int ) {
		newCalc = new rdoRuntime::RDOCalcDoubleToInt( parser->runtime, newCalc );
		newCalc->setSrcInfo( newCalc_div->src_info() );
	}
	RDOFUNArithm* arithm = new RDOFUNArithm( this, newType, newCalc, newCalc->src_info() );
	return arithm;
}

RDOFUNLogic* RDOFUNArithm::operator <( RDOFUNArithm& second )
{
	if ( type == RDORTPParamType::pt_enum || second.type == RDORTPParamType::pt_enum ) {
		if ( type == RDORTPParamType::pt_enum ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
		}
		parser->error( "Нельзя сравнивать перечислимые данные" );
//		parser->error("cannot compare enumerative types");
	} else if ( type == RDORTPParamType::pt_unknow || second.type == RDORTPParamType::pt_unknow ) {
		if ( type == RDORTPParamType::pt_unknow ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	}
	const rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(calc);
	const rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(second.calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser->runtime, calc1->calcValueBase( parser->runtime ) < calc2->calcValueBase( parser->runtime ) );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcIsLess::getStaticSrcInfo(calc1, calc2) );
	} else {
		newCalc = new rdoRuntime::RDOCalcIsLess( parser->runtime, calc, second.calc );
	}
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	return logic;
}

RDOFUNLogic* RDOFUNArithm::operator >( RDOFUNArithm& second )
{
	if ( type == RDORTPParamType::pt_enum || second.type == RDORTPParamType::pt_enum ) {
		if ( type == RDORTPParamType::pt_enum ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
		}
		parser->error( "Нельзя сравнивать перечислимые данные" );
	} else if ( type == RDORTPParamType::pt_unknow || second.type == RDORTPParamType::pt_unknow ) {
		if ( type == RDORTPParamType::pt_unknow ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	}
	const rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(calc);
	const rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(second.calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser->runtime, calc1->calcValueBase( parser->runtime ) > calc2->calcValueBase( parser->runtime ) );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcIsGreater::getStaticSrcInfo(calc1, calc2) );
	} else {
		newCalc = new rdoRuntime::RDOCalcIsGreater( parser->runtime, calc, second.calc );
	}
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	return logic;
}

RDOFUNLogic* RDOFUNArithm::operator <=( RDOFUNArithm& second )
{
	if ( type == RDORTPParamType::pt_enum || second.type == RDORTPParamType::pt_enum ) {
		if ( type == RDORTPParamType::pt_enum ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
		}
		parser->error( "Нельзя сравнивать перечислимые данные" );
	} else if ( type == RDORTPParamType::pt_unknow || second.type == RDORTPParamType::pt_unknow ) {
		if ( type == RDORTPParamType::pt_unknow ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	}
	const rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(calc);
	const rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(second.calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser->runtime, calc1->calcValueBase( parser->runtime ) <= calc2->calcValueBase( parser->runtime ) );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcIsLEQ::getStaticSrcInfo(calc1, calc2) );
	} else {
		newCalc = new rdoRuntime::RDOCalcIsLEQ( parser->runtime, calc, second.calc );
	}
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	return logic;
}

RDOFUNLogic* RDOFUNArithm::operator >=( RDOFUNArithm& second )
{
	if ( type == RDORTPParamType::pt_enum || second.type == RDORTPParamType::pt_enum ) {
		if ( type == RDORTPParamType::pt_enum ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
		}
		parser->error( "Нельзя сравнивать перечислимые данные" );
	} else if ( type == RDORTPParamType::pt_unknow || second.type == RDORTPParamType::pt_unknow ) {
		if ( type == RDORTPParamType::pt_unknow ) {
			parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
		} else {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
			parser->error( rdo::format("Неизвестный идентификатор: %s", second.str.c_str()) );
		}
	}
	const rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(calc);
	const rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(second.calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser->runtime, calc1->calcValueBase( parser->runtime ) >= calc2->calcValueBase( parser->runtime ) );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcIsGEQ::getStaticSrcInfo(calc1, calc2) );
	} else {
		newCalc = new rdoRuntime::RDOCalcIsGEQ( parser->runtime, calc, second.calc );
	}
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	return logic;
}

RDOFUNLogic* RDOFUNArithm::operator ==( RDOFUNArithm& second )
{
	if ( type == RDORTPParamType::pt_enum && second.type == RDORTPParamType::pt_enum ) {
		if ( enu != second.enu ) {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
			parser->error( "Нельзя сравнивать разные перечислимые типы" );
//			parser->error("cannot compare different enumerative types");
		}
	} else if ( type == RDORTPParamType::pt_enum && second.type == RDORTPParamType::pt_unknow ) {
		second.calc = new rdoRuntime::RDOCalcConst( parser->runtime, enu->findEnumValueWithThrow( second.src_info(), second.str ) );
		second.calc->setSrcInfo( second.src_info() );
//	} else if ( (type >= 2) || (second.type >= 2) ) {
	} else if ( (type == RDORTPParamType::pt_unknow && second.type == RDORTPParamType::pt_enum) || (type == RDORTPParamType::pt_unknow && second.type == RDORTPParamType::pt_unknow) ) {
		parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		parser->error( rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
//		parser->error("cannot compare enumerative type with nonenumerative type");
	}
	const rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(calc);
	const rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(second.calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser->runtime, calc1->calcValueBase( parser->runtime ) == calc2->calcValueBase( parser->runtime ) );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcIsEqual::getStaticSrcInfo(calc1, calc2) );
	} else {
		newCalc = new rdoRuntime::RDOCalcIsEqual( parser->runtime, calc, second.calc );
	}
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	return logic;
}

RDOFUNLogic* RDOFUNArithm::operator !=( RDOFUNArithm& second )
{
	if ( type == RDORTPParamType::pt_enum && second.type == RDORTPParamType::pt_enum ) {
		if ( enu != second.enu ) {
			parser->lexer_loc_set( second.src_pos().last_line, second.src_pos().last_pos );
			parser->error( "Нельзя сравнивать разные перечислимые типы" );
		}
	} else if ( type == RDORTPParamType::pt_enum && second.type == RDORTPParamType::pt_unknow ) {
		second.calc = new rdoRuntime::RDOCalcConst( parser->runtime, enu->findEnumValueWithThrow( second.src_info(), second.str ) );
		second.calc->setSrcInfo( second.src_info() );
	} else if ( (type == RDORTPParamType::pt_unknow && second.type == RDORTPParamType::pt_enum) || (type == RDORTPParamType::pt_unknow && second.type == RDORTPParamType::pt_unknow) ) {
		parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		parser->error( rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
	}
	const rdoRuntime::RDOCalcConst* calc1 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(calc);
	const rdoRuntime::RDOCalcConst* calc2 = dynamic_cast<const rdoRuntime::RDOCalcConst*>(second.calc);
	rdoRuntime::RDOCalc* newCalc;
	if ( calc1 && calc2 ) {
		newCalc = new rdoRuntime::RDOCalcConst( parser->runtime, calc1->calcValueBase( parser->runtime ) != calc2->calcValueBase( parser->runtime ) );
		newCalc->setSrcInfo( rdoRuntime::RDOCalcIsNotEqual::getStaticSrcInfo(calc1, calc2) );
	} else {
		newCalc = new rdoRuntime::RDOCalcIsNotEqual( parser->runtime, calc, second.calc );
	}
	RDOFUNLogic* logic = new RDOFUNLogic( newCalc );
	logic->setSrcInfo( newCalc->src_info() );
	return logic;
}

rdoRuntime::RDOCalc* RDOFUNArithm::createCalc( const RDORTPParamType* const forType )
{
	if ( type != RDORTPParamType::pt_unknow ) {
		if ( forType == NULL ) {
			return calc;
		}
		if ( forType->getType() != RDORTPParamType::pt_int ) {
			return calc;
		}
		rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcInt( parser->runtime, calc );
		newCalc->setSrcInfo( src_info() );
		return newCalc;
	} else if ( type == RDORTPParamType::pt_unknow && !forType && !str.empty() ) {
		parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		parser->error( rdo::format( "Неизвестный идентификатор: %s", str.c_str()) );
	}

	if ( !forType ) {
		parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		parser->error( "Неизвестный тип параметра" );
	}

	rdoRuntime::RDOCalc* newCalc = new rdoRuntime::RDOCalcConst( parser->runtime, forType->getRSSEnumValue(str, src_info()) );
	newCalc->setSrcInfo( src_info() );
	return newCalc;
}

void RDOFUNArithm::setSrcInfo( const RDOParserSrcInfo& src_info )
{
	RDOParserSrcInfo::setSrcInfo( src_info );
	if ( calc ) calc->setSrcInfo( src_info );
}

void RDOFUNArithm::setSrcInfo( const RDOParserSrcInfo& begin, const std::string& delim, const RDOParserSrcInfo& end )
{
	RDOParserSrcInfo::setSrcInfo( begin, delim, end );
	if ( calc ) calc->setSrcInfo( begin, delim, end );
}

void RDOFUNArithm::setSrcPos( const YYLTYPE& _error_pos )
{
	RDOParserSrcInfo::setSrcPos( _error_pos );
	if ( calc ) calc->setSrcPos( src_info().src_pos() );
}

void RDOFUNArithm::setSrcPos( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end )
{
	RDOParserSrcInfo::setSrcPos( _pos_begin, _pos_end );
	if ( calc ) calc->setSrcPos( src_info().src_pos() );
}

void RDOFUNArithm::setSrcPos( int first_line, int first_pos, int last_line, int last_pos )
{
	RDOParserSrcInfo::setSrcPos( first_line, first_pos, last_line, last_pos );
	if ( calc ) calc->setSrcPos( src_info().src_pos() );
}

void RDOFUNArithm::setSrcText( const std::string& value )
{
	RDOParserSrcInfo::setSrcText( value );
	if ( calc ) calc->setSrcText( src_info().src_text() );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElement
// ----------------------------------------------------------------------------
rdoRuntime::RDOCalcIsEqual* RDOFUNFunctionListElement::createIsEqualCalc( const RDORTPParamType* const retType, const rdoRuntime::RDOCalcFuncParam* const funcParam, const RDOParserSrcInfo& _src_pos ) const
{
	rdoRuntime::RDOCalcConst* constCalc = createResultCalc( retType, _src_pos );
	return new rdoRuntime::RDOCalcIsEqual( parser->runtime, funcParam, constCalc );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementIdentif
// ----------------------------------------------------------------------------
rdoRuntime::RDOCalcConst* RDOFUNFunctionListElementIdentif::createResultCalc( const RDORTPParamType* const retType, const RDOParserSrcInfo& _src_pos ) const
{
	rdoRuntime::RDOCalcConst* const_calc = new rdoRuntime::RDOCalcConst( parser->runtime, retType->getRSSEnumValue( src_text(), _src_pos ) );
	const_calc->setSrcInfo( _src_pos );
	return const_calc;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementReal
// ----------------------------------------------------------------------------
rdoRuntime::RDOCalcConst* RDOFUNFunctionListElementReal::createResultCalc( const RDORTPParamType* const retType, const RDOParserSrcInfo& _src_pos ) const
{
	rdoRuntime::RDOCalcConst* const_calc = new rdoRuntime::RDOCalcConst( parser->runtime, retType->getRSSRealValue( value, _src_pos ) );
	const_calc->setSrcInfo( _src_pos );
	return const_calc;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementInt
// ----------------------------------------------------------------------------
rdoRuntime::RDOCalcConst* RDOFUNFunctionListElementInt::createResultCalc( const RDORTPParamType* const retType, const RDOParserSrcInfo& _src_pos ) const
{
	rdoRuntime::RDOCalcConst* const_calc = new rdoRuntime::RDOCalcConst( parser->runtime, retType->getRSSIntValue( value, _src_pos ) );
	const_calc->setSrcInfo( _src_pos );
	return const_calc;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunctionListElementEq
// ----------------------------------------------------------------------------
rdoRuntime::RDOCalcConst* RDOFUNFunctionListElementEq::createResultCalc( const RDORTPParamType* const retType, const RDOParserSrcInfo& _src_pos ) const
{
	parser->error( _src_pos, "Внутренная ошибка парсера: RDOFUNFunctionListElementEq::createResultCalc" );
//	parser->error( "Internal parser error, incorrect state" );
	return NULL;	// unreachable code
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNCalculateIf
// ----------------------------------------------------------------------------
RDOFUNCalculateIf::RDOFUNCalculateIf( const RDOParserObject* _parent, RDOFUNLogic* _condition, RDOFUNArithm* _action ):
	RDOParserObject( _parent ),
	condition( _condition ),
	action( _action )
{
	parser->getLastFUNFunction()->getType()->checkParamType( action );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNFunction
// ----------------------------------------------------------------------------
RDOFUNFunction::RDOFUNFunction( RDOParser* _parser, const RDOParserSrcInfo& _src_info, const RDORTPParamType* const _retType ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _src_info ),
	retType( _retType ),
	functionCalc( NULL )
{
	parser->insertFUNFunction( this );
}

RDOFUNFunction::RDOFUNFunction( RDOParser* _parser, const std::string& _name, const RDORTPParamType* const _retType ):
	RDOParserObject( _parser ),
	RDOParserSrcInfo( _name ),
	retType( _retType ),
	functionCalc( NULL )
{
	parser->insertFUNFunction( this );
}

const RDOFUNFunctionParam* const RDOFUNFunction::findFUNFunctionParam( const std::string& paramName ) const 
{
	std::vector< const RDOFUNFunctionParam* >::const_iterator it = std::find_if( params.begin(), params.end(), compareName2<RDOFUNFunctionParam>(paramName) );
	return it != params.end() ? *it : NULL;
}

int RDOFUNFunction::findFUNFunctionParamNum( const std::string& paramName ) const
{
	std::vector< const RDOFUNFunctionParam* >::const_iterator it = std::find_if( params.begin(), params.end(), compareName2<RDOFUNFunctionParam>(paramName) );
	return it != params.end() ? it - params.begin() : -1;
}

void RDOFUNFunction::add( const RDOFUNFunctionParam* const _param )
{ 
	const RDOFUNFunctionParam* const param = findFUNFunctionParam( _param->getName() );
	if ( param ) {
		parser->error_push_only( _param->src_info(), rdo::format("Параметр уже существует: %s", _param->getName().c_str()) );
		parser->error( param->src_info(), rdo::format("См. первое определение") );
//		parser->error("Second appearance of the same parameter name: " + *(_param->getName()));
	}
	params.push_back( _param ); 
}

void RDOFUNFunction::add( const RDOFUNFunctionListElement* const _param )
{
	elements.push_back( _param ); 
}

void RDOFUNFunction::add( const RDOFUNCalculateIf* const _calculateIf )
{
	calculateIf.push_back( _calculateIf );
}

void RDOFUNFunction::createListCalc()
{
	if ( !getType()->dv->isExist() ) {
		parser->error( getType()->dv->src_info(), rdo::format("Функция '%s' должна иметь значение по-умолчанию", getName().c_str()) );
//		parser->error(("list function " + *name + " must have default result value").c_str());
	}
	if ( params.empty() ) {
		parser->error( src_info(), rdo::format("Функция '%s' должна иметь параметры, т.к. её тип list", getName().c_str()) );
	}
	if ( elements.empty() ) {
		parser->warning( src_info(), rdo::format("Функция '%s' не содержит списка", getName().c_str()) );
	}

	rdoRuntime::RDOCalcConst* default_value_calc = new rdoRuntime::RDOCalcConst( parser->runtime, getType()->getParamDefaultValue( getType()->dv->src_info() ) );
	default_value_calc->setSrcInfo( getType()->dv->src_info() );
	rdoRuntime::RDOFunListCalc* fun_calc = new rdoRuntime::RDOFunListCalc( parser->runtime, default_value_calc );
	fun_calc->setSrcInfo( src_info() );
	const RDOFUNFunctionListElement* arg_last = NULL;
	std::vector< const RDOFUNFunctionListElement* >::const_iterator elem_it = elements.begin();
	while ( elem_it != elements.end() ) {
		rdoRuntime::RDOCalc* case_calc = new rdoRuntime::RDOCalcConst( parser->runtime, true );
		case_calc->setSrcInfo( (*elem_it)->src_info() );
		int currParam_number = 0;
		std::vector< const RDOFUNFunctionParam* >::const_iterator param_it = params.begin();
		while ( param_it != params.end() ) {
			const RDOFUNFunctionParam* const param = *param_it;
			if ( elem_it == elements.end() ) {
				elem_it--;
				parser->error( (*elem_it)->src_info(), rdo::format("Ожидается значение для параметра '%s'", param->getName().c_str()) );
			}
			const RDOFUNFunctionListElement* elem = *elem_it;
			if ( elem->isEquivalence() ) {
				if ( (param_it + 1) == params.end() ) {
					parser->error( elem->src_info(), rdo::format("Перед знаком равенства ожидается значение для параметра: %s",param->getName().c_str()) );
				} else {
					std::string str = (*param_it)->src_text();
					param_it++;
					while ( param_it != params.end() ) {
						str += ", ";
						str += (*param_it)->src_text();
						param_it++;
					}
					parser->error( elem->src_info(), rdo::format("Перед знаком равенства ожидаются значения для параметров: %s", str.c_str()) );
				}
			}
			rdoRuntime::RDOCalcFuncParam* funcParam  = new rdoRuntime::RDOCalcFuncParam( parser->runtime, currParam_number, param->src_info() );
			rdoRuntime::RDOCalcIsEqual* compare_calc = elem->createIsEqualCalc( param->getType(), funcParam, elem->src_info() );
			rdoRuntime::RDOCalc* and_calc = new rdoRuntime::RDOCalcAnd( parser->runtime, case_calc, compare_calc );
			case_calc = and_calc;
			elem_it++;
			param_it++;
			currParam_number++;
		}
		if ( elem_it == elements.end() ) {
			elem_it--;
			param_it--;
			parser->error( (*elem_it)->src_info(), rdo::format("После значения для параметра '%s' ожидается знак равенства", (*param_it)->getName().c_str()) );
		}
		if ( !(*elem_it)->isEquivalence() ) {
			parser->error( (*elem_it)->src_info(), rdo::format("Слишком много значений, вместо '%s' ожидается знак равенства", (*elem_it)->src_text().c_str()) );
		}
		elem_it++;
		if ( elem_it == elements.end() ) {
			elem_it--;
			parser->error( (*elem_it)->src_info(), rdo::format("После знака равенства ожидается значение функции '%s'", getName().c_str()) );
		}
		rdoRuntime::RDOCalcConst* result_calc = (*elem_it)->createResultCalc( getType(), (*elem_it)->src_info() );
		fun_calc->addCase( case_calc, result_calc );
		elem_it++;
	}
	functionCalc = fun_calc;
}

void RDOFUNFunction::createTableCalc()
{
	int param_cnt = params.size();
	int range = 1;
	rdoRuntime::RDOCalc* calc = new rdoRuntime::RDOCalcConst( parser->runtime, 0 );
	for ( int currParam = 0; currParam < param_cnt; currParam++ ) {
		const RDOFUNFunctionParam* const param  = params.at(currParam);
		rdoRuntime::RDOCalcFuncParam* funcParam = new rdoRuntime::RDOCalcFuncParam( parser->runtime, currParam, param->src_info() );
		rdoRuntime::RDOCalc* val2 = funcParam;
		if ( param->getType()->getType() != RDORTPParamType::pt_enum ) {
			rdoRuntime::RDOCalcConst* const1 = new rdoRuntime::RDOCalcConst( parser->runtime, 1 );
			const1->setSrcInfo( param->src_info() );
			val2 = new rdoRuntime::RDOCalcMinus( parser->runtime, val2, const1 );
		}
		rdoRuntime::RDOCalcConst* const2 = new rdoRuntime::RDOCalcConst( parser->runtime, range );
		const2->setSrcInfo( param->src_info() );
		rdoRuntime::RDOCalcMult* mult = new rdoRuntime::RDOCalcMult( parser->runtime, const2, val2 );
		rdoRuntime::RDOCalcPlus* add = new rdoRuntime::RDOCalcPlus( parser->runtime, mult, calc );
		range *= param->getType()->getDiapTableFunc();
		calc = add;
	}

	if ( elements.size() != range ) {
		parser->lexer_loc_restore();
		parser->error( rdo::format("Неверное количество элементов табличной функции '%s': %d, требуется %d", getName().c_str(), elements.size(), range) );
//		parser->error(("wrong number of value in table function " + *name).c_str());
	}

	rdoRuntime::RDOFuncTableCalc* fun_calc = new rdoRuntime::RDOFuncTableCalc( parser->runtime, calc );
	fun_calc->setSrcInfo( src_info() );
	for ( int currElem = 0; currElem < range; currElem++ ) {
		const RDOFUNFunctionListElement* const elem = elements.at(currElem);
		if ( elem->isEquivalence() ) {
			parser->error( elem->src_info(), "Символ '=' недопустим в табличной функции" );
//			parser->error("\"=\" unexpected in table function");
		}
		rdoRuntime::RDOCalcConst* result_calc = elem->createResultCalc( getType(), elem->src_info() );
		fun_calc->addResultCalc( result_calc );
	}
	functionCalc = fun_calc;
}

void RDOFUNFunction::createAlgorithmicCalc( const RDOParserSrcInfo& _body_src_info )
{
	rdoRuntime::RDOFunAlgorithmicCalc* funcCalc = NULL;
	switch ( getType()->getType() ) {
		case RDORTPParamType::pt_int: if ( static_cast<const RDORTPIntParamType*>(getType())->diap->isExist() ) {
			funcCalc = new rdoRuntime::RDOFunAlgorithmicDiapCalc( parser->runtime, static_cast<const RDORTPIntParamType*>(getType())->diap->minVal, static_cast<const RDORTPIntParamType*>(getType())->diap->maxVal );
			break;
		}
		case RDORTPParamType::pt_real: if ( static_cast<const RDORTPRealParamType*>(getType())->diap->isExist() ) {
			funcCalc = new rdoRuntime::RDOFunAlgorithmicDiapCalc( parser->runtime, static_cast<const RDORTPRealParamType*>(getType())->diap->minVal, static_cast<const RDORTPRealParamType*>(getType())->diap->maxVal );
			break;
		}
	}
	if ( !funcCalc ) {
		funcCalc = new rdoRuntime::RDOFunAlgorithmicCalc( parser->runtime );
	}
	funcCalc->setSrcInfo( src_info() );
	bool default_flag = false;
	bool true_const   = false;
	const rdoRuntime::RDOCalcConst* calc_cond_const = NULL;
	int size = calculateIf.size();
	int cnt = 0;
	for ( int i = 0; i < size; i++ ) {
		calc_cond_const = dynamic_cast<const rdoRuntime::RDOCalcConst*>(calculateIf[i]->condition->calc);
		if ( true_const ) {
			parser->warning( calculateIf[i]->condition->src_info(), rdo::format("Условие не используется: %s", calculateIf[i]->condition->src_text().c_str()) );
			parser->warning( calc_cond_const->src_info(), rdo::format("Последнее рабочее условие функции: %s", calc_cond_const->src_text().c_str()) );
		} else if ( !calc_cond_const || calc_cond_const->calcValueBase( parser->runtime ) ) {
			// Игнорируем чистые false-условия предыдущей проверкой
			funcCalc->addCalcIf( calculateIf[i]->condition->calc, calculateIf[i]->action->createCalc(getType()) );
			cnt++;
		}
		if ( calc_cond_const && calc_cond_const->calcValueBase( parser->runtime ) ) {
			true_const   = true;
			default_flag = true;
		}
	}
	if ( !cnt ) {
		parser->warning( src_info(), rdo::format("Отсутствует тело функции '%s'", getName().c_str()) );
	}
	if ( !true_const ) {
		if ( getType()->dv->isExist() ) {
			rdoRuntime::RDOCalcConst* calc_cond = new rdoRuntime::RDOCalcConst( parser->runtime, 1 );
			rdoRuntime::RDOCalcConst* calc_act  = new rdoRuntime::RDOCalcConst( parser->runtime, getType()->getParamDefaultValue( getType()->dv->src_info() ) );
			calc_cond->setSrcInfo( getType()->src_info() );
			calc_act->setSrcInfo( getType()->src_info() );
			funcCalc->addCalcIf( calc_cond, calc_act );
			default_flag = true;
		}
	}
	if ( !default_flag ) {
		rdoRuntime::RDOCalcConst* calc_cond = new rdoRuntime::RDOCalcConst( parser->runtime, 1 );
		rdoRuntime::RDOCalcConst* calc_act  = NULL;
		switch ( getType()->getType() ) {
			case RDORTPParamType::pt_int: if ( static_cast<const RDORTPIntParamType*>(getType())->diap->isExist() ) {
				calc_act = new rdoRuntime::RDOCalcConst( parser->runtime, static_cast<const RDORTPIntParamType*>(getType())->diap->minVal );
				break;
			}
			case RDORTPParamType::pt_real: if ( static_cast<const RDORTPRealParamType*>(getType())->diap->isExist() ) {
				calc_act = new rdoRuntime::RDOCalcConst( parser->runtime, static_cast<const RDORTPRealParamType*>(getType())->diap->minVal );
				break;
			}
		}
		if ( !calc_act ) {
			calc_act = new rdoRuntime::RDOCalcConst( parser->runtime, 0 );
		}
		calc_cond->setSrcInfo( getType()->src_info() );
		calc_act->setSrcInfo( getType()->src_info() );
		funcCalc->addCalcIf( calc_cond, calc_act );
		parser->warning( src_info(), rdo::format("Для функции '%s' неопределено значение по-умолчанию", getName().c_str()) );
	}
	functionCalc = funcCalc;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNParams
// ----------------------------------------------------------------------------
RDOFUNArithm* RDOFUNParams::createCall( const std::string& funName ) const
{
	const RDOFUNFunction* const func = parser->findFunction( funName );
	if ( !func ) {
		return createSeqCall( funName );
	}

	int nParams = func->getParams().size();
	if ( nParams != params.size() ) {
		parser->lexer_loc_set( src_pos().last_line, src_pos().last_pos );
		parser->error( rdo::format("Неверное количество параметров функции: %s", funName.c_str()) );
//		parser->error(("Wrong parameters number in function call: " + *funName).c_str());
	}

	rdoRuntime::RDOCalcFunctionCall* funcCall = new rdoRuntime::RDOCalcFunctionCall( parser->runtime, func->getFunctionCalc() );
	funcCall->setSrcInfo( src_info() );
	for ( int i = 0; i < nParams; i++ ) {
		const RDORTPParamType* const funcParam = func->getParams()[i]->getType();
		RDOFUNArithm* arithm = params[i];
		switch ( funcParam->getType() ) {
			case RDORTPParamType::pt_int: {
				rdoRuntime::RDOCalc* arg = arithm->createCalc( NULL );
				if ( arithm->type == RDORTPParamType::pt_real ) {
					arg = new rdoRuntime::RDOCalcDoubleToInt( parser->runtime, arg );
				}
				if ( static_cast<const RDORTPIntParamType*>(funcParam)->diap->isExist() ) {
					arg = new rdoRuntime::RDOCalcCheckDiap( parser->runtime, static_cast<const RDORTPIntParamType*>(funcParam)->diap->minVal, static_cast<const RDORTPIntParamType*>(funcParam)->diap->maxVal, arg );
				}
				funcCall->addParameter( arg );
				break;
			}
			case RDORTPParamType::pt_real: {
				rdoRuntime::RDOCalc* arg = arithm->createCalc( NULL );
				if( static_cast<const RDORTPRealParamType*>(funcParam)->diap->isExist() ) {
					arg = new rdoRuntime::RDOCalcCheckDiap( parser->runtime, static_cast<const RDORTPRealParamType*>(funcParam)->diap->minVal, static_cast<const RDORTPRealParamType*>(funcParam)->diap->maxVal, arg );
				}
				funcCall->addParameter(arg);
				break;
			}
			default: {
				if ( arithm->type == RDORTPParamType::pt_int || arithm->type == RDORTPParamType::pt_real ) {
					parser->lexer_loc_set( arithm->src_pos().last_line, arithm->src_pos().last_pos );
					parser->error( "Неверный тип параметра функции" );
//					parser->error("wrong parameter type");
				}

				rdoRuntime::RDOCalc* arg;
				if ( arithm->type == RDORTPParamType::pt_enum ) {
					if ( arithm->enu != static_cast<const RDORTPEnumParamType*>(funcParam)->enu ) {
						parser->error( "Перечислимые типы не совпадают" );
//						parser->error("wrong enumerative parameter type");
					}
					arg = arithm->createCalc( NULL );
				} else {
					arg = new rdoRuntime::RDOCalcConst( parser->runtime, static_cast<const RDORTPEnumParamType*>(funcParam)->enu->findEnumValueWithThrow( arithm->src_info(), arithm->str ) );
					arg->setSrcInfo( arithm->src_info() );
				}
				funcCall->addParameter(arg);
				break;
			}
		}
	}

	RDOFUNArithm* res = new RDOFUNArithm( this, RDORTPParamType::pt_int, funcCall, src_pos() );
	res->type = func->getType()->getType();
	if ( func->getType()->getType() == RDORTPParamType::pt_enum ) {
		res->enu = ((RDORTPEnumParamType *)func->getType())->enu;
	}
	res->setSrcInfo( src_info() );

	return res;
}

RDOFUNArithm* RDOFUNParams::createSeqCall( const std::string& seqName ) const
{
	const RDOFUNSequence* const seq = parser->findSequence( seqName );
	if ( !seq ) {
		parser->lexer_loc_set( name_error_pos.src_pos().last_line, name_error_pos.src_pos().last_pos );
		parser->error( rdo::format("Неопределенная функция или последовательность: %s", seqName.c_str()) );
//		parser->error( "Undefined function or sequence: " + *seqName );
	}
	RDOFUNArithm* arithm = seq->createCallCalc( this, src_info() );
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroup
// ----------------------------------------------------------------------------
RDOFUNGroup::RDOFUNGroup( RDOParser* _parser, const std::string& _resType ):
	RDOParserObject( _parser )
{
	init( _resType );
}

RDOFUNGroup::RDOFUNGroup( const RDOParserObject* _parent, const std::string& _resType ):
	RDOParserObject( _parent )
{
	init( _resType );
}

void RDOFUNGroup::init( const std::string& _resType )
{
	resType = parser->findRTPResType( _resType );
	if ( !resType ) {
		parser->error( rdo::format("Неизвестный тип ресурса: %s", _resType.c_str()) );
	}
	parser->insertFUNGroup( this );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNGroupLogic
// ----------------------------------------------------------------------------
RDOFUNLogic* RDOFUNGroupLogic::createFunLogic( RDOFUNLogic* cond )
{
	rdoRuntime::RDOFunCalcGroup* calc;
	switch ( funType ) {
		case 1 : setSrcText( "Exist(" + resType->getName() + ": " + cond->src_text() + ")" );     calc = new rdoRuntime::RDOFunCalcExist    ( parser->runtime, resType->getNumber(), cond->calc ); break;
		case 2 : setSrcText( "NotExist(" + resType->getName() + ": " + cond->src_text() + ")" );  calc = new rdoRuntime::RDOFunCalcNotExist ( parser->runtime, resType->getNumber(), cond->calc ); break;
		case 3 : setSrcText( "ForAll(" + resType->getName() + ": " + cond->src_text() + ")" );    calc = new rdoRuntime::RDOFunCalcForAll   ( parser->runtime, resType->getNumber(), cond->calc ); break;
		case 4 : setSrcText( "NotForAll(" + resType->getName() + ": " + cond->src_text() + ")" ); calc = new rdoRuntime::RDOFunCalcNotForAll( parser->runtime, resType->getNumber(), cond->calc ); break;
		default: parser->error( "Несуществующий тип функции" );
	}
	parser->getFUNGroupStack().pop_back();
	calc->setSrcInfo( src_info() );
	RDOFUNLogic* logic = new RDOFUNLogic( calc );
	logic->setSrcInfo( src_info() );
	return logic;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSelect
// ----------------------------------------------------------------------------
// Сам Select как выборка по типу и условию
RDOFUNLogic* RDOFUNSelect::createFunSelect( RDOFUNLogic* cond )
{
	select = new rdoRuntime::RDOFunCalcSelect( parser->runtime, resType->getNumber(), cond ? cond->calc : new rdoRuntime::RDOCalcConst(parser->runtime, 1) );
	return new RDOFUNLogic( select );
}

// Групповая функция над выборкой Select'а
RDOFUNLogic* RDOFUNSelect::createFunSelectGroup( int funType, RDOFUNLogic* cond )
{
	rdoRuntime::RDOFunCalcSelectBase* calc;
	switch ( funType ) {
		case 1 : setSrcText( src_text() + ".Exist(" + cond->src_text() + ")" );     calc = new rdoRuntime::RDOFunCalcSelectExist    ( parser->runtime, select, cond->calc ); break;
		case 2 : setSrcText( src_text() + ".NotExist(" + cond->src_text() + ")" );  calc = new rdoRuntime::RDOFunCalcSelectNotExist ( parser->runtime, select, cond->calc ); break;
		case 3 : setSrcText( src_text() + ".ForAll(" + cond->src_text() + ")" );    calc = new rdoRuntime::RDOFunCalcSelectForAll   ( parser->runtime, select, cond->calc ); break;
		case 4 : setSrcText( src_text() + ".NotForAll(" + cond->src_text() + ")" ); calc = new rdoRuntime::RDOFunCalcSelectNotForAll( parser->runtime, select, cond->calc ); break;
		default: parser->error( "Неизвестный метод для списка ресурсов" );
	}
	parser->getFUNGroupStack().pop_back();
	calc->setSrcInfo( src_info() );
	RDOFUNLogic* logic = new RDOFUNLogic( calc );
	logic->setSrcInfo( src_info() );
	return logic;
}

RDOFUNLogic* RDOFUNSelect::createFunSelectEmpty()
{
	setSrcText( src_text() + ".Empty()" );
	parser->getFUNGroupStack().pop_back();
	RDOFUNLogic* logic = new RDOFUNLogic( select );
	logic->setSrcInfo( src_info() );
	return logic;
}

RDOFUNArithm* RDOFUNSelect::createFunSelectSize()
{
	setSrcText( src_text() + ".Size()" );
	parser->getFUNGroupStack().pop_back();
	RDOFUNArithm* arithm = new RDOFUNArithm( this, RDORTPParamType::pt_int, new rdoRuntime::RDOFunCalcSelectSize( parser->runtime, select ), src_pos() );
	arithm->setSrcInfo( src_info() );
	return arithm;
}

void RDOFUNSelect::setSrcInfo( const RDOParserSrcInfo& src_info )
{
	RDOParserSrcInfo::setSrcInfo( src_info );
	if ( select ) select->setSrcInfo( src_info );
}

void RDOFUNSelect::setSrcPos( const YYLTYPE& _error_pos )
{
	RDOParserSrcInfo::setSrcPos( _error_pos );
	if ( select ) select->setSrcPos( src_info().src_pos() );
}

void RDOFUNSelect::setSrcPos( const YYLTYPE& _pos_begin, const YYLTYPE& _pos_end )
{
	RDOParserSrcInfo::setSrcPos( _pos_begin, _pos_end );
	if ( select ) select->setSrcPos( src_info().src_pos() );
}

void RDOFUNSelect::setSrcPos( int first_line, int first_pos, int last_line, int last_pos )
{
	RDOParserSrcInfo::setSrcPos( first_line, first_pos, last_line, last_pos );
	if ( select ) select->setSrcPos( src_info().src_pos() );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequence
// ----------------------------------------------------------------------------
RDOFUNSequence::RDOFUNSequence( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base ):
	RDOParserObject( _parser ),
	header( _header ),
	base( _base )
{
	parser->insertFUNSequences( this );
}

void RDOFUNSequence::initResult()
{
	initCalcSrcInfo();
	switch ( header->type->getType() ) {
		case RDORTPParamType::pt_int: {
			next->res_real = false;
			if ( static_cast<RDORTPIntParamType*>(header->type)->diap->isExist() ) {
				next->diap     = true;
				next->diap_min = static_cast<RDORTPIntParamType*>(header->type)->diap->minVal;
				next->diap_max = static_cast<RDORTPIntParamType*>(header->type)->diap->maxVal;
			}
			break;
		}
		case RDORTPParamType::pt_real: {
			if ( static_cast<RDORTPRealParamType*>(header->type)->diap->isExist() ) {
				next->diap     = true;
				next->diap_min = static_cast<RDORTPRealParamType*>(header->type)->diap->minVal;
				next->diap_max = static_cast<RDORTPRealParamType*>(header->type)->diap->maxVal;
			}
			break;
		}
	}
}

void RDOFUNSequence::initCalcSrcInfo()
{
	initSeq->setSrcInfo( header->src_info() );
	initSeq->setSrcText( "Инициализация последовательности " + initSeq->src_text() );
	next->setSrcInfo( header->src_info() );
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceUniform
// ----------------------------------------------------------------------------
RDOFUNSequenceUniform::RDOFUNSequenceUniform( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base ):
	RDOFUNSequence( _parser, _header, _base )
{
	if ( header->type->getType() == RDORTPParamType::pt_enum ) {
		parser->error( "Последовательность типа uniform не может иметь перечислимый тип" );
//		parser->error("Uniform sequence cannot be enumerative type");
	}
	createCalcs();
}

void RDOFUNSequenceUniform::createCalcs()
{
	rdoRuntime::RandGeneratorUniform* gen = new rdoRuntime::RandGeneratorUniform();
	initSeq = new rdoRuntime::RDOCalcSeqInit( parser->runtime, base, gen );
	parser->runtime->addInitCalc( initSeq );
	next = new rdoRuntime::RDOCalcSeqNextUniform( parser->runtime, gen );
	initResult();
}

RDOFUNArithm* RDOFUNSequenceUniform::createCallCalc( const RDOFUNParams* const param, const RDOParserSrcInfo& src_info ) const
{
	if ( param->params.size() != 2 ) {
		parser->lexer_loc_set( src_info.src_pos().last_line, src_info.src_pos().last_pos );
		parser->error( rdo::format("Для равномерного закона распределения '%s' нужно указать два параметра: минимальную и максимальную границы диапазона", header->name.c_str()) );
//		parser->error("Wrong parameters number in uniform sequence call: " + *header->name);
	}

	rdoRuntime::RDOCalcFunctionCall* funcCall = new rdoRuntime::RDOCalcFunctionCall( parser->runtime, next );
	RDOFUNArithm *arithm1 = param->params[0];
	rdoRuntime::RDOCalc *arg1 = arithm1->createCalc(NULL);
	RDOFUNArithm *arithm2 = param->params[1];
	rdoRuntime::RDOCalc *arg2 = arithm2->createCalc(NULL);

	funcCall->addParameter(arg1);
	funcCall->addParameter(arg2);

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceUniform*>(this), RDORTPParamType::pt_int, funcCall, param->src_pos() );
	arithm->setSrcInfo( src_info );
	arithm->type = header->type->getType();
	if ( arithm->type == RDORTPParamType::pt_enum ) {
		parser->lexer_loc_set( src_info.src_pos().last_line, src_info.src_pos().last_pos );
		parser->error( "Внутренняя ошибка парсера" );
//		parser->error("Internal parser error");
	}
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceExponential
// ----------------------------------------------------------------------------
RDOFUNSequenceExponential::RDOFUNSequenceExponential( RDOParser* _parser, RDOFUNSequenceHeader *_header, int _base ):
	RDOFUNSequence( _parser, _header, _base )
{
	if ( header->type->getType() == RDORTPParamType::pt_enum ) {
		parser->error( "Последовательность типа exponential не может иметь перечислимый тип" );
//		parser->error("Exponential sequence cannot be enumerative type");
	}

	createCalcs();
}

void RDOFUNSequenceExponential::createCalcs()
{
	rdoRuntime::RandGeneratorExponential* gen = new rdoRuntime::RandGeneratorExponential();
	initSeq = new rdoRuntime::RDOCalcSeqInit( parser->runtime, base, gen );
	parser->runtime->addInitCalc( initSeq );
	next = new rdoRuntime::RDOCalcSeqNextExponential( parser->runtime, gen );
	initResult();
}

RDOFUNArithm* RDOFUNSequenceExponential::createCallCalc( const RDOFUNParams* const param, const RDOParserSrcInfo& src_info ) const
{
	if ( param->params.size() != 1 ) {
		parser->lexer_loc_set( src_info.src_pos().last_line, src_info.src_pos().last_pos );
		parser->error( rdo::format("Для экспоненциального закона распределения '%s' нужно указать один параметр: математическое ожидание", header->name.c_str()) );
//		parser->error("Wrong parameters number in exponential sequence call: " + *header->name);
	}

	rdoRuntime::RDOCalcFunctionCall* funcCall = new rdoRuntime::RDOCalcFunctionCall( parser->runtime, next );
	RDOFUNArithm *arithm1 = param->params[0];
	rdoRuntime::RDOCalc *arg1 = arithm1->createCalc(NULL);

	funcCall->addParameter(arg1);

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceExponential*>(this), RDORTPParamType::pt_int, funcCall, param->src_pos() );
	arithm->setSrcInfo( src_info );
	arithm->type = header->type->getType();
	if ( arithm->type == RDORTPParamType::pt_enum ) {
		parser->lexer_loc_set( src_info.src_pos().last_line, src_info.src_pos().last_pos );
		parser->error( "Внутренняя ошибка парсера" );
//		parser->error("Internal parser error");
	}
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceNormal
// ----------------------------------------------------------------------------
RDOFUNSequenceNormal::RDOFUNSequenceNormal( RDOParser* _parser, RDOFUNSequenceHeader* _header, int _base ):
	RDOFUNSequence( _parser, _header, _base )
{
	if ( header->type->getType() == RDORTPParamType::pt_enum ) {
		parser->error( "Последовательность типа normal не может иметь перечислимый тип" );
//		parser->error("Normal sequence cannot be enumerative type");
	}
	createCalcs();
}

void RDOFUNSequenceNormal::createCalcs()
{
	rdoRuntime::RandGeneratorNormal* gen = new rdoRuntime::RandGeneratorNormal();
	initSeq = new rdoRuntime::RDOCalcSeqInit( parser->runtime, base, gen );
	parser->runtime->addInitCalc( initSeq );
	next = new rdoRuntime::RDOCalcSeqNextNormal( parser->runtime, gen );
	initResult();
}

RDOFUNArithm* RDOFUNSequenceNormal::createCallCalc( const RDOFUNParams* const param, const RDOParserSrcInfo& src_info ) const
{
	if ( param->params.size() != 2 ) {
		parser->lexer_loc_set( src_info.src_pos().last_line, src_info.src_pos().last_pos );
		parser->error( rdo::format("Для нормального закона распределения '%s' нужно указать два параметра: математическое ожидание и среднее квадратическое отклонение", header->name.c_str()) );
//		parser->error("Wrong parameters number in normal sequence call: " + *header->name);
	}

	rdoRuntime::RDOCalcFunctionCall *funcCall = new rdoRuntime::RDOCalcFunctionCall(parser->runtime, next);
	RDOFUNArithm *arithm1 = param->params[0];
	rdoRuntime::RDOCalc *arg1 = arithm1->createCalc(NULL);
	RDOFUNArithm *arithm2 = param->params[1];
	rdoRuntime::RDOCalc *arg2 = arithm2->createCalc(NULL);

	funcCall->addParameter(arg1);
	funcCall->addParameter(arg2);

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceNormal*>(this), RDORTPParamType::pt_int, funcCall, param->src_pos() );
	arithm->setSrcInfo( src_info );
	arithm->type = header->type->getType();
	if ( arithm->type == RDORTPParamType::pt_enum ) {
		parser->lexer_loc_set( src_info.src_pos().last_line, src_info.src_pos().last_pos );
		parser->error( "Внутренняя ошибка парсера" );
//		parser->error("Internal parser error");
	}
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHist
// ----------------------------------------------------------------------------
RDOFUNArithm* RDOFUNSequenceByHist::createCallCalc( const RDOFUNParams* const param, const RDOParserSrcInfo& src_info ) const
{
	if ( param->params.size() != 0 ) {
		parser->lexer_loc_set( src_info.src_pos().last_line, src_info.src_pos().last_pos );
		parser->error( rdo::format("Гистограмма '%s' должна вызываться без параметров", header->name.c_str()) );
//		parser->error("Wrong parameters number in by_hist sequence call: " + *header->name);
	}

	rdoRuntime::RDOCalcFunctionCall* funcCall = new rdoRuntime::RDOCalcFunctionCall( parser->runtime, next );

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceByHist*>(this), RDORTPParamType::pt_int, funcCall, param->src_pos() );
	arithm->setSrcInfo( src_info );
	arithm->type = header->type->getType();
	if ( arithm->type == RDORTPParamType::pt_enum ) {
		arithm->enu = ((RDORTPEnumParamType *)header->type)->enu;
	}
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHistReal
// ----------------------------------------------------------------------------
void RDOFUNSequenceByHistReal::addReal( double _from, double _to, double _freq )
{
	from.push_back( _from );
	to.push_back( _to );
	freq.push_back( _freq );
}

void RDOFUNSequenceByHistReal::createCalcs()
{
	rdoRuntime::RandGeneratorByHist *gen = new rdoRuntime::RandGeneratorByHist();
	int size = from.size();
	for ( int i = 0; i < size; i++ ) {
		gen->addValues(from[i], to[i], freq[i]);
	}

	initSeq = new rdoRuntime::RDOCalcSeqInit(parser->runtime, base, gen);
	parser->runtime->addInitCalc(initSeq);
	next = new rdoRuntime::RDOCalcSeqNextByHist(parser->runtime, gen);
	initCalcSrcInfo();
	initResult();
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceByHistEnum
// ----------------------------------------------------------------------------
void RDOFUNSequenceByHistEnum::addEnum( const std::string& _val, double _freq )
{
	rdoRuntime::RDOValue enum_id = header->type->getRSSEnumValue( _val, RDOParserSrcInfo("левое") );
	if ( std::find( val.begin(), val.end(), enum_id ) != val.end() ) {
		parser->error( rdo::format("Перечислимое значение определено дважды: %s", _val.c_str()) );
	}
	val.push_back( enum_id );
	freq.push_back( _freq );
}

void RDOFUNSequenceByHistEnum::createCalcs()
{
	rdoRuntime::RandGeneratorByHistEnum* gen = new rdoRuntime::RandGeneratorByHistEnum();
	int size = val.size();
	for ( int i = 0; i < size; i++ ) {
		gen->addValues( val[i], freq[i] );
	}
	initSeq = new rdoRuntime::RDOCalcSeqInit( parser->runtime, base, gen );
	parser->runtime->addInitCalc( initSeq );
	next = new rdoRuntime::RDOCalcSeqNextByHist( parser->runtime, gen );
	initCalcSrcInfo();
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerative
// ----------------------------------------------------------------------------
RDOFUNArithm* RDOFUNSequenceEnumerative::createCallCalc( const RDOFUNParams* const param, const RDOParserSrcInfo& src_info ) const
{
	if ( param->params.size() != 0 ) {
		parser->lexer_loc_set( src_info.src_pos().last_line, src_info.src_pos().last_pos );
		parser->error( rdo::format("Перечисление '%s' должно вызываться без параметров", header->name.c_str()) );
//		parser->error("Wrong parameters number in enumerative sequence call: " + *header->name);
	}

	rdoRuntime::RDOCalcFunctionCall *funcCall = new rdoRuntime::RDOCalcFunctionCall(parser->runtime, next);

	RDOFUNArithm* arithm = new RDOFUNArithm( const_cast<RDOFUNSequenceEnumerative*>(this), RDORTPParamType::pt_int, funcCall, param->src_pos() );
	arithm->setSrcInfo( src_info );
	arithm->type = header->type->getType();
	if ( arithm->type == RDORTPParamType::pt_enum ) {
		arithm->enu = ((RDORTPEnumParamType *)header->type)->enu;
	}
	return arithm;
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerativeInt
// ----------------------------------------------------------------------------
void RDOFUNSequenceEnumerativeInt::addInt( int _val )
{
	val.push_back(_val);
}

void RDOFUNSequenceEnumerativeInt::createCalcs()
{
	rdoRuntime::RandGeneratorEnumerative* gen = new rdoRuntime::RandGeneratorEnumerative();
	int size = val.size();
	for ( int i = 0; i < size; i++ ) {
		gen->addValue(val[i]);
	}
	initSeq = new rdoRuntime::RDOCalcSeqInit(parser->runtime, base, gen);
	parser->runtime->addInitCalc(initSeq);
	next = new rdoRuntime::RDOCalcSeqNextByHist(parser->runtime, gen);
	initCalcSrcInfo();
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerativeReal
// ----------------------------------------------------------------------------
void RDOFUNSequenceEnumerativeReal::addReal( double _val )
{
	val.push_back( _val );
}

void RDOFUNSequenceEnumerativeReal::createCalcs()
{
	rdoRuntime::RandGeneratorEnumerative *gen = new rdoRuntime::RandGeneratorEnumerative();
	int size = val.size();
	for(int i = 0; i < size; i++)
		gen->addValue(val[i]);

	initSeq = new rdoRuntime::RDOCalcSeqInit(parser->runtime, base, gen);
	parser->runtime->addInitCalc(initSeq);
	next = new rdoRuntime::RDOCalcSeqNextByHist(parser->runtime, gen);
	initCalcSrcInfo();
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNSequenceEnumerativeEnum
// ----------------------------------------------------------------------------
void RDOFUNSequenceEnumerativeEnum::addEnum( const std::string& _val )
{
	val.push_back( header->type->getRSSEnumValue(_val, RDOParserSrcInfo("левое")) );
}

void RDOFUNSequenceEnumerativeEnum::createCalcs()
{
	rdoRuntime::RandGeneratorEnumerative *gen = new rdoRuntime::RandGeneratorEnumerative();
	int size = val.size();
	for(int i = 0; i < size; i++)
		gen->addValue(val[i]);

	initSeq = new rdoRuntime::RDOCalcSeqInit(parser->runtime, base, gen);
	parser->runtime->addInitCalc(initSeq);
	next = new rdoRuntime::RDOCalcSeqNextByHist(parser->runtime, gen);
	initCalcSrcInfo();
}

// ----------------------------------------------------------------------------
// ---------- RDOFUNConstant
// ----------------------------------------------------------------------------
RDOFUNConstant::RDOFUNConstant( RDOParser* _parser, RDORTPParam* _param ):
	RDOParserObject( _parser ),
	param( _param ),
	number( _parser->getFUNCONST_id() )
{
	parser->insertFUNConstant( this );
}

} // namespace rdoParse
