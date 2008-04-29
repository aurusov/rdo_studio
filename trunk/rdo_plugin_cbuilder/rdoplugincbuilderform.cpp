//---------------------------------------------------------------------------
#include <vcl.h>
#include <stdio.h>
#pragma hdrstop

#include "rdoplugincbuilderform.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TCBuilderPluginForm* CBuilderPluginForm = NULL;

AnsiString MiscFormat(char const *str, ...)
{
	va_list paramList;
	va_start(paramList, str);
	int size = vsnprintf(NULL, 0, str, paramList);
	va_end(paramList);
	if (size) {
		AnsiString formatStr;
		formatStr.SetLength(size);
		vsnprintf(formatStr.c_str(), size, str, paramList);
		return formatStr;
	}
	return "";
}
//---------------------------------------------------------------------------
__fastcall TCBuilderPluginForm::TCBuilderPluginForm( TComponent* Owner, const rdoPlugin::Studio* _studio ):
	TForm(Owner),
	studio( *_studio )
{
}
//---------------------------------------------------------------------------
void __fastcall TCBuilderPluginForm::FormCreate(TObject *Sender)
{
	updateControlsState();
}
//---------------------------------------------------------------------------
void TCBuilderPluginForm::updateControlsState()
{
	Save1->Enabled    = studio.model.isModify();
	Close2->Enabled   = studio.model.hasModel();
	Build1->Enabled   = studio.model.hasModel() && !studio.model.isRunning();
	Execute1->Enabled = studio.model.hasModel() && !studio.model.isRunning();
	Stop1->Enabled    = studio.model.isRunning();
}
//---------------------------------------------------------------------------
void __fastcall TCBuilderPluginForm::FormClose(TObject *Sender, TCloseAction &Action)
{
//	studio.stopPlugin( HInstance );
}
//---------------------------------------------------------------------------
void TCBuilderPluginForm::insertLine( const char* line )
{
	static int lines = 1;
	Memo->Lines->Append( IntToStr( lines++ ) + ". " + line );
//	Memo->Lines->Append( line );
}
//---------------------------------------------------------------------------
void __fastcall TCBuilderPluginForm::Close1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TCBuilderPluginForm::New1Click(TObject *Sender)
{
//	studio.model.newModel();
}
//---------------------------------------------------------------------------
void __fastcall TCBuilderPluginForm::Open1Click(TObject *Sender)
{
	studio.model.open( NULL );
}
//---------------------------------------------------------------------------
void __fastcall TCBuilderPluginForm::Save1Click(TObject *Sender)
{
	studio.model.save();
}
//---------------------------------------------------------------------------
void __fastcall TCBuilderPluginForm::Close2Click(TObject *Sender)
{
	studio.model.close();
}
//---------------------------------------------------------------------------
void __fastcall TCBuilderPluginForm::Build1Click(TObject *Sender)
{
	studio.model.build();
}
//---------------------------------------------------------------------------
void __fastcall TCBuilderPluginForm::Execute1Click(TObject *Sender)
{
	studio.model.run();
}
//---------------------------------------------------------------------------
void __fastcall TCBuilderPluginForm::Stop1Click(TObject *Sender)
{
	studio.model.stop();
}
//---------------------------------------------------------------------------

