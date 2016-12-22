#include "CInteger.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// CInteger: �o��class������ܾ�ƹϧΪ���O
	// 1. �n���o���I�s(�B��)��U�د�O�A���O�i�H�����U�C���{���O����N��
	// 2. �ۤv�g��B��CMovingBitmap���{���ɡA�i�H�ѦҤU�C�{�����g�k
	/////////////////////////////////////////////////////////////////////////////

	CMovingBitmap CInteger::digit[11];
	bool CInteger::isBmpLoaded = false;
	CInteger::CInteger(int digits)
		: NUMDIGITS(digits)
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
		// digit[i]��class varibale�A�ҥH�����קK����LoadBitmap
		//
		if (!isBmpLoaded) {
			int d[11] = { IDB_0,IDB_1,IDB_2,IDB_3,IDB_4,IDB_5,IDB_6,IDB_7,IDB_8,IDB_9,IDB_MINUS };
			for (int i = 0; i < 11; i++)
				digit[i].LoadBitmap(d[i], RGB(255, 255, 255));
			isBmpLoaded = true;
		}
	}

	void CInteger::SetInteger(int i)
	{
		n = i;
	}

	void CInteger::SetTopLeft(int nx, int ny)		// �N�ʵe�����W���y�в��� (x,y)
	{
		x = nx; y = ny;
	}

	void CInteger::ShowBitmap()
	{
		GAME_ASSERT(isBmpLoaded, "CInteger: �Х�����LoadBitmap�A�M��~��ShowBitmap");
		int nx;		// ����ܦ�ƪ� x �y��
		int MSB;	// �̥���(�t�Ÿ�)����ƪ��ƭ�
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
		if (n < 0) { // �p�G�p��0�A�h��ܭt��
			digit[10].SetTopLeft(nx, y);
			digit[10].ShowBitmap();
		}
	}

}