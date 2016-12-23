#pragma once
#include "CMovingBitmap.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class������ܾ�ƹϧΪ���O
	// �C��Public Interface���Ϊk���n���AImplementation�i�H����
	/////////////////////////////////////////////////////////////////////////////

	class CInteger {
	public:
		CInteger(int = 8);			// default 8 digits
		void ChangeDigit(int);
		void Add(int n);			// �W�[��ƭ�
		int  GetInteger();			// �^�Ǿ�ƭ�
		void LoadBitmap();			// ���J0..9�έt�����ϧ�
		void SetInteger(int);		// �]�w��ƭ�
		void SetTopLeft(int, int);	// �N�ʵe�����W���y�в��� (x,y)
		void ShowBitmap();			// �N�ʵe�K��ù�
	private:
		int NUMDIGITS;			// �@���NUMDIGITS�Ӧ��
		static CMovingBitmap digit[11]; // �x�s0..9�έt�����ϧ�(bitmap)
		int x, y;						// ��ܪ��y��
		int n;							// ��ƭ�
		static bool isBmpLoaded;				// �O�_�w�g���J�ϧ�
	};
}