object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 750
  ClientWidth = 1196
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 732
    Height = 732
    Caption = 'Panel1'
    DoubleBuffered = False
    ParentDoubleBuffered = False
    TabOrder = 0
    OnClick = Panel1Click
    OnMouseMove = Panel1MouseMove
  end
  object ValueListEditor1: TValueListEditor
    Left = 738
    Top = 0
    Width = 450
    Height = 233
    Strings.Strings = (
      'Name=80K6'
      'ADCClock, Hz=19178720'
      'ADCWidth=10'
      'PulseLength, ns=1668'
      'I_F, Hz=4794680'
      'LFM_Length=52'
      'Deviation, Hz=200000'
      'WaveLength, mm=100'
      'CountRotation=1'
      'DefaultRotation=6'
      'WidthDN=2')
    TabOrder = 1
    TitleCaptions.Strings = (
      #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1056#1051#1057
      #1047#1085#1072#1095#1077#1085#1080#1077)
    ColWidths = (
      153
      291)
  end
  object ValueListEditor2: TValueListEditor
    Left = 738
    Top = 239
    Width = 239
    Height = 162
    Strings.Strings = (
      'Target=MiG-29'
      'beta=90'
      'D, m=50000'
      'H, m=1000'
      'L, m=10'
      'S, m^2=10'
      'V, m/s=0')
    TabOrder = 2
    TitleCaptions.Strings = (
      #1055#1072#1088#1072#1084#1077#1090#1088#1099' '#1094#1077#1083#1080
      #1047#1085#1072#1095#1077#1085#1080#1077)
    ColWidths = (
      153
      80)
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 731
    Width = 1196
    Height = 19
    DoubleBuffered = True
    Panels = <>
    ParentDoubleBuffered = False
    SimplePanel = True
    ExplicitTop = 712
  end
  object TV: TTreeView
    Left = 738
    Top = 407
    Width = 239
    Height = 250
    Indent = 19
    TabOrder = 4
  end
  object ValueListEditor3: TValueListEditor
    Left = 983
    Top = 239
    Width = 205
    Height = 162
    Strings.Strings = (
      'D_start='
      'Az_start='
      'D_end='
      'Az_end='
      '')
    TabOrder = 5
    TitleCaptions.Strings = (
      #1050#1086#1086#1088#1076#1080#1085#1072#1090#1099' '#1094#1077#1083#1080
      #1047#1085#1072#1095#1077#1085#1080#1077)
    ColWidths = (
      99
      100)
  end
  object Button1: TButton
    Left = 738
    Top = 672
    Width = 75
    Height = 25
    Caption = 'Button1'
    TabOrder = 6
    OnClick = Button1Click
  end
  object edt_D0: TEdit
    Left = 992
    Top = 424
    Width = 121
    Height = 21
    TabOrder = 7
    Text = 'edt_D0'
  end
  object edt_beta0: TEdit
    Left = 992
    Top = 472
    Width = 121
    Height = 21
    TabOrder = 8
    Text = 'Edit1'
  end
  object edt_D3: TEdit
    Left = 992
    Top = 512
    Width = 121
    Height = 21
    TabOrder = 9
    Text = 'Edit1'
  end
  object edt_beta3: TEdit
    Left = 992
    Top = 552
    Width = 121
    Height = 21
    TabOrder = 10
    Text = 'Edit1'
  end
end
