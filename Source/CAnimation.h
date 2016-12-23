#pragma once
#include "StdAfx.h"
#include <list>
#include "CMovingBitmap.h"
using namespace std;

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 這個class提供可以移動的動畫
	// 每個Public Interface的用法都要懂，Implementation可以不懂
	/////////////////////////////////////////////////////////////////////////////

	class CAnimation {
	public:
		CAnimation(int = 10);				// Constructor (預設動畫播放頻率每1/3秒換一張圖)
		void  AddBitmap(int, COLORREF = CLR_INVALID);
		// 增加一張圖形至動畫(圖的編號及透明色)
		void  AddBitmap(char *, COLORREF = CLR_INVALID);
		// 增加一張圖形至動畫(圖的編號及透明色)
		int   GetCurrentBitmapNumber();	// 取得正在撥放的bitmap是第幾個bitmap
		int   Height();					// 取得動畫的高度
		bool  IsFinalBitmap();			// 回傳正在撥放的bitmap是否為最後一個bitmap
		int   Left();					// 取得動畫的左上角的 x 座標
		void  OnMove();					// 依頻率更換bitmap
		void  OnShow();					// 將動畫貼到螢幕
		void  Reset();					// 重設播放順序回到第一張圖形
		void  SetDelayCount(int);		// 設定動畫播放速度的常數(越大越慢)
		void  SetTopLeft(int, int);		// 將動畫的左上角座標移至 (x,y)
		int   Top();					// 取得動畫的左上角的 y 座標
		int   Width();					// 取得動畫的寬度
	private:
		list<CMovingBitmap>				bmp;			// list of CMovingBitmap
		list<CMovingBitmap>::iterator	bmp_iter;		// list iterator
		int								bmp_counter;	// 儲存bmp_iter為第n個bmp
		int								delay_counter;	// 延緩動畫播放速度的計數器
		int								delay_count;	// 動畫播放速度的常數
		int								x, y;			// 動畫的座標
	};
}
