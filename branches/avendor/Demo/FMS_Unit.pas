unit FMS_Unit;

interface

uses
  Windows, Messages, SysUtils, Classes, Graphics, Controls, Forms, Dialogs,
  ExtCtrls, ComCtrls, ToolWin, Menus, Buttons, Grids, StdCtrls, IniFiles,
  SplshWnd, FileUtil;

type

  TDemoState = (dsNone, dsRecord, dsRecPause, dsPlayback, dsRecordError, dsPlaybackError);

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
    Splitter: TSplitter;
    DemoButton: TSpeedButton;
    ToolButton4: TToolButton;
    N7: TMenuItem;
    DemoMenu: TMenuItem;
    procedure FormCreate(Sender: TObject);
    procedure TreeViewMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
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
    procedure DemoMenuClick(Sender: TObject);
    procedure DemoButtonMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
  private
    IniFile: TIniFile;
    RdoImitatorPathName, FPlaneFileName: string;
    FPlaneChange, FAllowChange, FPlaneLoaded, FCheckBoxChange, FShowHelpContext: boolean;
    ListSeparator: char;
    FDemoState: TDemoState;
    function  SaveAsFun: boolean;
    function  SaveQueryAndSave: boolean;
    function  SaveMenu: boolean;
    function  SaveReal: boolean;
    procedure FMSException(Sender: TObject; E: Exception);
    procedure CreateAllDemoObject;
    procedure ResetAllDemoObject;
    procedure LoadPlaneFile(Value: string);
    procedure ShowFMSObject(Value: TTreeNode);
    procedure BuildModel;
    procedure SetPlaneFileName(Value: string);
    procedure SetPlaneChange(Value: boolean);
    procedure SetPlaneLoaded(Value: boolean);
    procedure SetShowHelpContext(Value: boolean);
    procedure SetFMSModelPath(Value: string);
    procedure WndProc(var Message: TMessage); override;
    procedure AppMess(var Msg: TMsg; var Handled: boolean);
    procedure ChangeFMSObject;
    procedure SetDemoState(Value: TDemoState);
    property PlaneLoaded: boolean read FPlaneLoaded write SetPlaneLoaded;
    property ShowHelpContext: boolean read FShowHelpContext write SetShowHelpContext;
    property DemoState: TDemoState read FDemoState write SetDemoState;
  public
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
{$R FmsDemka.res}

// --------------------
// Demo
// --------------------
var
  PlayHookHandle: HHOOK;
  MsgList: TList;
  CurrentMsg: integer;
  bSleep: boolean;
  bFast : boolean = false;
  bLoop : boolean = true;

procedure ClearDemo;
var i: integer;
begin
  for i := 0 to MsgList.Count-1 do dispose(MsgList[i]);
  MsgList.Clear;
  FMSForm.DemoState := dsNone;
end;

procedure LoadDemo;
var
  rStream  : TResourceStream;
  lReader  : TReader;
  i, j     : integer;
  vEventMsg: PEventMsg;
begin
  ClearDemo;
  rStream := TResourceStream.CreateFromID(0, RC_Demka, RT_RCDATA);
  lReader := TReader.Create(rStream, 1024);
  try
    j := lReader.ReadInteger;
    for i := 0 to j-1 do
    begin
      new(vEventMsg);
      with vEventMsg^ do
      begin
        message := lReader.ReadInteger;
        paramL  := lReader.ReadInteger;
        paramH  := lReader.ReadInteger;
        time    := lReader.ReadInteger;
        hwnd    := lReader.ReadInteger;
        MsgList.Add(vEventMsg);
      end;
    end;
  finally
    lReader.Free;
    rStream.Free;
  end;
end;

procedure ResetDemo;
begin
  CurrentMsg := 0;
  bSleep     := true;
  FMSForm.ResetAllDemoObject;
  FMSForm.TreeView.FullCollapse;
end;

// ---------------------
// Play
// ---------------------
procedure UnHookPlayback;
begin
  if UnhookWindowsHookEx(PlayHookHandle) then FMSForm.DemoState := dsNone
                                         else FMSForm.DemoState := dsPlaybackError;
end;

function JournalPlaybackProc(Code: integer; wParam: WPARAM; lParam: LPARAM): LRESULT; stdcall;
begin
  if CurrentMsg >= MsgList.Count then
  begin
    if bLoop then
    begin
      ResetDemo;
      Result     := 0;
    end else begin
      Result := 0;
      UnHookPlayback;
    end;
    exit;
  end else case Code of
    HC_GETNEXT: begin
                  if FMSForm.DemoState = dsPlayback then
                  begin
                    with PEventMsg(MsgList[CurrentMsg])^ do
                    begin
                      PEventMsg(lParam).message := message;
                      PEventMsg(lParam).paramL  := paramL;
                      PEventMsg(lParam).paramH  := paramH;
                      PEventMsg(lParam).time    := time;
                      PEventMsg(lParam).hwnd    := hwnd;
                      if bSleep then begin if bFast then Result := 1 else Result := time; end
                                else begin Result := 0; inc(CurrentMsg); end;
                    end;
                    bSleep := not bSleep;
                  end else Result := 0;
                end;
    HC_SKIP:    begin
                  Result := 0;
                end;
    HC_SYSMODALOFF, HC_SYSMODALON, HC_NOREMOVE:
                begin
                  Result := 0;
                  UnHookPlayback;
                  FMSForm.DemoState := dsPlaybackError;
                end;
    else begin
           Result := CallNextHookEx(PlayHookHandle, Code, wParam, lParam);
         end;
  end;
end;
// ---------------------
// Play END
// ---------------------

procedure TFMSForm.SetDemoState(Value: TDemoState);
begin
  FDemoState := Value;
  DemoButton.Enabled := (FDemoState = dsNone) or (FDemoState = dsRecordError) or (FDemoState = dsPlaybackError);
  DemoMenu.Enabled   := DemoButton.Enabled;
  if FDemoState = dsPlaybackError then ShowErrorMessage(LoadStr(ErrorPlaybackError));
end;

procedure TFMSForm.DemoMenuClick(Sender: TObject);
begin
  DemoButtonMouseUp(nil, mbLeft, [], 0, 0);
end;

procedure TFMSForm.DemoButtonMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var flag: boolean;
begin
  // Проверить настройки системы
  flag := true;
  if (GetSystemMetrics(SM_CXSCREEN) <> 800) or (GetSystemMetrics(SM_CYSCREEN) <> 600) then flag := ShowErrorMessage(LoadStr(ErrorRunDemo));
  if flag then
  begin
    WindowState := wsMaximized;
    if (Left <> -4) or (Top <> -4) or (ClientWidth <> 800) or (ClientHeight <> 534) then flag := ShowErrorMessage(LoadStr(ErrorRunDemo));
    // ----------
    if flag then
    begin
      ResetDemo;
      DemoState  := dsPlayback;
      PlayHookHandle    := SetWindowsHookEx(WH_JOURNALPLAYBACK, JournalPlaybackProc, 0, 0);
      if PlayHookHandle = HHOOK(nil) then UnHookPlayback;
    end;
  end;
end;
// --------------------
// Demo END
// --------------------

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
    WM_CANCELJOURNAL: if DemoState = dsPlayback then DemoState := dsNone;
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

procedure TFMSForm.SetPlaneFileName(Value: string);
begin
  if FPlaneFileName <> Value then
  begin
    FPlaneFileName := Value;
    Caption := LoadStr(MainCaption);
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

procedure TFMSForm.CreateAllDemoObject;
const
  arrayCoor: array[0..11, 0..1] of double = ((13.85, 5),
                                             (13.85, 7.8),
                                             (13.85, 11.8),
                                             (7    , 11.8),
                                             (13.85, 15.8),
                                             (13.85, 20.8),
                                             (22.85, 18.4),
                                             (22.85, 15.8),
                                             (30   , 11.8),
                                             (22.85, 11.8),
                                             (22.85, 7.8),
                                             (22.85, 5));

  arrayRel: array[0..11, 0..11] of double = ((0,1,0,0,0,0,0,0,0,0,0,0),
                                             (1,0,1,0,0,0,0,0,0,0,1,0),
                                             (0,1,0,1,1,0,0,0,0,0,0,0),
                                             (0,0,1,0,0,0,0,0,0,0,0,0),
                                             (0,0,1,0,0,1,0,1,0,0,0,0),
                                             (0,0,0,0,1,0,0,0,0,0,0,0),
                                             (0,0,0,0,0,0,0,1,0,0,0,0),
                                             (0,0,0,0,1,0,1,0,0,1,0,0),
                                             (0,0,0,0,0,0,0,0,0,1,0,0),
                                             (0,0,0,0,0,0,0,1,1,0,1,0),
                                             (0,1,0,0,0,0,0,0,0,1,0,1),
                                             (0,0,0,0,0,0,0,0,0,0,1,0));
var
  SplashForm: TSplashWindow;
  i, j: integer;
  s: string;
  tempSklad: TSklad;
  tempOTO: TOTO;
  tempTU: TTU;
  tempNomenk: TNomenk;
  tempTP: ^TTP;
begin
  try
    SplashForm := ShowSplashWindow(Application.Icon, 'Обработка компоновки. Подождите пожалуйста...', false, nil);
    try
      OTOs.Clear;
      TUs.Clear;
      Sklads.Clear;
      Nomenks.Clear;
      // Константы
      with FMSConst do
      begin
        Scale      := 17.3;
        dX         := 0;
        dY         := 0;
        minX       := 0;
        minY       := 0;
        maxX       := 0;
        maxY       := 0;
        WorkTime   := 0;
        doWorkTime := false;
      end;
      // Узлы
      FMSPoint.Count := 12;
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
      // Создание складов (входного и выходного)
      tempSklad := TSklad.Create(Sklads, 'Склад входной');
      with tempSklad do
      begin
        ReadPicture('sklad');
        skladX     := 2.6;
        skladY     := 13;
        skladDX    := 10;
        skladDY    := 2.3;
        skladValue := -1;
        skladPointNumber := 4;
        pNakopBusy := true;
        pNakopWait := true;
      end;
      tempSklad := TSklad.Create(Sklads, 'Склад выходной');
      with tempSklad do
      begin
        ReadPicture('sklad');
        skladX     := 30.9;
        skladY     := 11.3;
        skladDX    := 10;
        skladDY    := 2.3;
        skladValue := 20;
        skladPointNumber := 9;
        pNakopBusy := false;
        pNakopWait := true;
      end;
      // ОТО
      tempOTO := TOTO.Create('16K20');
      with tempOTO do
      begin
        ReadPicture('oto1');
        RLow       := mlUniform;
        RLowValue1 := 10;
        RLowValue2 := 50;
        RTimeMin   := 1;
        RTimeMax   := 5;
        otoX       := 12;
        otoY       := 1;
        otoDX      := 3.7;
        otoDY      := 2.0;
        nakopValue := 25;
        nakopX     := 12.5;
        nakopY     := 3.2;
        nakopPointNumber := 1;
        pOtoBusy   := true;
        pOtoWait   := false;
        pOtoRepair := false;
        pNakopBusy := false;
        pNakopWait := true;
      end;
      tempOTO := TOTO.Create('1Ф616Ф3');
      with tempOTO do
      begin
        ReadPicture('oto2');
        RLow       := mlUniform;
        RLowValue1 := 10;
        RLowValue2 := 50;
        RTimeMin   := 4;
        RTimeMax   := 7;
        otoX       := 21;
        otoY       := 1;
        otoDX      := 3.2;
        otoDY      := 1.6;
        nakopValue := 25;
        nakopX     := 21.5;
        nakopY     := 3.2;
        nakopPointNumber := 12;
        pOtoBusy   := true;
        pOtoWait   := true;
        pOtoRepair := true;
        pNakopBusy := true;
        pNakopWait := false;
      end;
      tempOTO := TOTO.Create('1740РФ3');
      with tempOTO do
      begin
        ReadPicture('oto3');
        RLow       := mlUniform;
        RLowValue1 := 10;
        RLowValue2 := 50;
        RTimeMin   := 3;
        RTimeMax   := 8;
        otoX       := 11.05;
        otoY       := 23;
        otoDX      := 5.5;
        otoDY      := 4.3;
        nakopValue := 25;
        nakopX     := 11.55;
        nakopY     := 21.5;
        nakopPointNumber := 6;
        pOtoBusy   := true;
        pOtoWait   := true;
        pOtoRepair := true;
        pNakopBusy := false;
        pNakopWait := true;
      end;
      tempOTO := TOTO.Create('3Е721ВФ3');
      with tempOTO do
      begin
        ReadPicture('oto4');
        RLow       := mlUniform;
        RLowValue1 := 10;
        RLowValue2 := 50;
        RTimeMin   := 2;
        RTimeMax   := 8;
        otoX       := 21.03;
        otoY       := 20.6;
        otoDX      := 3.5;
        otoDY      := 3.5;
        nakopValue := 25;
        nakopX     := 21.53;
        nakopY     := 19.1;
        nakopPointNumber := 7;
        pOtoBusy   := false;
        pOtoWait   := true;
        pOtoRepair := false;
        pNakopBusy := true;
        pNakopWait := false;
      end;
      // ТУ
      temptu := TTU.Create('C4234');
      with tempTU do
      begin
        ReadPicture('tu');
        Speed    := 1.3;
        TimeLoad := 0.1;
        tuX      := 7;
        tuY      := 11.8;
        tuDX     := 2.5;
        tuDY     := 0.99;
        PredPointNumber := 3;
        OTOPos       := Sklads[0];
        pTuTransport := true;
        pTuFree      := true;
        pTuEmpty     := true;
        pTuLoad      := false;
      end;
      // Номенклатура
      tempNomenk := TNomenk.Create('Вентиль');
      with tempNomenk do
      begin
        ReadPicture('det1');
        AllValue   := 50;
        StartParty := 10;
        WorkParty  := 5;
        Low        := mlUniform;
        LowValue1  := 10;
        LowValue2  := 35;
        new(tempTP);
          AddTP(tempTP);
          tempTP^.OTO  := OTOs[0];
          tempTP^.Time := 30;
        new(tempTP);
          AddTP(tempTP);
          tempTP^.OTO  := OTOs[2];
          tempTP^.Time := 5;
        new(tempTP);
          AddTP(tempTP);
          tempTP^.OTO  := OTOs[3];
          tempTP^.Time := 10;
        new(tempTP);
          AddTP(tempTP);
          tempTP^.OTO  := OTOs[2];
          tempTP^.Time := 15;
      end;
      tempNomenk := TNomenk.Create('Винт');
      with tempNomenk do
      begin
        ReadPicture('det2');
        AllValue   := 40;
        StartParty := 10;
        WorkParty  := 4;
        Low        := mlUniform;
        LowValue1  := 10;
        LowValue2  := 35;
        new(tempTP);
          AddTP(tempTP);
          tempTP^.OTO  := OTOs[1];
          tempTP^.Time := 20;
        new(tempTP);
          AddTP(tempTP);
          tempTP^.OTO  := OTOs[3];
          tempTP^.Time := 10;
        new(tempTP);
          AddTP(tempTP);
          tempTP^.OTO  := OTOs[2];
          tempTP^.Time := 8;
        new(tempTP);
          AddTP(tempTP);
          tempTP^.OTO  := OTOs[3];
          tempTP^.Time := 10;
      end;
      ResetAllDemoObject;
    finally
      SplashForm.Free;
    end;
  except
    raise FMSError.CreateRes(ErrorClearAll);
  end;
end;

procedure TFMSForm.ResetAllDemoObject;
begin
  try
    with FMSConst do
    begin
      Scale      := 17.3;
      dX         := 0;
      dY         := 0;
      minX       := 0;
      minY       := 0;
      maxX       := 0;
      maxY       := 0;
      WorkTime   := 0;
      doWorkTime := false;
    end;
    with Sklads[0] do
    begin
      pNakopBusy := true;
      pNakopWait := true;
    end;
    with Sklads[1] do
    begin
      pNakopBusy := false;
      pNakopWait := true;
    end;
    with OTOs[0] do
    begin
      RLow       := mlUniform;
      RLowValue1 := 10;
      RLowValue2 := 50;
      pOtoBusy   := true;
      pOtoWait   := false;
      pOtoRepair := false;
      pNakopBusy := false;
      pNakopWait := true;
    end;
    with OTOs[1] do
    begin
      RLow       := mlUniform;
      RLowValue1 := 10;
      RLowValue2 := 50;
      pOtoBusy   := true;
      pOtoWait   := true;
      pOtoRepair := true;
      pNakopBusy := true;
      pNakopWait := false;
    end;
    with OTOs[2] do
    begin
      RLow       := mlUniform;
      RLowValue1 := 10;
      RLowValue2 := 50;
      pOtoBusy   := true;
      pOtoWait   := true;
      pOtoRepair := true;
      pNakopBusy := false;
      pNakopWait := true;
    end;
    with OTOs[3] do
    begin
      RLow       := mlUniform;
      RLowValue1 := 10;
      RLowValue2 := 50;
      pOtoBusy   := false;
      pOtoWait   := true;
      pOtoRepair := false;
      pNakopBusy := true;
      pNakopWait := false;
    end;
    with TUs[0] do
    begin
      pTuTransport := true;
      pTuFree      := true;
      pTuEmpty     := true;
      pTuLoad      := false;
    end;
    with Nomenks[0] do
    begin
      Low        := mlUniform;
      LowValue1  := 10;
      LowValue2  := 35;
    end;
    with Nomenks[1] do
    begin
      Low        := mlUniform;
      LowValue1  := 10;
      LowValue2  := 35;
    end;
    UpDateFMSObject;
    TreeView.Selected := OTOTreeNode;
    ShowFMSObject(OTOTreeNode);

    PlaneChange := not PlaneChange;
    PlaneChange := false;
    PlaneLoaded := not PlaneLoaded;
    PlaneLoaded := true;
  except
    raise FMSError.CreateRes(ErrorClearAll);
  end;
end;

procedure TFMSForm.SetFMSModelPath(Value: string);
begin
  FMSModelPath := GetNormalDir(Value);
end;

procedure TFMSForm.FormCreate(Sender: TObject);
var
  s: string;
  i: integer;
  autoRun: boolean;

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
  MsgList := TList.Create;
  if ShowQueryMessage(' Данная версия продукта ''Система моделирования компоновок'' (2.1) является демонстрационной.'+MyEOL+
                      ' Разработчик не гарантирует правильную работу данного продукта и не несет ответственность за возможный ущерб в результате использования данного продукта.'+MyEOL+
                      ' Вы согласны с данными требованиями', MB_YESNO) = IDNO then
  begin
    Application.Terminate;
    exit;
  end;
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
  LoadDemo;
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

  CreateAllDemoObject;
  IniFile := TIniFile.Create('fms.ini');
  // Задание пути РДО-имитатора
  RdoImitatorPathName := IniFile.ReadString('Application', 'RDO path', '');
  s := GetNormalDir(Application.ExeName)+'DemoModel\';
  CreateDir(s);
  PlaneFileName := s;
  SetFMSModelPath(PlaneFileName);
  // Обработать командную строку
  autoRun   := false;
  if ParamCount > 0 then
  begin
    // Автозапуск модели
    if GetParamBeginPos('-run') > 0 then autoRun := true;
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
  if autoRun then RunButtonClick(nil);
end;

procedure TFMSForm.LoadPlaneFile(Value: string);
begin
  ShowDemoMessage;
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
      flag := false;
      if SelTreeNode.Parent <> nil then
      begin
        flag := true;
        ActiveFMSObject := TFMSObject(SelTreeNode.Data);
        SelTreeNode := SelTreeNode.Parent;
      end else ActiveFMSObject := nil;
      flag := flag or (SelTreeNode.Index = TVPoint) or (SelTreeNode.Index = TVConst);
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

procedure TFMSForm.FormDestroy(Sender: TObject);
begin
  OTOs.Free;
  TUs.Free;
  Sklads.Free;
  Nomenks.Free;
  ClearDemo;
  MsgList.Free;
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
    CreateAllDemoObject;
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
  begin
    Title      := 'Сохранить файл компоновки';
    InitialDir := ExtractFileDir(PlaneFileName);
    DefaultExt := 'fms';
    Filter     := 'Файл компоновки (*.fms)|*.fms|Все файлы|*.*';
    FileName   := '';
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
//    Не надо менять директорию демо-модели
//    if Result then SetFMSModelPath(FileName);
  end;
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
    ShowDemoMessage;
    Result := true;
  except
    on E: FMSError do raise FMSError.CreateStopSubError(ErrorSaveFMSFile, E.Message);
    else raise FMSError.CreateStopRes(ErrorSaveFMSFile);
    Result := false;
  end;
end;

procedure TFMSForm.BuildButtonClick(Sender: TObject);
begin
  ShowInfoMessage(LoadStr(BuildDemoMessage));
end;

procedure TFMSForm.BuildModel;

  procedure SaveFromResource(Res: integer; FileExt: string);
  var ResStream: TResourceStream;
  begin
    ResStream := TResourceStream.CreateFromID(0, Res, RT_RCDATA);
    try ResStream.SaveToFile(FMSModelPath+FMSModelName+FileExt);
    finally ResStream.Free; end;
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
    SaveFromResource(RC_PAT, '.pat');
    SaveFromResource(RC_RTP, '.rtp');
    SaveFromResource(RC_RSS, '.rss');
    SaveFromResource(RC_OPR, '.opr');
    SaveFromResource(RC_FRM, '.frm');
    SaveFromResource(RC_FUN, '.fun');
    SaveFromResource(RC_SMR, '.smr');
    SaveFromResource(RC_PMD, '.pmd');
    OTOs[0].SavePicture;
    OTOs[1].SavePicture;
    OTOs[2].SavePicture;
    OTOs[3].SavePicture;
    TUs[0].SavePicture;
    Sklads[0].SavePicture;
    Sklads[1].SavePicture;
    SaveBMPFromResource('Controlp');
    SaveBMPFromResource('Controlm');
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
begin
  ShowDemoMessage;
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

end.

