#pragma once
#include "StdAfx.h"
#include <vector>
#include <ddraw.h>
using namespace std;

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class會建立DirectDraw物件，以提供其他class使用
	// 一般的遊戲並不需直接操作這個物件，因此可以不管這個class的使用方法
	/////////////////////////////////////////////////////////////////////////////

	class CDDraw {
		friend class CMovingBitmap;
	public:
		~CDDraw();
		static void  BltBackColor(DWORD);		// 將Back plain全部著上指定的顏色
		static void  BltBackToPrimary();		// 將Back plain貼至Primary plain
		static CDC*  GetBackCDC();				// 取得Back Plain的DC (device context)
		static void  GetClientRect(CRect &r);	// 取得設定的解析度
		static void  Init(int, int);			// Initialize direct draw
		static void  ReleaseBackCDC();			// 放掉Back Plain的DC (device context)
		static bool  SetFullScreen(bool);		// 設定為全螢幕模式/視窗模式
		static bool  IsFullScreen();			// 回答是否為全螢幕模式/視窗模式
	private:
		CDDraw();								// private constructor
		static void  BltBitmapToBack(unsigned SurfaceID, int x, int y);
		static void  BltBitmapToBack(unsigned SurfaceID, int x, int y, double factor);
		static void  BltBitmapToBitmap(unsigned SourceID, unsigned TargetID, int x, int y);
		static void	 CheckDDFail(char *s);
		static bool  CreateSurface();
		static bool  CreateSurfaceFullScreen();
		static bool  CreateSurfaceWindowed();
		static void  LoadBitmap(int i, int IDB_BITMAP);
		static void  LoadBitmap(int i, char *filename);
		static DWORD MatchColorKey(LPDIRECTDRAWSURFACE lpDDSurface, COLORREF color);
		static int   RegisterBitmap(int IDB_BITMAP, COLORREF ColorKey);
		static int   RegisterBitmap(char *filename, COLORREF ColorKey);
		static void  ReleaseSurface();
		static void  RestoreSurface();
		static void  SetColorKey(unsigned SurfaceID, COLORREF color);
		static HDC					hdc;
		static CDC					cdc;
		static CView				*pCView;
		static LPDIRECTDRAW2		lpDD;
		static LPDIRECTDRAWCLIPPER	lpClipperPrimary;
		static LPDIRECTDRAWCLIPPER	lpClipperBack;
		static LPDIRECTDRAWSURFACE	lpDDSPrimary;
		static LPDIRECTDRAWSURFACE	lpDDSBack;
		static vector<LPDIRECTDRAWSURFACE>	lpDDS;
		static HRESULT				ddrval;
		static vector<int>			BitmapID;
		static vector<string>		BitmapName;
		static vector<CRect>		BitmapRect;
		static vector<COLORREF>		BitmapColorKey;
		static bool					fullscreen;
		static CDDraw				ddraw;
		static int					size_x, size_y;
	};
}