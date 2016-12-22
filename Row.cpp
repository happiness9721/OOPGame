#include "Source\StdAfx.h"
#include "Row.h"

namespace game_framework {

	/////////////////////////////////////////////////////////////////////////////
	// ¦C´å¼Ð
	/////////////////////////////////////////////////////////////////////////////

	Row::Row()
	{
	}

	Row::~Row()
	{
	}
	void Row::initialize(int SetNote)
	{
		Cursor cs;
		for (int i = 0; i < 5; i++)
			CursorNote.push_back(cs);
		for (int i = 3; i >= 0; i--)
		{
			NoteType[i] = SetNote % 10;
			SetCursor(i);
			SetNote = SetNote / 10;
		}
		NoteType[4] = 0;
		SetCursor(4);
	}
	void Row::SetCursor(int i) {
		CursorNote[i].LoadBitmap();
		CursorNote[i].Initalize(i * 64);
	}
	void Row::OnShow(int player) {
		for (int i = 0; i < 5; i++)
		{
			CursorNote[i].OnShow(NoteType[i], i, player);
		}
	}
	void Row::OnMove(double bpms) {
		for (int i = 0; i < 5; i++)
		{
			CursorNote[i].OnMove(bpms);
		}
	}

	void Row::OnAnimationMove()
	{
		CursorNote[0].OnAnimationMove();
	}

	int Row::CheckNoteType(int i)
	{
		return NoteType[i];
	}

	void Row::NoteClear(int i)
	{
		if (NoteType[i] == 1 || NoteType[i] == 5)
			NoteType[i] = 0;
		if (NoteType[i] == 2)
			NoteType[i] = 4;
	}

	void Row::SetTime(int time)
	{
		perfectTime = time;

	}

	int Row::GetTime()
	{
		return perfectTime;
	}

	void Row::setSP(int SPtype)
	{
		NoteType[4] = SPtype + 5;
	}

	double* Row::getY(int index)
	{
		return CursorNote[index].getY();
	}

	void  Row::setY(double* y, int index)
	{
		CursorNote[index].setBotY(y);
	}

	void Row::setMine(int index)
	{
		NoteType[index] = 5;
	}
}