%{
#include "pch.h"

// #include <iostream>
// using namespace std;

#include "rdogramma.h"
#include "rdoparser.h"
#include "rdoparser_lexer.h"
#include "rdoparser_value.h"
#include <rdo_runtime.h>

#define LEXER     reinterpret_cast<rdoParse::RDOLexer*>(this)
#define PARSER    LEXER->m_parser
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
result				return(RDO_result);
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
$Priority           return(RDO_Priority);
$priority           return(RDO_Priority);

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

(\'[^\']*\')		{
						std::string n1(YYText());
						int len = n1.length();
						std::string name(n1, 1, len-2);
						LEXER->setvalue( (int)PARSER->addValue(new rdoParse::RDOValue(name, LEXER->m_lploc ? rdoParse::RDOParserSrcInfo(*LEXER->m_lploc, name) : rdoParse::RDOParserSrcInfo(name))) );
						return RDO_STRING_CONST;
					}

(\'[^\']*\n\r)		{
						LEXER->setvalue(NULL);
						return RDO_STRING_CONST_BAD;
					}

"{"						BEGIN(comment_rdo);
<comment_rdo>[^}.]*		;
<comment_rdo>[^}\n\r]*	;
<comment_rdo>"}"		BEGIN(INITIAL);

"/*"					BEGIN(comment_cpp);
<comment_cpp>[^*.]*		;
<comment_cpp>[^*\n\r]*	;
<comment_cpp>"*"+"/"	BEGIN(INITIAL);

"//"					BEGIN(comment_line);
<comment_line>[\n\r]+	BEGIN(INITIAL);

"}"|"*/"			{
						LEXER->loc_delta_pos( YYLeng() );
						PARSER->error( "Íå íàéäåíî íà÷àëî êîììåíòàðèÿ" );
					}

[\r\n\t ]+						;
[#`~@$%^&|:;(),=\[\]\.\*\>\<\+\-\/\\]	return(YYText()[YYLeng()-1]);

([+-]?[0-9]+"..")	{
						std::string n1(YYText());
						std::string::size_type pos = n1.find_first_of("..");
						std::string value(n1, 0, pos);
						unput('.');
						unput('.');
						LEXER->loc_delta_pos( -2 );
						LEXER->setvalue( (int)PARSER->addValue(new rdoParse::RDOValue(atoi(value.c_str()), LEXER->m_lploc ? rdoParse::RDOParserSrcInfo(*LEXER->m_lploc, value) : rdoParse::RDOParserSrcInfo(value))) );
						return RDO_INT_CONST;
					}

([+-]?[0-9]+"."[0-9]*([Ee][+-]?[0-9]+)?) {
						LEXER->setvalue( (int)PARSER->addValue(new rdoParse::RDOValue(atof(YYText()), LEXER->m_lploc ? rdoParse::RDOParserSrcInfo(*LEXER->m_lploc, YYText()) : rdoParse::RDOParserSrcInfo(YYText()))) );
						return RDO_REAL_CONST;
					}

[+-]?[0-9]+			{
						LEXER->setvalue( (int)PARSER->addValue(new rdoParse::RDOValue(atoi(YYText()), LEXER->m_lploc ? rdoParse::RDOParserSrcInfo(*LEXER->m_lploc, YYText()) : rdoParse::RDOParserSrcInfo(YYText()))) );
						return RDO_INT_CONST;
					}

true				{
						LEXER->setvalue( (int)PARSER->addValue(new rdoParse::RDOValue(true, LEXER->m_lploc ? rdoParse::RDOParserSrcInfo(*LEXER->m_lploc, YYText()) : rdoParse::RDOParserSrcInfo(YYText()))) );
						return RDO_BOOL_CONST;
					}

false				{
						LEXER->setvalue( (int)PARSER->addValue(new rdoParse::RDOValue(false, LEXER->m_lploc ? rdoParse::RDOParserSrcInfo(*LEXER->m_lploc, YYText()) : rdoParse::RDOParserSrcInfo(YYText()))) );
						return RDO_BOOL_CONST;
					}

([+-]?[0-9]+"."?[0-9]*([Ee][+-]?[0-9]+)?)[A-Za-zÀ-ßà-ÿ\xB8_][A-Za-zÀ-ßà-ÿ\xB80-9_$]*	{
						LEXER->setvalue(NULL);
						return RDO_IDENTIF_BAD;
					}

[A-Za-zÀ-ßà-ÿ\xB8_$][A-Za-zÀ-ßà-ÿ\xB80-9_$]*[\r\n\t ]+set[\r\n\t ]  {       
					std::string n1(YYText());
					std::string::size_type pos = n1.find_first_of("\r\n\t ");
					std::string name(n1, 0, pos);
					RUNTIME->memory_insert( name.length() + sizeof(std::string) );
					rdoParse::RDOValue* value;
					if ( LEXER->m_lploc )
					{
						YYLTYPE pos     = *LEXER->m_lploc;
						pos.last_line   = pos.first_line;
						pos.last_column = pos.first_column + name.length();
						value = new rdoParse::RDOValue(rdoParse::RDOParserSrcInfo(pos, name));
					}
					else
					{
						value = new rdoParse::RDOValue(rdoParse::RDOParserSrcInfo(name));
					}
					LEXER->setvalue( (int)PARSER->addValue(value) );
					return RDO_IDENTIF_set;
				}

[A-Za-zÀ-ßà-ÿ\xB8_$][A-Za-zÀ-ßà-ÿ\xB80-9_$]*[\r\n\t ]+NoChange[\r\n\t ]+NoChange  {       
					std::string n1(YYText());
					std::string::size_type pos = n1.find_first_of("\r\n\t :");
					std::string name(n1, 0, pos);
					RUNTIME->memory_insert( name.length() + sizeof(std::string) );
					rdoParse::RDOValue* value;
					if ( LEXER->m_lploc )
					{
						YYLTYPE pos     = *LEXER->m_lploc;
						pos.last_line   = pos.first_line;
						pos.last_column = pos.first_column + name.length();
						value = new rdoParse::RDOValue(rdoParse::RDOParserSrcInfo(pos, name));
					}
					else
					{
						value = new rdoParse::RDOValue(rdoParse::RDOParserSrcInfo(name));
					}
					LEXER->setvalue( (int)PARSER->addValue(value) );
					return RDO_IDENTIF_NoChange_NoChange;
				}

[A-Za-zÀ-ßà-ÿ\xB8_$][A-Za-zÀ-ßà-ÿ\xB80-9_$]*[\r\n\t ]+NoChange  {       
					std::string n1(YYText());
					std::string::size_type pos = n1.find_first_of("\r\n\t :");
					std::string name(n1, 0, pos);
					RUNTIME->memory_insert( name.length() + sizeof(std::string) );
					rdoParse::RDOValue* value;
					if ( LEXER->m_lploc )
					{
						YYLTYPE pos     = *LEXER->m_lploc;
						pos.last_line   = pos.first_line;
						pos.last_column = pos.first_column + name.length();
						value = new rdoParse::RDOValue(rdoParse::RDOParserSrcInfo(pos, name));
					}
					else
					{
						value = new rdoParse::RDOValue(rdoParse::RDOParserSrcInfo(name));
					}
					LEXER->setvalue( (int)PARSER->addValue(value) );
					return RDO_IDENTIF_NoChange;
				}

[A-Za-zÀ-ßà-ÿ\xB8_$][A-Za-zÀ-ßà-ÿ\xB80-9_$]*[\r\n\t ]*\:  {       
					std::string n1(YYText());
					std::string::size_type pos = n1.find_first_of("\r\n\t :");
					std::string name(n1, 0, pos);
					RUNTIME->memory_insert( name.length() + sizeof(std::string) );
					rdoParse::RDOValue* value;
					if ( LEXER->m_lploc )
					{
						YYLTYPE pos     = *LEXER->m_lploc;
						pos.last_line   = pos.first_line;
						pos.last_column = pos.first_column + name.length();
						value = new rdoParse::RDOValue(rdoParse::RDOParserSrcInfo(pos, name));
					}
					else
					{
						value = new rdoParse::RDOValue(rdoParse::RDOParserSrcInfo(name));
					}
					LEXER->setvalue( (int)PARSER->addValue(value) );
					return RDO_IDENTIF_COLON;
				}

[A-Za-zÀ-ßà-ÿ\xB8_$][A-Za-zÀ-ßà-ÿ\xB80-9_$]*  {       
					std::string name(YYText());
					RUNTIME->memory_insert( name.length() + sizeof(std::string) );
					LEXER->setvalue( (int)PARSER->addValue(new rdoParse::RDOValue(LEXER->m_lploc ? rdoParse::RDOParserSrcInfo(*LEXER->m_lploc, name) : rdoParse::RDOParserSrcInfo(name))) );
					return RDO_IDENTIF;
				}

%%
