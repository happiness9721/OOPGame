#pragma once
#include "StdAfx.h"
#include <list>
#include "CMovingBitmap.h"
using namespace std;

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���ѥi�H���ʪ��ʵe
	// �C��Public Interface���Ϊk���n���AImplementation�i�H����
	/////////////////////////////////////////////////////////////////////////////

	class CAnimation {
	public:
		CAnimation(int = 10);				// Constructor (�w�]�ʵe�����W�v�C1/3���@�i��)
		void  AddBitmap(int, COLORREF = CLR_INVALID);
		// �W�[�@�i�ϧΦܰʵe(�Ϫ��s���γz����)
		void  AddBitmap(char *, COLORREF = CLR_INVALID);
		// �W�[�@�i�ϧΦܰʵe(�Ϫ��s���γz����)
		int   GetCurrentBitmapNumber();	// ���o���b����bitmap�O�ĴX��bitmap
		int   Height();					// ���o�ʵe������
		bool  IsFinalBitmap();			// �^�ǥ��b����bitmap�O�_���̫�@��bitmap
		int   Left();					// ���o�ʵe�����W���� x �y��
		void  OnMove();					// ���W�v��bitmap
		void  OnShow();					// �N�ʵe�K��ù�
		void  Reset();					// ���]���񶶧Ǧ^��Ĥ@�i�ϧ�
		void  SetDelayCount(int);		// �]�w�ʵe����t�ת��`��(�V�j�V�C)
		void  SetTopLeft(int, int);		// �N�ʵe�����W���y�в��� (x,y)
		int   Top();					// ���o�ʵe�����W���� y �y��
		int   Width();					// ���o�ʵe���e��
	private:
		list<CMovingBitmap>				bmp;			// list of CMovingBitmap
		list<CMovingBitmap>::iterator	bmp_iter;		// list iterator
		int								bmp_counter;	// �x�sbmp_iter����n��bmp
		int								delay_counter;	// ���w�ʵe����t�ת��p�ƾ�
		int								delay_count;	// �ʵe����t�ת��`��
		int								x, y;			// �ʵe���y��
	};
}
