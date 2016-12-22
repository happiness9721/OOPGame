#pragma once

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class提供動態(可以移動)的圖形
	// 每個Public Interface的用法都要懂，Implementation可以不懂
	/////////////////////////////////////////////////////////////////////////////

	class CMovingBitmap {
	public:
		CMovingBitmap();
		int   Height();						// 取得圖形的高度
		int   Left();						// 取得圖形的左上角的 x 座標
		void  LoadBitmap(int, COLORREF = CLR_INVALID);		// 載入圖，指定圖的編號(resource)及透明色
		void  LoadBitmap(char *, COLORREF = CLR_INVALID);	// 載入圖，指定圖的檔名及透明色
		void  SetTopLeft(int, int);			// 將圖的左上角座標移至 (x,y)
		void  ShowBitmap();					// 將圖貼到螢幕
		void  ShowBitmap(double factor);	// 將圖貼到螢幕 factor < 1時縮小，>1時放大。注意：需要VGA卡硬體的支援，否則會很慢
		void  ShowBitmap(CMovingBitmap &);	// 將圖貼到到另一張圖上 (僅供特殊用途)
		int   Top();						// 取得圖形的左上角的 y 座標
		int   Width();						// 取得圖形的寬度
	protected:
		CRect    location;			// location of the bitmap
		bool     isBitmapLoaded;	// whether a bitmap has been loaded
		unsigned SurfaceID;			// the surface id of this bitmap
	};

	
}