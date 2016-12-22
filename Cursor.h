#pragma once
#include "Source\gamelib.h"
#include "Source\StdAfx.h"

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
		void OnAnimationMove();											//�ʵe
		void OnShow(int, int, int);											// �N�ϧζK��e��
		void Initalize(int);
		void setBotY(double*);												//�]�wholdCursor�n�l�ܪ��Y
		double* getY();

	private:
		static CAnimation leftCursor;
		static CAnimation downCursor;
		static CAnimation upCursor;
		static CAnimation rightCursor;
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
		static bool	 isLoadBitmap;
		double x, y;					// �y��
		double dx, dy;					// �첾�q
		double delay;					// ����ɶ�
		double *botY;
	};
}