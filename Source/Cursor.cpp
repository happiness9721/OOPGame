#include "StdAfx.h"
#include "Cursor.h"

namespace game_framework {
	
	//////////////////////////////////////////////////////////////////////////////
	// 游標 
	///////////////////////////////////////////////////////////////////////////////
	bool Cursor::isLoadBitmap = false;
	CAnimation Cursor::leftCursor;
	CAnimation Cursor::downCursor;
	CAnimation Cursor::upCursor;
	CAnimation Cursor::rightCursor;
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

	Cursor::Cursor()
	{
	}

	Cursor::~Cursor()
	{
	}
	void Cursor::Initalize(int position)
	{
		x = position;
		y = 480;
		dx = 0;
		dy = 0;

	}
	void Cursor::LoadBitmap()
	{
		if (!isLoadBitmap)
		{
			char s[2];
			char s2[80];
			for (int i = 1; i < 17; i++)
			{
				sprintf(s, "%d", i);
				strcpy(s2, "Bitmaps/cursor/leftCursor(");
				strcat(s2, s);
				strcat(s2, ").bmp");
				leftCursor.AddBitmap(s2, RGB(255, 255, 254));
				strcpy(s2, "Bitmaps/cursor/downCursor(");
				strcat(s2, s);
				strcat(s2, ").bmp");
				downCursor.AddBitmap(s2, RGB(255, 255, 254));
				strcpy(s2, "Bitmaps/cursor/upCursor(");
				strcat(s2, s);
				strcat(s2, ").bmp");
				upCursor.AddBitmap(s2, RGB(255, 255, 254));
				strcpy(s2, "Bitmaps/cursor/rightCursor(");
				strcat(s2, s);
				strcat(s2, ").bmp");
				rightCursor.AddBitmap(s2, RGB(255, 255, 254));
			}
			leftCursor.SetDelayCount(10);
			downCursor.SetDelayCount(10);
			upCursor.SetDelayCount(10);
			rightCursor.SetDelayCount(10);
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
	}
	void Cursor::OnMove(double bpms)
	{
		dy = (double)(420 / (4 * 60 * 60 / bpms));           //總移動時間為8個beat間隔，在8個beat間隔之前就要顯示note
		y -= dy;
	}
	void Cursor::OnAnimationMove()
	{
		leftCursor.OnMove();
		rightCursor.OnMove();
		upCursor.OnMove();
		downCursor.OnMove();
	}
	void Cursor::OnShow(int noteType, int index, int player)
	{
		int i = 1;
		switch (noteType)
		{
		case 1:
			switch (index)
			{
			case 0:
				leftCursor.SetTopLeft((int)x + player * 384, (int)y);
				leftCursor.OnShow();
				break;
			case 1:
				downCursor.SetTopLeft((int)x + player * 384, (int)y);
				downCursor.OnShow();
				break;
			case 2:
				upCursor.SetTopLeft((int)x + player * 384, (int)y);
				upCursor.OnShow();
				break;
			default:
				rightCursor.SetTopLeft((int)x + player * 384, (int)y);
				rightCursor.OnShow();
				break;
			}
			break;
		case 2:
			switch (index)
			{
			case 0:
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
			case 1:
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
			case 2:
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
			default:
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
			}
		case 3:
			break;
		case 4:
			switch (index)
			{
			case 0:
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
			case 1:
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
			case 2:
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
			default:
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
			}
		case 5:
			mine.SetTopLeft((int)x + player * 384, (int)y);
			mine.ShowBitmap();
			break;
		case 6:
			SP1.SetTopLeft((int)x + player * 64, (int)y);
			SP1.ShowBitmap();
			break;
		case 7:
			SP2.SetTopLeft((int)x + player * 64, (int)y);
			SP2.ShowBitmap();
			break;
		case 8:
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