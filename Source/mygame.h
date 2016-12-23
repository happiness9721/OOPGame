#pragma once
/*
 * mygame.h: 本檔案儲遊戲本身的class的interface
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

enum AUDIO_ID {				// 定義各種音效的編號
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
	// 這個class為遊戲的遊戲開頭畫面物件
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateInit : public CGameState {
	public:
		CGameStateInit(CGame *g);
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void SetPlayers(int);
	protected:
		void OnMove();
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		int processCounter; // 進入遊戲前選擇模式
		int selectIndex; // 選擇模式 0-單人 1-雙人
		int songIndex;
		int	timerCounter;
		int helpCounter; // 介紹頁面數
		CMovingBitmap  backgroundSelect[4];
		CMovingBitmap  modeSelect[2];
		CMovingBitmap  specialMode[2];
		CMovingBitmap  infoSelect[9];
		CMovingBitmap  background[2];
		bool	isHelping;
		CMovingBitmap  help[8];
		CAnimation	selectMode[2]; // 選擇
		CMovingBitmap	instruction[3]; // 提示

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
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
		void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
		void GetAllSongs();
		void ShowList();
		void SetBrowseCounter();
		void SetSongIndex(int);

		int  GetSongNum();
		int  GetSongIndex();


		string GetSongName(int);
		string GetSongPath(string);


	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面

	private:
		bool            keyMatchFlagUp;
		bool            keyMatchFlagDown;
		bool            keyMatchFlagLeft;
		bool            keyMatchFlagRight;
		bool			musicIsStart;
		bool            songChanged;
		bool			playBGM;
		bool			isSelected;

		string	tempString; //暫存歌曲名稱
		string	tempString1; //暫存歌曲名稱
		vector<string>     allName;
		vector<string>     allName1;
		vector<CMovingBitmap>     vecTitle; // 音樂圖片
		vector<CMovingBitmap>	  backgroundBMP; // 背景細部
		int				songNum;
		int				songIndex;
		int             tempSongIndex;
		int				browseCounter;
		int				timerCounter;
		int				animationCounter;

		int section_x;
		int section_y;

		CMovingBitmap   cursor; //游標
		CMovingBitmap	background[2];	// 背景圖
		static CMovingBitmap selectback[7];

		CAnimation   musicSelect; // 選到的歌
		CAnimation	 selected; // 選完歌
		CMovingBitmap	 instruction;

		CIntegerTimer   timer;

		static bool isBmpLoaded;

	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////


	class CGameStateRun : public CGameState {
	public:
		CGameStateRun(CGame *g);
		~CGameStateRun();
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();  								// 遊戲的初值及圖形設定
		void OnKeyDown(UINT, UINT, UINT);
		void OnKeyUp(UINT, UINT, UINT);
		void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
		void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
		void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
		void SetSongPath(string); // 設定路徑
		void SetSongIndex(int); //設定歌曲及人數
		void SetPlayers(int);
		void ClearMusic(); // 清除 music  
		void SetSpecialMode(bool); // 設定為特殊雙打 , true 開啟 , false 關閉
		bool judgeChanged(int, int);
		static CAnimation judgeBMP[2][3]; // 判定圖
		ScoreCount GetRecordInfo(int);
		Information      music[2];

	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面

	private:
		bool    keyMatchFlag[4];						//按鍵是否按下
		bool	fileExist;
		bool	musicIsStart;							//音樂是否撥放
		bool	mineFlag[2];							//是否有設置地雷
		bool	holdFlag[4];							//是否有跑到holdFlag			
		bool	isSpecialMode;							//是否為特殊雙打
		bool	SPfrost[2];								//是否開啟冰霜
		bool	SPflag[2][3];							//判斷是否有達到該種特殊效果的條件
		bool	SPshining[2];							//閃爍效果是否開啟
		bool    isInvincible; // 是否無敵
		bool    comboUnbreak; // 連擊數不斷
		void	ShowLifeProgress(int, int); // 生命條
		void	ShowTimeProgress(int);// 時間條


		vector<CMovingBitmap>	  backgroundBMP;
		CMovingBitmap   keyMatch[4]; //按鍵 0123
		CMovingBitmap   cursor; //游標
		CMovingBitmap	background;	// 背景圖
		CMovingBitmap	help;		// 說明圖
		CMovingBitmap	frost;
		static CMovingBitmap   combo[2]; //連擊

		CAnimation		right[2];
		CAnimation		down[2];
		CAnimation		up[2];
		CAnimation		left[2];
		CAnimation		SPcursor[2];
		CAnimation		ready;
		CAnimation		clear;
		CAnimation		firstPlace;

		string			songPath;    // 路徑
		Cursor          *cursor1;    // 游標陣列
		ScoreCount      allRecords1[2];

		int	num;       // 遊玩人數
		int songIndex; // 撥放歌曲
		int noteTime;			//放note的時間
		int judgeTime;			//判斷的時間
		int now;				//當下的時間
		int startRow[2];				//startRow到endRow的數目為畫面上所出現的行數
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

		//// 判定貼圖 ////
		int judge[2];
		static bool isBmpLoaded;
		//////////////////
	};

	/////////////////////////////////////////////////////////////////////////////
	// 這個class為遊戲的結束狀態(Game Over)
	// 每個Member function的Implementation都要弄懂
	/////////////////////////////////////////////////////////////////////////////

	class CGameStateOver : public CGameState {
	public:
		CGameStateOver(CGame *g);
		void OnBeginState();							// 設定每次重玩所需的變數
		void OnInit();
		void SetRecord(ScoreCount, int);
		void SetPlayers(int);
		void OnKeyDown(UINT, UINT, UINT);

	protected:
		void OnMove();									// 移動遊戲元素
		void OnShow();									// 顯示這個狀態的遊戲畫面
	private:
		int player;
		int counter;	// 倒數之計數器
		int animationCounter;
		CMovingBitmap   background[2];
		CMovingBitmap	failed;
		CMovingBitmap	fullCombo;
		CMovingBitmap	gradeBMP[2][7];
		CMovingBitmap	winner[3];

		CAnimation	 selected; // 選完歌
		CMovingBitmap	 instruction;

		CIntegerTimer	timer;
		vector<CMovingBitmap>	  backgroundBMP;
		ScoreCount allRecords2[2];

		int check[2];
	};
}
