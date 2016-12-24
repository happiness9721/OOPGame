#pragma once
#include "CAnimation.h"

namespace game_framework {

	/////////////////////////////////////////////////////////////////////////////
	// ���
	/////////////////////////////////////////////////////////////////////////////

	class Cursor {
	public:
		Cursor();
		~Cursor();

		void LoadBitmap();										// ���J�ϧ�
		void OnMove(double);											// ����
		void OnShow(int, int, int);											// �N�ϧζK��e��
		void Initalize(int);
		void setBotY(double*);												//�]�wholdCursor�n�l�ܪ��Y
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
		double x, y;					// �y��
		double dx, dy;					// �첾�q
		double delay;					// ����ɶ�
		double *botY;
	};
}