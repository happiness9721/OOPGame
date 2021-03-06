#pragma once
#include "CAnimation.h"

namespace game_framework {

	/////////////////////////////////////////////////////////////////////////////
	// 游標
	/////////////////////////////////////////////////////////////////////////////

	class Cursor {
	public:
		Cursor();
		~Cursor();

		void LoadBitmap();										// 載入圖形
		void OnMove(double);											// 移動
		void OnShow(int, int, int);											// 將圖形貼到畫面
		void Initalize(int);
		void setBotY(double*);												//設定holdCursor要追蹤的頭
		double* getY();

	private:
		CAnimation leftCursor;
		CAnimation downCursor;
		CAnimation upCursor;
		CAnimation rightCursor;
		static CMovingBitmap SP1;
		static CMovingBitmap SP2;
		static CMovingBitmap SP3;
		static CMovingBitmap leftHoldCursorHead;
		static CMovingBitmap leftHoldCursorBody;
		static CMovingBitmap leftHoldCursorBot;
		static CMovingBitmap downHoldCursorHead;
		static CMovingBitmap downHoldCursorBody;
		static CMovingBitmap downHoldCursorBot;
		static CMovingBitmap upHoldCursorHead;
		static CMovingBitmap upHoldCursorBody;
		static CMovingBitmap upHoldCursorBot;
		static CMovingBitmap rightHoldCursorHead;
		static CMovingBitmap rightHoldCursorBody;
		static CMovingBitmap rightHoldCursorBot;
		static CMovingBitmap mine;
		double x, y;					// 座標
		double dx, dy;					// 位移量
		double delay;					// 延遲時間
		double *botY;
	};
}