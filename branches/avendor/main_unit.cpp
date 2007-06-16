//---------------------------------------------------------------------------
const max_vire_count = 100;

#include <vcl.h>
#include <stdio.h>
#include <string.h>
#include <process.h>
#include <vector>
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
    int wire_number;
    bool operator< ( const fixsed_points& line ) const {
    	return sqrt( pow(pos_x - pos_x_end, 2) + pow(pos_y - pos_y_end, 2) ) <
               sqrt( pow(line.pos_x - line.pos_x_end, 2) + pow(line.pos_y - line.pos_y_end, 2) );
    }
};

//graph_tops wires[max_vire_count][50000];
graph_tops wires[max_vire_count][50];
best_ways wire_way[max_vire_count][50];
fixsed_points wire_points[max_vire_count], temp_point;
int all_tops[max_vire_count];
int field_width,field_height;
int scale;
int radius;
int last_point_number = 0,working_mode=0;

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
    field_width = CSpinEdit1->Value;
    field_height = CSpinEdit2->Value;
    scale = CSpinEdit3->Value;
    radius = scale / 5;
    redraving_();
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
    int x_,y_;
    x_= (X + scale/2) / scale + 1;
    y_= (Y + scale/2) / scale + 1;
    if(x_<= field_width && y_<=field_height) {
        Form1->Caption = "X=" + IntToStr(x_) + " Y=" + IntToStr(y_);
        PaintBox1->Canvas->Pen->Color = clLime;
        PaintBox1->Canvas->Ellipse(scale*(x_-1) - radius, scale*(y_-1) - radius, scale*(x_-1) + radius, scale*(y_-1) + radius);
        PaintBox1->Canvas->Pen->Color = clBlack;

        if(Button==mbLeft) {
            if(start_) {
                PaintBox1->Canvas->Pen->Color = clBtnFace;
                PaintBox1->Canvas->Ellipse(scale*(temp_point.pos_x-1) - radius, scale*(temp_point.pos_y-1) - radius, scale*(temp_point.pos_x-1) + radius, scale*(temp_point.pos_y-1) + radius);
                PaintBox1->Canvas->Pen->Color = clBlack;
                PaintBox1->Canvas->MoveTo(scale*(temp_point.pos_x-1) - radius, scale*(temp_point.pos_y-1));
                PaintBox1->Canvas->LineTo(scale*(temp_point.pos_x-1) + radius, scale*(temp_point.pos_y-1));
                PaintBox1->Canvas->MoveTo(scale*(temp_point.pos_x-1), scale*(temp_point.pos_y-1) - radius);
                PaintBox1->Canvas->LineTo(scale*(temp_point.pos_x-1), scale*(temp_point.pos_y-1) + radius);
            }
            temp_point.pos_x = x_;
            temp_point.pos_y = y_;
            start_=true;
        }
        if(Button==mbRight) {
            if(end_) {
                PaintBox1->Canvas->Pen->Color = clBtnFace;
                PaintBox1->Canvas->Ellipse(scale*(temp_point.pos_x_end-1) - radius, scale*(temp_point.pos_y_end-1) - radius, scale*(temp_point.pos_x_end-1) + radius, scale*(temp_point.pos_y_end-1) + radius);
                PaintBox1->Canvas->Pen->Color = clBlack;
                PaintBox1->Canvas->MoveTo(scale*(temp_point.pos_x_end-1) - radius, scale*(temp_point.pos_y_end-1));
                PaintBox1->Canvas->LineTo(scale*(temp_point.pos_x_end-1) + radius, scale*(temp_point.pos_y_end-1));
                PaintBox1->Canvas->MoveTo(scale*(temp_point.pos_x_end-1), scale*(temp_point.pos_y_end-1) - radius);
                PaintBox1->Canvas->LineTo(scale*(temp_point.pos_x_end-1), scale*(temp_point.pos_y_end-1) + radius);
            }
            temp_point.pos_x_end = x_;
            temp_point.pos_y_end = y_;
            end_=true;
        }
        if(end_ && start_) Button2->Enabled=true;
        temp_point.wire_number = CSpinEdit4->Value;
        Memo1->Lines->Clear();
        Memo1->Lines->Add("Тип провода: " + IntToStr(CSpinEdit4->Value));
        Memo1->Lines->Add("X1="+IntToStr(temp_point.pos_x)+" "+
                             "Y1="+IntToStr(temp_point.pos_y)+" " );
        Memo1->Lines->Add(   "X2="+IntToStr(temp_point.pos_x_end)+" "+
                             "Y2="+IntToStr(temp_point.pos_y_end) );

    }

}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button2Click(TObject *Sender)
{   int x_,y_;
    Button3->Enabled = true;
    Button2->Enabled = false;
    Memo1->Lines->Clear();
    start_=false;
    end_=false;
    wire_points[last_point_number].wire_number = temp_point.wire_number;
    CSpinEdit4->Value++;
    x_ = wire_points[last_point_number].pos_x       = temp_point.pos_x;
    y_ = wire_points[last_point_number].pos_y       = temp_point.pos_y;
    PaintBox1->Canvas->Ellipse(scale*(x_-1) - radius, scale*(y_-1) - radius, scale*(x_-1) + radius, scale*(y_-1) + radius);
    x_ = wire_points[last_point_number].pos_x_end   = temp_point.pos_x_end;
    y_ = wire_points[last_point_number].pos_y_end   = temp_point.pos_y_end;
    PaintBox1->Canvas->Ellipse(scale*(x_-1) - radius, scale*(y_-1) - radius, scale*(x_-1) + radius, scale*(y_-1) + radius);
    last_point_number++;
    ListBox1->Items->Add("Тип:" +IntToStr(temp_point.wire_number) +
                         " X1="+IntToStr(temp_point.pos_x)+" "+
                         " Y1="+IntToStr(temp_point.pos_y)+" "+
                         " X2="+IntToStr(temp_point.pos_x_end)+" "+
                         " Y2="+IntToStr(temp_point.pos_y_end) );
    //wire_points[last_point_number+1].wire_number = -2;

}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button4Click(TObject *Sender)
{
	const int fileCount = 5;
	int res[fileCount] = {PAT_MODEL, RTP_MODEL, FRM_MODEL, SMR_MODEL, DPT_MODEL};
	char* fileName[fileCount] = {"plata.pat", "plata.rtp", "plata.frm", "plata.smr", "plata.dpt"};
	for ( int i = 0; i < fileCount; i++ ) {
		TResourceStream* file = new TResourceStream( 0, res[i], RT_RCDATA );
		file->SaveToFile(fileName[i]);
		delete file;
    }

	FILE *sourse_model_files, * model_files;
    char temp;
    FILE *trace_file;
    char temp_str[100];
    int current_wire=-1,temp_var,j;

    // FUN
    TFileStream* file = new TFileStream( "plata.fun", fmCreate );
    TStringStream* str_stream = new TStringStream( "" );
    str_stream->WriteString( "$Constant\n" );
    str_stream->WriteString( "\tКол_во_точек_по_X : integer = " + AnsiString(field_width)  + "\n" );
    str_stream->WriteString( "\tКол_во_точек_по_Y : integer = " + AnsiString(field_height) + "\n" );
    str_stream->Seek( 0, soFromBeginning );
	file->CopyFrom( str_stream, str_stream->Size );
	TResourceStream* res_stream = new TResourceStream( 0, FUN_MODEL, RT_RCDATA );
	res_stream->SaveToStream( file );
	delete file;
	delete str_stream;
	delete res_stream;
	file       = NULL;
	str_stream = NULL;
	res_stream = NULL;

    // RSS
    file = new TFileStream( "plata.rss", fmCreate );
    str_stream = new TStringStream( "" );
    str_stream->WriteString( "$Resources\n" );
	std::vector< fixsed_points > lines;
    for ( int i = 0; i < last_point_number; i++ ) {
    	lines.push_back( wire_points[i] );
    }
    std::sort( lines.begin(), lines.end() );
    for ( int i = 0; i < last_point_number; i++ ) {
		wire_points[i] = lines[i];
    }
    int i = 1;
	std::vector< fixsed_points >::iterator it = lines.begin();
    while ( it != lines.end() ) {
		str_stream->WriteString( "\tТочка_провода_" + AnsiString(i) +
                                 " : Точки_провода " + AnsiString(it->wire_number) +
                                 " " + AnsiString(it->pos_x) +
                                 " " + AnsiString(it->pos_y) +
                                 " " + AnsiString(it->pos_x_end) +
                                 " " + AnsiString(it->pos_y_end) +
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

    spawnl( P_WAIT, "RAO-studio.exe", "RAO-studio.exe", "-autorun -autoexit plata.smr", NULL );
    if(!CheckBox1->Checked) {
        remove("plata.pat");
        remove("plata.rtp");
        remove("plata.rss");
        remove("plata.fun");
        remove("plata.dpt");
        remove("plata.frm");
        remove("plata.smr");
    }
    trace_file = fopen("plata.trc","r");
    while(!feof(trace_file)) {
        fscanf(trace_file,"%s",temp_str);
        if(!strcmp(temp_str,"SB")) {
            current_wire++;
            all_tops[current_wire] = 0;
            i = 0;
        }
       /* if(!strcmp(temp_str,"STN") || !strcmp(temp_str,"STR")) {
            fscanf(trace_file,"%d",&temp_var);
            wires[current_wire][temp_var].top_sign = 0;
            wires[current_wire][temp_var].top_number = temp_var;
            fscanf(trace_file,"%d",&wires[current_wire][temp_var].number_parent_top);
            fscanf(trace_file,"%d",&wires[current_wire][temp_var].way_cost);
            all_tops[current_wire]++;
        }*/

        if(!strcmp(temp_str,"SD")) {
            i = 0;
            do {
                fscanf(trace_file,"%d",&wire_way[current_wire][i].top_number);
                fscanf(trace_file,"%d",&wire_way[current_wire][i].action_number);
                i++;
                do {
                    fscanf(trace_file,"%c",&temp_str[0]);
                }
                while(temp_str[0]!='\n' && temp_str[0]!='S');
            }
            while(temp_str[0]!='S');
            wire_way[current_wire][i].top_number = -2;
            Form1->Caption = Form1->Caption + " " +IntToStr(all_tops[current_wire]);
        }


    }
    fclose(trace_file);
    working_mode = 1;
    redraving_();
//    Refresh();
}
//---------------------------------------------------------------------------

void TForm1::redraving_()
{  int i,j;
  field_width = CSpinEdit1->Value;
    field_height = CSpinEdit2->Value;
    scale = CSpinEdit3->Value;
    radius = scale / 5;
    PaintBox1->Width = field_width * scale;
    PaintBox1->Height = field_height * scale;
//    PaintBox1->Repaint();
    PaintBox1->Canvas->Pen->Color = clBlack;
    PaintBox1->Canvas->Pen->Width = 1;
    for(i=0;i<field_height;i++) {
        PaintBox1->Canvas->MoveTo(0,scale * i);
        PaintBox1->Canvas->LineTo(scale * (field_width - 1),scale * i);
    }

    for(i=0;i<field_width;i++) {
        PaintBox1->Canvas->MoveTo(scale * i, 0);
        PaintBox1->Canvas->LineTo(scale * i, scale * (field_height - 1));
    }

    for(i=0;i<last_point_number;i++) {
    PaintBox1->Canvas->Ellipse(scale*(wire_points[i].pos_x-1) - radius,
                               scale*(wire_points[i].pos_y-1) - radius,
                               scale*(wire_points[i].pos_x-1) + radius,
                               scale*(wire_points[i].pos_y-1) + radius
                               );
    PaintBox1->Canvas->Ellipse(scale*(wire_points[i].pos_x_end-1) - radius,
                               scale*(wire_points[i].pos_y_end-1) - radius,
                               scale*(wire_points[i].pos_x_end-1) + radius,
                               scale*(wire_points[i].pos_y_end-1) + radius);

    }
    
    if(working_mode!=0) {
        int x_,y_;
        PaintBox1->Canvas->Pen->Color = RGB(255,0,0);
        PaintBox1->Canvas->Pen->Width = 3;
        for(i=0;i<last_point_number;i++) {
            j = 0;
            x_ = scale*(wire_points[i].pos_x-1);
            y_ = scale*(wire_points[i].pos_y-1);
            PaintBox1->Canvas->MoveTo(x_,y_);
            do {
                switch (wire_way[i][j].action_number) {
                    case 9  : {PaintBox1->Canvas->LineTo(x_=x_-scale,y_); break;}
                    case 10 : {PaintBox1->Canvas->LineTo(x_=x_+scale,y_); break;}
                    case 11 : {PaintBox1->Canvas->LineTo(x_,y_=y_-scale); break;}
                    case 12 : {PaintBox1->Canvas->LineTo(x_,y_=y_+scale); break;}
                }
            j++;
            }
            while (wire_way[i][j].top_number!=-2);
        }
        PaintBox1->Canvas->Pen->Color = clBlack;
        PaintBox1->Canvas->Pen->Width = 1;
    }
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button3Click(TObject *Sender)
{   int i;
    if(ListBox1->ItemIndex==-1) ShowMessage("Ничего не выделено для удаления");
    else {
        for(i=ListBox1->ItemIndex;i<last_point_number - 1;i++)  {
            wire_points[i].wire_number = wire_points[i+1].wire_number;
            wire_points[i].pos_x = wire_points[i+1].pos_x;
            wire_points[i].pos_y = wire_points[i+1].pos_y;
            wire_points[i].pos_x_end = wire_points[i+1].pos_x_end;
            wire_points[i].pos_y_end = wire_points[i+1].pos_y_end;

        }
    ListBox1->Items->Delete(ListBox1->ItemIndex);
    last_point_number--;
    redraving_();
    }
    if(ListBox1->Items->Count==0) Button3->Enabled=false;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::PaintBox1Paint(TObject *Sender)
{
    redraving_();
}
//---------------------------------------------------------------------------


