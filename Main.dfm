object frmMain: TfrmMain
  Left = 0
  Top = 0
  Caption = 'Paper Manager'
  ClientHeight = 598
  ClientWidth = 1021
  Color = clBtnFace
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  WindowState = wsMaximized
  OnCreate = FormCreate
  OnMouseWheelDown = FormMouseWheelDown
  OnMouseWheelUp = FormMouseWheelUp
  PixelsPerInch = 96
  TextHeight = 13
  object pgeMain: TPageControl
    Left = 0
    Top = 0
    Width = 1021
    Height = 576
    ActivePage = tabRecognized
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    object tabRecognized: TTabSheet
      Caption = 'Recognized'
      object grdRecognized: TRzStringGrid
        Left = 0
        Top = 0
        Width = 1013
        Height = 548
        Align = alClient
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goFixedRowClick]
        ParentFont = False
        PopupMenu = mnuPopup
        TabOrder = 0
        OnDblClick = grdRecognizedDblClick
        OnDrawCell = grdRecognizedDrawCell
        OnFixedCellClick = grdRecognizedFixedCellClick
        OnKeyPress = grdRecognizedKeyPress
        OnMouseWheelDown = FormMouseWheelDown
        OnMouseWheelUp = FormMouseWheelUp
      end
    end
    object tabError: TTabSheet
      Caption = 'Error'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ImageIndex = 1
      ParentFont = False
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object grdError: TRzStringGrid
        Left = 0
        Top = 0
        Width = 1013
        Height = 548
        Align = alClient
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goFixedRowClick]
        ParentFont = False
        PopupMenu = mnuPopup
        TabOrder = 0
        OnDblClick = grdRecognizedDblClick
        OnDrawCell = grdRecognizedDrawCell
        OnFixedCellClick = grdRecognizedFixedCellClick
        OnKeyPress = grdRecognizedKeyPress
      end
    end
    object tabOther: TTabSheet
      Caption = 'Other'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ImageIndex = 2
      ParentFont = False
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 0
      ExplicitHeight = 0
      object grdOther: TRzStringGrid
        Left = 0
        Top = 0
        Width = 1013
        Height = 548
        Align = alClient
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goFixedRowClick]
        ParentFont = False
        PopupMenu = mnuPopup
        TabOrder = 0
        OnDblClick = grdRecognizedDblClick
        OnDrawCell = grdRecognizedDrawCell
        OnFixedCellClick = grdRecognizedFixedCellClick
        OnKeyPress = grdRecognizedKeyPress
      end
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 576
    Width = 1021
    Height = 22
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    object edtHighlight: TEdit
      Left = 0
      Top = 0
      Width = 721
      Height = 22
      Align = alClient
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnChange = edtHighlightChange
      OnKeyDown = edtHighlightKeyDown
      ExplicitHeight = 21
    end
    object stbMain: TStatusBar
      Left = 721
      Top = 0
      Width = 300
      Height = 22
      Align = alRight
      Panels = <>
      SimplePanel = True
    end
  end
  object mnuPopup: TPopupMenu
    OnPopup = mnuPopupPopup
    Left = 504
    Top = 304
    object mnuOpenFileLocation: TMenuItem
      Caption = '&Open File Location'
      OnClick = mnuOpenFileLocationClick
    end
    object mnuCopyBibFromBaidu: TMenuItem
      Caption = 'Copy &bib From Baidu Xueshu'
      OnClick = mnuCopyBibFromBaiduClick
    end
    object mnuSearchOnBaidu: TMenuItem
      Caption = 'Search On Baidu Xueshu'
      OnClick = mnuSearchOnGoogleClick
    end
    object mnuSearchOnGoogle: TMenuItem
      Caption = 'Search On Google Scholar'
      OnClick = mnuSearchOnGoogleClick
    end
    object N1: TMenuItem
      Caption = '-'
    end
    object mnuEnterCatalog: TMenuItem
      Caption = 'Enter Catalog'
      OnClick = mnuEnterCatalogClick
    end
    object mnuGotoParentCatalog: TMenuItem
      Caption = 'Goto Parent Catalog'
      OnClick = mnuGotoParentCatalogClick
    end
    object N2: TMenuItem
      Caption = '-'
    end
    object mnuCopy: TMenuItem
      Caption = '&Copy Grids'
      ShortCut = 16451
      OnClick = mnuCopyClick
    end
    object mnuRefresh: TMenuItem
      Caption = '&Refresh'
      ShortCut = 116
      OnClick = mnuRefreshClick
    end
  end
  object tmrWatchDirChange: TTimer
    OnTimer = tmrWatchDirChangeTimer
    Left = 696
    Top = 304
  end
  object tmrUpdateUI: TTimer
    Interval = 1
    OnTimer = tmrUpdateUITimer
    Left = 696
    Top = 384
  end
end
