//---------------------------------------------------------------------------
#include <vcl.h>
#include <list>
#pragma hdrstop

#include "rdo_plugin_game5_mainform.h"
#include "rdo_plugin_game5_model_const.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TMainForm* MainForm = NULL;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm( TComponent* Owner, const rdoPlugin::Studio& _studio, HINSTANCE _hInstance ):
	TForm( Owner /*_studio->mainFrame()*/ ),
   	studio( _studio ),
    hInstance( _hInstance )
{
	MakeChip.push_back( ChipNumType(1) );
	MakeChip.push_back( ChipNumType(2) );
	MakeChip.push_back( ChipNumType(3) );
	MakeChip.push_back( ChipNumType(4) );
	MakeChip.push_back( ChipNumType(5) );
	MakeChip.push_back( ChipNumType(6) );
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	if ( studio.model.hasModel() ) {
    	if ( studio.model.isModify() ) {
        	ShowMessage( "test" );
        } else {
        	studio.model.close();
        }
    }
	if ( studio.isShow() ) {
		studio.show( SW_HIDE );
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	if ( !studio.isShow() ) {
		studio.show( SW_SHOWNORMAL );
    }
	studio.plugin.stop( HInstance );
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Close1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SourceClick(TObject *Sender)
{
	if ( studio.isShow() ) {
		studio.show( SW_HIDE );
    } else {
		studio.show( SW_SHOWNORMAL );
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::RFun1Click(TObject *Sender)
{
	TRadioButton* rb = dynamic_cast<TRadioButton*>(Sender);
    if ( rb ) {
        rb->Checked = true;
        TEdit* edit = NULL;
        if ( rb->Name[1] == 'R' ) {
        	edit = RFun2Edit;
        } else if ( rb->Name[1] == 'L' ) {
            edit = LFun2Edit;
        } else if ( rb->Name[1] == 'U' ) {
        	edit = UFun2Edit;
        } else if ( rb->Name[1] == 'D' ) {
        	edit = DFun2Edit;
        }
        if ( rb->Name[5] == '2' ) {
            edit->Enabled = true;
            edit->Color   = clWindow;
            edit->SetFocus();
        } else {
	        edit->Enabled = false;
    	    edit->Color   = clInactiveBorder;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DPTButtonClick(TObject *Sender)
{
	ClientWidth        = 575;
	DPTButton->Visible = false;
	DPTBevel->Visible  = false;
}
//---------------------------------------------------------------------------
struct RDO_FILE {
    int res_id;
    rdoPlugin::ModelFileType file_type;
    RDO_FILE( int _res_id, rdoPlugin::ModelFileType _file_type ):
    	res_id( _res_id ),
        file_type( _file_type )
    {
    }
};

void __fastcall TMainForm::OkButtonClick(TObject *Sender)
{
	if ( !MainForm->studio.model.hasModel() ) {
		MainForm->studio.model.action.enable( rdoPlugin::maCreate );
        TCHAR PluginName[MAX_PATH + 1];
        GetModuleFileName( hInstance, PluginName, MAX_PATH + 1 );
    	MainForm->studio.model.create( "game5", AnsiString(ExtractFilePath( PluginName ) + "game5").c_str() );
    }
	std::list< RDO_FILE > model_temp;
    model_temp.push_back( RDO_FILE( PAT_MODEL, rdoPlugin::PAT ) );
    model_temp.push_back( RDO_FILE( RTP_MODEL, rdoPlugin::RTP ) );
    model_temp.push_back( RDO_FILE( FUN_MODEL, rdoPlugin::FUN ) );
    model_temp.push_back( RDO_FILE( SMR_MODEL, rdoPlugin::SMR ) );
	std::list< RDO_FILE >::const_iterator it = model_temp.begin();
    while ( it != model_temp.end() ) {
		TResourceStream* res = new TResourceStream( (int)studio.plugin.hInstance, it->res_id, RT_RCDATA );
        AnsiString str;
        TStringStream* stream = new TStringStream( str );
        res->SaveToStream( stream );
        stream->Seek( 0, soFromBeginning );
        str = stream->DataString;
        delete stream;
        delete res;
	    studio.model.write( it->file_type, str.c_str() );
    	it++;
    }
    AnsiString rss = "$Resources\r\n";
    for ( int i = 0; i < 5; i++ ) {
    	rss += "\t‘ишка" + AnsiString(i+1) + " : ‘ишка " + AnsiString(i+1) + " " + AnsiString(MakeChip[i].mesto) + "\r\n";
    }
    rss += "\tƒырка  : ƒырка_t " + AnsiString(MakeChip[5].mesto) + "\r\n$End\r\n";
    studio.model.write( rdoPlugin::RSS, rss.c_str() );
	MainForm->studio.model.action.disable( rdoPlugin::maCreate );
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::RendButtonClick(TObject *Sender)
{
	char* data = NULL;
    studio.model.read( rdoPlugin::SMR, &data );
    ShowMessage( data );
    delete[] data;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Chip1ImClick( TObject* Sender )
{
    int i = -1;
	TImage* image = dynamic_cast<TImage*>(Sender);
    if ( !image ) return;
    if ( image->Name == "Chip1Im" ) {
    	i = 0;
    } else if ( image->Name == "Chip2Im" ) {
    	i = 1;
    } else if ( image->Name == "Chip3Im" ) {
    	i = 2;
    } else if ( image->Name == "Chip4Im" ) {
    	i = 3;
    } else if ( image->Name == "Chip5Im" ) {
    	i = 4;
    }
    if ( MakeChip[i].mesto != 3 && MakeChip[i].mesto != 6 && MakeChip[i].mesto == MakeChip[5].mesto - 1 ) {
	    MakeChip[i].mesto++;
    	MakeChip[5].mesto--;
    } else if ( MakeChip[i].mesto != 1 && MakeChip[i].mesto != 4 && MakeChip[i].mesto == MakeChip[5].mesto + 1 ) {
    	MakeChip[i].mesto--;
    	MakeChip[5].mesto++;
    } else if ( MakeChip[i].mesto == MakeChip[5].mesto + 3 ) {
	    MakeChip[i].mesto -= 3;
    	MakeChip[5].mesto += 3;
    } else if ( MakeChip[i].mesto == MakeChip[5].mesto - 3 ) {
	    MakeChip[i].mesto += 3;
    	MakeChip[5].mesto -= 3;
    } else return;
    ReDrawChip();
}
//---------------------------------------------------------------------------
void TMainForm::ReDrawChip()
{
    for ( int i = 0; i < 5; i++ ) {
		TImage* image = NULL;
    	switch ( i ) {
        	case 0: image = Chip1Im; break;
        	case 1: image = Chip2Im; break;
        	case 2: image = Chip3Im; break;
        	case 3: image = Chip4Im; break;
        	case 4: image = Chip5Im; break;
        }
        switch ( MakeChip[i].mesto ) {
        	case 1: {
                image->Left = 48;
                image->Top  = 155;
            	break;
            }
        	case 2: {
                image->Left = 104;
                image->Top  = 155;
            	break;
            }
        	case 3: {
                image->Left = 160;
                image->Top  = 155;
            	break;
            }
        	case 4: {
                image->Left = 48;
                image->Top  = 210;
            	break;
            }
        	case 5: {
                image->Left = 104;
                image->Top  = 210;
            	break;
            }
        	case 6: {
                image->Left = 160;
                image->Top  = 210;
            	break;
            }
        }
    }
}

