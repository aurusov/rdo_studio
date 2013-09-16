/*!
  \copyright (c) RDO-Team, 2003-2013
  \file      lexer_model.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      20.02.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
// ----------------------------------------------------------------------- SYNOPSIS
#include "utils/src/locale/rdolocale.h"
#include "app/rdo_studio/src/editor/lexer/lexer_model.h"
#include "thirdparty/scintilla/include/Platform.h"
#include "thirdparty/scintilla/include/Scintilla.h"
#include "thirdparty/scintilla/include/SciLexer.h"
#include "thirdparty/scintilla/lexlib/WordList.h"
#include "thirdparty/scintilla/lexlib/LexAccessor.h"
#include "thirdparty/scintilla/lexlib/Accessor.h"
#include "thirdparty/scintilla/lexlib/StyleContext.h"
#include "thirdparty/scintilla/lexlib/CharacterSet.h"
// --------------------------------------------------------------------------------

namespace rdo { namespace gui { namespace lexer {

void SyntaxColor(unsigned int startPos, int length, int initStyle, WordList* keywordlists[], Accessor& styler)
{
	WordList& keywords  = *keywordlists[0];
	WordList& functions = *keywordlists[1];
	WordList& traces    = *keywordlists[2];
	WordList& colors    = *keywordlists[3];

	styler.StartAt(startPos);

	StyleContext sc(startPos, length, initStyle, styler);

	bool flag = sc.More();
	for (; flag; sc.Forward())
	{

		if (sc.state == SCE_RDO_OPERATOR)
		{
			sc.SetState(SCE_RDO_DEFAULT);
		}
		else if (sc.state == SCE_RDO_IDENTIFIER)
		{
			if (!isIdentifier(sc.ch))
			{
				char s[100];
				sc.GetCurrent(s, sizeof(s));
				if (keywords.InList(s))
				{
					sc.ChangeState(SCE_RDO_KEYWORD);
				}
				else if (functions.InList(s))
				{
					sc.ChangeState(SCE_RDO_FUNCTION);
				}
				else if (traces.InList(s))
				{
					sc.ChangeState(SCE_RDO_TRACE);
				}
				else if (colors.InList(s))
				{
					sc.ChangeState(SCE_RDO_FRAME_COLOR);
				}
				sc.SetState(SCE_RDO_DEFAULT);
			}
		}
		else if (sc.state == SCE_RDO_NUMBER)
		{
			if (!isDigit(sc.ch) && sc.ch != '.' && !(sc.ch == 'e' || sc.ch == 'E') && !((sc.ch == '+' || sc.ch == '-') && (sc.chPrev == 'e' || sc.chPrev == 'E')))
			{
				sc.SetState(SCE_RDO_DEFAULT);
			}
		}
		else if (sc.state == SCE_RDO_STRING)
		{
			if (sc.ch == '\'')
			{
				sc.ForwardSetState(SCE_RDO_DEFAULT);
			}
		}
		else if (sc.state == SCE_RDO_COMMENT_CPP)
		{
			if (sc.Match('*', '/'))
			{
				sc.Forward();
				sc.ForwardSetState(SCE_RDO_DEFAULT);
			}
		}
		else if (sc.state == SCE_RDO_COMMENT_LINE)
		{
			if (sc.ch == '\r' || sc.ch == '\n')
			{
				sc.ForwardSetState(SCE_RDO_DEFAULT);
			}
		}

		if (sc.state == SCE_RDO_DEFAULT)
		{
			if (sc.ch == '\'')
			{
				sc.SetState(SCE_RDO_STRING);
			}
			else if (sc.Match('/', '*'))
			{
				sc.SetState(SCE_RDO_COMMENT_CPP);
				sc.Forward();	// Eat the * so it isn't used for the end of the comment
			}
			else if (sc.Match('/', '/'))
			{
				sc.SetState(SCE_RDO_COMMENT_LINE);
			}
			else if (isDigit(sc.ch) || ((sc.ch == '-' || sc.ch == '+') && isDigit(sc.chNext)))
			{
				sc.SetState(SCE_RDO_NUMBER);
			}
			else if (isOperator(sc.ch))
			{
				sc.SetState(SCE_RDO_OPERATOR);
			}
			else if (isIdentifier(sc.ch))
				sc.SetState(SCE_RDO_IDENTIFIER);
		}

		flag = sc.More();
	}
	sc.Complete();
}

void SyntaxFold(unsigned int startPos, int length, int initStyle, WordList *[], Accessor &styler)
{
	unsigned int endPos  = startPos + length;
	int     visibleChars = 0;
	int     lineCurrent  = styler.GetLine(startPos);
	int     levelPrev    = styler.LevelAt(lineCurrent) & SC_FOLDLEVELNUMBERMASK;
	int     levelCurrent = levelPrev;
	char    chNext       = styler[startPos];
	int     styleNext    = styler.StyleAt(startPos);
	int     style        = initStyle;
	tstring str;
	for (unsigned int i = startPos; i < endPos; i++)
	{
		char ch       = chNext;
		chNext        = styler.SafeGetCharAt(i + 1);
		int stylePrev = style;
		style         = styleNext;
		styleNext     = styler.StyleAt(i + 1);
		bool atEOL    = (ch == '\r' && chNext != '\n') || (ch == '\n');
		if (style == SCE_RDO_COMMENT_CPP && styler.GetPropertyInt("CommentFold") != 0)
		{
			if (style != stylePrev)
			{
				levelCurrent++;
			}
			else if ((style != styleNext) && !atEOL)
			{
				levelCurrent--;
			}
		}
		if (style == SCE_RDO_KEYWORD)
		{
			str += ch;
		}
		else
		{
			if (str.length())
			{
				if (str == "$Pattern"  || str == "$Resource_type"  || str == "$Resources" || str == "$Operations" || str == "$Frame" || str == "$Constant" || str == "$Sequence" || str == "$Function" || str == "$Decision_point" || str == "$Process" || str == "$Results" || str == "$Sprite"
				 || str == "$pattern"  || str == "$resource_type"  || str == "$resources" || str == "$operations" || str == "$frame" || str == "$constant" || str == "$sequence" || str == "$function" || str == "$decision_point" || str == "$Process" || str == "$results" || str == "$sprite")
				{
					levelCurrent++;
				}
				else if (str == "$End")
				{
					levelCurrent--;
				}
				str = "";
			}
		}
		if (atEOL)
		{
			int lev = levelPrev;
			if ((levelCurrent > levelPrev) && (visibleChars > 0))
			{
				lev |= SC_FOLDLEVELHEADERFLAG;
			}
			if (lev != styler.LevelAt(lineCurrent))
			{
				styler.SetLevel(lineCurrent, lev);
			}
			lineCurrent++;
			levelPrev = levelCurrent;
			visibleChars = 0;
		}
		if (!isspacechar(ch))
		{
			visibleChars++;
		}
	}
	// Fill in the real level of the next line, keeping the current flags as they will be filled in later
	int flagsNext = styler.LevelAt(lineCurrent) & ~SC_FOLDLEVELNUMBERMASK;
	styler.SetLevel(lineCurrent, levelPrev | flagsNext);
}

bool isOperator(int ch)
{
	return
		ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == ':' ||
		ch == '[' || ch == ']' || ch == '(' || ch == ')' || ch == ',' ||
		ch == '<' || ch == '>' || ch == '=' || ch == ';' || ch == '!' ||
		ch == '{' || ch == '}' || ch == '.';
}

bool isIdentifier(int ch)
{
	return 
		iswalnum(ch) ||
		ch == '_' ||
		ch == '$' ||
		ch == '%';
}

bool isDigit(int ch)
{
	return iswdigit(ch) ? true : false;
}

}}} // namespace rdo::gui::lexer

LexerModule lexerRDOSyntax(SCLEX_RDO, rdo::gui::lexer::SyntaxColor, "rdo", rdo::gui::lexer::SyntaxFold);
