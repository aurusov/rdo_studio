/*!
  \copyright (c) RDO-Team, 2003-2012
  \file      rdoeditorbaseedit.cpp
  \author    Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      14.03.2003
  \brief     
  \indent    4T
*/

// ---------------------------------------------------------------------------- PCH
#include "app/rdo_studio/pch/stdpch.h"
// ----------------------------------------------------------------------- INCLUDES
#include <boost/algorithm/string.hpp>
// ----------------------------------------------------------------------- SYNOPSIS
#include "app/rdo_studio/src/editor/parser_edit.h"
#include "app/rdo_studio/res/scintilla/ac_function.xpm"
#include "app/rdo_studio/res/scintilla/ac_data.xpm"
#include "app/rdo_studio/res/scintilla/ac_trace.xpm"
#include "thirdparty/scintilla/include/SciLexer.h"
// --------------------------------------------------------------------------------

using namespace rdo::gui::editor;
using namespace rdo::gui::style;

Parser::Parser(PTR(QWidget) pParent)
	: super(pParent)
	, kw0("$Activities?0 $Back_picture?0 $Body?0 $Changes?0 $Compare_tops?0 $Condition?0 \
$Constant?0 $Decision_point?0 $Default?0 $End?0 $End_picture?0 $Evaluate_by?0 $Frame?0 $Function?0 \
$Include?0 $Multithreading?0 $multithreading?0 $Operations?0 $Parameters?0 $Fuzzy_Parameters?0 Term?0 $Pattern?0 $Process?0 $Relevant_resources?0 $Resource_type?0 \
$Resources?0 $Result_values?0 $Results?0 $Sequence?0 $Status?0 $Term_condition?0 $Time?0 $Tracing?0 \
$Type?0 $Watching?0 active?0 AExpCalcCounter?0 after?0 algorithmic?0 all?0 and?0 Animation?0 array?0 before?0 \
BExpCalcCounter?0 bitmap?0 Break_point?0 by_hist?0 Choice?0 Convert_begin?0 \
Convert_end?0 Convert_event?0 Convert_rule?0 Create?0 circle?0 ellipse?0 enumerative?0 Erase?0 EventCount?0 \
exponential?0 first?0 Frame_file?0 Frame_number?0 from?0 get_value?0 Get_value?0 integer?0 \
event?0 Keep?0 keyboard?0 line?0 list?0 longint?0 Model_name?0 Monitor?0 NO?0 Planning?0 Stopping?0 \
NoChange?0 NoCheck?0 NonExist?0 normal?0 NoShow?0 operation?0 \
OperRuleCheckCounter?0 OprIev_file?0 or?0 permanent?0 r_rect?0 real?0 enum?0 rect?0 Resource_file?0 \
Results_file?0 rule?0 Run_file?0 Run_StartTime?0 s_bmp?0 search?0 Seconds?0 selected?0 \
ADVANCE?0 SEIZE?0 RELEASE?0 GENERATE?0 QUEUE?0 DEPART?0 ASSIGN?0 TERMINATE?0 \
set?0 Show_mode?0 Show_rate?0 some?0 Statistic_file?0 such_as?0 table?0 temporary?0 \
Terminate_if?0 text?0 Time_now?0 Trace_EndTime?2 Trace_file?2 Trace_StartTime?2 triang?0 triangular?0 \
uniform?0 until?0 value?0 watch_par?0 watch_quant?0 watch_state?0 watch_value?0 with_max?0 \
with_min?0 YES?0 Seed?0 Системное_время?0 not?0 if?0 else?0 for?0 return?0 break?0 result?0 ruler?0 space?0 string?0 bool?0 true?0 false?0 \
External_Model?0 external_model?0 $Priority?0 $Parent?0 prior?0 cf?0 Terminate_counter?0 typedef?0 $Typedef?0")
	, kw1("Abs?1 ArcCos?1 ArcSin?1 ArcTan?1 Cos?1 Cotan?1 Exist?1 Exp?1 Floor?1 For_All?1 Frac?1 \
GetRelResNumber?1 GetResNumber?1 IAbs?1 IMax?1 IMin?1 Int?1 IntPower?1 Ln?1 Log10?1 Log2?1 LogN?1 Max?1 Min?1 \
Not_Exist?1 Not_For_All?1 Power?1 Round?1 Sin?1 Sqrt?1 Tan?1 Select?1 Size?1 Empty?1")
	, kw2("no_trace?2 trace?2 trace_all?2 trace_stat?2 trace_tops?2")
	, kw3("transparent last white black red green blue cyan magenta yellow gray")
{
	sendEditor(SCI_SETLEXER, SCLEX_RDO);
	sendEditor(SCI_GETLEXER);

	sendEditorString(SCI_SETPROPERTY, reinterpret_cast<unsigned long>("fold"), "1");

	sendEditor(SCI_SETMARGINWIDTHN, 1, 0);

	sendEditor(SCI_SETSTYLEBITS, 5);

	sendEditorString(SCI_SETKEYWORDS, 0, Parser::convertToLexer(kw0).c_str());
	sendEditorString(SCI_SETKEYWORDS, 1, Parser::convertToLexer(kw1).c_str());
	sendEditorString(SCI_SETKEYWORDS, 2, Parser::convertToLexer(kw2).c_str());
	sendEditorString(SCI_SETKEYWORDS, 3, Parser::convertToLexer(kw3).c_str());

	static PTR(char) s_wordCharacters = "0123456789_$abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZАаБбВвГгДдЕеЁёЖжЗзИиЙйКкЛлМмНнОоПпРрСсТтУуФфХхЦцЧчШшЩщЪъЫыЬьЭэЮюЯя";

	sendEditorString(SCI_SETWORDCHARS, 0, s_wordCharacters);
	sendEditor(SCI_REGISTERIMAGE, 0, reinterpret_cast<long>(xpm_ac_function));
	sendEditor(SCI_REGISTERIMAGE, 1, reinterpret_cast<unsigned long>(xpm_ac_data));
	sendEditor(SCI_REGISTERIMAGE, 2, reinterpret_cast<unsigned long>(xpm_ac_trace));
}

Parser::~Parser()
{}

tstring Parser::getAllKW() const
{
	return rdo::format("%s %s %s %s", kw0.c_str(), kw1.c_str(), kw2.c_str(), kw3.c_str());
}

tstring Parser::convertToLexer(CREF(tstring) kw)
{
	tstring s = kw;
	while (s.find('?') != tstring::npos)
	{
		tstring::size_type pos1 = s.find('?');
		tstring::size_type pos2 = s.find(' ', pos1);
		s.erase(pos1, pos2 - pos1);
	}
	tstring str_big = boost::algorithm::to_lower_copy(s);
	return str_big + " " + s;
}

void Parser::setEditorStyle(PTR(ParserStyle) pStyle)
{
	super::setEditorStyle(pStyle);
	if (!m_pStyle)
		return;

	// ----------
	// Colors
	sendEditor(SCI_STYLESETFORE, SCE_RDO_DEFAULT, convertColor(static_cast<ModelStyle*>(m_pStyle)->defaultColor));
	sendEditor(SCI_STYLESETBACK, SCE_RDO_DEFAULT, convertColor(static_cast<ModelStyle*>(m_pStyle)->backgroundColor));
	sendEditor(SCI_STYLESETFORE, SCE_RDO_IDENTIFIER, convertColor(static_cast<ModelStyle*>(m_pStyle)->identifierColor));
	sendEditor(SCI_STYLESETBACK, SCE_RDO_IDENTIFIER, convertColor(static_cast<ModelStyle*>(m_pStyle)->backgroundColor));
	sendEditor(SCI_STYLESETFORE, SCE_RDO_KEYWORD, convertColor(static_cast<ModelStyle*>(m_pStyle)->keywordColor));
	sendEditor(SCI_STYLESETBACK, SCE_RDO_KEYWORD, convertColor(static_cast<ModelStyle*>(m_pStyle)->backgroundColor));
	sendEditor(SCI_STYLESETFORE, SCE_RDO_FUNCTION, convertColor(static_cast<ModelStyle*>(m_pStyle)->functionsColor));
	sendEditor(SCI_STYLESETBACK, SCE_RDO_FUNCTION, convertColor(static_cast<ModelStyle*>(m_pStyle)->backgroundColor));
	sendEditor(SCI_STYLESETFORE, SCE_RDO_TRACE, convertColor(static_cast<ModelStyle*>(m_pStyle)->traceColor));
	sendEditor(SCI_STYLESETBACK, SCE_RDO_TRACE, convertColor(static_cast<ModelStyle*>(m_pStyle)->backgroundColor));
	sendEditor(SCI_STYLESETFORE, SCE_RDO_FRAME_COLOR, convertColor(static_cast<ModelStyle*>(m_pStyle)->colorColor));
	sendEditor(SCI_STYLESETBACK, SCE_RDO_FRAME_COLOR, convertColor(static_cast<ModelStyle*>(m_pStyle)->backgroundColor));
	sendEditor(SCI_STYLESETFORE, SCE_RDO_COMMENT_CPP, convertColor(static_cast<ModelStyle*>(m_pStyle)->commentColor));
	sendEditor(SCI_STYLESETBACK, SCE_RDO_COMMENT_CPP, convertColor(static_cast<ModelStyle*>(m_pStyle)->backgroundColor));
	sendEditor(SCI_STYLESETFORE, SCE_RDO_COMMENT_LINE, convertColor(static_cast<ModelStyle*>(m_pStyle)->commentColor));
	sendEditor(SCI_STYLESETBACK, SCE_RDO_COMMENT_LINE, convertColor(static_cast<ModelStyle*>(m_pStyle)->backgroundColor));
	sendEditor(SCI_STYLESETFORE, SCE_RDO_NUMBER, convertColor(static_cast<ModelStyle*>(m_pStyle)->numberColor));
	sendEditor(SCI_STYLESETBACK, SCE_RDO_NUMBER, convertColor(static_cast<ModelStyle*>(m_pStyle)->backgroundColor));
	sendEditor(SCI_STYLESETFORE, SCE_RDO_STRING, convertColor(static_cast<ModelStyle*>(m_pStyle)->stringColor));
	sendEditor(SCI_STYLESETBACK, SCE_RDO_STRING, convertColor(static_cast<ModelStyle*>(m_pStyle)->backgroundColor));
	sendEditor(SCI_STYLESETFORE, SCE_RDO_OPERATOR, convertColor(static_cast<ModelStyle*>(m_pStyle)->operatorColor));
	sendEditor(SCI_STYLESETBACK, SCE_RDO_OPERATOR, convertColor(static_cast<ModelStyle*>(m_pStyle)->backgroundColor));

	// ----------
	// Styles
	sendEditor(SCI_STYLESETBOLD     , SCE_RDO_DEFAULT, static_cast<ModelStyle*>(m_pStyle)->defaultStyle & StyleFont::BOLD     );
	sendEditor(SCI_STYLESETITALIC   , SCE_RDO_DEFAULT, static_cast<ModelStyle*>(m_pStyle)->defaultStyle & StyleFont::ITALIC   );
	sendEditor(SCI_STYLESETUNDERLINE, SCE_RDO_DEFAULT, static_cast<ModelStyle*>(m_pStyle)->defaultStyle & StyleFont::UNDERLINE);
	sendEditor(SCI_STYLESETBOLD     , SCE_RDO_IDENTIFIER, static_cast<ModelStyle*>(m_pStyle)->identifierStyle & StyleFont::BOLD     );
	sendEditor(SCI_STYLESETITALIC   , SCE_RDO_IDENTIFIER, static_cast<ModelStyle*>(m_pStyle)->identifierStyle & StyleFont::ITALIC   );
	sendEditor(SCI_STYLESETUNDERLINE, SCE_RDO_IDENTIFIER, static_cast<ModelStyle*>(m_pStyle)->identifierStyle & StyleFont::UNDERLINE);
	sendEditor(SCI_STYLESETBOLD     , SCE_RDO_KEYWORD, static_cast<ModelStyle*>(m_pStyle)->keywordStyle & StyleFont::BOLD     );
	sendEditor(SCI_STYLESETITALIC   , SCE_RDO_KEYWORD, static_cast<ModelStyle*>(m_pStyle)->keywordStyle & StyleFont::ITALIC   );
	sendEditor(SCI_STYLESETUNDERLINE, SCE_RDO_KEYWORD, static_cast<ModelStyle*>(m_pStyle)->keywordStyle & StyleFont::UNDERLINE);
	sendEditor(SCI_STYLESETBOLD     , SCE_RDO_FUNCTION, static_cast<ModelStyle*>(m_pStyle)->functionsStyle & StyleFont::BOLD     );
	sendEditor(SCI_STYLESETITALIC   , SCE_RDO_FUNCTION, static_cast<ModelStyle*>(m_pStyle)->functionsStyle & StyleFont::ITALIC   );
	sendEditor(SCI_STYLESETUNDERLINE, SCE_RDO_FUNCTION, static_cast<ModelStyle*>(m_pStyle)->functionsStyle & StyleFont::UNDERLINE);
	sendEditor(SCI_STYLESETBOLD     , SCE_RDO_TRACE, static_cast<ModelStyle*>(m_pStyle)->traceStyle & StyleFont::BOLD     );
	sendEditor(SCI_STYLESETITALIC   , SCE_RDO_TRACE, static_cast<ModelStyle*>(m_pStyle)->traceStyle & StyleFont::ITALIC   );
	sendEditor(SCI_STYLESETUNDERLINE, SCE_RDO_TRACE, static_cast<ModelStyle*>(m_pStyle)->traceStyle & StyleFont::UNDERLINE);
	sendEditor(SCI_STYLESETBOLD     , SCE_RDO_FRAME_COLOR, static_cast<ModelStyle*>(m_pStyle)->colorStyle & StyleFont::BOLD     );
	sendEditor(SCI_STYLESETITALIC   , SCE_RDO_FRAME_COLOR, static_cast<ModelStyle*>(m_pStyle)->colorStyle & StyleFont::ITALIC   );
	sendEditor(SCI_STYLESETUNDERLINE, SCE_RDO_FRAME_COLOR, static_cast<ModelStyle*>(m_pStyle)->colorStyle & StyleFont::UNDERLINE);
	sendEditor(SCI_STYLESETBOLD     , SCE_RDO_COMMENT_CPP, static_cast<ModelStyle*>(m_pStyle)->commentStyle & StyleFont::BOLD     );
	sendEditor(SCI_STYLESETITALIC   , SCE_RDO_COMMENT_CPP, static_cast<ModelStyle*>(m_pStyle)->commentStyle & StyleFont::ITALIC   );
	sendEditor(SCI_STYLESETUNDERLINE, SCE_RDO_COMMENT_CPP, static_cast<ModelStyle*>(m_pStyle)->commentStyle & StyleFont::UNDERLINE);
	sendEditor(SCI_STYLESETBOLD     , SCE_RDO_COMMENT_LINE, static_cast<ModelStyle*>(m_pStyle)->commentStyle & StyleFont::BOLD     );
	sendEditor(SCI_STYLESETITALIC   , SCE_RDO_COMMENT_LINE, static_cast<ModelStyle*>(m_pStyle)->commentStyle & StyleFont::ITALIC   );
	sendEditor(SCI_STYLESETUNDERLINE, SCE_RDO_COMMENT_LINE, static_cast<ModelStyle*>(m_pStyle)->commentStyle & StyleFont::UNDERLINE);
	sendEditor(SCI_STYLESETBOLD     , SCE_RDO_NUMBER, static_cast<ModelStyle*>(m_pStyle)->numberStyle & StyleFont::BOLD     );
	sendEditor(SCI_STYLESETITALIC   , SCE_RDO_NUMBER, static_cast<ModelStyle*>(m_pStyle)->numberStyle & StyleFont::ITALIC   );
	sendEditor(SCI_STYLESETUNDERLINE, SCE_RDO_NUMBER, static_cast<ModelStyle*>(m_pStyle)->numberStyle & StyleFont::UNDERLINE);
	sendEditor(SCI_STYLESETBOLD     , SCE_RDO_STRING, static_cast<ModelStyle*>(m_pStyle)->stringStyle & StyleFont::BOLD     );
	sendEditor(SCI_STYLESETITALIC   , SCE_RDO_STRING, static_cast<ModelStyle*>(m_pStyle)->stringStyle & StyleFont::ITALIC   );
	sendEditor(SCI_STYLESETUNDERLINE, SCE_RDO_STRING, static_cast<ModelStyle*>(m_pStyle)->stringStyle & StyleFont::UNDERLINE);
	sendEditor(SCI_STYLESETBOLD     , SCE_RDO_OPERATOR, static_cast<ModelStyle*>(m_pStyle)->operatorStyle & StyleFont::BOLD     );
	sendEditor(SCI_STYLESETITALIC   , SCE_RDO_OPERATOR, static_cast<ModelStyle*>(m_pStyle)->operatorStyle & StyleFont::ITALIC   );
	sendEditor(SCI_STYLESETUNDERLINE, SCE_RDO_OPERATOR, static_cast<ModelStyle*>(m_pStyle)->operatorStyle & StyleFont::UNDERLINE);

	// ----------
	// Font Name
	sendEditorString(SCI_STYLESETFONT, SCE_RDO_DEFAULT, m_pStyle->font.name.c_str());
	sendEditorString(SCI_STYLESETFONT, SCE_RDO_IDENTIFIER, m_pStyle->font.name.c_str());
	sendEditorString(SCI_STYLESETFONT, SCE_RDO_KEYWORD, m_pStyle->font.name.c_str());
	sendEditorString(SCI_STYLESETFONT, SCE_RDO_FUNCTION, m_pStyle->font.name.c_str());
	sendEditorString(SCI_STYLESETFONT, SCE_RDO_TRACE, m_pStyle->font.name.c_str());
	sendEditorString(SCI_STYLESETFONT, SCE_RDO_FRAME_COLOR, m_pStyle->font.name.c_str());
	sendEditorString(SCI_STYLESETFONT, SCE_RDO_COMMENT_CPP, m_pStyle->font.name.c_str());
	sendEditorString(SCI_STYLESETFONT, SCE_RDO_COMMENT_LINE, m_pStyle->font.name.c_str());
	sendEditorString(SCI_STYLESETFONT, SCE_RDO_NUMBER, m_pStyle->font.name.c_str());
	sendEditorString(SCI_STYLESETFONT, SCE_RDO_STRING, m_pStyle->font.name.c_str());
	sendEditorString(SCI_STYLESETFONT, SCE_RDO_OPERATOR, m_pStyle->font.name.c_str());

	// ----------
	// Font Size
	sendEditor(SCI_STYLESETSIZE, SCE_RDO_DEFAULT, m_pStyle->font.size);
	sendEditor(SCI_STYLESETSIZE, SCE_RDO_IDENTIFIER, m_pStyle->font.size);
	sendEditor(SCI_STYLESETSIZE, SCE_RDO_KEYWORD, m_pStyle->font.size);
	sendEditor(SCI_STYLESETSIZE, SCE_RDO_FUNCTION, m_pStyle->font.size);
	sendEditor(SCI_STYLESETSIZE, SCE_RDO_TRACE, m_pStyle->font.size);
	sendEditor(SCI_STYLESETSIZE, SCE_RDO_FRAME_COLOR, m_pStyle->font.size);
	sendEditor(SCI_STYLESETSIZE, SCE_RDO_COMMENT_CPP, m_pStyle->font.size);
	sendEditor(SCI_STYLESETSIZE, SCE_RDO_COMMENT_LINE, m_pStyle->font.size);
	sendEditor(SCI_STYLESETSIZE, SCE_RDO_NUMBER, m_pStyle->font.size);
	sendEditor(SCI_STYLESETSIZE, SCE_RDO_STRING, m_pStyle->font.size);
	sendEditor(SCI_STYLESETSIZE, SCE_RDO_OPERATOR, m_pStyle->font.size);

	// ----------
	// Codepage and Characterset
	sendEditor(SCI_STYLESETCHARACTERSET, SCE_RDO_DEFAULT, m_pStyle->font.characterSet);
	sendEditor(SCI_STYLESETCHARACTERSET, SCE_RDO_IDENTIFIER, m_pStyle->font.characterSet);
	sendEditor(SCI_STYLESETCHARACTERSET, SCE_RDO_KEYWORD, m_pStyle->font.characterSet);
	sendEditor(SCI_STYLESETCHARACTERSET, SCE_RDO_FUNCTION, m_pStyle->font.characterSet);
	sendEditor(SCI_STYLESETCHARACTERSET, SCE_RDO_TRACE, m_pStyle->font.characterSet);
	sendEditor(SCI_STYLESETCHARACTERSET, SCE_RDO_FRAME_COLOR, m_pStyle->font.characterSet);
	sendEditor(SCI_STYLESETCHARACTERSET, SCE_RDO_COMMENT_CPP, m_pStyle->font.characterSet);
	sendEditor(SCI_STYLESETCHARACTERSET, SCE_RDO_COMMENT_LINE, m_pStyle->font.characterSet);
	sendEditor(SCI_STYLESETCHARACTERSET, SCE_RDO_NUMBER, m_pStyle->font.characterSet);
	sendEditor(SCI_STYLESETCHARACTERSET, SCE_RDO_STRING, m_pStyle->font.characterSet);
	sendEditor(SCI_STYLESETCHARACTERSET, SCE_RDO_OPERATOR, m_pStyle->font.characterSet);
}

void Parser::replaceCurrent(CREF(tstring) str, int changePosValue) const
{
	int pos = changePosValue != -1
		? getCurrentPos()
		: 0;

	sendEditor(SCI_REPLACESEL, 0, (long)str.c_str());

	if (changePosValue != -1)
	{
		pos += changePosValue;
		setCurrentPos(pos);
	}
}
