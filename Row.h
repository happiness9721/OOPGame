#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "Cursor.h"

namespace game_framework {

	/////////////////////////////////////////////////////////////////////////////
	// 列游標
	/////////////////////////////////////////////////////////////////////////////
	class Row {
	public:
		Row();
		~Row();
		void SetTime(int);											//設定perfectTime
		int GetTime();												//取得perfectTime
		void OnMove(double);
		void OnAnimationMove();										//動畫
		void OnShow(int);
		void initialize(int);
		int CheckNoteType(int);										//取得是何種cursor
		void NoteClear(int);										//清掉cursor
		double* getY(int);											//取得儲存的cursor的y值
		void setY(double*, int);									//設定holdCursor的y
		void setMine(int index);									//設定地雷
		void setSP(int SPtype);										//設定特殊cursor

	private:
		int NoteType[5];											//儲存一行裡面有哪些cursor
		int perfectTime;
		vector <Cursor>CursorNote;            //0123  ←按鍵配置
		void SetCursor(int);
	};
}