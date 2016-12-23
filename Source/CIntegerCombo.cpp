#include "StdAfx.h"
#include "CIntegerCombo.h"
#include "gamelib.h"

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
		// digit[i]��class varibale�A�ҥH�����קK����LoadBitmap
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

	void CIntegerCombo::SetTopLeft(int nx, int ny)		// �N�ʵe�����W���y�в��� (x,y)
	{
		x = nx; y = ny;
	}

	void CIntegerCombo::ShowBitmap()
	{
		GAME_ASSERT(isBmpLoaded, "CInteger: �Х�����LoadBitmap�A�M��~��ShowBitmap");
		int nx;		// ����ܦ�ƪ� x �y��
		int MSB;	// �̥���(�t�Ÿ�)����ƪ��ƭ�
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
		if (n < 0) { // �p�G�p��0�A�h��ܭt��
			digit[10].SetTopLeft(nx, y);
			digit[10].ShowBitmap();
		}
	}
}