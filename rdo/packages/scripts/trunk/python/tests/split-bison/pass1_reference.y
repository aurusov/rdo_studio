/*!
  \copyright (c) RDO-Team, 2013
  \file      split-test.yx
  \author    Александровский Кирилл (k.alexandrovsky@gmail.com)
  \date      03.10.2013
  \brief     Тест для проверки правильности генерации файлов .y для двухпроходной компиляции
  \indent    4T
*/

%{
#define YYPARSE_PARAM lexer
#define YYLEX_PARAM lexer
%}

%pure-parser

// TOKENS

%token RDO_real
//    ...
%token RDO_End

%{
// ---------------------------------------------------------------------------- PCH
//    ...
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
//    ...
// --------------------------------------------------------------------------------

#define PARSER  LEXER->parser()
#define RUNTIME PARSER->runtime()

OPEN_RDO_PARSER_NAMESPACE
%}

%start test

%%

test_empty
	: /* empty */
	| test_brackets
	| test_for_one_pass_only
	| test_for_multiple_passes
	{
		// some useful comment
		some_code();
	}
	;

test_brackets
	: Token1 Token2
	{
		some_code();
	}
	| Token2 Token3 {
		some_code();
	}
	| Token3 Token4	{ some_code(); }
	| Token4 Token5	
	{
	
	}
	| Token5 Token6	{
	
	}
	| Token6 Token7	{}
	;

test_for_one_pass_only
	: Token1 Token2
	{
		/* ---------- COMPILER 1st PASS ---------- */
		
			some_code();
			{
				some_code();
			}
		/* -------------- END PASS 1 ------------- */
	}
	| Token2 Token3 {
		/* ---------- COMPILER 1st PASS ---------- */  for(int i=0; i<10; ++i){ some_code(); } /* -------------- END PASS 1 ------------- */
	}
	| Token3 Token4	{ /* ---------- COMPILER 1st PASS ---------- */ some_code(); /* -------------- END PASS 1 ------------- */ }
	| Token4 Token5	
	{
		


		
	}
	| Token5 Token6	
	{
		



		// comment
		



	}
	;
	
test_for_multiple_passes
	: Token1 Token2
	{
		/* ---------- COMPILER 1st PASS ---------- */
		
			pass_1_code();
		/* -------------- END PASS 1 ------------- */
		
		



		
		/* ---------- COMPILER 1st PASS ---------- */
		
			another_pass_1_code();
		/* -------------- END PASS 1 ------------- */

	}
	| Token2 Token3
	{
		common_code();
		{
			common_code();
			/* ---------- COMPILER 1st PASS ---------- */
			
				pass_1_code();
			/* -------------- END PASS 1 ------------- */
			



		}
	}
	;

%%

CLOSE_RDO_PARSER_NAMESPACE
