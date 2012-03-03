//---------------------------------------------------------------------------
#ifndef RDOPLUGINCBUILDERFORM_H
#define RDOPLUGINCBUILDERFORM_H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include <rdoplugin.h>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TCBuilderPluginForm: public TForm
{
__published:	// IDE-managed Components
	TMemo *Memo;
	TMainMenu *MainMenu;
	TMenuItem *Plugin1;
	TMenuItem *Close1;
	TMenuItem *Model1;
	TMenuItem *New1;
	TMenuItem *Open1;
	TMenuItem *Save1;
	TMenuItem *Close2;
	TMenuItem *N1;
	TMenuItem *Build1;
	TMenuItem *Execute1;
	TMenuItem *Stop1;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Close1Click(TObject *Sender);
	void __fastcall New1Click(TObject *Sender);
	void __fastcall Open1Click(TObject *Sender);
	void __fastcall Save1Click(TObject *Sender);
	void __fastcall Close2Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Build1Click(TObject *Sender);
	void __fastcall Execute1Click(TObject *Sender);
	void __fastcall Stop1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TCBuilderPluginForm( TComponent* Owner, const rdoPlugin::Studio* studio );

	// one static "studio" variable in dll for each *.exe file
	rdoPlugin::Studio studio;

	void insertLine( const char* line );
    void updateControlsState();
};
//---------------------------------------------------------------------------
extern PACKAGE TCBuilderPluginForm* CBuilderPluginForm;
//---------------------------------------------------------------------------
#endif // RDOPLUGINCBUILDERFORM_H
