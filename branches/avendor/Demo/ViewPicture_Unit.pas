unit ViewPicture_Unit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls;

type

  TViewPictureForm = class(TForm)
    ViewPanel: TPanel;
    ViewImage: TImage;
  end;

var
  ViewPictureForm: TViewPictureForm;

implementation

{$R *.DFM}

end.
