/*
 * copyright: (c) RDO-Team, 2009
 * filename : rdortp.y
 * author   : Александ Барс, Урусов Андрей
 * date     : 20.02.2003
 * bref     : Синтаксис типов ресурсов
 * indent   : 4T
 */

%{
#define YYPARSE_PARAM lexer
#define YYLEX_PARAM lexer
%}

%pure-parser

%token RDO_Resource_type
%token RDO_permanent
%token RDO_Parameters
%token RDO_integer
%token RDO_real
%token RDO_End
%token RDO_temporary
%token RDO_IDENTIF
%token RDO_INT_CONST
%token RDO_REAL_CONST
%token RDO_such_as
%token RDO_dblpoint
%token RDO_Resources
%token RDO_trace
%token RDO_no_trace
%token RDO_IDENTIF_COLON
%token RDO_Constant
%token RDO_Body
%token RDO_Function
%token RDO_Type
%token RDO_algorithmic
%token RDO_table
%token RDO_list
%token RDO_Exist
%token RDO_Not_Exist
%token RDO_For_All
%token RDO_Not_For_All
%token RDO_neq
%token RDO_leq
%token RDO_geq
%token RDO_NoCheck
%token RDO_Calculate_if
%token RDO_or
%token RDO_and
%token RDO_Sequence
%token RDO_uniform
%token RDO_exponential
%token RDO_normal
%token RDO_by_hist
%token RDO_enumerative

%token RDO_Pattern
%token RDO_operation
%token RDO_rule
%token RDO_keyboard
%token RDO_Relevant_resources
%token RDO_Keep
%token RDO_Create
%token RDO_Erase
%token RDO_NonExist
%token RDO_IDENTIF_NoChange
%token RDO_Time
%token RDO_Choice
%token RDO_from
%token RDO_first
%token RDO_Convert_begin
%token RDO_Convert_end
%token RDO_Convert_rule
%token RDO_Convert_event
%token RDO_with_max
%token RDO_with_min
%token RDO_set
%token RDO_IDENTIF_NoChange_NoChange
%token RDO_Operations

%token RDO_Results
%token RDO_watch_par
%token RDO_watch_state
%token RDO_watch_quant
%token RDO_watch_value
%token RDO_get_value

%token RDO_Model_name
%token RDO_Resource_file
%token RDO_OprIev_file
%token RDO_Frame_file
%token RDO_Statistic_file
%token RDO_Results_file
%token RDO_Trace_file
%token RDO_Show_mode
%token RDO_Frame_number
%token RDO_Show_rate
%token RDO_Run_StartTime
%token RDO_Trace_StartTime
%token RDO_Trace_EndTime
%token RDO_Terminate_if
%token RDO_Break_point
%token RDO_Seed
%token RDO_NoShow
%token RDO_Monitor
%token RDO_Animation
%token RDO_NoChange

%token RDO_Decision_point
%token RDO_search
%token RDO_trace_stat
%token RDO_trace_tops
%token RDO_trace_all
%token RDO_Condition
%token RDO_Term_condition
%token RDO_Evaluate_by
%token RDO_Compare_tops
%token RDO_NO
%token RDO_YES
%token RDO_Activities
%token RDO_value_before
%token RDO_value_after
%token RDO_some
%token RDO_Process
%token RDO_SEIZE
%token RDO_GENERATE
%token RDO_TERMINATE
%token RDO_ADVANCE
%token RDO_RELEASE
%token RDO_if
%token RDO_result
%token RDO_CF
%token RDO_Priority
%token RDO_prior
%token RDO_Parent
%token RDO_PlusEqual
%token RDO_MinusEqual
%token RDO_MultiplyEqual
%token RDO_DivideEqual
%token RDO_array
%token RDO_event
%token RDO_Planning
%token RDO_else
%token RDO_IncrEqual
%token RDO_DecrEqual
%token RDO_Stopping
%token RDO_Start
%token RDO_Stop

%token RDO_Frame
%token RDO_Show_if
%token RDO_Back_picture
%token RDO_Show
%token RDO_frm_cell
%token RDO_text
%token RDO_bitmap
%token RDO_s_bmp
%token RDO_rect
%token RDO_r_rect
%token RDO_line
%token RDO_ellipse
%token RDO_triang
%token RDO_active
%token RDO_ruler
%token RDO_space
%token RDO_color_transparent
%token RDO_color_last
%token RDO_color_white
%token RDO_color_black
%token RDO_color_red
%token RDO_color_green
%token RDO_color_blue
%token RDO_color_cyan
%token RDO_color_magenta
%token RDO_color_yellow
%token RDO_color_gray

%token RDO_IDENTIF_RELRES
%token RDO_typedef
%token RDO_enum

%token RDO_STRING_CONST
%token RDO_STRING_CONST_BAD
%token RDO_IDENTIF_BAD
%token RDO_Select
%token RDO_Size
%token RDO_Empty
%token RDO_not
%token RDO_UMINUS
%token RDO_string
%token RDO_bool
%token RDO_BOOL_CONST
%token RDO_Fuzzy
%token RDO_Fuzzy_Term
%token RDO_eq
%token RDO_External_Model
%token RDO_QUEUE
%token RDO_DEPART
%token RDO_ASSIGN

%{
// ====================================================================== PCH
#include "rdo_lib/rdo_parser/pch.h"
// ====================================================================== INCLUDES
// ====================================================================== SYNOPSIS
#include "rdo_lib/rdo_parser/rdoparser.h"
#include "rdo_lib/rdo_parser/rdoparser_lexer.h"
#include "rdo_lib/rdo_parser/rdortp.h"
#include "rdo_lib/rdo_parser/rdofun.h"
#include "rdo_lib/rdo_parser/rdo_type_range.h"
#include "rdo_lib/rdo_parser/rdo_array.h"
// ===============================================================================

#define PARSER  LEXER->parser()
#define RUNTIME PARSER->runtime()

#define P_RDOVALUE(A) reinterpret_cast<PTR(RDOValue)>(A)
#define RDOVALUE(A)   (*P_RDOVALUE(A))

OPEN_RDO_PARSER_NAMESPACE
%}

%start type_list

%%

type_list
	: /* empty */
	| type_list rtp_res_type
	| type_list ext_param_type
	| error
	{
		PARSER->error().error(rdoParse::RDOParserSrcInfo(), _T("Ожидается ключевое слово $Resource_type"));
	}
	;

ext_param_type
	: RDO_typedef RDO_enum ext_par_type_enum
	;

ext_par_type_enum
	: param_type_enum RDO_IDENTIF
	{
		PTR(RDOEnumType) enu = reinterpret_cast<PTR(RDOEnumType)>($1);
	}
	;

rtp_res_type
	: rtp_header RDO_Parameters rtp_body RDO_End
	{
		LPRDORTPResType pResourceType = PARSER->stack().pop<RDORTPResType>($1);
		ASSERT(pResourceType);
		if (pResourceType->getParams().empty())
		{
			PARSER->error().warning(@2, rdo::format(_T("Тип ресурса '%s' не содежит параметров"), pResourceType->name().c_str()));
		}
		$$ = PARSER->stack().push(pResourceType);
	}
	| rtp_header RDO_Parameters rtp_body
	{
		PARSER->error().error(@2, _T("Не найдено ключевое слово $End"));
	}
	| rtp_header error
	{
		PARSER->error().error(@2, _T("Не найдено ключевое слово $Parameters"));
	}
	;

rtp_header
	: RDO_Resource_type RDO_IDENTIF_COLON rtp_vid_res
	{
		LEXER->enumReset();
		PTR(RDOValue)       type_name = P_RDOVALUE($2);
		tstring             name      = type_name->value().getIdentificator();
		LPRDORTPResType     _rtp      = PARSER->findRTPResType(name);
		if (_rtp)
		{
			PARSER->error().push_only(type_name->src_info(), rdo::format(_T("Тип ресурса уже существует: %s"), name.c_str()));
			PARSER->error().push_only(_rtp->src_info(), _T("См. первое определение"));
			PARSER->error().push_done();
		}
		LPRDORTPResType pResourceType = rdo::Factory<RDORTPResType>::create(PARSER, type_name->src_info(), $3 != 0);
		ASSERT(pResourceType);
		$$ = PARSER->stack().push(pResourceType);
	}
	|	RDO_Resource_type RDO_IDENTIF_COLON RDO_IDENTIF_COLON rtp_vid_res
	{
		LEXER->enumReset();
		PTR(RDOValue)       type_name = P_RDOVALUE($2);
		tstring             name      = type_name->value().getIdentificator();
		LPRDORTPResType     _rtp      = PARSER->findRTPResType(name);
		if (_rtp)
		{
			PARSER->error().push_only(type_name->src_info(), rdo::format(_T("Тип ресурса уже существует: %s"), name.c_str()));
			PARSER->error().push_only(_rtp->src_info(), _T("См. первое определение"));
			PARSER->error().push_done();
		}
		PTR(RDOValue)       prnt_type_name = P_RDOVALUE($3);
		tstring             prnt_name      = prnt_type_name->value().getIdentificator();
		LPRDORTPResType     _rtp_prnt      = PARSER->findRTPResType(prnt_name);

		if (_rtp_prnt)
		{
			LPRDORTPResType pResourceType = rdo::Factory<RDORTPResType>::create(PARSER, type_name->src_info(), $4 != 0);
			ASSERT(pResourceType);
			ruint t_ind   = 0;
			ruint col_par = _rtp_prnt->getParams().size();
			while (t_ind < col_par)
			{
				pResourceType->addParam(_rtp_prnt->getParams()[t_ind]);
				PARSER->error().warning(_rtp_prnt->getParams()[t_ind]->src_info(), rdo::format(_T("Параметр %s передан от родителя %s потомку %s"), _rtp_prnt->getParams()[t_ind]->src_info().src_text().c_str(), prnt_name.c_str(), name.c_str()));
				t_ind++;
			}
			$$ = PARSER->stack().push(pResourceType);
			PARSER->error().warning(@2, rdo::format(_T("Тип ресурса %s является потомком типа ресурса %s"), name.c_str(), prnt_name.c_str()));
		}
		else
		{
			PARSER->error().push_only(@3, rdo::format(_T("Родительский тип ресурса не существует: %s"), prnt_name.c_str()));
			PARSER->error().push_done();
		}
	}
	| RDO_Resource_type RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@2, _T("Не указан вид ресурса"));
	}
	| RDO_Resource_type RDO_IDENTIF_COLON RDO_IDENTIF_COLON error
	{
		PARSER->error().error(@3, _T("Не указан вид ресурса"));
	}
	| RDO_Resource_type error
	{
		tstring str(LEXER->YYText());
		PARSER->error().error(@2, rdo::format(_T("Ошибка в описании имени типа ресурса: %s"), str.c_str()));
	}
	;

rtp_vid_res
	: RDO_permanent	{$$ = 1;}
	| RDO_temporary	{$$ = 0;}
	;

rtp_body
	: /* empty */
	| rtp_body rtp_param
	{
		LPRDORTPParam pParam = PARSER->stack().pop<RDORTPParam>($2);
		PARSER->getLastRTPResType()->addParam(pParam);
	}
	;

rtp_param
	: RDO_IDENTIF_COLON param_type
	{
		PTR(RDOValue)  param_name = P_RDOVALUE($1);
		LPRDOTypeParam param_type = PARSER->stack().pop<RDOTypeParam>($2);
		LPRDORTPParam  pParam     = rdo::Factory<RDORTPParam>::create(PARSER->getLastRTPResType(), param_type, param_name->src_info());
		$$ = PARSER->stack().push(pParam);
	}
	| RDO_IDENTIF_COLON error
	{
		if (PARSER->lexer_loc_line() == @1.m_last_line)
		{
			tstring str(LEXER->YYText());
			PARSER->error().error(@2, rdo::format(_T("Неверный тип параметра: %s"), str.c_str()));
		}
		else
		{
			PARSER->error().error(@1, _T("Ожидается тип параметра"));
		}
	}
	| error
	{
		PARSER->error().error(@1, _T("Неправильное описание параметра"));
	}
	;

// ----------------------------------------------------------------------------
// ---------- Описание типа параметра
// ----------------------------------------------------------------------------
param_type
	: RDO_integer param_type_range param_value_default
	{
		LPRDOTypeRangeRange pRange = PARSER->stack().pop<RDOTypeRangeRange>($2);
		LPRDOTypeParam pType;
		if (pRange)
		{
			if (pRange->getMin().typeID() != rdoRuntime::RDOType::t_int ||
			    pRange->getMax().typeID() != rdoRuntime::RDOType::t_int)
			{
				PARSER->error().error(@2, _T("Диапазон целого типа должен быть целочисленным"));
			}
			LPRDOTypeIntRange pIntRange = rdo::Factory<RDOTypeIntRange>::create(pRange);
			ASSERT(pIntRange);
			pType = rdo::Factory<RDOTypeParam>::create(pIntRange, RDOVALUE($3), RDOParserSrcInfo(@1, @3));
		}
		else
		{
			pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__int>::create(), RDOVALUE($3), RDOParserSrcInfo(@1, @3));
		}
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_real param_type_range param_value_default
	{
		LPRDOTypeRangeRange pRange = PARSER->stack().pop<RDOTypeRangeRange>($2);
		LPRDOTypeParam pType;
		if (pRange)
		{
			LPRDOTypeRealRange pRealRange = rdo::Factory<RDOTypeRealRange>::create(pRange);
			ASSERT(pRealRange);
			pType = rdo::Factory<RDOTypeParam>::create(pRealRange, RDOVALUE($3), RDOParserSrcInfo(@1, @3));
		}
		else
		{
			pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__real>::create(), RDOVALUE($3), RDOParserSrcInfo(@1, @3));
		}
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_string param_value_default
	{
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__string>::create(), RDOVALUE($2), RDOParserSrcInfo(@1, @2));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_array param_value_default
	{
		LPRDOArrayType pArray = PARSER->stack().pop<RDOArrayType>($1);
		ASSERT(pArray);
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(pArray, RDOVALUE($2), RDOParserSrcInfo(@1, @2));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| RDO_bool param_value_default
	{
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(rdo::Factory<RDOType__bool>::create(), RDOVALUE($2), RDOParserSrcInfo(@1, @2));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_enum param_value_default
	{
		LEXER->enumReset();
		LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
		ASSERT(pEnum);
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(pEnum, RDOVALUE($2), RDOParserSrcInfo(@1, @2));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	| param_type_such_as param_value_default
	{
		LPRDOTypeParam pTypeSuchAs = PARSER->stack().pop<RDOTypeParam>($1);
		ASSERT(pTypeSuchAs);
		RDOValue default = RDOVALUE($2);
		if (!default.defined())
		{
			if (pTypeSuchAs->default().defined())
			{
				default = pTypeSuchAs->default();
			}
		}
		LPRDOTypeParam pType = rdo::Factory<RDOTypeParam>::create(pTypeSuchAs->type(), default, RDOParserSrcInfo(@1, @2));
		ASSERT(pType);
		$$ = PARSER->stack().push(pType);
	}
	;
/*
				| RDO_integer error {
					PARSER->error().error(@2, _T("Ошибка после ключевого слова integer. Возможно, не хватает значения по-умолчанию."));
				}
				| RDO_real error {
					PARSER->error().error(@2, _T("Ошибка после ключевого слова real. Возможно, не хватает значения по-умолчанию."));
				}
				| param_type_enum error {
					PARSER->error().error(@2, _T("Ошибка после перечислимого типа. Возможно, не хватает значения по-умолчанию."));
				};
*/

param_type_range
	: /* empty */
	{
		$$ = PARSER->stack().push<RDOTypeRangeRange>(LPRDOTypeRangeRange());
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST ']'
	{
		LPRDOTypeRangeRange pRange = rdo::Factory<RDOTypeRangeRange>::create(RDOVALUE($2), RDOVALUE($4), RDOParserSrcInfo(@1, @5));
		ASSERT(pRange);
		pRange->checkRange();
		$$ = PARSER->stack().push(pRange);
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_REAL_CONST error
	{
		PARSER->error().error(@4, _T("Диапазон задан неверно"));
	}
	| '[' RDO_REAL_CONST RDO_dblpoint RDO_INT_CONST error
	{
		PARSER->error().error(@4, _T("Диапазон задан неверно"));
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_REAL_CONST error
	{
		PARSER->error().error(@4, _T("Диапазон задан неверно"));
	}
	| '[' RDO_INT_CONST RDO_dblpoint RDO_INT_CONST error
	{
		PARSER->error().error(@4, _T("Диапазон задан неверно"));
	}
	| '[' RDO_REAL_CONST RDO_dblpoint error
	{
		PARSER->error().error(@4, _T("Диапазон задан неверно"));
	}
	| '[' RDO_INT_CONST RDO_dblpoint error
	{
		PARSER->error().error(@4, _T("Диапазон задан неверно"));
	}
	| '[' error
	{
		PARSER->error().error(@2, _T("Диапазон задан неверно"));
	}
	;

param_type_enum
	: '(' param_type_enum_list ')'
	{
		LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($2);
		ASSERT(pEnum);
		$$ = PARSER->stack().push(pEnum);
	}
	| '(' param_type_enum_list error
	{
		PARSER->error().error(@2, _T("Перечисление должно заканчиваться скобкой"));
	}
	;

param_type_enum_list
	: RDO_IDENTIF
	{
		LPRDOEnumType pEnum = rdo::Factory<RDOEnumType>::create();
		ASSERT(pEnum);
		pEnum->add(RDOVALUE($1));
		LEXER->enumBegin();
		$$ = PARSER->stack().push(pEnum);
	}
	| param_type_enum_list ',' RDO_IDENTIF
	{
		if (!LEXER->enumEmpty())
		{
			LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
			ASSERT(pEnum);
			pEnum->add(RDOVALUE($3));
			$$ = PARSER->stack().push(pEnum);
		}
		else
		{
			PARSER->error().error(@3, _T("Ошибка в описании значений перечислимого типа"));
		}
	}
	| param_type_enum_list RDO_IDENTIF
	{
		if (!LEXER->enumEmpty())
		{
			LPRDOEnumType pEnum = PARSER->stack().pop<RDOEnumType>($1);
			ASSERT(pEnum);
			pEnum->add(RDOVALUE($2));
			$$ = PARSER->stack().push(pEnum);
			PARSER->error().warning(@1, rdo::format(_T("Пропущена запятая перед: %s"), RDOVALUE($2)->getIdentificator().c_str()));
		}
		else
		{
			PARSER->error().error(@2, _T("Ошибка в описании значений перечислимого типа"));
		}
	}
	| param_type_enum_list ',' RDO_INT_CONST
	{
		PARSER->error().error(@3, _T("Значение перечислимого типа не может быть цифрой"));
	}
	| param_type_enum_list ',' RDO_REAL_CONST
	{
		PARSER->error().error(@3, _T("Значение перечислимого типа не может быть цифрой"));
	}
	| param_type_enum_list RDO_INT_CONST
	{
		PARSER->error().error(@2, _T("Значение перечислимого типа не может быть цифрой"));
	}
	| param_type_enum_list RDO_REAL_CONST
	{
		PARSER->error().error(@2, _T("Значение перечислимого типа не может быть цифрой"));
	}
	| RDO_INT_CONST
	{
		PARSER->error().error(@1, _T("Значение перечислимого типа не может начинаться с цифры"));
	}
	| RDO_REAL_CONST
	{
		PARSER->error().error(@1, _T("Значение перечислимого типа не может начинаться с цифры"));
	}
	;

param_type_such_as
	: RDO_such_as RDO_IDENTIF '.' RDO_IDENTIF
	{
		tstring type  = RDOVALUE($2)->getIdentificator();
		tstring param = RDOVALUE($4)->getIdentificator();
		LPRDORTPResType pResType = PARSER->findRTPResType(type);
		if (!pResType)
		{
			PARSER->error().error(@2, rdo::format(_T("Ссылка на неизвестный тип ресурса: %s"), type.c_str()));
		}
		LPRDORTPParam pParam = pResType->findRTPParam(param);
		if (!pParam)
		{
			PARSER->error().error(@4, rdo::format(_T("Ссылка на неизвестный параметр ресурса: %s.%s"), type.c_str(), param.c_str()));
		}
		$$ = PARSER->stack().push(pParam->getParamType());
	}
	| RDO_such_as RDO_IDENTIF
	{
		tstring constName = RDOVALUE($2)->getIdentificator();
		LPRDOFUNConstant pConstant = PARSER->findFUNConstant(constName);
		if (!pConstant)
		{
			PARSER->error().error(@2, rdo::format(_T("Ссылка на несуществующую константу: %s"), constName.c_str()));
		}
		$$ = PARSER->stack().push(pConstant->getType());
	}
	| RDO_such_as RDO_IDENTIF '.' error
	{
		tstring type = RDOVALUE($2)->getIdentificator();
		LPRDORTPResType pResType = PARSER->findRTPResType(type);
		if (!pResType)
		{
			PARSER->error().error(@2, rdo::format(_T("Ссылка на неизвестный тип ресурса: %s"), type.c_str()));
		}
		else
		{
			PARSER->error().error(@4, _T("Ошибка при указании параметра"));
		}
	}
	| RDO_such_as error
	{
		PARSER->error().error(@2, _T("После ключевого слова such_as необходимо указать тип и параметер ресурса для ссылки"));
	}
	;

param_value_default
	: /* empty */
	{
		$$ = (int)PARSER->addValue(new rdoParse::RDOValue());
	}
	| '=' param_value
	{
		$$ = $2;
	}
	| '=' error
	{
		RDOParserSrcInfo src_info(@1, @2, true);
		if (src_info.src_pos().point())
		{
			PARSER->error().error(src_info, _T("Не указано значение по-умолчанию"));
		}
		else
		{
			PARSER->error().error(src_info, _T("Неверное значение по-умолчанию"));
		}
	}
	;

param_value
	: RDO_INT_CONST
	{
		$$ = $1;
	}
	| RDO_REAL_CONST {
		$$ = $1;
	}
	| RDO_STRING_CONST {
		$$ = $1;
	}
	| RDO_IDENTIF {
		$$ = $1;
	}
	| RDO_BOOL_CONST {
		$$ = $1;
	}
	| param_array_value {
		$$ = $1;
	}
	;

param_type_array
	: RDO_array '<' param_type '>'
	{
		LPRDOTypeParam pParamType = PARSER->stack().pop<RDOTypeParam>($3);
		ASSERT(pParamType);
		LPRDOArrayType pArray = rdo::Factory<RDOArrayType>::create(pParamType->type(), RDOParserSrcInfo(@1, @4));
		$$ = PARSER->stack().push(pArray);
	}
	;

param_array_value
	:	'[' array_item ']'
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($2);
		ASSERT(pArrayValue);
		$$ = (int)PARSER->addValue(new RDOValue(pArrayValue->getRArray(), pArrayValue->getArrayType(), RDOParserSrcInfo(@2)));
	}
	|'[' array_item error {
		PARSER->error().error(@2, _T("Массив должен закрываться скобкой"));
	}
	;

array_item
	: param_value
	{
		LPRDOArrayType pArrayType = rdo::Factory<RDOArrayType>::create(RDOVALUE($1).type(), RDOParserSrcInfo(@1));
		ASSERT(pArrayType);
		LPRDOArrayValue pArrayValue = rdo::Factory<RDOArrayValue>::create(pArrayType);
		pArrayValue->insertItem(RDOVALUE($1));
		$$ = PARSER->stack().push(pArrayValue);
	}
	| array_item ',' param_value
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($1);
		ASSERT(pArrayValue);
		pArrayValue->insertItem(RDOVALUE($3));
		$$ = PARSER->stack().push(pArrayValue);
	}
	| array_item param_value
	{
		LPRDOArrayValue pArrayValue = PARSER->stack().pop<RDOArrayValue>($1);
		ASSERT(pArrayValue);
		pArrayValue->insertItem(RDOVALUE($2));
		$$ = PARSER->stack().push(pArrayValue);
		PARSER->error().warning(@1, rdo::format(_T("Пропущена запятая перед: %s"), RDOVALUE($2)->getAsString().c_str()));
	}
	;

// ----------------------------------------------------------------------------

%%

CLOSE_RDO_PARSER_NAMESPACE
