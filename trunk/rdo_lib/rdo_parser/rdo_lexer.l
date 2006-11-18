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

#define YY_USER_INIT {                  \
	rdoParse::parser->lexer_loc_init(); \
}

#define YY_USER_ACTION {                  \
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
$resource_type		return(Resource_type);
permanent			return(permanent_kw);
$Parameters			return(Parameters);
$parameters			return(Parameters);
integer				return(integer);
real				return(real);
$End				return(End);
$end				return(End);
temporary			return(temporary);
such_as				return(such_as);
".."				return(dblpoint);

$Resources			return(Resources);
$resources			return(Resources);
trace				return(trace_keyword);
no_trace			return(no_trace);

$Constant			return(Constant);
$constant			return(Constant);
$Body				return(Body);
$body				return(Body);
$Function			return(Function_keyword);
$function			return(Function_keyword);
$Type				return(Type_keyword);
$type				return(Type_keyword);
algorithmic			return(algorithmic);
table				return(table_keyword);
list				return(list_keyword);
Calculate_if		return(Calculate_if);
calculate_if		return(Calculate_if);
$Sequence			return(Sequence);
$sequence			return(Sequence);
uniform				return(uniform);
exponential			return(exponential);
normal				return(normal_keyword);
by_hist				return(by_hist);
enumerative			return(enumerative);

Exist				return(Exist);
exist				return(Exist);
Not_Exist			return(Not_Exist);
not_exist			return(Not_Exist);
For_All				return(For_All);
for_all				return(For_All);
Not_For_All			return(Not_For_All);
not_for_all			return(Not_For_All);
\<\>				return(neq);
\<\=				return(leq);
\>\=				return(geq);
NoCheck				return(NoCheck);
nocheck				return(NoCheck);
or					return(or_keyword);
and					return(and_keyword);
!					return(not_keyword);
not					return(not_keyword);

$Pattern			return(Pattern);
$pattern			return(Pattern);
operation			return(operation_kw);
irregular_event		return(irregular_event);
rule				return(rule_keyword);
keyboard			return(keyboard);
$Relevant_resources	return(Relevant_resources);
$relevant_resources	return(Relevant_resources);
Keep				return(Keep);
keep				return(Keep);
Create				return(Create_keyword);
create				return(Create_keyword);
Erase				return(Erase);
erase				return(Erase);
NonExist			return(NonExist);
nonexist			return(NonExist);
NoChange			return(NoChange);
nochange			return(NoChange);
$Time				return(Time);
$time				return(Time);
Choice				return(Choice);
choice				return(Choice);
from				return(from_keyword);
first				return(first_keyword);
Convert_begin		return(Convert_begin);
convert_begin		return(Convert_begin);
Convert_end			return(Convert_end);
convert_end			return(Convert_end);
Convert_rule		return(Convert_rule);
convert_rule		return(Convert_rule);
Convert_event		return(Convert_event);
convert_event		return(Convert_event);
with_max			return(with_max);
with_min			return(with_min);
$Operations			return(Operations);
$operations			return(Operations);
$Results			return(Results);
$results			return(Results);
watch_par			return(watch_par);
watch_state			return(watch_state);
watch_quant			return(watch_quant);
watch_value			return(watch_value);
get_value			return(get_value);

Model_name			return(Model_name);
model_name			return(Model_name);
Resource_file		return(Resource_file);
resource_file		return(Resource_file);
OprIev_file			return(OprIev_file);
opriev_file			return(OprIev_file);
Frame_file			return(Frame_file);	
frame_file			return(Frame_file);	
Statistic_file		return(Statistic_file);
statistic_file		return(Statistic_file);
Results_file		return(Results_file);
results_file		return(Results_file);
Trace_file			return(Trace_file);
trace_file			return(Trace_file);
Show_mode			return(Show_mode);
show_mode			return(Show_mode);
Frame_number		return(Frame_number);
frame_number		return(Frame_number);
Show_rate			return(Show_rate);
show_rate			return(Show_rate);
Run_StartTime		return(Run_StartTime);
run_starttime		return(Run_StartTime);
Trace_StartTime		return(Trace_StartTime);
trace_starttime		return(Trace_StartTime);
Trace_EndTime		return(Trace_EndTime);
trace_endtime		return(Trace_EndTime);
Terminate_if		return(Terminate_if);
terminate_if		return(Terminate_if);
Break_point			return(Break_point);
break_point			return(Break_point);
Seed				return(Seed);
seed				return(Seed);
NoShow				return(NoShow);
noshow				return(NoShow);
Monitor 			return(Monitor);
monitor 			return(Monitor);
Animation			return(Animation);
animation			return(Animation);

$Decision_point		return(Decision_point);
$decision_point		return(Decision_point);
search				return(search_keyword);
trace_stat			return(trace_stat);
trace_tops			return(trace_tops);
trace_all			return(trace_all);
$Condition			return(Condition_keyword);
$condition			return(Condition_keyword);
$Term_condition		return(Term_condition);
$term_condition		return(Term_condition);
$Evaluate_by		return(Evaluate_by);
$evaluate_by		return(Evaluate_by);
$Compare_tops		return(Compare_tops);
$compare_tops		return(Compare_tops);
NO					return(NO);
no					return(NO);
YES					return(YES);
yes					return(YES);
$Activities			return(Activities);
$activities			return(Activities);
$Process			return(Process);
$process			return(Process);
SEIZE				return(SEIZE);
seize				return(SEIZE);
GENERATE			return(GENERATE);
generate			return(GENERATE);
ADVANCE				return(ADVANCE);
advance				return(ADVANCE);
RELEASE				return(RELEASE);
release				return(RELEASE);
TERMINATE			return(TERMINATE);
terminate			return(TERMINATE);
value[\r\n\t ]+before	return(value_before);
value[\r\n\t ]+after	return(value_after);
value_before		return(value_before);
value_after			return(value_after);
some				return(some);

$Frame				return(Frame);
$frame				return(Frame);
Show_if				return(Show_if);
show_if				return(Show_if);
$Back_picture		return(Back_picture);
$back_picture		return(Back_picture);
Show				return(Show);
show				return(Show);
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
Select				return(Select);
select				return(Select);
size				return(Size_kw);
Size				return(Size_kw);
empty				return(Empty_kw);
Empty				return(Empty_kw);

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

"}"|"*/"			{
						rdoParse::parser->lexer_loc_delta_pos( YYLeng() );
						rdoParse::parser->error( "Íå íàéäåíî íà÷àëî êîììåíòàðèÿ" );
					}

[\r\n\t ]+						;
[:(),=\[\]\.\*\>\<\+\-\/\\]		return(YYText()[YYLeng()-1]);

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

([+-]?[0-9]+"."?[0-9]*([Ee][+-]?[0-9]+)?)[A-Za-zÀ-ßà-ÿ\xB8_][A-Za-zÀ-ßà-ÿ\xB80-9_$]*	{
						std::string *name = new std::string(YYText());
						rdoParse::parser->addName(name);
						rdoParse::parser->lexer_setvalue((int)name);
						return IDENTIF_BAD;
					}

[A-Za-zÀ-ßà-ÿ\xB8_][A-Za-zÀ-ßà-ÿ\xB80-9_$]*[\r\n\t ]+set  {
					std::string n1(YYText());
					std::string::size_type pos = n1.find_first_of("\r\n\t :");
					std::string *name = new std::string(n1, 0, pos);
					rdoParse::parser->addName(name);
					rdoParse::parser->lexer_setvalue((int)name);
					return IDENTIF_set;
				}

[A-Za-zÀ-ßà-ÿ\xB8_][A-Za-zÀ-ßà-ÿ\xB80-9_$]*[\r\n\t ]+(NoChange|nochange)[\r\n\t ]+(NoChange|nochange)  {
					std::string n1(YYText());
					std::string::size_type pos = n1.find_first_of("\r\n\t :");
					std::string *name = new std::string(n1, 0, pos);
					rdoParse::parser->addName(name);
					rdoParse::parser->lexer_setvalue((int)name);
					return IDENTIF_NoChange_NoChange;
				}

[A-Za-zÀ-ßà-ÿ\xB8_][A-Za-zÀ-ßà-ÿ\xB80-9_$]*[\r\n\t ]+(NoChange|nochange)  {
					std::string n1(YYText());
					std::string::size_type pos = n1.find_first_of("\r\n\t :");
					std::string *name = new std::string(n1, 0, pos);
					rdoParse::parser->addName(name);
					rdoParse::parser->lexer_setvalue((int)name);
					return IDENTIF_NoChange;
				}

[A-Za-zÀ-ßà-ÿ\xB8_][A-Za-zÀ-ßà-ÿ\xB80-9_$]*[\r\n\t ]*\:  {
					std::string n1(YYText());
					std::string::size_type pos = n1.find_first_of("\r\n\t :");
					std::string *name = new std::string(n1, 0, pos);
					rdoParse::parser->addName(name);
					rdoParse::parser->lexer_setvalue((int)name);
					return IDENTIF_COLON;
				}

[A-Za-zÀ-ßà-ÿ\xB8_][A-Za-zÀ-ßà-ÿ\xB80-9_$]*  {
					std::string *name = new std::string(YYText());
					rdoParse::parser->addName(name);
					rdoParse::parser->lexer_setvalue((int)name);
					return IDENTIF;
				}

%%
