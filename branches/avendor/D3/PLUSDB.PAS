unit Plusdb;

interface
uses PlusMemo, Messages, Classes, Controls, DB, DBTables, dbCtrls;

type  TDBPlusMemo = class(TPlusMemo)
  private
    FDataLink: TFieldDataLink;
    FAutoDisplay: Boolean;
    FFocused: Boolean;
    FMemoLoaded: Boolean;
    FInDataChange, fInChange: Boolean;
    procedure DataChange(Sender: TObject);
    procedure EditingChange(Sender: TObject);
    function GetDataField: string;
    function GetDataSource: TDataSource;
    function GetField: TField;
    function GetReadOnly: Boolean;
    procedure SetDataField(const Value: string);
    procedure SetDataSource(Value: TDataSource);
    procedure SetReadOnly(Value: Boolean);
    procedure SetAutoDisplay(Value: Boolean);
    procedure SetFocused(Value: Boolean);
    procedure UpdateData(Sender: TObject);
    {procedure WMCut(var Message: TMessage); message WM_CUT;
    procedure WMPaste(var Message: TMessage); message WM_PASTE;}
    procedure CMEnter(var Message: TCMEnter); message CM_ENTER;
    procedure CMExit(var Message: TCMExit); message CM_EXIT;
    procedure WMLButtonDblClk(var Message: TWMLButtonDblClk); message WM_LBUTTONDBLCLK;
  protected
    procedure Change; override;
    procedure KeyDown(var Key: Word; Shift: TShiftState); override;
    procedure KeyPress(var Key: Char); override;
    procedure Notification(AComponent: TComponent; Operation: TOperation); override;
  public
    constructor Create(AOwner: TComponent); override;
    destructor Destroy; override;
    procedure LoadMemo;
    property Field: TField read GetField;
  published
    property AutoDisplay: Boolean read FAutoDisplay write SetAutoDisplay default True;
    property DataField: string read GetDataField write SetDataField;
    property DataSource: TDataSource read GetDataSource write SetDataSource;
    property ReadOnly: Boolean read GetReadOnly write SetReadOnly default False;
    end;

procedure Register;

implementation

uses WinTypes, WinProcs, SysUtils{, LibConst};

{ TDBPlusMemo }

constructor TDBPlusMemo.Create(AOwner: TComponent);
  begin
  inherited Create(AOwner);
  inherited ReadOnly := True;
  FAutoDisplay := True;
  FDataLink := TFieldDataLink.Create;
  FDataLink.Control := Self;
  FDataLink.OnDataChange := DataChange;
  FDataLink.OnEditingChange := EditingChange;
  FDataLink.OnUpdateData := UpdateData;
  end;

destructor TDBPlusMemo.Destroy;
  begin
  FDataLink.Free;
  FDataLink := nil;
  inherited Destroy;
  end;

procedure TDBPlusMemo.Notification(AComponent: TComponent; Operation: TOperation);
  begin
  inherited Notification(AComponent, Operation);
  if (Operation = opRemove) and (FDataLink <> nil) and
    (AComponent = DataSource) then DataSource := nil;
  end;

procedure TDBPlusMemo.KeyDown(var Key: Word; Shift: TShiftState);
  begin
  inherited KeyDown(Key, Shift);
  if FMemoLoaded then
    begin
    if (Key = VK_DELETE) or ((Key = VK_INSERT) and (ssShift in Shift)) then
                                                               FDataLink.Edit
    end
  else Key := 0;
end;

procedure TDBPlusMemo.KeyPress(var Key: Char);
var sstart, slen: Longint;
begin
if FMemoLoaded then
  begin
  if (Key in [#32..#255]) and (FDataLink.Field <> nil) and
      not FDataLink.Field.IsValidChar(Key) then
      begin
      MessageBeep($FFFF);
      Key := #0;
      end;
  case Key of
    ^B, ^F, ^H, ^E, ^I, ^J, ^M, ^T, ^U, ^V, ^X, #32..#255:
          begin
          sstart:= SelStart;
          slen:= SelLength;
          FDataLink.Edit;
          SelStart:= sstart;
          SelLength:= slen
          end;
    #27: FDataLink.Reset;
      end
  end

else begin
     if Key = #13 then LoadMemo;
     Key := #0;
     end;
inherited KeyPress(Key)
end;

procedure TDBPlusMemo.Change;
  begin
  if fInDataChange then Exit;
  fInChange:= True;
  FDataLink.Edit;
  FDataLink.Modified;
  FMemoLoaded := True;
  inherited Change;
  fInChange:= False
  end;

function TDBPlusMemo.GetDataSource: TDataSource;
  begin
  Result := FDataLink.DataSource;
  end;

procedure TDBPlusMemo.SetDataSource(Value: TDataSource);
  begin
  FDataLink.DataSource := Value;
  end;

function TDBPlusMemo.GetDataField: string;
  begin
  Result := FDataLink.FieldName;
  end;

procedure TDBPlusMemo.SetDataField(const Value: string);
  begin
  FDataLink.FieldName := Value;
  end;

function TDBPlusMemo.GetReadOnly: Boolean;
  begin
  Result := FDataLink.ReadOnly;
  end;

procedure TDBPlusMemo.SetReadOnly(Value: Boolean);
  begin
  FDataLink.ReadOnly := Value;
  end;

function TDBPlusMemo.GetField: TField;
  begin
  Result := FDataLink.Field;
  end;

procedure __AHSHIFT; far; external 'KERNEL' index 113;

procedure TDBPlusMemo.LoadMemo;
  var tmpStream: TMemoryStream;
      {$IFNDEF WIN32}
      pc: Pointer; cp: PChar;
      i: Longint;
      {$ENDIF}
  begin
  if not FMemoLoaded and (FDataLink.Field<>nil) and (FDataLink.Field is TBlobField) then
    begin
    {Lines.Assign(FDataLink.Field);}
    tmpStream:= TMemoryStream.Create;
    try
      try
      TBlobField(FDataLink.Field).SaveToStream(tmpStream);
      tmpStream.Position:= 0;

      {$IFNDEF WIN32}      {in 16bit world, #21 char are transformed to #167!!!}
          pc:= tmpStream.Memory;
          for i:= 0 to tmpStream.Size-1 do
            begin
            cp:= Ptr(LongRec(pc).Hi + LongRec(i).Hi shl Ofs(__AHSHIFT),
                      LongRec(i).Lo);
            if cp[0]=#167 then cp[0]:= #21
            end;
      {$ENDIF}

      LoadFromStream(tmpstream);
      FMemoLoaded := True;

      except on E:Exception do
                   begin
                   Clear;
                   Lines.Add(Format('(%s)', [E.Message]));
                   raise
                   end
          end;

    finally
      tmpStream.Free;
      EditingChange(Self)
      end
    end
end;

procedure TDBPlusMemo.DataChange(Sender: TObject);
var tmpstr: string;  {$IFNDEF WIN32} i: Integer;  {$ENDIF}
begin
if fInChange then Exit;
fInDataChange:= True;
  Clear;
  if FDataLink.Field <> nil then
    if FDataLink.Field is TBlobField then
      begin
      if FAutoDisplay or (FDataLink.Editing and FMemoLoaded) then
          begin
          FMemoLoaded := False;
          LoadMemo;
          end
      else begin
           Paragraphs[0]:= '(' + FDataLink.Field.DisplayLabel + ')';
           FMemoLoaded := False;
           end;
      end
    else begin
         if FFocused and FDataLink.CanModify then tmpstr := FDataLink.Field.Text
                                             else tmpstr := FDataLink.Field.DisplayText;
         {$IFNDEF WIN32}
           for i:= 1 to Length(tmpstr) do
                if tmpstr[i]= #167 then tmpstr[i]:= #21;
         {$ENDIF}
         Paragraphs[0]:= tmpstr;
         FMemoLoaded := True;
         end

  else begin
       if csDesigning in ComponentState then Paragraphs[0] := Name;
       FMemoLoaded := False;
       end;

inherited Change;
fInDataChange:= False;
end;

procedure TDBPlusMemo.EditingChange(Sender: TObject);
  begin
  inherited ReadOnly := not (FDataLink.Editing and FMemoLoaded);
  end;

procedure TDBPlusMemo.UpdateData(Sender: TObject);
  var tmpStream: TStream; tmpstr: string[255]; tlen: Word;
  begin
  if FDataLink.Field is TBlobField then
    begin
    {FDataLink.Field.Assign(Lines)}
    tmpStream:= TMemoryStream.Create;
    SaveToStream(tmpStream);
    tmpStream.Position:= 0;
    TBlobField(FDataLink.Field).LoadFromStream(tmpStream);
    tmpStream.Free
    end

  else begin
       tlen:= GetTextBuf(@tmpstr[1], 255);
       {$IFDEF WIN32} SetLength(tmpstr, tlen);
       {$ELSE}        tmpstr[0]:= Chr(tlen);
       {$ENDIF}
       FDataLink.Field.Text := tmpstr
       end;
end;

procedure TDBPlusMemo.SetFocused(Value: Boolean);
  begin
  if FFocused <> Value then
    begin
    FFocused := Value;
    if not (FDataLink.Field is TBlobField) then FDataLink.Reset
    end
  end;

procedure TDBPlusMemo.CMEnter(var Message: TCMEnter);
  begin
  SetFocused(True);
  inherited
  end;

procedure TDBPlusMemo.CMExit(var Message: TCMExit);
  begin
  if not (FDataLink.Field is TBlobField) then
    try
      FDataLink.UpdateRecord;
    except
      SetFocus;
      raise;
    end;
  SetFocused(False);
  inherited;
  end;

procedure TDBPlusMemo.SetAutoDisplay(Value: Boolean);
  begin
  if FAutoDisplay <> Value then
    begin
    FAutoDisplay := Value;
    if Value then LoadMemo;
    end;
  end;

procedure TDBPlusMemo.WMLButtonDblClk(var Message: TWMLButtonDblClk);
  begin
  if not FMemoLoaded then LoadMemo else inherited;
  end;

procedure Register;
  begin
  RegisterComponents('Additional', [TDBPlusMemo])
  end;


end.
