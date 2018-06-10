//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include "RzGrids.hpp"
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdHTTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TPageControl *pgeMain;
	TTabSheet *tabRecognized;
	TTabSheet *tabError;
	TTabSheet *tabOther;
	TRzStringGrid *grdError;
	TRzStringGrid *grdOther;
	TPopupMenu *mnuPopup;
	TMenuItem *mnuOpenFileLocation;
	TRzStringGrid *grdRecognized;
	TTimer *tmrWatchDirChange;
	TMenuItem *mnuCopy;
	TMenuItem *mnuRefresh;
	TPanel *Panel1;
	TEdit *edtHighlight;
	TStatusBar *stbMain;
	TTimer *tmrUpdateUI;
	TMenuItem *mnuSearchOnGoogle;
	TMenuItem *mnuSearchOnBaidu;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *mnuEnterCatalog;
	TMenuItem *mnuGotoParentCatalog;
	TMenuItem *mnuCopyBibFromBaidu;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall edtHighlightChange(TObject *Sender);
	void __fastcall grdRecognizedDblClick(TObject *Sender);
	void __fastcall grdRecognizedDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall edtHighlightKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall grdRecognizedFixedCellClick(TObject *Sender, int ACol, int ARow);
	void __fastcall mnuOpenFileLocationClick(TObject *Sender);
	void __fastcall tmrWatchDirChangeTimer(TObject *Sender);
	void __fastcall mnuCopyClick(TObject *Sender);
	void __fastcall mnuRefreshClick(TObject *Sender);
	void __fastcall grdRecognizedKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall tmrUpdateUITimer(TObject *Sender);
	void __fastcall FormMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall FormMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall mnuSearchOnGoogleClick(TObject *Sender);
	void __fastcall mnuEnterCatalogClick(TObject *Sender);
	void __fastcall mnuGotoParentCatalogClick(TObject *Sender);
	void __fastcall mnuPopupPopup(TObject *Sender);
	void __fastcall mnuCopyBibFromBaiduClick(TObject *Sender);
private:	// User declarations
	void __fastcall ScanFiles(String currentDir);
    void __fastcall SortGrid(TStringGrid* grd, int beginRow, int endRow, int keyCol, bool extractExt);
public:		// User declarations
	__fastcall TfrmMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
