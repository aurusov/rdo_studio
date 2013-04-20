/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_lexer.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      11.06.2006
  \brief     Лексика РДО
  \indent    4T
*/

%{
// ---------------------------------------------------------------------------- PCH
#include "simulator/compiler/parser/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/rdolocale.h"
#include "simulator/compiler/parser/grammar/rdobison.h"
#include "simulator/compiler/parser/grammar/rdogramma.h"
#include "simulator/compiler/parser/rdoparser.h"
#include "simulator/compiler/parser/rdoparser_lexer.h"
#include "simulator/compiler/parser/rdo_value.h"
#include "simulator/compiler/parser/rdopat.h"
#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

#ifdef LEXER
#undef LEXER
#endif
#ifdef LEXER_POS
#undef LEXER_POS
#endif

#define LEXER     reinterpret_cast<PTR(rdo::compiler::parser::RDOLexer)>(this)
#define LEXER_POS (*LEXER->m_lploc)
#define PARSER    LEXER->parser()
#define RUNTIME   PARSER->runtime()

#define YY_INTERACTIVE

#define YY_USER_INIT {   \
	LEXER->loc_init();   \
}

#define YY_USER_ACTION { \
	LEXER->loc_action(); \
}

%}
%option yylineno
%option c++
%option nounistd
%option noyywrap
%x comment_cpp
%x comment_line
%%

$Resource_type                 return(RDO_Resource_type);
$resource_type                 return(RDO_Resource_type);
$Resources                     return(RDO_Resources);
$resources                     return(RDO_Resources);
permanent                      return(RDO_permanent);
temporary                      return(RDO_temporary);
$Parameters                    return(RDO_Parameters);
$parameters                    return(RDO_Parameters);
$End                           return(RDO_End);
$end                           return(RDO_End);
such_as                        return(RDO_such_as);
".."                           return(RDO_dblpoint);

integer                        return(RDO_integer);
longint                        return(RDO_integer);
real                           return(RDO_real);
string                         return(RDO_string);
bool                           return(RDO_bool);
array                          return(RDO_array);

$Constant                      return(RDO_Constant);
$constant                      return(RDO_Constant);
$Body                          return(RDO_Body);
$body                          return(RDO_Body);
$Function                      return(RDO_Function);
$function                      return(RDO_Function);
$Type                          return(RDO_Type);
$type                          return(RDO_Type);
algorithmic                    return(RDO_algorithmic);
table                          return(RDO_table);
list                           return(RDO_list);
$Sequence                      return(RDO_Sequence);
$sequence                      return(RDO_Sequence);
uniform                        return(RDO_uniform);
exponential                    return(RDO_exponential);
normal                         return(RDO_normal);
triangular                     return(RDO_triangular);
by_hist                        return(RDO_by_hist);
enumerative                    return(RDO_enumerative);

$Pattern                       return(RDO_Pattern);
$pattern                       return(RDO_Pattern);
operation                      return(RDO_operation);
event                          return(RDO_event);
rule                           return(RDO_rule);
keyboard                       return(RDO_keyboard);
$Relevant_resources            return(RDO_Relevant_resources);
$relevant_resources            return(RDO_Relevant_resources);
Keep                           return(RDO_Keep);
keep                           return(RDO_Keep);
Create                         return(RDO_Create);
create                         return(RDO_Create);
Erase                          return(RDO_Erase);
erase                          return(RDO_Erase);
NonExist                       return(RDO_NonExist);
nonexist                       return(RDO_NonExist);
NoChange                       return(RDO_NoChange);
nochange                       return(RDO_NoChange);
$Time                          return(RDO_Time);
$time                          return(RDO_Time);
Choice                         return(RDO_Choice);
choice                         return(RDO_Choice);
from                           return(RDO_from);
first                          return(RDO_first);
Convert_begin                  return(RDO_Convert_begin);
convert_begin                  return(RDO_Convert_begin);
Convert_end                    return(RDO_Convert_end);
convert_end                    return(RDO_Convert_end);
Convert_rule                   return(RDO_Convert_rule);
convert_rule                   return(RDO_Convert_rule);
Convert_event                  return(RDO_Convert_event);
convert_event                  return(RDO_Convert_event);
with_max                       return(RDO_with_max);
with_min                       return(RDO_with_min);
$Operations                    return(RDO_Operations);
$operations                    return(RDO_Operations);
$Results                       return(RDO_Results);
$results                       return(RDO_Results);

watch_par                      return(RDO_watch_par);
watch_state                    return(RDO_watch_state);
watch_quant                    return(RDO_watch_quant);
watch_value                    return(RDO_watch_value);
get_value                      return(RDO_get_value);

External_Model                 return(RDO_External_Model);
external_model                 return(RDO_External_Model);
Show_mode                      return(RDO_Show_mode);
show_mode                      return(RDO_Show_mode);
Frame_number                   return(RDO_Frame_number);
frame_number                   return(RDO_Frame_number);
Show_rate                      return(RDO_Show_rate);
show_rate                      return(RDO_Show_rate);
Run_StartTime                  return(RDO_Run_StartTime);
run_starttime                  return(RDO_Run_StartTime);
Trace_StartTime                return(RDO_Trace_StartTime);
trace_starttime                return(RDO_Trace_StartTime);
Trace_EndTime                  return(RDO_Trace_EndTime);
trace_endtime                  return(RDO_Trace_EndTime);
Terminate_if                   return(RDO_Terminate_if);
terminate_if                   return(RDO_Terminate_if);
Break_point                    return(RDO_Break_point);
break_point                    return(RDO_Break_point);
Seed                           return(RDO_Seed);
seed                           return(RDO_Seed);
NoShow                         return(RDO_NoShow);
noshow                         return(RDO_NoShow);
Monitor                        return(RDO_Monitor);
monitor                        return(RDO_Monitor);
Animation                      return(RDO_Animation);
animation                      return(RDO_Animation);

$Decision_point                return(RDO_Decision_point);
$decision_point                return(RDO_Decision_point);
search                         return(RDO_search);
$Condition                     return(RDO_Condition);
$condition                     return(RDO_Condition);
$Term_condition                return(RDO_Term_condition);
$term_condition                return(RDO_Term_condition);
$Evaluate_by                   return(RDO_Evaluate_by);
$evaluate_by                   return(RDO_Evaluate_by);
$Compare_tops                  return(RDO_Compare_tops);
$compare_tops                  return(RDO_Compare_tops);
NO                             return(RDO_NO);
no                             return(RDO_NO);
YES                            return(RDO_YES);
yes                            return(RDO_YES);
$Activities                    return(RDO_Activities);
$activities                    return(RDO_Activities);
value[\r\n\t ]+before          return(RDO_value_before);
value[\r\n\t ]+after           return(RDO_value_after);
value_before                   return(RDO_value_before);
value_after                    return(RDO_value_after);
some                           return(RDO_some);
prior                          return(RDO_prior);
cf                             return(RDO_CF);
CF                             return(RDO_CF);
$Priority                      return(RDO_Priority);
$priority                      return(RDO_Priority);
$Parent                        return(RDO_Parent);
$parent                        return(RDO_Parent);
$Multithreading                return(RDO_Multithreading);
$multithreading                return(RDO_Multithreading);

trace                          return(RDO_trace);
no_trace                       return(RDO_no_trace);
trace_stat                     return(RDO_trace_stat);
trace_tops                     return(RDO_trace_tops);
trace_all                      return(RDO_trace_all);

ProcessStart                   return(RDO_ProcessStart);
$Process                       return(RDO_Process);
$process                       return(RDO_Process);
QUEUE                          return(RDO_QUEUE);
queue                          return(RDO_QUEUE);
DEPART                         return(RDO_DEPART);
depart                         return(RDO_DEPART);
SEIZE                          return(RDO_SEIZE);
seize                          return(RDO_SEIZE);
GENERATE                       return(RDO_GENERATE);
generate                       return(RDO_GENERATE);
ADVANCE                        return(RDO_ADVANCE);
advance                        return(RDO_ADVANCE);
RELEASE                        return(RDO_RELEASE);
release                        return(RDO_RELEASE);
TERMINATE                      return(RDO_TERMINATE);
terminate                      return(RDO_TERMINATE);
ASSIGN                         return(RDO_ASSIGN);
assign                         return(RDO_ASSIGN);

"=="                           return(RDO_eq);
"<>"                           return(RDO_neq);
"!="                           return(RDO_neq);
"<="                           return(RDO_leq);
">="                           return(RDO_geq);
or                             return(RDO_or);
and                            return(RDO_and);
!                              return(RDO_not);
not                            return(RDO_not);
"+="                           return(RDO_PlusEqual);
"-="                           return(RDO_MinusEqual);
"*="                           return(RDO_MultiplyEqual);
"/="                           return(RDO_DivideEqual);
"++"                           return(RDO_IncrEqual);
"--"                           return(RDO_DecrEqual);

NoCheck                        return(RDO_NoCheck);
nocheck                        return(RDO_NoCheck);
Calculate_if                   return(RDO_Calculate_if);
calculate_if                   return(RDO_Calculate_if);
if                             return(RDO_if);
else                           return(RDO_else);
for                            return(RDO_for);
return                         return(RDO_Return);
break                          return(RDO_Break);
set                            return(RDO_set);
planning                       return(RDO_Planning);
Planning                       return(RDO_Planning);
stopping                       return(RDO_Stopping);
Stopping                       return(RDO_Stopping);
Exist                          return(RDO_Exist);
exist                          return(RDO_Exist);
Not_Exist                      return(RDO_Not_Exist);
not_exist                      return(RDO_Not_Exist);
For_All                        return(RDO_For_All);
for_all                        return(RDO_For_All);
Not_For_All                    return(RDO_Not_For_All);
not_for_all                    return(RDO_Not_For_All);
Select                         return(RDO_Select);
select                         return(RDO_Select);
Size                           return(RDO_Size);
size                           return(RDO_Size);
empty                          return(RDO_Empty);
Empty                          return(RDO_Empty);
getArray                       return(RDO_Select_Array);
get_array                      return(RDO_Select_Array);
watch_start                    return(RDO_WatchStart);
watch_stop                     return(RDO_WatchStop);

$Frame                         return(RDO_Frame);
$frame                         return(RDO_Frame);
$Back_picture                  return(RDO_Back_picture);
$back_picture                  return(RDO_Back_picture);
frm_cell                       return(RDO_frm_cell);
text                           return(RDO_text);
bitmap                         return(RDO_bitmap);
s_bmp                          return(RDO_s_bmp);
rect                           return(RDO_rect);
r_rect                         return(RDO_r_rect);
line                           return(RDO_line);
ellipse                        return(RDO_ellipse);
triang                         return(RDO_triang);
active                         return(RDO_active);
ruler                          return(RDO_ruler);
space                          return(RDO_space);
circle                         return(RDO_circle);

transparent                    return(RDO_color_transparent);
white                          return(RDO_color_white);
black                          return(RDO_color_black);
red                            return(RDO_color_red);
green                          return(RDO_color_green);
blue                           return(RDO_color_blue);
cyan                           return(RDO_color_cyan);
magenta                        return(RDO_color_magenta);
yellow                         return(RDO_color_yellow);
gray                           return(RDO_color_gray);
last                           return(RDO_color_last);

typedef                        return(RDO_typedef);
$Typedef                       return(RDO_typedef);
$typedef                       return(RDO_typedef);
enum                           return(RDO_enum);
$Enum                          return(RDO_enum);
$enum                          return(RDO_enum);

$Fuzzy                         return(RDO_Fuzzy);
$fuzzy                         return(RDO_Fuzzy);
$Term                          return(RDO_Fuzzy_Term);
$term                          return(RDO_Fuzzy_Term);

$Sprite                        return(RDO_Sprite);
$sprite                        return(RDO_Sprite);
sprite                         return(RDO_sprite_call);

(\'[^\']*\') {
	tstring n1(YYText());
	int len = n1.length();
	tstring name(rdo::locale::convertFromCLocale(tstring(n1, 1, len-2)));
	rdo::compiler::parser::LPTypeInfo pType = rdo::Factory<rdo::compiler::parser::TypeInfo>::delegate<rdo::compiler::parser::RDOType__string>(
		LEXER->m_lploc ? rdo::compiler::parser::RDOParserSrcInfo(LEXER_POS, name) : rdo::compiler::parser::RDOParserSrcInfo(name)
	);
	ASSERT(pType);
	/// @todo TypeInfo, убрать rdo::compiler::parser::RDOParserSrcInfo()
	LEXER->setvalue(PARSER->stack().push(rdo::Factory<rdo::compiler::parser::RDOValue>::create(name, pType->src_info(rdo::compiler::parser::RDOParserSrcInfo()), pType)));
	return RDO_STRING_CONST;
}

(\'[^\']*\n\r) {
	LEXER->setvalue(0);
	return RDO_STRING_CONST_BAD;
}

"/*"					BEGIN(comment_cpp);
<comment_cpp>[^*.]*		;
<comment_cpp>[^*\n\r]*	;
<comment_cpp>"*"+"/"	BEGIN(INITIAL);

"//"					BEGIN(comment_line);
<comment_line>[\n\r]+	BEGIN(INITIAL);

"*/" {
	LEXER->loc_delta_pos(YYLeng());
	PARSER->error().error(rdo::compiler::parser::RDOParserSrcInfo(), "Не найдено начало комментария");
}

[\r\n\t ]+
{}

[#`~@$%^&|:;(),=\[\]\.\*\>\<\+\-\/\\\{\}]	return(YYText()[YYLeng()-1]);

([+-]?[0-9]+"..") {
	tstring n1(YYText());
	tstring::size_type pos = n1.find_first_of("..");
	tstring yText(n1, 0, pos);
	unput('.');
	unput('.');
	LEXER->loc_delta_pos(-2);

	rdo::compiler::parser::RDOParserSrcInfo srcInfo(
		LEXER->m_lploc
			? rdo::compiler::parser::RDOParserSrcInfo(LEXER_POS, yText)
			: rdo::compiler::parser::RDOParserSrcInfo(yText)
	);

	rdo::compiler::parser::LPTypeInfo pType = rdo::Factory<rdo::compiler::parser::TypeInfo>::delegate<rdo::compiler::parser::RDOType__int>(srcInfo);
	ASSERT(pType);

	/// @todo TypeInfo, убрать rdo::compiler::parser::RDOParserSrcInfo()
	int value = 0;
	try
	{
		value = boost::lexical_cast<int>(yText);
	}
	catch (const boost::bad_lexical_cast&)
	{
		PARSER->error().error(srcInfo, boost::str(boost::format("Ошибка преобразования числа '%1%'. Возможно, число слишком большое.") % yText) );
	}
	LEXER->setvalue(PARSER->stack().push(
		rdo::Factory<rdo::compiler::parser::RDOValue>::create(
			value,
			pType->src_info(rdo::compiler::parser::RDOParserSrcInfo()),
			pType
		)
	));
	return RDO_INT_CONST;
}

([+-]?[0-9]+"."[0-9]*([Ee][+-]?[0-9]+)?) {

	tstring yText = YYText();

	rdo::compiler::parser::RDOParserSrcInfo srcInfo(
		LEXER->m_lploc
			? rdo::compiler::parser::RDOParserSrcInfo(LEXER_POS, yText)
			: rdo::compiler::parser::RDOParserSrcInfo(yText)
	);

	rdo::compiler::parser::LPTypeInfo pType = rdo::Factory<rdo::compiler::parser::TypeInfo>::delegate<rdo::compiler::parser::RDOType__real>(srcInfo);
	ASSERT(pType);

	/// @todo TypeInfo, убрать rdo::compiler::parser::RDOParserSrcInfo()
	double value = 0;
	try
	{
		value = boost::lexical_cast<double>(yText);
	}
	catch (const boost::bad_lexical_cast&)
	{
		PARSER->error().error(srcInfo, boost::str(boost::format("Ошибка преобразования числа '%1%'. Возможно, число слишком большое.") % yText) );
	}
	LEXER->setvalue(PARSER->stack().push(
		rdo::Factory<rdo::compiler::parser::RDOValue>::create(
			value,
			pType->src_info(rdo::compiler::parser::RDOParserSrcInfo()),
			pType
		)
	));

	return RDO_REAL_CONST;
}

[+-]?[0-9]+ {

	tstring yText = YYText();

	rdo::compiler::parser::RDOParserSrcInfo srcInfo(
		LEXER->m_lploc
			? rdo::compiler::parser::RDOParserSrcInfo(LEXER_POS, yText)
			: rdo::compiler::parser::RDOParserSrcInfo(yText)
	);

	rdo::compiler::parser::LPTypeInfo pType = rdo::Factory<rdo::compiler::parser::TypeInfo>::delegate<rdo::compiler::parser::RDOType__int>(srcInfo);
	ASSERT(pType);

	/// @todo TypeInfo, убрать rdo::compiler::parser::RDOParserSrcInfo()
	int value = 0;
	try
	{
		value = boost::lexical_cast<int>(yText);
	}
	catch (const boost::bad_lexical_cast&)
	{
		PARSER->error().error(srcInfo, boost::str(boost::format("Ошибка преобразования числа '%1%'. Возможно, число слишком большое.") % yText) );
	}
	LEXER->setvalue(PARSER->stack().push(
		rdo::Factory<rdo::compiler::parser::RDOValue>::create(
			value,
			pType->src_info(rdo::compiler::parser::RDOParserSrcInfo()),
			pType
		)
	));
	return RDO_INT_CONST;
}

true {
	rdo::compiler::parser::LPTypeInfo pType = rdo::Factory<rdo::compiler::parser::TypeInfo>::delegate<rdo::compiler::parser::RDOType__bool>(
		LEXER->m_lploc ? rdo::compiler::parser::RDOParserSrcInfo(LEXER_POS, YYText()) : rdo::compiler::parser::RDOParserSrcInfo(YYText())
	);
	ASSERT(pType);
	/// @todo TypeInfo, убрать rdo::compiler::parser::RDOParserSrcInfo()
	LEXER->setvalue(PARSER->stack().push(rdo::Factory<rdo::compiler::parser::RDOValue>::create(rdo::runtime::RDOValue(true), pType->src_info(rdo::compiler::parser::RDOParserSrcInfo()), pType)));
	return RDO_BOOL_CONST;
}

false {
	rdo::compiler::parser::LPTypeInfo pType = rdo::Factory<rdo::compiler::parser::TypeInfo>::delegate<rdo::compiler::parser::RDOType__bool>(
		LEXER->m_lploc ? rdo::compiler::parser::RDOParserSrcInfo(LEXER_POS, YYText()) : rdo::compiler::parser::RDOParserSrcInfo(YYText())
	);
	ASSERT(pType);
	/// @todo TypeInfo, убрать rdo::compiler::parser::RDOParserSrcInfo()
	LEXER->setvalue(PARSER->stack().push(rdo::Factory<rdo::compiler::parser::RDOValue>::create(rdo::runtime::RDOValue(false), pType->src_info(rdo::compiler::parser::RDOParserSrcInfo()), pType)));
	return RDO_BOOL_CONST;
}

([+-]?[0-9]+"."?[0-9]*([Ee][+-]?[0-9]+)?)[A-Za-z\xA8\xB8\xC0-\xFF_][A-Za-z\xA8\xB8\xC0-\xFF0-9_$]*
{
	LEXER->setvalue(NULL);
	return RDO_IDENTIF_BAD;
}

[A-Za-z\xA8\xB8\xC0-\xFF_$][A-Za-z\xA8\xB8\xC0-\xFF0-9_$]*[\r\n\t ]+NoChange[\r\n\t ]+NoChange {
	tstring n1(YYText());
	tstring::size_type pos = n1.find_first_of("\r\n\t :");
	tstring name(n1, 0, pos);
	tstring nameUtf8(rdo::locale::convertFromCLocale(name));
	RUNTIME->memory_insert(nameUtf8.length() + sizeof(tstring));
	rdo::compiler::parser::LPRDOValue pValue;
	if (LEXER->m_lploc)
	{
		YYLTYPE pos     = LEXER_POS;
		pos.m_last_line = pos.m_first_line;
		pos.m_last_pos  = pos.m_first_pos  + name.length();
		pos.m_last_seek = pos.m_first_seek + name.length();
		pValue = rdo::Factory<rdo::compiler::parser::RDOValue>::create(rdo::compiler::parser::RDOParserSrcInfo(pos, nameUtf8));
	}
	else
	{
		pValue = rdo::Factory<rdo::compiler::parser::RDOValue>::create(rdo::compiler::parser::RDOParserSrcInfo(nameUtf8));
	}
	ASSERT(pValue);
	LEXER->setvalue(PARSER->stack().push(pValue));
	return RDO_IDENTIF_NoChange_NoChange;
}

[A-Za-z\xA8\xB8\xC0-\xFF_$][A-Za-z\xA8\xB8\xC0-\xFF0-9_$]*[\r\n\t ]+NoChange {
	tstring n1(YYText());
	tstring::size_type pos = n1.find_first_of("\r\n\t :");
	tstring name(n1, 0, pos);
	tstring nameUtf8(rdo::locale::convertFromCLocale(name));
	RUNTIME->memory_insert(nameUtf8.length() + sizeof(tstring));
	rdo::compiler::parser::LPRDOValue pValue;
	if (LEXER->m_lploc)
	{
		YYLTYPE pos     = LEXER_POS;
		pos.m_last_line = pos.m_first_line;
		pos.m_last_pos  = pos.m_first_pos  + name.length();
		pos.m_last_seek = pos.m_first_seek + name.length();
		pValue = rdo::Factory<rdo::compiler::parser::RDOValue>::create(rdo::compiler::parser::RDOParserSrcInfo(pos, nameUtf8));
	}
	else
	{
		pValue = rdo::Factory<rdo::compiler::parser::RDOValue>::create(rdo::compiler::parser::RDOParserSrcInfo(nameUtf8));
	}
	ASSERT(pValue);
	LEXER->setvalue(PARSER->stack().push(pValue));
	return RDO_IDENTIF_NoChange;
}

[A-Za-z\xA8\xB8\xC0-\xFF_$][A-Za-z\xA8\xB8\xC0-\xFF0-9_$]*[\r\n\t ]*\: {
	tstring n1(YYText());
	tstring::size_type pos = n1.find_first_of("\r\n\t :");
	tstring name(n1, 0, pos);
	tstring nameUtf8(rdo::locale::convertFromCLocale(name));
	RUNTIME->memory_insert(nameUtf8.length() + sizeof(tstring));
	rdo::compiler::parser::LPRDOValue pValue;
	if (LEXER->m_lploc)
	{
		YYLTYPE pos     = LEXER_POS;
		pos.m_last_line = pos.m_first_line;
		pos.m_last_pos  = pos.m_first_pos  + name.length();
		pos.m_last_seek = pos.m_first_seek + name.length();
		pValue = rdo::Factory<rdo::compiler::parser::RDOValue>::create(rdo::compiler::parser::RDOParserSrcInfo(pos, nameUtf8));
	}
	else
	{
		pValue = rdo::Factory<rdo::compiler::parser::RDOValue>::create(rdo::compiler::parser::RDOParserSrcInfo(nameUtf8));
	}
	ASSERT(pValue);
	LEXER->setvalue(PARSER->stack().push(pValue));
	return RDO_IDENTIF_COLON;
}

[A-Za-z\xA8\xB8\xC0-\xFF_$][A-Za-z\xA8\xB8\xC0-\xFF0-9_$]* {

	tstring name(rdo::locale::convertFromCLocale(YYText()));
	RUNTIME->memory_insert(name.length() + sizeof(tstring));

	LEXER->setvalue(PARSER->stack().push(rdo::Factory<rdo::compiler::parser::RDOValue>::create(LEXER->m_lploc ? rdo::compiler::parser::RDOParserSrcInfo(LEXER_POS, name) : rdo::compiler::parser::RDOParserSrcInfo(name))));
	switch (PARSER->getFileToParse())
	{
	case rdoModelObjects::EVN:
	case rdoModelObjects::PAT:
		if (PARSER->isPattern())
		{
			rdo::compiler::parser::LPRDOPATPattern pPattern = PARSER->getLastPATPattern();
			ASSERT(pPattern);
			rdo::compiler::parser::LPRDORelevantResource pCurrRelRes = pPattern->m_pCurrRelRes;
			if (pCurrRelRes)
			{
				rdo::compiler::parser::LPRDORTPResType pResType = pCurrRelRes->getType();
				ASSERT(pResType);
				if (pResType->findRTPParam(name))
				{
					return RDO_IDENTIF;
				}
			}
			if (pPattern->findRelevantResource(name))
			{
				return RDO_IDENTIF_RELRES;
			}
		}
		break;

	default:
		break;
	}
	return RDO_IDENTIF;
}

%%
