/*!
  \copyright (c) RDO-Team, 2011
  \file      rdofrm.y
  \authors   Барс Александр
  \authors   Урусов Андрей (rdo@rk9.bmstu.ru)
  \date      
  \brief     
  \indent    4T
*/

%{
#define YYPARSE_PARAM lexer
#define YYLEX_PARAM lexer
%}

%pure-parser

%token RDO_Resource_type				257
%token RDO_permanent					258
%token RDO_Parameters					259
%token RDO_integer						260
%token RDO_real							261
%token RDO_End							262
%token RDO_temporary					263
%token RDO_IDENTIF						264
%token RDO_INT_CONST					265
%token RDO_REAL_CONST					266
%token RDO_such_as						267
%token RDO_dblpoint						268
%token RDO_Resources					269
%token RDO_trace						270
%token RDO_no_trace						271
%token RDO_IDENTIF_COLON				272
%token RDO_Constant						273
%token RDO_Body							274
%token RDO_Function						275
%token RDO_Type							276
%token RDO_algorithmic					277
%token RDO_table						278
%token RDO_list							279
%token RDO_Exist						281
%token RDO_Not_Exist					282
%token RDO_For_All						283
%token RDO_Not_For_All					284
%token RDO_neq							285
%token RDO_leq							286
%token RDO_geq							287
%token RDO_NoCheck						288
%token RDO_Calculate_if					289
%token RDO_or							290
%token RDO_and							291
%token RDO_Sequence						292
%token RDO_uniform						293
%token RDO_exponential					294
%token RDO_normal						295
%token RDO_by_hist						296
%token RDO_enumerative					297

%token RDO_Pattern						298
%token RDO_operation					299
%token RDO_irregular_event				300
%token RDO_rule							301
%token RDO_keyboard						302
%token RDO_Relevant_resources			303
%token RDO_Keep							304
%token RDO_Create						305
%token RDO_Erase						306
%token RDO_NonExist						307
%token RDO_IDENTIF_NoChange				308
%token RDO_Time							309
%token RDO_Choice						310
%token RDO_from							311
%token RDO_first						312
%token RDO_Convert_begin				313
%token RDO_Convert_end					314
%token RDO_Convert_rule					315
%token RDO_Convert_event				316
%token RDO_with_max						317
%token RDO_with_min						318
%token RDO_set							319
%token RDO_IDENTIF_NoChange_NoChange	320
%token RDO_Operations					321
	
%token RDO_Results						322
%token RDO_watch_par					323
%token RDO_watch_state					324
%token RDO_watch_quant					325
%token RDO_watch_value					326
%token RDO_get_value					327

%token RDO_Model_name					328
%token RDO_Resource_file				329
%token RDO_OprIev_file					330
%token RDO_Frame_file					331
%token RDO_Statistic_file				332
%token RDO_Results_file					333
%token RDO_Trace_file					334
%token RDO_Show_mode					335
%token RDO_Frame_number					336
%token RDO_Show_rate					337
%token RDO_Run_StartTime				338
%token RDO_Trace_StartTime				339
%token RDO_Trace_EndTime				340
%token RDO_Terminate_if					341
%token RDO_Break_point					342
%token RDO_Seed							343
%token RDO_NoShow						344
%token RDO_Monitor						345
%token RDO_Animation					346
%token RDO_NoChange						347

%token RDO_Decision_point				348
%token RDO_search						349
%token RDO_trace_stat					350
%token RDO_trace_tops					351
%token RDO_trace_all					352
%token RDO_Condition					353
%token RDO_Term_condition				354
%token RDO_Evaluate_by					355
%token RDO_Compare_tops					356
%token RDO_NO							357
%token RDO_YES							358
%token RDO_Activities					359
%token RDO_value_before					360
%token RDO_value_after					361
%token RDO_some							362
%token RDO_Process						363
%token RDO_SEIZE						364
%token RDO_GENERATE						365
%token RDO_TERMINATE					366
%token RDO_ADVANCE						367
%token RDO_RELEASE						368
%token RDO_if							369
%token RDO_CF							370
%token RDO_Priority						371
%token RDO_prior						372
%token RDO_Parent						373

%token RDO_Frame						400
%token RDO_Show_if						401
%token RDO_Back_picture					402
%token RDO_Show							403
%token RDO_frm_cell						404
%token RDO_text							405
%token RDO_bitmap						406
%token RDO_s_bmp						407
%token RDO_rect							408
%token RDO_r_rect						409
%token RDO_line							410
%token RDO_ellipse						411
%token RDO_triang						412
%token RDO_active						413
%token RDO_ruler						414
%token RDO_space						415
%token RDO_color_transparent			416
%token RDO_color_last					417
%token RDO_color_white					418
%token RDO_color_black					419
%token RDO_color_red					420
%token RDO_color_green					421
%token RDO_color_blue					422
%token RDO_color_cyan					423
%token RDO_color_magenta				424
%token RDO_color_yellow					425
%token RDO_color_gray					426

%token RDO_IDENTIF_RELRES				427
%token RDO_typedef						428
%token RDO_enum							429

%token RDO_STRING_CONST					430
%token RDO_STRING_CONST_BAD				431
%token RDO_IDENTIF_BAD					432
%token RDO_Select						433
%token RDO_Size							434
%token RDO_Empty						435
%token RDO_not							436
%token RDO_UMINUS						437
%token RDO_string						438
%token RDO_bool							439
%token RDO_BOOL_CONST					440
%token RDO_Fuzzy						441
%token RDO_Fuzzy_Term					442
%token RDO_eq							443
%token RDO_External_Model				444
%token RDO_QUEUE						445
%token RDO_DEPART						446
%token RDO_ASSIGN						447


%{
// ---------------------------------------------------------------------------- PCH
#include "converter/smr2rdox/pch.h"
// ----------------------------------------------------------------------- INCLUDES
// ----------------------------------------------------------------------- SYNOPSIS
#include "converter/smr2rdox/rdoparser.h"
#include "converter/smr2rdox/rdoparser_lexer.h"
#include "converter/smr2rdox/rdortp.h"
#include "converter/smr2rdox/rdofun.h"
#include "converter/smr2rdox/rdofrm.h"
#include "converter/smr2rdox/rdopat.h"
#include "converter/smr2rdox/rdoopr.h"
#include "converter/smr2rdox/rdodpt.h"
#include "converter/smr2rdox/rdo_type_range.h"
#include "converter/smr2rdox/update/update.h"
#include "simulator/runtime/calc/calc_base.h"
#include "utils/rdoanimation.h"
// --------------------------------------------------------------------------------

#define CONVERTER LEXER->converter()
#define RUNTIME   CONVERTER->runtime()

#define P_RDOVALUE(A) reinterpret_cast<PTR(RDOValue)>(A)
#define RDOVALUE(A)   (*P_RDOVALUE(A))

OPEN_RDO_CONVERTER_NAMESPACE

typedef rdoRuntime::RDOFRMFrame::RDOFRMColor      RDOFRMColor;
typedef rdoRuntime::RDOFRMFrame::LPRDOFRMColor    LPRDOFRMColor;
typedef rdoRuntime::RDOFRMFrame::RDOFRMPosition   RDOFRMPosition;
typedef rdoRuntime::RDOFRMFrame::LPRDOFRMPosition LPRDOFRMPosition;
typedef rdoRuntime::RDOFRMFrame::RDOFRMRulet      RDOFRMRulet;
typedef rdoRuntime::RDOFRMFrame::LPRDOFRMRulet    LPRDOFRMRulet;
typedef rdoRuntime::RDOFRMText                    RDOFRMText;
typedef rdoRuntime::LPRDOFRMText                  LPRDOFRMText;
typedef rdoRuntime::RDOFRMBitmap                  RDOFRMBitmap;
typedef rdoRuntime::LPRDOFRMBitmap                LPRDOFRMBitmap;
typedef rdoRuntime::RDOFRMBitmapStretch           RDOFRMBitmapStretch;
typedef rdoRuntime::LPRDOFRMBitmapStretch         LPRDOFRMBitmapStretch;
typedef rdoRuntime::RDOFRMRect                    RDOFRMRect;
typedef rdoRuntime::LPRDOFRMRect                  LPRDOFRMRect;
typedef rdoRuntime::RDOFRMRectRound               RDOFRMRectRound;
typedef rdoRuntime::LPRDOFRMRectRound             LPRDOFRMRectRound;
typedef rdoRuntime::RDOFRMEllipse                 RDOFRMEllipse;
typedef rdoRuntime::LPRDOFRMEllipse               LPRDOFRMEllipse;
typedef rdoRuntime::RDOFRMLine                    RDOFRMLine;
typedef rdoRuntime::LPRDOFRMLine                  LPRDOFRMLine;
typedef rdoRuntime::RDOFRMTriang                  RDOFRMTriang;
typedef rdoRuntime::LPRDOFRMTriang                LPRDOFRMTriang;
typedef rdoRuntime::RDOFRMActive                  RDOFRMActive;
typedef rdoRuntime::LPRDOFRMActive                LPRDOFRMActive;
typedef rdoRuntime::RDOFRMSpace                   RDOFRMSpace;
typedef rdoRuntime::LPRDOFRMSpace                 LPRDOFRMSpace;

%}

%left RDO_or
%left RDO_and
%left '+' '-'
%left '*' '/'
%left RDO_not
%left RDO_UMINUS

%start frm_main

%%

// --------------------------------------------------------------------------------
// -------------------- Основной цикл
// --------------------------------------------------------------------------------
frm_main
	: /* empty */
	| frm_main frm_end
	| error
	{
		CONVERTER->error().error(@1, _T("Неизвестная ошибка"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Фрейм
// --------------------------------------------------------------------------------
frm_begin
	: RDO_Frame RDO_IDENTIF
	{
		LPRDOFRMFrame pFrame = rdo::Factory<RDOFRMFrame>::create(P_RDOVALUE($2)->src_info());
		ASSERT(pFrame);
		$$ = CONVERTER->stack().push(pFrame);
	}
	| RDO_Frame RDO_IDENTIF RDO_Show_if fun_logic
	{
		LPRDOFRMFrame pFrame = rdo::Factory<RDOFRMFrame>::create(P_RDOVALUE($2)->src_info(), CONVERTER->stack().pop<RDOFUNLogic>($4));
		ASSERT(pFrame);
		$$ = CONVERTER->stack().push(pFrame);
	}
	| RDO_Frame RDO_IDENTIF RDO_Show_if error
	{
		CONVERTER->error().error(@4, _T("Ошибка в логическом выражении"))
	}
	;

frm_background
	: frm_begin RDO_Back_picture '=' frm_color
	{
		LPRDOFRMFrame pFrame = CONVERTER->stack().pop<RDOFRMFrame>($1);
		ASSERT(pFrame);
		LPRDOFRMColor pBgColor = CONVERTER->stack().pop<RDOFRMColor>($4);
		ASSERT(pBgColor);
		if (pBgColor->getType() != RDOFRMColor::CT_TRANSPARENT && pBgColor->getType() != RDOFRMColor::CT_RGB)
		{
			CONVERTER->error().error(@4, _T("Цвет фона не может быть указан ссылкой на последнее значение"));
		}
		pFrame->frame()->setBackgroundColor(pBgColor);
		$$ = CONVERTER->stack().push(pFrame);
	}
	| frm_begin RDO_Back_picture '='
	{
		LPRDOFRMFrame pFrame = CONVERTER->stack().pop<RDOFRMFrame>($1);
		ASSERT(pFrame);
		$$ = CONVERTER->stack().push(pFrame);
	}
	| frm_begin RDO_Back_picture error
	{
		CONVERTER->error().error(@3, _T("После ключевого слова $Back_picture ожидается знак равенства"));
	}
	| frm_begin error
	{
		CONVERTER->error().error(@2, _T("После имени кадра ожидается ключевое слово $Back_picture"));
	}
	;

frm_backpicture
	: frm_background RDO_IDENTIF
	{
		LPRDOFRMFrame pFrame = CONVERTER->stack().pop<RDOFRMFrame>($1);
		ASSERT(pFrame);
		pFrame->frame()->setBackPicture(P_RDOVALUE($2)->value().getIdentificator());
		$$ = CONVERTER->stack().push(pFrame);
	}
	| frm_background RDO_INT_CONST RDO_INT_CONST
	{
		LPRDOFRMFrame pFrame = CONVERTER->stack().pop<RDOFRMFrame>($1);
		ASSERT(pFrame);
		pFrame->frame()->setBackPicture(P_RDOVALUE($2)->value().getInt(), P_RDOVALUE($3)->value().getInt());
		$$ = CONVERTER->stack().push(pFrame);
	}
	| frm_background RDO_INT_CONST RDO_INT_CONST error
	{
		CONVERTER->error().error(@4, _T("Описание заголовка кадра окончено, ожидается ключевое слово $Show"));
	}
	| frm_background RDO_IDENTIF error
	{
		CONVERTER->error().error(@3, _T("Описание заголовка кадра окончено, ожидается ключевое слово $Show"));
	}
	| frm_background RDO_INT_CONST error
	{
		CONVERTER->error().error(@2, _T("После ширины кадра необходимо указать его высоту"));
	}
	| frm_background
	{
		CONVERTER->error().error(@1, _T("Необходимо указать имя фоновой картинки или размер кадра"));
	}
	;

frm_show
	: RDO_Show
	{
		LPRDOFRMFrame pFrame = CONVERTER->getLastFRMFrame();
		ASSERT(pFrame);
		pFrame->frame()->startShow();
	}
	| RDO_Show_if fun_logic
	{
		LPRDOFRMFrame pFrame = CONVERTER->getLastFRMFrame();
		pFrame->frame()->startShow(CONVERTER->stack().pop<RDOFUNLogic>($2)->getCalc());
	}
	| RDO_Show_if error
	{
		CONVERTER->error().error(@2, _T("Ошибка в логическом выражении"))
	}
	;

frm_item
	: /* empty */
	| frm_item frm_show
	| frm_item frm_text    {CONVERTER->getLastFRMFrame()->frame()->addItem (CONVERTER->stack().pop<RDOFRMText         >($2));}
	| frm_item frm_bitmap  {CONVERTER->getLastFRMFrame()->frame()->addItem (CONVERTER->stack().pop<RDOFRMBitmap       >($2));}
	| frm_item frm_rect    {CONVERTER->getLastFRMFrame()->frame()->addItem (CONVERTER->stack().pop<RDOFRMRect         >($2));}
	| frm_item frm_line    {CONVERTER->getLastFRMFrame()->frame()->addItem (CONVERTER->stack().pop<RDOFRMLine         >($2));}
	| frm_item frm_ellipse {CONVERTER->getLastFRMFrame()->frame()->addItem (CONVERTER->stack().pop<RDOFRMEllipse      >($2));}
	| frm_item frm_r_rect  {CONVERTER->getLastFRMFrame()->frame()->addItem (CONVERTER->stack().pop<RDOFRMRectRound    >($2));}
	| frm_item frm_triang  {CONVERTER->getLastFRMFrame()->frame()->addItem (CONVERTER->stack().pop<RDOFRMTriang       >($2));}
	| frm_item frm_s_bmp   {CONVERTER->getLastFRMFrame()->frame()->addItem (CONVERTER->stack().pop<RDOFRMBitmapStretch>($2));}
	| frm_item frm_active  {CONVERTER->getLastFRMFrame()->frame()->addItem (CONVERTER->stack().pop<RDOFRMActive       >($2));}
	| frm_item frm_ruler   {CONVERTER->getLastFRMFrame()->frame()->addRulet(CONVERTER->stack().pop<RDOFRMRulet        >($2));}
	| frm_item frm_space   {CONVERTER->getLastFRMFrame()->frame()->addItem (CONVERTER->stack().pop<RDOFRMSpace        >($2));}
	;

frm_header
	: frm_backpicture frm_item
	;

frm_end
	: frm_header RDO_End
	;

// --------------------------------------------------------------------------------
// -------------------- Элементы
// --------------------------------------------------------------------------------
frm_color
	: RDO_color_transparent
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(RDOFRMColor::CT_TRANSPARENT);
		ASSERT(pColor);
		$$ = CONVERTER->stack().push(pColor);
	}
	| RDO_color_last
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create();
		ASSERT(pColor);
		$$ = CONVERTER->stack().push(pColor);
	}
	| RDO_color_white
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(rbyte(255), rbyte(255), rbyte(255), RDOParserSrcInfo(@1, LEXER->YYText()));
		ASSERT(pColor);
		$$ = CONVERTER->stack().push(pColor);
	}
	| RDO_color_black
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(rbyte(0), rbyte(0), rbyte(0), RDOParserSrcInfo(@1, LEXER->YYText()));
		ASSERT(pColor);
		$$ = CONVERTER->stack().push(pColor);
	}
	| RDO_color_red
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(rbyte(255), rbyte(0), rbyte(0), RDOParserSrcInfo(@1, LEXER->YYText()));
		ASSERT(pColor);
		$$ = CONVERTER->stack().push(pColor);
	}
	| RDO_color_green
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(rbyte(0), rbyte(255), rbyte(0), RDOParserSrcInfo(@1, LEXER->YYText()));
		ASSERT(pColor);
		$$ = CONVERTER->stack().push(pColor);
	}
	| RDO_color_blue
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(rbyte(0), rbyte(0), rbyte(255), RDOParserSrcInfo(@1, LEXER->YYText()));
		ASSERT(pColor);
		$$ = CONVERTER->stack().push(pColor);
	}
	| RDO_color_cyan
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(rbyte(0), rbyte(255), rbyte(255), RDOParserSrcInfo(@1, LEXER->YYText()));
		ASSERT(pColor);
		$$ = CONVERTER->stack().push(pColor);
	}
	| RDO_color_magenta
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(rbyte(255), rbyte(0), rbyte(255), RDOParserSrcInfo(@1, LEXER->YYText()));
		ASSERT(pColor);
		$$ = CONVERTER->stack().push(pColor);
	}
	| RDO_color_yellow
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(rbyte(255), rbyte(255), rbyte(0), RDOParserSrcInfo(@1, LEXER->YYText()));
		ASSERT(pColor);
		$$ = CONVERTER->stack().push(pColor);
	}
	| RDO_color_gray
	{
		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(rbyte(127), rbyte(127), rbyte(127), RDOParserSrcInfo(@1, LEXER->YYText()));
		ASSERT(pColor);
		$$ = CONVERTER->stack().push(pColor);
	}
	| '<' RDO_INT_CONST RDO_INT_CONST RDO_INT_CONST '>'
	{
		LPRDOFUNArithm pRed   = rdo::Factory<RDOFUNArithm>::create(RDOVALUE($2));
		LPRDOFUNArithm pGreen = rdo::Factory<RDOFUNArithm>::create(RDOVALUE($3));
		LPRDOFUNArithm pBlue  = rdo::Factory<RDOFUNArithm>::create(RDOVALUE($4));
		LPRDOTypeRangeRange pRange    = rdo::Factory<RDOTypeRangeRange>::create(RDOValue(0), RDOValue(255), RDOParserSrcInfo());
		LPRDOTypeIntRange   pIntRange = rdo::Factory<RDOTypeIntRange>::create(pRange);
		LPRDOTypeParam      pType     = rdo::Factory<RDOTypeParam>::create(pIntRange, RDOParserSrcInfo());
		pRed->checkParamType  (pType);
		pGreen->checkParamType(pType);
		pBlue->checkParamType (pType);

		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(pRed->createCalc(), pGreen->createCalc(), pBlue->createCalc());
		ASSERT(pColor);
		$$ = CONVERTER->stack().push(pColor);
	}
	| '<' RDO_INT_CONST RDO_INT_CONST RDO_INT_CONST error
	{
		CONVERTER->error().error(@4, _T("Найдены все составляющие цвета, ожидается '>'"));
	}
	| '<' RDO_INT_CONST RDO_INT_CONST error
	{
		CONVERTER->error().error(@3, @4, _T("Ожидается синяя составляющая цвета"));
	}
	| '<' RDO_INT_CONST error
	{
		CONVERTER->error().error(@2, @3, _T("Ожидается зеленая составляющая цвета"));
	}
	| '<' fun_arithm ',' fun_arithm ',' fun_arithm '>'
	{
		LPRDOFUNArithm pRed   = CONVERTER->stack().pop<RDOFUNArithm>($2);
		LPRDOFUNArithm pGreen = CONVERTER->stack().pop<RDOFUNArithm>($4);
		LPRDOFUNArithm pBlue  = CONVERTER->stack().pop<RDOFUNArithm>($6);
		LPRDOTypeRangeRange pRange    = rdo::Factory<RDOTypeRangeRange>::create(RDOValue(0), RDOValue(255), RDOParserSrcInfo());
		LPRDOTypeIntRange   pIntRange = rdo::Factory<RDOTypeIntRange>::create(pRange);
		LPRDOTypeParam      pType     = rdo::Factory<RDOTypeParam>::create(pIntRange, RDOParserSrcInfo());
		pRed->checkParamType  (pType);
		pGreen->checkParamType(pType);
		pBlue->checkParamType (pType);

		LPRDOFRMColor pColor = rdo::Factory<RDOFRMColor>::create(pRed->createCalc(), pGreen->createCalc(), pBlue->createCalc());
		ASSERT(pColor);
		$$ = CONVERTER->stack().push(pColor);
	}
	| '<' fun_arithm ',' fun_arithm ',' fun_arithm error
	{
		CONVERTER->error().error(@6, _T("Найдены все составляющие цвета, ожидается '>'"));
	}
	| '<' fun_arithm ',' fun_arithm ',' error
	{
		CONVERTER->error().error(@5, @6, _T("Ожидается синяя составляющая цвета"));
	}
	| '<' fun_arithm ',' fun_arithm error
	{
		CONVERTER->error().error(@4, _T("После зеленой составляющей цвета ожидается запятая"));
	}
	| '<' fun_arithm ',' error
	{
		CONVERTER->error().error(@3, @4, _T("Ожидается зеленая составляющая цвета"));
	}
	| '<' fun_arithm error
	{
		CONVERTER->error().error(@2, _T("После красной составляющей цвета ожидается запятая"));
	}
	| '<' error
	{
		CONVERTER->error().error(@1, _T("После '<' ожидается красная составляющая цвета"));
	}
	;

frm_postype
	: /* empty */
	{
		$$ = RDOFRMPosition::PT_ABSOLUTE;
	}
	| '+'
	{
		$$ = RDOFRMPosition::PT_DELTA;
	}
	| '*'
	{
		$$ = RDOFRMPosition::PT_MULT;
	}
	| '#' RDO_INT_CONST
	{
		int rilet_id = P_RDOVALUE($2)->value().getInt();
		if (rilet_id <= 0)
		{
			CONVERTER->error().error(@2, _T("Номер рулетки должен быть больше нуля"));
		}
		if (!RUNTIME->lastFrame()->findRulet(rilet_id))
		{
			CONVERTER->error().error(@2, rdo::format(_T("Рулетки с номером '%d' не существует"), rilet_id));
		}
		$$ = RDOFRMPosition::PT_RULET + rilet_id;
	}
	| '#' error
	{
		CONVERTER->error().error(@1, _T("После '#' ожидается целочисленный номер рулетки"));
	}
	;

frm_postype_xy
	: frm_postype
	| '='
	{
		$$ = RDOFRMPosition::PT_GABARIT;
	}
	;

frm_postype_wh
	: frm_postype;
	| '='
	{
		CONVERTER->error().error(@1, _T("Нельзя использовать данное выравнивание для ширины или высоты"));
	}
	;

frm_position_xy
	: fun_arithm frm_postype_xy
	{
		rdoRuntime::LPRDOCalc pCalc = CONVERTER->stack().pop<RDOFUNArithm>($1)->createCalc();
		if ($2 >= RDOFRMPosition::PT_RULET)
		{
			LPRDOFRMPosition pPosition = rdo::Factory<RDOFRMPosition>::create(pCalc, RDOFRMPosition::PT_RULET, $2 - RDOFRMPosition::PT_RULET);
			ASSERT(pPosition);
			$$ = CONVERTER->stack().push(pPosition);
		}
		else
		{
			LPRDOFRMPosition pPosition = rdo::Factory<RDOFRMPosition>::create(pCalc, (RDOFRMPosition::PositionType)$2);
			ASSERT(pPosition);
			$$ = CONVERTER->stack().push(pPosition);
		}
	}
	;

frm_position_wh
	: fun_arithm frm_postype_wh
	{
		rdoRuntime::LPRDOCalc pCalc = CONVERTER->stack().pop<RDOFUNArithm>($1)->createCalc();
		if ($2 >= RDOFRMPosition::PT_RULET)
		{
			LPRDOFRMPosition pPosition = rdo::Factory<RDOFRMPosition>::create(pCalc, RDOFRMPosition::PT_RULET, $2 - RDOFRMPosition::PT_RULET);
			ASSERT(pPosition);
			$$ = CONVERTER->stack().push(pPosition);
		}
		else
		{
			LPRDOFRMPosition pPosition = rdo::Factory<RDOFRMPosition>::create(pCalc, (RDOFRMPosition::PositionType)$2);
			ASSERT(pPosition);
			$$ = CONVERTER->stack().push(pPosition);
		}
	}
	;

frm_ruler
	: RDO_ruler '[' RDO_INT_CONST ',' frm_position_xy ',' frm_position_xy ']'
	{
		LPRDOFRMRulet pRulet = RUNTIME->lastFrame()->findRulet(P_RDOVALUE($3)->value().getInt());
		if (pRulet)
		{
			CONVERTER->error().push_only(@3, rdo::format(_T("Рулетка с номером '%d' уже существует"), P_RDOVALUE($3)->value().getInt()));
			CONVERTER->error().push_only(pRulet->src_info(), _T("См. первое определение"));
			CONVERTER->error().push_done();
		}
		LPRDOFRMPosition pX = CONVERTER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pY = CONVERTER->stack().pop<RDOFRMPosition>($7);
		ASSERT(pX);
		ASSERT(pY);
		if (pX->getType() != RDOFRMPosition::PT_ABSOLUTE)
		{
			CONVERTER->error().error(@5, _T("Координаты рулетки должны быть абсолютными"));
		}
		if (pY->getType() != RDOFRMPosition::PT_ABSOLUTE)
		{
			CONVERTER->error().error(@7, _T("Координаты рулетки должны быть абсолютными"));
		}
		pRulet = rdo::Factory<RDOFRMRulet>::create(RDOParserSrcInfo(@1), P_RDOVALUE($3)->value().getInt(), pX, pY);
		ASSERT(pRulet);
		$$ = CONVERTER->stack().push(pRulet);
	}
	| RDO_ruler '[' RDO_INT_CONST ',' frm_position_xy ',' frm_position_xy error
	{
		CONVERTER->error().error(@7, _T("Ожидается ']'"));
	}
	| RDO_ruler '[' RDO_INT_CONST ',' frm_position_xy ',' error
	{
		CONVERTER->error().error(@6, @7, _T("Ожидается координата по оси Y"));
	}
	| RDO_ruler '[' RDO_INT_CONST ',' frm_position_xy error
	{
		CONVERTER->error().error(@5, _T("Ожидается запятая"));
	}
	| RDO_ruler '[' RDO_INT_CONST ',' error
	{
		CONVERTER->error().error(@4, @5, _T("Ожидается координата по оси X"));
	}
	| RDO_ruler '[' RDO_INT_CONST error
	{
		CONVERTER->error().error(@3, _T("Ожидается запятая"));
	}
	| RDO_ruler '[' error
	{
		CONVERTER->error().error(@2, @3, _T("Ожидается номер рулетки"));
	}
	| RDO_ruler error
	{
		CONVERTER->error().error(@1, _T("Ожидается '['"));
	}
	;

frm_space
	: RDO_space '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ']'
	{
		LPRDOFRMPosition pX      = CONVERTER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = CONVERTER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = CONVERTER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = CONVERTER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMSpace pSpace = rdo::Factory<RDOFRMSpace>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight);
		ASSERT(pSpace);
		$$ = CONVERTER->stack().push(pSpace);
	}
	| RDO_space '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error
	{
		CONVERTER->error().error(@9, _T("Ожидается ']'"));
	}
	| RDO_space '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error
	{
		CONVERTER->error().error(@8, @9, _T("Ожидается высота"));
	}
	| RDO_space '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error
	{
		CONVERTER->error().error(@7, _T("Ожидается запятая"));
	}
	| RDO_space '[' frm_position_xy ',' frm_position_xy ',' error
	{
		CONVERTER->error().error(@6, @7, _T("Ожидается ширина"));
	}
	| RDO_space '[' frm_position_xy ',' frm_position_xy error
	{
		CONVERTER->error().error(@5, _T("Ожидается запятая"));
	}
	| RDO_space '[' frm_position_xy ',' error
	{
		CONVERTER->error().error(@4, @5, _T("Ожидается координата по оси Y"));
	}
	| RDO_space '[' frm_position_xy error
	{
		CONVERTER->error().error(@3, _T("Ожидается запятая"));
	}
	| RDO_space '[' error
	{
		CONVERTER->error().error(@2, @3, _T("Ожидается координата по оси X"));
	}
	| RDO_space error
	{
		CONVERTER->error().error(@1, _T("Ожидается '['"));
	}
	;

frm_text_align
	: /* empty */ {$$ = rdoAnimation::RDOTextElement::TETA_LEFT;  }
	| '<'         {$$ = rdoAnimation::RDOTextElement::TETA_LEFT;  }
	| '='         {$$ = rdoAnimation::RDOTextElement::TETA_CENTER;}
	| '>'         {$$ = rdoAnimation::RDOTextElement::TETA_RIGHT; }
	;

frm_text_common
	: RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color ','
	{
		LPRDOFRMPosition pX      = CONVERTER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = CONVERTER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = CONVERTER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = CONVERTER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pBgColor = CONVERTER->stack().pop<RDOFRMColor>($11);
		LPRDOFRMColor pFgColor = CONVERTER->stack().pop<RDOFRMColor>($13);
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG_TEXT);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG_TEXT);
		LPRDOFRMText pText = rdo::Factory<RDOFRMText>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pBgColor, pFgColor);
		ASSERT(pText);
		$$ = CONVERTER->stack().push(pText);
	}
/*
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ','
	{
		LPRDOFRMPosition pX      = CONVERTER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = CONVERTER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = CONVERTER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = CONVERTER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pBgColor = rdp::Factory<RDOFRMColor>::create();
		LPRDOFRMColor pFgColor = CONVERTER->stack().pop<RDOFRMColor>($11);
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG_TEXT);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG_TEXT);
		LPRDOFRMText pText = rdo::Factory<RDOFRMText>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pBgColor, pFgColor);
		ASSERT(pText);
		$$ = CONVERTER->stack().push(pText);
	}
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ','
	{
		LPRDOFRMPosition pX      = CONVERTER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = CONVERTER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = CONVERTER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = CONVERTER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pBgColor = rdp::Factory<RDOFRMColor>::create();
		LPRDOFRMColor pFgColor = rdp::Factory<RDOFRMColor>::create();
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG_TEXT);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG_TEXT);
		LPRDOFRMText pText = rdo::Factory<RDOFRMText>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pBgColor, pFgColor);
		ASSERT(pText);
		$$ = CONVERTER->stack().push(pText);
	}
*/
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color error
	{
		CONVERTER->error().error(@13, _T("Ожидается запятая"));
	}
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' error
	{
		CONVERTER->error().error(@12, @13, _T("Ожидается цвет текста"));
	}
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color error
	{
		CONVERTER->error().error(@11, _T("Ожидается запятая"));
	}
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' error
	{
		CONVERTER->error().error(@10, @11, _T("Ожидается цвет фона"));
	}
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error
	{
		CONVERTER->error().error(@9, _T("Ожидается запятая"));
	}
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error
	{
		CONVERTER->error().error(@8, @9, _T("Ожидается высота"));
	}
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error
	{
		CONVERTER->error().error(@7, _T("Ожидается запятая"));
	}
	| RDO_text '[' frm_position_xy ',' frm_position_xy ',' error
	{
		CONVERTER->error().error(@6, @7, _T("Ожидается ширина"));
	}
	| RDO_text '[' frm_position_xy ',' frm_position_xy error
	{
		CONVERTER->error().error(@5, _T("Ожидается запятая"));
	}
	| RDO_text '[' frm_position_xy ',' error
	{
		CONVERTER->error().error(@4, @5, _T("Ожидается координата по оси Y"));
	}
	| RDO_text '[' frm_position_xy error
	{
		CONVERTER->error().error(@3, _T("Ожидается запятая"));
	}
	| RDO_text '[' error
	{
		CONVERTER->error().error(@2, @3, _T("Ожидается координата по оси X"));
	}
	| RDO_text error
	{
		CONVERTER->error().error(@1, _T("Ожидается '['"));
	}
	;

frm_text
	: frm_text_common frm_text_align fun_arithm ']'
	{
		LPRDOFRMText pText = CONVERTER->stack().pop<RDOFRMText>($1);
		ASSERT(pText);
		pText->setText((rdoAnimation::RDOTextElement::TextAlign)$2, CONVERTER->stack().pop<RDOFUNArithm>($3)->createCalc());
		$$ = CONVERTER->stack().push(pText);
	}
	| frm_text_common frm_text_align RDO_STRING_CONST ']'
	{
		LPRDOFRMText pText = CONVERTER->stack().pop<RDOFRMText>($1);
		ASSERT(pText);
		pText->setText((rdoAnimation::RDOTextElement::TextAlign)$2, P_RDOVALUE($3)->value().getString());
		$$ = CONVERTER->stack().push(pText);
	}
	| frm_text_common frm_text_align fun_arithm error
	{
		CONVERTER->error().error(@3, _T("Ожидается ']'"));
	}
	| frm_text_common frm_text_align RDO_STRING_CONST error
	{
		CONVERTER->error().error(@3, _T("Ожидается ']'"));
	}
	| frm_text_common frm_text_align error
	{
		CONVERTER->error().error(@2, _T("Ожидается параметр ресурса или строка"));
	}
	;

frm_bitmap
	: RDO_bitmap '[' frm_position_xy ',' frm_position_xy ',' RDO_IDENTIF ']'
	{
		LPRDOFRMPosition pX = CONVERTER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY = CONVERTER->stack().pop<RDOFRMPosition>($5);
		ASSERT(pX);
		ASSERT(pY);
		LPRDOFRMBitmap pBitmap = rdo::Factory<RDOFRMBitmap>::create(RUNTIME->lastFrame(), pX, pY, P_RDOVALUE($7)->value().getIdentificator());
		ASSERT(pBitmap);
		$$ = CONVERTER->stack().push(pBitmap);
	}
	| RDO_bitmap '[' frm_position_xy ',' frm_position_xy ',' RDO_IDENTIF ',' RDO_IDENTIF ']'
	{
		LPRDOFRMPosition pX = CONVERTER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY = CONVERTER->stack().pop<RDOFRMPosition>($5);
		ASSERT(pX);
		ASSERT(pY);
		LPRDOFRMBitmap pBitmap = rdo::Factory<RDOFRMBitmap>::create(RUNTIME->lastFrame(), pX, pY, P_RDOVALUE($7)->value().getIdentificator(), P_RDOVALUE($9)->value().getIdentificator());
		ASSERT(pBitmap);
		$$ = CONVERTER->stack().push(pBitmap);
	}
	| RDO_bitmap '[' frm_position_xy ',' frm_position_xy ',' RDO_IDENTIF ',' RDO_IDENTIF error
	{
		CONVERTER->error().error(@9, _T("Ожидается ']'"));
	}
	| RDO_bitmap '[' frm_position_xy ',' frm_position_xy ',' RDO_IDENTIF ',' error
	{
		CONVERTER->error().error(@8, @9, _T("Ожидается имя маски для картинки"));
	}
	| RDO_bitmap '[' frm_position_xy ',' frm_position_xy ',' RDO_IDENTIF error
	{
		CONVERTER->error().error(@7, _T("Ожидается ']'"));
	}
	| RDO_bitmap '[' frm_position_xy ',' frm_position_xy ',' error
	{
		CONVERTER->error().error(@6, @7, _T("Ожидается имя картинки"));
	}
	| RDO_bitmap '[' frm_position_xy ',' frm_position_xy error
	{
		CONVERTER->error().error(@5, _T("Ожидается запятая"));
	}
	| RDO_bitmap '[' frm_position_xy ',' error
	{
		CONVERTER->error().error(@4, @5, _T("Ожидается координата по оси Y"));
	}
	| RDO_bitmap '[' frm_position_xy error
	{
		CONVERTER->error().error(@3, _T("Ожидается запятая"));
	}
	| RDO_bitmap '[' error
	{
		CONVERTER->error().error(@2, @3, _T("Ожидается координата по оси X"));
	}
	| RDO_bitmap error
	{
		CONVERTER->error().error(@1, _T("Ожидается '['"));
	}
	;

frm_s_bmp
	: RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' RDO_IDENTIF ']'
	{
		LPRDOFRMPosition pX      = CONVERTER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = CONVERTER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = CONVERTER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = CONVERTER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMBitmapStretch pBitmap = rdo::Factory<RDOFRMBitmapStretch>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, P_RDOVALUE($11)->value().getIdentificator());
		ASSERT(pBitmap);
		$$ = CONVERTER->stack().push(pBitmap);
	}
	| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' RDO_IDENTIF ',' RDO_IDENTIF ']'
	{
		LPRDOFRMPosition pX      = CONVERTER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = CONVERTER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = CONVERTER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = CONVERTER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMBitmapStretch pBitmap = rdo::Factory<RDOFRMBitmapStretch>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, P_RDOVALUE($11)->value().getIdentificator(), P_RDOVALUE($13)->value().getIdentificator());
		ASSERT(pBitmap);
		$$ = CONVERTER->stack().push(pBitmap);
	}
	| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' RDO_IDENTIF ',' RDO_IDENTIF error
	{
		CONVERTER->error().error(@13, _T("Ожидается ']'"));
	}
	| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' RDO_IDENTIF ',' error
	{
		CONVERTER->error().error(@12, @13, _T("Ожидается имя маски для картинки"));
	}
	| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' RDO_IDENTIF error
	{
		CONVERTER->error().error(@11, _T("Ожидается ']'"));
	}
	| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' error
	{
		CONVERTER->error().error(@10, @11, _T("Ожидается имя картинки"));
	}
	| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error
	{
		CONVERTER->error().error(@9, _T("Ожидается запятая"));
	}
	| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error
	{
		CONVERTER->error().error(@8, @9, _T("Ожидается высота"));
	}
	| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error
	{
		CONVERTER->error().error(@7, _T("Ожидается запятая"));
	}
	| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' error
	{
		CONVERTER->error().error(@6, @7, _T("Ожидается ширина"));
	}
	| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy error
	{
		CONVERTER->error().error(@5, _T("Ожидается запятая"));
	}
	| RDO_s_bmp '[' frm_position_xy ',' error
	{
		CONVERTER->error().error(@4, @5, _T("Ожидается координата по оси Y"));
	}
	| RDO_s_bmp '[' frm_position_xy error
	{
		CONVERTER->error().error(@3, _T("Ожидается запятая"));
	}
	| RDO_s_bmp '[' error
	{
		CONVERTER->error().error(@2, @3, _T("Ожидается координата по оси X"));
	}
	| RDO_s_bmp error
	{
		CONVERTER->error().error(@1, _T("Ожидается '['"));
	}
	;

frm_rect
	: RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color ']'
	{
		LPRDOFRMPosition pX      = CONVERTER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = CONVERTER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = CONVERTER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = CONVERTER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pBgColor = CONVERTER->stack().pop<RDOFRMColor>($11);
		LPRDOFRMColor pFgColor = CONVERTER->stack().pop<RDOFRMColor>($13);
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG);
		LPRDOFRMRect pRect = rdo::Factory<RDOFRMRect>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pBgColor, pFgColor);
		ASSERT(pRect);
		$$ = CONVERTER->stack().push(pRect);
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ']'
	{
		LPRDOFRMPosition pX      = CONVERTER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = CONVERTER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = CONVERTER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = CONVERTER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pBgColor = CONVERTER->stack().pop<RDOFRMColor>($11);
		LPRDOFRMColor pFgColor = rdo::Factory<RDOFRMColor>::create();
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG);
		LPRDOFRMRect pRect = rdo::Factory<RDOFRMRect>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pBgColor, pFgColor);
		ASSERT(pRect);
		$$ = CONVERTER->stack().push(pRect);
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ']'
	{
		LPRDOFRMPosition pX      = CONVERTER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = CONVERTER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = CONVERTER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = CONVERTER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pBgColor = rdo::Factory<RDOFRMColor>::create();
		LPRDOFRMColor pFgColor = rdo::Factory<RDOFRMColor>::create();
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG);
		LPRDOFRMRect pRect = rdo::Factory<RDOFRMRect>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pBgColor, pFgColor);
		ASSERT(pRect);
		$$ = CONVERTER->stack().push(pRect);
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ']'
	{
		LPRDOFRMPosition pX      = CONVERTER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = CONVERTER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = CONVERTER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = rdo::Factory<RDOFRMPosition>::create(rdo::Factory<rdoRuntime::RDOCalcConst>::create(0), RDOFRMPosition::PT_DELTA);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pBgColor = rdo::Factory<RDOFRMColor>::create();
		LPRDOFRMColor pFgColor = rdo::Factory<RDOFRMColor>::create();
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG);
		LPRDOFRMRect pRect = rdo::Factory<RDOFRMRect>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pBgColor, pFgColor);
		ASSERT(pRect);
		$$ = CONVERTER->stack().push(pRect);
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ']'
	{
		LPRDOFRMPosition pX      = CONVERTER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = CONVERTER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = rdo::Factory<RDOFRMPosition>::create(rdo::Factory<rdoRuntime::RDOCalcConst>::create(0), RDOFRMPosition::PT_DELTA);
		LPRDOFRMPosition pHeight = rdo::Factory<RDOFRMPosition>::create(rdo::Factory<rdoRuntime::RDOCalcConst>::create(0), RDOFRMPosition::PT_DELTA);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pBgColor = rdo::Factory<RDOFRMColor>::create();
		LPRDOFRMColor pFgColor = rdo::Factory<RDOFRMColor>::create();
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG);
		LPRDOFRMRect pRect = rdo::Factory<RDOFRMRect>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pBgColor, pFgColor);
		ASSERT(pRect);
		$$ = CONVERTER->stack().push(pRect);
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color error
	{
		CONVERTER->error().error(@13, _T("Ожидается ']'"));
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' error
	{
		CONVERTER->error().error(@12, @13, _T("Ожидается цвет линии прямоугольника"));
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color error
	{
		CONVERTER->error().error(@11, _T("Ожидается запятая"));
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' error
	{
		CONVERTER->error().error(@10, @11, _T("Ожидается цвет фона"));
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error
	{
		CONVERTER->error().error(@9, _T("Ожидается запятая"));
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error
	{
		CONVERTER->error().error(@8, @9, _T("Ожидается высота"));
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error
	{
		CONVERTER->error().error(@7, _T("Ожидается запятая"));
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' error
	{
		CONVERTER->error().error(@6, @7, _T("Ожидается ширина"));
	}
	| RDO_rect '[' frm_position_xy ',' frm_position_xy error
	{
		CONVERTER->error().error(@5, _T("Ожидается запятая"));
	}
	| RDO_rect '[' frm_position_xy ',' error
	{
		CONVERTER->error().error(@4, @5, _T("Ожидается координата по оси Y"));
	}
	| RDO_rect '[' frm_position_xy error
	{
		CONVERTER->error().error(@3, _T("Ожидается запятая"));
	}
	| RDO_rect '[' error
	{
		CONVERTER->error().error(@2, @3, _T("Ожидается координата по оси X"));
	}
	| RDO_rect error
	{
		CONVERTER->error().error(@1, _T("Ожидается '['"));
	}
	;

frm_r_rect
	: RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color ']'
	{
		LPRDOFRMPosition pX      = CONVERTER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = CONVERTER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = CONVERTER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = CONVERTER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pBgColor = CONVERTER->stack().pop<RDOFRMColor>($11);
		LPRDOFRMColor pFgColor = CONVERTER->stack().pop<RDOFRMColor>($13);
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG);
		LPRDOFRMRectRound pRoundRect = rdo::Factory<RDOFRMRectRound>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pBgColor, pFgColor);
		ASSERT(pRoundRect);
		$$ = CONVERTER->stack().push(pRoundRect);
	}
	| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color error
	{
		CONVERTER->error().error(@13, _T("Ожидается ']'"));
	}
	| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' error
	{
		CONVERTER->error().error(@12, @13, _T("Ожидается цвет линии прямоугольника"));
	}
	| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color error
	{
		CONVERTER->error().error(@11, _T("Ожидается запятая"));
	}
	| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' error
	{
		CONVERTER->error().error(@10, @11, _T("Ожидается цвет фона"));
	}
	| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error
	{
		CONVERTER->error().error(@9, _T("Ожидается запятая"));
	}
	| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error
	{
		CONVERTER->error().error(@8, @9, _T("Ожидается высота"));
	}
	| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error
	{
		CONVERTER->error().error(@7, _T("Ожидается запятая"));
	}
	| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' error
	{
		CONVERTER->error().error(@6, @7, _T("Ожидается ширина"));
	}
	| RDO_r_rect '[' frm_position_xy ',' frm_position_xy error
	{
		CONVERTER->error().error(@5, _T("Ожидается запятая"));
	}
	| RDO_r_rect '[' frm_position_xy ',' error
	{
		CONVERTER->error().error(@4, @5, _T("Ожидается координата по оси Y"));
	}
	| RDO_r_rect '[' frm_position_xy error
	{
		CONVERTER->error().error(@3, _T("Ожидается запятая"));
	}
	| RDO_r_rect '[' error
	{
		CONVERTER->error().error(@2, @3, _T("Ожидается координата по оси X"));
	}
	| RDO_r_rect error
	{
		CONVERTER->error().error(@1, _T("Ожидается '['"));
	}
	;

frm_ellipse
	: RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color ']'
	{
		LPRDOFRMPosition pX      = CONVERTER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = CONVERTER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = CONVERTER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = CONVERTER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pBgColor = CONVERTER->stack().pop<RDOFRMColor>($11);
		LPRDOFRMColor pFgColor = CONVERTER->stack().pop<RDOFRMColor>($13);
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG);
		LPRDOFRMEllipse pEllipse = rdo::Factory<RDOFRMEllipse>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pBgColor, pFgColor);
		ASSERT(pEllipse);
		$$ = CONVERTER->stack().push(pEllipse);
	}
	| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color error
	{
		CONVERTER->error().error(@13, _T("Ожидается ']'"));
	}
	| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' error
	{
		CONVERTER->error().error(@12, @13, _T("Ожидается цвет линии эллипса"));
	}
	| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color error
	{
		CONVERTER->error().error(@11, _T("Ожидается запятая"));
	}
	| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' error
	{
		CONVERTER->error().error(@10, @11, _T("Ожидается цвет фона"));
	}
	| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error
	{
		CONVERTER->error().error(@9, _T("Ожидается запятая"));
	}
	| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error
	{
		CONVERTER->error().error(@8, @9, _T("Ожидается высота"));
	}
	| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error
	{
		CONVERTER->error().error(@7, _T("Ожидается запятая"));
	}
	| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' error
	{
		CONVERTER->error().error(@6, @7, _T("Ожидается ширина"));
	}
	| RDO_ellipse '[' frm_position_xy ',' frm_position_xy error
	{
		CONVERTER->error().error(@5, _T("Ожидается запятая"));
	}
	| RDO_ellipse '[' frm_position_xy ',' error
	{
		CONVERTER->error().error(@4, @5, _T("Ожидается координата по оси Y"));
	}
	| RDO_ellipse '[' frm_position_xy error
	{
		CONVERTER->error().error(@3, _T("Ожидается запятая"));
	}
	| RDO_ellipse '[' error
	{
		CONVERTER->error().error(@2, @3, _T("Ожидается координата по оси X"));
	}
	| RDO_ellipse error
	{
		CONVERTER->error().error(@1, _T("Ожидается '['"));
	}
	;

frm_line
	: RDO_line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color ']'
	{
		LPRDOFRMPosition pX      = CONVERTER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY      = CONVERTER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pWidth  = CONVERTER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pHeight = CONVERTER->stack().pop<RDOFRMPosition>($9);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMColor pColor = CONVERTER->stack().pop<RDOFRMColor>($11);
		ASSERT(pColor);
		pColor->setType(RDOFRMColor::CT_LAST_FG);
		LPRDOFRMLine pLine = rdo::Factory<RDOFRMLine>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, pColor);
		ASSERT(pLine);
		$$ = CONVERTER->stack().push(pLine);
	}
	| RDO_line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color error
	{
		CONVERTER->error().error(@11, _T("Ожидается ']'"));
	}
	| RDO_line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error
	{
		CONVERTER->error().error(@10, @11, _T("Ожидается цвет линии"));
	}
	| RDO_line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy error
	{
		CONVERTER->error().error(@9, _T("Ожидается запятая"));
	}
	| RDO_line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error
	{
		CONVERTER->error().error(@8, @9, _T("Ожидается Y2"));
	}
	| RDO_line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy error
	{
		CONVERTER->error().error(@7, _T("Ожидается запятая"));
	}
	| RDO_line '[' frm_position_xy ',' frm_position_xy ',' error
	{
		CONVERTER->error().error(@6, @7, _T("Ожидается X2"));
	}
	| RDO_line '[' frm_position_xy ',' frm_position_xy error
	{
		CONVERTER->error().error(@5, _T("Ожидается запятая"));
	}
	| RDO_line '[' frm_position_xy ',' error
	{
		CONVERTER->error().error(@4, @5, _T("Ожидается Y1"));
	}
	| RDO_line '[' frm_position_xy error
	{
		CONVERTER->error().error(@3, _T("Ожидается запятая"));
	}
	| RDO_line '[' error
	{
		CONVERTER->error().error(@2, @3, _T("Ожидается X1"));
	}
	| RDO_line error
	{
		CONVERTER->error().error(@1, _T("Ожидается '['"));
	}
	;

frm_triang
	: RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color ',' frm_color ']'
	{
		LPRDOFRMPosition pX1 = CONVERTER->stack().pop<RDOFRMPosition>($3);
		LPRDOFRMPosition pY1 = CONVERTER->stack().pop<RDOFRMPosition>($5);
		LPRDOFRMPosition pX2 = CONVERTER->stack().pop<RDOFRMPosition>($7);
		LPRDOFRMPosition pY2 = CONVERTER->stack().pop<RDOFRMPosition>($9);
		LPRDOFRMPosition pX3 = CONVERTER->stack().pop<RDOFRMPosition>($11);
		LPRDOFRMPosition pY3 = CONVERTER->stack().pop<RDOFRMPosition>($13);
		ASSERT(pX1);
		ASSERT(pY1);
		ASSERT(pX2);
		ASSERT(pY2);
		ASSERT(pX3);
		ASSERT(pY3);
		LPRDOFRMColor pBgColor = CONVERTER->stack().pop<RDOFRMColor>($15);
		LPRDOFRMColor pFgColor = CONVERTER->stack().pop<RDOFRMColor>($17);
		ASSERT(pBgColor);
		ASSERT(pFgColor);
		pBgColor->setType(RDOFRMColor::CT_LAST_BG);
		pFgColor->setType(RDOFRMColor::CT_LAST_FG);
		LPRDOFRMTriang pTriang = rdo::Factory<RDOFRMTriang>::create(RUNTIME->lastFrame(), pX1, pY1, pX2, pY2, pX3, pY3, pBgColor, pFgColor);
		ASSERT(pTriang);
		$$ = CONVERTER->stack().push(pTriang);
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color ',' frm_color error
	{
		CONVERTER->error().error(@17, _T("Ожидается ']'"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color ',' error
	{
		CONVERTER->error().error(@16, @17, _T("Ожидается цвет линии"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color error
	{
		CONVERTER->error().error(@15, _T("Ожидается запятая"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error
	{
		CONVERTER->error().error(@14, @15, _T("Ожидается цвет треугольника"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy error
	{
		CONVERTER->error().error(@13, _T("Ожидается запятая"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error
	{
		CONVERTER->error().error(@12, @13, _T("Ожидается Y3"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy error
	{
		CONVERTER->error().error(@11, _T("Ожидается запятая"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error
	{
		CONVERTER->error().error(@10, @11, _T("Ожидается X3"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy error
	{
		CONVERTER->error().error(@9, _T("Ожидается запятая"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error
	{
		CONVERTER->error().error(@8, @9, _T("Ожидается Y2"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy error
	{
		CONVERTER->error().error(@7, _T("Ожидается запятая"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' error
	{
		CONVERTER->error().error(@6, @7, _T("Ожидается X2"));
	}
	| RDO_triang '[' frm_position_xy ',' frm_position_xy error
	{
		CONVERTER->error().error(@5, _T("Ожидается запятая"));
	}
	| RDO_triang '[' frm_position_xy ',' error
	{
		CONVERTER->error().error(@4, @5, _T("Ожидается Y1"));
	}
	| RDO_triang '[' frm_position_xy error
	{
		CONVERTER->error().error(@3, _T("Ожидается запятая"));
	}
	| RDO_triang '[' error
	{
		CONVERTER->error().error(@2, @3, _T("Ожидается X1"));
	}
	| RDO_triang error
	{
		CONVERTER->error().error(@1, _T("Ожидается '['"));
	}
	;

frm_active
	: RDO_active RDO_IDENTIF '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ']'
	{
		tstring oprName = P_RDOVALUE($2)->value().getIdentificator();
		LPRDOOPROperation pOperation = CONVERTER->findOPROperation(oprName);
		if (!pOperation)
		{
			LPRDODPTFreeActivity pActivity = CONVERTER->findDPTFreeActivity(oprName);
			if (!pActivity)
			{
				CONVERTER->error().error(@2, rdo::format(_T("Опереация '%s' не найдена"), oprName.c_str()));
			}
			else
			{
				if (pActivity->pattern()->getType() != RDOPATPattern::PT_Keyboard)
				{
					CONVERTER->error().push_only(@2, rdo::format(_T("Активность '%s' должна быть клавиатурной"), pActivity->name().c_str()));
					CONVERTER->error().push_only(pActivity->src_info(), _T("См. акивность"));
					CONVERTER->error().push_only(pActivity->pattern()->src_info(), _T("См. образец"));
					CONVERTER->error().push_done();
				}
			}
		}
		else
		{
			if (pOperation->pattern()->getType() != RDOPATPattern::PT_Keyboard)
			{
				CONVERTER->error().push_only(@2, rdo::format(_T("Операция '%s' должна быть клавиатурной"), pOperation->name().c_str()));
				CONVERTER->error().push_only(pOperation->src_info(), _T("См. операцию"));
				CONVERTER->error().push_only(pOperation->pattern()->src_info(), _T("См. образец"));
				CONVERTER->error().push_done();
			}
		}
		LPRDOFRMPosition pX      = CONVERTER->stack().pop<RDOFRMPosition>($4);
		LPRDOFRMPosition pY      = CONVERTER->stack().pop<RDOFRMPosition>($6);
		LPRDOFRMPosition pWidth  = CONVERTER->stack().pop<RDOFRMPosition>($8);
		LPRDOFRMPosition pHeight = CONVERTER->stack().pop<RDOFRMPosition>($10);
		ASSERT(pX     );
		ASSERT(pY     );
		ASSERT(pWidth );
		ASSERT(pHeight);
		LPRDOFRMActive pActive = rdo::Factory<RDOFRMActive>::create(RUNTIME->lastFrame(), pX, pY, pWidth, pHeight, oprName);
		ASSERT(pActive);
		$$ = CONVERTER->stack().push(pActive);
	}
	| RDO_active RDO_IDENTIF '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error
	{
		CONVERTER->error().error(@10, _T("Ожидается ']'"));
	}
	| RDO_active RDO_IDENTIF '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error
	{
		CONVERTER->error().error(@9, @10, _T("Ожидается высота"));
	}
	| RDO_active RDO_IDENTIF '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error
	{
		CONVERTER->error().error(@8, _T("Ожидается запятая"));
	}
	| RDO_active RDO_IDENTIF '[' frm_position_xy ',' frm_position_xy ',' error
	{
		CONVERTER->error().error(@7, @8, _T("Ожидается ширина"));
	}
	| RDO_active RDO_IDENTIF '[' frm_position_xy ',' frm_position_xy error
	{
		CONVERTER->error().error(@6, _T("Ожидается запятая"));
	}
	| RDO_active RDO_IDENTIF '[' frm_position_xy ',' error
	{
		CONVERTER->error().error(@5, @6, _T("Ожидается координата по оси Y"));
	}
	| RDO_active RDO_IDENTIF '[' frm_position_xy error
	{
		CONVERTER->error().error(@4, _T("Ожидается запятая"));
	}
	| RDO_active RDO_IDENTIF '[' error
	{
		CONVERTER->error().error(@3, @4, _T("Ожидается координата по оси X"));
	}
	| RDO_active RDO_IDENTIF error
	{
		CONVERTER->error().error(@2, _T("Ожидается '['"));
	}
	| RDO_active error
	{
		CONVERTER->error().error(@1, _T("Ожидается имя клавиатурной операции"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Общие составные токены для всех объектов РДО
// --------------------------------------------------------------------------------
// -------------------- Логические выражения
// --------------------------------------------------------------------------------
fun_logic_eq
	: '='
	{
		LPDocUpdate pInsert = rdo::Factory<UpdateInsert>::create(@1.m_last_seek, _T("="));
		ASSERT(pInsert);
		CONVERTER->insertDocUpdate(pInsert);

		$$ = RDO_eq;
	}
	| RDO_eq
	{
		$$ = RDO_eq;
	}
	;

fun_logic
	: fun_arithm fun_logic_eq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = CONVERTER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator ==(pArithm2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_arithm RDO_neq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = CONVERTER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator !=(pArithm2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_arithm '<' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = CONVERTER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator <(pArithm2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_arithm '>' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = CONVERTER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator >(pArithm2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_arithm RDO_leq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = CONVERTER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator <=(pArithm2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_arithm RDO_geq fun_arithm
	{
		LPRDOFUNArithm pArithm1 = CONVERTER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNLogic pResult = pArithm1->operator >=(pArithm2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_logic RDO_and fun_logic
	{
		LPRDOFUNLogic pLogic1 = CONVERTER->stack().pop<RDOFUNLogic>($1);
		LPRDOFUNLogic pLogic2 = CONVERTER->stack().pop<RDOFUNLogic>($3);
		ASSERT(pLogic1);
		ASSERT(pLogic2);
		LPRDOFUNLogic pResult = pLogic1->operator &&(pLogic2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_logic RDO_or fun_logic
	{
		LPRDOFUNLogic pLogic1 = CONVERTER->stack().pop<RDOFUNLogic>($1);
		LPRDOFUNLogic pLogic2 = CONVERTER->stack().pop<RDOFUNLogic>($3);
		ASSERT(pLogic1);
		ASSERT(pLogic2);
		LPRDOFUNLogic pResult = pLogic1->operator ||(pLogic2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_arithm
	{
		LPRDOFUNArithm pArithm = CONVERTER->stack().pop<RDOFUNArithm>($1);
		ASSERT(pArithm);
		LPRDOFUNLogic pResult = rdo::Factory<RDOFUNLogic>::create(pArithm);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_group
	| fun_select_logic
	| '[' fun_logic ']'
	{
		LPRDOFUNLogic pLogic = CONVERTER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		pLogic->setSrcPos (@1, @3);
		pLogic->setSrcText(_T("[") + pLogic->src_text() + _T("]"));
		$$ = CONVERTER->stack().push(pLogic);
	}
	| '(' fun_logic ')'
	{
		LPRDOFUNLogic pLogic = CONVERTER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		pLogic->setSrcPos (@1, @3);
		pLogic->setSrcText(_T("(") + pLogic->src_text() + _T(")"));
		$$ = CONVERTER->stack().push(pLogic);
	}
	| RDO_not fun_logic
	{
		LPRDOFUNLogic pLogic = CONVERTER->stack().pop<RDOFUNLogic>($2);
		ASSERT(pLogic);
		LPRDOFUNLogic pLogicNot = pLogic->operator_not();
		ASSERT(pLogicNot);
		pLogicNot->setSrcPos (@1, @2);
		pLogicNot->setSrcText(_T("not ") + pLogic->src_text());
		$$ = CONVERTER->stack().push(pLogicNot);
	}
	| '[' fun_logic error
	{
		CONVERTER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
	}
	| '(' fun_logic error
	{
		CONVERTER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Арифметические выражения
// --------------------------------------------------------------------------------
fun_arithm
	: RDO_INT_CONST                      { $$ = CONVERTER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
	| RDO_REAL_CONST                     { $$ = CONVERTER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
	| RDO_BOOL_CONST                     { $$ = CONVERTER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
	| RDO_STRING_CONST                   { $$ = CONVERTER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
	| RDO_IDENTIF                        { $$ = CONVERTER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1))); }
	| RDO_IDENTIF '.' RDO_IDENTIF        { $$ = CONVERTER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1), RDOVALUE($3))); }
	| RDO_IDENTIF_RELRES '.' RDO_IDENTIF { $$ = CONVERTER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOVALUE($1), RDOVALUE($3))); }
	| fun_arithm '+' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = CONVERTER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator +(pArithm2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_arithm '-' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = CONVERTER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator -(pArithm2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_arithm '*' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = CONVERTER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator *(pArithm2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_arithm '/' fun_arithm
	{
		LPRDOFUNArithm pArithm1 = CONVERTER->stack().pop<RDOFUNArithm>($1);
		LPRDOFUNArithm pArithm2 = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pArithm1);
		ASSERT(pArithm2);
		LPRDOFUNArithm pResult = pArithm1->operator /(pArithm2);
		ASSERT(pResult);
		$$ = CONVERTER->stack().push(pResult);
	}
	| fun_arithm_func_call
	| fun_select_arithm
	| '(' fun_arithm ')'
	{
		LPRDOFUNArithm pArithm = CONVERTER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		pArithm->setSrcPos (@1, @3);
		pArithm->setSrcText(_T("(") + pArithm->src_text() + _T(")"));
		$$ = CONVERTER->stack().push(pArithm);
	}
	| '-' fun_arithm %prec RDO_UMINUS
	{
		LPRDOFUNArithm pArithm = CONVERTER->stack().pop<RDOFUNArithm>($2);
		ASSERT(pArithm);
		RDOParserSrcInfo info;
		info.setSrcPos (@1, @2);
		info.setSrcText(_T("-") + pArithm->src_text());
		$$ = CONVERTER->stack().push(rdo::Factory<RDOFUNArithm>::create(RDOValue(pArithm->type(), info), rdo::Factory<rdoRuntime::RDOCalcUMinus>::create(info.src_pos(), pArithm->createCalc())));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Функции и последовательности
// --------------------------------------------------------------------------------
fun_arithm_func_call
	: RDO_IDENTIF '(' ')'
	{
		LPRDOFUNParams pFunParams = rdo::Factory<RDOFUNParams>::create();
		ASSERT(pFunParams);
		tstring funName = RDOVALUE($1)->getIdentificator();
		pFunParams->getFunseqName().setSrcInfo(RDOParserSrcInfo(@1, funName));
		pFunParams->setSrcPos (@1, @3);
		pFunParams->setSrcText(funName + _T("()"));
		LPRDOFUNArithm pArithm = pFunParams->createCall(funName);
		ASSERT(pArithm);
		$$ = CONVERTER->stack().push(pArithm);
	}
	| RDO_IDENTIF '(' fun_arithm_func_call_pars ')'
	{
		LPRDOFUNParams pFunParams = CONVERTER->stack().pop<RDOFUNParams>($3);
		ASSERT(pFunParams);
		tstring funName = RDOVALUE($1)->getIdentificator();
		pFunParams->getFunseqName().setSrcInfo(RDOParserSrcInfo(@1, funName));
		pFunParams->setSrcPos (@1, @4);
		pFunParams->setSrcText(funName + _T("(") + pFunParams->src_text() + _T(")"));
		LPRDOFUNArithm pArithm = pFunParams->createCall(funName);
		ASSERT(pArithm);
		$$ = CONVERTER->stack().push(pArithm);
	}
	| RDO_IDENTIF '(' error
	{
		CONVERTER->error().error(@3, _T("Ошибка в параметрах функции"));
	}
	;

fun_arithm_func_call_pars
	: fun_arithm
	{
		LPRDOFUNParams pFunParams = rdo::Factory<RDOFUNParams>::create();
		LPRDOFUNArithm pArithm    = CONVERTER->stack().pop<RDOFUNArithm>($1);
		ASSERT(pFunParams);
		ASSERT(pArithm   );
		pFunParams->setSrcText  (pArithm->src_text());
		pFunParams->addParameter(pArithm);
		$$ = CONVERTER->stack().push(pFunParams);
	}
	| fun_arithm_func_call_pars ',' fun_arithm
	{
		LPRDOFUNParams pFunParams = CONVERTER->stack().pop<RDOFUNParams>($1);
		LPRDOFUNArithm pArithm    = CONVERTER->stack().pop<RDOFUNArithm>($3);
		ASSERT(pFunParams);
		ASSERT(pArithm   );
		pFunParams->setSrcText  (pFunParams->src_text() + _T(", ") + pArithm->src_text());
		pFunParams->addParameter(pArithm);
		$$ = CONVERTER->stack().push(pFunParams);
	}
	| fun_arithm_func_call_pars error
	{
		CONVERTER->error().error(@2, _T("Ошибка в арифметическом выражении"));
	}
	| fun_arithm_func_call_pars ',' error
	{
		CONVERTER->error().error(@3, _T("Ошибка в арифметическом выражении"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Групповые выражения
// --------------------------------------------------------------------------------
fun_group_keyword
	: RDO_Exist       { $$ = RDOFUNGroupLogic::fgt_exist;     }
	| RDO_Not_Exist   { $$ = RDOFUNGroupLogic::fgt_notexist;  }
	| RDO_For_All     { $$ = RDOFUNGroupLogic::fgt_forall;    }
	| RDO_Not_For_All { $$ = RDOFUNGroupLogic::fgt_notforall; }
	;

fun_group_header
	: fun_group_keyword '(' RDO_IDENTIF_COLON
	{
		PTR(RDOValue) type_name = P_RDOVALUE($3);
		$$ = CONVERTER->stack().push(rdo::Factory<RDOFUNGroupLogic>::create((RDOFUNGroupLogic::FunGroupType)$1, type_name->src_info()));
	}
	| fun_group_keyword '(' error
	{
		CONVERTER->error().error(@3, _T("Ожидается имя типа"));
	}
	| fun_group_keyword error
	{
		CONVERTER->error().error(@1, _T("После имени функции ожидается октрывающаяся скобка"));
	}
	;

fun_group
	: fun_group_header fun_logic ')'
	{
		LPRDOFUNGroupLogic pGroupFun = CONVERTER->stack().pop<RDOFUNGroupLogic>($1);
		LPRDOFUNLogic      pLogic    = CONVERTER->stack().pop<RDOFUNLogic>     ($2);
		ASSERT(pGroupFun);
		ASSERT(pLogic   );
		pGroupFun->setSrcPos(@1, @3);
		$$ = CONVERTER->stack().push(pGroupFun->createFunLogic(pLogic));
	}
	| fun_group_header RDO_NoCheck ')'
	{
		LPRDOFUNGroupLogic pGroupFun = CONVERTER->stack().pop<RDOFUNGroupLogic>($1);
		ASSERT(pGroupFun);
		pGroupFun->setSrcPos(@1, @3);
		LPRDOFUNLogic pTrueLogic = rdo::Factory<RDOFUNLogic>::create(rdo::Factory<rdoRuntime::RDOCalcConst>::create(1), false);
		ASSERT(pTrueLogic);
		pTrueLogic->setSrcPos (@2);
		pTrueLogic->setSrcText(_T("NoCheck"));
		$$ = CONVERTER->stack().push(pGroupFun->createFunLogic(pTrueLogic));
	}
	| fun_group_header fun_logic error
	{
		CONVERTER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
	}
	| fun_group_header RDO_NoCheck error
	{
		CONVERTER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
	}
	| fun_group_header error
	{
		CONVERTER->error().error(@1, @2, _T("Ошибка в логическом выражении"));
	}
	;

// --------------------------------------------------------------------------------
// -------------------- Select
// --------------------------------------------------------------------------------
fun_select_header
	: RDO_Select '(' RDO_IDENTIF_COLON
	{
		PTR(RDOValue)  type_name = P_RDOVALUE($3);
		LPRDOFUNSelect pSelect   = rdo::Factory<RDOFUNSelect>::create(type_name->src_info());
		ASSERT(pSelect);
		pSelect->setSrcText(_T("Select(") + type_name->value().getIdentificator() + _T(": "));
		$$ = CONVERTER->stack().push(pSelect);
	}
	| RDO_Select '(' error
	{
		CONVERTER->error().error(@3, _T("Ожидается имя типа"));
	}
	| RDO_Select error
	{
		CONVERTER->error().error(@1, _T("Ожидается октрывающаяся скобка"));
	}
	;

fun_select_body
	: fun_select_header fun_logic ')'
	{
		LPRDOFUNSelect pSelect = CONVERTER->stack().pop<RDOFUNSelect>($1);
		LPRDOFUNLogic  pLogic  = CONVERTER->stack().pop<RDOFUNLogic> ($2);
		ASSERT(pSelect);
		ASSERT(pLogic );
		pSelect->setSrcText(pSelect->src_text() + pLogic->src_text() + _T(")"));
		pSelect->initSelect(pLogic);
		$$ = CONVERTER->stack().push(pSelect);
	}
	| fun_select_header RDO_NoCheck ')'
	{
		LPRDOFUNSelect pSelect = CONVERTER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		RDOParserSrcInfo logicInfo(@2, _T("NoCheck"));
		pSelect->setSrcText(pSelect->src_text() + logicInfo.src_text() + _T(")"));
		rdoRuntime::LPRDOCalcConst pCalc  = rdo::Factory<rdoRuntime::RDOCalcConst>::create(1);
		ASSERT(pCalc);
		LPRDOFUNLogic pLogic = rdo::Factory<RDOFUNLogic>::create(pCalc, true);
		ASSERT(pLogic);
		pLogic->setSrcInfo(logicInfo);
		pSelect->initSelect(pLogic);
		$$ = CONVERTER->stack().push(pSelect);
	}
	| fun_select_header fun_logic error
	{
		CONVERTER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
	}
	| fun_select_header RDO_NoCheck error
	{
		CONVERTER->error().error(@2, _T("Ожидается закрывающаяся скобка"));
	}
	| fun_select_header error
	{
		CONVERTER->error().error(@1, @2, _T("Ошибка в логическом выражении"));
	}
	;

fun_select_keyword
	: RDO_Exist			{ $$ = RDOFUNGroupLogic::fgt_exist;     }
	| RDO_Not_Exist		{ $$ = RDOFUNGroupLogic::fgt_notexist;  }
	| RDO_For_All		{ $$ = RDOFUNGroupLogic::fgt_forall;    }
	| RDO_Not_For_All	{ $$ = RDOFUNGroupLogic::fgt_notforall; }
	;

fun_select_logic
	: fun_select_body '.' fun_select_keyword '(' fun_logic ')'
	{
		LPRDOFUNSelect pSelect = CONVERTER->stack().pop<RDOFUNSelect>($1);
		LPRDOFUNLogic  pLogic  = CONVERTER->stack().pop<RDOFUNLogic> ($5);
		ASSERT(pSelect);
		ASSERT(pLogic );
		pSelect->setSrcPos(@1, @6);
		LPRDOFUNLogic pLogicSelect = pSelect->createFunSelectGroup((RDOFUNGroupLogic::FunGroupType)$3, pLogic);
		ASSERT(pLogicSelect);
		$$ = CONVERTER->stack().push(pLogicSelect);
	}
	| fun_select_body '.' fun_select_keyword '(' error
	{
		CONVERTER->error().error(@4, @5, _T("Ошибка в логическом выражении"));
	}
	| fun_select_body '.' fun_select_keyword error
	{
		CONVERTER->error().error(@3, _T("Ожидается октрывающаяся скобка"));
	}
	| fun_select_body '.' RDO_Empty '(' ')'
	{
		LPRDOFUNSelect pSelect = CONVERTER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		pSelect->setSrcPos(@1, @5);
		RDOParserSrcInfo emptyInfo(@3, @5, _T("Empty()"));
		LPRDOFUNLogic pLogic = pSelect->createFunSelectEmpty(emptyInfo);
		ASSERT(pLogic);
		$$ = CONVERTER->stack().push(pLogic);
	}
	| fun_select_body '.' RDO_Empty '(' error
	{
		CONVERTER->error().error(@4, _T("Ожидается закрывающаяся скобка"));
	}
	| fun_select_body '.' RDO_Empty error
	{
		CONVERTER->error().error(@3, _T("Ожидается октрывающаяся скобка"));
	}
	| fun_select_body '.' error
	{
		CONVERTER->error().error(@2, @3, _T("Ожидается метод списка ресурсов"));
	}
	| fun_select_body error
	{
		CONVERTER->error().error(@1, _T("Ожидается '.' (точка) для вызова метода списка ресурсов"));
	}
	;

fun_select_arithm
	: fun_select_body '.' RDO_Size '(' ')'
	{
		LPRDOFUNSelect pSelect = CONVERTER->stack().pop<RDOFUNSelect>($1);
		ASSERT(pSelect);
		pSelect->setSrcPos(@1, @5);
		RDOParserSrcInfo sizeInfo(@3, @5, _T("Size()"));
		LPRDOFUNArithm pArithm = pSelect->createFunSelectSize(sizeInfo);
		ASSERT(pArithm);
		$$ = CONVERTER->stack().push(pArithm);
	}
	| fun_select_body '.' RDO_Size error
	{
		CONVERTER->error().error(@3, _T("Ожидается октрывающаяся скобка"));
	}
	| fun_select_body '.' RDO_Size '(' error
	{
		CONVERTER->error().error(@4, _T("Ожидается закрывающаяся скобка"));
	}
	;

%%

CLOSE_RDO_CONVERTER_NAMESPACE
