#include "StdAfx.h"

namespace game_framework {

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���Ѯɶ��B���~������
	// �@�몺�C���ä��ݪ����ާ@�o�Ӫ���A�]���i�H���޳o��class���ϥΤ�k
	/////////////////////////////////////////////////////////////////////////////

	class CSpecialEffect {
	public:
		static void  SetCurrentTime();					// �x�s�ثe���ɶ���ctime
		static DWORD GetEllipseTime();					// Ū���ثe���ɶ� - ctime
		static int   GetCurrentTimeCount();				// Ū���x�sctime������
		static void  Delay(DWORD ms);					// ���� x ms
		static void  DelayFromSetCurrentTime(DWORD ms);	// ��ctime�_��A���� x ms
	private:
		static DWORD ctime;
		static int	 ctimeCount;
	};
}