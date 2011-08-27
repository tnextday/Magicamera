unit MainUnt;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, StdCtrls, zlib, ShellAPI;

type
  TFrmMain = class(TForm)
    mm_vs: TMemo;
    mm_fs: TMemo;
    lbe_vs: TLabel;
    Label2: TLabel;
    Splitter1: TSplitter;
    Panel1: TPanel;
    btn_pack: TButton;
    le_output: TLabeledEdit;
    btn_explorer: TButton;
    sd_savefile: TSaveDialog;
    procedure btn_explorerClick(Sender: TObject);
    procedure btn_packClick(Sender: TObject);
    procedure FormCreate(Sender: TObject);
  private
    { Private declarations }
  public
    procedure MyDrag (var Msg: TMessage); message WM_DROPFILES;
    { Public declarations }
  end;

var
  FrmMain: TFrmMain;

function GetDragDrop(var Msg:TMessage):string ;  //获取拖放的地址
function SaveShaderPack(vs,fs:AnsiString; output : string ):boolean;

implementation

{$R *.dfm}

{********************************************
 * sp文件结构
 * sp关键字[4字节]|原始数据大小[4字节]|压缩后大小[4字节]|数据
 * 解压后文件结构：
 * 数据大小[4字节]|vertex shader[#0结尾的字符串]|数据大小[4字节]|fragment shader[#0结尾的字符串]|
*********************************************}

function SaveShaderPack(vs,fs:AnsiString; output : string ):boolean;
var
  mstrm, mstrm2 : TMemoryStream;
  zcstrm : TCompressionStream;
  len : Integer;
  osize: Integer;
begin
  mstrm := TMemoryStream.Create;
  len := Length(vs)+1;
  mstrm.Write(len, 4);
  mstrm.Write(PAnsiChar(vs)^, len);
  len := Length(fs)+1;
  mstrm.Write(len, 4);
  mstrm.Write(PAnsiChar(fs)^, len);
  osize := mstrm.Size;
  mstrm2 := TMemoryStream.Create;
  zcstrm := TCompressionStream.Create(mstrm2);
  zcstrm.Write(mstrm.Memory^, osize);
  zcstrm.Free;
  mstrm.Clear;
  mstrm.Write(PAnsiChar('SPV1')^, 4);
  mstrm.Write(osize, 4);
  len := mstrm2.Size;
  mstrm.Write(len, 4);
  mstrm.Write(mstrm2.Memory^, mstrm2.Size);
  mstrm.SaveToFile(output);
  mstrm.Free;
  mstrm2.Free;
end;

function GetDragDrop(var Msg:TMessage):string ;  //获取拖放的地址
var
  pcFileName: array [0..MAX_PATH] of Char;
  iSize : Integer;
begin
  Result := '';
  iSize := DragQueryFile(Msg.wParam, 0, nil, 0) + 1;
  DragQueryFile(Msg.wParam, 0, pcFileName, iSize);
  DragFinish(Msg.wParam);
  if (GetFileAttributes(pcFileName) and faDirectory) = 0 then
    Result := pcFileName;
end;

procedure TFrmMain.btn_explorerClick(Sender: TObject);
var
  str : String;
begin
  if (sd_savefile.Execute(Self.Handle)) then
  begin
    str := sd_savefile.FileName;
    if (str <> '') and (LowerCase(ExtractFileExt(str)) <> '.sp')then
      str := str + '.sp';
    le_output.Text := str;
  end;
end;

procedure TFrmMain.btn_packClick(Sender: TObject);
var
  str_vs : AnsiString;
  str_fs : AnsiString;
begin
  if le_output.Text = '' then
    Exit;
  str_vs := mm_vs.Text;
  str_fs := mm_fs.Text;
  SaveShaderPack(str_vs, str_fs, le_output.Text);
end;

procedure TFrmMain.FormCreate(Sender: TObject);
begin
  DragAcceptFiles(Handle, True);
end;

procedure TFrmMain.MyDrag(var Msg: TMessage);
var
  str_file : string;
  str_ext : string;
  str_name : string;
  str_path : string;
begin
  str_file := GetDragDrop(msg);
  str_ext := LowerCase(ExtractFileExt(str_file));
  if (str_ext <> '.vs') and (str_ext <> '.fs') then
  begin
    Exit;
  end;
  str_path := ExtractFileDir(str_file);
  str_name := ExtractFileName(str_file);
  Delete(str_name, Length(str_name)-2, 3);
  mm_vs.Lines.LoadFromFile(str_path+'\'+str_name+'.vs');
  mm_fs.Lines.LoadFromFile(str_path+'\'+str_name+'.fs');
  le_output.Text := str_path+'\'+str_name+'.sp';
end;

end.
