#include "StdAfx.h"
#include "CInteger.h"
#include "gamelib.h"
#include "resource.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// CInteger: 這個class提供顯示整數圖形的能力
	// 1. 要懂得怎麼呼叫(運用)其各種能力，但是可以不懂下列的程式是什麼意思
	// 2. 自己寫到運用CMovingBitmap的程式時，可以參考下列程式的寫法
	/////////////////////////////////////////////////////////////////////////////

	CMovingBitmap CInteger::digit[11];
	bool CInteger::isBmpLoaded = false;
	CInteger::CInteger(int digits): NUMDIGITS(digits)
	{
		isBmpLoaded = false;
	}

	void CInteger::Add(int x)
	{
		n += x;
	}

	int CInteger::GetInteger()
	{
		return n;
	}

	void CInteger::ChangeDigit(int index)
	{
		NUMDIGITS = index;
	}

	void CInteger::LoadBitmap()
	{
		//
		// digit[i]為class varibale，所以必須避免重複LoadBitmap
		//
		if (!isBmpLoaded) {
			char* d[11] = { 
				"Bitmaps/numbers/0.bmp",
				"Bitmaps/numbers/1.bmp",
				"Bitmaps/numbers/2.bmp",
				"Bitmaps/numbers/3.bmp",
				"Bitmaps/numbers/4.bmp",
				"Bitmaps/numbers/5.bmp",
				"Bitmaps/numbers/6.bmp",
				"Bitmaps/numbers/7.bmp",
				"Bitmaps/numbers/8.bmp",
				"Bitmaps/numbers/9.bmp",
				"Bitmaps/numbers/00.bmp"
			};
			for (int i = 0; i < 11; i++)
				digit[i].LoadBitmap(d[i], RGB(255, 255, 255));
			isBmpLoaded = true;
		}
	}

	void CInteger::SetInteger(int i)
	{
		n = i;
	}

	void CInteger::SetTopLeft(int nx, int ny)		// 將動畫的左上角座標移至 (x,y)
	{
		x = nx; y = ny;
	}

	void CInteger::ShowBitmap()
	{
		GAME_ASSERT(isBmpLoaded, "CInteger: 請先執行LoadBitmap，然後才能ShowBitmap");
		int nx;		// 待顯示位數的 x 座標
		int MSB;	// 最左邊(含符號)的位數的數值
		if (n >= 0) {
			MSB = n;
			nx = x + digit[0].Width()*(NUMDIGITS - 1);
		}
		else {
			MSB = -n;
			nx = x + digit[0].Width()*NUMDIGITS;
		}
		for (int i = 0; i < NUMDIGITS; i++) {
			int d = MSB % 10;
			MSB /= 10;
			digit[d].SetTopLeft(nx, y);
			digit[d].ShowBitmap();
			nx -= digit[d].Width();
		}
		if (n < 0) { // 如果小於0，則顯示負號
			digit[10].SetTopLeft(nx, y);
			digit[10].ShowBitmap();
		}
	}

}