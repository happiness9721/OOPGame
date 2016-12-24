#include "StdAfx.h"
#include "CAnimation.h"
#include "gamelib.h"

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// CAnimation: 提供動畫的能力
	// 1. 要懂得怎麼呼叫(運用)其各種能力，但是可以不懂下列的程式是什麼意思
	// 2. 自己寫到運用CMovingBitmap的程式時，可以參考下列程式的寫法
	// 3. 如果需要修改或擴充CAnimation的功能時，請用繼承或直接拷貝的方式，最好
	//    不要直接改CAnimation。
	/////////////////////////////////////////////////////////////////////////////

	CAnimation::CAnimation(int count)
	{
		delay_count = count;
		delay_counter = delay_count;
		x = y = bmp_counter = 0;
	}

	void CAnimation::AddBitmap(int IDB_BITMAP, COLORREF colorkey)
	{
		CMovingBitmap add_bmp;
		add_bmp.LoadBitmap(IDB_BITMAP, colorkey);
		bmp.insert(bmp.end(), add_bmp);
		Reset();
	}

	void CAnimation::AddBitmap(char *filename, COLORREF colorkey)
	{
		CMovingBitmap add_bmp;
		add_bmp.LoadBitmap(filename, colorkey);
		bmp.insert(bmp.end(), add_bmp);
		Reset();
	}

	void CAnimation::AddBitmap(CMovingBitmap add_bmp)
	{
		bmp.insert(bmp.end(), add_bmp);
		Reset();
	}

	int CAnimation::GetCurrentBitmapNumber()
	{
		return bmp_counter;
	}

	int CAnimation::Height()
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
		return bmp_iter->Height();
	}

	bool CAnimation::IsFinalBitmap()
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
		return bmp_counter == (bmp.size() - 1);
	}

	int CAnimation::Left()
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
		return x;
	}

	void CAnimation::OnMove()
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
		if (--delay_counter <= 0) {
			delay_counter = delay_count;
			bmp_iter++;
			bmp_counter++;
			if (bmp_iter == bmp.end()) {
				bmp_iter = bmp.begin();
				bmp_counter = 0;
			}
		}
	}

	void CAnimation::Reset()
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
		delay_counter = delay_count;
		bmp_iter = bmp.begin();
		bmp_counter = 0;
	}

	void CAnimation::SetDelayCount(int dc)
	{
		GAME_ASSERT(dc > 0, "CAnimation: Delay count must be >= 1");
		delay_count = dc;
	}

	void CAnimation::SetTopLeft(int nx, int ny)
	{
		x = nx, y = ny;
		bmp_iter->SetTopLeft(x, y);
	}

	void CAnimation::OnShow()
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded before they are shown.");
		bmp_iter->SetTopLeft(x, y);
		bmp_iter->ShowBitmap();
	}

	int CAnimation::Top()
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
		return y;
	}

	int CAnimation::Width()
	{
		GAME_ASSERT(bmp.size() != 0, "CAnimation: Bitmaps must be loaded first.");
		return bmp_iter->Width();
	}
}
