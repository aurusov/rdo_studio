program Game5_Project;

uses
  Forms,
  Game_Unit in 'Game_Unit.pas' {GraphForm},
  TopBox_Unit in 'TopBox_Unit.pas',
  Var_Unit in 'Var_Unit.pas',
  Result_Unit in 'Result_Unit.pas' {ResultForm},
  Result_Top_Unit in 'Result_Top_Unit.pas' {ResultTopForm},
  RDO_Edit_Unit in 'RDO_Edit_Unit.pas' {RDOEdit_Game5},
  MakeChip_Unit in 'MakeChip_Unit.pas' {MakeChipForm},
  hh in 'hh.pas',
  hh_funcs in 'hh_funcs.pas',
  About_Unit in 'About_Unit.pas' {AboutForm};

{$R *.RES}

begin
  Application.Initialize;
  Application.Title := 'Игра 5';
  Application.CreateForm(TRDOEdit_Game5, RDOEdit_Game5);
  Application.CreateForm(TGraphForm, GraphForm);
  Application.CreateForm(TResultTopForm, ResultTopForm);
  Application.CreateForm(TResultForm, ResultForm);
  Application.CreateForm(TMakeChipForm, MakeChipForm);
  Application.CreateForm(TAboutForm, AboutForm);
  Application.Run;
end.
