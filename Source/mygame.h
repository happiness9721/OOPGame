#pragma once
/*
 * mygame.h: ���ɮ��x�C��������class��interface
 * Copyright (C) 2002-2008 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
 *
 * This file is part of game, a free game development framework for windows.
 *
 * game is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * game is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *   2006-02-08 V4.2
 *      1. Rename OnInitialUpdate() -> OnInit().
 *      2. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      3. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress.
*/

/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

//#include <iostream>
#include <vector>
#include "CDDraw.h"
#include "CAnimation.h"
#include "CIntegerTimer.h"
#include "ScoreCount.h"
#include "Cursor.h"
#include "Information.h"

enum AUDIO_ID {				// �w�q�U�ح��Ī��s��
	AUDIO_SELECT,				// 0
	AUDIO_MUSIC1,				// 1
	AUDIO_MUSIC2,				// 2
	AUDIO_MUSIC3,   			    // 3
	AUDIO_MUSIC4,   			    // 4
	AUDIO_MENU,
	AUDIO_MENU1,
	AUDIO_RESULT,
	AUDIO_GAMEOVER,
	AUDIO_HELP,
	AUDIO_DIE,
	AUDIO_MINE,
	AUDIO_MINE2,
	AUDIO_FROST,
	AUDIO_SHINING
};

namespace game_framework {

	using namespace std;

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C���}�Y�e������
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// �C������Ȥιϧγ]�w
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT); 				// �B�z��LUp���ʧ@
		void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
		void SetPlayers(int);
	protected:
		void OnMove();
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
	private:
		int processCounter; // �i�J�C���e��ܼҦ�
		int selectIndex; // ��ܼҦ� 0-��H 1-���H
		int songIndex;
		int	timerCounter;
		int helpCounter; // ���Э�����
		CMovingBitmap  backgroundSelect[4];
		CMovingBitmap  modeSelect[2];
		CMovingBitmap  specialMode[2];
		CMovingBitmap  infoSelect[9];
		CMovingBitmap  background[2];
		bool	isHelping;
		CMovingBitmap  help[8];
		CAnimation	selectMode[2]; // ���
		CMovingBitmap	instruction[3]; // ����

		vector<CMovingBitmap>	  backgroundBMP;
		CIntegerTimer   timer;

	};

	/////////////////////////////////////////////////////////////////////////////
	// StateMenu
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateMenu : public CGameState {
	public:
		CGameStateMenu(CGame *g);
		~CGameStateMenu();
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnInit();  								// �C������Ȥιϧγ]�w
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
		void OnLButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
		void OnRButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
		void OnRButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
		void GetAllSongs();
		void ShowList();
		void SetBrowseCounter();
		void SetSongIndex(int);

		int  GetSongNum();
		int  GetSongIndex();


		string GetSongName(int);
		string GetSongPath(string);


	protected:
		void OnMove();									// ���ʹC������
		void OnShow();									// ��ܳo�Ӫ��A���C���e��

	private:
		bool            keyMatchFlagUp;
		bool            keyMatchFlagDown;
		bool            keyMatchFlagLeft;
		bool            keyMatchFlagRight;
		bool			musicIsStart;
		bool            songChanged;
		bool			playBGM;
		bool			isSelected;

		string	tempString; //�Ȧs�q���W��
		string	tempString1; //�Ȧs�q���W��
		vector<string>     allName;
		vector<string>     allName1;
		vector<CMovingBitmap>     vecTitle; // ���ֹϤ�
		vector<CMovingBitmap>	  backgroundBMP; // �I���ӳ�
		int				songNum;
		int				songIndex;
		int             tempSongIndex;
		int				browseCounter;
		int				timerCounter;
		int				animationCounter;

		int section_x;
		int section_y;

		CMovingBitmap   cursor; //���
		CMovingBitmap	background[2];	// �I����
		static CMovingBitmap selectback[7];

		CAnimation   musicSelect; // ��쪺�q
		CAnimation	 selected; // �粒�q
		CMovingBitmap	 instruction;

		CIntegerTimer   timer;

		static bool isBmpLoaded;

	};

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////


	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnInit();  								// �C������Ȥιϧγ]�w
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
		void OnLButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
		void OnRButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
		void OnRButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
		void SetSongPath(string); // �]�w���|
		void SetSongIndex(int); //�]�w�q���ΤH��
		void SetPlayers(int);
		void ClearMusic(); // �M�� music  
		void SetSpecialMode(bool); // �]�w���S������ , true �}�� , false ����
		bool judgeChanged(int, int);
		static CAnimation judgeBMP[2][3]; // �P�w��
		ScoreCount GetRecordInfo(int);
		Information      music[2];

	protected:
		void OnMove();									// ���ʹC������
		void OnShow();									// ��ܳo�Ӫ��A���C���e��

	private:
		bool    keyMatchFlag[4];						//����O�_���U
		bool	fileExist;
		bool	musicIsStart;							//���֬O�_����
		bool	mineFlag[2];							//�O�_���]�m�a�p
		bool	holdFlag[4];							//�O�_���]��holdFlag			
		bool	isSpecialMode;							//�O�_���S������
		bool	SPfrost[2];								//�O�_�}�ҦB��
		bool	SPflag[2][3];							//�P�_�O�_���F��ӺدS��ĪG������
		bool	SPshining[2];							//�{�{�ĪG�O�_�}��
		bool    isInvincible; // �O�_�L��
		bool    comboUnbreak; // �s���Ƥ��_
		void	ShowLifeProgress(int, int); // �ͩR��
		void	ShowTimeProgress(int);// �ɶ���


		vector<CMovingBitmap>	  backgroundBMP;
		CMovingBitmap   keyMatch[4]; //���� 0123
		CMovingBitmap   cursor; //���
		CMovingBitmap	background;	// �I����
		CMovingBitmap	help;		// ������
		CMovingBitmap	frost;
		static CMovingBitmap   combo[2]; //�s��

		CAnimation		right[2];
		CAnimation		down[2];
		CAnimation		up[2];
		CAnimation		left[2];
		CAnimation		SPcursor[2];
		CAnimation		ready;
		CAnimation		clear;
		CAnimation		firstPlace;

		string			songPath;    // ���|
		Cursor          *cursor1;    // ��а}�C
		ScoreCount      allRecords1[2];

		int	num;       // �C���H��
		int songIndex; // ����q��
		int noteTime;			//��note���ɶ�
		int judgeTime;			//�P�_���ɶ�
		int now;				//��U���ɶ�
		int startRow[2];				//startRow��endRow���ƥج��e���W�ҥX�{�����
		int holdRowFlag[2][4];
		int holdRow[2];
		int endRow[2];
		int judgeRow[2];
		int OverCounter;
		int PercentageCounter;
		int counter;
		int animationCounter[2];
		int delay;
		int frostCount[2];
		int shiningCount[2];

		//// �P�w�K�� ////
		int judge[2];
		static bool isBmpLoaded;
		//////////////////
	};

	/////////////////////////////////////////////////////////////////////////////
	// �o��class���C�����������A(Game Over)
	// �C��Member function��Implementation���n����
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// �]�w�C�������һݪ��ܼ�
		void OnInit();
		void SetRecord(ScoreCount, int);
		void SetPlayers(int);
		void OnKeyDown(UINT, UINT, UINT);

	protected:
		void OnMove();									// ���ʹC������
		void OnShow();									// ��ܳo�Ӫ��A���C���e��
	private:
		int player;
		int counter;	// �˼Ƥ��p�ƾ�
		int animationCounter;
		CMovingBitmap   background[2];
		CMovingBitmap	failed;
		CMovingBitmap	fullCombo;
		CMovingBitmap	gradeBMP[2][7];
		CMovingBitmap	winner[3];

		CAnimation	 selected; // �粒�q
		CMovingBitmap	 instruction;

		CIntegerTimer	timer;
		vector<CMovingBitmap>	  backgroundBMP;
		ScoreCount allRecords2[2];

		int check[2];
	};
}
