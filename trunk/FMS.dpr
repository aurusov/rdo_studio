program FMS;

uses
  Forms,
  FMS_Unit in 'FMS_Unit.pas' {FMSForm},
  ConstRes_Unit in 'ConstRes_Unit.pas',
  About_Unit in 'About_Unit.pas' {AboutForm},
  Misc_Unit in 'Misc_Unit.pas',
  Const_Unit in 'Const_Unit.pas',
  FMSObjects_Unit in 'FMSObjects_Unit.pas',
  Var_Unit in 'Var_Unit.pas',
  Map_Unit in 'Map_Unit.pas' {MapForm},
  ViewPicture_Unit in 'ViewPicture_Unit.pas' {ViewPictureForm};

{$R *.RES}

begin
  Application.Initialize;
  Application.Title := 'Моделирование компоновок';
  Application.HelpFile := 'Fms.hlp';
  Application.CreateForm(TFMSForm, FMSForm);
  Application.Run;
end.
