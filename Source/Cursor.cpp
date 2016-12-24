#include "StdAfx.h"
#include "Cursor.h"
#include <vector>
#include "cursor_enum.h"
using namespace cursor_enum;

namespace game_framework {
	
	//////////////////////////////////////////////////////////////////////////////
	// 游標 
	///////////////////////////////////////////////////////////////////////////////
	Cursor::Cursor()
	{
		leftCursor = CAnimation();
		downCursor = CAnimation();
		upCursor = CAnimation();
		rightCursor = CAnimation();
	}

	Cursor::~Cursor()
	{
	}

	CMovingBitmap Cursor::leftHoldCursorHead;
	CMovingBitmap Cursor::leftHoldCursorBody;
	CMovingBitmap Cursor::leftHoldCursorBot;
	CMovingBitmap Cursor::downHoldCursorHead;
	CMovingBitmap Cursor::downHoldCursorBody;
	CMovingBitmap Cursor::downHoldCursorBot;
	CMovingBitmap Cursor::upHoldCursorHead;
	CMovingBitmap Cursor::upHoldCursorBody;
	CMovingBitmap Cursor::upHoldCursorBot;
	CMovingBitmap Cursor::rightHoldCursorHead;
	CMovingBitmap Cursor::rightHoldCursorBody;
	CMovingBitmap Cursor::rightHoldCursorBot;
	CMovingBitmap Cursor::mine;
	CMovingBitmap Cursor::SP1;
	CMovingBitmap Cursor::SP2;
	CMovingBitmap Cursor::SP3;

	void Cursor::Initalize(int position)
	{
		x = position;
		y = 480;
		dx = 0;
		dy = 0;
	}

	void Cursor::LoadBitmap()
	{
		static bool isLoadBitmap = false;
		static vector<CMovingBitmap> leftCursors = vector<CMovingBitmap>();
		static vector<CMovingBitmap> downCursors = vector<CMovingBitmap>();
		static vector<CMovingBitmap> upCursors = vector<CMovingBitmap>();
		static vector<CMovingBitmap> rightCursors = vector<CMovingBitmap>();

		if (!isLoadBitmap)
		{
			char filename[80];
			for (int i = 1; i < 17; i++)
			{
				sprintf(filename, "Bitmaps/cursor/leftCursor(%d).bmp", i);
				CMovingBitmap leftCursorBmp = CMovingBitmap();
				leftCursorBmp.LoadBitmap(filename, RGB(255, 255, 254));
				leftCursors.push_back(leftCursorBmp);

				sprintf(filename, "Bitmaps/cursor/downCursor(%d).bmp", i);
				CMovingBitmap downCursorBmp = CMovingBitmap();
				downCursorBmp.LoadBitmap(filename, RGB(255, 255, 254));
				downCursors.push_back(downCursorBmp);

				sprintf(filename, "Bitmaps/cursor/upCursor(%d).bmp", i);
				CMovingBitmap upCursorBmp = CMovingBitmap();
				upCursorBmp.LoadBitmap(filename, RGB(255, 255, 254));
				upCursors.push_back(upCursorBmp);

				sprintf(filename, "Bitmaps/cursor/rightCursor(%d).bmp", i);
				CMovingBitmap rightCursorBmp = CMovingBitmap();
				rightCursorBmp.LoadBitmap(filename, RGB(255, 255, 254));
				rightCursors.push_back(rightCursorBmp);
			}

			downHoldCursorHead.LoadBitmap("Bitmaps/cursor/Down Hold Head Inactive.bmp", RGB(255, 255, 255));
			downHoldCursorBody.LoadBitmap("Bitmaps/cursor/Down Hold Body Inactive.bmp", RGB(255, 255, 255));
			downHoldCursorBot.LoadBitmap("Bitmaps/cursor/Down Hold BottomCap Inactive.bmp", RGB(255, 255, 255));
			rightHoldCursorHead.LoadBitmap("Bitmaps/cursor/Right Hold Head Inactive.bmp", RGB(255, 255, 255));
			rightHoldCursorBody.LoadBitmap("Bitmaps/cursor/Right Hold Body Inactive.bmp", RGB(255, 255, 255));
			rightHoldCursorBot.LoadBitmap("Bitmaps/cursor/Right Hold BottomCap Inactive.bmp", RGB(255, 255, 255));
			leftHoldCursorHead.LoadBitmap("Bitmaps/cursor/Left Hold Head Inactive.bmp", RGB(255, 255, 255));
			leftHoldCursorBody.LoadBitmap("Bitmaps/cursor/Left Hold Body Inactive.bmp", RGB(255, 255, 255));
			leftHoldCursorBot.LoadBitmap("Bitmaps/cursor/Left Hold BottomCap Inactive.bmp", RGB(255, 255, 255));
			upHoldCursorHead.LoadBitmap("Bitmaps/cursor/Up Hold Head Inactive.bmp", RGB(255, 255, 255));
			upHoldCursorBody.LoadBitmap("Bitmaps/cursor/Up Hold Body Inactive.bmp", RGB(255, 255, 255));
			upHoldCursorBot.LoadBitmap("Bitmaps/cursor/Up Hold BottomCap Inactive.bmp", RGB(255, 255, 255));
			mine.LoadBitmap("Bitmaps/cursor/mine.bmp", RGB(255, 255, 255));
			SP1.LoadBitmap("Bitmaps/cursor/SPcursor2.bmp", RGB(0, 0, 0));
			SP2.LoadBitmap("Bitmaps/cursor/SPcursor3.bmp", RGB(0, 0, 0));
			SP3.LoadBitmap("Bitmaps/cursor/SPcursor4.bmp", RGB(0, 0, 0));
			isLoadBitmap = true;
		}

		for (int i = 0; i < 16; i++)
		{
			leftCursor.AddBitmap(leftCursors[i]);
			downCursor.AddBitmap(downCursors[i]);
			upCursor.AddBitmap(upCursors[i]);
			rightCursor.AddBitmap(rightCursors[i]);
		}

		leftCursor.SetDelayCount(10);
		downCursor.SetDelayCount(10);
		upCursor.SetDelayCount(10);
		rightCursor.SetDelayCount(10);
	}

	void Cursor::OnMove(double bpms)
	{
		leftCursor.OnMove();
		rightCursor.OnMove();
		upCursor.OnMove();
		downCursor.OnMove();
		dy = (double)(420 / (4 * 60 * 60 / bpms));  //總移動時間為8個beat間隔，在8個beat間隔之前就要顯示note
		y -= dy;
	}

	void Cursor::OnShow(int noteType, int index, int player)
	{
		int i = 1;
		switch (noteType)
		{
		case NOTE_TYPE::NORMAL:
			switch (index)
			{
			case NOTE_INDEX::LEFT:
				leftCursor.SetTopLeft((int)x + player * 384, (int)y);
				leftCursor.OnShow();
				break;
			case NOTE_INDEX::DOWN:
				downCursor.SetTopLeft((int)x + player * 384, (int)y);
				downCursor.OnShow();
				break;
			case NOTE_INDEX::UP:
				upCursor.SetTopLeft((int)x + player * 384, (int)y);
				upCursor.OnShow();
				break;
			case NOTE_INDEX::RIGHT:
				rightCursor.SetTopLeft((int)x + player * 384, (int)y);
				rightCursor.OnShow();
				break;
			default:
				break;
			}
			break;
		case NOTE_TYPE::LONG_PRESS_1:
			switch (index)
			{
			case NOTE_INDEX::LEFT:
				while (y + i * leftHoldCursorBody.Height() < int(*botY))
				{
					leftHoldCursorBody.SetTopLeft((int)x + player * 384, (int)(y + leftHoldCursorBody.Height()*i));
					leftHoldCursorBody.ShowBitmap();
					i++;
				}
				leftHoldCursorBot.SetTopLeft((int)x + player * 384, (int)(*botY));
				leftHoldCursorBot.ShowBitmap();
				leftHoldCursorHead.SetTopLeft((int)x + player * 384, (int)y);
				leftHoldCursorHead.ShowBitmap();
				break;
			case NOTE_INDEX::DOWN:
				while (y + i * downHoldCursorBody.Height() < int(*botY))
				{
					downHoldCursorBody.SetTopLeft((int)x + player * 384, (int)(y + downHoldCursorBody.Height()*i));
					downHoldCursorBody.ShowBitmap();
					i++;
				}
				downHoldCursorBot.SetTopLeft((int)x + player * 384, (int)(*botY));
				downHoldCursorBot.ShowBitmap();
				downHoldCursorHead.SetTopLeft((int)x + player * 384, (int)y);
				downHoldCursorHead.ShowBitmap();
				break;
			case NOTE_INDEX::UP:
				while (y + i * upHoldCursorBody.Height() < int(*botY))
				{
					upHoldCursorBody.SetTopLeft((int)x + player * 384, (int)(y + upHoldCursorBody.Height()*i));
					upHoldCursorBody.ShowBitmap();
					i++;
				}
				upHoldCursorBot.SetTopLeft((int)x + player * 384, (int)(*botY));
				upHoldCursorBot.ShowBitmap();
				upHoldCursorHead.SetTopLeft((int)x + player * 384, (int)y);
				upHoldCursorHead.ShowBitmap();
				break;
			case NOTE_INDEX::RIGHT:
				while (y + i * rightHoldCursorBody.Height() < int(*botY))
				{
					rightHoldCursorBody.SetTopLeft((int)x + player * 384, (int)(y + rightHoldCursorBody.Height()*i));
					rightHoldCursorBody.ShowBitmap();
					i++;
				}
				rightHoldCursorBot.SetTopLeft((int)x + player * 384, (int)(*botY));
				rightHoldCursorBot.ShowBitmap();
				rightHoldCursorHead.SetTopLeft((int)x + player * 384, (int)y);
				rightHoldCursorHead.ShowBitmap();
				break;
			default:
				break;
			}
		case NOTE_TYPE::UNKNOWN:
			break;

		case NOTE_TYPE::LONG_PRESS_2:
			switch (index)
			{
			case NOTE_INDEX::LEFT:
				while (y + i * leftHoldCursorBody.Height() < int(*botY))
				{
					leftHoldCursorBody.SetTopLeft((int)x + player * 384, (int)(y + leftHoldCursorBody.Height()*i));
					leftHoldCursorBody.ShowBitmap();
					i++;
				}
				leftHoldCursorBot.SetTopLeft((int)x + player * 384, (int)(*botY));
				leftHoldCursorBot.ShowBitmap();
				leftHoldCursorHead.SetTopLeft((int)x + player * 384, (int)y);
				leftHoldCursorHead.ShowBitmap();
				break;
			case NOTE_INDEX::DOWN:
				while (y + i * downHoldCursorBody.Height() < int(*botY))
				{
					downHoldCursorBody.SetTopLeft((int)x + player * 384, (int)(y + downHoldCursorBody.Height()*i));
					downHoldCursorBody.ShowBitmap();
					i++;
				}
				downHoldCursorBot.SetTopLeft((int)x + player * 384, (int)(*botY));
				downHoldCursorBot.ShowBitmap();
				downHoldCursorHead.SetTopLeft((int)x + player * 384, (int)y);
				downHoldCursorHead.ShowBitmap();
				break;
			case NOTE_INDEX::UP:
				while (y + i * upHoldCursorBody.Height() < int(*botY))
				{
					upHoldCursorBody.SetTopLeft((int)x + player * 384, (int)(y + upHoldCursorBody.Height()*i));
					upHoldCursorBody.ShowBitmap();
					i++;
				}
				upHoldCursorBot.SetTopLeft((int)x + player * 384, (int)(*botY));
				upHoldCursorBot.ShowBitmap();
				upHoldCursorHead.SetTopLeft((int)x + player * 384, (int)y);
				upHoldCursorHead.ShowBitmap();
				break;
			case NOTE_INDEX::RIGHT:
				while (y + i * rightHoldCursorBody.Height() < int(*botY))
				{
					rightHoldCursorBody.SetTopLeft((int)x + player * 384, (int)(y + rightHoldCursorBody.Height()*i));
					rightHoldCursorBody.ShowBitmap();
					i++;
				}
				rightHoldCursorBot.SetTopLeft((int)x + player * 384, (int)(*botY));
				rightHoldCursorBot.ShowBitmap();
				rightHoldCursorHead.SetTopLeft((int)x + player * 384, (int)y);
				rightHoldCursorHead.ShowBitmap();
				break;
			default:
				break;
			}
		case NOTE_TYPE::MINE:
			mine.SetTopLeft((int)x + player * 384, (int)y);
			mine.ShowBitmap();
			break;
		case NOTE_TYPE::SP1:
			SP1.SetTopLeft((int)x + player * 64, (int)y);
			SP1.ShowBitmap();
			break;
		case NOTE_TYPE::SP2:
			SP2.SetTopLeft((int)x + player * 64, (int)y);
			SP2.ShowBitmap();
			break;
		case NOTE_TYPE::SP3:
			SP3.SetTopLeft((int)x + player * 64, (int)y);
			SP3.ShowBitmap();
			break;
		default:
			break;
		}
	}

	void Cursor::setBotY(double * yy)
	{
		botY = yy;
	}

	double* Cursor::getY()
	{
		return &y;
	}
}