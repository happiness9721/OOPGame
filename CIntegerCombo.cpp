#include "CIntegerCombo.h"

namespace game_framework {
	CMovingBitmap CIntegerCombo::digit[10];
	bool CIntegerCombo::isBmpLoaded = false;
	CIntegerCombo::CIntegerCombo(int digits)
		: NUMDIGITS(digits)
	{
		//	isBmpLoaded = false;
	}

	void CIntegerCombo::Add(int x)
	{
		n += x;
	}

	int CIntegerCombo::GetInteger()
	{
		return n;
	}

	void CIntegerCombo::LoadBitmap()
	{
		//
		// digit[i]為class varibale，所以必須避免重複LoadBitmap
		//
		if (!isBmpLoaded) {
			for (int i = 0; i < 10; i++)
			{
				char str[80];
				sprintf(str, "Bitmaps/numbers/combo/%d.bmp", i);
				digit[i].LoadBitmap(str, RGB(255, 255, 255));
			}
			isBmpLoaded = true;
		}
	}

	void CIntegerCombo::SetInteger(int i)
	{
		n = i;
	}

	void CIntegerCombo::SetTopLeft(int nx, int ny)		// 將動畫的左上角座標移至 (x,y)
	{
		x = nx; y = ny;
	}

	void CIntegerCombo::ShowBitmap()
	{
		GAME_ASSERT(isBmpLoaded, "CInteger: 請先執行LoadBitmap，然後才能ShowBitmap");
		int nx;		// 待顯示位數的 x 座標
		int MSB;	// 最左邊(含符號)的位數的數值
		CString s;
		s.Format("%d", n);
		int num = s.GetLength();
		if (n >= 0) {
			MSB = n;
			nx = x + digit[0].Width()*(num - 1);
		}
		else {
			MSB = -n;
			nx = x + digit[0].Width()*num;
		}
		for (int i = 0; i < num; i++) {
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