unit MakeChip_Unit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, Var_Unit, Buttons, StdCtrls;

type
  PImage = ^TImage;

  TMakeChipForm = class(TForm)
    DPTPanel: TPanel;
    ChipPanel: TPanel;
    ResetBevel: TBevel;
    ResetButton: TSpeedButton;
    BevelRend: TBevel;
    RendButton: TSpeedButton;
    Chip1Im: TImage;
    Chip2Im: TImage;
    Chip3Im: TImage;
    Chip4Im: TImage;
    Chip5Im: TImage;
    BGImage: TImage;
    DPTBevel: TBevel;
    DPTButton: TSpeedButton;
    BevelOk: TBevel;
    OkButton: TSpeedButton;
    BevelCancel: TBevel;
    CancelButton: TSpeedButton;
    EBLabel: TLabel;
    EBBevel: TBevel;
    EBRB1: TRadioButton;
    EBRB2: TRadioButton;
    EBRBLast: TRadioButton;
    EBEdit: TEdit;
    EBLabel1: TLabel;
    EBLabel2: TLabel;
    EBLabelLast: TLabel;
    CTPanel: TPanel;
    CTBevel: TBevel;
    CTLabel: TLabel;
    CTYes: TRadioButton;
    CTNo: TRadioButton;
    ActPanel: TPanel;
    ActBevel: TBevel;
    ActLabel: TLabel;
    DirLabel: TLabel;
    VLabel: TLabel;
    DirRVPanel: TPanel;
    DirLVPanel: TPanel;
    RVB: TRadioButton;
    RVA: TRadioButton;
    DirUVPanel: TPanel;
    DirDVPanel: TPanel;
    LVB: TRadioButton;
    LVA: TRadioButton;
    UVB: TRadioButton;
    UVA: TRadioButton;
    DVB: TRadioButton;
    DVA: TRadioButton;
    DirRPanel: TPanel;
    DirLPanel: TPanel;
    DirUPanel: TPanel;
    DirDPanel: TPanel;
    DirShape: TShape;
    RFunPanel: TPanel;
    RFun1: TRadioButton;
    RFun2: TRadioButton;
    RFun2Edit: TEdit;
    LFunPanel: TPanel;
    LFun1: TRadioButton;
    LFun2: TRadioButton;
    LFun2Edit: TEdit;
    UFunPanel: TPanel;
    UFun1: TRadioButton;
    UFun2: TRadioButton;
    UFun2Edit: TEdit;
    DFunPanel: TPanel;
    DFun1: TRadioButton;
    DFun2: TRadioButton;
    DFun2Edit: TEdit;
    Fun1Label: TLabel;
    Fun2Label: TLabel;
    Fun1Shape: TShape;
    ArrayBevel: TBevel;
    ArrayButton: TSpeedButton;
    RFun2Label: TLabel;
    LFun2Label: TLabel;
    UFun2Label: TLabel;
    DFun2Label: TLabel;
    EBRB3: TRadioButton;
    EBLabel3: TLabel;
    Shape1: TShape;
    Shape2: TShape;
    procedure ResetButtonClick(Sender: TObject);
    procedure Chip1ImClick(Sender: TObject);
    procedure CancelButtonClick(Sender: TObject);
    procedure OkButtonClick(Sender: TObject);
    procedure RendButtonClick(Sender: TObject);
    procedure DPTButtonClick(Sender: TObject);
    procedure EBRB1Click(Sender: TObject);
    procedure EBLabel1Click(Sender: TObject);
    procedure RFun1Click(Sender: TObject);
    procedure ArrayButtonClick(Sender: TObject);
    procedure FormShow(Sender: TObject);
    procedure RFun2LabelClick(Sender: TObject);
  private
    ActiveImage: PImage;
    MakeChip: array[1..6] of ChipNumType;
    RandChip: array[1..6] of ChipNumType;
    procedure ReDrawChip;
  public
    { Public declarations }
  end;

var
  MakeChipForm: TMakeChipForm;

implementation

uses RDO_Edit_Unit, Game_Unit;

{$R *.DFM}

procedure TMakeChipForm.ReDrawChip;
var i: integer;
begin
  for i := 1 to 5 do
  begin
    case i of
      1: ActiveImage := @Chip1Im;
      2: ActiveImage := @Chip2Im;
      3: ActiveImage := @Chip3Im;
      4: ActiveImage := @Chip4Im;
      5: ActiveImage := @Chip5Im;
    end;
    case MakeChip[i].Mesto of
      1: begin
           ActiveImage^.Left := 48;
           ActiveImage^.Top  := 155;
         end;
      2: begin
           ActiveImage^.Left := 104;
           ActiveImage^.Top  := 155;
         end;
      3: begin
           ActiveImage^.Left := 160;
           ActiveImage^.Top  := 155;
         end;
      4: begin
           ActiveImage^.Left := 48;
           ActiveImage^.Top  := 210;
         end;
      5: begin
           ActiveImage^.Left := 104;
           ActiveImage^.Top  := 210;
         end;
      6: begin
           ActiveImage^.Left := 160;
           ActiveImage^.Top  := 210;
         end;
    end;
  end;
end;

procedure TMakeChipForm.ResetButtonClick(Sender: TObject);
var
  i, j, j1: integer;
  str, str1: string;
begin
  try
    for i := 1 to 6 do
      MakeChip[i].Mesto := OldChip[i].Mesto;
    ReDrawChip;
    // DPT
    with RDOEdit_Game5.DPTRich do
    begin
      // $Evaluate_by
      j := 0;
      repeat
        str := Paragraphs[j];
        i := Pos('$Evaluate_by ', str);
        inc(j);
      until (i > 0) or (j = ParagraphCount);
      if i = 0 then raise Exc.Create('$Evaluate_by');
      str1 := PickOutStr(str, 2);
//      EBEdit.Text := str1;
      if str1 = '0' then EBRB1Click(EBRB1) else
        if str1 = 'Кол_во_фишек_не_на_месте(0)' then EBRB1Click(EBRB2) else
          if str1 = 'Расстояния_фишек_до_мест(0)' then EBRB1Click(EBRB3)
                                                  else EBRB1Click(EBRBLast);
      // $Compare_tops
      j := 0;
      repeat
        str := Paragraphs[j];
        i := Pos('$Compare_tops', str);
        inc(j);
      until (i > 0) or (j = ParagraphCount);
      if i = 0 then raise Exc.Create('$Compare_tops');
      if str[Length(str)] = '=' then str1 := PickOutStr(str, 2)
                                else str1 := PickOutStr(str, 3);
      if str1 = 'YES' then begin CTYES.Checked := true; CTNO.Checked := false end
                      else begin CTYES.Checked := false;CTNO.Checked := true  end;
      // $Activities
      j := 0;
      repeat
        str := Paragraphs[j];
        i := Pos('$Activities', str);
        inc(j);
      until (i > 0) or (j = ParagraphCount);
      if i = 0 then raise Exc.Create('$Activities');
      // Right
      j1 := j;
      repeat
        str := Paragraphs[j1];
        i := Pos('Перемещение_вправо', str);
        inc(j1);
      until (i > 0) or (j1 = ParagraphCount);
      if i = 0 then raise Exc.Create('Перемещение_вправо');
      str1 := PickOutStr(str, 6);
      if str1 = 'before' then RVB.Checked := true
                         else RVB.Checked := false;
      RVA.Checked := not RVB.Checked;
      str1 := PickOutStr(str, 7);
      RFun2Edit.Text := str1;
      if str1 = '1' then RFun1Click(RFun1)
                    else RFun1Click(RFun2);
      // Left
      j1 := j;
      repeat
        str := Paragraphs[j1];
        i := Pos('Перемещение_влево', str);
        inc(j1);
      until (i > 0) or (j1 = ParagraphCount);
      if i = 0 then raise Exc.Create('Перемещение_влево');
      str1 := PickOutStr(str, 6);
      if str1 = 'before' then LVB.Checked := true
                         else LVB.Checked := false;
      LVA.Checked := not LVB.Checked;
      str1 := PickOutStr(str, 7);
      LFun2Edit.Text := str1;
      if str1 = '1' then RFun1Click(LFun1)
                    else RFun1Click(LFun2);
      // Up
      j1 := j;
      repeat
        str := Paragraphs[j1];
        i := Pos('Перемещение_вверх', str);
        inc(j1);
      until (i > 0) or (j1 = ParagraphCount);
      if i = 0 then raise Exc.Create('Перемещение_вверх');
      str1 := PickOutStr(str, 6);
      if str1 = 'before' then UVB.Checked := true
                         else UVB.Checked := false;
      UVA.Checked := not UVB.Checked;
      str1 := PickOutStr(str, 7);
      UFun2Edit.Text := str1;
      if str1 = '1' then RFun1Click(UFun1)
                    else RFun1Click(UFun2);
      // Down
      j1 := j;
      repeat
        str := Paragraphs[j1];
        i := Pos('Перемещение_вниз', str);
        inc(j1);
      until (i > 0) or (j1 = ParagraphCount);
      if i = 0 then raise Exc.Create('Перемещение_вниз');
      str1 := PickOutStr(str, 6);
      if str1 = 'before' then DVB.Checked := true
                         else DVB.Checked := false;
      DVA.Checked := not DVB.Checked;
      str1 := PickOutStr(str, 7);
      DFun2Edit.Text := str1;
      if str1 = '1' then RFun1Click(DFun1)
                    else RFun1Click(DFun2);
    end;
  except
    on E: Exc do raise Exc.Create('В файле точки принятия решений не найдено описание: '+
      MyEOL+E.Message+'.');
    on E: Exception do begin
      RDOEdit_Game5.ShowError('Возникла неизвестная ошибка чтения файла точки принятия решений: '+
        MyEOL+''''+E.Message+'''.'+MyEOL+'Обратитесь к разработчику.');
    end;
  else
    RDOEdit_Game5.ShowError('Возникла неизвестная ошибка чтения файла точки принятия решений.'+
      MyEOL+'Обратитесь к разработчику.');
  end;
end;

procedure TMakeChipForm.Chip1ImClick(Sender: TObject);
var i: integer;
begin
  i := -1;
  if Sender is TImage then
  begin
    with Sender as TImage do
    begin
      if Name = 'Chip1Im' then i := 1;
      if Name = 'Chip2Im' then i := 2;
      if Name = 'Chip3Im' then i := 3;
      if Name = 'Chip4Im' then i := 4;
      if Name = 'Chip5Im' then i := 5;
    end;
    if (MakeChip[i].Mesto <> 3) and (MakeChip[i].Mesto <> 6) and
       (MakeChip[i].Mesto = MakeChip[6].Mesto - 1) then
    begin
      inc(MakeChip[i].Mesto);
      dec(MakeChip[6].Mesto);
    end else
      if (MakeChip[i].Mesto <> 1) and (MakeChip[i].Mesto <> 4) and
         (MakeChip[i].Mesto = MakeChip[6].Mesto + 1) then
      begin
        dec(MakeChip[i].Mesto);
        inc(MakeChip[6].Mesto);
      end else
        if MakeChip[i].Mesto = MakeChip[6].Mesto + 3 then
        begin
          dec(MakeChip[i].Mesto, 3);
          inc(MakeChip[6].Mesto, 3);
        end else
          if MakeChip[i].Mesto = MakeChip[6].Mesto - 3 then
          begin
            inc(MakeChip[i].Mesto, 3);
            dec(MakeChip[6].Mesto, 3);
          end else exit;
    ReDrawChip;
  end;
end;

procedure TMakeChipForm.CancelButtonClick(Sender: TObject);
begin
  ModalResult := mrCancel;
end;

procedure TMakeChipForm.OkButtonClick(Sender: TObject);
var
  i: integer;
  str, str1: string;
begin
  RDOEdit_Game5.TRCScanButton.Enabled := false;
  if TopBoxFirst = nil then RDOEdit_Game5.ShowGraphButton.Enabled := false;
  // RSS
  with RDOEdit_Game5.RSSRich do
  begin
    Clear;
    SetSelTextBuf(PChar('$Resources'+MyEOL+
                        '  Фишка1 : Фишка 1 ' + IntToStr(MakeChip[1].Mesto)+MyEOL+
                        '  Фишка2 : Фишка 2 ' + IntToStr(MakeChip[2].Mesto)+MyEOL+
                        '  Фишка3 : Фишка 3 ' + IntToStr(MakeChip[3].Mesto)+MyEOL+
                        '  Фишка4 : Фишка 4 ' + IntToStr(MakeChip[4].Mesto)+MyEOL+
                        '  Фишка5 : Фишка 5 ' + IntToStr(MakeChip[5].Mesto)+MyEOL+
                        '  Дырка  : Дырка_t ' + IntToStr(MakeChip[6].Mesto)+MyEOL+
                        '$End'));
  end;
  for i := 1 to 6 do
    OldChip[i].Mesto := MakeChip[i].Mesto;
  // DPT
  with RDOEdit_Game5.DPTRich do
  begin
    Clear;
    if EBRB1.Checked then str := '0' else
      if EBRB2.Checked then str := 'Кол_во_фишек_не_на_месте(0)' else
        if EBRB3.Checked then str := 'Расстояния_фишек_до_мест(0)' else
          if EBRBLast.Checked then str := EBEdit.Text;
    SetSelTextBuf(PChar('$Decision_point Расстановка_фишек : search trace_all'+MyEOL+
                        '$Condition Exist(Фишка: Фишка.Номер <> Фишка.Местоположение)'+MyEOL+
                        '$Term_condition'+MyEOL+
                        '  For_All(Фишка: Фишка.Номер = Фишка.Местоположение)'+MyEOL+
                        '$Evaluate_by '+str+MyEOL+
                        '$Compare_tops = '));
    if CTYes.Checked then str := 'YES' else str := 'NO';
    SetSelTextBuf(PChar(str+MyEOL+
                        '$Activities'+MyEOL+
                        '  Перемещение_вправо: Перемещение_фишки справа  1 value '));
    if RVB.Checked then str1 := 'before' else str1 := 'after';
    if RFun1.Checked then str := '1'
                     else str := RFun2Edit.Text;
    SetSelTextBuf(PChar(str1+' '+str+MyEOL+
                        '  Перемещение_влево : Перемещение_фишки слева  -1 value '));
    if LFun1.Checked then str := '1'
                     else str := LFun2Edit.Text;
    if LVB.Checked then str1 := 'before' else str1 := 'after';
    SetSelTextBuf(PChar(str1+' '+str+MyEOL+
                        '  Перемещение_вверх : Перемещение_фишки сверху -3 value '));
    if UVB.Checked then str1 := 'before' else str1 := 'after';
    if UFun1.Checked then str := '1'
                     else str := UFun2Edit.Text;
    SetSelTextBuf(PChar(str1+' '+str+MyEOL+
                        '  Перемещение_вниз  : Перемещение_фишки снизу   3 value '));
    if DVB.Checked then str1 := 'before' else str1 := 'after';
    if DFun1.Checked then str := '1'
                     else str := DFun2Edit.Text;
    SetSelTextBuf(PChar(str1+' '+str+MyEOL+
                        '$End'+MyEOL));
  end;
  ModalResult := mrOk;
end;

procedure TMakeChipForm.RendButtonClick(Sender: TObject);
var
  i, j: integer;
  flag: boolean;
begin
  repeat
    RandChip[1].Mesto := Random(6)+1;
    RandChip[4].Mesto := Random(6)+1;
    RandChip[3].Mesto := Random(6)+1;
    RandChip[5].Mesto := Random(6)+1;
    RandChip[2].Mesto := Random(6)+1;
    RandChip[6].Mesto := Random(6)+1;
    i := Random(5)+1;
    case i of
      1: Chip1ImClick(Chip1Im);
      2: Chip1ImClick(Chip2Im);
      3: Chip1ImClick(Chip3Im);
      4: Chip1ImClick(Chip4Im);
      5: Chip1ImClick(Chip5Im);
    end;
    Application.ProcessMessages;
    flag := true;
    for i := 1 to 5 do
      for j := i+1 to 6 do
        if RandChip[i].Mesto = RandChip[j].Mesto then flag := false;
  until flag;
  for i := 1 to 6 do
    MakeChip[i].Mesto := RandChip[i].Mesto;
  ReDrawChip;
end;

procedure TMakeChipForm.DPTButtonClick(Sender: TObject);
begin
  MakeChipForm.ClientWidth := 533;
  DPTButton.Visible := false;
  DPTBevel.Visible  := false;
end;

procedure TMakeChipForm.EBRB1Click(Sender: TObject);
begin
  if Sender is TRadioButton then
  begin
    with Sender as TRadioButton do
    begin
      Checked := true;
      if Name = 'EBRBLast'
        then begin
          EBEdit.Enabled := true;
          EBEdit.Color   := clWindow;
          EBEdit.SetFocus;
        end else begin
          EBEdit.Enabled := false;
          EBEdit.Color   := clInactiveBorder;
        end;
    end;
  end;
end;

procedure TMakeChipForm.EBLabel1Click(Sender: TObject);
begin
  if Sender is TLabel then
    with Sender as TLabel do
    begin
      if Name = 'EBLabel1' then EBRB1Click(EBRB1);
      if Name = 'EBLabel2' then EBRB1Click(EBRB2);
      if Name = 'EBLabel3' then EBRB1Click(EBRB3);
      if Name = 'EBLabelLast' then EBRB1Click(EBRBLast);
    end;
end;

procedure TMakeChipForm.RFun1Click(Sender: TObject);
var ActiveEdit: ^TEdit;
begin
  if Sender is TRadioButton then
    with Sender as TRadioButton do
    begin
      Checked := true;
      ActiveEdit := nil;
      if Pos('R', Name) > 0 then ActiveEdit := @RFun2Edit;
      if Pos('L', Name) > 0 then ActiveEdit := @LFun2Edit;
      if Pos('U', Name) > 0 then ActiveEdit := @UFun2Edit;
      if Pos('D', Name) > 0 then ActiveEdit := @DFun2Edit;
      if Pos('2', Name) > 0 then
      begin
        ActiveEdit^.Enabled := true;
        ActiveEdit^.Color   := clWindow;
        ActiveEdit^.SetFocus;
      end
      else begin
        ActiveEdit^.Enabled := false;
        ActiveEdit^.Color   := clInactiveBorder;
      end;
    end;
end;

procedure TMakeChipForm.ArrayButtonClick(Sender: TObject);
var i: integer;
begin
  for i := 1 to 6 do
    MakeChip[i].Mesto := i;
  ReDrawChip;
end;

procedure TMakeChipForm.FormShow(Sender: TObject);
begin
  MakeChipForm.ClientWidth := 262;
  DPTButton.Visible := true;
  DPTBevel.Visible  := true;
  ResetButtonClick(Self);
end;

procedure TMakeChipForm.RFun2LabelClick(Sender: TObject);
begin
  if Sender is TLabel then
    with Sender as TLabel do
      begin
        if Name = 'RFun2Label' then RFun1Click(RFun2);
        if Name = 'LFun2Label' then RFun1Click(LFun2);
        if Name = 'UFun2Label' then RFun1Click(UFun2);
        if Name = 'DFun2Label' then RFun1Click(DFun2);
      end;
end;

end.
