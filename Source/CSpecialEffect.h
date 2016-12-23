#include "StdAfx.h"

namespace game_framework {

	/////////////////////////////////////////////////////////////////////////////
	// 這個class提供時間、錯誤等控制
	// 一般的遊戲並不需直接操作這個物件，因此可以不管這個class的使用方法
	/////////////////////////////////////////////////////////////////////////////

	class CSpecialEffect {
	public:
		static void  SetCurrentTime();					// 儲存目前的時間至ctime
		static DWORD GetEllipseTime();					// 讀取目前的時間 - ctime
		static int   GetCurrentTimeCount();				// 讀取儲存ctime的次數
		static void  Delay(DWORD ms);					// 延遲 x ms
		static void  DelayFromSetCurrentTime(DWORD ms);	// 自ctime起算，延遲 x ms
	private:
		static DWORD ctime;
		static int	 ctimeCount;
	};
}