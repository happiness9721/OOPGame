#pragma once
#include "StdAfx.h"
#include <vector>
#include <ddraw.h>
using namespace std;

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class�|�إ�DirectDraw����A�H���Ѩ�Lclass�ϥ�
	// �@�몺�C���ä��ݪ����ާ@�o�Ӫ���A�]���i�H���޳o��class���ϥΤ�k
	/////////////////////////////////////////////////////////////////////////////

	class CDDraw {
		friend class CMovingBitmap;
	public:
		~CDDraw();
		static void  BltBackColor(DWORD);		// �NBack plain�����ۤW���w���C��
		static void  BltBackToPrimary();		// �NBack plain�K��Primary plain
		static CDC*  GetBackCDC();				// ���oBack Plain��DC (device context)
		static void  GetClientRect(CRect &r);	// ���o�]�w���ѪR��
		static void  Init(int, int);			// Initialize direct draw
		static void  ReleaseBackCDC();			// ��Back Plain��DC (device context)
		static bool  SetFullScreen(bool);		// �]�w�����ù��Ҧ�/�����Ҧ�
		static bool  IsFullScreen();			// �^���O�_�����ù��Ҧ�/�����Ҧ�
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