unit FMS_Unit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, ComCtrls, ToolWin, Menus, Buttons, Grids, StdCtrls, IniFiles,
  SplshWnd, FileUtil;

type

  TFMSForm = class(TForm)
    MainMenu: TMainMenu;
    File1: TMenuItem;
    Exit1: TMenuItem;
    N1: TMenuItem;
    SaveAs1: TMenuItem;
    Save1: TMenuItem;
    Open1: TMenuItem;
    New1: TMenuItem;
    ToolBar: TToolBar;
    TreeView: TTreeView;
    NewButton: TSpeedButton;
    OpenButton: TSpeedButton;
    SaveButton: TSpeedButton;
    Help1: TMenuItem;
    About1: TMenuItem;
    SearchforHelpOn1: TMenuItem;
    Contents1: TMenuItem;
    Model1: TMenuItem;
    Run1: TMenuItem;
    Compile1: TMenuItem;
    ToolButton1: TToolButton;
    BuildButton: TSpeedButton;
    RunButton: TSpeedButton;
    PathButton: TSpeedButton;
    TreeViewPopup: TPopupMenu;
    TreeViewCreateMenu: TMenuItem;
    Create1: TMenuItem;
    CreateOTO1: TMenuItem;
    CreateTU1: TMenuItem;
    CreateNomenk1: TMenuItem;
    TreeViewDelMenu: TMenuItem;
    StatusBar: TStatusBar;
    OpenDialog: TOpenDialog;
    SaveDialog: TSaveDialog;
    TreeViewImageList: TImageList;
    N2: TMenuItem;
    RdoPath1: TMenuItem;
    N4: TMenuItem;
    PageControl: TPageControl;
    OTOSheet: TTabSheet;
    TUSheet: TTabSheet;
    SkladSheet: TTabSheet;
    NomenkSheet: TTabSheet;
    PointSheet: TTabSheet;
    ConstSheet: TTabSheet;
    TUPanel: TPanel;
    TUNameLabel: TStaticText;
    TUPositionLabel: TStaticText;
    TUCoorLabel: TStaticText;
    TUXLabel: TStaticText;
    TUYLabel: TStaticText;
    TUPredPointLabel: TStaticText;
    TUPositionNameLabel: TStaticText;
    TUName: TEdit;
    TUPosition: TEdit;
    TUX: TEdit;
    TUY: TEdit;
    TUPredPoint: TEdit;
    NomenkPanel: TPanel;
    NomenkNameLabel: TStaticText;
    NomenkLowLabel: TStaticText;
    NomenkLowValue1Label: TStaticText;
    NomenkLowValue2Label: TStaticText;
    NomenkAllValueLabel: TStaticText;
    TPLabel: TStaticText;
    NomenkName: TEdit;
    NomenkLowValue1: TEdit;
    NomenkLowValue2: TEdit;
    NomenkAllValue: TEdit;
    TPGrid: TStringGrid;
    ConstPanel: TPanel;
    ScaleLabel: TStaticText;
    Scale: TEdit;
    SkladPanel: TPanel;
    SkladNameLabel: TStaticText;
    SkladValueLabel: TStaticText;
    SkladCoorLabel: TStaticText;
    SkladXLabel: TStaticText;
    SkladYLabel: TStaticText;
    SkladPointLabel: TStaticText;
    SkladName: TEdit;
    SkladValue: TEdit;
    SkladX: TEdit;
    SkladY: TEdit;
    SkladPoint: TEdit;
    OTOPanel: TPanel;
    NakopBevel: TBevel;
    OTONameLabel: TStaticText;
    NakopValueLabel: TStaticText;
    OTOCoorLabel: TStaticText;
    OTOYLabel: TStaticText;
    OTOXLabel: TStaticText;
    NakopXLabel: TStaticText;
    NakopYLabel: TStaticText;
    NakopCoorLabel: TStaticText;
    NakopPointLabel: TStaticText;
    NakopLabel: TStaticText;
    OTOName: TEdit;
    NakopValue: TEdit;
    OTOX: TEdit;
    OTOY: TEdit;
    NakopY: TEdit;
    NakopX: TEdit;
    NakopPoint: TEdit;
    PointPanel: TPanel;
    PointCoorLabelLabel: TStaticText;
    PointRelLabel: TStaticText;
    PointCoorGrid: TStringGrid;
    PointRelGrid: TStringGrid;
    ScrollBox: TScrollBox;
    RLowValue1: TEdit;
    RLowValue2: TEdit;
    RLowValue2Label: TStaticText;
    RLowValue1Label: TStaticText;
    RLowLabel: TStaticText;
    RTimeLabel: TStaticText;
    RTimeMin: TEdit;
    RTimeMinLabel: TStaticText;
    RTimeMaxLabel: TStaticText;
    RTimeMax: TEdit;
    OTOPBevel: TBevel;
    OTOPLabel: TStaticText;
    OTOPBusy: TCheckBox;
    OTOPWait: TCheckBox;
    OTOPRepair: TCheckBox;
    NakopPLabel: TStaticText;
    NakopPBevel: TBevel;
    NakopPBusy: TCheckBox;
    NakopPWait: TCheckBox;
    TUSpeedLabel: TStaticText;
    TUSpeed: TEdit;
    TUTimeLoadLabel: TStaticText;
    TUTimeLoad: TEdit;
    TUPLabel: TStaticText;
    TUPBevel: TBevel;
    TUPFree: TCheckBox;
    TUPTransport: TCheckBox;
    TUPEmpty: TCheckBox;
    TUPLoad: TCheckBox;
    SkladPLabel: TStaticText;
    SkladPBevel: TBevel;
    SkladPBusy: TCheckBox;
    SkladPWait: TCheckBox;
    NomenkStartPartyLabel: TStaticText;
    NomenkStartParty: TEdit;
    NomenkWorkPartyLabel: TStaticText;
    NomenkWorkParty: TEdit;
    OTODXLabel: TStaticText;
    OTODYLabel: TStaticText;
    OTODX: TEdit;
    OTODY: TEdit;
    TUDXLabel: TStaticText;
    TUDYLabel: TStaticText;
    TUDX: TEdit;
    TUDY: TEdit;
    ToolButton2: TToolButton;
    ResultButton: TSpeedButton;
    N5: TMenuItem;
    Result1: TMenuItem;
    Splitter: TSplitter;
    MapSpeedButton: TSpeedButton;
    MapButton: TSpeedButton;
    N3: TMenuItem;
    Show1: TMenuItem;
    PicImageBevel: TBevel;
    SkladDXLabel: TStaticText;
    SkladDYLabel: TStaticText;
    SkladDY: TEdit;
    SkladDX: TEdit;
    PicImagePanel: TPanel;
    PicImage: TImage;
    PicImageZoomButton: TSpeedButton;
    RLow: TComboBox;
    NomenkLow: TComboBox;
    HelpSpeedButton: TSpeedButton;
    ToolButton3: TToolButton;
    WorkTime: TEdit;
    TerminateLabel: TStaticText;
    TerminateBevel: TBevel;
    AllProgrammRadioButton: TRadioButton;
    WorkTimeRadioButton: TRadioButton;
    HelpPopupMenu: TPopupMenu;
    WhatIsMenu: TMenuItem;
    DetailHelpMenu: TMenuItem;
    procedure FormCreate(Sender: TObject);
    procedure TreeViewMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure TreeViewCreateMenuClick(Sender: TObject);
    procedure TreeViewDelMenuClick(Sender: TObject);
    procedure CreateOTO1Click(Sender: TObject);
    procedure CreateTU1Click(Sender: TObject);
    procedure CreateNomenk1Click(Sender: TObject);
    procedure FormDestroy(Sender: TObject);
    procedure NewButtonClick(Sender: TObject);
    procedure TreeViewKeyUp(Sender: TObject; var Key: Word;
      Shift: TShiftState);
    procedure OpenButtonClick(Sender: TObject);
    procedure BuildButtonClick(Sender: TObject);
    procedure PathButtonClick(Sender: TObject);
    procedure RunButtonClick(Sender: TObject);
    procedure About1Click(Sender: TObject);
    procedure Exit1Click(Sender: TObject);
    procedure SaveAs1Click(Sender: TObject);
    procedure FormCloseQuery(Sender: TObject; var CanClose: Boolean);
    procedure ResultButtonClick(Sender: TObject);
    procedure Contents1Click(Sender: TObject);
    procedure SearchforHelpOn1Click(Sender: TObject);
    procedure MapSpeedButtonClick(Sender: TObject);
    procedure PicImageZoomButtonClick(Sender: TObject);
    procedure TreeViewChanging(Sender: TObject; Node: TTreeNode;
      var AllowChange: Boolean);
    procedure RLowChange(Sender: TObject);
    procedure OTOPBusyClick(Sender: TObject);
    procedure HelpSpeedButtonClick(Sender: TObject);
    procedure AllProgrammRadioButtonClick(Sender: TObject);
    procedure WhatIsMenuClick(Sender: TObject);
    procedure Save1Click(Sender: TObject);
  private
    IniFile: TIniFile;
    RdoImitatorPathName, FPlaneFileName: string;
    FPlaneChange, FAllowChange, FPlaneLoaded, FCheckBoxChange, FShowHelpContext: boolean;
    ListSeparator: char;
    function  SaveAsFun: boolean;
    function  SaveQueryAndSave: boolean;
    function  SaveMenu: boolean;
    function  SaveReal: boolean;
    procedure FMSException(Sender: TObject; E: Exception);
    procedure ClearAllFMSObject;
    procedure LoadPlaneFile(Value: string);
    procedure ShowFMSObject(Value: TTreeNode);
    procedure BuildModel;
    procedure SetPlaneFileName(Value: string);
    procedure SetPlaneChange(Value: boolean);
    procedure SetPlaneLoaded(Value: boolean);
    procedure SetShowHelpContext(Value: boolean);
    procedure SetFMSModelPath(Value: string);
    procedure UpDateListSeparator;
    procedure WndProc(var Message: TMessage); override;
    procedure AppMess(var Msg: TMsg; var Handled: boolean);
    procedure ChangeFMSObject;
    property PlaneLoaded: boolean read FPlaneLoaded write SetPlaneLoaded;
    property ShowHelpContext: boolean read FShowHelpContext write SetShowHelpContext;
  public
    PlaneFile: TIniFile;
    FMSModelPath: string;
    procedure UpDateFMSObject;
    property PlaneFileName: string  read FPlaneFileName write SetPlaneFileName;
    property PlaneChange  : boolean read FPlaneChange   write SetPlaneChange;
  end;

var
  FMSForm: TFMSForm;
  OTOTreeNode, TUTreeNode, SkladTreeNode, NomenkTreeNode, PointTreeNode, ConstTreeNode: TTreeNode;

implementation

uses FMSObjects_Unit, Const_Unit, Var_Unit, ConstRes_Unit, About_Unit, Misc_Unit,
  Map_Unit, ViewPicture_Unit;

{$R *.DFM}
{$R FmsModel.res}
{$R PictureBMP.res}

procedure TFMSForm.SetPlaneFileName(Value: string);
var s: string;
begin
  if FPlaneFileName <> Value then
  begin
    FPlaneFileName := Value;
    s := ExtractFileName(FPlaneFileName);
    if LowerCase(ExtractFileExt(s)) = '.fms' then delete(s, Length(s)-3, 4);
    if s <> '' then s := ' - ' + s;
    Caption := LoadStr(MainCaption) + s;
  end;
end;

procedure TFMSForm.WndProc(var Message: TMessage);
var
  WinControl: TWinControl;
  sPos      : TSmallPoint;
begin
  case Message.Msg of
    WM_LBUTTONDOWN: if ShowHelpContext then
      begin
        WinControl := GetWinControlFromPoint(ClientToScreen(Point(Message.LParamLo, Message.LParamHi)), sPos);
        ShowHelpContext := false;
        if (WinControl <> nil) and (GetPopupHelpContext(WinControl.HelpContext) <> 0) then
        begin
          Application.HelpCommand(HELP_SETPOPUP_POS, Longint(sPos));
          Application.HelpCommand(HELP_CONTEXTPOPUP, GetPopupHelpContext(WinControl.HelpContext));
          exit;
        end;
      end;
    WM_HELP: exit;
  end;
  inherited WndProc(Message);
end;

procedure TFMSForm.AppMess(var Msg: TMsg; var Handled: boolean);
var
  WinControl: TWinControl;
  sPos      : TSmallPoint;
  i         : integer;
  s         : string;
begin
  case Msg.message of
    WM_RBUTTONDOWN: if not ShowHelpContext then
      begin
        WinControl := FindControl(Msg.hwnd);
        if WinControl <> nil then
        begin
          WinControl := GetWinControlFromPoint(WinControl.ClientToScreen(Point(LoWord(Msg.lParam), HiWord(Msg.lParam))), sPos);
          if (WinControl <> nil) and (WinControl.HelpContext <> 0) and (not isExistPropValue(WinControl, 'PopupMenu')) then
          begin
            with HelpContextPopupInfo do
            begin
              PopupHelpContext  := GetPopupHelpContext(WinControl.HelpContext);
              DetailHelpContext := GetDetailHelpContext(WinControl.HelpContext);
              sPos              := sPos;
            end;
            s := '';
            i := GetWindowTextLength(WinControl.Handle);
            if i > 0 then
            begin
              SetLength(s, i);
              if GetWindowText(WinControl.Handle, PChar(s), Length(s)+1) = 0 then s := '';
            end;
            try
              StrToFloat(s);
              s := '';
            except end;
            if (GetSystemDefaultLangID and $3FF) = LANG_RUSSIAN then
            begin
              if s <> '' then s := FmtLoadStr(WhatIsR1, [s])
                         else s := LoadStr(WhatIsR2);
            end else begin
              if s <> '' then s := FmtLoadStr(WhatIsE1, [s])
                         else s := LoadStr(WhatIsE2);
            end;
            WhatIsMenu.Caption     := s;
            WhatIsMenu.Visible     := HelpContextPopupInfo.PopupHelpContext <> 0;
            DetailHelpMenu.Visible := HelpContextPopupInfo.DetailHelpContext <> 0;
            HelpPopupMenu.Popup(sPos.x, sPos.y);
            Handled := true;
          end;
        end;
      end;
    WM_KEYDOWN: if Msg.wParam = VK_F1 then
      begin
        Contents1Click(nil);
        Handled := true;
      end;
  end;
end;

procedure TFMSForm.SetPlaneChange(Value: boolean);
begin
  if FPlaneChange <> Value then
  begin
    FPlaneChange       := Value;
    SaveButton.Enabled := Value;
    Save1.Enabled      := Value;
  end;
end;

procedure TFMSForm.SetPlaneLoaded(Value: boolean);
begin
  if FPlaneLoaded <> Value then
  begin
    FPlaneLoaded := Value;
    if not Value then PlaneChange := Value;
    BuildButton.Enabled    := Value;
    RunButton.Enabled      := Value;
    MapButton.Enabled      := Value;
    MapSpeedButton.Enabled := Value;
    ResultButton.Enabled   := Value;
    SaveAs1.Enabled        := Value;
    Show1.Enabled          := Value;
    Run1.Enabled           := Value;
    Compile1.Enabled       := Value;
    Result1.Enabled        := Value;
    TreeView.Enabled       := Value;
  end;
end;

procedure TFMSForm.SetShowHelpContext(Value: boolean);
const cur: array[boolean] of TCursor = (crDefault, crHelp);
begin
  if FShowHelpContext <> Value then
  begin
    FShowHelpContext     := Value;
    Screen.Cursor        := cur[Value];
    FMSForm.MouseCapture := Value;
    HelpSpeedButton.Perform(CM_MOUSELEAVE, 0, 0);
  end;
end;

procedure TFMSForm.ClearAllFMSObject;
const
  arrayCoor: array[0..9, 0..1] of double = ((5 , 10),
                                            (5 , 5),
                                            (10, 5),
                                            (15, 5),
                                            (20, 5),
                                            (20, 10),
                                            (20, 15),
                                            (15, 15),
                                            (10, 15),
                                            (5 , 15));
  arrayRel: array[0..9, 0..9] of double = ((0, 5, 0, 0, 0, 0, 0, 0, 0, 5),
                                           (5, 0, 5, 0, 0, 0, 0, 0, 0, 0),
                                           (0, 5, 0, 5, 0, 0, 0, 0, 0, 0),
                                           (0, 0, 5, 0, 5, 0, 0, 0, 0, 0),
                                           (0, 0, 0, 5, 0, 5, 0, 0, 0, 0),
                                           (0, 0, 0, 0, 5, 0, 5, 0, 0, 0),
                                           (0, 0, 0, 0, 0, 5, 0, 5, 0, 0),
                                           (0, 0, 0, 0, 0, 0, 5, 0, 5, 0),
                                           (0, 0, 0, 0, 0, 0, 0, 5, 0, 5),
                                           (5, 0, 0, 0, 0, 0, 0, 0, 5, 0));
var
  i, j: integer;
  tempSklad: TSklad;
begin
  try
    OTOs.Clear;
    TUs.Clear;
    Sklads.Clear;
    Nomenks.Clear;
    // Константы
    with FMSConst do
    begin
      Scale      := 24.0;
      dX         := 0;
      dY         := 0;
      minX       := MaxInt;
      minY       := MaxInt;
      maxX       := 0;
      maxY       := 0;
      WorkTime   := 0;
      doWorkTime := false;
    end;
    // Узлы
    FMSPoint.Count := 10;
    FMSPoint.PointCoor := VarArrayCreate([0, FMSPoint.Count-1, 0, 1], varDouble);
    for i := 0 to FMSPoint.Count-1 do
    begin
      FMSPoint.PointCoor[i, 0] := arrayCoor[i, 0];
      FMSPoint.PointCoor[i, 1] := arrayCoor[i, 1];
    end;
    FMSPoint.PointRel := VarArrayCreate([0, FMSPoint.Count-1, 0, FMSPoint.Count-1], varDouble);
    for i := 0 to FMSPoint.Count-1 do
      for j := 0 to FMSPoint.Count-1 do
        FMSPoint.PointRel[i, j] := arrayRel[i, j];
    // Создание складов (входного и выходного)
    tempSklad := TSklad.Create(Sklads, 'Склад входной');
    with tempSklad do
    begin
      skladX     := 3;
      skladY     := 9.5;
      skladDX    := 1.6;
      skladDY    := 1;
      skladValue := -1;
      skladPointNumber := 1;
      pNakopBusy := false;
      pNakopWait := false;
    end;
    tempSklad := TSklad.Create(Sklads, 'Склад выходной');
    with tempSklad do
    begin
      skladX     := 20.5;
      skladY     := 9.5;
      skladDX    := 1.6;
      skladDY    := 1;
      skladValue := 20;
      skladPointNumber := 6;
      pNakopBusy := false;
      pNakopWait := false;
    end;

    TreeView.Selected := OTOTreeNode;
    ShowFMSObject(OTOTreeNode);

    PlaneLoaded := not PlaneLoaded;
    PlaneLoaded := false;
  except
    raise FMSError.CreateRes(ErrorClearAll);
  end;
end;

procedure TFMSForm.SetFMSModelPath(Value: string);
begin
  FMSModelPath := GetNormalDir(Value);
  IniFile.WriteString('Application', 'Plane path', Value);
end;

procedure TFMSForm.FormCreate(Sender: TObject);
var
  s: string;
  i: integer;
  autoRun, flagPlane: boolean;

  function GetParamBeginPos(Value: string): integer;
  var i: integer;
  begin
    Result := 0;
    for i := 1 to ParamCount do
      if Pos(Value, ParamStr(i)) > 0 then begin Result := i; exit; end;
  end;

  procedure LoadBMPFromResource(Value: string);
  var
    ResBMP: TBitmap;
  begin
    ResBMP := TBitMap.Create;
    ResBMP.LoadFromResourceName(0, Value);
    TreeViewImageList.AddMasked(ResBMP, ResBMP.TransparentColor);
    ResBMP.Free;
  end;

begin
  with Application do
  begin
    OnException := FMSException;
    OnMessage   := AppMess;
    if HelpFile <> '' then HelpFile := GetNormalDir(ExeName)+ExtractFileName(HelpFile);
  end;
  FAllowChange    := true;
  FPlaneChange    := false;
  FPlaneLoaded    := false;
  FCheckBoxChange := true;
  ShowHelpContext := false;
  // Загрузка картинок в TreeViewImageList
  LoadBMPFromResource('TreeViewB1');
  LoadBMPFromResource('TreeViewB2');
  LoadBMPFromResource('TreeViewB3');
  LoadBMPFromResource('TreeViewB4');
  // Поддерживаемые в моделировнии законы поместили в выпадающие списки
  for i := Low(IntToLow) to High(IntToLow) do
    RLow.Items.Add(LowToStr[IntToLow[i]]);
  NomenkLow.Items.Assign(RLow.Items);
  // Создали поля TreeView
  OTOs    := TOTOs.Create(TOTO);
  TUs     := TTUs.Create(TTU);
  Sklads  := TSklads.Create(TSklad);
  Nomenks := TNomenks.Create(TNomenk);
  Create1.Caption       := LoadStr(CreateMenuCap);
  CreateOTO1.Caption    := LoadStr(CreateOTO);
  CreateTU1.Caption     := LoadStr(CreateTU);
  CreateNomenk1.Caption := LoadStr(CreateNomenk);
  OTOTreeNode    := TreeView.Items.Add(nil, LoadStr(OTONodeStr));
  TUTreeNode     := TreeView.Items.Add(nil, LoadStr(TUNodeStr));
  SkladTreeNode  := TreeView.Items.Add(nil, LoadStr(SkladNodeStr));
  NomenkTreeNode := TreeView.Items.Add(nil, LoadStr(NomenkNodeStr));
  PointTreeNode  := TreeView.Items.Add(nil, LoadStr(PointNodeStr));
  ConstTreeNode  := TreeView.Items.Add(nil, LoadStr(ConstNodeStr));
  OTOTreeNode.ImageIndex    := 0; OTOTreeNode.SelectedIndex    := 1;
  TUTreeNode.ImageIndex     := 0; TUTreeNode.SelectedIndex     := 1;
  SkladTreeNode.ImageIndex  := 0; SkladTreeNode.SelectedIndex  := 1;
  NomenkTreeNode.ImageIndex := 0; NomenkTreeNode.SelectedIndex := 1;
  PointTreeNode.ImageIndex  := 2; PointTreeNode.SelectedIndex  := 3;
  ConstTreeNode.ImageIndex  := 2; ConstTreeNode.SelectedIndex  := 3;

  ClearAllFMSObject;

  IniFile := TIniFile.Create('fms.ini');
  // Задание пути РДО-имитатора
  RdoImitatorPathName := IniFile.ReadString('Application', 'RDO path', '');
  // Задание пути к передаточному файлу
  PlaneFileName       := IniFile.ReadString('Application', 'Plane path', '');
  s := GetNormalDir(Application.ExeName)+'plane.fms';
  if PlaneFileName = ''
    then PlaneFileName := s
    else if not FileExists(PlaneFileName) then PlaneFileName := s;
  SetFMSModelPath(PlaneFileName);
  // Обработать командную строку
  autoRun   := false;
  flagPlane := false;
  if ParamCount > 0 then
  begin
    // Автозапуск модели
    if GetParamBeginPos('-run') > 0 then autoRun := true;
    // Получить имя передаточного файла
    i := GetParamBeginPos('-planefile');
    if i > 0 then
      if not (i > ParamCount)  then
      begin
        PlaneFileName := ParamStr(i+1);
        flagPlane     := true;
      end else raise FMSError.CreateResFmt(ErrorCmdParams, ['-planefile <no name>']);
    // Получить путь РДО-имитатора
    i := GetParamBeginPos('-rdofile');
    if i > 0 then
      if not (i > ParamCount)  then
      begin
        RdoImitatorPathName := ParamStr(i+1);
        if FileExists(RdoImitatorPathName)
          then IniFile.WriteString('Application', 'RDO path', RdoImitatorPathName)
          else raise FMSError.CreateResFmt(ErrorCmdParams, [RdoImitatorPathName]);
      end else raise FMSError.CreateResFmt(ErrorCmdParams, ['-rdofile <no name>']);
  end;
  FMSModelPath := GetNormalDir(PlaneFileName);
  // Загрузить передаточный файл
  if FileExists(PlaneFileName) then
  begin
    LoadPlaneFile(PlaneFileName);
    if autoRun then RunButtonClick(nil);
  end else begin
    if flagPlane then raise FMSError.CreateResFmt(ErrorCmdParams, [PlaneFileName]);
    PlaneFileName := '';
  end;
end;

procedure TFMSForm.UpDateListSeparator;
var Buffer: array[0..1] of Char;
begin
  ListSeparator := ';';
  try
    if GetLocaleInfo(LOCALE_SYSTEM_DEFAULT, LOCALE_SLIST, Buffer, SizeOf(Buffer)) > 0
      then ListSeparator := Buffer[0];
  except
  end;
end;

procedure TFMSForm.LoadPlaneFile(Value: string);
var
  i, j, k: integer;
  s, s2, StrIni: string;
  SectionList: TStringList;
  tempFMS: TFMSObject;
  tempTP: ^TTP;
  SplashForm: TSplashWindow;
  tempFileStream: TFileStream;

  function CutFirstListItem(var Value: string): string;
  var i: integer;
  begin
    i := Pos(ListSeparator, Value);
    if i = 0 then i := Length(Value)+1;
    Result := Copy(Value, 1, i-1);
    Delete(Value, 1, i);
  end;

begin
  try
    SplashForm := ShowSplashWindow(Application.Icon, 'Обработка компоновки. Подождите пожалуйста...', false, nil);
    try
      UpDateListSeparator;
      if ListSeparator = DecimalSeparator then raise FMSError.CreateRes(ErrorDecimalAndListSeparators);
      SetFMSModelPath(Value);
      try
        tempFileStream := TFileStream.Create(Value, fmOpenRead);
        tempFileStream.Free;
      except raise FMSError.CreateResFmt(ErrorFileOpen, [Value]);
      end;
//      CreateDir(FMSModelPath+'DBBitmaps');
      if Assigned(PlaneFile) then PlaneFile.Free;
      PlaneFile := TIniFile.Create(Value);
      ClearAllFMSObject;
      // Обработка файла
      // Константы
      StrIni := 'Константы';
      with FMSConst do
      begin
        Scale      := StrToFloat(PlaneFile.ReadString(StrIni, 'Scale', '24'));
        WorkTime   := StrToFloat(PlaneFile.ReadString(StrIni, 'WorkTime', '0'));
        doWorkTime := WorkTime <> 0;
      end;
      // Склад входной
      StrIni := 'Склад_входной';
      with TSklad(Sklads[0]) do
      begin
        ReadPicture(StrIni);
        skladX     := StrToFloat(PlaneFile.ReadString(StrIni, 'skladX', '3'));
        skladY     := StrToFloat(PlaneFile.ReadString(StrIni, 'skladY', '10'));
        skladDX    := StrToFloat(PlaneFile.ReadString(StrIni, 'skladDX', '2'));
        skladDY    := StrToFloat(PlaneFile.ReadString(StrIni, 'skladDY', '1'));
        skladValue := -1;
        skladPointNumber := PlaneFile.ReadInteger(StrIni, 'skladPoint', 1);
        pNakopBusy := PlaneFile.ReadBool(StrIni, 'pNakopBusy', false);
        pNakopWait := PlaneFile.ReadBool(StrIni, 'pNakopWait', false);
      end;
      // Склад выходной
      StrIni := 'Склад_выходной';
      with TSklad(Sklads[1]) do
      begin
        ReadPicture(StrIni);
        skladX     := StrToFloat(PlaneFile.ReadString(StrIni, 'skladX', '20'));
        skladY     := StrToFloat(PlaneFile.ReadString(StrIni, 'skladY', '10'));
        skladDX    := StrToFloat(PlaneFile.ReadString(StrIni, 'skladDX', '2'));
        skladDY    := StrToFloat(PlaneFile.ReadString(StrIni, 'skladDY', '1'));
        skladValue := PlaneFile.ReadInteger(StrIni, 'skladValue', 20);
        skladPointNumber := PlaneFile.ReadInteger(StrIni, 'skladPoint', 1);
        pNakopBusy := PlaneFile.ReadBool(StrIni, 'pNakopBusy', false);
        pNakopWait := PlaneFile.ReadBool(StrIni, 'pNakopWait', false);
      end;

      SectionList := TStringList.Create;
      try
        // ОТО
        StrIni := 'ОТО_ID_';
        i      := 1;
        repeat
          s := StrIni+IntToStr(i);
          SectionList.Clear;
          PlaneFile.ReadSection(s, SectionList);
          if SectionList.Count > 0 then
          begin
            tempFMS := TOTO.Create(PlaneFile.ReadString(s, 'otoName', ''));
            with tempFMS as TOTO do
            begin
              ReadPicture(s);
              RLow       := GetModelLow(PlaneFile.ReadString(s, 'RLow', 'uniform'));
              RLowValue1 := StrToFloat(PlaneFile.ReadString(s, 'RLowValue1', '10'));
              RLowValue2 := StrToFloat(PlaneFile.ReadString(s, 'RLowValue2', '50'));
              RTimeMin   := StrToFloat(PlaneFile.ReadString(s, 'RTimeMin', '3'));
              RTimeMax   := StrToFloat(PlaneFile.ReadString(s, 'RTimeMax', '5'));
              otoX       := StrToFloat(PlaneFile.ReadString(s, 'otoX', '1'));
              otoY       := StrToFloat(PlaneFile.ReadString(s, 'otoY', '1'));
              otoDX      := StrToFloat(PlaneFile.ReadString(s, 'otoDX', '3'));
              otoDY      := StrToFloat(PlaneFile.ReadString(s, 'otoDY', '2'));
              nakopX     := StrToFloat(PlaneFile.ReadString(s, 'nakopX', '2'));
              nakopY     := StrToFloat(PlaneFile.ReadString(s, 'nakopY', '2'));
              nakopValue := PlaneFile.ReadInteger(s, 'nakopValue', 25);
              nakopPointNumber := PlaneFile.ReadInteger(s, 'nakopPoint', 1);
              pOtoBusy   := PlaneFile.ReadBool(s, 'pOtoBusy', false);
              pOtoWait   := PlaneFile.ReadBool(s, 'pOtoWait', false);
              pOtoRepair := PlaneFile.ReadBool(s, 'pOtoRepair', false);
              pNakopBusy := PlaneFile.ReadBool(s, 'pNakopBusy', false);
              pNakopWait := PlaneFile.ReadBool(s, 'pNakopWait', false);
            end;
          end;
          inc(i);
        until SectionList.Count = 0;
        // ТУ
        StrIni := 'ТУ_ID_';
        i      := 1;
        repeat
          s := StrIni+IntToStr(i);
          SectionList.Clear;
          PlaneFile.ReadSection(s, SectionList);
          if SectionList.Count > 0 then
          begin
            tempFMS := TTU.Create(PlaneFile.ReadString(s, 'tuName', ''));
            with tempFMS as TTU do
            begin
              ReadPicture(s);
              Speed    := StrToFloat(PlaneFile.ReadString(s, 'Speed', '30'));
              TimeLoad := StrToFloat(PlaneFile.ReadString(s, 'TimeLoad', '1'));
              tuX      := StrToFloat(PlaneFile.ReadString(s, 'tuX', '4'));
              tuY      := StrToFloat(PlaneFile.ReadString(s, 'tuY', '4'));
              tuDX     := StrToFloat(PlaneFile.ReadString(s, 'tuDX', '1'));
              tuDY     := StrToFloat(PlaneFile.ReadString(s, 'tuDY', '1'));
              PredPointNumber := PlaneFile.ReadInteger(s, 'PredPoint', 1);
              pTuTransport := PlaneFile.ReadBool(s, 'pTuTransport', false);
              pTuFree      := PlaneFile.ReadBool(s, 'pTuFree', false);
              pTuEmpty     := PlaneFile.ReadBool(s, 'pTuEmpty', false);
              pTuLoad      := PlaneFile.ReadBool(s, 'pTuLoad', false);
              j            := PlaneFile.ReadInteger(s, 'Position', 1);
              if j = 0 then raise FMSError.CreateResFmt(ErrorSelectTU_OTO, [Name]);
              try
                if j > 0 then OTOPos := OTOs[j-1]
                         else OTOPos := Sklads[-1*j-1];
              except
                raise FMSError.CreateResFmt(ErrorSelectTU_OTO, [Name]);
              end;
            end;
          end;
          inc(i);
        until SectionList.Count = 0;
        // Номенклатура
        StrIni := 'Номенклатура_ID_';
        i      := 1;
        repeat
          s := StrIni+IntToStr(i);
          SectionList.Clear;
          PlaneFile.ReadSection(s, SectionList);
          if SectionList.Count > 0 then
          begin
            tempFMS := TNomenk.Create(PlaneFile.ReadString(s, 'nomenkName', ''));
            with tempFMS as TNomenk do
            begin
              k := 1;
              repeat
                j := PlaneFile.ReadInteger(s, 'TPOTO'+IntToStr(k), 0);
                if j <> 0 then
                begin
                  new(tempTP);
                  AddTP(tempTP);
                  if j > 0 then tempTP^.OTO := OTOs[j-1]
                    else if j < 0 then tempTP^.OTO := Sklads[-1*j-1];
                  tempTP^.Time := StrToFloat(PlaneFile.ReadString(s, 'TPTime'+IntToStr(k), '1'));
                end;
                inc(k);
              until j = 0;
              ReadPicture(s);
              AllValue   := PlaneFile.ReadInteger(s, 'AllValue', 40);
              StartParty := PlaneFile.ReadInteger(s, 'StartParty', 10);
              WorkParty  := PlaneFile.ReadInteger(s, 'WorkParty', 4);
              Low        := GetModelLow(PlaneFile.ReadString(s, 'Low', 'uniform'));
              LowValue1  := StrToFloat(PlaneFile.ReadString(s, 'LowValue1', '10'));
              LowValue2  := StrToFloat(PlaneFile.ReadString(s, 'LowValue2', '35'));
            end;
          end;
          inc(i);
        until SectionList.Count = 0;
        // Узлы
        StrIni := 'Узлы';
        i      := 0;
        repeat
          s := PlaneFile.ReadString(StrIni, 'Coor'+IntToStr(i+1), '');
          if s <> '' then inc(i);
        until s = '';
        if i < 1 then raise FMSError.CreateRes(ErrorPointCoor);
        FMSPoint.Count := i;
        FMSPoint.PointCoor := UnAssigned;
        FMSPoint.PointRel  := UnAssigned;
        FMSPoint.PointCoor := VarArrayCreate([0, FMSPoint.Count-1, 0, 1], varDouble);
        FMSPoint.PointRel  := VarArrayCreate([0, FMSPoint.Count-1, 0, FMSPoint.Count-1], varDouble);
        // Матрица координат узлов
        for i := 0 to FMSPoint.Count-1 do
        begin
          s := PlaneFile.ReadString(StrIni, 'Coor'+IntToStr(i+1), '');
          if s <> '' then
          begin
            k := 0;
            while s <> '' do
            begin
              s2 := CutFirstListItem(s);
              if s2 <> '' then
              begin
                if k > 1 then raise FMSError.CreateRes(ErrorPointCoor);
                FMSPoint.PointCoor[i, k] := StrToFloat(s2);
              end;
              inc(k);
            end;
            if k <> 2 then raise FMSError.CreateRes(ErrorPointCoor);
          end else raise FMSError.CreateRes(ErrorPointCoor);
        end;
        // Матрица связанности узлов
        for i := 0 to FMSPoint.Count-1 do
        begin
          s := PlaneFile.ReadString(StrIni, 'Rel'+IntToStr(i+1), '');
          if s <> '' then
          begin
            k := 0;
            while s <> '' do
            begin
              s2 := CutFirstListItem(s);
              if s2 <> '' then
              begin
                if k > FMSPoint.Count-1 then raise FMSError.CreateRes(ErrorPointRel);
                FMSPoint.PointRel[i, k] := StrToFloat(s2);
              end;
              inc(k);
            end;
            if k <> FMSPoint.Count then raise FMSError.CreateRes(ErrorPointRel);
          end else raise FMSError.CreateRes(ErrorPointRel);
        end;
        // Подсчитать расстояния и занести в матрицу (вместо единиц)
        with FMSPoint do
          for i := 0 to Count-2 do
            for j := i+1 to Count-1 do
              if PointRel[i, j] = 1 then
              begin
                s := Format('%3.2f', [Sqrt(Sqr(PointCoor[i, 0]-PointCoor[j, 0])+Sqr(PointCoor[i, 1]-PointCoor[j, 1]))]);
                PointRel[i, j] := StrToFloat(s);
                PointRel[j, i] := PointRel[i, j];
              end;
        with FMSConst do GetMinMaxMetrics(minX, minY, maxX, maxY);
        PlaneLoaded := true;
      finally
        SectionList.Free;
      end;
    finally
      PlaneChange := false;
      SplashForm.Free;
    end;
  except
    on E: FMSError do raise FMSError.CreateSubError(ErrorLoadFMSFile, E.Message);
    else raise FMSError.CreateRes(ErrorLoadFMSFile);
  end;
end;

procedure TFMSForm.FMSException(Sender: TObject; E: Exception);
var
  flag: boolean;
  i: integer;
  s1, s2, s3: string;
begin
  flag := true;
  if Pos('is not a valid floating point value', E.Message) > 0 then
    flag := ShowErrorMessage(FmtLoadStr(ErrorDecimalSeparator, [DecimalSeparator, ListSeparator]));
  if flag then
    if E.ClassName = 'FMSError' then flag := ShowErrorMessage(E.Message)
                                else Application.ShowException(E);
  if flag then
  begin
    s1 := Sender.ClassName;
    if Sender is TComponent then s1 := s1 + '_' + (Sender as TComponent).Name;
    s1 := s1 + '_' + IntToStr(Round(Random(30000000)));
    s2 := E.Message;
    s3 := '';
    for i := 1 to Length(s2) do
      if (s2[i] = #13) or (s2[i] = #10) then s3 := s3+'_'
                                        else s3 := s3+s2[i];
    IniFile.WriteString('Error', s1, s3);
  end;
end;

procedure TFMSForm.TreeViewMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
begin
  if FAllowChange then
  begin
    TreeView.Selected := TreeView.GetNodeAt(X, Y);
    UpDateFMSObject;
  end;
end;

procedure TFMSForm.ShowFMSObject(Value: TTreeNode);
const
  BevelCoor : array[0..3, 1..2] of integer = ((280, 114), (276, 170), (260, 102), (16, 246));
  ImageDelta: integer = 2;
var
  SelTreeNode: TTreeNode;
  tempPanel: TPanel;
  i, j: integer;
  flag: boolean;

  procedure HidePanel(Value: TPanel);
  var s: string;
  begin
    if tempPanel <> Value then
    begin
      s := Value.Name;
      Delete(s, Length(s)-4, 5);
      Value.Parent := TWinControl(FindComponent(s+'Sheet'));
    end;
  end;

  procedure ShowLowValue(tempLow: TModelLow; var comboLow: TComboBox; var Value1, Value2: TStaticText; var Value2Edit: TEdit);
  begin
    comboLow.ItemIndex := LowToInt[tempLow];
    case tempLow of
      mlUniform    : begin
                       Value1.Caption := 'от';
                       Value2.Caption := 'до';
                     end;
      mlNormal     : begin
                       Value1.Caption := 'среднее';
                       Value2.Caption := 'дисперсия';
                     end;
      mlExponential: Value1.Caption := 'среднее';
      mlConst      : Value1.Caption := 'число';
    end;
    with Value2 do
    begin
      Visible := (tempLow = mlUniform) or (tempLow = mlNormal);
      Value2Edit.Visible := Visible;
      if not Visible then Caption := '';
    end;
  end;

begin
  try
    try
      FCheckBoxChange := false; // Запретить обработку изменений в событии OnClick у CheckBox'ов
      SelTreeNode := Value;
      TreeViewDelMenu.Visible := false;
      flag := false;
      if SelTreeNode.Parent <> nil then
      begin
        flag := true;
        ActiveFMSObject := TFMSObject(SelTreeNode.Data);
        TreeViewDelMenu.Caption := LoadStr(DelStr)+ActiveFMSObject.Name;
        SelTreeNode := SelTreeNode.Parent;
        TreeViewDelMenu.Visible := (SelTreeNode.Index = TVOTO) or (SelTreeNode.Index = TVTU) or (SelTreeNode.Index = TVNomenk);
      end else ActiveFMSObject := nil;
      flag := flag or (SelTreeNode.Index = TVPoint) or (SelTreeNode.Index = TVConst);
      TreeViewCreateMenu.Visible := true;
      case SelTreeNode.Index of
        TVOTO    : TreeViewCreateMenu.Caption := LoadStr(CreateOTO);
        TVTU     : TreeViewCreateMenu.Caption := LoadStr(CreateTU);
        TVNomenk : TreeViewCreateMenu.Caption := LoadStr(CreateNomenk);
        else TreeViewCreateMenu.Visible := false;
      end;
  // ----------- Удалить следующие две строчки в нормальной версии
      TreeViewCreateMenu.Visible := false;
      TreeViewDelMenu.Visible    := false;
      case SelTreeNode.Index of
        TVOTO    : tempPanel := OTOPanel;
        TVTU     : tempPanel := TUPanel;
        TVSklad  : tempPanel := SkladPanel;
        TVNomenk : tempPanel := NomenkPanel;
        TVPoint  : tempPanel := PointPanel;
        TVConst  : tempPanel := ConstPanel;
        else raise FMSError.CreateRes(ErrorSelectTreeNode);
      end;
      HidePanel(OTOPanel);
      HidePanel(TUPanel);
      HidePanel(SkladPanel);
      HidePanel(NomenkPanel);
      HidePanel(PointPanel);
      HidePanel(ConstPanel);
      if (tempPanel = OTOPanel)   or (tempPanel = TUPanel) or
         (tempPanel = SkladPanel) or (tempPanel = NomenkPanel) then
      begin
        PicImageBevel.Parent := tempPanel;
        PicImagePanel.Parent := tempPanel;
        PicImageBevel.Left := BevelCoor[SelTreeNode.Index, 1];
        PicImageBevel.Top  := BevelCoor[SelTreeNode.Index, 2];
        PicImagePanel.Left := PicImageBevel.Left + 2;
        PicImagePanel.Top  := PicImageBevel.Top  + 2;
      end;
      for i := 0 to tempPanel.ControlCount-1 do
        if (not flag) or ((tempPanel.Controls[i].Name <> 'RLowValue2') and
                          (tempPanel.Controls[i].Name <> 'NomenkLowValue2')) then
          tempPanel.Controls[i].Visible := flag;
      tempPanel.Left := 0 - ScrollBox.HorzScrollBar.Position;
      tempPanel.Top  := 0 - ScrollBox.VertScrollBar.Position;;
      tempPanel.Parent := ScrollBox;
      if flag then
      begin
        case SelTreeNode.Index of
          TVOTO    : with ActiveFMSObject as TOTO do
                     begin
                       OTOName.Text            := Name;
                       FMSForm.RLowValue1.Text := FloatToStr(RLowValue1);
                       FMSForm.RLowValue2.Text := FloatToStr(RLowValue2);
                       FMSForm.RTimeMin.Text   := FloatToStr(RTimeMin);
                       FMSForm.RTimeMax.Text   := FloatToStr(RTimeMax);
                       FMSForm.OTOX.Text       := FloatToStr(otoX);
                       FMSForm.OTOY.Text       := FloatToStr(otoY);
                       FMSForm.OTODX.Text      := FloatToStr(otoDX);
                       FMSForm.OTODY.Text      := FloatToStr(otoDY);
                       FMSForm.NakopValue.Text := FloatToStr(nakopValue);
                       FMSForm.NakopX.Text     := FloatToStr(nakopX);
                       FMSForm.NakopY.Text     := FloatToStr(nakopY);
                       NakopPoint.Text         := IntToStr(nakopPointNumber);
                       OTOPBusy.Checked        := pOtoBusy;
                       OTOPWait.Checked        := pOtoWait;
                       OTOPRepair.Checked      := pOtoRepair;
                       NakopPBusy.Checked      := pNakopBusy;
                       NakopPWait.Checked      := pNakopWait;
                       ShowLowValue(RLow, FMSForm.RLow, RLowValue1Label, RLowValue2Label, FMSForm.RLowValue2);
                       DrawPicture(otoDX, otoDY, true);
                     end;
          TVTU     : with ActiveFMSObject as TTU do
                     begin
                       TUName.Text      := Name;
                       TUSpeed.Text     := FloatToStr(Speed);
                       TUTimeLoad.Text  := FloatToStr(TimeLoad);
                       if OTOPos = nil then
                       begin
                         TUPosition.Text := 'нет';
                         TUPositionNameLabel.Caption := '';
                       end else begin
                         TUPosition.Text := IntToStr(OTOPos.ItemID);
                         TUPositionNameLabel.Caption := OTOPos.Name;
                       end;
                       FMSForm.TUX.Text  := FloatToStr(tuX);
                       FMSForm.TUY.Text  := FloatToStr(tuY);
                       FMSForm.TUDX.Text := FloatToStr(tuDX);
                       FMSForm.TUDY.Text := FloatToStr(tuDY);
                       TUPredPoint.Text  := IntToStr(PredPointNumber);
                       TUPTransport.Checked := pTuTransport;
                       TUPFree.Checked      := pTuFree;
                       TUPEmpty.Checked     := pTuEmpty;
                       TUPLoad.Checked      := pTuLoad;
                       DrawPicture(tuDX, tuDY, true);
                     end;
          TVSklad  : with ActiveFMSObject as TSklad do
                     begin
                       SkladName.Text := Name;
                       if skladValue <> - 1
                         then FMSForm.SkladValue.Text := FloatToStr(skladValue)
                         else FMSForm.SkladValue.Text := 'не ограничена';
                       FMSForm.SkladX.Text  := FloatToStr(skladX);
                       FMSForm.SkladY.Text  := FloatToStr(skladY);
                       FMSForm.SkladDX.Text := FloatToStr(skladDX);
                       FMSForm.SkladDY.Text := FloatToStr(skladDY);
                       SkladPoint.Text      := IntToStr(skladPointNumber);
                       SkladPBusy.Checked   := pNakopBusy;
                       SkladPWait.Checked   := pNakopWait;
                       DrawPicture(skladDX, skladDY, true);
                     end;
          TVNomenk : with ActiveFMSObject as TNomenk do
                     begin
                       NomenkName.Text       := Name;
                       NomenkAllValue.Text   := IntToStr(AllValue);
                       NomenkStartParty.Text := IntToStr(StartParty);
                       NomenkWorkParty.Text  := IntToStr(WorkParty);
                       NomenkLowValue1.Text  := FloatToStr(LowValue1);
                       NomenkLowValue2.Text  := FloatToStr(LowValue2);
                       with TPGrid do
                       begin
                         Cells[0, 0] := '   №';
                         Cells[1, 0] := '  ОТО';
                         Cells[2, 0] := ' Длит.';
                         if TPCount+1 = 1 then RowCount := 2
                                          else RowCount := TPCount+1;
                         Cells[0, 1] := '';
                         Cells[1, 1] := '';
                         Cells[2, 1] := '';
                         for i := 0 to TPCount-1 do
                         begin
                           Cells[0, i+1] := ' '+IntToStr(i+1);
                           Cells[1, i+1] := TP[i].OTO.Name;
                           Cells[2, i+1] := FloatToStr(TP[i].Time);
                         end;
                       end;
                       ShowLowValue(Low, NomenkLow, NomenkLowValue1Label, NomenkLowValue2Label, NomenkLowValue2);
                       DrawPicture(0, 0, false);
                     end;
          TVPoint  : begin
                       with PointCoorGrid do
                       begin
                         Cells[1, 0] := '  X';
                         Cells[2, 0] := '  Y';
                         if FMSPoint.Count+1 = 1 then RowCount := 2
                                                 else RowCount := FMSPoint.Count+1;
                         for i := 0 to FMSPoint.Count-1 do
                         begin
                           Cells[0, i+1] := ' '+IntToStr(i+1);
                           Cells[1, i+1] := FloatToStr(FMSPoint.PointCoor[i, 0]);
                           Cells[2, i+1] := FloatToStr(FMSPoint.PointCoor[i, 1]);
                         end;
                       end;
                       with PointRelGrid do
                       begin
                         ColCount := PointCoorGrid.RowCount;
                         RowCount := ColCount;
                         for i := 0 to FMSPoint.Count-1 do
                         begin
                           Cells[i+1, 0] := ' '+IntToStr(i+1);
                           Cells[0, i+1] := ' '+IntToStr(i+1);
                         end;
                         for i := 0 to FMSPoint.Count-1 do
                           for j := 0 to FMSPoint.Count-1 do
                             if FMSPoint.PointRel[i, j] < 0 then Cells[j+1, i+1] := '-1' else
                               if FMSPoint.PointRel[i, j] = 0 then Cells[j+1, i+1] := '0'
                                                              else Cells[j+1, i+1] := '1'
                       end;
                     end;
          TVConst  : begin
                       Scale.Text := FloatToStr(FMSConst.Scale);
                       with WorkTime do
                       begin
                         Text    := FloatToStr(FMSConst.WorkTime);
                         Enabled := FMSConst.doWorkTime;
                         Ctl3D   := FMSConst.doWorkTime;
                       end;
                       WorkTimeRadioButton.Checked := FMSConst.doWorkTime;
                     end;
        end;
      end;
      if ActiveFMSObject <> nil
        then StatusBar.Panels[0].Text := Format('Идентификатор устройства в группе = %d', [ActiveFMSObject.ItemID])
        else StatusBar.Panels[0].Text := '';
    finally
      FCheckBoxChange := true; // Разрешить обработку изменений в событии OnClick у CheckBox'ов
    end;
  except
    on E: FMSError do raise FMSError.CreateSubError(ErrorShowFMSObject, E.Message);
    else raise FMSError.CreateRes(ErrorShowFMSObject);
  end;
end;

procedure TFMSForm.TreeViewCreateMenuClick(Sender: TObject);
var SelTreeNode: TTreeNode;
begin
  SelTreeNode := TreeView.Selected;
  if SelTreeNode.Parent <> nil then SelTreeNode := SelTreeNode.Parent;
  case SelTreeNode.Index of
    TVOTO    : TOTO.Create('');
    TVTU     : TTU.Create('');
    TVNomenk : TNomenk.Create('');
  end;
end;

procedure TFMSForm.TreeViewDelMenuClick(Sender: TObject);
begin
  if TreeView.Selected.Data <> nil then TFMSObject(TreeView.Selected.Data).Free;
end;

procedure TFMSForm.CreateOTO1Click(Sender: TObject);
begin
  TOTO.Create('');
end;

procedure TFMSForm.CreateTU1Click(Sender: TObject);
begin
  TTU.Create('');
end;

procedure TFMSForm.CreateNomenk1Click(Sender: TObject);
begin
  TNomenk.Create('');
end;

procedure TFMSForm.FormDestroy(Sender: TObject);
begin
  OTOs.Free;
  TUs.Free;
  Sklads.Free;
  Nomenks.Free;
end;

procedure TFMSForm.TreeViewKeyUp(Sender: TObject; var Key: Word;
  Shift: TShiftState);
begin
  UpDateFMSObject;
end;

procedure TFMSForm.PathButtonClick(Sender: TObject);
begin
  with OpenDialog do
  begin
    Title      := 'Задать РДО-имитатор';
    InitialDir := ExtractFileDir(RdoImitatorPathName);
    DefaultExt := 'exe';
    Filter     := 'РДО-имитатор v3.0|rdosim.exe|РДО-имитатор|*.exe|Все|*.*';
    FileName   := '';
    if Execute then
    begin
      RdoImitatorPathName := FileName;
      IniFile.WriteString('Application', 'RDO path', RdoImitatorPathName);
    end;
  end;
end;

procedure TFMSForm.NewButtonClick(Sender: TObject);
begin
  if SaveQueryAndSave then
  begin
    ClearAllFMSObject;
    PlaneFileName := '';
    PlaneLoaded   := true;
  end;
end;

procedure TFMSForm.OpenButtonClick(Sender: TObject);
begin
  if SaveQueryAndSave then
    with OpenDialog do
    begin
      Title      := 'Открыть файл компоновки';
      InitialDir := ExtractFileDir(PlaneFileName);
      DefaultExt := 'fms';
      Filter     := 'Файл компоновки|*.fms|Все файлы|*.*';
      FileName   := '';
      if Execute then
      begin
        PlaneFileName := FileName;
        LoadPlaneFile(PlaneFileName);
      end;
    end;
end;

procedure TFMSForm.Save1Click(Sender: TObject);
begin
  SaveMenu;
end;

procedure TFMSForm.SaveAs1Click(Sender: TObject);
begin
  if SaveAsFun then SaveReal;
end;

function TFMSForm.SaveAsFun: boolean;
begin
  Result := false;
  with SaveDialog do
    if Execute then
    begin
      if FileExists(FileName)
        then case ShowQueryMessage(FmtLoadStr(SaveAsQuery, [FileName]), MB_YESNO) of
               mrYes: begin
                        Result := true;
                      end;
               else begin
                 end;
             end
        else begin
            Result := true;
          end;
    end;
  if Result then SetFMSModelPath(SaveDialog.FileName);
end;

function TFMSForm.SaveQueryAndSave: boolean;
begin
  Result := false;
  ChangeFMSObject;
  if PlaneChange then
    case ShowQueryMessage(LoadStr(sCloseQuery), MB_YESNOCANCEL) of
      IDYES   : Result := SaveMenu;
      IDNO    : Result := true;
      IDCANCEL: Result := false;
    end
  else Result := true;
end;

function TFMSForm.SaveMenu: boolean;
var flag: boolean;
begin
  Result := false;
  ChangeFMSObject;
  flag := PlaneFileName <> '';
  if not flag then flag := SaveAsFun;
  if flag then Result := SaveReal;
end;

function TFMSForm.SaveReal: boolean;
begin
  try
    ChangeFMSObject;
    PlaneChange := false;
    ShowInfoMessage('Запись данных была проведена виртуально. После УТВЕРЖДЕНИЯ формата файла и РЕАЛИЗАЦИИ его поддержки со стороны базы данных, будут проведены работы по физической записи данных');
    Result := true;
  except
    on E: FMSError do raise FMSError.CreateStopSubError(ErrorSaveFMSFile, E.Message);
    else raise FMSError.CreateStopRes(ErrorSaveFMSFile);
    Result := false;
  end;
end;

procedure TFMSForm.BuildButtonClick(Sender: TObject);
begin
  BuildModel;
end;

procedure TFMSForm.BuildModel;
const
  ModelLow: array[TModelLow] of string = ('Равномерный_Закон(Число_1, Число_2)',
                                          'Нормальный_Закон_Abs(Нормальный_Закон(Число_1, Число_2))',
                                          'Экспоненциальный_Закон(Число_1)',
                                          'Число_1');
  SkladName : array[0..1] of string = (' Накопитель_вх', ' Накопитель_вых');
  addScale  : string = ' * Const.Scale, ';
  addScaleX : string = ' * Const.Scale + Const.dX, ';
  addScaleY : string = ' * Const.Scale + Const.dY, ';
  sTextColor: string = 'transparent,<255 255 255>, ';
var
  ResStream: TResourceStream;
  ModelStream: TFileStream;
  ModelStrings: TStringList;
  i, j, MaxOperationNumber: integer;
  s, s2, s3, sDX2, sDY2: string;
  tempDecSeparator: Char;

  procedure SaveResourceToStream(const ID: integer; StreamValue: TFileStream);
  begin
    ResStream := TResourceStream.CreateFromID(0, ID, RT_RCDATA);
    ResStream.SaveToStream(StreamValue);
    ResStream.Free;
  end;

  function GetMaxOperationNumber: integer;
  var i: integer;
  begin
    Result := -1;
    for i := 0 to Nomenks.Count-1 do
      if Result < Nomenks[i].TPCount then Result := Nomenks[i].TPCount
  end;

  function RdoFloatToStr(Value: double): string;
  var s: char;
  begin
    s := DecimalSeparator;
    DecimalSeparator := '.';
    Result := FloatToStr(Value);
    DecimalSeparator := s;
  end;

  function RdoZeroFloatToStr(Value: double): string;
  begin
    Result := RdoFloatToStr(Value);
    if Pos('.', Result) = 0 then Result := Result + '.0';
  end;

  function GetStringScale(DX, DY: double): string;
  begin
    Result := RdoFloatToStr(DX)+' * Const.Scale, '+RdoFloatToStr(DY)+' * Const.Scale, ';
  end;

  procedure SaveBMPFromResource(Value: string);
  var ResBMP: TBitmap;
  begin
    ResBMP := TBitMap.Create;
    ResBMP.LoadFromResourceName(0, Value);
    ResBMP.SaveToFile(FMSModelPath+Value+'.bmp');
    ResBMP.Free;
  end;

begin
  try
    ChangeFMSObject;
    ModelStrings := TStringList.Create;
    try
      MaxOperationNumber := GetMaxOperationNumber;
      // PAT
      ResStream := TResourceStream.CreateFromID(0, RC_PAT, RT_RCDATA);
      ResStream.SaveToFile(FMSModelPath+FMSModelName+'.pat');
      ResStream.Free;
      // RTP
      ModelStream := TFileStream.Create(FMSModelPath+FMSModelName+'.rtp', fmCreate);
      SaveResourceToStream(RC_RTP1, ModelStream);
      with ModelStrings do
      begin
        // ОТО
        Clear;
        Add('  Пор_ном_ОТО : integer[1..'+IntToStr(OTOs.Count)+']');
        SaveToStream(ModelStream);
        SaveResourceToStream(RC_RTP2, ModelStream);
        Clear;
        for i := 1 to OTOs.Count do
          Add('                 ОТО' + IntToStr(i)+',');
        SaveToStream(ModelStream);
        SaveResourceToStream(RC_RTP3, ModelStream);
        // ТУ
        Clear;
        s := IntToStr(FMSPoint.Count)+'] = 1';
        Add(s);
        Add('  Пред_узел    : integer[1..' + s);
        SaveToStream(ModelStream);
        SaveResourceToStream(RC_RTP4, ModelStream);
        // Система
        Clear;
        for i := 1 to Nomenks.Count do
        begin
          s := 'тип'+IntToStr(i);
          if i = 1 then
          begin
            s2 := s;
            s  := '  Тип           : (' + s;
          end else s := '                   ' + s;
          if i = Nomenks.Count then s := s + ') = '+s2
                               else s := s +',';
          Add(s);
        end;
        SaveToStream(ModelStream);
        SaveResourceToStream(RC_RTP5, ModelStream);
        // Пер_партии
        Clear;
        Add(IntToStr(MaxOperationNumber+1)+'] = 1');
        SaveToStream(ModelStream);
      end;
      SaveResourceToStream(RC_RTP6, ModelStream);
      ModelStream.Free;
      // RSS
      ModelStream := TFileStream.Create(FMSModelPath+FMSModelName+'.rss', fmCreate);
      with ModelStrings do
      begin
        Clear;
        Add('$Resources');
        // Константы
        tempDecSeparator := DecimalSeparator;
        DecimalSeparator := '.';
        with FMSConst do
          Add(Format('  Const : _Const %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f %3.2f *', [Scale, dX, dY, minX, minY, maxX, maxY]));
        DecimalSeparator := tempDecSeparator;
        Add('');
        // ОТО
        for i := 0 to OTOs.Count-1 do
        begin
          s := IntToStr(i+1)+' ';
          with OTOs[i] do
            Add('  ОТО_'+s+': ОТО trace '+s+'* * ОТО'+s+'* * * '+
              RdoFloatToStr(RTimeMin)+' '+RdoFloatToStr(RTimeMax)+' '+
              RdoFloatToStr(otoX)+' '+RdoFloatToStr(otoY));
        end;
        // ТУ
        Add('');
        for i := 0 to TUs.Count-1 do
        begin
          s := IntToStr(i+1)+' ';
          with TUs[i] do
          begin
            if OTOPos is TOTO
              then s2 := 'ОТО'+IntToStr(OTOs.IndexOf(OTOPos)+1)
              else if OTOPos is TSklad then begin
                if Sklads.IndexOf(OTOPos) = 1 then s2 := 'Складвых' else s2 := '';
                if Sklads.IndexOf(OTOPos) = 0 then s2 := 'Складвх';
              end;
            Add('  ТУ_'+s+': ТУ trace '+s+s2+' * * * * '+IntToStr(PredPointNumber)+' * * '+
              RdoFloatToStr(Speed)+' '+RdoFloatToStr(TimeLoad)+' '+
              RdoFloatToStr(tuX)+' '+RdoFloatToStr(tuY)+' * *');
          end;
        end;
        // Накопители
        Add('');
          // Склад входной
        Add('  Накопитель_вх : Накопители trace Складвх 0 0 * '+RdoFloatToStr(Sklads[0].skladX)+' '+RdoFloatToStr(Sklads[0].skladY));
          // Накопители ОТО
        for i := 0 to OTOs.Count-1 do
        begin
          s  := IntToStr(i+1)+' ';
          s2 := IntToStr(OTOs[i].nakopValue)+' ';
          Add('  Накопитель_'+s+': Накопители trace ОТО'+s+s2+s2+'* '+RdoFloatToStr(OTOs[i].nakopX)+' '+RdoFloatToStr(OTOs[i].nakopY));
        end;
          // Склад выходной
        s := IntToStr(Sklads[1].skladValue)+' ';
        Add('  Накопитель_вых : Накопители trace Складвых '+s+s+'* '+RdoFloatToStr(Sklads[1].skladX)+' '+RdoFloatToStr(Sklads[1].skladY));
        // Система
        Add('');
        Add('  Систем : Система trace * * * * * * * *');
        // Типы
        Add('');
        for i := 0 to Nomenks.Count-1 do
        begin
          s := IntToStr(i+1)+' ';
          with Nomenks[i] do
            Add('  Тип_'+s+': Типы trace '+'тип'+s+
              RdoFloatToStr(AllValue)+' * * '+
              RdoFloatToStr(StartParty)+' '+RdoFloatToStr(WorkParty));
        end;
        Add('$End');
        SaveToStream(ModelStream);
      end;
      ModelStream.Free;
      // OPR
      ModelStream := TFileStream.Create(FMSModelPath+FMSModelName+'.opr', fmCreate);
      SaveResourceToStream(RC_OPR, ModelStream);
      with ModelStrings do
      begin
        Clear;
        // Оп_Приход_партии
        for i := 0 to Nomenks.Count-1 do
        begin
          s := IntToStr(i+1)+' ';
          Add('  Оп_Приход_партии_'+s+'     : Приход_партии_тип тип'+s+
            RdoZeroFloatToStr(Nomenks[i].LowValue1)+' '+
            RdoZeroFloatToStr(Nomenks[i].LowValue2));
        end;
        // Оп_Поломка_ОТО
        for i := 0 to OTOs.Count-1 do
        begin
          s := IntToStr(i+1)+' ';
          Add('  Оп_Поломка_ОТО_'+s+'       : Поломка_ОТО '+s+
            RdoZeroFloatToStr(OTOs[i].RLowValue1)+' '+
            RdoZeroFloatToStr(OTOs[i].RLowValue2));
        end;
        // Оп_Движение_тр
        for i := 1 to TUs.Count do
        begin
          s := IntToStr(i);
          Add('  Оп_Движение_тр_'+s+'        : Движение_тр '+s);
        end;
        Add('$End');
        SaveToStream(ModelStream);
      end;
      ModelStream.Free;
      // FRM
      ModelStream := TFileStream.Create(FMSModelPath+FMSModelName+'.frm', fmCreate);
      SaveResourceToStream(RC_FRM1, ModelStream);
      with ModelStrings do
      begin
        // Кадр Оборудование
          // ОТО
        Clear;
        s := IntToStr(30+15*OTOs.Count);
        Add('  rect [5  ,25,321,'+s+',transparent,< 0 0 0 >]');
        Add('  rect [6  ,26,319,'+IntToStr(28+15*OTOs.Count)+',transparent,< 0 0 0 >]');
        Add('  rect [105,25,110,'+s+',transparent,< 0 0 0 >]');
        Add('');
        for i := 0 to OTOs.Count-1 do
        begin
          s  := IntToStr(37+15*(i+1));
          s2 := IntToStr(i+1);
          Add('  text [10 ,'+s+',94 ,15,transparent,< 0 0 0 >, < '''+OTOs[i].Name+''']');
          Add('  text [110,'+s+',100,15,transparent,< 255 255 255 >, = ОТО_'+s2+'.Состояние]');
          Add('  text [220,'+s+',100,15,transparent,< 255 255 255 >, = ОТО_'+s2+'.Номер_дет]');
          Add('');
        end;
        SaveToStream(ModelStream);
          // ТУ
        Clear;
        s  := IntToStr(62+15*OTOs.Count);
        s2 := IntToStr(30+15*TUs.Count);
        Add('  rect [5  ,'+s+',430,'+s2+',transparent,< 0 0 0 >]');
        Add('  rect [6  ,'+IntToStr(63+15*OTOs.Count)+',428,'+IntToStr(28+15*TUs.Count)+',transparent,< 0 0 0 >]');
        Add('  rect [105,'+s+',110,'+s2+',transparent,< 0 0 0 >]');
        Add('  rect [325,'+s+',110,'+s2+',transparent,< 0 0 0 >]');
        s := IntToStr(87+15*OTOs.Count);
        Add('  line [5  ,'+s+',435,'+s+',< 0 0 0 >]');
        s := IntToStr(66+15*OTOs.Count);
        Add('  text [10 ,'+s+',100,20,transparent,< 0 0 0 >, < ''Транспорт'']');
        Add('  text [110,'+s+',100,20,transparent,< 0 0 0 >, = ''Состояние'']');
        Add('  text [220,'+s+',100,20,transparent,< 0 0 0 >, = ''Позиция'']');
        Add('  text [330,'+s+',100,20,transparent,< 0 0 0 >, = ''Куда перейти'']');
        Add('');
        j := 74+15*OTOs.Count;
        for i := 0 to TUs.Count-1 do
        begin
          s  := IntToStr(j+15*(i+1));
          s2 := IntToStr(i+1);
          Add('  text [10 ,'+s+',94 ,15,transparent,< 0 0 0 >, < '''+TUs[i].Name+''']');
          Add('  text [110,'+s+',100,15,transparent,< 255 255 255 >, = ТУ_'+s2+'.Состояние]');
          Add('  text [220,'+s+',100,15,transparent,< 255 255 255 >, = ТУ_'+s2+'.Позиция]');
          Add('  text [330,'+s+',100,15,transparent,< 255 255 255 >, = ТУ_'+s2+'.Куда]');
          Add('');
        end;
        SaveToStream(ModelStream);
          // Накопители
        Clear;
        j := 15*OTOs.Count+15*TUs.Count;
        s  := IntToStr(99+j);
        s2 := IntToStr(30+15*OTOs.Count);
        Add('  rect [5  ,'+s+',321,'+s2+',transparent,< 0 0 0 >]');
        Add('  rect [6  ,'+IntToStr(100+j)+',319,'+IntToStr(28+15*OTOs.Count)+',transparent,< 0 0 0 >]');
        Add('  rect [105,'+s+',110,'+s2+',transparent,< 0 0 0 >]');
        s := IntToStr(124+j);
        Add('  line [5  ,'+s+',325,'+s+',< 0 0 0 >]');
        s := IntToStr(102+j);
        Add('  text [10 ,'+s+',100,20,transparent,< 0 0 0 >, < ''Накопители'']');
        Add('  text [110,'+s+',100,20,transparent,< 0 0 0 >, = ''Свободно'']');
        Add('  text [220,'+s+',100,20,transparent,< 0 0 0 >, = ''Емкость'']');
        Add('');
        inc(j, 111);
        for i := 0 to OTOs.Count-1 do
        begin
          s  := IntToStr(j+15*(i+1));
          s2 := IntToStr(i+1);
          Add('  text [10 ,'+s+',94 ,15,transparent,< 0 0 0 >, < ''Накопитель '+s2+''']');
          Add('  text [110,'+s+',100,15,transparent,< 255 255 255 >, = Накопитель_'+s2+'.Свободно]');
          Add('  text [220,'+s+',100,15,transparent,< 255 255 255 >, = Накопитель_'+s2+'.Емкость]');
          Add('');
        end;
        SaveToStream(ModelStream);
        SaveResourceToStream(RC_FRM2, ModelStream);
        // Кадр Окно_Анимация
          // Транспортный путь
        Clear;
        for i := 0 to FMSPoint.Count-2 do
          for j := i+1 to FMSPoint.Count-1 do
            if FMSPoint.PointRel[i, j] > 0 then
              Add('  line [Координаты_узлов(Х,'+IntToStr(i+1)+')'+addScaleX+
                          'Координаты_узлов(У,'+IntToStr(i+1)+')'+addScaleY+
                          'Координаты_узлов(Х,'+IntToStr(j+1)+')'+addScaleX+
                          'Координаты_узлов(У,'+IntToStr(j+1)+')'+addScaleY+'<0 0 0>]');
        Add('');
        SaveToStream(ModelStream);
          // ТУ
        Clear;
        for i := 0 to TUs.Count-1 do
        begin
          s  := IntToStr(i+1);
          s2 := ' ТУ_'+s+'.Состояние = ';
          with TUs[i] do
          begin
            sDX2 := RdoFloatToStr(tuDX/2);
            sDY2 := RdoFloatToStr(tuDY/2);
            if not PictureExist then s3 := GetStringScale(tuDX, tuDY)
            else begin
              Add('Show s_bmp[(ТУ_'+s+'.Х_коор - '+sDX2+')'+addScaleX+'(ТУ_'+s+'.У_коор - '+sDY2+')'+addScaleY+GetStringScale(tuDX, tuDY)+'tu'+s+'p, tu'+s+'m]');
              SavePicture;
              s3 := GetStringScale(0.5, 0.5);
            end;
            Add('Show_if'+s2+'Движение_св or'+s2+'Загружается or'+s2+'Свободен or'+s2+'Проверка_узла or'+s2+'Узел_стоп');
            Add('  rect [(ТУ_'+s+'.Х_коор - '+sDX2+')'+addScaleX+'(ТУ_'+s+'.У_коор - '+sDY2+')'+addScaleY+s3+'<0 128 0>,<255 255 0>]');
            Add('Show_if'+s2+'Движение_заг or'+s2+'Разгружается or'+s2+'Загружен or'+s2+'Проверка_узла1 or'+s2+'Узел_стоп1');
            Add('  rect [(ТУ_'+s+'.Х_коор - '+sDX2+')'+addScaleX+'(ТУ_'+s+'.У_коор - '+sDY2+')'+addScaleY+s3+'<0 0 240>,<255 255 0>]');
            if ShowText then Add('Show text [(ТУ_'+s+'.Х_коор - '+sDX2+')'+addScaleX+'(ТУ_'+s+'.У_коор - '+sDY2+')'+addScaleY+GetStringScale(tuDX, tuDY)+sTextColor+'='''+s+''']');
          end;
          Add('');
        end;
        SaveToStream(ModelStream);
          // Накопители
        Clear;
        Add('Show');
        with Sklads[0] do
        begin
          if not PictureExist then
          begin
            Add('  rect [(Накопитель_вх.Х_коор)'+addScaleX+'(Накопитель_вх.У_коор)'+addScaleY+GetStringScale(skladDX, skladDY)+'<0 128 0>,<255 255 0>]');
          end else begin
            Add('  s_bmp[(Накопитель_вх.Х_коор)'+addScaleX+'(Накопитель_вх.У_коор)'+addScaleY+GetStringScale(skladDX, skladDY)+'sklad1p, sklad1m]');
            SavePicture;
          end;
          if ShowText then Add('  text [(Накопитель_вх.Х_коор)'+addScaleX+'(Накопитель_вх.У_коор)'+addScaleY+GetStringScale(skladDX, skladDY)+sTextColor+'=''вх'']');
        end;
        for i := 0 to OTOs.Count-1 do
        begin
          s  := 'Накопитель_'+IntToStr(i+1);
          if not OTOs[i].PictureExist then s2 := '1'
                                      else s2 := '0.3';
          Add('  rect [('+s+'.Х_коор)'+addScaleX+'('+s+'.У_коор)'+addScaleY+'2'+addScale+s2+addScale+'<0 0 240>,<255 255 0>]');
          Add('  rect [('+s+'.Х_коор)'+addScaleX+'('+s+'.У_коор)'+addScaleY+'2 * Const.Scale * '+s+'.Свободно / '+s+'.Емкость, '+s2+addScale+'<0 128 0>,<255 255 0>]');
        end;
        with Sklads[1] do
        begin
          if not PictureExist then
          begin
            Add('  rect [(Накопитель_вых.Х_коор)'+addScaleX+'(Накопитель_вых.У_коор)'+addScaleY+GetStringScale(skladDX, skladDY)+'<0 0 240>,<255 255 0>]');
            Add('  rect [(Накопитель_вых.Х_коор)'+addScaleX+'(Накопитель_вых.У_коор)'+addScaleY+RdoFloatToStr(skladDX)+addScale+RdoFloatToStr(skladDY)+' * Const.Scale * Накопитель_вых.Свободно / Накопитель_вых.Емкость, <0 128 0>,<255 255 0>]');
          end else begin
            Add('  s_bmp[(Накопитель_вых.Х_коор)'+addScaleX+'(Накопитель_вых.У_коор)'+addScaleY+GetStringScale(skladDX, skladDY)+'sklad2p, sklad2m]');
            Add('  rect [(Накопитель_вых.Х_коор)'+addScaleX+'(Накопитель_вых.У_коор)'+addScaleY+GetStringScale(2, 0.3)+'<0 0 240>,<255 255 0>]');
            Add('  rect [(Накопитель_вых.Х_коор)'+addScaleX+'(Накопитель_вых.У_коор)'+addScaleY+'2 * Const.Scale * Накопитель_вых.Свободно / Накопитель_вых.Емкость, 0.3'+addScale+'<0 128 0>,<255 255 0>]');
            SavePicture;
          end;
          if ShowText then Add('  text [(Накопитель_вых.Х_коор)'+addScaleX+'(Накопитель_вых.У_коор)'+addScaleY+GetStringScale(skladDX, skladDY)+sTextColor+'=''вых'']');
        end;
        Add('');
        SaveToStream(ModelStream);
          // ОТО
        Clear;
        for i := 0 to OTOs.Count-1 do
        begin
          s  := 'ОТО_'+IntToStr(i+1)+'.';
          s2 := 'Show_if '+s+'Состояние = ';
          with OTOs[i] do
          begin
            if not PictureExist then s3 := GetStringScale(otoDX, otoDY)
            else begin
              s3 := 'oto'+IntToStr(i+1);
              Add('Show s_bmp[('+s+'Х_коор)'+addScaleX+'('+s+'У_коор)'+addScaleY+GetStringScale(otoDX, otoDY)+s3+'p, '+s3+'m]');
              SavePicture;
              s3 := GetStringScale(0.5, 0.5);
            end;
            Add(s2+'Свободно');
            Add('  rect [('+s+'Х_коор)'+addScaleX+'('+s+'У_коор)'+addScaleY+s3+'<0 128 0>,<255 255 0>]');
            Add(s2+'Работа');
            Add('  rect [('+s+'Х_коор)'+addScaleX+'('+s+'У_коор)'+addScaleY+s3+'<0 0 240>,<255 255 0>]');
            Add(s2+'Ремонт');
            Add('  rect [('+s+'Х_коор)'+addScaleX+'('+s+'У_коор)'+addScaleY+s3+'<255 255 0>,<255 255 0>]');
            if ShowText then
            begin
              Add('Show');
              Add('  text [('+s+'Х_коор)'+addScaleX+'('+s+'У_коор)'+addScaleY+GetStringScale(otoDX, otoDY)+sTextColor+''''+Name+''']');
              Add('  text [('+s+'Х_коор)'+addScaleX+'('+s+'У_коор)'+addScaleY+GetStringScale(otoDX, 1)+sTextColor+'>'''+IntToStr(ItemID)+''']');
            end;
            Add('');
          end;
        end;
        Add('Show bitmap[3, 10, Controlp, Controlm]');
        Add('');
        Add('$End');
        SaveBMPFromResource('Controlp');
        SaveBMPFromResource('Controlm');
        SaveToStream(ModelStream);
      end;
      ModelStream.Free;
      // FUN
      ModelStream := TFileStream.Create(FMSModelPath+FMSModelName+'.fun', fmCreate);
      SaveResourceToStream(RC_FUN1, ModelStream);
      with ModelStrings do
      begin
        // Константы
        Clear;
        j := 0;
        for i := 0 to Nomenks.Count-1 do
          inc(j, Nomenks[i].AllValue);
        Add(IntToStr(j));
        Add('  Количество_узлов  : integer = ' + IntToStr(FMSPoint.Count));
        SaveToStream(ModelStream);
        SaveResourceToStream(RC_FUN2, ModelStream);
        // Генератор_заказов
        Clear;
        for i := 0 to Nomenks.Count-1 do
          Add('  Calculate_if _тип = тип'+IntToStr(i+1)+' Генератор_заказов = '+ModelLow[Nomenks[i].Low]);
        SaveToStream(ModelStream);
        SaveResourceToStream(RC_FUN3, ModelStream);
        // Генератор_аварий
        Clear;
        for i := 0 to OTOs.Count-1 do
          Add('  Calculate_if Номер = '+IntToStr(i+1)+' Генератор_аварий = '+ModelLow[OTOs[i].RLow]);
        SaveToStream(ModelStream);
        SaveResourceToStream(RC_FUN4, ModelStream);
        // Нак_Узлы
        Clear;
        s := '  '+IntToStr(Sklads[0].skladPointNumber)+' ';
        for i := 0 to OTOs.Count-1 do
          s := s + IntToStr(OTOs[i].nakopPointNumber)+' ';
        Add(s + IntToStr(Sklads[1].skladPointNumber));
        SaveToStream(ModelStream);
        SaveResourceToStream(RC_FUN5, ModelStream);
        // Тех_путь
        Clear;
        for i := 0 to Nomenks.Count-1 do
        begin
          s := '  ';
          for j := 0 to Nomenks[i].TPCount-1 do
            s := s+'ОТО'+IntToStr(OTOs.IndexOf(Nomenks[i].TP[j].OTO)+1)+' ';
          s := s + 'Складвых';
          j := Nomenks[i].TPCount;
          while j < MaxOperationNumber do
          begin
            inc(j);
            s := s+' Складвх';
          end;
          Add(s);
        end;
        SaveToStream(ModelStream);
        SaveResourceToStream(RC_FUN6, ModelStream);
        // Времена
        Clear;
        Add(IntToStr(MaxOperationNumber)+']');
        SaveToStream(ModelStream);
        SaveResourceToStream(RC_FUN7, ModelStream);
        Clear;
        for i := 0 to Nomenks.Count-1 do
        begin
          s := ' ';
          for j := 0 to Nomenks[i].TPCount-1 do
            s := s+' '+RdoFloatToStr(Nomenks[i].TP[j].Time);
          j := Nomenks[i].TPCount;
          while j < MaxOperationNumber do
          begin
            inc(j);
            s := s+' 0';
          end;
          Add(s);
        end;
        SaveToStream(ModelStream);
        SaveResourceToStream(RC_FUN8, ModelStream);
        // Расстояние
        Clear;
        for i := 0 to FMSPoint.Count-1 do
        begin
          s := ' ';
          for j := 0 to FMSPoint.Count-1 do
            s := s+' '+RdoFloatToStr(FMSPoint.PointRel[i, j]);
          Add(s);
        end;
        SaveToStream(ModelStream);
        SaveResourceToStream(RC_FUN9, ModelStream);
        // Координаты_узлов
        Clear;
        for i := 0 to FMSPoint.Count-1 do
        begin
          s := '  '+RdoFloatToStr(FMSPoint.PointCoor[i, 0])+' '+RdoFloatToStr(FMSPoint.PointCoor[i, 1]);
          Add(s);
        end;
        SaveToStream(ModelStream);
      end;
      SaveResourceToStream(RC_FUN10, ModelStream);
      ModelStream.Free;
      // PMD
      ModelStream := TFileStream.Create(FMSModelPath+FMSModelName+'.pmd', fmCreate);
      with ModelStrings do
      begin
        Clear;
        Add('$Results');
        // ОТО
        for i := 0 to OTOs.Count-1 do
        begin
          s  := ' ОТО_'+IntToStr(i+1);
          s2 := ' Накопитель_'+IntToStr(i+1);
          with OTOs[i] do
          begin
            if pOtoBusy   then Add(' '+s+'_Загрузка         : watch_state'+s+'.Состояние = Работа');
            if pOtoWait   then Add(' '+s+'_Простой          : watch_state'+s+'.Состояние = Свободно');
            if pOtoRepair then Add(' '+s+'_Ремонт           : watch_state'+s+'.Состояние = Ремонт');
            if pNakopBusy then Add(' '+s2+'_Загрузка  : watch_par  '+s2+'.Свободно');
            if pNakopWait then Add(' '+s2+'_Простой   : watch_state'+s2+'.Свободно ='+s2+'.Емкость');
          end;
        end;
        // ТУ
        for i := 0 to TUs.Count-1 do
        begin
          s := ' ТУ_'+IntToStr(i+1);
          with TUs[i] do
          begin
            if pTuTransport then Add(' '+s+'_Транспортировка   : watch_state'+s+'.Состояние = Движение_заг');
            if pTuFree      then Add(' '+s+'_Свободен          : watch_state'+s+'.Состояние = Свободен');
            if pTuEmpty     then Add(' '+s+'_Хол_ход           : watch_state'+s+'.Состояние = Движение_св');
            if pTuLoad      then Add(' '+s+'_Погрузка          : watch_state'+s+'.Состояние = Загружается or'+s+'.Состояние = Разгружается');
          end;
        end;
        // Склады входной и выходной
        for i := 0 to 1 do
          with Sklads[i] do
          begin
            if pNakopBusy then Add(' '+SkladName[i]+'_Загрузка : watch_par  '+SkladName[i]+'.Свободно');
            if pNakopWait then Add(' '+SkladName[i]+'_Простой  : watch_state'+SkladName[i]+'.Свободно ='+SkladName[i]+'.Емкость');
          end;
        SaveToStream(ModelStream);
      end;
      SaveResourceToStream(RC_PMD, ModelStream);
      ModelStream.Free;
      // SMR
      ModelStream := TFileStream.Create(FMSModelPath+FMSModelName+'.smr', fmCreate);
      SaveResourceToStream(RC_SMR, ModelStream);
      with ModelStrings do
      begin
        Clear;
        if FMSConst.doWorkTime then Add('Terminate_if  [Time_now / (Масштаб_времени * 60) >= '+RdoZeroFloatToStr(FMSConst.WorkTime)+'] or'+MyEOL+'              [Систем.Обраб_деталей = Объем_выпуска]')
                               else Add('Terminate_if  Систем.Обраб_деталей = Объем_выпуска');
        SaveToStream(ModelStream);
      end;
      ModelStream.Free;
    finally
      ModelStrings.Free;
    end;
  except
    on E: FMSError do raise FMSError.CreateStopSubError(ErrorBuildModel, E.Message);
    else raise FMSError.CreateStopRes(ErrorBuildModel);
  end;
end;

procedure TFMSForm.RunButtonClick(Sender: TObject);
var
  si : TStartupInfo;
  pi : TProcessInformation;
  str: string;
begin
  BuildModel;
  SetCurrentDir(ExtractFileDir(FMSModelPath));
  str := RdoImitatorPathName+' '+FMSModelName+'.smr';
  ZeroMemory (@si, SizeOf(TStartupInfo));
  si.cb          := SizeOf(TStartupInfo);
  si.wShowWindow := SW_SHOWMAXIMIZED;
  si.dwFlags     := STARTF_USESHOWWINDOW;
  if not CreateProcess(nil, PChar(str), nil, nil, true,
                   NORMAL_PRIORITY_CLASS, nil, nil, si, pi) then
    raise FMSError.CreateRes(ErrorRunRdo);
end;

procedure TFMSForm.About1Click(Sender: TObject);
begin
  try
    AboutForm := TAboutForm.Create(Self);
    AboutForm.ShowModal;
  finally
    AboutForm.Free;
  end;
end;

procedure TFMSForm.Exit1Click(Sender: TObject);
begin
  Close;
end;

procedure TFMSForm.FormCloseQuery(Sender: TObject; var CanClose: Boolean);
begin
  CanClose := SaveQueryAndSave;
end;

procedure TFMSForm.ResultButtonClick(Sender: TObject);
var
  ShowModelResult: TShowModelResult;
  ResultDLL: THandle;
begin
  ResultDLL := LoadLibrary('FMSresult.dll');
  if ResultDLL < 32 then raise FMSError.CreateRes(ErrorLoadResultDLL);
  @ShowModelResult := GetProcAddress(ResultDLL, 'ShowModellingResults');
  ShowModelResult(PlaneFileName);
  if ResultDll >= 32 then FreeLibrary(ResultDll);
end;

procedure TFMSForm.Contents1Click(Sender: TObject);
begin
  Application.HelpCommand(HELP_FINDER, 0)
end;

procedure TFMSForm.SearchforHelpOn1Click(Sender: TObject);
const
  EmptyString: PChar = '';
begin
  Application.HelpCommand(HELP_PARTIALKEY, Longint(EmptyString));
end;

procedure TFMSForm.MapSpeedButtonClick(Sender: TObject);
begin
  ChangeFMSObject;
  try
    MapForm := TMapForm.Create(Self);
    MapForm.ShowModal;
  finally
    with MapForm.MapScrollBox, FMSConst do
    begin
      dX := -HorzScrollBar.Position;
      dY := -VertScrollBar.Position;
      if Scale <> MapForm.Scale then
      begin
        Scale := MapForm.Scale;
        PlaneChange := true;
      end;
    end;
    MapForm.Free;
  end;
  UpDateFMSObject;
end;

procedure TFMSForm.UpDateFMSObject;
begin
  if TreeView.Selected <> nil then ShowFMSObject(TreeView.Selected);
end;

procedure TFMSForm.PicImageZoomButtonClick(Sender: TObject);
var delta: integer;
begin
  try
    ViewPictureForm := TViewPictureForm.Create(Self);
    with ViewPictureForm do
    begin
      ViewImage.Picture := PicImage.Picture;
      delta := 0;
      with ViewPanel do
      begin
        if BevelInner <> bvNone then delta := BevelWidth * 2;
        if BevelOuter <> bvNone then inc(delta, BevelWidth * 2);
      end;
      ClientWidth  := PicImage.Width+delta;
      ClientHeight := PicImage.Height+delta;
      ShowModal;
    end;
  finally
    ViewPictureForm.Free;
  end;
end;

procedure TFMSForm.RLowChange(Sender: TObject);
begin
  ChangeFMSObject;
end;

procedure TFMSForm.OTOPBusyClick(Sender: TObject);
begin
  if FCheckBoxChange then ChangeFMSObject;
end;

procedure TFMSForm.AllProgrammRadioButtonClick(Sender: TObject);
begin
  ChangeFMSObject;
end;

procedure TFMSForm.TreeViewChanging(Sender: TObject; Node: TTreeNode;
  var AllowChange: Boolean);
begin
  try
    FAllowChange := true;
    ChangeFMSObject;
  except
    on E: Exception do begin
      FAllowChange := false;
      AllowChange  := false;
      ShowErrorMessage(E.Message);
    end;
  end;
end;

procedure TFMSForm.ChangeFMSObject;
var
  ObjName: string;
  tempWorkTime: double;
  flag: boolean;

  procedure CheckLow(var mlLow: TModelLow; var Value1, Value2: double);
  var
    tempValue1, tempValue2: double;
    LowComboBox: TComboBox;
    Value1Edit, Value2Edit: TEdit;
  begin
    if ActiveFMSObject is TOTO then
    begin
      LowComboBox := RLow;
      Value1Edit  := RLowValue1;
      Value2Edit  := RLowValue2;
    end else begin
      LowComboBox := NomenkLow;
      Value1Edit  := NomenkLowValue1;
      Value2Edit  := NomenkLowValue2;
    end;
    PlaneChange := PlaneChange or (mlLow <> IntToLow[LowComboBox.ItemIndex]);
    mlLow := IntToLow[LowComboBox.ItemIndex];
    try
      tempValue1 := StrToFloat(Value1Edit.Text);
    except raise FMSError.CreateResFmt(ErrorInputDoubleValue, [Value1Edit.Text]); end;
    try
      tempValue2 := StrToFloat(Value2Edit.Text);
    except raise FMSError.CreateResFmt(ErrorInputDoubleValue, [Value2Edit.Text]); end;
    if tempValue1 <= 0 then raise FMSError.CreateRes(Error_mlLowTimeMinus);
    case mlLow of
      mlUniform : if tempValue2 <= tempValue1 then raise FMSError.CreateRes(Error_mlUniform);
      mlNormal  : if sqrt(tempValue2)*3 >= tempValue1 then raise FMSError.CreateRes(Error_mlNormal);
    end;
    PlaneChange := PlaneChange or (Value1 <> tempValue1);
    PlaneChange := PlaneChange or (Value2 <> tempValue2);
    Value1 := tempValue1;
    Value2 := tempValue2;
  end;

  procedure bCheckPlaneChange(var Value: boolean; Src: TCheckBox);
  begin
    PlaneChange := PlaneChange or (Value <> Src.Checked);
    Value := Src.Checked;
  end;

begin
  try
    // Проверить ActiveFMSObject
    if (ActiveFMSObject <> nil) and (not ActiveFMSObject.doFree) then
      try
        // ОТО
        ObjName := ActiveFMSObject.Name;
        if ActiveFMSObject is TOTO then
          with ActiveFMSObject as TOTO do
          begin
            bCheckPlaneChange(pOtoBusy, OTOPBusy);
            bCheckPlaneChange(pOtoWait, OTOPWait);
            bCheckPlaneChange(pOtoRepair, OTOPRepair);
            bCheckPlaneChange(pNakopBusy, NakopPBusy);
            bCheckPlaneChange(pNakopWait, NakopPWait);
            CheckLow(RLow, RLowValue1, RLowValue2);
          end else if ActiveFMSObject is TTU then
            // ТУ
            with ActiveFMSObject as TTU do
            begin
              bCheckPlaneChange(pTuTransport, TUPTransport);
              bCheckPlaneChange(pTuFree, TUPFree);
              bCheckPlaneChange(pTuEmpty, TUPEmpty);
              bCheckPlaneChange(pTuLoad, TUPLoad);
            end else if ActiveFMSObject is TSklad then
              // Склад
              with ActiveFMSObject as TSklad do
              begin
                bCheckPlaneChange(pNakopBusy, SkladPBusy);
                bCheckPlaneChange(pNakopWait, SkladPWait);
              end else if ActiveFMSObject is TNomenk then
                // Номенклатура
                with ActiveFMSObject as TNomenk do
                begin
                  CheckLow(Low, LowValue1, LowValue2);
                end;
      finally
        UpDateFMSObject;
      end;
    // Проверить Константы
    if TreeView.Selected = ConstTreeNode then
      try
        ObjName := 'Константы';
        flag := FMSConst.doWorkTime and WorkTimeRadioButton.Checked;
        try
          tempWorkTime := StrToFloat(WorkTime.Text);
        except raise FMSError.CreateResFmt(ErrorInputDoubleValue, [WorkTime.Text]); end;
        if flag then begin if tempWorkTime <= 0 then raise FMSError.CreateRes(ErrorWorkTimeMinus); end
                else begin if tempWorkTime <  0 then raise FMSError.CreateRes(ErrorWorkTimeMinus); end;
        PlaneChange  := PlaneChange or (FMSConst.WorkTime <> tempWorkTime);
        FMSConst.WorkTime := tempWorkTime;
        PlaneChange := PlaneChange or (FMSConst.doWorkTime <> WorkTimeRadioButton.Checked);
        FMSConst.doWorkTime := WorkTimeRadioButton.Checked;
      finally
        UpDateFMSObject;
      end;
  except
    on E: FMSError do raise FMSError.CreateFmt(LoadStr(ErrorChangeFMSObject)+':%s%s', [ObjName, MyEOL, E.Message]);
    else raise FMSError.CreateResFmt(ErrorChangeFMSObject, [ObjName]);
  end;
end;

procedure TFMSForm.HelpSpeedButtonClick(Sender: TObject);
begin
  ShowHelpContext := not ShowHelpContext;
end;

procedure TFMSForm.WhatIsMenuClick(Sender: TObject);
begin
  with HelpContextPopupInfo do
  begin
    if Sender is TMenuItem then
    begin
      if ((Sender as TMenuItem).Name = 'WhatIsMenu') and (PopupHelpContext <> 0) then
      begin
        Application.HelpCommand(HELP_SETPOPUP_POS, Longint(sPos));
        Application.HelpCommand(HELP_CONTEXTPOPUP, PopupHelpContext);
      end else
        if ((Sender as TMenuItem).Name = 'DetailHelpMenu') and (DetailHelpContext <> 0) then
          WinHelp(Application.Handle, PChar(Application.HelpFile + '>info'), HELP_CONTEXT, DetailHelpContext);
    end;
    PopupHelpContext  := 0;
    DetailHelpContext := 0;
    sPos.x            := 0;
    sPos.y            := 0;
  end;
end;

end.

