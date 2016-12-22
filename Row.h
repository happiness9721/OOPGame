#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "Cursor.h"

namespace game_framework {

	/////////////////////////////////////////////////////////////////////////////
	// �C���
	/////////////////////////////////////////////////////////////////////////////
	class Row {
	public:
		Row();
		~Row();
		void SetTime(int);											//�]�wperfectTime
		int GetTime();												//���operfectTime
		void OnMove(double);
		void OnAnimationMove();										//�ʵe
		void OnShow(int);
		void initialize(int);
		int CheckNoteType(int);										//���o�O���cursor
		void NoteClear(int);										//�M��cursor
		double* getY(int);											//���o�x�s��cursor��y��
		void setY(double*, int);									//�]�wholdCursor��y
		void setMine(int index);									//�]�w�a�p
		void setSP(int SPtype);										//�]�w�S��cursor

	private:
		int NoteType[5];											//�x�s�@��̭�������cursor
		int perfectTime;
		vector <Cursor>CursorNote;            //0123  ������t�m
		void SetCursor(int);
	};
}