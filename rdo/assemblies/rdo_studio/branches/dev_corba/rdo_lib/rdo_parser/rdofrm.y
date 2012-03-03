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
%token RDO_IDENTIF_set					319
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
%token RDO_result						370
%token RDO_CF							371
%token RDO_Priority                     372

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

%{
#include "pch.h"

#include <FlexLexer.h>

#include "rdoparser.h"
#include "rdoparser_lexer.h"
#include "rdortp.h"
#include "rdofun.h"
#include "rdofrm.h"
#include "rdopat.h"
#include "rdoopr.h"
#include "rdodpt.h"
#include <rdocalc.h>
#include <rdoanimation.h>

#define PARSER  reinterpret_cast<rdoParse::RDOLexer*>(lexer)->m_parser
#define RUNTIME PARSER->runtime()

namespace rdoParse 
{
%}

%left RDO_or
%left RDO_and
%left '+' '-'
%left '*' '/'
%left RDO_not
%left RDO_UMINUS

%start frm_main

%%

// ----------------------------------------------------------------------------
// ---------- Основной цикл
// ----------------------------------------------------------------------------
frm_main:	/* empty */
			| frm_main frm_end
			| error {
				PARSER->error( @1, "Неизвестная ошибка" );
			};


// ----------------------------------------------------------------------------
// ---------- Фрейм
// ----------------------------------------------------------------------------
frm_begin:			RDO_Frame RDO_IDENTIF {
						$$ = (int)new RDOFRMFrame( PARSER, reinterpret_cast<RDOValue*>($2)->src_info() );;
					}
					| RDO_Frame RDO_IDENTIF RDO_Show_if fun_logic {
						$$ = (int)new RDOFRMFrame( PARSER, reinterpret_cast<RDOValue*>($2)->src_info(), reinterpret_cast<RDOFUNLogic*>($4) );;
					}
					| RDO_Frame RDO_IDENTIF RDO_Show_if error {
						PARSER->error( @4, "Ошибка в логическом выражении" )
					};

frm_background:		frm_begin RDO_Back_picture '=' frm_color {
						RDOFRMFrame* frame                             = reinterpret_cast<RDOFRMFrame*>($1);
						rdoRuntime::RDOFRMFrame::RDOFRMColor* bg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($4);
						if ( bg_color->getColorType() != rdoRuntime::RDOFRMFrame::RDOFRMColor::color_transparent && bg_color->getColorType() != rdoRuntime::RDOFRMFrame::RDOFRMColor::color_rgb ) {
							PARSER->error( @4, "Цвет фона не может быть указан ссылкой на последнее значение" );
						}
						frame->frame()->setBackgroundColor( bg_color );
					}
					| frm_begin RDO_Back_picture '=' {
					}
					| frm_begin RDO_Back_picture error {
						PARSER->error( @3, "После ключевого слова $Back_picture ожидается знак равенства" );
					}
					| frm_begin error {
						PARSER->error( @2, "После имени кадра ожидается ключевое слово $Back_picture" );
					};

frm_backpicture:	frm_background RDO_IDENTIF {
						RDOFRMFrame* frame = reinterpret_cast<RDOFRMFrame*>($1);
						frame->frame()->setBackPicture( reinterpret_cast<RDOValue*>($2)->value().getIdentificator() );
					}
					| frm_background RDO_INT_CONST RDO_INT_CONST {
						RDOFRMFrame* frame = reinterpret_cast<RDOFRMFrame*>($1);
						frame->frame()->setBackPicture( reinterpret_cast<RDOValue*>($2)->value().getInt(), reinterpret_cast<RDOValue*>($3)->value().getInt() );
					}
					| frm_background RDO_INT_CONST RDO_INT_CONST error {
						PARSER->error( @4, "Описание заголовка кадра окончено, ожидается ключевое слово $Show" );
					}
					| frm_background RDO_IDENTIF error {
						PARSER->error( @3, "Описание заголовка кадра окончено, ожидается ключевое слово $Show" );
					}
					| frm_background RDO_INT_CONST error {
						PARSER->error( @2, "После ширины кадра необходимо указать его высоту" );
					}
					| frm_background {
						PARSER->error( @1, "Необходимо указать имя фоновой картинки или размер кадра" );
					};

/*
frm_show:			frm_backpicture {
						RDOFRMFrame* frame = reinterpret_cast<RDOFRMFrame*>($1);
						frame->startShow();
					}
					| frm_backpicture RDO_Show {
						RDOFRMFrame* frame = reinterpret_cast<RDOFRMFrame*>($1);
						frame->startShow();
					}
					| frm_backpicture RDO_Show_if fun_logic {
						RDOFRMFrame* frame = reinterpret_cast<RDOFRMFrame*>($1);
						frame->startShow( reinterpret_cast<RDOFUNLogic*>($3)->calc );
					}
					| frm_backpicture RDO_Show_if error {
						PARSER->error( @3, "Ошибка в логическом выражении" )
					};
*/

frm_show:	RDO_Show {
				RDOFRMFrame* frame = PARSER->getLastFRMFrame();
				frame->frame()->startShow();
			}
			| RDO_Show_if fun_logic {
				RDOFRMFrame* frame = PARSER->getLastFRMFrame();
				frame->frame()->startShow( reinterpret_cast<RDOFUNLogic*>($2)->getCalc() );
			}
			| RDO_Show_if error {
				PARSER->error( @2, "Ошибка в логическом выражении" )
			};

frm_item:	/* empty */
			| frm_item frm_show
			| frm_item frm_text    { PARSER->getLastFRMFrame()->frame()->addItem((rdoRuntime::RDOFRMText *)$2); }
			| frm_item frm_bitmap  { PARSER->getLastFRMFrame()->frame()->addItem((rdoRuntime::RDOFRMBitmap *)$2); }
			| frm_item frm_rect    { PARSER->getLastFRMFrame()->frame()->addItem((rdoRuntime::RDOFRMRect *)$2); }
			| frm_item frm_line    { PARSER->getLastFRMFrame()->frame()->addItem((rdoRuntime::RDOFRMLine *)$2); }
			| frm_item frm_ellipse { PARSER->getLastFRMFrame()->frame()->addItem((rdoRuntime::RDOFRMEllipse *)$2); }
			| frm_item frm_r_rect  { PARSER->getLastFRMFrame()->frame()->addItem((rdoRuntime::RDOFRMRectRound *)$2); }
			| frm_item frm_triang  { PARSER->getLastFRMFrame()->frame()->addItem((rdoRuntime::RDOFRMTriang *)$2); }
			| frm_item frm_s_bmp   { PARSER->getLastFRMFrame()->frame()->addItem((rdoRuntime::RDOFRMBitmapStretch *)$2); }
			| frm_item frm_active  { PARSER->getLastFRMFrame()->frame()->addItem((rdoRuntime::RDOFRMActive *)$2); }
			| frm_item frm_ruler   { PARSER->getLastFRMFrame()->frame()->addRulet((rdoRuntime::RDOFRMFrame::RDOFRMRulet*)$2); }
			| frm_item frm_space   { PARSER->getLastFRMFrame()->frame()->addItem((rdoRuntime::RDOFRMSpace*)$2); };

frm_header:	frm_backpicture frm_item;

frm_end:	frm_header RDO_End {
			};

// ----------------------------------------------------------------------------
// ---------- Элементы
// ----------------------------------------------------------------------------
frm_color:	RDO_color_transparent {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame(), rdoRuntime::RDOFRMFrame::RDOFRMColor::color_transparent );
			}
			| RDO_color_last {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame() );
			}
			| RDO_color_white {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame(), 255, 255, 255 );
			}
			| RDO_color_black {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame(), 0, 0, 0 );
			}
			| RDO_color_red {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame(), 255, 0, 0 );
			}
			| RDO_color_green {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame(), 0, 255, 0 );
			}
			| RDO_color_blue {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame(), 0, 0, 255 );
			}
			| RDO_color_cyan {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame(), 0, 255, 255 );
			}
			| RDO_color_magenta {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame(), 255, 0, 255 );
			}
			| RDO_color_yellow {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame(), 255, 255, 0 );
			}
			| RDO_color_gray {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame(), 127, 127, 127 );
			}
			| '<' RDO_INT_CONST RDO_INT_CONST RDO_INT_CONST '>' {
				RDOFUNArithm* red   = new RDOFUNArithm( PARSER, *reinterpret_cast<RDOValue*>($2) );
				RDOFUNArithm* green = new RDOFUNArithm( PARSER, *reinterpret_cast<RDOValue*>($3) );
				RDOFUNArithm* blue  = new RDOFUNArithm( PARSER, *reinterpret_cast<RDOValue*>($4) );
				RDORTPIntParamType intType( PARSER, new RDORTPIntDiap( PARSER, 0, 255, @1, @1 ), new RDORTPDefVal(PARSER) );
				intType.checkParamType( red );
				intType.checkParamType( green );
				intType.checkParamType( blue );
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame(), red->createCalc(), green->createCalc(), blue->createCalc() );
			}
			| '<' RDO_INT_CONST RDO_INT_CONST RDO_INT_CONST error {
				PARSER->error( @4, "Найдены все составляющие цвета, ожидается '>'" );
			}
			| '<' RDO_INT_CONST RDO_INT_CONST error {
				PARSER->error( @3, @4, "Ожидается синяя составляющая цвета" );
			}
			| '<' RDO_INT_CONST error {
				PARSER->error( @2, @3, "Ожидается зеленая составляющая цвета" );
			}
			| '<' fun_arithm ',' fun_arithm ',' fun_arithm '>' {
				RDOFUNArithm* red   = reinterpret_cast<RDOFUNArithm*>($2);
				RDOFUNArithm* green = reinterpret_cast<RDOFUNArithm*>($4);
				RDOFUNArithm* blue  = reinterpret_cast<RDOFUNArithm*>($6);
				RDORTPIntParamType intType( PARSER, new RDORTPIntDiap( PARSER, 0, 255, @1, @1 ), new RDORTPDefVal(PARSER) );
				intType.checkParamType( red );
				intType.checkParamType( green );
				intType.checkParamType( blue );
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame(), red->createCalc(), green->createCalc(), blue->createCalc() );
			}
			| '<' fun_arithm ',' fun_arithm ',' fun_arithm error {
				PARSER->error( @6, "Найдены все составляющие цвета, ожидается '>'" );
			}
			| '<' fun_arithm ',' fun_arithm ',' error {
				PARSER->error( @5, @6, "Ожидается синяя составляющая цвета" );
			}
			| '<' fun_arithm ',' fun_arithm error {
				PARSER->error( @4, "После зеленой составляющей цвета ожидается запятая" );
			}
			| '<' fun_arithm ',' error {
				PARSER->error( @3, @4, "Ожидается зеленая составляющая цвета" );
			}
			| '<' fun_arithm error {
				PARSER->error( @2, "После красной составляющей цвета ожидается запятая" );
			}
			| '<' error {
				PARSER->error( @1, "После '<' ожидается красная составляющая цвета" );
			};

frm_postype:	/* empty */     { $$ = rdoRuntime::RDOFRMFrame::RDOFRMPosition::absolute; }
				| '+'           { $$ = rdoRuntime::RDOFRMFrame::RDOFRMPosition::delta;    }
				| '*'           { $$ = rdoRuntime::RDOFRMFrame::RDOFRMPosition::mult;     }
				| '#' RDO_INT_CONST {
					int rilet_id = reinterpret_cast<RDOValue*>($2)->value().getInt();
					if ( rilet_id <= 0 ) {
						PARSER->error( @2, "Номер рулетки должен быть больше нуля" );
					}
					if ( !RUNTIME->lastFrame()->findRulet( rilet_id ) ) {
						PARSER->error( @2, rdo::format("Рулетки с номером '%d' не существует", rilet_id) );
					}
					$$ = rdoRuntime::RDOFRMFrame::RDOFRMPosition::rulet + rilet_id;
				}
				| '#' error {
					PARSER->error( @1, "После '#' ожидается целочисленный номер рулетки" );
				};

frm_postype_xy: frm_postype
				| '=' {
					$$ = rdoRuntime::RDOFRMFrame::RDOFRMPosition::gabarit;
				};

frm_postype_wh:	frm_postype;
				| '=' {
					PARSER->error( @1, "Нельзя использовать данное выравнивание для ширины или высоты" );
				};

frm_position_xy: fun_arithm frm_postype_xy {
					rdoRuntime::RDOCalc* calc = reinterpret_cast<RDOFUNArithm*>($1)->createCalc();
					if ( $2 >= rdoRuntime::RDOFRMFrame::RDOFRMPosition::rulet ) {
						$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMPosition( RUNTIME->lastFrame(), calc, rdoRuntime::RDOFRMFrame::RDOFRMPosition::rulet, $2 - rdoRuntime::RDOFRMFrame::RDOFRMPosition::rulet );
					} else {
						$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMPosition( RUNTIME->lastFrame(), calc, (rdoRuntime::RDOFRMFrame::RDOFRMPosition::PositionType)$2 );
					}
				};

frm_position_wh: fun_arithm frm_postype_wh {
					rdoRuntime::RDOCalc* calc = reinterpret_cast<RDOFUNArithm*>($1)->createCalc();
					if ( $2 >= rdoRuntime::RDOFRMFrame::RDOFRMPosition::rulet ) {
						$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMPosition( RUNTIME->lastFrame(), calc, rdoRuntime::RDOFRMFrame::RDOFRMPosition::rulet, $2 - rdoRuntime::RDOFRMFrame::RDOFRMPosition::rulet );
					} else {
						$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMPosition( RUNTIME->lastFrame(), calc, (rdoRuntime::RDOFRMFrame::RDOFRMPosition::PositionType)$2 );
					}
				};

frm_ruler:		RDO_ruler '[' RDO_INT_CONST ',' frm_position_xy ',' frm_position_xy ']' {
					const rdoRuntime::RDOFRMFrame::RDOFRMRulet* rulet = RUNTIME->lastFrame()->findRulet( reinterpret_cast<RDOValue*>($3)->value().getInt() );
					if ( rulet ) {
						PARSER->error_push_only( @3, rdo::format("Рулетка с номером '%d' уже существует", reinterpret_cast<RDOValue*>($3)->value().getInt()) );
						PARSER->error_push_only( rulet->src_info(), "См. первое определение" );
						PARSER->error_push_done();
					}
					rdoRuntime::RDOFRMFrame::RDOFRMPosition* x = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
					rdoRuntime::RDOFRMFrame::RDOFRMPosition* y = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
					if ( x->type != rdoRuntime::RDOFRMFrame::RDOFRMPosition::absolute ) {
						PARSER->error( @5, "Коодинаты рулетки должны быть абсолютными" );
					}
					if ( y->type != rdoRuntime::RDOFRMFrame::RDOFRMPosition::absolute ) {
						PARSER->error( @7, "Коодинаты рулетки должны быть абсолютными" );
					}
					$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMRulet( RDOParserSrcInfo(@1), reinterpret_cast<RDOValue*>($3)->value().getInt(), x, y );
				}
				| RDO_ruler '[' RDO_INT_CONST ',' frm_position_xy ',' frm_position_xy error {
					PARSER->error( @7, "Ожидается ']'" );
				}
				| RDO_ruler '[' RDO_INT_CONST ',' frm_position_xy ',' error {
					PARSER->error( @6, @7, "Ожидается координата по оси Y" );
				}
				| RDO_ruler '[' RDO_INT_CONST ',' frm_position_xy error {
					PARSER->error( @5, "Ожидается запятая" );
				}
				| RDO_ruler '[' RDO_INT_CONST ',' error {
					PARSER->error( @4, @5, "Ожидается координата по оси X" );
				}
				| RDO_ruler '[' RDO_INT_CONST error {
					PARSER->error( @3, "Ожидается запятая" );
				}
				| RDO_ruler '[' error {
					PARSER->error( @2, @3, "Ожидается номер рулетки" );
				}
				| RDO_ruler error {
					PARSER->error( @1, "Ожидается '['" );
				};

frm_space:	RDO_space '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
				$$ = (int)new rdoRuntime::RDOFRMSpace( RUNTIME->lastFrame(), x, y, width, height );
			}
			| RDO_space '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error {
				PARSER->error( @9, "Ожидается ']'" );
			}
			| RDO_space '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error {
				PARSER->error( @8, @9, "Ожидается высота" );
			}
			| RDO_space '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error {
				PARSER->error( @7, "Ожидается запятая" );
			}
			| RDO_space '[' frm_position_xy ',' frm_position_xy ',' error {
				PARSER->error( @6, @7, "Ожидается ширина" );
			}
			| RDO_space '[' frm_position_xy ',' frm_position_xy error {
				PARSER->error( @5, "Ожидается запятая" );
			}
			| RDO_space '[' frm_position_xy ',' error {
				PARSER->error( @4, @5, "Ожидается координата по оси Y" );
			}
			| RDO_space '[' frm_position_xy error {
				PARSER->error( @3, "Ожидается запятая" );
			}
			| RDO_space '[' error {
				PARSER->error( @2, @3, "Ожидается координата по оси X" );
			}
			| RDO_space error {
				PARSER->error( @1, "Ожидается '['" );
			};

frm_text_align: /* empty */ { $$ = rdoAnimation::RDOTextElement::TETA_LEFT;   }
				| '<'       { $$ = rdoAnimation::RDOTextElement::TETA_LEFT;   }
				| '='       { $$ = rdoAnimation::RDOTextElement::TETA_CENTER; }
				| '>'       { $$ = rdoAnimation::RDOTextElement::TETA_RIGHT;  };

frm_text_common:	RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color ',' {
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
						rdoRuntime::RDOFRMFrame::RDOFRMColor* bg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($11);
						rdoRuntime::RDOFRMFrame::RDOFRMColor* fg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($13);
						bg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg_text );
						fg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg_text );
						$$ = (int)new rdoRuntime::RDOFRMText( RUNTIME->lastFrame(), x, y, width, height, bg_color, fg_color );
					}
/*
					| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' {
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
						rdoRuntime::RDOFRMFrame::RDOFRMColor* bg_color = new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame() );
						rdoRuntime::RDOFRMFrame::RDOFRMColor* fg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($11);
						bg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg_text );
						fg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg_text );
						$$ = (int)new rdoRuntime::RDOFRMText( RUNTIME->lastFrame(), x, y, width, height, bg_color, fg_color );
					}
					| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' {
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
						rdoRuntime::RDOFRMFrame::RDOFRMColor* bg_color = new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame() );
						rdoRuntime::RDOFRMFrame::RDOFRMColor* fg_color = new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame() );
						bg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg_text );
						fg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg_text );
						$$ = (int)new rdoRuntime::RDOFRMText( RUNTIME->lastFrame(), x, y, width, height, bg_color, fg_color );
					}
*/
					| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color error {
						PARSER->error( @13, "Ожидается запятая" );
					}
					| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' error {
						PARSER->error( @12, @13, "Ожидается цвет текста" );
					}
					| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color error {
						PARSER->error( @11, "Ожидается запятая" );
					}
					| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' error {
						PARSER->error( @10, @11, "Ожидается цвет фона" );
					}
					| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error {
						PARSER->error( @9, "Ожидается запятая" );
					}
					| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error {
						PARSER->error( @8, @9, "Ожидается высота" );
					}
					| RDO_text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error {
						PARSER->error( @7, "Ожидается запятая" );
					}
					| RDO_text '[' frm_position_xy ',' frm_position_xy ',' error {
						PARSER->error( @6, @7, "Ожидается ширина" );
					}
					| RDO_text '[' frm_position_xy ',' frm_position_xy error {
						PARSER->error( @5, "Ожидается запятая" );
					}
					| RDO_text '[' frm_position_xy ',' error {
						PARSER->error( @4, @5, "Ожидается координата по оси Y" );
					}
					| RDO_text '[' frm_position_xy error {
						PARSER->error( @3, "Ожидается запятая" );
					}
					| RDO_text '[' error {
						PARSER->error( @2, @3, "Ожидается координата по оси X" );
					}
					| RDO_text error {
						PARSER->error( @1, "Ожидается '['" );
					};

frm_text:	frm_text_common frm_text_align fun_arithm ']' {
				((rdoRuntime::RDOFRMText *)$1)->setText( (rdoAnimation::RDOTextElement::TextAlign)$2, ((RDOFUNArithm *)$3)->createCalc() );
			}
			| frm_text_common frm_text_align RDO_STRING_CONST ']' {
				((rdoRuntime::RDOFRMText *)$1)->setText( (rdoAnimation::RDOTextElement::TextAlign)$2, reinterpret_cast<RDOValue*>($3)->value().getString() );
			}
			| frm_text_common frm_text_align fun_arithm error {
				PARSER->error( @3, "Ожидается ']'" );
			}
			| frm_text_common frm_text_align RDO_STRING_CONST error {
				PARSER->error( @3, "Ожидается ']'" );
			}
			| frm_text_common frm_text_align error {
				PARSER->error( @2, "Ожидается параметр ресурса или строка" );
			};

frm_bitmap:	RDO_bitmap '[' frm_position_xy ',' frm_position_xy ',' RDO_IDENTIF ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				$$ = (int)new rdoRuntime::RDOFRMBitmap( RUNTIME->lastFrame(), x, y, reinterpret_cast<RDOValue*>($7)->value().getIdentificator() );
			}
			| RDO_bitmap '[' frm_position_xy ',' frm_position_xy ',' RDO_IDENTIF ',' RDO_IDENTIF ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				$$ = (int)new rdoRuntime::RDOFRMBitmap( RUNTIME->lastFrame(), x, y, reinterpret_cast<RDOValue*>($7)->value().getIdentificator(), reinterpret_cast<RDOValue*>($9)->value().getIdentificator() );
			}
			| RDO_bitmap '[' frm_position_xy ',' frm_position_xy ',' RDO_IDENTIF ',' RDO_IDENTIF error  {
				PARSER->error( @9, "Ожидается ']'" );
			}
			| RDO_bitmap '[' frm_position_xy ',' frm_position_xy ',' RDO_IDENTIF ',' error  {
				PARSER->error( @8, @9, "Ожидается имя маски для картинки" );
			}
			| RDO_bitmap '[' frm_position_xy ',' frm_position_xy ',' RDO_IDENTIF error  {
				PARSER->error( @7, "Ожидается ']'" );
			}
			| RDO_bitmap '[' frm_position_xy ',' frm_position_xy ',' error  {
				PARSER->error( @6, @7, "Ожидается имя картинки" );
			}
			| RDO_bitmap '[' frm_position_xy ',' frm_position_xy error  {
				PARSER->error( @5, "Ожидается запятая" );
			}
			| RDO_bitmap '[' frm_position_xy ',' error  {
				PARSER->error( @4, @5, "Ожидается координата по оси Y" );
			}
			| RDO_bitmap '[' frm_position_xy error  {
				PARSER->error( @3, "Ожидается запятая" );
			}
			| RDO_bitmap '[' error  {
				PARSER->error( @2, @3, "Ожидается координата по оси X" );
			}
			| RDO_bitmap error  {
				PARSER->error( @1, "Ожидается '['" );
			};

frm_s_bmp:	RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' RDO_IDENTIF ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
				$$ = (int)new rdoRuntime::RDOFRMBitmapStretch( RUNTIME->lastFrame(), x, y, width, height, reinterpret_cast<RDOValue*>($11)->value().getIdentificator() );
			}
			| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' RDO_IDENTIF ',' RDO_IDENTIF ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
				$$ = (int)new rdoRuntime::RDOFRMBitmapStretch( RUNTIME->lastFrame(), x, y, width, height, reinterpret_cast<RDOValue*>($11)->value().getIdentificator(), reinterpret_cast<RDOValue*>($13)->value().getIdentificator() );
			}
			| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' RDO_IDENTIF ',' RDO_IDENTIF error {
				PARSER->error( @13, "Ожидается ']'" );
			}
			| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' RDO_IDENTIF ',' error {
				PARSER->error( @12, @13, "Ожидается имя маски для картинки" );
			}
			| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' RDO_IDENTIF error {
				PARSER->error( @11, "Ожидается ']'" );
			}
			| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' error {
				PARSER->error( @10, @11, "Ожидается имя картинки" );
			}
			| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error {
				PARSER->error( @9, "Ожидается запятая" );
			}
			| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error {
				PARSER->error( @8, @9, "Ожидается высота" );
			}
			| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error {
				PARSER->error( @7, "Ожидается запятая" );
			}
			| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy ',' error {
				PARSER->error( @6, @7, "Ожидается ширина" );
			}
			| RDO_s_bmp '[' frm_position_xy ',' frm_position_xy error {
				PARSER->error( @5, "Ожидается запятая" );
			}
			| RDO_s_bmp '[' frm_position_xy ',' error {
				PARSER->error( @4, @5, "Ожидается координата по оси Y" );
			}
			| RDO_s_bmp '[' frm_position_xy error {
				PARSER->error( @3, "Ожидается запятая" );
			}
			| RDO_s_bmp '[' error {
				PARSER->error( @2, @3, "Ожидается координата по оси X" );
			}
			| RDO_s_bmp error {
				PARSER->error( @1, "Ожидается '['" );
			};

frm_rect:	RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
				rdoRuntime::RDOFRMFrame::RDOFRMColor* bg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($11);
				rdoRuntime::RDOFRMFrame::RDOFRMColor* fg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($13);
				bg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg );
				fg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg );
				$$ = (int)new rdoRuntime::RDOFRMRect( RUNTIME->lastFrame(), x, y, width, height, bg_color, fg_color );
			}
			| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
				rdoRuntime::RDOFRMFrame::RDOFRMColor* bg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($11);
				rdoRuntime::RDOFRMFrame::RDOFRMColor* fg_color = new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame() );
				bg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg );
				fg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg );
				$$ = (int)new rdoRuntime::RDOFRMRect( RUNTIME->lastFrame(), x, y, width, height, bg_color, fg_color );
			}
			| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
				rdoRuntime::RDOFRMFrame::RDOFRMColor* bg_color = new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame() );
				rdoRuntime::RDOFRMFrame::RDOFRMColor* fg_color = new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame() );
				bg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg );
				fg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg );
				$$ = (int)new rdoRuntime::RDOFRMRect( RUNTIME->lastFrame(), x, y, width, height, bg_color, fg_color );
			}
			| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = new rdoRuntime::RDOFRMFrame::RDOFRMPosition( RUNTIME->lastFrame(), new rdoRuntime::RDOCalcConst( RUNTIME, 0 ), rdoRuntime::RDOFRMFrame::RDOFRMPosition::delta );
				rdoRuntime::RDOFRMFrame::RDOFRMColor* bg_color = new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame() );
				rdoRuntime::RDOFRMFrame::RDOFRMColor* fg_color = new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame() );
				bg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg );
				fg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg );
				$$ = (int)new rdoRuntime::RDOFRMRect( RUNTIME->lastFrame(), x, y, width, height, bg_color, fg_color );
			}
			| RDO_rect '[' frm_position_xy ',' frm_position_xy ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = new rdoRuntime::RDOFRMFrame::RDOFRMPosition( RUNTIME->lastFrame(), new rdoRuntime::RDOCalcConst( RUNTIME, 0 ), rdoRuntime::RDOFRMFrame::RDOFRMPosition::delta );
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = new rdoRuntime::RDOFRMFrame::RDOFRMPosition( RUNTIME->lastFrame(), new rdoRuntime::RDOCalcConst( RUNTIME, 0 ), rdoRuntime::RDOFRMFrame::RDOFRMPosition::delta );
				rdoRuntime::RDOFRMFrame::RDOFRMColor* bg_color = new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame() );
				rdoRuntime::RDOFRMFrame::RDOFRMColor* fg_color = new rdoRuntime::RDOFRMFrame::RDOFRMColor( RUNTIME->lastFrame() );
				bg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg );
				fg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg );
				$$ = (int)new rdoRuntime::RDOFRMRect( RUNTIME->lastFrame(), x, y, width, height, bg_color, fg_color );
			}
			| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color error {
				PARSER->error( @13, "Ожидается ']'" );
			}
			| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' error {
				PARSER->error( @12, @13, "Ожидается цвет линии прямоугольника" );
			}
			| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color error {
				PARSER->error( @11, "Ожидается запятая" );
			}
			| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' error {
				PARSER->error( @10, @11, "Ожидается цвет фона" );
			}
			| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error {
				PARSER->error( @9, "Ожидается запятая" );
			}
			| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error {
				PARSER->error( @8, @9, "Ожидается высота" );
			}
			| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error {
				PARSER->error( @7, "Ожидается запятая" );
			}
			| RDO_rect '[' frm_position_xy ',' frm_position_xy ',' error {
				PARSER->error( @6, @7, "Ожидается ширина" );
			}
			| RDO_rect '[' frm_position_xy ',' frm_position_xy error {
				PARSER->error( @5, "Ожидается запятая" );
			}
			| RDO_rect '[' frm_position_xy ',' error {
				PARSER->error( @4, @5, "Ожидается координата по оси Y" );
			}
			| RDO_rect '[' frm_position_xy error {
				PARSER->error( @3, "Ожидается запятая" );
			}
			| RDO_rect '[' error {
				PARSER->error( @2, @3, "Ожидается координата по оси X" );
			}
			| RDO_rect error {
				PARSER->error( @1, "Ожидается '['" );
			};

frm_r_rect:	RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
				rdoRuntime::RDOFRMFrame::RDOFRMColor* bg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($11);
				rdoRuntime::RDOFRMFrame::RDOFRMColor* fg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($13);
				bg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg );
				fg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg );
				$$ = (int)new rdoRuntime::RDOFRMRectRound( RUNTIME->lastFrame(), x, y, width, height, bg_color, fg_color );
			}
			| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color error {
				PARSER->error( @13, "Ожидается ']'" );
			}
			| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' error {
				PARSER->error( @12, @13, "Ожидается цвет линии прямоугольника" );
			}
			| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color error {
				PARSER->error( @11, "Ожидается запятая" );
			}
			| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' error {
				PARSER->error( @10, @11, "Ожидается цвет фона" );
			}
			| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error {
				PARSER->error( @9, "Ожидается запятая" );
			}
			| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error {
				PARSER->error( @8, @9, "Ожидается высота" );
			}
			| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error {
				PARSER->error( @7, "Ожидается запятая" );
			}
			| RDO_r_rect '[' frm_position_xy ',' frm_position_xy ',' error {
				PARSER->error( @6, @7, "Ожидается ширина" );
			}
			| RDO_r_rect '[' frm_position_xy ',' frm_position_xy error {
				PARSER->error( @5, "Ожидается запятая" );
			}
			| RDO_r_rect '[' frm_position_xy ',' error {
				PARSER->error( @4, @5, "Ожидается координата по оси Y" );
			}
			| RDO_r_rect '[' frm_position_xy error {
				PARSER->error( @3, "Ожидается запятая" );
			}
			| RDO_r_rect '[' error {
				PARSER->error( @2, @3, "Ожидается координата по оси X" );
			}
			| RDO_r_rect error {
				PARSER->error( @1, "Ожидается '['" );
			};

frm_ellipse: RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
				rdoRuntime::RDOFRMFrame::RDOFRMColor* bg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($11);
				rdoRuntime::RDOFRMFrame::RDOFRMColor* fg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($13);
				bg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg );
				fg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg );
				$$ = (int)new rdoRuntime::RDOFRMEllipse( RUNTIME->lastFrame(), x, y, width, height, bg_color, fg_color );
			}
			| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color error {
				PARSER->error( @13, "Ожидается ']'" );
			}
			| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' error {
				PARSER->error( @12, @13, "Ожидается цвет линии эллипса" );
			}
			| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color error {
				PARSER->error( @11, "Ожидается запятая" );
			}
			| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' error {
				PARSER->error( @10, @11, "Ожидается цвет фона" );
			}
			| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error {
				PARSER->error( @9, "Ожидается запятая" );
			}
			| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error {
				PARSER->error( @8, @9, "Ожидается высота" );
			}
			| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error {
				PARSER->error( @7, "Ожидается запятая" );
			}
			| RDO_ellipse '[' frm_position_xy ',' frm_position_xy ',' error {
				PARSER->error( @6, @7, "Ожидается ширина" );
			}
			| RDO_ellipse '[' frm_position_xy ',' frm_position_xy error {
				PARSER->error( @5, "Ожидается запятая" );
			}
			| RDO_ellipse '[' frm_position_xy ',' error {
				PARSER->error( @4, @5, "Ожидается координата по оси Y" );
			}
			| RDO_ellipse '[' frm_position_xy error {
				PARSER->error( @3, "Ожидается запятая" );
			}
			| RDO_ellipse '[' error {
				PARSER->error( @2, @3, "Ожидается координата по оси X" );
			}
			| RDO_ellipse error {
				PARSER->error( @1, "Ожидается '['" );
			};

frm_line:	RDO_line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
				rdoRuntime::RDOFRMFrame::RDOFRMColor* color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($11);
				color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg );
				$$ = (int)new rdoRuntime::RDOFRMLine( RUNTIME->lastFrame(), x, y, width, height, color );
			}
			| RDO_line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color error {
				PARSER->error( @11, "Ожидается ']'" );
			}
			| RDO_line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error {
				PARSER->error( @10, @11, "Ожидается цвет линии" );
			}
			| RDO_line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy error {
				PARSER->error( @9, "Ожидается запятая" );
			}
			| RDO_line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error {
				PARSER->error( @8, @9, "Ожидается Y2" );
			}
			| RDO_line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy error {
				PARSER->error( @7, "Ожидается запятая" );
			}
			| RDO_line '[' frm_position_xy ',' frm_position_xy ',' error {
				PARSER->error( @6, @7, "Ожидается X2" );
			}
			| RDO_line '[' frm_position_xy ',' frm_position_xy error {
				PARSER->error( @5, "Ожидается запятая" );
			}
			| RDO_line '[' frm_position_xy ',' error {
				PARSER->error( @4, @5, "Ожидается Y1" );
			}
			| RDO_line '[' frm_position_xy error {
				PARSER->error( @3, "Ожидается запятая" );
			}
			| RDO_line '[' error {
				PARSER->error( @2, @3, "Ожидается X1" );
			}
			| RDO_line error {
				PARSER->error( @1, "Ожидается '['" );
			};

frm_triang:	RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color ',' frm_color ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x1 = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y1 = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x2 = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y2 = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x3 = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($11);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y3 = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($13);
				rdoRuntime::RDOFRMFrame::RDOFRMColor* bg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($15);
				rdoRuntime::RDOFRMFrame::RDOFRMColor* fg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($17);
				bg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg );
				fg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg );
				$$ = (int)new rdoRuntime::RDOFRMTriang( RUNTIME->lastFrame(), x1, y1, x2, y2, x3, y3, bg_color, fg_color );
			}
			| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color ',' frm_color error {
				PARSER->error( @17, "Ожидается ']'" );
			}
			| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color ',' error {
				PARSER->error( @16, @17, "Ожидается цвет линии" );
			}
			| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color error {
				PARSER->error( @15, "Ожидается запятая" );
			}
			| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error {
				PARSER->error( @14, @15, "Ожидается цвет треугольника" );
			}
			| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy error {
				PARSER->error( @13, "Ожидается запятая" );
			}
			| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error {
				PARSER->error( @12, @13, "Ожидается Y3" );
			}
			| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy error {
				PARSER->error( @11, "Ожидается запятая" );
			}
			| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error {
				PARSER->error( @10, @11, "Ожидается X3" );
			}
			| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy error {
				PARSER->error( @9, "Ожидается запятая" );
			}
			| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error {
				PARSER->error( @8, @9, "Ожидается Y2" );
			}
			| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy error {
				PARSER->error( @7, "Ожидается запятая" );
			}
			| RDO_triang '[' frm_position_xy ',' frm_position_xy ',' error {
				PARSER->error( @6, @7, "Ожидается X2" );
			}
			| RDO_triang '[' frm_position_xy ',' frm_position_xy error {
				PARSER->error( @5, "Ожидается запятая" );
			}
			| RDO_triang '[' frm_position_xy ',' error {
				PARSER->error( @4, @5, "Ожидается Y1" );
			}
			| RDO_triang '[' frm_position_xy error {
				PARSER->error( @3, "Ожидается запятая" );
			}
			| RDO_triang '[' error {
				PARSER->error( @2, @3, "Ожидается X1" );
			}
			| RDO_triang error {
				PARSER->error( @1, "Ожидается '['" );
			};

frm_active:	RDO_active RDO_IDENTIF '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ']' {
				std::string opr_name = reinterpret_cast<RDOValue*>($2)->value().getIdentificator();
				const RDOOPROperation* opr = PARSER->findOPROperation( opr_name );
				if ( !opr ) {
					const RDODPTFreeActivity* activity = PARSER->findDPTFreeActivity( opr_name );
					if ( !activity ) {
						PARSER->error( @2, rdo::format("Опреация '%s' не найдена", opr_name.c_str()) );
					} else {
						if ( activity->pattern()->getType() != RDOPATPattern::PT_Keyboard ) {
							PARSER->error_push_only( @2, rdo::format("Активность '%s' должна быть клавиатурной", activity->name().c_str()) );
							PARSER->error_push_only( activity->src_info(), "См. акивность" );
							PARSER->error_push_only( activity->pattern()->src_info(), "См. образец" );
							PARSER->error_push_done();
						}
					}
				} else {
					if ( opr->pattern()->getType() != RDOPATPattern::PT_Keyboard ) {
						PARSER->error_push_only( @2, rdo::format("Операция '%s' должна быть клавиатурной", opr->name().c_str()) );
						PARSER->error_push_only( opr->src_info(), "См. операцию" );
						PARSER->error_push_only( opr->pattern()->src_info(), "См. образец" );
						PARSER->error_push_done();
					}
				}
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($4);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($6);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($8);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($10);
				$$ = (int)new rdoRuntime::RDOFRMActive( RUNTIME->lastFrame(), x, y, width, height, opr_name );
			}
			| RDO_active RDO_IDENTIF '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error {
				PARSER->error( @10, "Ожидается ']'" );
			}
			| RDO_active RDO_IDENTIF '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error {
				PARSER->error( @9, @10, "Ожидается высота" );
			}
			| RDO_active RDO_IDENTIF '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error {
				PARSER->error( @8, "Ожидается запятая" );
			}
			| RDO_active RDO_IDENTIF '[' frm_position_xy ',' frm_position_xy ',' error {
				PARSER->error( @7, @8, "Ожидается ширина" );
			}
			| RDO_active RDO_IDENTIF '[' frm_position_xy ',' frm_position_xy error {
				PARSER->error( @6, "Ожидается запятая" );
			}
			| RDO_active RDO_IDENTIF '[' frm_position_xy ',' error {
				PARSER->error( @5, @6, "Ожидается координата по оси Y" );
			}
			| RDO_active RDO_IDENTIF '[' frm_position_xy error {
				PARSER->error( @4, "Ожидается запятая" );
			}
			| RDO_active RDO_IDENTIF '[' error {
				PARSER->error( @3, @4, "Ожидается координата по оси X" );
			}
			| RDO_active RDO_IDENTIF error {
				PARSER->error( @2, "Ожидается '['" );
			}
			| RDO_active error {
				PARSER->error( @1, "Ожидается имя клавиатурной операции" );
			};

// ----------------------------------------------------------------------------
// ---------- Логические выражения
// ----------------------------------------------------------------------------
fun_logic_eq: '='    { $1 = RDO_eq; }
			| RDO_eq { $1 = RDO_eq; };

fun_logic:	  fun_arithm  fun_logic_eq  fun_arithm   { $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) == *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_arithm  RDO_neq       fun_arithm   { $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) != *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_arithm  '<'           fun_arithm   { $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) <  *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_arithm  '>'           fun_arithm   { $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) >  *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_arithm  RDO_leq       fun_arithm   { $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) <= *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_arithm  RDO_geq       fun_arithm   { $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) >= *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_logic   RDO_and       fun_logic    { $$ = (int)(*reinterpret_cast<RDOFUNLogic*>($1) && *reinterpret_cast<RDOFUNLogic*>($3));   }
			| fun_logic   RDO_or        fun_logic    { $$ = (int)(*reinterpret_cast<RDOFUNLogic*>($1) || *reinterpret_cast<RDOFUNLogic*>($3));   }
			| fun_arithm                             { $$ = (int)new RDOFUNLogic( *reinterpret_cast<RDOFUNArithm*>($1) );                        }
			| fun_group
			| fun_select_logic
			| '[' fun_logic ']'
			{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setSrcPos( @1, @3 );
				logic->setSrcText( "[" + logic->src_text() + "]" );
				$$ = $2;
			}
			| '(' fun_logic ')'
			{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setSrcPos( @1, @3 );
				logic->setSrcText( "(" + logic->src_text() + ")" );
				$$ = $2;
			}
			| RDO_not fun_logic
			{
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				RDOFUNLogic* logic_not = logic->operator_not();
				logic_not->setSrcPos( @1, @2 );
				logic_not->setSrcText( "not " + logic->src_text() );
				$$ = (int)logic_not;
			}
			| '[' fun_logic error {
				PARSER->error( @2, "Ожидается закрывающаяся скобка" );
			}
			| '(' fun_logic error {
				PARSER->error( @2, "Ожидается закрывающаяся скобка" );
			};

// ----------------------------------------------------------------------------
// ---------- Арифметические выражения
// ----------------------------------------------------------------------------
fun_arithm:	  RDO_INT_CONST                 { $$ = (int)new RDOFUNArithm( PARSER, *reinterpret_cast<RDOValue*>($1) ); }
			| RDO_REAL_CONST                { $$ = (int)new RDOFUNArithm( PARSER, *reinterpret_cast<RDOValue*>($1) ); }
			| RDO_BOOL_CONST                { $$ = (int)new RDOFUNArithm( PARSER, *reinterpret_cast<RDOValue*>($1) ); }
			| RDO_STRING_CONST              { $$ = (int)new RDOFUNArithm( PARSER, *reinterpret_cast<RDOValue*>($1) ); }
			| RDO_IDENTIF                   { $$ = (int)new RDOFUNArithm( PARSER, *reinterpret_cast<RDOValue*>($1) ); }
			| RDO_IDENTIF '.' RDO_IDENTIF   { $$ = (int)new RDOFUNArithm( PARSER, *reinterpret_cast<RDOValue*>($1), *reinterpret_cast<RDOValue*>($3) ); }
			| fun_arithm '+' fun_arithm		{ $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) + *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_arithm '-' fun_arithm		{ $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) - *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_arithm '*' fun_arithm		{ $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) * *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_arithm '/' fun_arithm		{ $$ = (int)(*reinterpret_cast<RDOFUNArithm*>($1) / *reinterpret_cast<RDOFUNArithm*>($3)); }
			| fun_arithm_func_call
			| fun_select_arithm
			| '(' fun_arithm ')'
			{
				RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($2);
				arithm->setSrcPos( @1, @3 );
				arithm->setSrcText( "(" + arithm->src_text() + ")" );
				$$ = $2;
			}
			| '-' fun_arithm %prec RDO_UMINUS
			{
				RDOParserSrcInfo info;
				info.setSrcPos( @1, @2 );
				info.setSrcText( "-" + reinterpret_cast<RDOFUNArithm*>($2)->src_text() );
				$$ = (int)new RDOFUNArithm( PARSER, RDOValue(reinterpret_cast<RDOFUNArithm*>($2)->type(), info), new rdoRuntime::RDOCalcUMinus( RUNTIME, reinterpret_cast<RDOFUNArithm*>($2)->createCalc() ) );
			};

// ----------------------------------------------------------------------------
// ---------- Функции и последовательности
// ----------------------------------------------------------------------------
fun_arithm_func_call:	RDO_IDENTIF '(' ')' {
							RDOFUNParams* fun = new RDOFUNParams( PARSER );
							std::string fun_name = reinterpret_cast<RDOValue*>($1)->value().getIdentificator();
							fun->funseq_name.setSrcInfo( RDOParserSrcInfo(@1, fun_name) );
							fun->setSrcPos( @1, @3 );
							fun->setSrcText( fun_name + "()" );
							RDOFUNArithm* arithm = fun->createCall( fun_name );
							$$ = (int)arithm;
						}
						| RDO_IDENTIF '(' fun_arithm_func_call_pars ')' {
							RDOFUNParams* fun    = reinterpret_cast<RDOFUNParams*>($3);
							std::string fun_name = reinterpret_cast<RDOValue*>($1)->value().getIdentificator();
							fun->funseq_name.setSrcInfo( RDOParserSrcInfo(@1, fun_name) );
							fun->setSrcPos( @1, @4 );
							fun->setSrcText( fun_name + "(" + fun->src_text() + ")" );
							RDOFUNArithm* arithm = fun->createCall( fun_name );
							$$ = (int)arithm;
						}
						| RDO_IDENTIF '(' error {
							PARSER->error( @3, "Ошибка в параметрах функции" );
						};

fun_arithm_func_call_pars:	fun_arithm {
								RDOFUNParams* fun = new RDOFUNParams( PARSER );
								RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($1);
								fun->setSrcText( arithm->src_text() );
								fun->addParameter( arithm );
								$$ = (int)fun;
							}
							| fun_arithm_func_call_pars ',' fun_arithm {
								RDOFUNParams* fun    = reinterpret_cast<RDOFUNParams*>($1);
								RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($3);
								fun->setSrcText( fun->src_text() + ", " + arithm->src_text() );
								fun->addParameter( arithm );
								$$ = (int)fun;
							}
							| fun_arithm_func_call_pars error {
								PARSER->error( @2, "Ошибка в арифметическом выражении" );
							}
							| fun_arithm_func_call_pars ',' error {
								PARSER->error( @3, "Ошибка в арифметическом выражении" );
							};

// ----------------------------------------------------------------------------
// ---------- Групповые выражения
// ----------------------------------------------------------------------------
fun_group_keyword:	RDO_Exist			{ $$ = RDOFUNGroupLogic::fgt_exist;     }
					| RDO_Not_Exist		{ $$ = RDOFUNGroupLogic::fgt_notexist;  }
					| RDO_For_All		{ $$ = RDOFUNGroupLogic::fgt_forall;    }
					| RDO_Not_For_All	{ $$ = RDOFUNGroupLogic::fgt_notforall; };

fun_group_header:	fun_group_keyword '(' RDO_IDENTIF_COLON {
						RDOValue* type_name = reinterpret_cast<RDOValue*>($3);
						$$ = (int)(new RDOFUNGroupLogic( PARSER, (RDOFUNGroupLogic::FunGroupType)$1, type_name->src_info() ));
					}
					| fun_group_keyword '(' error {
						PARSER->error( @3, "Ожидается имя типа" );
					}
					| fun_group_keyword error {
						PARSER->error( @1, "После имени функции ожидается октрывающаяся скобка" );
					};

fun_group:			fun_group_header fun_logic ')' {
						RDOFUNGroupLogic* groupfun = reinterpret_cast<RDOFUNGroupLogic*>($1);
						groupfun->setSrcPos( @1, @3 );
						$$ = (int)groupfun->createFunLogic( reinterpret_cast<RDOFUNLogic*>($2) );
					}
					| fun_group_header RDO_NoCheck ')' {
						RDOFUNGroupLogic* groupfun = reinterpret_cast<RDOFUNGroupLogic*>($1);
						groupfun->setSrcPos( @1, @3 );
						RDOFUNLogic* trueLogic = new RDOFUNLogic( groupfun, new rdoRuntime::RDOCalcConst( RUNTIME, 1 ) );
						trueLogic->setSrcPos( @2 );
						trueLogic->setSrcText( "NoCheck" );
						$$ = (int)groupfun->createFunLogic( trueLogic );
					}
					| fun_group_header fun_logic error {
						PARSER->error( @2, "Ожидается закрывающаяся скобка" );
					}
					| fun_group_header RDO_NoCheck error {
						PARSER->error( @2, "Ожидается закрывающаяся скобка" );
					}
					| fun_group_header error {
						PARSER->error( @1, @2, "Ошибка в логическом выражении" )
					};

// ----------------------------------------------------------------------------
// ---------- Select
// ----------------------------------------------------------------------------
fun_select_header:	RDO_Select '(' RDO_IDENTIF_COLON {
						RDOValue* type_name = reinterpret_cast<RDOValue*>($3);
						RDOFUNSelect* select = new RDOFUNSelect( PARSER, type_name->src_info() );
						select->setSrcText( "Select(" + type_name->value().getIdentificator() + ": " );
						$$ = (int)select;
					}
					| RDO_Select '(' error {
						PARSER->error( @3, "Ожидается имя типа" );
					}
					| RDO_Select error {
						PARSER->error( @1, "Ожидается октрывающаяся скобка" );
					};

fun_select_body:	fun_select_header fun_logic ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						RDOFUNLogic*  flogic = reinterpret_cast<RDOFUNLogic*>($2);
						select->setSrcText( select->src_text() + flogic->src_text() + ")" );
						select->initSelect( flogic );
					}
					| fun_select_header RDO_NoCheck ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						RDOParserSrcInfo logic_info(@2, "NoCheck");
						select->setSrcText( select->src_text() + logic_info.src_text() + ")" );
						rdoRuntime::RDOCalcConst* calc_nocheck = new rdoRuntime::RDOCalcConst( RUNTIME, 1 );
						RDOFUNLogic* flogic = new RDOFUNLogic( select, calc_nocheck, true );
						flogic->setSrcInfo( logic_info );
						select->initSelect( flogic );
					}
					| fun_select_header fun_logic error {
						PARSER->error( @2, "Ожидается закрывающаяся скобка" );
					}
					| fun_select_header RDO_NoCheck error {
						PARSER->error( @2, "Ожидается закрывающаяся скобка" );
					}
					| fun_select_header error {
						PARSER->error( @1, @2, "Ошибка в логическом выражении" )
					};

fun_select_keyword:	RDO_Exist			{ $$ = RDOFUNGroupLogic::fgt_exist;     }
					| RDO_Not_Exist		{ $$ = RDOFUNGroupLogic::fgt_notexist;  }
					| RDO_For_All		{ $$ = RDOFUNGroupLogic::fgt_forall;    }
					| RDO_Not_For_All	{ $$ = RDOFUNGroupLogic::fgt_notforall; };

fun_select_logic:	fun_select_body '.' fun_select_keyword '(' fun_logic ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @6 );
						RDOFUNLogic* logic = select->createFunSelectGroup( (RDOFUNGroupLogic::FunGroupType)$3, reinterpret_cast<RDOFUNLogic*>($5) );
						$$ = (int)logic;
					}
					| fun_select_body '.' fun_select_keyword '(' error {
						PARSER->error( @4, @5, "Ошибка в логическом выражении" )
					}
					| fun_select_body '.' fun_select_keyword error {
						PARSER->error( @3, "Ожидается октрывающаяся скобка" );
					}
					| fun_select_body '.' RDO_Empty '(' ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @5 );
						RDOParserSrcInfo empty_info(@3, @5, "Empty()");
						RDOFUNLogic* logic = select->createFunSelectEmpty( empty_info );
						$$ = (int)logic;
					}
					| fun_select_body '.' RDO_Empty '(' error {
						PARSER->error( @4, "Ожидается закрывающаяся скобка" );
					}
					| fun_select_body '.' RDO_Empty error {
						PARSER->error( @3, "Ожидается октрывающаяся скобка" );
					}
					| fun_select_body '.' error {
						PARSER->error( @2, @3, "Ожидается метод списка ресурсов" );
					}
					| fun_select_body error {
						PARSER->error( @1, "Ожидается '.' (точка) для вызова метода списка ресурсов" );
					};

fun_select_arithm:	fun_select_body '.' RDO_Size '(' ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @5 );
						RDOParserSrcInfo size_info(@3, @5, "Size()");
						RDOFUNArithm* arithm = select->createFunSelectSize( size_info );
						$$ = (int)arithm;
					}
					| fun_select_body '.' RDO_Size error {
						PARSER->error( @3, "Ожидается октрывающаяся скобка" );
					}
					| fun_select_body '.' RDO_Size '(' error {
						PARSER->error( @4, "Ожидается закрывающаяся скобка" );
					};

%%

}
