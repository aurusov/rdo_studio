%{
#define YYPARSE_PARAM lexer
#define YYLEX_PARAM lexer
%}

%pure-parser

%token Resource_type		257
%token permanent_kw			258
%token Parameters			259
%token integer				260
%token real					261
%token End					262
%token temporary_kw			263
%token IDENTIF				264
%token INT_CONST			265
%token REAL_CONST			266
%token such_as				267
%token dblpoint				268
%token Resources			269
%token trace_keyword		270
%token no_trace				271
%token IDENTIF_COLON		272
%token Constant				273
%token Body					274
%token Function_keyword		275
%token Type_keyword			276
%token algorithmic			277
%token table_keyword		278
%token list_keyword			279
%token Exist				281
%token Not_Exist			282
%token For_All				283
%token Not_For_All			284
%token neq					285
%token leq					286
%token geq					287
%token NoCheck				288
%token Calculate_if			289
%token or_keyword			290
%token and_keyword			291
%token Sequence				292
%token uniform				293
%token exponential			294
%token normal_keyword		295
%token by_hist				296
%token enumerative			297

%token Pattern				298
%token operation_kw			299
%token irregular_event		300
%token rule_keyword			301
%token keyboard				302
%token Relevant_resources	303
%token Keep					304
%token Create_keyword		305
%token Erase				306
%token NonExist				307
%token IDENTIF_NoChange		308
%token Time					309
%token Choice				310
%token from_keyword			311
%token first_keyword		312
%token Convert_begin		313
%token Convert_end			314
%token Convert_rule			315
%token Convert_event		316
%token with_max				317
%token with_min				318
%token IDENTIF_set			319
%token IDENTIF_NoChange_NoChange	320
%token Operations			321
	
%token Results				322
%token watch_par			323
%token watch_state			324
%token watch_quant			325
%token watch_value			326
%token get_value			327

%token Model_name			328
%token Resource_file		329
%token OprIev_file			330
%token Frame_file			331
%token Statistic_file		332
%token Results_file			333
%token Trace_file			334
%token Show_mode			335
%token Frame_number			336
%token Show_rate			337
%token Run_StartTime		338
%token Trace_StartTime		339
%token Trace_EndTime		340
%token Terminate_if			341
%token Break_point			342
%token Seed					343
%token NoShow				344
%token Monitor				345
%token Animation			346
%token NoChange				347

%token Decision_point		348
%token search_keyword		349
%token trace_stat			350
%token trace_tops			351
%token trace_all			352
%token Condition_keyword	353
%token Term_condition		354
%token Evaluate_by			355
%token Compare_tops			356
%token NO					357
%token YES					358
%token Activities			359
%token value_before			360
%token value_after			361
%token some					362
%token Process				363
%token SEIZE				364
%token if_keyword			369
%token result_keyword		370

%token Frame				400
%token Show_if				401
%token Back_picture			402
%token Show					403
%token frm_cell				404
%token text					405
%token bitmap				406
%token s_bmp				407
%token rect_keyword			408
%token r_rect				409
%token line					410
%token ellipse				411
%token triang				412
%token active				413
%token ruler				414
%token space_kw				415
%token color_transparent_kw	416
%token color_last_kw		417
%token color_white_kw		418
%token color_black_kw		419
%token color_red_kw			420
%token color_green_kw		421
%token color_blue_kw		422
%token color_cyan_kw		423
%token color_magenta_kw		424
%token color_yellow_kw		425
%token color_gray_kw		426

%token QUOTED_IDENTIF		430
%token QUOTED_IDENTIF_BAD	431
%token IDENTIF_BAD			432
%token Select				433
%token Size_kw				434
%token Empty_kw				435
%token not_keyword			436
%token UMINUS				437

%{
#include "pch.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <FlexLexer.h>

#include "rdoparser.h"
#include "rdoparser_rdo.h"
#include "rdofun.h"
#include "rdofrm.h"
#include "rdoopr.h"
#include "rdopat.h"
#include <rdoframe.h>
#include <rdocalc.h>

namespace rdoParse 
{
%}

%left or_keyword
%left and_keyword
%left '+' '-'
%left '*' '/'
%left not_keyword
%left UMINUS

%start frm_main

%%

// ----------------------------------------------------------------------------
// ---------- Основной цикл
// ----------------------------------------------------------------------------
frm_main:	/* empty */
			| frm_main frm_end
			| error {
				parser->error( @1, "Неизвестная ошибка" );
			};


// ----------------------------------------------------------------------------
// ---------- Фрейм
// ----------------------------------------------------------------------------
frm_begin:	Frame IDENTIF {
				$$ = (int)(new rdoRuntime::RDOFRMFrame( parser->runtime, RDOParserSrcInfo(@2, *reinterpret_cast<std::string*>($2)).src_info() ));
			}
			| Frame IDENTIF Show_if fun_logic {
				$$ = (int)(new rdoRuntime::RDOFRMFrame( parser->runtime, RDOParserSrcInfo(@2, *reinterpret_cast<std::string*>($2)).src_info(), ((RDOFUNLogic *)$4)->calc) );
			}
			| Frame IDENTIF Show_if {
				parser->error( @3, "После ключевого слова Show_if ожидается логическое выражение" );
			};

frm_background:		frm_begin Back_picture '=' {
					}
					| frm_begin Back_picture '=' frm_color {
						rdoRuntime::RDOFRMFrame* frame                 = reinterpret_cast<rdoRuntime::RDOFRMFrame*>($1);
						rdoRuntime::RDOFRMFrame::RDOFRMColor* bg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($4);
						if ( bg_color->getColorType() == rdoRuntime::RDOFRMFrame::RDOFRMColor::color_transparent ) {
							parser->error( @4, "Цвет фона не может быть прозрачным" );
						} else if ( bg_color->getColorType() != rdoRuntime::RDOFRMFrame::RDOFRMColor::color_rgb ) {
							parser->error( @4, "Цвет фона не может быть указан ссылкой на последнее значение" );
						}
						frame->setBackgroundColor( bg_color );
					}
					| frm_begin Back_picture error {
						parser->error( @3, "После ключевого слова $Back_picture ожидается знак равенства" );
					}
					| frm_begin error {
						parser->error( @2, "После имени кадра ожидается ключевое слово $Back_picture" );
					};

frm_backpicture:	frm_background INT_CONST INT_CONST {
						rdoRuntime::RDOFRMFrame* frame = reinterpret_cast<rdoRuntime::RDOFRMFrame*>($1);
						frame->setBackPicture( $2, $3 );
					}
					| frm_background IDENTIF {
						rdoRuntime::RDOFRMFrame* frame = reinterpret_cast<rdoRuntime::RDOFRMFrame*>($1);
						frame->setBackPicture( *reinterpret_cast<std::string*>($2) );
					}
					| frm_background INT_CONST INT_CONST error {
						parser->error( @4, "Описание заголовка кадра окончено, ожидается ключевое слово $Show" );
					}
					| frm_background IDENTIF error {
						parser->error( @3, "Описание заголовка кадра окончено, ожидается ключевое слово $Show" );
					}
					| frm_background INT_CONST error {
						parser->error( @2, "После ширины кадра необходимо указать его высоту" );
					}
					| frm_background {
						parser->error( @1, "Необходимо указать имя фоновой картинки или размер кадра" );
					};

frm_pre_show:		frm_backpicture
					| frm_item;

frm_show:	frm_pre_show                      { ((rdoRuntime::RDOFRMFrame *)$1)->startShow();                         }
			| frm_pre_show Show               { ((rdoRuntime::RDOFRMFrame *)$1)->startShow();                         }
			| frm_pre_show Show_if fun_logic  { ((rdoRuntime::RDOFRMFrame *)$1)->startShow(((RDOFUNLogic*)$3)->calc); }
			| frm_pre_show error {
				parser->error( @2, "Ожидается описание графического элемента" );
			};

frm_item: frm_show
		  | frm_item frm_text    { ((rdoRuntime::RDOFRMFrame *)$1)->addItem((rdoRuntime::RDOFRMText *)$2); }
		  | frm_item frm_bitmap  { ((rdoRuntime::RDOFRMFrame *)$1)->addItem((rdoRuntime::RDOFRMBitmap *)$2); }
		  | frm_item frm_rect    { ((rdoRuntime::RDOFRMFrame *)$1)->addItem((rdoRuntime::RDOFRMRect *)$2); }
		  | frm_item frm_line    { ((rdoRuntime::RDOFRMFrame *)$1)->addItem((rdoRuntime::RDOFRMLine *)$2); }
		  | frm_item frm_ellipse { ((rdoRuntime::RDOFRMFrame *)$1)->addItem((rdoRuntime::RDOFRMEllipse *)$2); }
		  | frm_item frm_r_rect  { ((rdoRuntime::RDOFRMFrame *)$1)->addItem((rdoRuntime::RDOFRMRectRound *)$2); }
		  | frm_item frm_triang  { ((rdoRuntime::RDOFRMFrame *)$1)->addItem((rdoRuntime::RDOFRMTriang *)$2); }
		  | frm_item frm_s_bmp   { ((rdoRuntime::RDOFRMFrame *)$1)->addItem((rdoRuntime::RDOFRMBitmapStretch *)$2); }
		  | frm_item frm_active  { ((rdoRuntime::RDOFRMFrame *)$1)->addItem((rdoRuntime::RDOFRMActive *)$2); }
		  | frm_item frm_ruler   { ((rdoRuntime::RDOFRMFrame *)$1)->addRulet((rdoRuntime::RDOFRMFrame::RDOFRMRulet*)$2); }
		  | frm_item frm_space   { ((rdoRuntime::RDOFRMFrame *)$1)->addItem((rdoRuntime::RDOFRMSpace*)$2); };

frm_end:	frm_item End {
				reinterpret_cast<rdoRuntime::RDOFRMFrame*>($1)->end();
			};

// ----------------------------------------------------------------------------
// ---------- Элементы
// ----------------------------------------------------------------------------
frm_color:	color_transparent_kw {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( parser->runtime->lastFrame(), rdoRuntime::RDOFRMFrame::RDOFRMColor::color_transparent );
			}
			| color_last_kw {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( parser->runtime->lastFrame() );
			}
			| color_white_kw {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( parser->runtime->lastFrame(), 255, 255, 255 );
			}
			| color_black_kw {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( parser->runtime->lastFrame(), 0, 0, 0 );
			}
			| color_red_kw {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( parser->runtime->lastFrame(), 255, 0, 0 );
			}
			| color_green_kw {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( parser->runtime->lastFrame(), 0, 255, 0 );
			}
			| color_blue_kw {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( parser->runtime->lastFrame(), 0, 0, 255 );
			}
			| color_cyan_kw {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( parser->runtime->lastFrame(), 0, 255, 255 );
			}
			| color_magenta_kw {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( parser->runtime->lastFrame(), 255, 0, 255 );
			}
			| color_yellow_kw {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( parser->runtime->lastFrame(), 255, 255, 0 );
			}
			| color_gray_kw {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( parser->runtime->lastFrame(), 127, 127, 127 );
			}
			| '<' fun_arithm fun_arithm fun_arithm error {
				parser->error( @4, "Найдены все составляющие цвета, ожидается '>'" );
			}
			| '<' fun_arithm fun_arithm error {
				parser->error( @3, "Ожидается синяя составляющая цвета" );
			}
			| '<' fun_arithm error {
				parser->error( @2, "Ожидается зеленая составляющая цвета" );
			}
			| '<' error {
				parser->error( @1, "После '<' ожидается красная составляющая цвета" );
			}
			| error {
				parser->error( @1, "Ожидается цвет в формате '< r g b >', например '< 100 255 100 >'" );
			}
			| '<' fun_arithm fun_arithm fun_arithm '>' {
				RDOFUNArithm* red   = reinterpret_cast<RDOFUNArithm*>($2);
				RDOFUNArithm* green = reinterpret_cast<RDOFUNArithm*>($3);
				RDOFUNArithm* blue  = reinterpret_cast<RDOFUNArithm*>($4);
				RDORTPIntParamType intType( parser, new RDORTPIntDiap( parser, 0, 255, @1, @1 ), new RDORTPIntDefVal(parser) );
				intType.checkParamType( red );
				intType.checkParamType( green );
				intType.checkParamType( blue );
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( parser->runtime->lastFrame(), red->createCalc(), green->createCalc(), blue->createCalc() );
			}
			| {
				$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMColor( parser->runtime->lastFrame() );
			};

frm_postype:	/* empty */     { $$ = rdoRuntime::RDOFRMFrame::RDOFRMPosition::absolute; }
				| '+'           { $$ = rdoRuntime::RDOFRMFrame::RDOFRMPosition::delta;    }
				| '*'           { $$ = rdoRuntime::RDOFRMFrame::RDOFRMPosition::mult;     }
				| '#' INT_CONST {
					int rilet_id = $2;
					if ( rilet_id <= 0 ) {
						parser->error( @2, "Номер рулетки должен быть больше нуля" );
					}
					if ( !parser->runtime->lastFrame()->findRulet( rilet_id ) ) {
						parser->error( @2, rdo::format("Рулетки с номером '%d' не существует", rilet_id) );
					}
					$$ = rdoRuntime::RDOFRMFrame::RDOFRMPosition::rulet + rilet_id;
				}
				| '#' error {
					parser->error( @1, "После '#' ожидается целочисленный номер рулетки" );
				};

frm_postype_xy: frm_postype
				| '=' {
					$$ = rdoRuntime::RDOFRMFrame::RDOFRMPosition::gabarit;
				};

frm_postype_wh:	frm_postype
				| '=' {
					parser->error( @1, "Нельзя использовать данное выравнивание для ширины или высоты" );
				};

frm_position_xy: fun_arithm frm_postype_xy {
					rdoRuntime::RDOCalc* calc = reinterpret_cast<RDOFUNArithm*>($1)->createCalc();
					if ( $2 >= rdoRuntime::RDOFRMFrame::RDOFRMPosition::rulet ) {
						$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMPosition( parser->runtime->lastFrame(), calc, rdoRuntime::RDOFRMFrame::RDOFRMPosition::rulet, $2 - rdoRuntime::RDOFRMFrame::RDOFRMPosition::rulet );
					} else {
						$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMPosition( parser->runtime->lastFrame(), calc, (rdoRuntime::RDOFRMFrame::RDOFRMPosition::PositionType)$2 );
					}
				};

frm_position_wh: fun_arithm frm_postype_wh {
					rdoRuntime::RDOCalc* calc = reinterpret_cast<RDOFUNArithm*>($1)->createCalc();
					if ( $2 >= rdoRuntime::RDOFRMFrame::RDOFRMPosition::rulet ) {
						$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMPosition( parser->runtime->lastFrame(), calc, rdoRuntime::RDOFRMFrame::RDOFRMPosition::rulet, $2 - rdoRuntime::RDOFRMFrame::RDOFRMPosition::rulet );
					} else {
						$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMPosition( parser->runtime->lastFrame(), calc, (rdoRuntime::RDOFRMFrame::RDOFRMPosition::PositionType)$2 );
					}
				};

frm_ruler:		ruler '[' INT_CONST ',' frm_position_xy ',' frm_position_xy ']' {
					const rdoRuntime::RDOFRMFrame::RDOFRMRulet* rulet = parser->runtime->lastFrame()->findRulet( $3 );
					if ( rulet ) {
						parser->error_push_only( @3, rdo::format("Рулетка с номером '%d' уже существует", $3) );
						parser->error_push_only( rulet->src_info(), "См. первое определение" );
						parser->error_push_done();
					}
					rdoRuntime::RDOFRMFrame::RDOFRMPosition* x = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
					rdoRuntime::RDOFRMFrame::RDOFRMPosition* y = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
					if ( x->type != rdoRuntime::RDOFRMFrame::RDOFRMPosition::absolute ) {
						parser->error( @5, "Коодинаты рулетки должны быть абсолютными" );
					}
					if ( y->type != rdoRuntime::RDOFRMFrame::RDOFRMPosition::absolute ) {
						parser->error( @7, "Коодинаты рулетки должны быть абсолютными" );
					}
					$$ = (int)new rdoRuntime::RDOFRMFrame::RDOFRMRulet( RDOParserSrcInfo(@1), $3, x, y );
				}
				| ruler '[' INT_CONST ',' frm_position_xy ',' frm_position_xy error {
					parser->error( @7, "Ожидается ']'" );
				}
				| ruler '[' INT_CONST ',' frm_position_xy ',' error {
					parser->error( @6, @7, "Ожидается координата по оси Y" );
				}
				| ruler '[' INT_CONST ',' frm_position_xy error {
					parser->error( @5, "Ожидается запятая" );
				}
				| ruler '[' INT_CONST ',' error {
					parser->error( @4, @5, "Ожидается координата по оси X" );
				}
				| ruler '[' INT_CONST error {
					parser->error( @3, "Ожидается запятая" );
				}
				| ruler '[' error {
					parser->error( @2, @3, "Ожидается номер рулетки" );
				}
				| ruler error {
					parser->error( @1, "Ожидается '['" );
				};

frm_space:	space_kw '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
				$$ = (int)new rdoRuntime::RDOFRMSpace( parser->runtime->lastFrame(), x, y, width, height );
			}
			| space_kw '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error {
				parser->error( @9, "Ожидается ']'" );
			}
			| space_kw '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error {
				parser->error( @8, @9, "Ожидается высота" );
			}
			| space_kw '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error {
				parser->error( @7, "Ожидается запятая" );
			}
			| space_kw '[' frm_position_xy ',' frm_position_xy ',' error {
				parser->error( @6, @7, "Ожидается ширина" );
			}
			| space_kw '[' frm_position_xy ',' frm_position_xy error {
				parser->error( @5, "Ожидается запятая" );
			}
			| space_kw '[' frm_position_xy ',' error {
				parser->error( @4, @5, "Ожидается координата по оси Y" );
			}
			| space_kw '[' frm_position_xy error {
				parser->error( @3, "Ожидается запятая" );
			}
			| space_kw '[' error {
				parser->error( @2, @3, "Ожидается координата по оси X" );
			}
			| space_kw error {
				parser->error( @1, "Ожидается '['" );
			};

frm_text_align: /* empty */ { $$ = rdoSimulator::RDOTextElement::left;   }
				| '<'       { $$ = rdoSimulator::RDOTextElement::left;   }
				| '='       { $$ = rdoSimulator::RDOTextElement::center; }
				| '>'       { $$ = rdoSimulator::RDOTextElement::right;  };

frm_text_common:	text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color ',' {
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
						rdoRuntime::RDOFRMFrame::RDOFRMColor* bg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($11);
						rdoRuntime::RDOFRMFrame::RDOFRMColor* fg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($13);
						bg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg_text );
						fg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg_text );
						$$ = (int)new rdoRuntime::RDOFRMText( parser->runtime->lastFrame(), x, y, width, height, bg_color, fg_color );
					}
					| text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' {
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
						rdoRuntime::RDOFRMFrame::RDOFRMColor* bg_color = new rdoRuntime::RDOFRMFrame::RDOFRMColor( parser->runtime->lastFrame() );
						rdoRuntime::RDOFRMFrame::RDOFRMColor* fg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($11);
						bg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg_text );
						fg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg_text );
						$$ = (int)new rdoRuntime::RDOFRMText( parser->runtime->lastFrame(), x, y, width, height, bg_color, fg_color );
					}
					| text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' {
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
						rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
						rdoRuntime::RDOFRMFrame::RDOFRMColor* bg_color = new rdoRuntime::RDOFRMFrame::RDOFRMColor( parser->runtime->lastFrame() );
						rdoRuntime::RDOFRMFrame::RDOFRMColor* fg_color = new rdoRuntime::RDOFRMFrame::RDOFRMColor( parser->runtime->lastFrame() );
						bg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg_text );
						fg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg_text );
						$$ = (int)new rdoRuntime::RDOFRMText( parser->runtime->lastFrame(), x, y, width, height, bg_color, fg_color );
					}
					| text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color error {
						parser->error( @13, "Ожидается запятая" );
					}
					| text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' error {
						parser->error( @12, @13, "Ожидается цвет текста" );
					}
					| text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color error {
						parser->error( @11, "Ожидается запятая" );
					}
					| text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' error {
						parser->error( @10, @11, "Ожидается цвет фона" );
					}
					| text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error {
						parser->error( @9, "Ожидается запятая" );
					}
					| text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error {
						parser->error( @8, @9, "Ожидается высота" );
					}
					| text '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error {
						parser->error( @7, "Ожидается запятая" );
					}
					| text '[' frm_position_xy ',' frm_position_xy ',' error {
						parser->error( @6, @7, "Ожидается ширина" );
					}
					| text '[' frm_position_xy ',' frm_position_xy error {
						parser->error( @5, "Ожидается запятая" );
					}
					| text '[' frm_position_xy ',' error {
						parser->error( @4, @5, "Ожидается координата по оси Y" );
					}
					| text '[' frm_position_xy error {
						parser->error( @3, "Ожидается запятая" );
					}
					| text '[' error {
						parser->error( @2, @3, "Ожидается координата по оси X" );
					}
					| text error {
						parser->error( @1, "Ожидается '['" );
					};

frm_text:	frm_text_common frm_text_align fun_arithm ']' {
				((rdoRuntime::RDOFRMText *)$1)->setText( (rdoSimulator::RDOTextElement::RDOTextAlign)$2, ((RDOFUNArithm *)$3)->createCalc(), NULL /*//qq((RDOFUNArithm *)$3)->enu*/ );
			}
			| frm_text_common frm_text_align QUOTED_IDENTIF ']' {
				((rdoRuntime::RDOFRMText *)$1)->setText( (rdoSimulator::RDOTextElement::RDOTextAlign)$2, (std::string *)$3);
			}
			| frm_text_common frm_text_align fun_arithm error {
				parser->error( @3, "Ожидается ']'" );
			}
			| frm_text_common frm_text_align QUOTED_IDENTIF error {
				parser->error( @3, "Ожидается ']'" );
			}
			| frm_text_common frm_text_align error {
				parser->error( @2, "Ожидается параметр ресурса или строка" );
			};

frm_bitmap:	bitmap '[' frm_position_xy ',' frm_position_xy ',' IDENTIF ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				$$ = (int)new rdoRuntime::RDOFRMBitmap( parser->runtime->lastFrame(), x, y, *reinterpret_cast<std::string*>($7) );
			}
			| bitmap '[' frm_position_xy ',' frm_position_xy ',' IDENTIF ',' IDENTIF ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				$$ = (int)new rdoRuntime::RDOFRMBitmap( parser->runtime->lastFrame(), x, y, *reinterpret_cast<std::string*>($7), *reinterpret_cast<std::string*>($9) );
			}
			| bitmap '[' frm_position_xy ',' frm_position_xy ',' IDENTIF ',' IDENTIF error  {
				parser->error( @9, "Ожидается ']'" );
			}
			| bitmap '[' frm_position_xy ',' frm_position_xy ',' IDENTIF ',' error  {
				parser->error( @8, @9, "Ожидается имя маски для картинки" );
			}
			| bitmap '[' frm_position_xy ',' frm_position_xy ',' IDENTIF error  {
				parser->error( @7, "Ожидается ']'" );
			}
			| bitmap '[' frm_position_xy ',' frm_position_xy ',' error  {
				parser->error( @6, @7, "Ожидается имя картинки" );
			}
			| bitmap '[' frm_position_xy ',' frm_position_xy error  {
				parser->error( @5, "Ожидается запятая" );
			}
			| bitmap '[' frm_position_xy ',' error  {
				parser->error( @4, @5, "Ожидается координата по оси Y" );
			}
			| bitmap '[' frm_position_xy error  {
				parser->error( @3, "Ожидается запятая" );
			}
			| bitmap '[' error  {
				parser->error( @2, @3, "Ожидается координата по оси X" );
			}
			| bitmap error  {
				parser->error( @1, "Ожидается '['" );
			};

frm_s_bmp:	s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' IDENTIF ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
				$$ = (int)new rdoRuntime::RDOFRMBitmapStretch( parser->runtime->lastFrame(), x, y, width, height, *reinterpret_cast<std::string*>($11) );
			}
			| s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' IDENTIF ',' IDENTIF ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
				$$ = (int)new rdoRuntime::RDOFRMBitmapStretch( parser->runtime->lastFrame(), x, y, width, height, *reinterpret_cast<std::string*>($11), *reinterpret_cast<std::string*>($13) );
			}
			| s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' IDENTIF ',' IDENTIF error {
				parser->error( @13, "Ожидается ']'" );
			}
			| s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' IDENTIF ',' error {
				parser->error( @12, @13, "Ожидается имя маски для картинки" );
			}
			| s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' IDENTIF error {
				parser->error( @11, "Ожидается ']'" );
			}
			| s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' error {
				parser->error( @10, @11, "Ожидается имя картинки" );
			}
			| s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error {
				parser->error( @9, "Ожидается запятая" );
			}
			| s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error {
				parser->error( @8, @9, "Ожидается высота" );
			}
			| s_bmp '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error {
				parser->error( @7, "Ожидается запятая" );
			}
			| s_bmp '[' frm_position_xy ',' frm_position_xy ',' error {
				parser->error( @6, @7, "Ожидается ширина" );
			}
			| s_bmp '[' frm_position_xy ',' frm_position_xy error {
				parser->error( @5, "Ожидается запятая" );
			}
			| s_bmp '[' frm_position_xy ',' error {
				parser->error( @4, @5, "Ожидается координата по оси Y" );
			}
			| s_bmp '[' frm_position_xy error {
				parser->error( @3, "Ожидается запятая" );
			}
			| s_bmp '[' error {
				parser->error( @2, @3, "Ожидается координата по оси X" );
			}
			| s_bmp error {
				parser->error( @1, "Ожидается '['" );
			};

frm_rect:	rect_keyword '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
				rdoRuntime::RDOFRMFrame::RDOFRMColor* bg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($11);
				rdoRuntime::RDOFRMFrame::RDOFRMColor* fg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($13);
				bg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg );
				fg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg );
				$$ = (int)new rdoRuntime::RDOFRMRect( parser->runtime->lastFrame(), x, y, width, height, bg_color, fg_color );
			}
			| rect_keyword '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
				rdoRuntime::RDOFRMFrame::RDOFRMColor* bg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($11);
				rdoRuntime::RDOFRMFrame::RDOFRMColor* fg_color = new rdoRuntime::RDOFRMFrame::RDOFRMColor( parser->runtime->lastFrame() );
				bg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg );
				fg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg );
				$$ = (int)new rdoRuntime::RDOFRMRect( parser->runtime->lastFrame(), x, y, width, height, bg_color, fg_color );
			}
			| rect_keyword '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color error {
				parser->error( @13, "Ожидается ']'" );
			}
			| rect_keyword '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' error {
				parser->error( @12, @13, "Ожидается цвет линии прямоугольника" );
			}
			| rect_keyword '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color error {
				parser->error( @11, "Ожидается запятая" );
			}
			| rect_keyword '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' error {
				parser->error( @10, @11, "Ожидается цвет фона" );
			}
			| rect_keyword '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error {
				parser->error( @9, "Ожидается запятая" );
			}
			| rect_keyword '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error {
				parser->error( @8, @9, "Ожидается высота" );
			}
			| rect_keyword '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error {
				parser->error( @7, "Ожидается запятая" );
			}
			| rect_keyword '[' frm_position_xy ',' frm_position_xy ',' error {
				parser->error( @6, @7, "Ожидается ширина" );
			}
			| rect_keyword '[' frm_position_xy ',' frm_position_xy error {
				parser->error( @5, "Ожидается запятая" );
			}
			| rect_keyword '[' frm_position_xy ',' error {
				parser->error( @4, @5, "Ожидается координата по оси Y" );
			}
			| rect_keyword '[' frm_position_xy error {
				parser->error( @3, "Ожидается запятая" );
			}
			| rect_keyword '[' error {
				parser->error( @2, @3, "Ожидается координата по оси X" );
			}
			| rect_keyword error {
				parser->error( @1, "Ожидается '['" );
			};

frm_r_rect:	r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
				rdoRuntime::RDOFRMFrame::RDOFRMColor* bg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($11);
				rdoRuntime::RDOFRMFrame::RDOFRMColor* fg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($13);
				bg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg );
				fg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg );
				$$ = (int)new rdoRuntime::RDOFRMRectRound( parser->runtime->lastFrame(), x, y, width, height, bg_color, fg_color );
			}
			| r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color error {
				parser->error( @13, "Ожидается ']'" );
			}
			| r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' error {
				parser->error( @12, @13, "Ожидается цвет линии прямоугольника" );
			}
			| r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color error {
				parser->error( @11, "Ожидается запятая" );
			}
			| r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' error {
				parser->error( @10, @11, "Ожидается цвет фона" );
			}
			| r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error {
				parser->error( @9, "Ожидается запятая" );
			}
			| r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error {
				parser->error( @8, @9, "Ожидается высота" );
			}
			| r_rect '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error {
				parser->error( @7, "Ожидается запятая" );
			}
			| r_rect '[' frm_position_xy ',' frm_position_xy ',' error {
				parser->error( @6, @7, "Ожидается ширина" );
			}
			| r_rect '[' frm_position_xy ',' frm_position_xy error {
				parser->error( @5, "Ожидается запятая" );
			}
			| r_rect '[' frm_position_xy ',' error {
				parser->error( @4, @5, "Ожидается координата по оси Y" );
			}
			| r_rect '[' frm_position_xy error {
				parser->error( @3, "Ожидается запятая" );
			}
			| r_rect '[' error {
				parser->error( @2, @3, "Ожидается координата по оси X" );
			}
			| r_rect error {
				parser->error( @1, "Ожидается '['" );
			};

frm_ellipse: ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
				rdoRuntime::RDOFRMFrame::RDOFRMColor* bg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($11);
				rdoRuntime::RDOFRMFrame::RDOFRMColor* fg_color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($13);
				bg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_bg );
				fg_color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg );
				$$ = (int)new rdoRuntime::RDOFRMEllipse( parser->runtime->lastFrame(), x, y, width, height, bg_color, fg_color );
			}
			| ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' frm_color error {
				parser->error( @13, "Ожидается ']'" );
			}
			| ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color ',' error {
				parser->error( @12, @13, "Ожидается цвет линии эллипса" );
			}
			| ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' frm_color error {
				parser->error( @11, "Ожидается запятая" );
			}
			| ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ',' error {
				parser->error( @10, @11, "Ожидается цвет фона" );
			}
			| ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error {
				parser->error( @9, "Ожидается запятая" );
			}
			| ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error {
				parser->error( @8, @9, "Ожидается высота" );
			}
			| ellipse '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error {
				parser->error( @7, "Ожидается запятая" );
			}
			| ellipse '[' frm_position_xy ',' frm_position_xy ',' error {
				parser->error( @6, @7, "Ожидается ширина" );
			}
			| ellipse '[' frm_position_xy ',' frm_position_xy error {
				parser->error( @5, "Ожидается запятая" );
			}
			| ellipse '[' frm_position_xy ',' error {
				parser->error( @4, @5, "Ожидается координата по оси Y" );
			}
			| ellipse '[' frm_position_xy error {
				parser->error( @3, "Ожидается запятая" );
			}
			| ellipse '[' error {
				parser->error( @2, @3, "Ожидается координата по оси X" );
			}
			| ellipse error {
				parser->error( @1, "Ожидается '['" );
			};

frm_line:	line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color ']' {
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($3);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($5);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($7);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($9);
				rdoRuntime::RDOFRMFrame::RDOFRMColor* color = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMColor*>($11);
				color->setColorType( rdoRuntime::RDOFRMFrame::RDOFRMColor::color_last_fg );
				$$ = (int)new rdoRuntime::RDOFRMLine( parser->runtime->lastFrame(), x, y, width, height, color );
			}
			| line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color error {
				parser->error( @11, "Ожидается ']'" );
			}
			| line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error {
				parser->error( @10, @11, "Ожидается цвет линии" );
			}
			| line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy error {
				parser->error( @9, "Ожидается запятая" );
			}
			| line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error {
				parser->error( @8, @9, "Ожидается Y2" );
			}
			| line '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy error {
				parser->error( @7, "Ожидается запятая" );
			}
			| line '[' frm_position_xy ',' frm_position_xy ',' error {
				parser->error( @6, @7, "Ожидается X2" );
			}
			| line '[' frm_position_xy ',' frm_position_xy error {
				parser->error( @5, "Ожидается запятая" );
			}
			| line '[' frm_position_xy ',' error {
				parser->error( @4, @5, "Ожидается Y1" );
			}
			| line '[' frm_position_xy error {
				parser->error( @3, "Ожидается запятая" );
			}
			| line '[' error {
				parser->error( @2, @3, "Ожидается X1" );
			}
			| line error {
				parser->error( @1, "Ожидается '['" );
			};

frm_triang:	triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color ',' frm_color ']' {
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
				$$ = (int)new rdoRuntime::RDOFRMTriang( parser->runtime->lastFrame(), x1, y1, x2, y2, x3, y3, bg_color, fg_color );
			}
			| triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color ',' frm_color error {
				parser->error( @17, "Ожидается ']'" );
			}
			| triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color ',' error {
				parser->error( @16, @17, "Ожидается цвет линии" );
			}
			| triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_color error {
				parser->error( @15, "Ожидается запятая" );
			}
			| triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error {
				parser->error( @14, @15, "Ожидается цвет треугольника" );
			}
			| triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy error {
				parser->error( @13, "Ожидается запятая" );
			}
			| triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error {
				parser->error( @12, @13, "Ожидается Y3" );
			}
			| triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy error {
				parser->error( @11, "Ожидается запятая" );
			}
			| triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error {
				parser->error( @10, @11, "Ожидается X3" );
			}
			| triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' frm_position_xy error {
				parser->error( @9, "Ожидается запятая" );
			}
			| triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy ',' error {
				parser->error( @8, @9, "Ожидается Y2" );
			}
			| triang '[' frm_position_xy ',' frm_position_xy ',' frm_position_xy error {
				parser->error( @7, "Ожидается запятая" );
			}
			| triang '[' frm_position_xy ',' frm_position_xy ',' error {
				parser->error( @6, @7, "Ожидается X2" );
			}
			| triang '[' frm_position_xy ',' frm_position_xy error {
				parser->error( @5, "Ожидается запятая" );
			}
			| triang '[' frm_position_xy ',' error {
				parser->error( @4, @5, "Ожидается Y1" );
			}
			| triang '[' frm_position_xy error {
				parser->error( @3, "Ожидается запятая" );
			}
			| triang '[' error {
				parser->error( @2, @3, "Ожидается X1" );
			}
			| triang error {
				parser->error( @1, "Ожидается '['" );
			};

frm_active:	active IDENTIF '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh ']' {
				std::string opr_name = *reinterpret_cast<std::string*>($2);
				const RDOOPROperation* opr = parser->findOperation( opr_name );
				if ( !opr ) {
					parser->error( @2, rdo::format("Опреация '%s' не найдена", opr_name.c_str()) );
				}
				if ( opr->getType()->getPatType() != RDOPATPattern::PT_Keyboard ) {
					parser->error_push_only( @2, rdo::format("Операция '%s' должна быть клавиатурной", opr->getName().c_str()) );
					parser->error_push_only( opr->src_info(), "См. операцию" );
					parser->error_push_only( opr->getType()->src_info(), "См. образец" );
					parser->error_push_done();
				}
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* x      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($4);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* y      = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($6);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* width  = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($8);
				rdoRuntime::RDOFRMFrame::RDOFRMPosition* height = reinterpret_cast<rdoRuntime::RDOFRMFrame::RDOFRMPosition*>($10);
				$$ = (int)new rdoRuntime::RDOFRMActive( parser->runtime->lastFrame(), x, y, width, height, opr_name );
			}
			| active IDENTIF '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' frm_position_wh error {
				parser->error( @10, "Ожидается ']'" );
			}
			| active IDENTIF '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh ',' error {
				parser->error( @9, @10, "Ожидается высота" );
			}
			| active IDENTIF '[' frm_position_xy ',' frm_position_xy ',' frm_position_wh error {
				parser->error( @8, "Ожидается запятая" );
			}
			| active IDENTIF '[' frm_position_xy ',' frm_position_xy ',' error {
				parser->error( @7, @8, "Ожидается ширина" );
			}
			| active IDENTIF '[' frm_position_xy ',' frm_position_xy error {
				parser->error( @6, "Ожидается запятая" );
			}
			| active IDENTIF '[' frm_position_xy ',' error {
				parser->error( @5, @6, "Ожидается координата по оси Y" );
			}
			| active IDENTIF '[' frm_position_xy error {
				parser->error( @4, "Ожидается запятая" );
			}
			| active IDENTIF '[' error {
				parser->error( @3, @4, "Ожидается координата по оси X" );
			}
			| active IDENTIF error {
				parser->error( @2, "Ожидается '['" );
			}
			| active error {
				parser->error( @1, "Ожидается имя клавиатурной операции" );
			};

// ----------------------------------------------------------------------------
// ---------- Логические выражения
// ----------------------------------------------------------------------------
fun_logic:	fun_arithm '=' fun_arithm         { $$ = (int)(*(RDOFUNArithm *)$1 == *(RDOFUNArithm *)$3); }
			| fun_arithm neq fun_arithm       { $$ = (int)(*(RDOFUNArithm *)$1 != *(RDOFUNArithm *)$3); }
			| fun_arithm '<' fun_arithm       { $$ = (int)(*(RDOFUNArithm *)$1 <  *(RDOFUNArithm *)$3); }
			| fun_arithm '>' fun_arithm       { $$ = (int)(*(RDOFUNArithm *)$1 >  *(RDOFUNArithm *)$3); }
			| fun_arithm leq fun_arithm       { $$ = (int)(*(RDOFUNArithm *)$1 <= *(RDOFUNArithm *)$3); }
			| fun_arithm geq fun_arithm       { $$ = (int)(*(RDOFUNArithm *)$1 >= *(RDOFUNArithm *)$3); }
			| fun_logic and_keyword fun_logic { $$ = (int)(*(RDOFUNLogic *)$1 && *(RDOFUNLogic *)$3);   }
			| fun_logic or_keyword fun_logic  { $$ = (int)(*(RDOFUNLogic *)$1 || *(RDOFUNLogic *)$3);   }
			| '[' fun_logic ']' {
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setSrcPos( @1, @3 );
				logic->setSrcText( "[" + logic->src_text() + "]" );
				$$ = $2;
			}
			| '(' fun_logic ')' {
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				logic->setSrcPos( @1, @3 );
				logic->setSrcText( "(" + logic->src_text() + ")" );
				$$ = $2;
			}
			| not_keyword fun_logic {
				RDOFUNLogic* logic = reinterpret_cast<RDOFUNLogic*>($2);
				RDOFUNLogic* logic_not = logic->operator_not();
				logic_not->setSrcPos( @1, @2 );
				logic_not->setSrcText( "not " + logic->src_text() );
				$$ = (int)logic_not;
			}
			| fun_group {
			}
			| fun_select_logic {
			}
			| '[' fun_logic error {
				parser->error( @2, "Ожидается закрывающаяся скобка" );
			}
			| '(' fun_logic error {
				parser->error( @2, "Ожидается закрывающаяся скобка" );
			}
			| error {
				parser->error( @1, "Ошибка в логическом выражении" );
			};

// ----------------------------------------------------------------------------
// ---------- Арифметические выражения
// ----------------------------------------------------------------------------
fun_arithm: fun_arithm '+' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 + *(RDOFUNArithm *)$3); }
			| fun_arithm '-' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 - *(RDOFUNArithm *)$3); }
			| fun_arithm '*' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 * *(RDOFUNArithm *)$3); }
			| fun_arithm '/' fun_arithm		{ $$ = (int)(*(RDOFUNArithm *)$1 / *(RDOFUNArithm *)$3); }
			| '(' fun_arithm ')' {
				RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($2);
				arithm->setSrcPos( @1, @3 );
				arithm->setSrcText( "(" + arithm->src_text() + ")" );
				$$ = $2;
			}
			| fun_arithm_func_call {
			}
			| fun_select_arithm {
			}
			| IDENTIF '.' IDENTIF {
				$$ = (int)new RDOFUNArithm( parser, RDOParserSrcInfo( @1, *reinterpret_cast<std::string*>($1) ), RDOParserSrcInfo( @3, *reinterpret_cast<std::string*>($3) ) );
			}
			| INT_CONST                   { $$ = (int)new RDOFUNArithm( parser, (int)$1, RDOParserSrcInfo( @1, reinterpret_cast<RDOLexer*>(lexer)->YYText() ) );     }
			| REAL_CONST                  { $$ = (int)new RDOFUNArithm( parser, (double*)$1, RDOParserSrcInfo( @1, reinterpret_cast<RDOLexer*>(lexer)->YYText() ) ); }
			| IDENTIF                     { $$ = (int)new RDOFUNArithm( parser, *(std::string*)$1, @1 );                                                             }
			| '-' fun_arithm %prec UMINUS {
				RDOParserSrcInfo info;
				info.setSrcPos( @1, @2 );
				info.setSrcText( "-" + reinterpret_cast<RDOFUNArithm*>($2)->src_text() );
				$$ = (int)new RDOFUNArithm( parser, reinterpret_cast<RDOFUNArithm*>($2)->getType(), new rdoRuntime::RDOCalcUMinus( parser->runtime, reinterpret_cast<RDOFUNArithm*>($2)->createCalc() ), info );
			}
			| error {
				if ( @1.first_line = @1.last_line ) {
					std::string str = reinterpret_cast<RDOLexer*>(lexer)->YYText();
					if ( str.length() == 1 && str.find_first_of("!#`~@$%^&|:(),=[].*><+-/\\") != std::string::npos ) {
						parser->error( @1, rdo::format("Неизвестный символ: %s", str.c_str()) );
					} else {
						parser->error( @1, rdo::format("Неизвестный идентификатор: %s", str.c_str()) );
					}
				} else {
					parser->error( @1, "Ошибка в арифметическом выражении" );
				}
			};

// ----------------------------------------------------------------------------
// ---------- Функции и последовательности
// ----------------------------------------------------------------------------
fun_arithm_func_call:	IDENTIF '(' fun_arithm_func_call_pars ')' {
							RDOFUNParams* fun    = reinterpret_cast<RDOFUNParams*>($3);
							std::string fun_name = *reinterpret_cast<std::string*>($1);
							fun->funseq_name.setSrcInfo( RDOParserSrcInfo(@1, fun_name) );
							fun->setSrcPos( @1, @4 );
							fun->setSrcText( fun_name + "(" + fun->src_text() + ")" );
							RDOFUNArithm* arithm = fun->createCall( fun_name );
							$$ = (int)arithm;
						}
						| IDENTIF '(' error {
							parser->error( @3, "Ошибка в параметрах функции" );
						};

fun_arithm_func_call_pars:	/* empty */ {
								RDOFUNParams* fun = new RDOFUNParams( parser );
								$$ = (int)fun;
							}
							| fun_arithm_func_call_pars fun_arithm {
								RDOFUNParams* fun    = reinterpret_cast<RDOFUNParams*>($1);
								RDOFUNArithm* arithm = reinterpret_cast<RDOFUNArithm*>($2);
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
							};

// ----------------------------------------------------------------------------
// ---------- Групповые выражения
// ----------------------------------------------------------------------------
fun_group_keyword:	Exist			{ $$ = 1; }
					| Not_Exist		{ $$ = 2; }
					| For_All		{ $$ = 3; }
					| Not_For_All	{ $$ = 4; };

fun_group_header:	fun_group_keyword '(' IDENTIF_COLON {
						std::string type_name = *reinterpret_cast<std::string*>($3);
						$$ = (int)(new RDOFUNGroupLogic( parser, $1, RDOParserSrcInfo(@3, type_name, RDOParserSrcInfo::psi_align_bytext) ));
					}
					| fun_group_keyword '(' error {
						parser->error( @3, "Ожидается имя типа" );
					}
					| fun_group_keyword error {
						parser->error( @1, "После имени функции ожидается октрывающаяся скобка" );
					};

fun_group:			fun_group_header fun_logic ')' {
						RDOFUNGroupLogic* groupfun = reinterpret_cast<RDOFUNGroupLogic*>($1);
						groupfun->setSrcPos( @1, @3 );
						$$ = (int)groupfun->createFunLogic( reinterpret_cast<RDOFUNLogic*>($2) );
					}
					| fun_group_header NoCheck ')' {
						RDOFUNGroupLogic* groupfun = reinterpret_cast<RDOFUNGroupLogic*>($1);
						groupfun->setSrcPos( @1, @3 );
						RDOFUNLogic* trueLogic = new RDOFUNLogic( groupfun, new rdoRuntime::RDOCalcConst( parser->runtime, 1 ) );
						trueLogic->setSrcPos( @2 );
						trueLogic->setSrcText( "NoCheck" );
						$$ = (int)groupfun->createFunLogic( trueLogic );
					}
					| fun_group_header fun_logic error {
						parser->error( @2, "Ожидается закрывающаяся скобка" );
					}
					| fun_group_header NoCheck error {
						parser->error( @2, "Ожидается закрывающаяся скобка" );
					};

// ----------------------------------------------------------------------------
// ---------- Select
// ----------------------------------------------------------------------------
fun_select_header:	Select '(' IDENTIF_COLON {
						std::string type_name = *reinterpret_cast<std::string*>($3);
						RDOFUNSelect* select = new RDOFUNSelect( parser, RDOParserSrcInfo(@3, type_name, RDOParserSrcInfo::psi_align_bytext) );
						select->setSrcText( "Select(" + type_name + ": " );
						$$ = (int)select;
					}
					| Select '(' error {
						parser->error( @3, "Ожидается имя типа" );
					}
					| Select error {
						parser->error( @1, "Ожидается октрывающаяся скобка" );
					};

fun_select_body:	fun_select_header fun_logic ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						RDOFUNLogic*  flogic = reinterpret_cast<RDOFUNLogic*>($2);
						select->setSrcText( select->src_text() + flogic->src_text() + ")" );
						select->initSelect( flogic );
					}
					| fun_select_header NoCheck ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						RDOParserSrcInfo logic_info(@2, "NoCheck");
						select->setSrcText( select->src_text() + logic_info.src_text() + ")" );
						rdoRuntime::RDOCalcConst* calc_nocheck = new rdoRuntime::RDOCalcConst( parser->runtime, 1 );
						RDOFUNLogic* flogic = new RDOFUNLogic( select, calc_nocheck, true );
						flogic->setSrcInfo( logic_info );
						select->initSelect( flogic );
					}
					| fun_select_header fun_logic error {
						parser->error( @2, "Ожидается закрывающаяся скобка" );
					}
					| fun_select_header NoCheck error {
						parser->error( @2, "Ожидается закрывающаяся скобка" );
					};

fun_select_keyword:	Exist			{ $$ = 1; }
					| Not_Exist		{ $$ = 2; }
					| For_All		{ $$ = 3; }
					| Not_For_All	{ $$ = 4; };

fun_select_logic:	fun_select_body '.' fun_select_keyword '(' fun_logic ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @6 );
						RDOFUNLogic* logic = select->createFunSelectGroup( $3, reinterpret_cast<RDOFUNLogic*>($5) );
						$$ = (int)logic;
					}
					| fun_select_body '.' Empty_kw '(' ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @5 );
						RDOParserSrcInfo empty_info(@3, @5, "Empty()");
						RDOFUNLogic* logic = select->createFunSelectEmpty( empty_info );
						$$ = (int)logic;
					}
					| fun_select_body error {
						parser->error( @1, "Ожидается '.' (точка) для вызова метода списка ресурсов" );
					}
					| fun_select_body '.' error {
						parser->error( @2, @3, "Ожидается метод списка ресурсов" );
					}
					| fun_select_body '.' fun_select_keyword error {
						parser->error( @3, "Ожидается октрывающаяся скобка" );
					}
					| fun_select_body '.' Empty_kw error {
						parser->error( @3, "Ожидается октрывающаяся скобка" );
					}
					| fun_select_body '.' fun_select_keyword '(' error ')' {
						parser->error( @5, "Ошибка в логическом выражении" );
					}
					| fun_select_body '.' Empty_kw '(' error {
						parser->error( @4, "Ожидается закрывающаяся скобка" );
					};

fun_select_arithm:	fun_select_body '.' Size_kw '(' ')' {
						RDOFUNSelect* select = reinterpret_cast<RDOFUNSelect*>($1);
						select->setSrcPos( @1, @5 );
						RDOParserSrcInfo size_info(@3, @5, "Size()");
						RDOFUNArithm* arithm = select->createFunSelectSize( size_info );
						$$ = (int)arithm;
					}
					| fun_select_body error {
						parser->error( @1, "Ожидается '.' (точка) для вызова метода списка ресурсов" );
					}
					| fun_select_body '.' error {
						parser->error( @2, @3, "Ожидается метод списка ресурсов: Size()" );
					}
					| fun_select_body '.' Size_kw error {
						parser->error( @3, "Ожидается октрывающаяся скобка" );
					}
					| fun_select_body '.' Size_kw '(' error {
						parser->error( @4, "Ожидается закрывающаяся скобка" );
					};

%%

}
