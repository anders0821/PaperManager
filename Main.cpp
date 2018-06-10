//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
#include <Clipbrd.hpp>
#include <IOUtils.hpp>
#include <memory>
using namespace std;
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RzGrids"
#pragma resource "*.dfm"
TfrmMain *frmMain;
//---------------------------------------------------------------------------
__fastcall TfrmMain::TfrmMain(TComponent* Owner)
	: TForm(Owner)
{
}
//-------------------------------------------------------------------------
void __fastcall TfrmMain::FormCreate(TObject *Sender)
{
	// 彻底清空旧数据
	// 减少RowCount ColCount旧数据仍然存在 可以节省内存 或许可以降低BUG
    for(int i=0; i<grdRecognized->RowCount; i++)
	    for(int j=0; j<grdRecognized->ColCount; j++)
			grdRecognized->Cells[j][i] = "";
    for(int i=0; i<grdError->RowCount; i++)
	    for(int j=0; j<grdError->ColCount; j++)
			grdError->Cells[j][i] = "";
    for(int i=0; i<grdOther->RowCount; i++)
	    for(int j=0; j<grdOther->ColCount; j++)
			grdOther->Cells[j][i] = "";

	// 初始化表1
	grdRecognized->DefaultRowHeight = 24;
	grdRecognized->RowCount = 2;
	grdRecognized->ColCount = 7;
	grdRecognized->FixedRows = 1;
    grdRecognized->FixedCols = 1;
	grdRecognized->Cells[0][0] = "";
	grdRecognized->Cells[1][0] = "Author";
	grdRecognized->Cells[2][0] = "Year";
	grdRecognized->Cells[3][0] = "Title";
	grdRecognized->Cells[4][0] = "Note";
	grdRecognized->Cells[5][0] = "Catalog";
	grdRecognized->Cells[6][0] = "Path";
	grdRecognized->Cells[0][1] = "1";
	grdRecognized->Cells[1][1] = "";
	grdRecognized->Cells[2][1] = "";
	grdRecognized->Cells[3][1] = "";
	grdRecognized->Cells[4][1] = "";
	grdRecognized->Cells[5][1] = "";
	grdRecognized->Cells[6][1] = "";
	grdRecognized->ColWidths[0] = 50;
	grdRecognized->ColWidths[1] = 100;
	grdRecognized->ColWidths[2] = 100;
	grdRecognized->ColWidths[3] = 600;
	grdRecognized->ColWidths[4] = 600;
	grdRecognized->ColWidths[5] = 400;
	grdRecognized->ColWidths[6] = 0;

	// 初始化表2
	grdError->DefaultRowHeight = 24;
	grdError->RowCount = 2;
	grdError->ColCount = 2;
    grdError->FixedRows = 1;
    grdError->FixedCols = 1;
	grdError->Cells[0][0] = "";
	grdError->Cells[1][0] = "Path";
	grdError->Cells[0][1] = "1";
	grdError->Cells[1][1] = "";
	grdError->ColWidths[0] = 50;
	grdError->ColWidths[1] = 1400;

	// 初始化表3
	grdOther->DefaultRowHeight = 24;
	grdOther->RowCount = 2;
	grdOther->ColCount = 2;
    grdOther->FixedRows = 1;
    grdOther->FixedCols = 1;
	grdOther->Cells[0][0] = "";
	grdOther->Cells[1][0] = "Path";
	grdOther->Cells[0][1] = "1";
	grdOther->Cells[1][1] = "";
	grdOther->ColWidths[0] = 50;
	grdOther->ColWidths[1] = 1400;

    // 扫描文件
    ScanFiles(".\\");

    // 修改tab标题
    if(grdRecognized->Cells[grdRecognized->ColCount-1][1]!="")
		tabRecognized->Caption = String() + "Recognized (" + (grdRecognized->RowCount-1) + ")";
    else
		tabRecognized->Caption = String() + "Recognized (" + (grdRecognized->RowCount-2) + ")";
    if(grdError->Cells[grdError->ColCount-1][1]!="")
		tabError->Caption = String() + "Error (" + (grdError->RowCount-1) + ")";
    else
		tabError->Caption = String() + "Error (" + (grdError->RowCount-2) + ")";
    if(grdOther->Cells[grdOther->ColCount-1][1]!="")
		tabOther->Caption = String() + "Other (" + (grdOther->RowCount-1) + ")";
    else
		tabOther->Caption = String() + "Other (" + (grdOther->RowCount-2) + ")";

	// 修改窗体标题
	static String oldCaption = Caption;
	Caption = oldCaption + " - " + IncludeTrailingBackslash(GetCurrentDir());
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::edtHighlightChange(TObject *Sender)
{
	// 更新过滤器输入框背景色
	if(edtHighlight->Text!="")
		edtHighlight->Color = clYellow;
    else
    	edtHighlight->Color = clWindow;

    // 通知重绘
    grdRecognized->Repaint();
    grdError->Repaint();
    grdOther->Repaint();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::ScanFiles(String cd)
{
	// 递归扫描文件夹
	WIN32_FIND_DATA ffd = {0};
	HANDLE hFind = FindFirstFile((IncludeTrailingBackslash(cd)+"*").c_str(), &ffd);
	assert(hFind!=INVALID_HANDLE_VALUE);
	do
    {
		if(String(ffd.cFileName)=="." || String(ffd.cFileName)==".." || String(ffd.cFileName)=="$RECYCLE.BIN" || String(ffd.cFileName)=="System Volume Information")
        {
            // . ..
        }
        else if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            // 递归处理子目录
            ScanFiles(cd+ffd.cFileName+"\\");
        }
        else
        {
            // 文件
            String fn = ffd.cFileName;
            if(fn.SubString(1,1)=="[")// 文件名首尾为[则尝试识别
			{
            	// 识别文件名成分
            	String fn2 = ChangeFileExt(fn, "");
                if(fn2.SubString(1,1)=="[" && fn2.SubString(fn2.Length(),1)=="]")
                {
					fn2 = fn2.SubString(2,fn2.Length()-2);// 去掉首尾的[]
                    fn2 = StringReplace(fn2, "][", "?", TReplaceFlags()<<rfReplaceAll);// ][替换为? 本来文件名中不会有?
                    if(fn2.Pos("[")==0 && fn2.Pos("]")==0)// 剩下不应该有[]
                    {
						auto_ptr<TStringList> sl(new TStringList);
                        sl->StrictDelimiter = true;
                        sl->Delimiter = L'?';
                        sl->DelimitedText = fn2;
                        if(sl->Count>=3)// 应该被?分割为3个以上部分
                        {
                        	int year = StrToIntDef(sl->Strings[1], -1);
                        	if(year>=1900 && year<=2100)// 年份应在范围内
                            {
                                // recognized输出
                                if(grdRecognized->Cells[grdRecognized->ColCount-1][1]!="")
                                    grdRecognized->RowCount++;
                                grdRecognized->Cells[0][grdRecognized->RowCount-1] = grdRecognized->RowCount-1;
                                grdRecognized->Cells[1][grdRecognized->RowCount-1] = sl->Strings[0];
                                grdRecognized->Cells[2][grdRecognized->RowCount-1] = sl->Strings[1];
                                grdRecognized->Cells[3][grdRecognized->RowCount-1] = sl->Strings[2];
                                grdRecognized->Cells[4][grdRecognized->RowCount-1] = "";
                                for(int i=3; i<sl->Count; i++)
                                    grdRecognized->Cells[4][grdRecognized->RowCount-1] = grdRecognized->Cells[4][grdRecognized->RowCount-1] + sl->Strings[i] + " ";
                                grdRecognized->Cells[5][grdRecognized->RowCount-1] = cd.SubString(3, cd.Length()-3);
                                grdRecognized->Cells[6][grdRecognized->RowCount-1] = cd+fn;

                                // 跳过error输出
                                continue;
                            }
                        }
                    }
                }

                // error输出
				if(grdError->Cells[grdError->ColCount-1][1]!="")
                    grdError->RowCount++;
                grdError->Cells[0][grdError->RowCount-1] = grdError->RowCount-1;
                grdError->Cells[1][grdError->RowCount-1] = cd+fn;
            }
            else
            {
            	// other输出
                if(grdOther->Cells[grdOther->ColCount-1][1]!="")
                    grdOther->RowCount++;
                grdOther->Cells[0][grdOther->RowCount-1] = grdOther->RowCount-1;
				grdOther->Cells[1][grdOther->RowCount-1] = cd+fn;
            }
        }
    }
    while (FindNextFile(hFind, &ffd) != 0);
    FindClose(hFind);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::grdRecognizedDblClick(TObject *Sender)
{
    TStringGrid* grd = (TStringGrid*)Sender;

    // 点中表头或左侧退出
    TPoint mouse;
	bool r = GetCursorPos(&mouse);
    assert(r);
    mouse = grdRecognized->ScreenToClient(mouse);
    int ACol = -1;
    int ARow = -1;
    grd->MouseToCell(mouse.X, mouse.Y, ACol, ARow);
	if(ARow==-1 || ACol==-1 || ARow==0 || ACol==0)
		return;

    // 读取文件名
    assert(grd->Cells[grd->ColCount-1][0] == "Path");
    String fn = grd->Cells[grd->ColCount-1][grd->Row];

    // 文件不存在退出
	if(!FileExists(fn, false))
    {
    	ShowMessage("This file does not exist!");
        return;
    }

	// 打开文件
    ShellExecute(Handle, L"open", fn.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::grdRecognizedFixedCellClick(TObject *Sender, int ACol, int ARow)
{
    TStringGrid* grd = (TStringGrid*)Sender;

    // 没有点中表头退出
    if(ARow!=0)
    	return;

    if(ACol==0)
		SortGrid(grd, 1, grd->RowCount-1, grd->ColCount-1, true);// 点中第一个表头 按列的扩展名排序
    else
    	SortGrid(grd, 1, grd->RowCount-1, ACol, false);// 排序

    // 重写序号
    for(int i=1; i<grd->RowCount; i++)
		grd->Cells[0][i] = i;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::SortGrid(TStringGrid* grd, int beginRow, int endRow, int keyCol, bool extractExt)
{
	int n = endRow-beginRow+1;

    for (int j = 0; j < n - 1; j++)
    {
    	//OutputDebugString((String()+"---------"+j).c_str());
        for (int i = 0; i < n - 1 - j; i++)
        {
        	//OutputDebugString(String(i).c_str());

			int ii = i+beginRow;
        	assert(ii>=beginRow);
        	assert(ii<=endRow-1);
        	//OutputDebugString(String(ii).c_str());

        	String key1 = grd->Cells[keyCol][ii];
        	String key2 = grd->Cells[keyCol][ii+1];
            if(extractExt)
            {
            	key1 = ExtractFileExt(key1);
            	key2 = ExtractFileExt(key2);
            }
            if(key1 > key2)
            {
                // 交换
				auto_ptr<TStringList> tmp(new TStringList);
                tmp->Assign(grd->Rows[ii]);// tmp = ii
                grd->Rows[ii]->Assign(grd->Rows[ii+1]);// ii = ii+1
                grd->Rows[ii+1]->Assign(tmp.get());// ii+1 = tmp
            }
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::grdRecognizedDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{
    TStringGrid* grd = (TStringGrid*)Sender;

    // 是表头则退出
    if(ARow==-1 || ACol==-1 || ARow==0 || ACol==0)
		return;

    // 左侧显示图标再退出
    if(ACol==0)
    {
    	if(grd->Cells[grd->ColCount-1][ARow]=="")
        	return;

		TSHFileInfo fi = {};
        DWORD r = SHGetFileInfo(grd->Cells[grd->ColCount-1][ARow].c_str(), NULL, &fi, sizeof(TSHFileInfo), SHGFI_ICON | SHGFI_SMALLICON);
        assert((SUCCEEDED(r)));

        auto_ptr<TBitmap> bmp(new TBitmap());
        bmp->PixelFormat = pf32bit;
        bmp->SetSize(16, 16);
        DrawIconEx(bmp->Canvas->Handle, 0, 0, fi.hIcon, 16, 16, 0, NULL, DI_NORMAL );
        grd->Canvas->Draw(Rect.Right-6-16, (Rect.Bottom+Rect.Top)/2-8, bmp.get());
        return;
    }

    // 第一张表的最后一列退出
    if(grd == grdRecognized && ACol==grd->ColCount-1)
    	return;

    // 高亮显示
	int pos = grd->Cells[ACol][ARow].LowerCase().Pos(edtHighlight->Text.LowerCase());
    if(pos>0)
    {
    	// 备份canvas状态
    	TColor bak = grd->Canvas->Brush->Color;
    	TColor bak2 = grd->Canvas->Font->Color;
    	TBrushStyle bak3 = grd->Canvas->Brush->Style;
    	TColor bak4 = grd->Canvas->Pen->Color;

        // 绘黄底黑字
    	grd->Canvas->Brush->Color = clYellow;
        TRect Rect2 = Rect;
        Rect2.Left = Rect2.Left-4;
    	grd->Canvas->FillRect(Rect2);
        grd->Canvas->TextRect(Rect, Rect.Left + 2, Rect.Top + 5, grd->Cells[ACol][ARow]);

        // 绘红字
        grd->Canvas->Brush->Style = bsClear;
        grd->Canvas->Font->Color = clRed;
        int offset = grd->Canvas->TextWidth(grd->Cells[ACol][ARow].SubString(1,pos-1));
        String text = grd->Cells[ACol][ARow].SubString(pos, edtHighlight->Text.Length());// 由于大小写这里必须重新计算
        grd->Canvas->TextRect(Rect, Rect.Left + 2 + offset, Rect.Top + 5, text);

        // 选中者绘红框
        grd->Canvas->Pen->Color = clRed;
        if(State.Contains(gdSelected))
        {
        	grd->Canvas->Rectangle(Rect2.Left, Rect2.Top, Rect2.Right, Rect2.Bottom);
            //grd->Canvas->DrawFocusRect(Rect2);
        }

        // 还原canvas状态
		grd->Canvas->Brush->Color = bak;
		grd->Canvas->Font->Color = bak2;
        grd->Canvas->Brush->Style = bak3;
        grd->Canvas->Pen->Color = bak4;
    }
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::edtHighlightKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	// 在Highlight上回车搜索
	if(Key!=VK_RETURN)
    	return;

	// Highlight为空
	if(edtHighlight->Text=="")
    	return;

    // 找到显示当前的grd
    TStringGrid* grd = NULL;
    if(pgeMain->ActivePage==tabRecognized)
        grd = grdRecognized;
	else if(pgeMain->ActivePage==tabError)
        grd = grdError;
	else if(pgeMain->ActivePage==tabOther)
        grd = grdOther;
    else
    	assert(false);

    // 不是单选则选中左上角单元格 从头开始搜索
	if(grd->Selection.Left!=grd->Selection.Right || grd->Selection.Top!=grd->Selection.Bottom)
    {
    	grd->Row = 1;
    	grd->Col = 1;
    }

    // 搜索下半 再搜索上半
    int curPos = (grd->Row)*grd->ColCount + grd->Col;
    for(int i=curPos+1; i<curPos+grd->RowCount*grd->ColCount; i++)
    {
		int ii = i%(grd->RowCount*grd->ColCount);
        int iiRow = ii/grd->ColCount;
        int iiCol = ii%grd->ColCount;

        // 跳过不能选中的表头
        if(iiRow==0 || iiCol==0)
        	continue;

        // 跳过第一张表的最后一列
        if(grd == grdRecognized && iiCol==grd->ColCount-1)
        	continue;

        // 命中
		if(grd->Cells[iiCol][iiRow].LowerCase().Pos(edtHighlight->Text.LowerCase()))
        {
            grd->Row = iiRow;
            grd->Col = iiCol;
            return;
        }
    }

    // 显示未找到
    ShowMessage("Search string '"+edtHighlight->Text+"' not found");
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuOpenFileLocationClick(TObject *Sender)
{
	// 找到显示当前的grd
	TStringGrid* grd = NULL;
	if(pgeMain->ActivePage==tabRecognized)
		grd = grdRecognized;
	else if(pgeMain->ActivePage==tabError)
		grd = grdError;
	else if(pgeMain->ActivePage==tabOther)
		grd = grdOther;
	else
		assert(false);

	// 点中表头或左侧退出
	TPoint mouse = mnuPopup->PopupPoint;;
	mouse = grdRecognized->ScreenToClient(mouse);
	int ACol = -1;
	int ARow = -1;
	grd->MouseToCell(mouse.X, mouse.Y, ACol, ARow);
	if(ARow==-1 || ACol==-1 || ARow==0 || ACol==0)
		return;

	// 获取文件名
	String fn = grd->Cells[grd->ColCount-1][ARow];
	if(fn=="")
		return;

	// 打开文件夹并选中
    fn = ExpandFileName(fn);
    ITEMIDLIST *pidl = ILCreateFromPath(fn.c_str());
    if(pidl)
    {
        SHOpenFolderAndSelectItems(pidl,0,0,0);
        ILFree(pidl);
    }
}
//---------------------------------------------------------------------------
String getHtml(String url)
{
	url = TIdURI::URLEncode(url);

	// http
	auto_ptr<TIdHTTP> http(new TIdHTTP());

	// 自定义http头
	http->Request->UserAgent = "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:12.0) Gecko/20100101 Firefox/12.0";

	// post获取html
	auto_ptr<TStringStream> ss(new TStringStream());
	///try
	///{
		http->Get(url, ss.get());// postData会自动被%转义
	///}
	///catch(...)
	///{
	///	return "postHtml error";
    ///}
	AnsiString utf8Html = (char*)ss->Memory;// 不可直接用ss->DataString这里用到UnicodeString遇到  "啊"  这种情况会错乱

	// utf8转unicode
	String html = UTF8ToString(utf8Html);

	return html;
}
String getStringBetween(String s, String prefix, String suffix)
{
	// 截断前缀
	int index = s.Pos(prefix);
	if(index==0)
		return "";
	s = s.SubString(index+prefix.Length(), s.Length());

	// 截断后缀
	index = s.Pos(suffix);
	if(index==0)
		return "";
	s = s.SubString(1, index-1);

	return s;
}
void __fastcall TfrmMain::mnuCopyBibFromBaiduClick(TObject *Sender)
{
	// 找到显示当前的grd
    TStringGrid* grd = NULL;
    if(pgeMain->ActivePage==tabRecognized)
        grd = grdRecognized;
	else if(pgeMain->ActivePage==tabError)
        grd = grdError;
	else if(pgeMain->ActivePage==tabOther)
        grd = grdOther;
    else
    	assert(false);

    if(grd != grdRecognized)
    	return;

    // 点中表头或左侧退出
    TPoint mouse = mnuPopup->PopupPoint;;
    mouse = grdRecognized->ScreenToClient(mouse);
    int ACol = -1;
    int ARow = -1;
	grd->MouseToCell(mouse.X, mouse.Y, ACol, ARow);
	if(ARow==-1 || ACol==-1 || ARow==0 || ACol==0)
		return;

    // 获取Title
    String title = grd->Cells[3][ARow];
	if(title=="")
		return;

	// getHtml
	String url = "http://xueshu.baidu.com/s?wd="+title;
	String html = getHtml(url);

	// 解析
	String dataLink = getStringBetween(html, "<a class=\"sc_q\" href=\"javascript:;\" data-link=\"", "\" data-sign=\"");
	String dataSign = getStringBetween(html, "<a class=\"sc_q\" href=\"javascript:;\" data-link=\""+dataLink+"\" data-sign=\"", "\" data-click=\"{'button_tp':'cite'}\">");
	if(dataLink=="" || dataSign=="")
	{
		ShowMessage("Failed to obtain bib.");
		return;
	}

	// getHtml
	url = "http://xueshu.baidu.com/u/citation?&url="+dataLink+"&sign="+dataSign;
	html = getHtml(url);

	// 复制
	Clipboard()->AsText = html;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuSearchOnGoogleClick(TObject *Sender)
{
	// 找到显示当前的grd
    TStringGrid* grd = NULL;
    if(pgeMain->ActivePage==tabRecognized)
        grd = grdRecognized;
	else if(pgeMain->ActivePage==tabError)
        grd = grdError;
	else if(pgeMain->ActivePage==tabOther)
        grd = grdOther;
    else
    	assert(false);

    if(grd != grdRecognized)
    	return;

    // 点中表头或左侧退出
    TPoint mouse = mnuPopup->PopupPoint;;
    mouse = grdRecognized->ScreenToClient(mouse);
    int ACol = -1;
    int ARow = -1;
	grd->MouseToCell(mouse.X, mouse.Y, ACol, ARow);
	if(ARow==-1 || ACol==-1 || ARow==0 || ACol==0)
		return;

    // 获取Title
    String title = grd->Cells[3][ARow];
    if(title=="")
    	return;

	// 打开浏览器
	String url;
	if(Sender==mnuSearchOnGoogle)
		url = "https://scholar.google.com/scholar?q="+title;
	else if(Sender==mnuSearchOnBaidu)
		url = "http://xueshu.baidu.com/s?wd="+title;
	else
		assert(false);
	ShellExecute(NULL, L"open", url.c_str(), NULL, NULL, SW_SHOWNORMAL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuCopyClick(TObject *Sender)
{
	// 找到显示当前的grd
	TStringGrid* grd = NULL;
	if(pgeMain->ActivePage==tabRecognized)
		grd = grdRecognized;
	else if(pgeMain->ActivePage==tabError)
		grd = grdError;
	else if(pgeMain->ActivePage==tabOther)
		grd = grdOther;
	else
		assert(false);

	// 组织为tab 回车分割
	String clip;
	for(int i=grd->Selection.Top; i<=grd->Selection.Bottom; i++)
	{
		if(i!=grd->Selection.Top)
			clip += "\r\n";
		for(int j=grd->Selection.Left; j<=grd->Selection.Right; j++)
		{
			if(j!=grd->Selection.Left)
				clip += "\t";

			String str = grd->Cells[j][i];
			str = StringReplace(str, "\t", "", TReplaceFlags()<<rfReplaceAll);
			str = StringReplace(str, "\r", "", TReplaceFlags()<<rfReplaceAll);
			str = StringReplace(str, "\n", "", TReplaceFlags()<<rfReplaceAll);
			clip += str;
		}
	}

    // 复制
	Clipboard()->AsText = clip;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuRefreshClick(TObject *Sender)
{
	// 热键F5刷新
	FormCreate(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::tmrWatchDirChangeTimer(TObject *Sender)
{
	// 首次FindFirstChangeNotification
	static HANDLE h = NULL;
	static bool first = true;
    if(first)
    {
    	first = false;
        h = FindFirstChangeNotification(L".\\", true, FILE_NOTIFY_CHANGE_FILE_NAME|FILE_NOTIFY_CHANGE_DIR_NAME);
        assert(h != INVALID_HANDLE_VALUE);
    }

	// 异步
    // WaitForSingleObject FindNextChangeNotification直到超时
    bool changed = false;
    while (true)
    {
        DWORD r = WaitForSingleObject(h, 1);
        assert(r==WAIT_OBJECT_0||r==WAIT_TIMEOUT);

        // WAIT_TIMEOUT
        if(r==WAIT_TIMEOUT)
            break;

        // WAIT_OBJECT_0
        changed = true;
        bool r2 = FindNextChangeNotification(h);
        assert(r2);
    }

	// 有改动
    if(changed)
    	FormCreate(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::grdRecognizedKeyPress(TObject *Sender, System::WideChar &Key)
{
    if(!edtHighlight->Focused())
    {
        edtHighlight->Text = "";
		edtHighlight->SetFocus();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmMain::tmrUpdateUITimer(TObject *Sender)
{
	// 找到显示当前的grd
	TStringGrid* grd = NULL;
	if(pgeMain->ActivePage==tabRecognized)
		grd = grdRecognized;
	else if(pgeMain->ActivePage==tabError)
		grd = grdError;
	else if(pgeMain->ActivePage==tabOther)
		grd = grdOther;
	else
		assert(false);

	// 显示当前选中数量
	int nSel = grd->Selection.Bottom-grd->Selection.Top+1;
	stbMain->SimpleText = String() + nSel + " rows selected";
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled)
{
    Handled = true;

    // 找到显示当前的grd
	TStringGrid* grd = NULL;
    if(pgeMain->ActivePage==tabRecognized)
        grd = grdRecognized;
	else if(pgeMain->ActivePage==tabError)
        grd = grdError;
	else if(pgeMain->ActivePage==tabOther)
        grd = grdOther;
    else
    	assert(false);

	// 原来滚轮滚动选择Cell 改为正常的滚动条
    // 必须grd form同时使用改写wheel时间才能达到最佳效果
    grd->Perform(WM_VSCROLL, SB_LINEDOWN, 1);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::FormMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled)
{
    Handled = true;

    // 找到显示当前的grd
    TStringGrid* grd = NULL;
    if(pgeMain->ActivePage==tabRecognized)
		grd = grdRecognized;
	else if(pgeMain->ActivePage==tabError)
        grd = grdError;
	else if(pgeMain->ActivePage==tabOther)
        grd = grdOther;
    else
    	assert(false);

	// 原来滚轮滚动选择Cell 改为正常的滚动条
    // 必须grd form同时使用改写wheel时间才能达到最佳效果
    grd->Perform(WM_VSCROLL, SB_LINEUP, 1);
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuEnterCatalogClick(TObject *Sender)
{
	// 找到显示当前的grd
	TStringGrid* grd = NULL;
	if(pgeMain->ActivePage==tabRecognized)
		grd = grdRecognized;
	else if(pgeMain->ActivePage==tabError)
		grd = grdError;
	else if(pgeMain->ActivePage==tabOther)
		grd = grdOther;
	else
		assert(false);

	// 点中表头或左侧退出
	TPoint mouse = mnuPopup->PopupPoint;;
	mouse = grdRecognized->ScreenToClient(mouse);
	int ACol = -1;
	int ARow = -1;
	grd->MouseToCell(mouse.X, mouse.Y, ACol, ARow);
	if(ARow==-1 || ACol==-1 || ARow==0 || ACol==0)
		return;

	// 获取文件名
	String catalog = grd->Cells[grd->ColCount-2][ARow];
	if(catalog=="")
		return;
	ShowMessage(catalog);

	// 创建新进程
	String newCd = IncludeTrailingBackslash(IncludeTrailingBackslash(GetCurrentDir()) + catalog);
	STARTUPINFO si = {sizeof(STARTUPINFO)};
	PROCESS_INFORMATION pi = {};
	bool r = CreateProcess(Application->ExeName.c_str(), ("\""+Application->ExeName+"\"").c_str(), NULL, NULL, true, 0, NULL, newCd.c_str(), &si, &pi);
	assert(r);

	// 关闭自己
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuGotoParentCatalogClick(TObject *Sender)
{
	// 创建新进程
	String newCd = IncludeTrailingBackslash(IncludeTrailingBackslash(GetCurrentDir()) + "..\\");
	STARTUPINFO si = {sizeof(STARTUPINFO)};
	PROCESS_INFORMATION pi = {};
	bool r = CreateProcess(Application->ExeName.c_str(), ("\""+Application->ExeName+"\"").c_str(), NULL, NULL, true, 0, NULL, newCd.c_str(), &si, &pi);
	assert(r);

	// 关闭自己
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TfrmMain::mnuPopupPopup(TObject *Sender)
{
	// 找到显示当前的grd
	TStringGrid* grd = NULL;
	if(pgeMain->ActivePage==tabRecognized)
		grd = grdRecognized;
	else if(pgeMain->ActivePage==tabError)
		grd = grdError;
	else if(pgeMain->ActivePage==tabOther)
		grd = grdOther;
	else
		assert(false);

	// 点中表头或左侧退出
	TPoint mouse = mnuPopup->PopupPoint;;
	mouse = grdRecognized->ScreenToClient(mouse);
	int ACol = -1;
	int ARow = -1;
	grd->MouseToCell(mouse.X, mouse.Y, ACol, ARow);
	if(ARow==-1 || ACol==-1 || ARow==0 || ACol==0)
		Abort();

	// 获取文件名
	String fn = grd->Cells[grd->ColCount-1][ARow];

	// 是否选中 防止选中空行
	bool rowSelectValid = fn!="";
	mnuOpenFileLocation->Enabled = rowSelectValid;
	mnuCopyBibFromBaidu->Enabled = rowSelectValid;
	mnuSearchOnBaidu->Enabled = rowSelectValid;
	mnuSearchOnGoogle->Enabled = rowSelectValid;
	mnuEnterCatalog->Enabled = rowSelectValid;
	mnuCopy->Enabled = rowSelectValid;

	// 盘符根目录不再上一层了
	mnuGotoParentCatalog->Enabled = IncludeTrailingBackslash(GetCurrentDir()).Length() > 3;
}
//---------------------------------------------------------------------------

