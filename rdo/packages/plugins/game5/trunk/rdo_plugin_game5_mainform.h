//---------------------------------------------------------------------------

#ifndef rdo_plugin_game5_mainformH
#define rdo_plugin_game5_mainformH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <rdoplugin.h>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <vector>
//---------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *N51;
	TMenuItem *Close1;
	TMenuItem *Source;
        TMenuItem *N2;
	TPanel *ChipPanel;
	TImage *Image1;
	TBevel *BevelRend;
	TSpeedButton *RendButton;
	TImage *Chip1Im;
	TImage *Chip2Im;
	TImage *Chip3Im;
	TImage *Chip4Im;
	TImage *Chip5Im;
	TBevel *DPTBevel;
	TSpeedButton *DPTButton;
	TBevel *BevelOk;
	TSpeedButton *OkButton;
	TBevel *ArrayBevel;
	TSpeedButton *ArrayButton;
	TPanel *DPTPanel;
	TBevel *EBBevel;
	TLabel *EBLabel;
	TLabel *EBLabel1;
	TLabel *EBLabel2;
	TLabel *EBLabel3;
	TRadioButton *EBRB1;
	TRadioButton *EBRB2;
	TRadioButton *EBRB3;
	TEdit *EBEdit;
	TPanel *CTPanel;
	TBevel *CTBevel;
	TLabel *CTLabel;
	TRadioButton *CTYes;
	TRadioButton *CTNo;
	TPanel *ActPanel;
	TBevel *ActBevel;
	TLabel *ActLabel;
	TLabel *DirLabel;
	TLabel *VLabel;
	TShape *DirShape;
	TLabel *Fun1Label;
	TLabel *Fun2Label;
	TShape *Fun1Shape;
	TPanel *DirRVPanel;
	TRadioButton *RVB;
	TRadioButton *RVA;
	TPanel *DirLVPanel;
	TRadioButton *LVB;
	TRadioButton *LVA;
	TPanel *DirUVPanel;
	TRadioButton *UVB;
	TRadioButton *UVA;
	TPanel *DirDVPanel;
	TRadioButton *DVB;
	TRadioButton *DVA;
	TPanel *DirRPanel;
	TPanel *DirLPanel;
	TPanel *DirUPanel;
	TPanel *DirDPanel;
	TPanel *RFunPanel;
	TLabel *RFun2Label;
	TRadioButton *RFun1;
	TRadioButton *RFun2;
	TEdit *RFun2Edit;
	TPanel *LFunPanel;
	TLabel *LFun2Label;
	TRadioButton *LFun1;
	TRadioButton *LFun2;
	TEdit *LFun2Edit;
	TPanel *UFunPanel;
	TLabel *UFun2Label;
	TRadioButton *UFun1;
	TRadioButton *UFun2;
	TEdit *UFun2Edit;
	TPanel *DFunPanel;
	TLabel *DFun2Label;
	TRadioButton *DFun1;
	TRadioButton *DFun2;
	TEdit *DFun2Edit;
	TLabel *Label1;
	TRadioButton *RadioButton1;
	TShape *Shape1;
	TShape *Shape2;
	TShape *Shape3;
	TSpeedButton *CancelButton;
	TBevel *BevelCancel;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Close1Click(TObject *Sender);
        void __fastcall SourceClick(TObject *Sender);
	void __fastcall RFun1Click(TObject *Sender);
	void __fastcall DPTButtonClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall OkButtonClick(TObject *Sender);
	void __fastcall RendButtonClick(TObject *Sender);
	void __fastcall Chip1ImClick(TObject *Sender);

protected:
    HINSTANCE hInstance;
    struct ChipNumType {
    	int  mesto;
        bool look;
        ChipNumType( int _mesto ): mesto( _mesto ), look( false ) {};
    };
    std::vector< ChipNumType > MakeChip;
	void ReDrawChip();

public:
	rdoPlugin::Studio studio;
	__fastcall TMainForm( TComponent* Owner, const rdoPlugin::Studio& studio, HINSTANCE _hInstance );
};
//---------------------------------------------------------------------------
extern PACKAGE TMainForm *MainForm;
//---------------------------------------------------------------------------
#endif
