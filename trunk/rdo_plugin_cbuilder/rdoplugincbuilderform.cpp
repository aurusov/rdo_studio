//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "rdoplugincbuilderform.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
TCBuilderPluginForm* CBuilderPluginForm = NULL;
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
	studio.stopPlugin( HInstance );
}
//---------------------------------------------------------------------------
void TCBuilderPluginForm::insertLine( const char* line )
{
	Memo->Lines->Append( line );
}
//---------------------------------------------------------------------------
void __fastcall TCBuilderPluginForm::Close1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TCBuilderPluginForm::New1Click(TObject *Sender)
{
	studio.model.newModel();
}
//---------------------------------------------------------------------------
void __fastcall TCBuilderPluginForm::Open1Click(TObject *Sender)
{
	studio.model.openModel( NULL );
}
//---------------------------------------------------------------------------
void __fastcall TCBuilderPluginForm::Save1Click(TObject *Sender)
{
	studio.model.saveModel();
}
//---------------------------------------------------------------------------
void __fastcall TCBuilderPluginForm::Close2Click(TObject *Sender)
{
	studio.model.closeModel();
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

