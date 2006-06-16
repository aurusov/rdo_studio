%{
#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "rdogramma.h"
#include "rdoparser.h"

#define YY_INTERACTIVE

#define YY_USER_INIT {                      \
	rdoParse::parser->lexer_loc_init(); \
}

#define YY_USER_ACTION {                      \
	rdoParse::parser->lexer_loc_action(); \
}

%}
%option yylineno
%option c++
%x comment_rdo
%x comment_cpp
%x comment_line
%%

$Resource_type		return(Resource_type);
permanent			return(permanent_kw);
$Parameters			return(Parameters);
integer				return(integer);
real				return(real);
$End				return(End);
temporary			return(temporary);
such_as				return(such_as);
".."				return(dblpoint);

$Resources			return(Resources);
trace				return(trace_keyword);
no_trace			return(no_trace);

$Constant			return(Constant);
$Body				return(Body);
$Function			return(Function_keyword);
$Type				return(Type_keyword);
algorithmic			return(algorithmic);
table				return(table_keyword);
list				return(list_keyword);
Calculate_if		return(Calculate_if);
$Sequence			return(Sequence);
uniform				return(uniform);
exponential			return(exponential);
normal				return(normal_keyword);
by_hist				return(by_hist);
enumerative			return(enumerative);

Exist				return(Exist);
Not_Exist			return(Not_Exist);
For_All				return(For_All);
Not_For_All			return(Not_For_All);
\<\>				return(neq);
\<\=				return(leq);
\>\=				return(geq);
NoCheck				return(NoCheck);
Calculate_if		return(Calculate_if);
or					return(or_keyword);
and					return(and_keyword);


$Pattern			return(Pattern);
operation			return(operation_kw);
irregular_event		return(irregular_event);
rule				return(rule_keyword);
keyboard			return(keyboard);
$Relevant_resources	return(Relevant_resources);
Keep				return(Keep);
Create				return(Create_keyword);
Erase				return(Erase);
NonExist			return(NonExist);
NoChange			return(NoChange);
$Time				return(Time);
Choice				return(Choice);
from				return(from_keyword);
first				return(first_keyword);
Convert_begin		return(Convert_begin);
Convert_end			return(Convert_end);
Convert_rule		return(Convert_rule);
Convert_event		return(Convert_event);
with_max			return(with_max);
with_min			return(with_min);
$Operations			return(Operations);
$Results			return(Results);
watch_par			return(watch_par);
watch_state			return(watch_state);
watch_quant			return(watch_quant);
watch_value			return(watch_value);
get_value			return(get_value);

Model_name			return(Model_name);
Resource_file		return(Resource_file);
OprIev_file			return(OprIev_file);
Frame_file			return(Frame_file);	
Statistic_file		return(Statistic_file);
Results_file		return(Results_file);
Trace_file			return(Trace_file);
Show_mode			return(Show_mode);
Frame_number		return(Frame_number);
Show_rate			return(Show_rate);
Run_StartTime		return(Run_StartTime);
Trace_StartTime		return(Trace_StartTime);
Trace_EndTime		return(Trace_EndTime);
Terminate_if		return(Terminate_if);
Break_point			return(Break_point);
Seed				return(Seed);
NoShow				return(NoShow);
Monitor 			return(Monitor);
Animation			return(Animation);


$Decision_point		return(Decision_point);
search				return(search_keyword);
trace_stat			return(trace_stat);
trace_tops			return(trace_tops);
trace_all			return(trace_all);
$Condition			return(Condition_keyword);
$Term_condition		return(Term_condition);
$Evaluate_by		return(Evaluate_by);
$Compare_tops		return(Compare_tops);
NO					return(NO);
YES					return(YES);
$Activities			return(Activities);
$Process			return(Process);
SEIZE				return(SEIZE);
GENERATE			return(GENERATE);
ADVANCE				return(ADVANCE);
TERMINATE			return(TERMINATE);
value[\r\n\t ]+before	return(value_before);
value[\r\n\t ]+after	return(value_after);
value_before		return(value_before);
value_after			return(value_after);
some				return(some);

$Frame				return(Frame);
Show_if				return(Show_if);
$Back_picture		return(Back_picture);
Show				return(Show);
frm_cell			return(frm_cell);
text				return(text);
transparent			return(transparent);
bitmap				return(bitmap);
s_bmp				return(s_bmp);
rect				return(rect_keyword);
r_rect				return(r_rect);
line				return(line);
ellipse				return(ellipse);
triang				return(triang);
active				return(active);

(\'[^\']*\')		{
						std::string n1(YYText());
						int len = n1.length();
						std::string *name = new std::string(n1, 1, len-2);
						rdoParse::parser->addName(name);
						rdoParse::parser->lexer_setvalue((int)name);
						return QUOTED_IDENTIF;
					}

(\'[^\']*\n\r)		{
						std::string *name = new std::string(YYText());
						rdoParse::parser->addName(name);
						rdoParse::parser->lexer_setvalue((int)name);
						return QUOTED_IDENTIF_BAD;
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


[\r\n\t ]+						;
[()},=\[\]\.\*\>\<\+\-\/]		return(YYText()[YYLeng()-1]);

([+-]?[0-9]+"..")	{
						std::string n1(YYText(), YYLeng()-2);
						rdoParse::parser->lexer_setvalue(atoi(n1.c_str()));
						unput('.');
						unput('.');
						rdoParse::parser->lexer_loc_delta_pos( -2 );
						return INT_CONST;
					}

[+-]?[0-9]+			{
						rdoParse::parser->lexer_setvalue(atoi(YYText()));
						return INT_CONST;
					}

([+-]?[0-9]+"."?[0-9]*([Ee][+-]?[0-9]+)?)	{
						double *val = rdoParse::parser->addDouble(new double(atof(YYText())));
						rdoParse::parser->lexer_setvalue((int)val);
						return REAL_CONST;
					}

[+-]?[0-9]+"."?[0-9]?[Ee]?[+-]?[0-9]?[A-Za-zР-пр-џ\xB8_$][A-Za-zР-пр-џ\xB80-9_$]*	{
						std::string *name = new std::string(YYText());
						rdoParse::parser->addName(name);
						rdoParse::parser->lexer_setvalue((int)name);
						return IDENTIF_BAD;
					}

[A-Za-zР-пр-џ\xB8_$][A-Za-zР-пр-џ\xB80-9_$]*[\r\n\t ]+set  {
					std::string n1(YYText());
					std::string::size_type pos = n1.find_first_of("\r\n\t :");
					std::string *name = new std::string(n1, 0, pos);
					rdoParse::parser->addName(name);
					rdoParse::parser->lexer_setvalue((int)name);
					return IDENTIF_set;
				}

[A-Za-zР-пр-џ\xB8_$][A-Za-zР-пр-џ\xB80-9_$]*[\r\n\t ]+NoChange[\r\n\t ]+NoChange  {
					std::string n1(YYText());
					std::string::size_type pos = n1.find_first_of("\r\n\t :");
					std::string *name = new std::string(n1, 0, pos);
					rdoParse::parser->addName(name);
					rdoParse::parser->lexer_setvalue((int)name);
					return IDENTIF_NoChange_NoChange;
				}

[A-Za-zР-пр-џ\xB8_$][A-Za-zР-пр-џ\xB80-9_$]*[\r\n\t ]+NoChange  {
					std::string n1(YYText());
					std::string::size_type pos = n1.find_first_of("\r\n\t :");
					std::string *name = new std::string(n1, 0, pos);
					rdoParse::parser->addName(name);
					rdoParse::parser->lexer_setvalue((int)name);
					return IDENTIF_NoChange;
				}

[A-Za-zР-пр-џ\xB8_$][A-Za-zР-пр-џ\xB80-9_$]*[\r\n\t ]*\:  {
					std::string n1(YYText());
					std::string::size_type pos = n1.find_first_of("\r\n\t :");
					std::string *name = new std::string(n1, 0, pos);
					rdoParse::parser->addName(name);
					rdoParse::parser->lexer_setvalue((int)name);
					return IDENTIF_COLON;
				}

[A-Za-zР-пр-џ\xB8_$][A-Za-zР-пр-џ\xB80-9_$]*  {
					std::string *name = new std::string(YYText());
					rdoParse::parser->addName(name);
					rdoParse::parser->lexer_setvalue((int)name);
					return IDENTIF;
				}

%%
