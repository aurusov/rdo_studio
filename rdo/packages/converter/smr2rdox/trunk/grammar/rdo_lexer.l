/*!
  \copyright (c) RDO-Team, 2011
  \file      rdo_lexer.h
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

%{
// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/grammar/rdobison.h"
#include "converter/smr2rdox/grammar/generated/rdogramma.h"
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdoparser_lexer.h"
#include "converter/smr2rdox/rdo_value.h"
#include "converter/smr2rdox/rdopat.h"
#include "converter/smr2rdox/update/update.h"

#include "simulator/runtime/rdo_runtime.h"
// --------------------------------------------------------------------------------

#ifdef LEXER
#undef LEXER
#endif
#ifdef LEXER_POS
#undef LEXER_POS
#endif

#define LEXER     reinterpret_cast<PTR(rdo::converter::smr2rdox::RDOLexer)>(this)
#define LEXER_POS (*LEXER->m_lploc)
#define CONVERTER LEXER->converter()
#define RUNTIME   CONVERTER->runtime()

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
%x comment_rdo
%x comment_cpp
%x comment_line
%%

$Resource_type		return(RDO_Resource_type);
$resource_type		return(RDO_Resource_type);
permanent			return(RDO_permanent);
$Parameters			return(RDO_Parameters);
$parameters			return(RDO_Parameters);
integer				return(RDO_integer);
longint				return(RDO_integer);
real				return(RDO_real);
string				return(RDO_string);
bool				return(RDO_bool);
$End				return(RDO_End);
$end				return(RDO_End);
temporary			return(RDO_temporary);
such_as				return(RDO_such_as);
".."				return(RDO_dblpoint);

$Resources			return(RDO_Resources);
$resources			return(RDO_Resources);
trace				return(RDO_trace);
no_trace			return(RDO_no_trace);

$Constant			return(RDO_Constant);
$constant			return(RDO_Constant);
$Body				return(RDO_Body);
$body				return(RDO_Body);
$Function			return(RDO_Function);
$function			return(RDO_Function);
$Type				return(RDO_Type);
$type				return(RDO_Type);
algorithmic			return(RDO_algorithmic);
table				return(RDO_table);
list				return(RDO_list);
Calculate_if		return(RDO_Calculate_if);
calculate_if		return(RDO_Calculate_if);
if					return(RDO_if);
$Sequence			return(RDO_Sequence);
$sequence			return(RDO_Sequence);
uniform				return(RDO_uniform);
exponential			return(RDO_exponential);
normal				return(RDO_normal);
by_hist				return(RDO_by_hist);
enumerative			return(RDO_enumerative);

Exist				return(RDO_Exist);
exist				return(RDO_Exist);
Not_Exist			return(RDO_Not_Exist);
not_exist			return(RDO_Not_Exist);
For_All				return(RDO_For_All);
for_all				return(RDO_For_All);
Not_For_All			return(RDO_Not_For_All);
not_for_all			return(RDO_Not_For_All);
\=\=				return(RDO_eq);
\<\>				return(RDO_neq);
\!\=				return(RDO_neq);
\<\=				return(RDO_leq);
\>\=				return(RDO_geq);
NoCheck				return(RDO_NoCheck);
nocheck				return(RDO_NoCheck);
or					return(RDO_or);
and					return(RDO_and);
!					return(RDO_not);
not					return(RDO_not);

$Pattern			return(RDO_Pattern);
$pattern			return(RDO_Pattern);
operation			return(RDO_operation);
irregular_event		return(RDO_irregular_event);
rule				return(RDO_rule);
keyboard			return(RDO_keyboard);
$Relevant_resources	return(RDO_Relevant_resources);
$relevant_resources	return(RDO_Relevant_resources);
Keep				return(RDO_Keep);
keep				return(RDO_Keep);
Create				return(RDO_Create);
create				return(RDO_Create);
Erase				return(RDO_Erase);
erase				return(RDO_Erase);
NonExist			return(RDO_NonExist);
nonexist			return(RDO_NonExist);
NoChange			return(RDO_NoChange);
nochange			return(RDO_NoChange);
$Time				return(RDO_Time);
$time				return(RDO_Time);
Choice				return(RDO_Choice);
choice				return(RDO_Choice);
from				return(RDO_from);
first				return(RDO_first);
Convert_begin		return(RDO_Convert_begin);
convert_begin		return(RDO_Convert_begin);
Convert_end			return(RDO_Convert_end);
convert_end			return(RDO_Convert_end);
Convert_rule		return(RDO_Convert_rule);
convert_rule		return(RDO_Convert_rule);
Convert_event		return(RDO_Convert_event);
convert_event		return(RDO_Convert_event);
with_max			return(RDO_with_max);
with_min			return(RDO_with_min);
$Operations			return(RDO_Operations);
$operations			return(RDO_Operations);
$Results			return(RDO_Results);
$results			return(RDO_Results);
watch_par			return(RDO_watch_par);
watch_state			return(RDO_watch_state);
watch_quant			return(RDO_watch_quant);
watch_value			return(RDO_watch_value);
get_value			return(RDO_get_value);

Model_name			return(RDO_Model_name);
model_name			return(RDO_Model_name);
Resource_file		return(RDO_Resource_file);
resource_file		return(RDO_Resource_file);
OprIev_file			return(RDO_OprIev_file);
opriev_file			return(RDO_OprIev_file);
Frame_file			return(RDO_Frame_file);	
frame_file			return(RDO_Frame_file);	
Statistic_file		return(RDO_Statistic_file);
statistic_file		return(RDO_Statistic_file);
Results_file		return(RDO_Results_file);
results_file		return(RDO_Results_file);
Trace_file			return(RDO_Trace_file);
trace_file			return(RDO_Trace_file);
External_Model      return(RDO_External_Model);
external_model      return(RDO_External_Model);
Show_mode			return(RDO_Show_mode);
show_mode			return(RDO_Show_mode);
Frame_number		return(RDO_Frame_number);
frame_number		return(RDO_Frame_number);
Show_rate			return(RDO_Show_rate);
show_rate			return(RDO_Show_rate);
Run_StartTime		return(RDO_Run_StartTime);
run_starttime		return(RDO_Run_StartTime);
Trace_StartTime		return(RDO_Trace_StartTime);
trace_starttime		return(RDO_Trace_StartTime);
Trace_EndTime		return(RDO_Trace_EndTime);
trace_endtime		return(RDO_Trace_EndTime);
Terminate_if		return(RDO_Terminate_if);
terminate_if		return(RDO_Terminate_if);
Break_point			return(RDO_Break_point);
break_point			return(RDO_Break_point);
Seed				return(RDO_Seed);
seed				return(RDO_Seed);
NoShow				return(RDO_NoShow);
noshow				return(RDO_NoShow);
Monitor 			return(RDO_Monitor);
monitor 			return(RDO_Monitor);
Animation			return(RDO_Animation);
animation			return(RDO_Animation);

$Decision_point		return(RDO_Decision_point);
$decision_point		return(RDO_Decision_point);
search				return(RDO_search);
trace_stat			return(RDO_trace_stat);
trace_tops			return(RDO_trace_tops);
trace_all			return(RDO_trace_all);
$Condition			return(RDO_Condition);
$condition			return(RDO_Condition);
$Term_condition		return(RDO_Term_condition);
$term_condition		return(RDO_Term_condition);
$Evaluate_by		return(RDO_Evaluate_by);
$evaluate_by		return(RDO_Evaluate_by);
$Compare_tops		return(RDO_Compare_tops);
$compare_tops		return(RDO_Compare_tops);
NO					return(RDO_NO);
no					return(RDO_NO);
YES					return(RDO_YES);
yes					return(RDO_YES);
$Activities			return(RDO_Activities);
$activities			return(RDO_Activities);
$Process			return(RDO_Process);
$process			return(RDO_Process);
QUEUE				return(RDO_QUEUE);
queue				return(RDO_QUEUE);
DEPART				return(RDO_DEPART);
depart				return(RDO_DEPART);
SEIZE				return(RDO_SEIZE);
seize				return(RDO_SEIZE);
GENERATE			return(RDO_GENERATE);
generate			return(RDO_GENERATE);
ADVANCE				return(RDO_ADVANCE);
advance				return(RDO_ADVANCE);
RELEASE				return(RDO_RELEASE);
release				return(RDO_RELEASE);
TERMINATE			return(RDO_TERMINATE);
terminate			return(RDO_TERMINATE);
ASSIGN				return(RDO_ASSIGN);
assign				return(RDO_ASSIGN);
value[\r\n\t ]+before	return(RDO_value_before);
value[\r\n\t ]+after	return(RDO_value_after);
value_before		return(RDO_value_before);
value_after			return(RDO_value_after);
some				return(RDO_some);
prior				return(RDO_prior);
cf					return(RDO_CF);
CF					return(RDO_CF);
$Priority			return(RDO_Priority);
$priority			return(RDO_Priority);
$Parent				return(RDO_Parent);
$parent				return(RDO_Parent);

$Frame				return(RDO_Frame);
$frame				return(RDO_Frame);
Show_if				return(RDO_Show_if);
show_if				return(RDO_Show_if);
$Back_picture		return(RDO_Back_picture);
$back_picture		return(RDO_Back_picture);
Show				return(RDO_Show);
show				return(RDO_Show);
frm_cell			return(RDO_frm_cell);
text				return(RDO_text);
bitmap				return(RDO_bitmap);
s_bmp				return(RDO_s_bmp);
rect				return(RDO_rect);
r_rect				return(RDO_r_rect);
line				return(RDO_line);
ellipse				return(RDO_ellipse);
triang				return(RDO_triang);
active				return(RDO_active);
ruler				return(RDO_ruler);
space				return(RDO_space);
transparent			return(RDO_color_transparent);
white				return(RDO_color_white);
black				return(RDO_color_black);
red					return(RDO_color_red);
green				return(RDO_color_green);
blue				return(RDO_color_blue);
cyan				return(RDO_color_cyan);
magenta				return(RDO_color_magenta);
yellow				return(RDO_color_yellow);
gray				return(RDO_color_gray);
last				return(RDO_color_last);

typedef				return(RDO_typedef);
$Typedef			return(RDO_typedef);
$typedef			return(RDO_typedef);
enum				return(RDO_enum);
$Enum				return(RDO_enum);
$enum				return(RDO_enum);

Select				return(RDO_Select);
select				return(RDO_Select);
Size				return(RDO_Size);
size				return(RDO_Size);
empty				return(RDO_Empty);
Empty				return(RDO_Empty);

$Fuzzy				return(RDO_Fuzzy);
$fuzzy				return(RDO_Fuzzy);
$Term				return(RDO_Fuzzy_Term);
$term				return(RDO_Fuzzy_Term);

set					return(RDO_set);

(\'[^\']*\') {
	tstring n1(YYText());
	int len = n1.length();
	tstring name(n1, 1, len-2);
	LEXER->setvalue(CONVERTER->stack().push(
		rdo::Factory<rdo::converter::smr2rdox::RDOValue>::create(
			name,
			rdo::Factory<rdo::converter::smr2rdox::RDOType__string>::create(),
			LEXER->m_lploc
				? rdo::converter::smr2rdox::RDOParserSrcInfo(LEXER_POS, name)
				: rdo::converter::smr2rdox::RDOParserSrcInfo(name)
		)
	));
	return RDO_STRING_CONST;
}

(\'[^\']*\n\r) {
	LEXER->setvalue(0);
	return RDO_STRING_CONST_BAD;
}

"{"						{
	BEGIN(comment_rdo);

	rdo::converter::smr2rdox::LPDocUpdate pReplace = rdo::Factory<rdo::converter::smr2rdox::UpdateReplace>::create(LEXER_POS.m_first_seek, LEXER_POS.m_first_seek + 1, "/*");
	ASSERT(pReplace);
	CONVERTER->insertDocUpdate(pReplace);
};

<comment_rdo>[^}.]*		;
<comment_rdo>[^}\n\r]*	;
<comment_rdo>"}"		{
	BEGIN(INITIAL);

	rdo::converter::smr2rdox::LPDocUpdate pReplace = rdo::Factory<rdo::converter::smr2rdox::UpdateReplace>::create(LEXER_POS.m_first_seek, LEXER_POS.m_first_seek + 1, "*/");
	ASSERT(pReplace);
	CONVERTER->insertDocUpdate(pReplace);
};

"/*"					BEGIN(comment_cpp);
<comment_cpp>[^*.]*		;
<comment_cpp>[^*\n\r]*	;
<comment_cpp>"*"+"/"	BEGIN(INITIAL);

"//"					BEGIN(comment_line);
<comment_line>[\n\r]+	BEGIN(INITIAL);

"}"|"*/"				{
	LEXER->loc_delta_pos(YYLeng());
	CONVERTER->error().error(rdo::converter::smr2rdox::RDOParserSrcInfo(), "Не найдено начало комментария");
}

[\r\n\t ]+
{}

[#`~@$%^&|:;(),=\[\]\.\*\>\<\+\-\/\\]	return(YYText()[YYLeng()-1]);

([+-]?[0-9]+"..") {
	tstring n1(YYText());
	tstring::size_type pos = n1.find_first_of("..");
	tstring yText(n1, 0, pos);
	unput('.');
	unput('.');
	LEXER->loc_delta_pos(-2);

	rdo::converter::smr2rdox::RDOParserSrcInfo srcInfo(
		LEXER->m_lploc
			? rdo::converter::smr2rdox::RDOParserSrcInfo(LEXER_POS, yText)
			: rdo::converter::smr2rdox::RDOParserSrcInfo(yText)
	);

	int value = 0;
	try
	{
		value = boost::lexical_cast<int>(yText);
	}
	catch (const boost::bad_lexical_cast&)
	{
		CONVERTER->error().error(srcInfo, boost::str(boost::format("Ошибка преобразования числа '%1%'. Возможно, число слишком большое.") % yText) );
	}

	LEXER->setvalue(CONVERTER->stack().push(
		rdo::Factory<rdo::converter::smr2rdox::RDOValue>::create(
			value,
			rdo::Factory<rdo::converter::smr2rdox::RDOType__int>::create(),
			srcInfo
		)
	));

	return RDO_INT_CONST;
}

([+-]?[0-9]+"."[0-9]*([Ee][+-]?[0-9]+)?) {

	tstring yText = YYText();

	rdo::converter::smr2rdox::RDOParserSrcInfo srcInfo(
		LEXER->m_lploc
			? rdo::converter::smr2rdox::RDOParserSrcInfo(LEXER_POS, yText)
			: rdo::converter::smr2rdox::RDOParserSrcInfo(yText)
	);

	double value = 0;
	try
	{
		value = boost::lexical_cast<double>(yText);
	}
	catch (const boost::bad_lexical_cast&)
	{
		CONVERTER->error().error(srcInfo, boost::str(boost::format("Ошибка преобразования числа '%1%'. Возможно, число слишком большое.") % yText) );
	}

	LEXER->setvalue(CONVERTER->stack().push(
		rdo::Factory<rdo::converter::smr2rdox::RDOValue>::create(
			value,
			rdo::Factory<rdo::converter::smr2rdox::RDOType__real>::create(),
			srcInfo
		)
	));

	return RDO_REAL_CONST;
}

[+-]?[0-9]+ {

	tstring yText = YYText();

	rdo::converter::smr2rdox::RDOParserSrcInfo srcInfo(
		LEXER->m_lploc
			? rdo::converter::smr2rdox::RDOParserSrcInfo(LEXER_POS, yText)
			: rdo::converter::smr2rdox::RDOParserSrcInfo(yText)
	);

	int value = 0;
	try
	{
		value = boost::lexical_cast<int>(yText);
	}
	catch (const boost::bad_lexical_cast&)
	{
		CONVERTER->error().error(srcInfo, boost::str(boost::format("Ошибка преобразования числа '%1%'. Возможно, число слишком большое.") % yText) );
	}

	LEXER->setvalue(CONVERTER->stack().push(
		rdo::Factory<rdo::converter::smr2rdox::RDOValue>::create(
			value,
			rdo::Factory<rdo::converter::smr2rdox::RDOType__int>::create(),
			srcInfo
		)
	));

	return RDO_INT_CONST;
}

true {

	LEXER->setvalue(CONVERTER->stack().push(
		rdo::Factory<rdo::converter::smr2rdox::RDOValue>::create(
			rdo::runtime::RDOValue(true),
			rdo::Factory<rdo::converter::smr2rdox::RDOType__bool>::create(),
			LEXER->m_lploc
				? rdo::converter::smr2rdox::RDOParserSrcInfo(LEXER_POS, YYText())
				: rdo::converter::smr2rdox::RDOParserSrcInfo(YYText())
		)
	));

	return RDO_BOOL_CONST;
}

false {

	LEXER->setvalue(CONVERTER->stack().push(
		rdo::Factory<rdo::converter::smr2rdox::RDOValue>::create(
			rdo::runtime::RDOValue(false),
			rdo::Factory<rdo::converter::smr2rdox::RDOType__bool>::create(),
			LEXER->m_lploc
				? rdo::converter::smr2rdox::RDOParserSrcInfo(LEXER_POS, YYText())
				: rdo::converter::smr2rdox::RDOParserSrcInfo(YYText())
		)
	));

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
	RUNTIME->memory_insert(name.length() + sizeof(tstring));
	rdo::converter::smr2rdox::LPRDOValue pValue;
	if (LEXER->m_lploc)
	{
		YYLTYPE pos     = LEXER_POS;
		pos.m_last_line = pos.m_first_line;
		pos.m_last_pos  = pos.m_first_pos  + name.length();
		pos.m_last_seek = pos.m_first_seek + name.length();
		pValue = rdo::Factory<rdo::converter::smr2rdox::RDOValue>::create(rdo::converter::smr2rdox::RDOParserSrcInfo(pos, name));
	}
	else
	{
		pValue = rdo::Factory<rdo::converter::smr2rdox::RDOValue>::create(rdo::converter::smr2rdox::RDOParserSrcInfo(name));
	}

	LEXER->setvalue(CONVERTER->stack().push(pValue));

	return RDO_IDENTIF_NoChange_NoChange;
}

[A-Za-z\xA8\xB8\xC0-\xFF_$][A-Za-z\xA8\xB8\xC0-\xFF0-9_$]*[\r\n\t ]+NoChange {
	tstring n1(YYText());
	tstring::size_type pos = n1.find_first_of("\r\n\t :");
	tstring name(n1, 0, pos);
	RUNTIME->memory_insert(name.length() + sizeof(tstring));
	rdo::converter::smr2rdox::LPRDOValue pValue;
	if (LEXER->m_lploc)
	{
		YYLTYPE pos     = LEXER_POS;
		pos.m_last_line = pos.m_first_line;
		pos.m_last_pos  = pos.m_first_pos  + name.length();
		pos.m_last_seek = pos.m_first_seek + name.length();
		pValue = rdo::Factory<rdo::converter::smr2rdox::RDOValue>::create(rdo::converter::smr2rdox::RDOParserSrcInfo(pos, name));
	}
	else
	{
		pValue = rdo::Factory<rdo::converter::smr2rdox::RDOValue>::create(rdo::converter::smr2rdox::RDOParserSrcInfo(name));
	}

	LEXER->setvalue(CONVERTER->stack().push(pValue));

	return RDO_IDENTIF_NoChange;
}

[A-Za-z\xA8\xB8\xC0-\xFF_$][A-Za-z\xA8\xB8\xC0-\xFF0-9_$]*[\r\n\t ]*\: {
	tstring n1(YYText());
	tstring::size_type pos = n1.find_first_of("\r\n\t :");
	tstring name(n1, 0, pos);
	RUNTIME->memory_insert(name.length() + sizeof(tstring));
	rdo::converter::smr2rdox::LPRDOValue pValue;
	if (LEXER->m_lploc)
	{
		YYLTYPE pos     = LEXER_POS;
		pos.m_last_line = pos.m_first_line;
		pos.m_last_pos  = pos.m_first_pos  + name.length();
		pos.m_last_seek = pos.m_first_seek + name.length();
		pValue = rdo::Factory<rdo::converter::smr2rdox::RDOValue>::create(rdo::converter::smr2rdox::RDOParserSrcInfo(pos, name));
	}
	else
	{
		pValue = rdo::Factory<rdo::converter::smr2rdox::RDOValue>::create(rdo::converter::smr2rdox::RDOParserSrcInfo(name));
	}

	LEXER->setvalue(CONVERTER->stack().push(pValue));

	return RDO_IDENTIF_COLON;
}

[A-Za-z\xA8\xB8\xC0-\xFF_$][A-Za-z\xA8\xB8\xC0-\xFF0-9_$]* {
	tstring name(YYText());
	RUNTIME->memory_insert(name.length() + sizeof(tstring));

	LEXER->setvalue(CONVERTER->stack().push(
		rdo::Factory<rdo::converter::smr2rdox::RDOValue>::create(
			LEXER->m_lploc
				? rdo::converter::smr2rdox::RDOParserSrcInfo(LEXER_POS, name)
				: rdo::converter::smr2rdox::RDOParserSrcInfo(name)
		)
	));

	switch (CONVERTER->getFileToParse())
	{
	case rdo::converter::smr2rdox::PAT_IN:
		if (CONVERTER->isPattern())
		{
			rdo::converter::smr2rdox::LPRDOPATPattern pPattern = CONVERTER->getLastPATPattern();
			ASSERT(pPattern);
			rdo::converter::smr2rdox::LPRDORelevantResource pCurrRelRes = pPattern->m_pCurrRelRes;
			if (pCurrRelRes)
			{
				rdo::converter::smr2rdox::LPRDORTPResType pResType = pCurrRelRes->getType();
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
