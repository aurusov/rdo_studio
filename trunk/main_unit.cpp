//---------------------------------------------------------------------------
const max_vire_count = 100;

#include <vcl.h>
#include <stdio.h>
#include <string.h>
#include <process.h>
#include <vector>
#include <list>
#pragma hdrstop

#include "main_unit.h"
#include "res_const.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma resource "*.dfm"
TForm1 *Form1;
typedef struct {
    int top_sign;
    int top_number;
    int number_parent_top;
    int way_cost;
    int remainded_way_cost;
    int action_number;
} graph_tops;

typedef struct {
    int top_number;
    int action_number;
} best_ways;

struct fixsed_points {
    int pos_x, pos_y, pos_x_end, pos_y_end;
    bool operator< ( const fixsed_points& line ) const {
    	return sqrt( pow(pos_x - pos_x_end, 2) + pow(pos_y - pos_y_end, 2) ) <
               sqrt( pow(line.pos_x - line.pos_x_end, 2) + pow(line.pos_y - line.pos_y_end, 2) );
    }
    AnsiString toString() const {
    	return AnsiString( "(" + IntToStr(pos_x) + ", " +
		                         IntToStr(pos_y) + ") -> (" +
		                         IntToStr(pos_x_end) + ", " +
		                         IntToStr(pos_y_end) + ")" );
    }
};

//graph_tops wires[max_vire_count][50000];
graph_tops wires[max_vire_count][50];
//best_ways wire_way[max_vire_count][50];
fixsed_points temp_point;
std::vector< fixsed_points > wire_points;
std::vector< std::vector<best_ways> > wire_way;
int all_tops[max_vire_count];
int field_width,field_height;
int scale;
int radius;
int working_mode=0;
bool wasRun = false;
const int border_x = 10;
const int border_y = 10;

bool start_=false,end_=false;
randomize();
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TForm1::FormActivate(TObject *Sender)
{
    field_width  = CSpinEdit1->Value;
    field_height = CSpinEdit2->Value;
    scale = CSpinEdit3->Value;
    radius = scale / 5;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PaintBox1MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{   int x_,y_;
    x_= (X + scale/2) / scale + 1;
    y_= (Y + scale/2) / scale + 1;
    if(x_<= field_width && y_<=field_height) Form1->Caption = "Координаты точки     X=" + IntToStr(x_) + "   Y=" + IntToStr(y_);
    else Form1->Caption = "Координаты точки вне поля";
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PaintBox1MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if ( wasRun ) return;
    int x_,y_;
    x_= (X - border_x + scale/2) / scale + 1;
    y_= (Y - border_y + scale/2) / scale + 1;
    if(x_<= field_width && y_<=field_height) {
        Form1->Caption = "X=" + IntToStr(x_) + " Y=" + IntToStr(y_);
        PaintBox1->Canvas->Pen->Width   = 1;
        PaintBox1->Canvas->Pen->Color   = clWhite;
        PaintBox1->Canvas->Pen->Style   = psSolid;
        PaintBox1->Canvas->Brush->Color = clRed;
        PaintBox1->Canvas->Brush->Style = bsSolid;
        PaintBox1->Canvas->Ellipse( border_x + scale*(x_-1) - radius, border_y + scale*(y_-1) - radius, border_x + scale*(x_-1) + radius, border_y + scale*(y_-1) + radius );

        if(Button==mbLeft) {
            if(start_) {
                PaintBox1->Canvas->Pen->Color   = clBtnFace;
		        PaintBox1->Canvas->Brush->Color = clBtnFace;
                PaintBox1->Canvas->Ellipse( border_x + scale*(temp_point.pos_x-1) - radius, border_y + scale*(temp_point.pos_y-1) - radius, border_x + scale*(temp_point.pos_x-1) + radius, border_y + scale*(temp_point.pos_y-1) + radius );
                PaintBox1->Canvas->Pen->Color   = clBlack;
                PaintBox1->Canvas->MoveTo(border_x + scale*(temp_point.pos_x-1) - radius, border_y + scale*(temp_point.pos_y-1));
                PaintBox1->Canvas->LineTo(border_x + scale*(temp_point.pos_x-1) + radius, border_y + scale*(temp_point.pos_y-1));
                PaintBox1->Canvas->MoveTo(border_x + scale*(temp_point.pos_x-1), border_y + scale*(temp_point.pos_y-1) - radius);
                PaintBox1->Canvas->LineTo(border_x + scale*(temp_point.pos_x-1), border_y + scale*(temp_point.pos_y-1) + radius);
            }
            temp_point.pos_x = x_;
            temp_point.pos_y = y_;
            start_=true;
        }
        if(Button==mbRight) {
            if(end_) {
                PaintBox1->Canvas->Pen->Color   = clBtnFace;
		        PaintBox1->Canvas->Brush->Color = clBtnFace;
                PaintBox1->Canvas->Ellipse(border_x + scale*(temp_point.pos_x_end-1) - radius, border_y + scale*(temp_point.pos_y_end-1) - radius, border_x + scale*(temp_point.pos_x_end-1) + radius, border_y + scale*(temp_point.pos_y_end-1) + radius);
                PaintBox1->Canvas->Pen->Color   = clBlack;
                PaintBox1->Canvas->MoveTo(border_x + scale*(temp_point.pos_x_end-1) - radius, border_y + scale*(temp_point.pos_y_end-1));
                PaintBox1->Canvas->LineTo(border_x + scale*(temp_point.pos_x_end-1) + radius, border_y + scale*(temp_point.pos_y_end-1));
                PaintBox1->Canvas->MoveTo(border_x + scale*(temp_point.pos_x_end-1), border_y + scale*(temp_point.pos_y_end-1) - radius);
                PaintBox1->Canvas->LineTo(border_x + scale*(temp_point.pos_x_end-1), border_y + scale*(temp_point.pos_y_end-1) + radius);
            }
            temp_point.pos_x_end = x_;
            temp_point.pos_y_end = y_;
            end_=true;
        }
        if ( end_ && start_ ) ButtonInsert->Enabled = true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonInsertClick(TObject *Sender)
{   int x_,y_;
    ButtonDelete->Enabled = true;
    ButtonInsert->Enabled = false;
    start_=false;
    end_=false;
    wire_points.push_back( temp_point );
    ListBox1->Items->Add( wire_points.back().toString() );
	PaintBox1->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button4Click(TObject *Sender)
{
	const int fileCount = 5;
	int res[fileCount] = {PAT_MODEL, RTP_MODEL, FRM_MODEL, SMR_MODEL, DPT_MODEL};
	char* fileName[fileCount] = {"plata.pat", "plata.rtp", "plata.frm", "plata.smr", "plata.dpt"};
	for ( int i = 0; i < fileCount; i++ ) {
    	if ( !FileExists( fileName[i] ) ) {
			TResourceStream* file = new TResourceStream( 0, res[i], RT_RCDATA );
			file->SaveToFile(fileName[i]);
			delete file;
        }
    }

	FILE *sourse_model_files, * model_files;
    char temp;
    FILE *trace_file;
    char temp_str[100];
    int current_wire=-1,temp_var,j;

    // FUN
   	if ( !FileExists( "plata.fun" ) ) {
	    TFileStream* file = new TFileStream( "plata.fun", fmCreate );
	    TStringStream* str_stream = new TStringStream( "" );
	    str_stream->WriteString( "$Constant\n" );
	    str_stream->WriteString( "\tКол_во_точек_по_X : integer = " + IntToStr(field_width)  + "\n" );
	    str_stream->WriteString( "\tКол_во_точек_по_Y : integer = " + IntToStr(field_height) + "\n" );
	    str_stream->Seek( 0, soFromBeginning );
		file->CopyFrom( str_stream, str_stream->Size );
		TResourceStream* res_stream = new TResourceStream( 0, FUN_MODEL, RT_RCDATA );
		res_stream->SaveToStream( file );
		delete file;
		delete str_stream;
		delete res_stream;
    }

    // RSS
    TFileStream* file = new TFileStream( "plata.rss", fmCreate );
    TStringStream* str_stream = new TStringStream( "" );
    str_stream->WriteString( "$Resources\n" );
    std::sort( wire_points.begin(), wire_points.end() );
    int i = 1;
	std::vector< fixsed_points >::iterator it = wire_points.begin();
    while ( it != wire_points.end() ) {
		str_stream->WriteString( "\tТочка_провода_" + IntToStr(i) +
                                 " : Точки_провода " + IntToStr(i) +
                                 " " + IntToStr(it->pos_x) +
                                 " " + IntToStr(it->pos_y) +
                                 " " + IntToStr(it->pos_x_end) +
                                 " " + IntToStr(it->pos_y_end) +
                                 " *\n" );
        i++;
    	it++;
    }
    str_stream->WriteString( "\tТочка_провода_текущая_ : Точка_провода_текущая 0 0 0 0 0\n" );
    str_stream->WriteString( "\tСистема_плата : Система  * *\n" );
    str_stream->WriteString( "\tСчетчик_X     : Счетчики * * 1\n" );
    str_stream->WriteString( "\tСчетчик_Y     : Счетчики * * 2\n" );
    str_stream->WriteString( "$End\n");
    str_stream->Seek( 0, soFromBeginning );
	file->CopyFrom( str_stream, str_stream->Size );
	delete file;
	delete str_stream;
	file       = NULL;
	str_stream = NULL;

	STARTUPINFO si;
    PROCESS_INFORMATION pi;
	memset( &si, 0, sizeof(STARTUPINFO) );
	memset( &pi, 0, sizeof(PROCESS_INFORMATION) );
	si.cb          = sizeof(STARTUPINFO);
	si.wShowWindow = SW_SHOWMAXIMIZED;
	si.dwFlags     = STARTF_USESHOWWINDOW;
	AnsiString str = "RAO-studio.exe -autorun -autoexit plata.smr";
//    str = "rdosim.exe plata.smr";

	if ( ::CreateProcess( NULL, str.c_str(), NULL, NULL, false, CREATE_SUSPENDED | NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi ) ) {
		::ResumeThread( pi.hThread );
		::WaitForSingleObject( pi.hProcess, INFINITE );
	}

    trace_file = fopen("plata.trc","r");
    if ( trace_file ) {
    while ( !feof(trace_file) ) {
        fscanf(trace_file,"%s",temp_str);
        if( !strcmp(temp_str,"SB") ) {
        	wire_way.push_back( std::vector<best_ways>() );
            current_wire++;
            all_tops[current_wire] = 0;
        }
       /* if(!strcmp(temp_str,"STN") || !strcmp(temp_str,"STR")) {
            fscanf(trace_file,"%d",&temp_var);
            wires[current_wire][temp_var].top_sign = 0;
            wires[current_wire][temp_var].top_number = temp_var;
            fscanf(trace_file,"%d",&wires[current_wire][temp_var].number_parent_top);
            fscanf(trace_file,"%d",&wires[current_wire][temp_var].way_cost);
            all_tops[current_wire]++;
        }*/

        if ( !strcmp(temp_str,"SD") ) {
            do {
            	best_ways bway;
                fscanf(trace_file,"%d",&bway.top_number);
                fscanf(trace_file,"%d",&bway.action_number);
				wire_way.back().push_back( bway );
                do {
                    fscanf( trace_file, "%c", &temp_str[0] );
                } while( temp_str[0] != '\n' && temp_str[0] != 'S' );
            } while( temp_str[0] != 'S' );
			wire_way.back().erase( wire_way.back().begin() + wire_way.back().size() - 1 );
            Form1->Caption = Form1->Caption + " " +IntToStr(all_tops[current_wire]);
        }
    }
    fclose(trace_file);
    wasRun = true;
    }
    working_mode = 1;
    ButtonInsert->Enabled = false;
    ButtonDelete->Enabled = false;
    CSpinEdit1->Enabled = false;
    CSpinEdit2->Enabled = false;
    CSpinEdit3->Enabled = false;
	PaintBox1->Repaint();
    if ( !CheckBox1->Checked ) {
		remove( "plata.pat" );
		remove( "plata.rtp" );
		remove( "plata.rss" );
		remove( "plata.fun" );
		remove( "plata.dpt" );
		remove( "plata.frm" );
		remove( "plata.smr" );
		remove( "plata.pmv" );
		remove( "plata.trc" );
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::ButtonDeleteClick(TObject *Sender)
{   int i;
    if(ListBox1->ItemIndex==-1) ShowMessage("Ничего не выделено для удаления");
    else {
		std::vector< fixsed_points >::iterator it = wire_points.begin();
		while ( it != wire_points.end() ) {
			if ( ListBox1->Items->Strings[ListBox1->ItemIndex] == it->toString() ) {
				wire_points.erase( it );
				break;
			}
        	it++;
        }
	    ListBox1->Items->Delete( ListBox1->ItemIndex );
        PaintBox1->Repaint();
    }
    if(ListBox1->Items->Count==0) ButtonDelete->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PaintBox1Paint(TObject *Sender)
{
    if ( !wire_points.empty() ) {
	    int max_x = -1;
    	int max_y = -1;
		std::vector< fixsed_points >::iterator it = wire_points.begin();
		while ( it != wire_points.end() ) {
        	if ( max_x < it->pos_x     ) max_x = it->pos_x;
        	if ( max_x < it->pos_x_end ) max_x = it->pos_x_end;
        	if ( max_y < it->pos_y     ) max_y = it->pos_y;
        	if ( max_y < it->pos_y_end ) max_y = it->pos_y_end;
    		it++;
		}
        if ( CSpinEdit1->Value < max_x ) {
            CSpinEdit1->Value = max_x;
            return;
        }
        if ( field_height < max_y ) {
            CSpinEdit2->Value = max_y;
            return;
        }
    }
    if ( CSpinEdit3->Value < 10 ) {
    	CSpinEdit3->Value = 10;
        return;
    }
	field_width  = CSpinEdit1->Value;
    field_height = CSpinEdit2->Value;
    scale        = CSpinEdit3->Value;
    radius = scale / 5;
    PaintBox1->Width = field_width * scale;
    PaintBox1->Height = field_height * scale;
    PaintBox1->Canvas->Pen->Color = clBlack;
    PaintBox1->Canvas->Pen->Width = 1;
    for ( int i = 0; i < field_height; i++ ) {
        PaintBox1->Canvas->MoveTo( border_x - radius, border_y + scale * i );
        PaintBox1->Canvas->LineTo( border_x + radius + scale * (field_width - 1), border_y + scale * i );
    }
    for ( int i = 0; i < field_width; i++ ) {
        PaintBox1->Canvas->MoveTo( border_x + scale * i, border_y - radius );
        PaintBox1->Canvas->LineTo( border_x + scale * i, border_y + radius + scale * (field_height - 1) );
    }
    PaintBox1->Canvas->Pen->Width   = 1;
    PaintBox1->Canvas->Pen->Color   = clWhite;
    PaintBox1->Canvas->Brush->Color = clRed;
	std::vector< fixsed_points >::iterator it = wire_points.begin();
	while ( it != wire_points.end() ) {
	    PaintBox1->Canvas->Ellipse(border_x + scale*(it->pos_x-1) - radius,
    	                           border_y + scale*(it->pos_y-1) - radius,
        	                       border_x + scale*(it->pos_x-1) + radius,
            	                   border_y + scale*(it->pos_y-1) + radius);
	    PaintBox1->Canvas->Ellipse(border_x + scale*(it->pos_x_end-1) - radius,
    	                           border_y + scale*(it->pos_y_end-1) - radius,
        	                       border_x + scale*(it->pos_x_end-1) + radius,
            	                   border_y + scale*(it->pos_y_end-1) + radius);
		it++;
	}

    if ( working_mode != 0 ) {
        int x_, y_;
        PaintBox1->Canvas->Pen->Color = clRed;
        PaintBox1->Canvas->Pen->Width = 3;
        std::vector< std::vector<best_ways> >::const_iterator wire_way_it = wire_way.begin();
        for ( int i = 0; i < wire_points.size(); i++ ) {
	        if ( wire_way_it == wire_way.end() ) return;
            x_ = border_x + scale*(wire_points[i].pos_x-1);
            y_ = border_y + scale*(wire_points[i].pos_y-1);
            PaintBox1->Canvas->MoveTo(x_, y_);
            std::vector<best_ways>::const_iterator bestway_it = wire_way_it->begin();
            while ( bestway_it != wire_way_it->end() ) {
                switch (bestway_it->action_number) {
                    case 9  : {PaintBox1->Canvas->LineTo(x_=x_-scale,y_); break;}
                    case 10 : {PaintBox1->Canvas->LineTo(x_=x_+scale,y_); break;}
                    case 11 : {PaintBox1->Canvas->LineTo(x_,y_=y_-scale); break;}
                    case 12 : {PaintBox1->Canvas->LineTo(x_,y_=y_+scale); break;}
                }
                bestway_it++;
            }
            wire_way_it++;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
    if ( FileExists( "plata.pat" ) || FileExists( "plata.rtp" ) || FileExists( "plata.rss" ) || FileExists( "plata.frm" ) || FileExists( "plata.fun" ) || FileExists( "plata.dpt" ) || FileExists( "plata.smr" ) || FileExists( "plata.pmv" ) || FileExists( "plata.trc" ) ) {
    	if ( ::MessageBox( Application->Handle, "Найдены файлы старой модели. Стереть их и создать новую модель ?", "RAO-platareader", MB_ICONQUESTION | MB_YESNO ) == IDYES ) {
			remove( "plata.pat" );
			remove( "plata.rtp" );
			remove( "plata.rss" );
			remove( "plata.frm" );
			remove( "plata.fun" );
			remove( "plata.dpt" );
			remove( "plata.smr" );
			remove( "plata.pmv" );
			remove( "plata.trc" );
        } else {
			CheckBox1->Checked = true;
        }
    }
}
//---------------------------------------------------------------------------

