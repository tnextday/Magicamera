object FrmMain: TFrmMain
  Left = 0
  Top = 0
  Caption = 'Shader Packer'
  ClientHeight = 424
  ClientWidth = 584
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = #24494#36719#38597#40657
  Font.Style = []
  Padding.Left = 10
  Padding.Top = 10
  Padding.Right = 10
  Padding.Bottom = 10
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 17
  object lbe_vs: TLabel
    Left = 10
    Top = 10
    Width = 564
    Height = 17
    Align = alTop
    Caption = 'Vertex shader:'
    ExplicitWidth = 84
  end
  object Label2: TLabel
    Left = 10
    Top = 180
    Width = 564
    Height = 17
    Align = alTop
    Caption = 'Fragment shader:'
    ExplicitWidth = 102
  end
  object Splitter1: TSplitter
    Left = 10
    Top = 177
    Width = 564
    Height = 3
    Cursor = crVSplit
    Align = alTop
    ExplicitLeft = 0
    ExplicitTop = 130
    ExplicitWidth = 239
  end
  object mm_vs: TMemo
    Left = 10
    Top = 27
    Width = 564
    Height = 150
    Align = alTop
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object mm_fs: TMemo
    Left = 10
    Top = 197
    Width = 564
    Height = 176
    Align = alClient
    ScrollBars = ssVertical
    TabOrder = 1
  end
  object Panel1: TPanel
    Left = 10
    Top = 373
    Width = 564
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 2
    DesignSize = (
      564
      41)
    object btn_pack: TButton
      Left = 440
      Top = 8
      Width = 116
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = #25171' '#21253
      TabOrder = 0
      OnClick = btn_packClick
    end
    object le_output: TLabeledEdit
      Left = 72
      Top = 8
      Width = 305
      Height = 25
      Anchors = [akLeft, akRight, akBottom]
      EditLabel.Width = 60
      EditLabel.Height = 17
      EditLabel.Caption = #36755#20986#25991#20214#65306
      LabelPosition = lpLeft
      TabOrder = 1
    end
    object btn_explorer: TButton
      Left = 383
      Top = 6
      Width = 34
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = '>>'
      TabOrder = 2
      OnClick = btn_explorerClick
    end
  end
  object sd_savefile: TSaveDialog
    Filter = 'Shader File|*.s'
    Left = 328
    Top = 248
  end
end
