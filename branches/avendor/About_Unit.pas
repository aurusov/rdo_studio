unit About_Unit;

interface

uses
  SysUtils, Forms, Classes, Controls, StdCtrls, ExtCtrls;

type

  TAboutForm = class(TForm)
    AboutImageBMSTU: TImage;
    AboutLabelAddres: TLabel;
    AboutBevel: TBevel;
    OKButton: TButton;
    AboutLabelTel: TLabel;
    AboutLabel: TLabel;
    AboutNameLabel1: TLabel;
    AboutNameLabel2: TLabel;
    procedure FormShow(Sender: TObject);
  end;

var
  AboutForm: TAboutForm;

implementation

uses ConstRes_Unit;

{$R *.DFM}

procedure TAboutForm.FormShow(Sender: TObject);
begin
  Caption                  := LoadStr(AboutCaption);
  AboutLabel.Caption       := LoadStr(AboutLabelCaption);
  AboutNameLabel1.Caption  := LoadStr(AboutNameLabelCaption1);
  AboutNameLabel2.Caption  := LoadStr(AboutNameLabelCaption2);
  AboutLabelAddres.Caption := LoadStr(AboutLabelAddresCaption);
  AboutLabelTel.Caption    := LoadStr(AboutLabelTelCaption);
end;


end.
