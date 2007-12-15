unit Result_Top_Unit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, StdCtrls, ToolWin, ComCtrls, Buttons, Var_Unit;

type
  PImage = ^TImage;

  TResultTopForm = class(TForm)
    BGImage: TImage;
    TopL: TLabel;
    TopBossL: TLabel;
    MoneyPathL: TLabel;
    MoneyEndPathL: TLabel;
    MoneyRuleL: TLabel;
    NumberChipL: TLabel;
    DirectL: TLabel;
    DirectT1: TLabel;
    DirectT2: TLabel;
    MoneyRuleT: TLabel;
    BackButton: TSpeedButton;
    NextButton: TSpeedButton;
    CloseButton: TSpeedButton;
    MainL: TLabel;
    TopT: TLabel;
    TopBossT: TLabel;
    MoneyPathT: TLabel;
    MoneyEndPathT: TLabel;
    NumberChipT: TLabel;
    MainT: TLabel;
    Chip1Im: TImage;
    Chip2Im: TImage;
    Chip3Im: TImage;
    Chip4Im: TImage;
    Chip5Im: TImage;
    ResultEndImage: TImage;
    procedure CloseButtonClick(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure BackButtonClick(Sender: TObject);
    procedure NextButtonClick(Sender: TObject);
  private
    ChipM: array[1..5] of ChipNumType;
    ActiveImage: PImage;
  public
  end;

var
  ResultTopForm: TResultTopForm;

implementation

uses Game_Unit, TopBox_Unit, RDO_Edit_Unit;

{$R *.DFM}

procedure TResultTopForm.CloseButtonClick(Sender: TObject);
begin
  Close;
end;

procedure TResultTopForm.FormShow(Sender: TObject);
var
  TopTemp: PTopBox;
  i      : integer;
begin
  ResultEndImage.Visible := false;
  if GraphForm.ActiveTopBox <> nil then
  begin
    with GraphForm.ActiveTopBox^ do
    begin
      TopT.Caption          := IntToStr(NumberTopBox);
      TopBossT.Caption      := IntToStr(TopBoss);
      MoneyPathT.Caption    := FloatToStr(MoneyPath);
      MoneyEndPathT.Caption := FloatToStr(MoneyEndPath);
      MoneyRuleT.Caption    := FloatToStr(MoneyRule);
      NumberChipT.Caption   := IntToStr(NumberChip);
      case Direct of
        cdRight : begin
                    DirectT1.Caption := 'вправо';
                    DirectT2.Caption := '(с '+IntToStr(ChipMesto-1)+' на '+
                      IntToStr(ChipMesto)+')';
                  end;
        cdLeft  : begin
                    DirectT1.Caption := 'влево';
                    DirectT2.Caption := '(с '+IntToStr(ChipMesto+1)+' на '+
                      IntToStr(ChipMesto)+')';
                  end;
        cdTop   : begin
                    DirectT1.Caption := 'вверх';
                    DirectT2.Caption := '(с '+IntToStr(ChipMesto+3)+' на '+
                      IntToStr(ChipMesto)+')';
                  end;
        cdBottom: begin
                    DirectT1.Caption := 'вниз';
                    DirectT2.Caption := '(с '+IntToStr(ChipMesto-3)+' на '+
                      IntToStr(ChipMesto)+')';
                  end;
        cdNone  : begin
                    DirectT1.Caption := 'начало поиска';
                    DirectT2.Caption := '';
                  end;
        cdError : begin
                    DirectT1.Caption := 'ошибка';
                    DirectT2.Caption := '';
                  end;
      end;
      BackButton.Enabled := true;
      NextButton.Enabled := true;
      if MainTop then
      begin
        MainT.Font.Color := clSilver;
        MainT.Caption    := 'да';
      end else
      begin
        MainT.Font.Color := clNavy;
        MainT.Caption    := 'нет';
      end;
      if NumberTopBox = 1 then
      begin
        BackButton.Enabled := false;
        MainT.Font.Color := clNavy;
        MainT.Caption := '---';
      end;
      if FindChild(GraphForm.ActiveTopBox) = nil then
        NextButton.Enabled := false;
    end;
    for i := 1 to 5 do
    begin
      ChipM[i].Mesto := OldChip[i].Mesto;
      ChipM[i].Look := false;
    end;
    TopTemp := GraphForm.ActiveTopBox;
    repeat
      if not ChipM[TopTemp^.NumberChip].Look then
      begin
        ChipM[TopTemp^.NumberChip].Mesto:= TopTemp^.ChipMesto;
        ChipM[TopTemp^.NumberChip].Look := true;
      end;
      if TopTemp^.NumberTopBox <> 1 then TopTemp := FindTopBox(TopTemp^.TopBoss);
    until TopTemp^.NumberTopBox = 1;
    for i := 1 to 5 do
    begin
      case i of
        1: ActiveImage := @Chip1Im;
        2: ActiveImage := @Chip2Im;
        3: ActiveImage := @Chip3Im;
        4: ActiveImage := @Chip4Im;
        5: ActiveImage := @Chip5Im;
      end;
      case ChipM[i].Mesto of
        1: begin
             ActiveImage^.Left := 46;
             ActiveImage^.Top  := 153;
           end;
        2: begin
             ActiveImage^.Left := 102;
             ActiveImage^.Top  := 153;
           end;
        3: begin
             ActiveImage^.Left := 158;
             ActiveImage^.Top  := 153;
           end;
        4: begin
             ActiveImage^.Left := 46;
             ActiveImage^.Top  := 208;
           end;
        5: begin
             ActiveImage^.Left := 102;
             ActiveImage^.Top  := 208;
           end;
        6: begin
             ActiveImage^.Left := 158;
             ActiveImage^.Top  := 208;
           end;
      end;
    end;
  end;
  if (ChipM[1].Mesto = 1) and (ChipM[2].Mesto = 2) and (ChipM[3].Mesto = 3) and
     (ChipM[4].Mesto = 4) and (ChipM[5].Mesto = 5) then
    ResultEndImage.Visible := true;
end;

procedure TResultTopForm.BackButtonClick(Sender: TObject);
begin
  with GraphForm do
  begin
    ActiveTopBox := FindTopBox(ActiveTopBox^.TopBoss);
    if ActiveTopBox = nil then raise Exc.Create('Не найден передок для вершины.');
    TopScrollBox.ScrollInView(ActiveTopBox^);
  end;
end;

procedure TResultTopForm.NextButtonClick(Sender: TObject);
var TopTemp: PTopBox;
begin
  with GraphForm do
  begin
    TopTemp := FindMainChild(ActiveTopBox);
    if TopTemp = nil then TopTemp := FindChild(ActiveTopBox);
    ActiveTopBox := TopTemp;
    if ActiveTopBox <> nil then
    begin
      if FindChild(ActiveTopBox) = nil then
        NextButton.Enabled := false;
    end else raise Exc.Create('Не найден потомок для вершины.');
    TopScrollBox.ScrollInView(GraphForm.ActiveTopBox^);
  end;
end;

end.
