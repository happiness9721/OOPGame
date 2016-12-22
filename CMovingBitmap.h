#pragma once

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �o��class���ѰʺA(�i�H����)���ϧ�
	// �C��Public Interface���Ϊk���n���AImplementation�i�H����
	/////////////////////////////////////////////////////////////////////////////

	class CMovingBitmap {
	public:
		CMovingBitmap();
		int   Height();						// ���o�ϧΪ�����
		int   Left();						// ���o�ϧΪ����W���� x �y��
		void  LoadBitmap(int, COLORREF = CLR_INVALID);		// ���J�ϡA���w�Ϫ��s��(resource)�γz����
		void  LoadBitmap(char *, COLORREF = CLR_INVALID);	// ���J�ϡA���w�Ϫ��ɦW�γz����
		void  SetTopLeft(int, int);			// �N�Ϫ����W���y�в��� (x,y)
		void  ShowBitmap();					// �N�϶K��ù�
		void  ShowBitmap(double factor);	// �N�϶K��ù� factor < 1���Y�p�A>1�ɩ�j�C�`�N�G�ݭnVGA�d�w�骺�䴩�A�_�h�|�ܺC
		void  ShowBitmap(CMovingBitmap &);	// �N�϶K���t�@�i�ϤW (�ȨѯS��γ~)
		int   Top();						// ���o�ϧΪ����W���� y �y��
		int   Width();						// ���o�ϧΪ��e��
	protected:
		CRect    location;			// location of the bitmap
		bool     isBitmapLoaded;	// whether a bitmap has been loaded
		unsigned SurfaceID;			// the surface id of this bitmap
	};

	
}