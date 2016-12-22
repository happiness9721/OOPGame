#pragma once
#include "CMovingBitmap.h"

namespace game_framework {
	class CIntegerCombo {
	public:
		CIntegerCombo(int = 2);			// default 5 digits
		void Add(int n);			// �W�[��ƭ�
		int  GetInteger();			// �^�Ǿ�ƭ�
		void LoadBitmap();			// ���J0..9�έt�����ϧ�
		void SetInteger(int);		// �]�w��ƭ�
		void SetTopLeft(int, int);	// �N�ʵe�����W���y�в��� (x,y)
		void ShowBitmap();			// �N�ʵe�K��ù�
	private:
		const int NUMDIGITS;			// �@���NUMDIGITS�Ӧ��
		static CMovingBitmap digit[10]; // �x�s0..9�έt�����ϧ�(bitmap)
		int x, y;						// ��ܪ��y��
		int n;							// ��ƭ�
		static bool isBmpLoaded;				// �O�_�w�g���J�ϧ�
	};
}