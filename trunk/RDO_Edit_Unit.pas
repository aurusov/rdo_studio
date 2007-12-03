unit RDO_EDIT_Unit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  Buttons, ExtCtrls, Menus, IniFiles, ComCtrls, StdCtrls, RichEdit, ToolWin,
  WinTypes, WinProcs, Plusmemo, RXCtrls, DrSpeedButton, DrMemoRich, hh,
  About_Unit, Shellapi;

{$H+}

const
  MyEOL = #13+#10;
  EndRdoWordReserved = 120; // Кол-во ключевых слов RDO
  EndRdoWordTrace    = 6;   // Кол-во trace
  EndRdoWordFunction = 29;  // Кол-во функций
  // Кол-во ключевых слов RDO (общее)
  EndRdoWord = EndRdoWordReserved + EndRdoWordTrace + EndRdoWordFunction;
  RdoWord : array [1..EndRdoWord] of string = ('$Resource_type', '$Parameters',
    '$End', 'temporary', 'permanent', 'integer', 'real', 'such_as',
    '$Resources', '$Constant', '$Function', '$Type',
    '$Body', 'list', 'table', 'algorithmic', 'Calculate_if', '$Sequence',
    'uniform', 'exponential', 'normal', 'by_hist', 'enumerative',
    '$Operations', '$Pattern', '$Relevant_resources', '$Time', 'operation',
    'irregular_event', 'rule', 'keyboard', 'Keep', 'Create', 'Erase',
    'NonExist', 'NoChange', 'Convert_begin', 'Convert_end', 'Convert_event',
    'Convert_rule', 'Choice', 'from', 'NoCheck', 'first', 'with_max',
    'with_min', 'set', '$Operation', '$Frame', 'Show_if', '$Back_picture',
    '$End_picture', 'Show', '$Results', 'watch_par', 'watch_state',
    'watch_quant', 'watch_value', 'get_value', 'Model_name', 'Resource_file',
    'OprIev_file', 'Frame_file', 'Statistic_file', 'Results_file',
    'Trace_file', 'Show_mode', 'NoShow', 'Monitor', 'Animation', 'Frame_number',
    'Show_rate', 'Run_StartTime', 'Trace_StartTime', 'Trace_EndTime',
    'Terminate_if', 'Break_point', 'or', 'and', 'Системное_время', '$Include',
    '$Default', 'all1', 'Get_value', 'Run_file', '$Changes', '$Status',
    'USER_BREAK', '$Result_values', 'EventCount', 'OperRuleCheckCounter',
    'AExpCalcCounter', 'BExpCalcCounter', 'Time_now', 'text', 'bitmap', 'rect',
    'line', 'ellipse', 'r_rect', 'triang', 's_bmp', 'active', 'YES', 'NO',
    'NORMAL_TERMINATION', 'NO_MORE_EVENTS', 'RUN_TIME_ERR', '$Decision_point',
    '$Condition', '$Term_condition', '$Evaluate_by', '$Compare_tops', 'some',
    'search', '$Compare_tops','$Activities', 'value before', 'value after',
    '$Watching',
    '$Tracing', 'no_trace', 'trace_stat', 'trace_tops', 'trace_all', 'trace',
    'Abs', 'ArcCos', 'ArcSin', 'ArcTan', 'Cos', 'Cotan', 'Exp', 'Floor', 'Frac',
    'IAbs', 'IMax', 'IMin', 'Int', 'IntPower', 'Ln', 'Log10', 'Log2', 'LogN',
    'Max', 'Min', 'Power', 'Round', 'Sin', 'Sqrt', 'Tan', 'Exist', 'Not_Exist',
    'For_All', 'Not_For_All');

type

  Exc   = class(Exception);
  ExcUp = class(Exception);

  TMemoRichColor = record
    ColorFg, ColorBg: TColor;
    Style: TFontStyles;
  end;

  PRich = ^TDrMemoRich;

  TRDOEdit_Game5 = class(TForm)
    FileDialog: TOpenDialog;
    MainMenu: TMainMenu;
    File1: TMenuItem;
    Exit1: TMenuItem;
    Run1: TMenuItem;
    Path1: TMenuItem;
    FindDialog: TFindDialog;
    ReplaceDialog: TReplaceDialog;
    Edit05: TMenuItem;
    Replace1: TMenuItem;
    Find1: TMenuItem;
    N3: TMenuItem;
    Paste1: TMenuItem;
    Copy1: TMenuItem;
    Cut1: TMenuItem;
    N4: TMenuItem;
    Undo1: TMenuItem;
    Repeate1: TMenuItem;
    PopupMenu: TPopupMenu;
    Replace2: TMenuItem;
    Find2: TMenuItem;
    Paste2: TMenuItem;
    Copy2: TMenuItem;
    Cut2: TMenuItem;
    N6: TMenuItem;
    Run2: TMenuItem;
    N2: TMenuItem;
    Repeate2: TMenuItem;
    PageControl: TPageControl;
    PATSheet: TTabSheet;
    RTPSheet: TTabSheet;
    RSSSheet: TTabSheet;
    FUNSheet: TTabSheet;
    SMRSheet: TTabSheet;
    TRCSheet: TTabSheet;
    StatusBar: TStatusBar;
    TRCScan: TMenuItem;
    MainToolBar: TToolBar;
    RunButton: TDrSpeedButton;
    FindButton: TDrSpeedButton;
    ReplaceButton: TDrSpeedButton;
    TRCScanButton: TDrSpeedButton;
    PATRich: TDrMemoRich;
    DPTSheet: TTabSheet;
    RTPRich: TDrMemoRich;
    RSSRich: TDrMemoRich;
    FUNRich: TDrMemoRich;
    DPTRich: TDrMemoRich;
    SMRRich: TDrMemoRich;
    TRCRich: TDrMemoRich;
    FileSaveDialog: TSaveDialog;
    SourcePageControl: TPageControl;
    PATSourceSheet: TTabSheet;
    RTPSourceSheet: TTabSheet;
    RSSSourceSheet: TTabSheet;
    FUNSourceSheet: TTabSheet;
    DPTSourceSheet: TTabSheet;
    SMRSourceSheet: TTabSheet;
    PATSource: TRichEdit;
    RTPSource: TRichEdit;
    RSSSource: TRichEdit;
    FUNSource: TRichEdit;
    DPTSource: TRichEdit;
    SMRSource: TRichEdit;
    TRCSouceSheet: TTabSheet;
    TRCSource: TRichEdit;
    ShowGraphButton: TDrSpeedButton;
    ToolButton1: TToolButton;
    ToolButton4: TToolButton;
    MakeChipButton: TDrSpeedButton;
    Showgraph1: TMenuItem;
    Makeit1: TMenuItem;
    N1: TMenuItem;
    N5: TMenuItem;
    Help1: TMenuItem;
    Help2: TMenuItem;
    Guide1: TMenuItem;
    About1: TMenuItem;
    N7: TMenuItem;
    Metoda1: TMenuItem;
    procedure LoadButtonClick(Sender: TObject);
    procedure SaveButtonClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
    procedure RunButtonClick(Sender: TObject);
    procedure FormClose(Sender: TObject; var Action: TCloseAction);
    procedure RdoPathButtonClick(Sender: TObject);
    procedure Exit1Click(Sender: TObject);
    procedure FindButtonClick(Sender: TObject);
    procedure ReplaceButtonClick(Sender: TObject);
    procedure NewOpenButtonClick(Sender: TObject);
    procedure FindDialogFind(Sender: TObject);
    procedure Repeate1Click(Sender: TObject);
    procedure ReplaceDialogFind(Sender: TObject);
    procedure ReplaceDialogReplace(Sender: TObject);
    procedure Cut1Click(Sender: TObject);
    procedure Copy1Click(Sender: TObject);
    procedure Paste1Click(Sender: TObject);
    procedure Undo1Click(Sender: TObject);
    procedure PATRichChange(Sender: TObject);
    procedure PATRichMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure PATRichEnter(Sender: TObject);
    procedure PATRichKeyDown(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure PageControlChange(Sender: TObject);
    procedure TRCScanButtonClick(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure Saveas1Click(Sender: TObject);
    procedure FindDialogClose(Sender: TObject);
    procedure ReplaceDialogClose(Sender: TObject);
    procedure ReplaceDialogShow(Sender: TObject);
    procedure FindDialogShow(Sender: TObject);
    procedure N11Click(Sender: TObject);
    procedure ShowGraphButtonClick(Sender: TObject);
    procedure MakeChipButtonClick(Sender: TObject);
    procedure SaveButtonEnabled(Sender: TObject);
    procedure RunButtonEnabled(Sender: TObject);
    procedure FindButtonEnabled(Sender: TObject);
    procedure TRCScanButtonEnabled(Sender: TObject);
    procedure ShowGraphButtonEnabled(Sender: TObject);
    procedure MakeChipButtonEnabled(Sender: TObject);
    procedure Help2Click(Sender: TObject);
    procedure About1Click(Sender: TObject);
    procedure Guide1Click(Sender: TObject);
    procedure Metoda1Click(Sender: TObject);
  private
    CaptionFont: TFont;
    pi         : TProcessInformation;
    ActiveRich : PRich;
    MySearch   : (Tekughiy, Nachalo);
    MyRepeate  : (None, MyFind, MyReplace);
    MyRepeatAll: boolean;
    LineSearch, PosSearch, OldLineSearch          : integer;
    LineReplace, PosReplace, ReplaceSelStart      : integer;
    FFileChange, FMyFileOpen, FRdoSelect          : boolean;
    RdoFileFullName,  RdoImitatorPathName         : string;
    FRdoFileName, FRdoFileExt, FRdoFileDir        : string;
    function  MyExtractFileName(Value: string): string;
    function  SaveIfFileChange: boolean;
    function  SetActiveRich(Value: string): PRich;
    function  FindActiveRich: PRich;
    function  CheckChar(ch: char): boolean;
    function  FindPickOut: string;
    function  GetRdoFileDir: string;
    function  GetRdoFileName: string;
    function  GetRdoFileExt: string;
    function  GetNormalDir( dir: string ): string;
    function  GetTempDir: string;
    procedure SetRdoFileDir(Value: string);
    procedure SetRdoFileName(Value: string);
    procedure SetRdoFileExt(Value: string);
    procedure DeleteReopenItem(Value: integer);
    procedure LoadReopenIni;
    procedure SaveReopenIni;
    procedure ShowSelection(Value: boolean);
    procedure SetBeginKey(MR: PRich);
    procedure SetBeginStartStop(MR: PRich);
    procedure SaveAll;
    procedure SetFileChange(Value: boolean);
    procedure SetMyFileOpen(Value: boolean);
    procedure SetRdoSelect(Value: boolean);
    procedure GetCursorPosition;
    procedure LoadProject;
    procedure LoadRdoFile;
    procedure MyException(Sender: TObject; E: Exception);
    procedure CheckRdo;
    function getFullHelpFileName: string;
    property FileChange: boolean read FFileChange write SetFileChange;
    property MyFileOpen: boolean read FMyFileOpen write SetMyFileOpen;
    property RdoSelect : boolean read FRdoSelect  write SetRdoSelect;
  public
    IniFile: TIniFile;
    TextModelColor, CommentColor, ReservedColor          : TMemoRichColor;
    FunctionColor, TraceColor, ParamRnColor, ParamSqColor: TMemoRichColor;
    procedure SetAllMemoRichColor;
    procedure OnlyTextColor;
    procedure OnlyStartStopColor;
    procedure OnlyReservedColor;
    procedure OnlyTraceColor;
    procedure OnlyFunctionColor;
    procedure OnlyAplly;
    procedure ShowError(Value: string);
    property RdoFileDir   : string read GetRdoFileDir  write SetRdoFileDir;
    property RdoFileName  : string read GetRdoFileName write SetRdoFileName;
    property RdoFileExt   : string read GetRdoFileExt  write SetRdoFileExt;
  end;

  TLocal_Mes_Hook = procedure;
  procedure SearchRSS;

var
  RDOEdit_Game5: TRDOEdit_Game5;

implementation

uses Game_Unit, Var_Unit, TopBox_Unit, Result_Unit, Result_Top_Unit,
  MakeChip_Unit;

{$R *.DFM}

procedure TRDOEdit_Game5.CheckRdo;
begin
  if not FileExists(RdoImitatorPathName) then
  begin
    RdoSelect := false;
    IniFile.WriteString('Application', 'RDO path', '');
    Application.MessageBox('Не найден РДО-имитатор', 'РДО-редактор', MB_ICONERROR);
  end;
end;

procedure SearchRSS;
var
  i, j, j1: integer;
  str: string;
  ChipTemp: array[1..6] of ChipNumType;
begin
try
  try
    RDOEdit_Game5.ShowGraphButton.Enabled := false;
    // RSS
    with RDOEdit_Game5.RSSRich do
    begin
      j := 0;
      repeat
        str := Paragraphs[j];
        i := Pos('$Resources', str);
        inc(j);
      until (i > 0) or (j = ParagraphCount);
      if i = 0 then raise ExcUp.Create('$Resources');
      // Фишка1
      j1 := j;
      repeat
        str := Paragraphs[j1];
        i := Pos('Фишка1', str);
        inc(j1);
      until (i > 0) or (j1 = ParagraphCount);
      if i = 0 then raise ExcUp.Create('Фишка1');
      ChipTemp[1].Mesto := PickOut(str, 4);
      // Фишка2
      j1 := j;
      repeat
        str := Paragraphs[j1];
        i := Pos('Фишка2', str);
        inc(j1);
      until (i > 0) or (j1 = ParagraphCount);
      if i = 0 then raise ExcUp.Create('Фишка2');
      ChipTemp[2].Mesto := PickOut(str, 4);
      // Фишка3
      j1 := j;
      repeat
        str := Paragraphs[j1];
        i := Pos('Фишка3', str);
        inc(j1);
      until (i > 0) or (j1 = ParagraphCount);
      if i = 0 then raise ExcUp.Create('Фишка3');
      ChipTemp[3].Mesto := PickOut(str, 4);
      // Фишка4
      j1 := j;
      repeat
        str := Paragraphs[j1];
        i := Pos('Фишка4', str);
        inc(j1);
      until (i > 0) or (j1 = ParagraphCount);
      if i = 0 then raise ExcUp.Create('Фишка4');
      ChipTemp[4].Mesto := PickOut(str, 4);
      // Фишка5
      j1 := j;
      repeat
        str := Paragraphs[j1];
        i := Pos('Фишка5', str);
        inc(j1);
      until (i > 0) or (j1 = ParagraphCount);
      if i = 0 then raise ExcUp.Create('Фишка5');
      ChipTemp[5].Mesto := PickOut(str, 4);
      // Дырка
      j1 := j;
      repeat
        str := Paragraphs[j1];
        i := Pos('Дырка', str);
        inc(j1);
      until (i > 0) or (j1 = ParagraphCount);
      if i = 0 then raise ExcUp.Create('Дырка');
      ChipTemp[6].Mesto := PickOut(str, 3);
    end;
    for i := 1 to 5 do
      for j := i+1 to 6 do
        if ChipTemp[i].Mesto = ChipTemp[j].Mesto then
          raise Exc.Create('Совпало местоположение фишек: '+IntToStr(i)+' и '+IntToStr(j));
    for i := 1 to 6 do
      OldChip[i].Mesto := ChipTemp[i].Mesto;
  except
    on E: Exc do raise Exc.Create('Файл ресурсов: '+MyEOL+E.Message);
    on E: ExcUp do raise Exc.Create('В файле ресурсов не найдено описание: '+
      MyEOL+E.Message+'.');
    on E: Exception do
      RDOEdit_Game5.ShowError('Возникла неизвестная ошибка чтения файла ресурсов: '+
        MyEOL+''''+E.Message+'''.'+MyEOL+'Обратитесь к разработчику.');
  else
    RDOEdit_Game5.ShowError('Возникла неизвестная ошибка чтения файла ресурсов.'+
      MyEOL+'Обратитесь к разработчику.');
  end;
except
  on E: Exception do
  begin
    RDOEdit_Game5.TRCScanButton.Enabled := false;
    Application.ShowException(E);
  end;
end;
end;

procedure TRDOEdit_Game5.ShowError(Value: string);
begin
  MessageDlg(Value, mtError, [mbOk], 0);
end;

procedure TRDOEdit_Game5.MyException(Sender: TObject; E: Exception);
var
  i: integer;
  str, str1, str2: string;
begin
  Application.ShowException(E);
  str := Sender.ClassName;
  if Sender is TControl
    then str := str + '_' + (Sender as TControl).Name
    else if Sender is TComponent then
      str := str + '_' + (Sender as TComponent).Name;
  str := str + '_' + IntToStr(Round(Random(30000000)));
  str1 := E.Message;
  str2 := '';
  for i := 1 to Length(str1) do
    if (str1[i] = #13) or (str1[i] = #10) then str2 := str2+'_'
                                          else str2 := str2+str1[i];
  IniFile.WriteString('Error', str, str2);
end;

function TRDOEdit_Game5.CheckChar(ch: char): boolean;
begin
  case ch of
    '0'..'1', 'A'..'Z', 'a'..'z', 'А'..'Я', 'а'..'я', '_', '$': Result := true;
    else Result := false;
  end;
end;

function TRDOEdit_Game5.FindPickOut: string;
var
  str1: string;
  i, k: integer;
begin
  ActiveRich := FindActiveRich;
  if ActiveRich = nil then exit;
  with ActiveRich^ do
  begin
    str1 := Paragraphs[SelPar];
    k := SelCol;
    for i := k downto 1 do
      if not CheckChar(str1[i]) then
        begin Delete(str1, 1, i); break; end;
    for i := 1 to Length(str1) do
      if not CheckChar(str1[i]) then
        begin Delete(str1, i, Length(str1)-i+1); break; end;
  end;
  if Length(str1) > 250 then str1 := '';
  Result := str1;
end;

procedure TRDOEdit_Game5.LoadReopenIni;
//var
//  i: integer;
//  flag: boolean;
//  str, str2: string;
begin
{
  with ReopenMenu do
  begin
    flag := false;
    for i := IndexOf(N11) to Count-1 do
    begin
      Items[i].Visible := true;
      if i <> IndexOf(ReopenSep) then
      begin
        str := IniFile.ReadString('Reopen', 'Model'+IntToStr(i), '');
        System.Insert('&   ', str, 1);
        str2  := IntToHex((i), 1);
        if Length(str) > 4
          then flag := true
          else begin
            str:= '&   ';
            Items[i].Visible := false;
          end;
        str[2]:= str2[1];
        Items[i].Caption := str;
      end;
    end;
    if Length(Items[IndexOf(ReopenSep)+1].Caption) > 4
      then ReopenSep.Visible := true
      else ReopenSep.Visible := false;
    ReopenMenu.Enabled := flag;
  end;
}
end;

procedure TRDOEdit_Game5.SaveReopenIni;
//var
//  i: integer;
//  str: string;
begin
{
  with ReopenMenu do
    for i := IndexOf(N11) to Count-1 do
      if i <> IndexOf(ReopenSep) then
      begin
        IniFile.DeleteKey('Reopen', 'Model'+IntToStr(i));
        if Length(Items[i].Caption) > 4
          then begin
            str  := Items[i].Caption;
            System.Delete(str, 1, 4);
            IniFile.WriteString('Reopen', 'Model'+IntToStr(i), str);
          end
      end;
}
end;

procedure TRDOEdit_Game5.ShowSelection(Value: boolean);
begin
  Value := not Value;
  PATRich.HideSelection := Value;
  RTPRich.HideSelection := Value;
  RSSRich.HideSelection := Value;
  FUNRich.HideSelection := Value;
  DPTRich.HideSelection := Value;
  SMRRich.HideSelection := Value;
  TRCRich.HideSelection := Value;
end;

procedure TRDOEdit_Game5.SetBeginKey(MR: PRich);
begin
  with MR^ do
  begin
    ApplyKeyWords := true;
    Keywords.Clear;
    Delimiters := [' ', '{', '}', '(', ')', '[', ']', '=', '.'];
  end;
end;

procedure TRDOEdit_Game5.SetBeginStartStop(MR: PRich);
begin
  with MR^ do
  begin
    ApplyStartStopKeys := true;
    StartStopKeys.Clear;
    StartStopKeys.AddStartStopKey('{', '}', [woWholeWordsOnly], CommentColor.Style,
      1, crDefault, CommentColor.ColorBg, CommentColor.ColorFg, false);
    StartStopKeys.AddStartStopKey('(', ')', [woWholeWordsOnly], ParamRnColor.Style,
      2, crDefault, ParamRnColor.ColorBg, ParamRnColor.ColorFg, false);
    StartStopKeys.AddStartStopKey('[', ']', [woWholeWordsOnly], ParamSqColor.Style,
      3, crDefault, ParamSqColor.ColorBg, ParamSqColor.ColorFg, false);
  end;
end;

procedure TRDOEdit_Game5.OnlyTextColor;
begin
  PATRich.Font.Color := TextModelColor.ColorFg;
  RTPRich.Font.Color := TextModelColor.ColorFg;
  RSSRich.Font.Color := TextModelColor.ColorFg;
  FUNRich.Font.Color := TextModelColor.ColorFg;
  DPTRich.Font.Color := TextModelColor.ColorFg;
  SMRRich.Font.Color := TextModelColor.ColorFg;
  TRCRich.Font.Color := TextModelColor.ColorFg;
  PATRich.Font.Style := TextModelColor.Style;
  RTPRich.Font.Style := TextModelColor.Style;
  RSSRich.Font.Style := TextModelColor.Style;
  FUNRich.Font.Style := TextModelColor.Style;
  DPTRich.Font.Style := TextModelColor.Style;
  SMRRich.Font.Style := TextModelColor.Style;
  TRCRich.Font.Style := TextModelColor.Style;
  PATRich.Color := TextModelColor.ColorBg;
  RTPRich.Color := TextModelColor.ColorBg;
  RSSRich.Color := TextModelColor.ColorBg;
  FUNRich.Color := TextModelColor.ColorBg;
  DPTRich.Color := TextModelColor.ColorBg;
  SMRRich.Color := TextModelColor.ColorBg;
  TRCRich.Color := TextModelColor.ColorBg;
end;

procedure TRDOEdit_Game5.OnlyStartStopColor;
begin
  SetBeginStartStop(@PATRich);
  SetBeginStartStop(@RTPRich);
  SetBeginStartStop(@RSSRich);
  SetBeginStartStop(@FUNRich);
  SetBeginStartStop(@DPTRich);
  SetBeginStartStop(@SMRRich);
end;

procedure TRDOEdit_Game5.OnlyReservedColor;
var i: integer;
begin
  for i := 1 to EndRdoWordReserved do
  begin
    PATRich.Keywords.AddKeyWord(RdoWord[i], [woWholeWordsOnly], ReservedColor.Style,
      i+3, crDefault, ReservedColor.ColorBg, ReservedColor.ColorFg);
    RTPRich.Keywords.AddKeyWord(RdoWord[i], [woWholeWordsOnly], ReservedColor.Style,
      i+3, crDefault, ReservedColor.ColorBg, ReservedColor.ColorFg);
    RSSRich.Keywords.AddKeyWord(RdoWord[i], [woWholeWordsOnly], ReservedColor.Style,
      i+3, crDefault, ReservedColor.ColorBg, ReservedColor.ColorFg);
    FUNRich.Keywords.AddKeyWord(RdoWord[i], [woWholeWordsOnly], ReservedColor.Style,
      i+3, crDefault, ReservedColor.ColorBg, ReservedColor.ColorFg);
    DPTRich.Keywords.AddKeyWord(RdoWord[i], [woWholeWordsOnly], ReservedColor.Style,
      i+3, crDefault, ReservedColor.ColorBg, ReservedColor.ColorFg);
    SMRRich.Keywords.AddKeyWord(RdoWord[i], [woWholeWordsOnly], ReservedColor.Style,
      i+3, crDefault, ReservedColor.ColorBg, ReservedColor.ColorFg);
  end;
end;

procedure TRDOEdit_Game5.OnlyTraceColor;
var i, j: integer;
begin
  j := EndRdoWordReserved;
  for i := j+1 to j+EndRdoWordTrace do
  begin
    PATRich.Keywords.AddKeyWord(RdoWord[i], [woWholeWordsOnly], TraceColor.Style,
      i+3, crDefault, TraceColor.ColorBg, TraceColor.ColorFg);
    RTPRich.Keywords.AddKeyWord(RdoWord[i], [woWholeWordsOnly], TraceColor.Style,
      i+3, crDefault, TraceColor.ColorBg, TraceColor.ColorFg);
    RSSRich.Keywords.AddKeyWord(RdoWord[i], [woWholeWordsOnly], TraceColor.Style,
      i+3, crDefault, TraceColor.ColorBg, TraceColor.ColorFg);
    FUNRich.Keywords.AddKeyWord(RdoWord[i], [woWholeWordsOnly], TraceColor.Style,
      i+3, crDefault, TraceColor.ColorBg, TraceColor.ColorFg);
    DPTRich.Keywords.AddKeyWord(RdoWord[i], [woWholeWordsOnly], TraceColor.Style,
      i+3, crDefault, TraceColor.ColorBg, TraceColor.ColorFg);
    SMRRich.Keywords.AddKeyWord(RdoWord[i], [woWholeWordsOnly], TraceColor.Style,
      i+3, crDefault, TraceColor.ColorBg, TraceColor.ColorFg);
  end;
end;

procedure TRDOEdit_Game5.OnlyFunctionColor;
var i, j: integer;
begin
  j := EndRdoWordReserved+EndRdoWordTrace;
  for i := j+1 to j+EndRdoWordFunction do
  begin
    PATRich.Keywords.AddKeyWord(RdoWord[i], [woWholeWordsOnly], FunctionColor.Style,
      i+3, crDefault, FunctionColor.ColorBg, FunctionColor.ColorFg);
    RTPRich.Keywords.AddKeyWord(RdoWord[i], [woWholeWordsOnly], FunctionColor.Style,
      i+3, crDefault, FunctionColor.ColorBg, FunctionColor.ColorFg);
    RSSRich.Keywords.AddKeyWord(RdoWord[i], [woWholeWordsOnly], FunctionColor.Style,
      i+3, crDefault, FunctionColor.ColorBg, FunctionColor.ColorFg);
    FUNRich.Keywords.AddKeyWord(RdoWord[i], [woWholeWordsOnly], FunctionColor.Style,
      i+3, crDefault, FunctionColor.ColorBg, FunctionColor.ColorFg);
    DPTRich.Keywords.AddKeyWord(RdoWord[i], [woWholeWordsOnly], FunctionColor.Style,
      i+3, crDefault, FunctionColor.ColorBg, FunctionColor.ColorFg);
    SMRRich.Keywords.AddKeyWord(RdoWord[i], [woWholeWordsOnly], FunctionColor.Style,
      i+3, crDefault, FunctionColor.ColorBg, FunctionColor.ColorFg);
  end;
end;

procedure TRDOEdit_Game5.OnlyAplly;
var flag: boolean;
begin
  flag := FileChange;
  PATRich.ReApplyKeywords;
  RTPRich.ReApplyKeywords;
  RSSRich.ReApplyKeywords;
  FUNRich.ReApplyKeywords;
  DPTRich.ReApplyKeywords;
  SMRRich.ReApplyKeywords;
  TRCRich.ReApplyKeywords;
  FileChange := flag;
  PageControlChange(Self);
end;

procedure TRDOEdit_Game5.SetAllMemoRichColor;
begin
  SetBeginKey(@PATRich);
  SetBeginKey(@RTPRich);
  SetBeginKey(@RSSRich);
  SetBeginKey(@FUNRich);
  SetBeginKey(@DPTRich);
  SetBeginKey(@SMRRich);
  SetBeginKey(@TRCRich);
  OnlyStartStopColor;
  OnlyTextColor;
  OnlyReservedColor;
  OnlyTraceColor;
  OnlyFunctionColor;
  // Отдельно для TRCRich
  with TRCRich.Keywords do
  begin
    AddKeyWord('Results_file', [woWholeWordsOnly], ReservedColor.Style,
      1, crDefault, ReservedColor.ColorBg, ReservedColor.ColorFg);
    AddKeyWord('Run_file', [woWholeWordsOnly], ReservedColor.Style,
      2, crDefault, ReservedColor.ColorBg, ReservedColor.ColorFg);
    AddKeyWord('Model_name', [woWholeWordsOnly], ReservedColor.Style,
      3, crDefault, ReservedColor.ColorBg, ReservedColor.ColorFg);
    AddKeyWord('Resource_file', [woWholeWordsOnly], ReservedColor.Style,
      4, crDefault, ReservedColor.ColorBg, ReservedColor.ColorFg);
    AddKeyWord('OprIev_file', [woWholeWordsOnly], ReservedColor.Style,
      5, crDefault, ReservedColor.ColorBg, ReservedColor.ColorFg);
    AddKeyWord('$Changes', [woWholeWordsOnly], ReservedColor.Style,
      6, crDefault, ReservedColor.ColorBg, ReservedColor.ColorFg);
    AddKeyWord('$Watching', [woWholeWordsOnly], ReservedColor.Style,
      7, crDefault, ReservedColor.ColorBg, ReservedColor.ColorFg);
    AddKeyWord('$Tracing', [woWholeWordsOnly], ReservedColor.Style,
      8, crDefault, ReservedColor.ColorBg, ReservedColor.ColorFg);
    AddKeyWord('$Status', [woWholeWordsOnly], ReservedColor.Style,
      9, crDefault, ReservedColor.ColorBg, ReservedColor.ColorFg);
    AddKeyWord('· NORMAL_TERMINATION', [woWholeWordsOnly], ReservedColor.Style,
      10, crDefault, ReservedColor.ColorBg, ReservedColor.ColorFg);
    AddKeyWord('NO_MORE_EVENTS', [woWholeWordsOnly], ReservedColor.Style,
      11, crDefault, ReservedColor.ColorBg, ReservedColor.ColorFg);
    AddKeyWord('USER_BREAK', [woWholeWordsOnly], ReservedColor.Style,
      12, crDefault, ReservedColor.ColorBg, ReservedColor.ColorFg);
    AddKeyWord('RUN_TIME_ERROR', [woWholeWordsOnly], ReservedColor.Style,
      13, crDefault, ReservedColor.ColorBg, ReservedColor.ColorFg);
  end;
  OnlyAplly;
end;

function TRDOEdit_Game5.SetActiveRich(Value: string): PRich;
begin
  Value := UpperCase(Value);
  with PageControl do
  begin
    if Value = 'PAT' then ActivePage := PatSheet;
    if Value = 'RTP' then ActivePage := RtpSheet;
    if Value = 'RSS' then ActivePage := RssSheet;
    if Value = 'FUN' then ActivePage := FunSheet;
    if Value = 'DPT' then ActivePage := DptSheet;
    if Value = 'SMR' then ActivePage := SmrSheet;
    if Value = 'TRC' then ActivePage := TrcSheet;
  end;
  PageControlChange(Self);
  Result := FindActiveRich;
end;

function TRDOEdit_Game5.GetRdoFileDir: string;
begin
  Result := AnsiLowerCase(FRdoFileDir);
end;

procedure TRDOEdit_Game5.SetRdoFileDir(Value: string);
begin
  FRdoFileDir := Value;
end;

function TRDOEdit_Game5.GetRdoFileName: string;
begin
  Result := AnsiLowerCase(FRdoFileName);
end;

procedure TRDOEdit_Game5.SetRdoFileName(Value: string);
begin
  FRdoFileName := Value;
end;

function TRDOEdit_Game5.GetRdoFileExt: string;
begin
  Result := AnsiLowerCase(FRdoFileExt);
end;

procedure TRDOEdit_Game5.SetRdoFileExt(Value: string);
begin
  FRdoFileExt := Value;
end;

procedure TRDOEdit_Game5.SetMyFileOpen(Value: boolean);
begin
  FMyFileOpen           := Value;
  // Button
  FindButton.Enabled     := Value;
  TRCScanButton.Enabled  := Value;
  // Menu
//  Saveas1.Enabled       := Value;
  Edit05.Enabled        := Value;
//  Options1.Enabled      := Value;
  // Run
  if Value and RdoSelect then RunButton.Enabled := true
                         else RunButton.Enabled := false;
end;

procedure TRDOEdit_Game5.SetRdoSelect(Value: boolean);
begin
  FRdoSelect := Value;
  if Value and MyFileOpen then RunButton.Enabled := true
                          else RunButton.Enabled := false;
end;

procedure TRDOEdit_Game5.SetFileChange(Value: boolean);
begin
  FFileChange        := Value;
//  SaveButton.Enabled := Value;
  if not Value then
  begin
    PATRich.Modified := false;
    RTPRich.Modified := false;
    RSSRich.Modified := false;
    FUNRich.Modified := false;
    DPTRich.Modified := false;
    SMRRich.Modified := false;
    TRCRich.Modified := false;
    PageControlChange(Self);
  end;
end;

function TRDOEdit_Game5.MyExtractFileName(Value: string): string;
var str: string;
begin
  str := ExtractFileName(Value);
  while Pos('.smr', str) > 0 do
    Delete(str, Pos('.smr', str), Length('.smr'));
  Result := str;
end;

procedure TRDOEdit_Game5.LoadProject;
var tempName, tempDir, tempExt: string;
begin
  with FileDialog do
  begin
    InitialDir := RdoFileDir;
    Title      := 'Открыть модель';
    Filter     := 'Файл прогона|*.smr|Все|*.*';
    FileName   := '';
    if Execute then
    begin
      tempDir  := RdoFileDir;
      tempName := RdoFileName;
      tempExt  := RdoFileExt;
      RdoFileDir  := GetNormalDir( ExtractFileDir(FileName) );
      RdoFileExt  := ExtractFileExt(FileName);
      RdoFileName := MyExtractFileName(FileName);
      if RdoFileExt = '.smr'
        then begin
          LoadRdoFile;
        end
        else begin
          RdoFileDir  := tempDir;
          RdoFileName := tempName;
          RdoFileExt  := tempExt;
        end;
    end;
  end;
end;

procedure TRDOEdit_Game5.DeleteReopenItem(Value: integer);
//var
//  str, str2: string;
//  i, k2, n: integer;
begin
{
  with ReopenMenu do
  begin
    n := Value;
    for i := Value to Count-2 do
    begin
      k2 := i+1;
      if i = IndexOf(ReopenSep)-1 then inc(k2);
      if i <> IndexOf(ReopenSep) then begin
        str   := Items[k2].Caption;
        str2  := IntToHex((n), 1);
        str[2]:= str2[1];
        Items[i].Caption := str;
        inc(n);
      end;
    end;
  end;
}
end;

procedure TRDOEdit_Game5.LoadRdoFile;
//var
//  str, str1: string;
//  i, k1, k2: integer;
//  flag: boolean;
begin
  PageControl.Visible := false;
  ShowGraphButton.Enabled := false;
  GraphForm.Close;
  DelAllTopBox;
  RSSRich.Clear;
  FUNRich.Clear;
  DPTRich.Clear;
  TRCRich.Clear;
  MyFileOpen := false;
  try
    // RSS
    RdoFileFullName := RdoFileDir + RdoFileName + '.rss';
    RSSRich.LoadFromFile(RdoFileFullName);
    // FUN
    RdoFileFullName := RdoFileDir + RdoFileName + '.fun';
    FUNRich.LoadFromFile(RdoFileFullName);
    // DPT
    RdoFileFullName := RdoFileDir + RdoFileName + '.dpt';
    DPTRich.LoadFromFile(RdoFileFullName);
 {
    with ReopenMenu do
    begin
      str1 := RdoFileDir+RdoFileName+RdoFileExt;
      for i := IndexOf(N11) to Count-1 do
      begin
        str := Items[i].Caption;
        System.Delete(str, 1, 4);
        str := AnsiLowerCase(str);
        if str = str1 then DeleteReopenItem(i);
      end;
      k1 := IndexOf(ReopenSep)+1;
      k2 := Count-1;
      for i := k2 downto k1 do
      begin
        str   := Items[i-1].Caption;
        str1  := IntToHex((i-1), 1);
        str[2]:= str1[1];
        Items[i].Caption := str;
      end;
      k1 := IndexOf(ReopenSep)-1;
      k2 := IndexOf(ReopenSep)+1;
      str   := Items[k1].Caption;
      str1  := IntToHex((k2-1), 1);
      str[2]:= str1[1];
      Items[k2].Caption := str;
      k1 := IndexOf(N11)+1;
      k2 := IndexOf(ReopenSep)-1;
      for i := k2 downto k1 do
      begin
        str   := Items[i-1].Caption;
        str1  := IntToHex((i), 1);
        str[2]:= str1[1];
        Items[i].Caption := str;
      end;
      N11.Caption := '&0  '+FRdoFileDir+FRdoFileName+FRdoFileExt;
      flag := false;
      for i := IndexOf(N11) to Count-1 do
        if Length(Items[i].Caption) > 4
          then begin
            Items[i].Visible := true;
            flag := true;
          end
          else Items[i].Visible := false;
      if Length(Items[IndexOf(ReopenSep)+1].Caption) > 4
        then ReopenSep.Visible := true
        else ReopenSep.Visible := false;
      ReopenMenu.Enabled := flag;
      SaveReopenIni;
    end;
 }
  except
    showmessage('a');
    FileChange := false;
    Visible    := true;
    PageControl.Visible := true;
    PageControl.Enabled := false;
    PageControlChange(Self);
    exit;
  end;
  FileChange := false;
  MyFileOpen := true;
  PATRich.SelStart := 0;
  RTPRich.SelStart := 0;
  RSSRich.SelStart := 0;
  FUNRich.SelStart := 0;
  DPTRich.SelStart := 0;
  SMRRich.SelStart := 0;
  Visible             := true;
  PageControl.Enabled := true;
  PageControl.Visible := true;
  FindActiveRich^.SetFocus;
  SearchRSS;
end;

procedure TRDOEdit_Game5.LoadButtonClick(Sender: TObject);
begin
//  if not LoadButton.Enabled then exit;
  if SaveIfFileChange then LoadProject;
end;

procedure TRDOEdit_Game5.SaveButtonClick(Sender: TObject);
begin
  if not FileChange then exit;
  if RdoFileName <> '' then SaveAll
                       else SaveAs1Click(Sender);
end;

function TRDOEdit_Game5.GetNormalDir( dir: string ): string;
begin
  if (dir[Length(dir)] <> '\') and (dir[Length(dir)] <> '/') then
  begin
    dir := dir + '\';
  end;
  Result := dir;
end;

function TRDOEdit_Game5.GetTempDir: string;
var
  Buffer: array[0..1023] of Char;
  dir: string;
begin
  SetString( dir, Buffer, GetTempPath(SizeOf(Buffer), Buffer) );
  Result := GetNormalDir( dir );
end;

procedure TRDOEdit_Game5.FormCreate(Sender: TObject);
begin
  Application.OnException := MyException;
  TopBox     := nil;
  TopBoxFirst:= nil;
  TopBoxLast := nil;
  PATRich.EnableHotKeys := false;
  RTPRich.EnableHotKeys := false;
  RSSRich.EnableHotKeys := false;
  FUNRich.EnableHotKeys := false;
  DPTRich.EnableHotKeys := false;
  SMRRich.EnableHotKeys := false;
  TRCRich.EnableHotKeys := false;
  CaptionFont      := TFont.Create;
  CaptionFont.Name := Font.Name;
  FileChange := false;
  MyFileOpen := false;
  RdoSelect  := false;
  MyRepeate  := None;
  PageControl.Enabled := false;
  ShowWindow(Handle, SW_MAXIMIZE);
  IniFile := TIniFile.Create('game5.ini');
  // ---- Загрузка пути РДО-имитатора
  RdoImitatorPathName := IniFile.ReadString('Application', 'RDO path', '');
  if not FileExists( RdoImitatorPathName ) then
  begin
    RdoImitatorPathName := GetNormalDir( ExtractFileDir( Application.ExeName ) ) + 'RAO-studio.exe';
    if not FileExists( RdoImitatorPathName ) then
      RdoImitatorPathName := '';
  end;
  if RdoImitatorPathName <> '' then
  begin
    RdoSelect := true;
    CheckRdo;
  end;
  RdoFileDir := GetNormalDir( ExtractFileDir( Application.ExeName ) ) + 'model\';
  if not SetCurrentDir( RdoFileDir ) then
    if CreateDir( RdoFileDir )
      then SetCurrentDir( RdoFileDir )
      else begin
        RdoFileDir := GetTempDir + 'model\';
        if CreateDir( RdoFileDir )
          then SetCurrentDir( RdoFileDir )
      end;
  // ---- Colors
  TextModelColor.ColorFg:= clBlack;
  TextModelColor.ColorBg:= clWhite;
  TextModelColor.Style  := [];
  CommentColor.ColorFg  := clNavy;
  CommentColor.ColorBg  := clWhite;
  CommentColor.Style    := [fsItalic];
  ReservedColor.ColorFg := clBlack;
  ReservedColor.ColorBg := clWhite;
  ReservedColor.Style   := [fsBold];
  FunctionColor.ColorFg := clBlack;
  FunctionColor.ColorBg := clWhite;
  FunctionColor.Style   := [fsBold];
  TraceColor.ColorFg    := clBlack;
  TraceColor.ColorBg    := clWhite;
  TraceColor.Style      := [fsBold];;
  ParamRnColor.ColorFg  := clBlack;
  ParamRnColor.ColorBg  := clWhite;
  ParamRnColor.Style    := [];
  ParamSqColor.ColorFg  := clBlack;
  ParamSqColor.ColorBg  := clWhite;
  ParamSqColor.Style    := [];
  SetAllMemoRichColor;
  // ---- Загрузка Reopen
  LoadReopenIni;
  Randomize;
  NewOpenButtonClick(Self);
end;

procedure TRDOEdit_Game5.RunButtonClick(Sender: TObject);
var
  si: TStartupInfo;
  str: string;
  exitCode: DWORD;
begin
  if not RunButton.Enabled then exit;
  SaveAll;
  SetCurrentDir(RdoFileDir);
  str := RdoImitatorPathName + ' "' + RdoFileDir + RdoFileName + '.smr" -autorun -autoexit -dont_close_if_error';
  ZeroMemory (@si, SizeOf(TStartupInfo));
  si.cb          := SizeOf(TStartupInfo);
  si.wShowWindow := SW_SHOWMAXIMIZED;
  si.dwFlags     := STARTF_USESHOWWINDOW;
  if CreateProcess(nil, PChar(str), nil, nil, true,
                   NORMAL_PRIORITY_CLASS, nil, nil, si, pi) then
  begin
    ShowWindow( Handle, SW_HIDE );
    ShowGraphButton.Enabled := false;
    TRCScanButton.Enabled   := true;
    GraphForm.Close;
    DelAllTopBox;
    WaitForSingleObject( pi.hProcess, INFINITE );
    ShowWindow( Handle, SW_SHOW );
    if GetExitCodeProcess( pi.hProcess, exitCode ) then
    begin
      if exitCode = 0 then
      begin
        TRCScanButtonClick( Self );
        ShowGraphButtonClick( Self );
      end
    end
  end else CheckRdo;
end;

procedure TRDOEdit_Game5.FormClose(Sender: TObject; var Action: TCloseAction);
begin
  if not FileChange then
    Action := caFree
  else
    case MessageDlg('Сохранить изменения в проекте?',
           mtConfirmation, mbYesNoCancel, 0) of
      mrYes: begin
               SaveButtonClick(Sender);
               Action := caFree;
             end;
      mrNo:     Action := caFree;
      mrCancel: Action := TCloseAction(caNone);
    end;
end;

procedure TRDOEdit_Game5.RdoPathButtonClick(Sender: TObject);
begin
  with FileDialog do
  begin
    InitialDir := ExtractFileDir(RdoImitatorPathName);
    Title      := 'Открыть РДО-студия';
    Filter     := 'РДО-студия|RAO-Studio.exe|Все|*.*';
    FileName   := '';
    if Execute then
    begin
      RdoImitatorPathName := FileName;
      RdoSelect := true;
      IniFile.WriteString('Application', 'RDO path', RdoImitatorPathName);
      CheckRdo;
    end;
  end;
end;

procedure TRDOEdit_Game5.Exit1Click(Sender: TObject);
begin
  Close;
end;

function TRDOEdit_Game5.FindActiveRich: PRich;
begin
  FindActiveRich := nil;
  if PageControl.ActivePage = PatSheet then FindActiveRich := @PATRich;
  if PageControl.ActivePage = RtpSheet then FindActiveRich := @RTPRich;
  if PageControl.ActivePage = RssSheet then FindActiveRich := @RSSRich;
  if PageControl.ActivePage = FunSheet then FindActiveRich := @FUNRich;
  if PageControl.ActivePage = DptSheet then FindActiveRich := @DPTRich;
  if PageControl.ActivePage = SmrSheet then FindActiveRich := @SMRRich;
  if PageControl.ActivePage = TrcSheet then FindActiveRich := @TRCRich;
end;

procedure TRDOEdit_Game5.GetCursorPosition;
begin
  ActiveRich := FindActiveRich;
  if ActiveRich = nil then exit;
  StatusBar.Panels[0].Text := IntToStr(ActiveRich^.SelLine+1)+': '+
    IntToStr(ActiveRich^.SelCol+1)+'  ';
end;

function TRDOEdit_Game5.SaveIfFileChange: boolean;
begin
  Result := true;
  if FileChange then
    case MessageDlg('Сохранить изменения в проекте?',
           mtConfirmation, mbYesNoCancel, 0) of
      mrYes   : SaveButtonClick(Self);
      mrCancel: Result := false;
    end;
end;

procedure TRDOEdit_Game5.NewOpenButtonClick(Sender: TObject);
begin
  OldChip[1].Mesto := 4;
  OldChip[2].Mesto := 5;
  OldChip[3].Mesto := 2;
  OldChip[4].Mesto := 3;
  OldChip[5].Mesto := 6;
  OldChip[6].Mesto := 1;
  try
    GraphForm.Close;
  except
  end;
  if not SaveIfFileChange then exit;
  PageControl.Visible := false;
  MyFileOpen  := true;
  RdoFileName := 'game5';
  RdoFileExt  := '.smr';
  PATRich.Clear;
  RTPRich.Clear;
  RSSRich.Clear;
  FUNRich.Clear;
  DPTRich.Clear;
  SMRRich.Clear;
  TRCRich.Clear;
  PATRich.SetSelTextBuf(PChar(PATSource.Text));
  RTPRich.SetSelTextBuf(PChar(RTPSource.Text));
  RSSRich.SetSelTextBuf(PChar(RSSSource.Text));
  FUNRich.SetSelTextBuf(PChar(FUNSource.Text));
  DPTRich.SetSelTextBuf(PChar(DPTSource.Text));
  SMRRich.SetSelTextBuf(PChar(SMRSource.Text));
  TRCRich.SetSelTextBuf(PChar(TRCSource.Text));
  PATRich.SelStart := 0; PATRich.ScrollInView;
  RTPRich.SelStart := 0; RTPRich.ScrollInView;
  RSSRich.SelStart := 0; RSSRich.ScrollInView;
  FUNRich.SelStart := 0; FUNRich.ScrollInView;
  DPTRich.SelStart := 0; DPTRich.ScrollInView;
  SMRRich.SelStart := 0; SMRRich.ScrollInView;
  TRCRich.SelStart := 0; TRCRich.ScrollInView;
  TRCScanButton.Enabled := false;
  FileChange          := false;
  PageControl.Visible := true;
  PageControl.Enabled := true;
  SetActiveRich('PAT');
  if PATRich.Visible then PATRich.SetFocus;
  ShowGraphButton.Enabled := true;
  DelAllTopBox;
  try
    GraphForm.TopScrollBox.OnResize := GraphForm.TopScrollBoxResize;
  except
  end;
end;

procedure TRDOEdit_Game5.FindButtonClick(Sender: TObject);
var str: string;
begin
  if not FindButton.Enabled then exit;
  ActiveRich := FindActiveRich;
  if ActiveRich = nil then exit;
  str := FindPickOut;
  if str <> '' then FindDialog.FindText := str;
  if FindDialog.Execute then
  begin
    MyRepeate := MyFind;
    MySearch  := Tekughiy;
  end;
end;

procedure TRDOEdit_Game5.FindDialogFind(Sender: TObject);
var i, j: integer;
begin
  ActiveRich := FindActiveRich;
  if ActiveRich = nil then exit;
  LineSearch := ActiveRich^.SelLine;
  if LineSearch = OldLineSearch then inc(LineSearch);
  repeat
    if LineSearch >= ActiveRich^.LineCount then
    begin
      if MySearch = Nachalo then
      begin
        ShowMessage('Текст просмотрен полностью');
        FindDialog.CloseDialog;
        exit;
      end;
      if MySearch = Tekughiy then
      begin
        case MessageDlg('Текст просмотрен до конца. Продолжить с начала?',
               mtInformation, [mbYes, mbNo], 0) of
          mrYes: begin
                   LineSearch := 0;
                   MySearch := Nachalo;
                 end;
          mrNo : begin
                   FindDialog.CloseDialog;
                   exit;
                 end;
        end;
      end;
    end;
    if (FindDialog.Options*[frMatchCase]) = [frMatchCase]
    then
      PosSearch := Pos(FindDialog.FindText, ActiveRich^.Lines[LineSearch])
    else
      PosSearch := Pos(AnsiLowerCase(FindDialog.FindText),
                     AnsiLowerCase(ActiveRich^.Lines[LineSearch]));
    if PosSearch > 0 then
    begin
      j := 0;
      for i := 0 to LineSearch-1 do
        inc(j, Length(ActiveRich^.Lines[i])+2);
      with ActiveRich^ do
      begin
        SelStart := j+PosSearch-1;
        SelLength := Length(FindDialog.FindText);
        ScrollInView;
      end;
      OldLineSearch := LineSearch;
    end;
    inc(LineSearch);
  until PosSearch <> 0;
  GetCursorPosition;
end;

procedure TRDOEdit_Game5.ReplaceButtonClick(Sender: TObject);
var str: string;
begin
  if not ReplaceButton.Enabled then exit;
  ActiveRich := FindActiveRich;
  if ActiveRich = nil then exit;
  str := FindPickOut;
  if str <> '' then ReplaceDialog.FindText := str;
  if ReplaceDialog.Execute then
  begin
    MyRepeate  := MyReplace;
    MySearch   := Tekughiy;
    LineReplace:= -1;
    PosReplace := -1;
  end;
end;

procedure TRDOEdit_Game5.ReplaceDialogFind(Sender: TObject);
var i, j: integer;
begin
  ActiveRich := FindActiveRich;
  if ActiveRich = nil then exit;
  LineSearch := ActiveRich^.SelLine;
  if LineSearch = OldLineSearch then inc(LineSearch);
  repeat
    if LineSearch >= ActiveRich^.LineCount then
    begin
      if MySearch = Nachalo then
      begin
        ShowMessage('Текст просмотрен полностью');
        ReplaceDialog.CloseDialog;
        exit;
      end;
      if MySearch = Tekughiy then
      begin
        case MessageDlg('Текст просмотрен до конца. Продолжить с начала?',
               mtInformation, [mbYes, mbNo], 0) of
          mrYes: begin
                   LineSearch  :=  0;
                   LineReplace := -1;
                   PosReplace  := -1;
                   MySearch := Nachalo;
                 end;
          mrNo : begin
                   ReplaceDialog.CloseDialog;
                   exit;
                 end;
        end;
      end;
    end;
    if (ReplaceDialog.Options*[frMatchCase]) = [frMatchCase]
    then
      PosSearch := Pos(ReplaceDialog.FindText, ActiveRich^.Lines[LineSearch])
    else
      PosSearch := Pos(AnsiLowerCase(ReplaceDialog.FindText),
                     AnsiLowerCase(ActiveRich^.Lines[LineSearch]));
    if PosSearch > 0 then
    begin
      j := 0;
      for i := 0 to LineSearch-1 do
        inc(j, Length(ActiveRich^.Lines[i])+2);
      with ActiveRich^ do
      begin
        SelStart := j+PosSearch-1;
        SelLength := Length(ReplaceDialog.FindText);
        ScrollInView;
        ReplaceSelStart := SelStart;
      end;
      LineReplace   := LineSearch;
      PosReplace    := PosSearch;
      OldLineSearch := LineSearch;
    end;
    inc(LineSearch);
  until PosSearch <> 0;
  GetCursorPosition;
end;

procedure TRDOEdit_Game5.ReplaceDialogReplace(Sender: TObject);
var
  str: string;
  i, j: integer;
begin
  ActiveRich := FindActiveRich;
  if ActiveRich = nil then exit;
  if (ReplaceDialog.Options*[frReplace]) = [frReplace] then
    if (LineReplace <> -1) and (PosReplace <> -1) then
    begin
      str := ActiveRich^.Lines[LineReplace];
      Delete(str, PosReplace, Length(ReplaceDialog.FindText));
      Insert(ReplaceDialog.ReplaceText, str, PosReplace);
      with ActiveRich^ do
      begin
        Paragraphs[LineReplace] := str;
        SelStart  := ReplaceSelStart;
        SelLength := Length(ReplaceDialog.ReplaceText);
        ScrollInView;
      end;
      LineReplace := -1;
      PosReplace  := -1;
    end;
  if (ReplaceDialog.Options*[frReplaceAll]) = [frReplaceAll] then
  begin
    LineSearch := ActiveRich^.SelLine;
    if LineSearch = OldLineSearch then inc(LineSearch);
    repeat
      if LineSearch >= ActiveRich^.LineCount then
      begin
        if MySearch = Nachalo then
        begin
          ShowMessage('Текст просмотрен полностью');
          ReplaceDialog.CloseDialog;
          exit;
        end;
        if MySearch = Tekughiy then
        begin
          case MessageDlg('Текст просмотрен до конца. Продолжить с начала?',
                 mtInformation, [mbYes, mbNo], 0) of
            mrYes: begin
                     LineSearch  :=  0;
                     LineReplace := -1;
                     PosReplace  := -1;
                     MySearch    := Nachalo;
                   end;
            mrNo : begin
                     ReplaceDialog.CloseDialog;
                     exit;
                   end;
          end;
        end;
      end;
      if (ReplaceDialog.Options*[frMatchCase]) = [frMatchCase]
      then
        PosSearch := Pos(ReplaceDialog.FindText, ActiveRich^.Lines[LineSearch])
      else
        PosSearch := Pos(AnsiLowerCase(ReplaceDialog.FindText),
                       AnsiLowerCase(ActiveRich^.Lines[LineSearch]));
      while PosSearch > 0 do
      begin
        j := 0;
        for i := 0 to LineSearch-1 do
          inc(j, Length(ActiveRich^.Lines[i])+2);
        LineReplace := LineSearch;
        PosReplace  := PosSearch;
        with ActiveRich^ do
        begin
          SelStart := j+PosSearch-1;
          SelLength := Length(ReplaceDialog.FindText);
          ScrollInView;
          ReplaceSelStart := SelStart;
          str := Lines[LineReplace];
        end;
        Delete(str, PosReplace, Length(ReplaceDialog.FindText));
        Insert(ReplaceDialog.ReplaceText, str, PosReplace);
        with ActiveRich^ do
        begin
          Paragraphs[LineReplace] := str;
          SelStart  := ReplaceSelStart;
          SelLength := Length(ReplaceDialog.ReplaceText);
          ScrollInView;
        end;
        LineReplace   := -1;
        PosReplace    := -1;
        OldLineSearch := LineSearch;
        if (ReplaceDialog.Options*[frMatchCase]) = [frMatchCase]
        then
          PosSearch := Pos(ReplaceDialog.FindText, ActiveRich^.Lines[LineSearch])
        else
          PosSearch := Pos(AnsiLowerCase(ReplaceDialog.FindText),
                         AnsiLowerCase(ActiveRich^.Lines[LineSearch]));
      end;
      inc(LineSearch);
    until 1 > 2;
  end;
  GetCursorPosition;
end;

procedure TRDOEdit_Game5.Repeate1Click(Sender: TObject);
label lab1;
var
  str: string;
  i, j: integer;
begin
  if not FindButton.Enabled then exit;
  if not ReplaceButton.Enabled then exit;
  ActiveRich := FindActiveRich;
  if ActiveRich = nil then exit;
  case MyRepeate of
    None     : exit;
    MyFind   : FindDialogFind(Sender);
    MyReplace: begin
      ShowSelection(true);
      LineSearch := ActiveRich^.SelLine;
      if LineSearch = OldLineSearch then inc(LineSearch);
      repeat
lab1:
        if LineSearch >= ActiveRich^.LineCount then
        begin
          if MySearch = Nachalo then
          begin
            MyRepeatAll := false;
            ShowMessage('Текст просмотрен полностью');
            ReplaceDialog.CloseDialog;
            exit;
          end;
          if MySearch = Tekughiy then
          begin
            MyRepeatAll := false;
            case MessageDlg('Текст просмотрен до конца. Продолжить с начала?',
                   mtInformation, [mbYes, mbNo], 0) of
              mrYes: begin
                       LineSearch  :=  0;
                       LineReplace := -1;
                       PosReplace  := -1;
                       MySearch := Nachalo;
                     end;
              mrNo : begin
                       ReplaceDialog.CloseDialog;
                       exit;
                     end;
            end;
          end;
        end;
        if (ReplaceDialog.Options*[frMatchCase]) = [frMatchCase]
          then PosSearch := Pos(ReplaceDialog.FindText,
                              ActiveRich^.Lines[LineSearch])
          else PosSearch := Pos(AnsiLowerCase(ReplaceDialog.FindText),
                              AnsiLowerCase(ActiveRich^.Lines[LineSearch]));
        while PosSearch > 0 do
        begin
          j := 0;
          for i := 0 to LineSearch-1 do
            inc(j, Length(ActiveRich^.Lines[i])+2);
          LineReplace := LineSearch;
          PosReplace  := PosSearch;
          with ActiveRich^ do
          begin
            SelStart  := j+PosSearch-1;
            SelLength := Length(ReplaceDialog.FindText);
            ScrollInView;
            ReplaceSelStart := SelStart;
            str             := Lines[LineReplace];
          end;
          if not MyRepeatAll then
            case MessageDlg('Заменить '''+ReplaceDialog.FindText+''' ?',
                mtConfirmation, mbYesNoCancel+[mbAll], 0) of
              mrYes: str := str;
              mrNo : begin inc(LineSearch); goto lab1; end;
              mrAll: MyRepeatAll := true;
              else begin ShowSelection(false); exit; end;
            end;
          Delete(str, PosReplace, Length(ReplaceDialog.FindText));
          Insert(ReplaceDialog.ReplaceText, str, PosReplace);
          ActiveRich^.Paragraphs[LineReplace] := str;
          with ActiveRich^ do
          begin
            SelStart  := ReplaceSelStart;
            SelLength := Length(ReplaceDialog.ReplaceText);
            ScrollInView;
          end;
          LineReplace := -1;
          PosReplace  := -1;
          OldLineSearch := LineSearch;
          if (ReplaceDialog.Options*[frMatchCase]) = [frMatchCase]
          then
            PosSearch := Pos(ReplaceDialog.FindText,
                           ActiveRich^.Lines[LineSearch])
          else
            PosSearch := Pos(AnsiLowerCase(ReplaceDialog.FindText),
                           AnsiLowerCase(ActiveRich^.Lines[LineSearch]));
        end;
        inc(LineSearch);
      until PosSearch <> 0;
    end;
  end;
  ShowSelection(false);
end;

procedure TRDOEdit_Game5.Cut1Click(Sender: TObject);
begin
  ActiveRich := FindActiveRich;
  if (ActiveRich = nil) or (not MyFileOpen) then exit;
  ActiveRich^.CutToClipboard;
end;

procedure TRDOEdit_Game5.Copy1Click(Sender: TObject);
begin
  ActiveRich := FindActiveRich;
  if (ActiveRich = nil) or (not MyFileOpen) then exit;
  ActiveRich^.CopyToClipboard;
end;

procedure TRDOEdit_Game5.Paste1Click(Sender: TObject);
begin
  ActiveRich := FindActiveRich;
  if (ActiveRich = nil) or (not MyFileOpen) then exit;
  ActiveRich^.PasteFromClipboard;
end;

procedure TRDOEdit_Game5.Undo1Click(Sender: TObject);
begin
  ActiveRich := FindActiveRich;
  if (ActiveRich = nil) or (not MyFileOpen)
    or (not Undo1.Enabled) then exit;
  if ActiveRich^.CanUndo then ActiveRich^.Undo;
end;

procedure TRDOEdit_Game5.PATRichChange(Sender: TObject);
begin
  with (Sender as TDrMemoRich) do
  begin
    if RChange then
    begin
      FileChange := true;
      StatusBar.Panels[1].Text := 'Modified';
    end;
    if CanUndo then Undo1.Enabled := true
               else Undo1.Enabled := false;
  end;
  GetCursorPosition;
end;

procedure TRDOEdit_Game5.PATRichMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  with (Sender as TDrMemoRich) do ClearHigh;
  GetCursorPosition;
end;

procedure TRDOEdit_Game5.PATRichEnter(Sender: TObject);
begin
  with (Sender as TDrMemoRich) do
    if CanUndo then Undo1.Enabled := true
               else Undo1.Enabled := false;
  GetCursorPosition;
end;

procedure TRDOEdit_Game5.PATRichKeyDown(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  with (Sender as TDrMemoRich) do ClearHigh;
  GetCursorPosition;
end;

procedure TRDOEdit_Game5.PageControlChange(Sender: TObject);
begin
  with FindActiveRich^ do
  begin
    if Modified then StatusBar.Panels[1].Text := 'Modified'
                else StatusBar.Panels[1].Text := '';
    if ReadOnly then
    begin
      Cut1.Enabled   := false;
      Cut2.Enabled   := false;
      Paste1.Enabled := false;
      Paste2.Enabled := false;
      StatusBar.Panels[1].Text := 'Read only';
    end else
    begin
      Cut1.Enabled   := true;
      Cut2.Enabled   := true;
      Paste1.Enabled := true;
      Paste2.Enabled := true;
    end;
  end;
end;

procedure TRDOEdit_Game5.TRCScanButtonClick(Sender: TObject);
begin
  if not TRCScanButton.Enabled then exit;
  PageControl.Visible := false;
  // TRC
  with TRCRich do
  begin
    Clear;
    RdoFileFullName := RdoFileDir + RdoFileName + '.trc';
    if LoadFromFile(RdoFileFullName) then
    begin
      TRCScanButton.Enabled   := false;
      ShowGraphButton.Enabled := true;
    end;
    SelStart := 0;
    ScrollInView;
  end;
  PageControlChange(Self);
  PageControl.Visible := true;
end;

procedure TRDOEdit_Game5.SaveAll;
begin
  SetCurrentDir(RdoFileDir);
  // PAT
  RdoFileFullName := RdoFileDir + RdoFileName + '.pat';
  PATRich.SaveToFile(RdoFileFullName);
  // RTP
  RdoFileFullName := RdoFileDir + RdoFileName + '.rtp';
  RTPRich.SaveToFile(RdoFileFullName);
  // RSS
  RdoFileFullName := RdoFileDir + RdoFileName + '.rss';
  RSSRich.SaveToFile(RdoFileFullName);
  // FUN
  RdoFileFullName := RdoFileDir + RdoFileName + '.fun';
  FUNRich.SaveToFile(RdoFileFullName);
  // DPT
  RdoFileFullName := RdoFileDir + RdoFileName + '.dpt';
  DPTRich.SaveToFile(RdoFileFullName);
  // SMR
  RdoFileFullName := RdoFileDir + RdoFileName + '.smr';
  SMRRich.SaveToFile(RdoFileFullName);
  // TRC
  RdoFileFullName := RdoFileDir + RdoFileName + '.trc';
  TRCRich.SaveToFile(RdoFileFullName);

  FileChange := false;
end;

procedure TRDOEdit_Game5.FormDestroy(Sender: TObject);
begin
  CaptionFont.Free;
  DelAllTopBox;
end;

procedure TRDOEdit_Game5.Saveas1Click(Sender: TObject);
var tempDir, tempName, tempExt: string;
begin
  with FileSaveDialog do
  begin
    InitialDir := RdoFileDir;
    FileName   := RdoFileName+RdoFileExt;
    Title      := 'Сохранить '+RdoFileName+' как...';
    Filter     := 'Файл прогона|*.smr|Все|*.*';
    DefaultExt := '';
    if Execute then
    begin
      tempDir     := RdoFileDir;
      tempName    := RdoFileName;
      tempExt     := RdoFileExt;
      RdoFileDir  := ExtractFileDir (FileName)+'\';
      RdoFileExt  := ExtractFileExt (FileName);
      RdoFileName := MyExtractFileName(FileName);
      if FileExists(RdoFileDir+RdoFileName+RdoFileExt)
        then case MessageDlg('Файл уже существует. Перезаписать?',
                                mtConfirmation, [mbYes]+[mbNo], 0) of
               mrYes: begin
                        SaveAll;
                      end;
               else begin
                   RdoFileDir  := tempDir;
                   RdoFileName := tempName;
                   RdoFileExt  := tempExt;
                   exit;
                 end;
             end
        else begin
            SaveAll;
          end;
    end;
  end;
end;

procedure TRDOEdit_Game5.FindDialogShow(Sender: TObject);
begin
  ShowSelection(true);
  OldLineSearch := -1;
end;

procedure TRDOEdit_Game5.FindDialogClose(Sender: TObject);
begin
  ShowSelection(false);
  OldLineSearch := -1;
end;

procedure TRDOEdit_Game5.ReplaceDialogShow(Sender: TObject);
begin
  ShowSelection(true);
  OldLineSearch := -1;
  MyRepeatAll   := false;
end;

procedure TRDOEdit_Game5.ReplaceDialogClose(Sender: TObject);
begin
  ShowSelection(false);
  OldLineSearch := -1;
  MyRepeatAll   := false;
end;

procedure TRDOEdit_Game5.N11Click(Sender: TObject);
var tempDir, tempName, tempExt, str: string;
begin
  with Sender as TMenuItem do
  begin
    str := Caption;
    System.Delete(str, 1, 4);
    if FileExists(str) then
    begin
      tempDir     := RdoFileDir;
      tempName    := RdoFileName;
      tempExt     := RdoFileExt;
      RdoFileDir  := ExtractFileDir(str)+'\';
      RdoFileExt  := ExtractFileExt(str);
      RdoFileName := MyExtractFileName(str);
      if RdoFileExt = '.smr'
        then begin if SaveIfFileChange then LoadRdoFile end
        else begin
          RdoFileName := tempName;
          RdoFileDir  := tempDir;
          RdoFileExt  := tempExt;
        end;
    end else begin
      DeleteReopenItem(MenuIndex);
      SaveReopenIni;
      MessageDlg('Ошибка чтения '''+str+'''.', mtError, [mbOK], 0);
    end;
  end;
end;

procedure TRDOEdit_Game5.ShowGraphButtonClick(Sender: TObject);
begin
  if GraphForm.Visible then ShowWindow(GraphForm.Handle, SW_MAXIMIZE)
                       else GraphForm.Visible := true;
  if TopBoxFirst = nil then GraphForm.ConvertTraceFile;
   GraphForm.ZoomOneClick( Self );
end;

procedure TRDOEdit_Game5.MakeChipButtonClick(Sender: TObject);
begin
  GraphForm.Close;
  if MakeChipForm.ShowModal = mrOk then
  begin
    TRCScanButton.Enabled   := false;
    ShowGraphButton.Enabled := false;
    RunButtonClick( Self );
  end;
end;

procedure TRDOEdit_Game5.SaveButtonEnabled(Sender: TObject);
begin
//  Save1.Enabled := SaveButton.Enabled;
end;

procedure TRDOEdit_Game5.RunButtonEnabled(Sender: TObject);
begin
  Run1.Enabled := RunButton.Enabled;
  Run2.Enabled := RunButton.Enabled;
end;

procedure TRDOEdit_Game5.FindButtonEnabled(Sender: TObject);
begin
  ReplaceButton.Enabled := FindButton.Enabled;
  Find1.Enabled         := FindButton.Enabled;
  Find2.Enabled         := FindButton.Enabled;
  Replace1.Enabled      := FindButton.Enabled;
  Replace2.Enabled      := FindButton.Enabled;
  Repeate1.Enabled      := FindButton.Enabled;
  Repeate2.Enabled      := FindButton.Enabled;
end;

procedure TRDOEdit_Game5.TRCScanButtonEnabled(Sender: TObject);
begin
  TRCScan.Enabled := TRCScanButton.Enabled;
end;

procedure TRDOEdit_Game5.ShowGraphButtonEnabled(Sender: TObject);
begin
  Showgraph1.Enabled := ShowGraphButton.Enabled;
end;

procedure TRDOEdit_Game5.MakeChipButtonEnabled(Sender: TObject);
begin
  Makeit1.Enabled := MakeChipButton.Enabled;
end;

function TRDOEdit_Game5.getFullHelpFileName: string;
var
  chm_filename: string;
begin
  chm_filename := ExtractFilePath( Application.ExeName ) + 'RAO-language.chm';
  if FileExists( chm_filename ) then result := chm_filename
                               else begin
                                 MessageDlg( 'Невозможно найти файл справки ''' + chm_filename + '''. Он должен быть расположен в директории с RAO-game5.exe', mtError, [mbOK], 0 );
                                 result := '';
                               end;
end;

procedure TRDOEdit_Game5.Help2Click(Sender: TObject);
var
  word: string;
  pos, pos_current: integer;
  link: HH_AKLINK;
begin
  if Length( getFullHelpFileName ) = 0 then exit;
  if (FindActiveRich <> nil) and (FindActiveRich.Focused) then begin
    // Определим текущее слово
    word := FindActiveRich.SelText;
    Trim( word );
    if Length( word ) > 0 then begin
      pos_current := 1;
      while (pos_current <= Length( word )) and ((word[ pos_current ] = ' ') or (word[ pos_current ] = #9) or (word[ pos_current ] = ':')) do begin
        Inc( pos_current );
      end;
    end else begin
      word := FindActiveRich.Lines[ FindActiveRich.SelLine ];
      pos_current := FindActiveRich.SelCol;
      while (pos_current > 0) and ((word[ pos_current ] = ' ') or (word[ pos_current ] = #9) or (word[ pos_current ] = ':')) do begin
        Dec( pos_current );
      end;
    end;
    // Подрежем слева
    if Length( word ) > 0 then begin
      pos := pos_current;
      while (pos <= Length( word ) + 1) and (word[ pos ] <> ' ') and (word[ pos ] <> #9) and (word[ pos ] <> ':') do begin
        Inc( pos );
      end;
      if ( pos <= Length( word ) ) then Delete( word, pos, Length(word) + 1 );
    end;
    // Подрежем справа
    if Length( word ) > 0 then begin
      pos := pos_current;
      while (pos > 0) and (word[ pos ] <> ' ') and (word[ pos ] <> #9) and (word[ pos ] <> ':') do begin
        Dec( pos );
      end;
      if ( pos > 0 ) then Delete( word, 1, pos );
    end;
    Trim( word );
    if (Length( word ) = 0) or (FindActiveRich.Keywords.IndexOf( word ) = 0) then begin
      if FindActiveRich = @PATRich then word := 'PAT'
      else if FindActiveRich = @RTPRich then word := 'RTP'
      else if FindActiveRich = @RSSRich then word := 'RSS'
      else if FindActiveRich = @FUNRich then word := 'FUN'
      else if FindActiveRich = @DPTRich then word := 'DPT'
      else if FindActiveRich = @SMRRich then word := 'SMR'
      else if FindActiveRich = @TRCRich then word := 'TRC';
    end;
    ZeroMemory( @link, sizeof( HH_AKLINK ) );
    link.cbStruct     := sizeof( HH_AKLINK );
    link.fIndexOnFail := TRUE;
    link.pszKeywords  := PChar(word);
    HtmlHelp( GetDesktopWindow(), PChar(getFullHelpFileName), HH_KEYWORD_LOOKUP, integer(@link) );
  end else begin
    HtmlHelp( GetDesktopWindow(), PChar(getFullHelpFileName), HH_DISPLAY_TOPIC, 0 );
  end;
end;

procedure TRDOEdit_Game5.About1Click(Sender: TObject);
begin
  AboutForm.ShowModal;
end;

procedure TRDOEdit_Game5.Guide1Click(Sender: TObject);
var
  res: integer;
begin
  res := ShellExecute( Application.Handle, 'open', 'RAO-game5 - Руководство пользователя.pdf', nil, PChar(ExtractFileDir(Application.ExeName)), SW_SHOWNORMAL );
  if res < 32 then begin
    case res of
      ERROR_FILE_NOT_FOUND: MessageBox( Application.Handle, 'Руководство пользователя не найдено. Файл RAO-game5 - Руководство пользователя.pdf должен находится в одной с RAO-game5.exe директории.', 'Ошибка', MB_OK or MB_ICONERROR );
      SE_ERR_NOASSOC      : MessageBox( Application.Handle, 'Руководство пользователя не может быть загружено. Возможно, у Вас не установлен Adobe Reader.', 'Ошибка', MB_OK or MB_ICONERROR );
    else
      MessageBox( Application.Handle, 'Ошибка загрузки руководства пользователя.', 'Ошибка', MB_OK or MB_ICONERROR );
    end;
  end
end;

procedure TRDOEdit_Game5.Metoda1Click(Sender: TObject);
var
  res: integer;
begin
  res := ShellExecute( Application.Handle, 'open', 'Поиск на графе.pdf', nil, PChar(ExtractFileDir(Application.ExeName)), SW_SHOWNORMAL );
  if res < 32 then begin
    case res of
      ERROR_FILE_NOT_FOUND: MessageBox( Application.Handle, 'Методичка по поиску не найдена. Файл ''Поиск на графе.pdf'' должен находится в одной с RAO-game5.exe директории.', 'Ошибка', MB_OK or MB_ICONERROR );
      SE_ERR_NOASSOC      : MessageBox( Application.Handle, 'Методичка по поиску не может быть загружена. Возможно, у Вас не установлен Adobe Reader.', 'Ошибка', MB_OK or MB_ICONERROR );
    else
      MessageBox( Application.Handle, 'Ошибка загрузки методички по поиску.', 'Ошибка', MB_OK or MB_ICONERROR );
    end;
  end
end;

end.

