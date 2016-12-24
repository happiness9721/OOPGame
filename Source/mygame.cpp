/*
 * mygame.cpp: 本檔案儲遊戲本身的class的implementation
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
 * History:
 *   2002-03-04 V3.1
 *          Add codes to demostrate the use of CMovingBitmap::ShowBitmap(CMovingBitmap &).
 *	 2004-03-02 V4.0
 *      1. Add CGameStateInit, CGameStateRun, and CGameStateOver to
 *         demonstrate the use of states.
 *      2. Demo the use of CInteger in CGameStateRun.
 *   2005-09-13
 *      Rewrite the codes for CBall and CEraser.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add codes to display IDC_GAMECURSOR in GameStateRun.
 *   2006-02-08 V4.2
 *      1. Revise sample screens to display in English only.
 *      2. Add code in CGameStateInit to demo the use of PostQuitMessage().
 *      3. Rename OnInitialUpdate() -> OnInit().
 *      4. Fix the bug that OnBeginState() of GameStateInit is not called.
 *      5. Replace AUDIO_CANYON as AUDIO_NTUT.
 *      6. Add help bitmap to CGameStateRun.
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *   2006-12-30
 *      1. Bug fix: fix a memory leak problem by replacing PostQuitMessage(0) as
 *         PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0).
 *   2008-02-15 V4.4
 *      1. Add namespace game_framework.
 *      2. Replace the demonstration of animation as a new bouncing ball.
 *      3. Use ShowInitProgress(percent) to display loading progress. 
 *   2010-03-23 V4.6
 *      1. Demo MP3 support: use lake.mp3 to replace lake.wav.
*/

#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "gamelib.h"
#include "mygame.h"
#include <fstream>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <string>
#include "audio.h"

//#include <vector>

namespace game_framework {
using namespace std ;


/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame *g)
: CGameState(g)
{
}


void CGameStateInit::OnMove()
{
	/*
	if( isHelping )
	{
		if( !help.IsFinalBitmap() ) help.OnMove() ;
	}
	*/
	if( processCounter == 1 )
	{
		selectMode[0].OnMove();
		selectMode[0].SetTopLeft(150+((selectIndex-1)*200) , 300) ;
		if( timerCounter > 0 )
		{
			timer.SetInteger(timerCounter/60);
			timerCounter--;
		}
		if( timerCounter == 0 )
		{
			SetPlayers( selectIndex ) ;
			if( selectIndex == 1 )
			{
				CAudio::Instance()->Stop( AUDIO_MENU1 );
				GotoGameState(GAME_STATE_MENU);
			}
			timerCounter = 100*60 ;
			selectIndex = 1 ;
			processCounter++ ;
		}
	}
	if( processCounter == 2 )
	{		
		selectMode[1].OnMove();
		selectMode[1].SetTopLeft(170+((selectIndex-1)*200) , 300) ;
		if( timerCounter > 0 )
		{
			timer.SetInteger(timerCounter/60);
			timerCounter--;
		}
		if( timerCounter == 0 )
		{
			CAudio::Instance()->Stop( AUDIO_MENU1 );
			if( selectIndex == 2 )
			{
				game->gameStateTable[GAME_STATE_RUN]->SetSpecialMode( true );
			}
			else
			{
				game->gameStateTable[GAME_STATE_RUN]->SetSpecialMode( false );
			}
			GotoGameState(GAME_STATE_MENU);
		}		
	}
	
}


void CGameStateInit::OnInit()
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(0);	// 一開始的loading進度為0%
	//

	background[0].LoadBitmap("Bitmaps/initback.bmp", RGB(0,0,0)) ;
	background[0].SetTopLeft(0,0) ;
	background[1].LoadBitmap("Bitmaps/Background.bmp", RGB(0,0,0)) ;
	background[1].SetTopLeft(0,0) ;

	backgroundSelect[0].LoadBitmap("Bitmaps/menu/selection1.bmp" , RGB(0,0,0));
	backgroundSelect[1].LoadBitmap("Bitmaps/menu/selection2.bmp", RGB(0,0,0));
	backgroundSelect[2].LoadBitmap("Bitmaps/menu/selectied1.bmp", RGB(0,0,0));
	backgroundSelect[3].LoadBitmap("Bitmaps/menu/selectied2.bmp", RGB(0,0,0));
	backgroundSelect[0].SetTopLeft(20,70);
	backgroundSelect[1].SetTopLeft(20,91);
	backgroundSelect[2].SetTopLeft(20,70);
	backgroundSelect[3].SetTopLeft(20,91);

	CAudio::Instance()->Load(AUDIO_MENU1,  "Sounds/menumusic.mp3");	
	CAudio::Instance()->Load(AUDIO_HELP,  "Sounds/helpmusic.mp3");
	CAudio::Instance()->Load(AUDIO_MINE, "Sounds/mine1.wav");
	CAudio::Instance()->Load(AUDIO_MINE2, "Sounds/mine2.wav");
	CAudio::Instance()->Load(AUDIO_FROST, "Sounds/frost.wav");
	CAudio::Instance()->Load(AUDIO_SHINING, "Sounds/shining.wav");

	// 模式選擇/////////////////////
	modeSelect[0].LoadBitmap("Bitmaps/menu/single.bmp" , RGB(255,255,255)) ; // 單人
	modeSelect[1].LoadBitmap("Bitmaps/menu/versus.bmp", RGB(255,255,255)) ; // 雙人
	specialMode[0].LoadBitmap("Bitmaps/menu/normal.bmp", RGB(255,255,255)) ; // 雙人一般
	specialMode[1].LoadBitmap("Bitmaps/menu/special.bmp", RGB(255,255,255)) ; //雙人特別
	infoSelect[0].LoadBitmap("Bitmaps/menu/singleInfo.bmp", RGB(0,0,0)) ; // 單人說明
	infoSelect[1].LoadBitmap("Bitmaps/menu/versusInfo.bmp", RGB(0,0,0));  // 雙人說明
	infoSelect[2].LoadBitmap("Bitmaps/menu/ScreenSelectStylepicture1.bmp", RGB(0,0,0)) ; // 單人圖片
	infoSelect[3].LoadBitmap("Bitmaps/menu/ScreenSelectStylepicture2.bmp", RGB(0,0,0));  // 雙人圖片
	infoSelect[4].LoadBitmap("Bitmaps/menu/normalInfo.bmp", RGB(0,0,0)) ; // 對戰說明
	infoSelect[5].LoadBitmap("Bitmaps/menu/specialInfo.bmp", RGB(0,0,0));  // 特殊說明
	infoSelect[6].LoadBitmap("Bitmaps/menu/normalPic.bmp", RGB(0,0,0)) ; // 對戰圖片
	infoSelect[7].LoadBitmap("Bitmaps/menu/specialPic.bmp", RGB(0,0,0));  // 特殊圖片
	infoSelect[8].LoadBitmap("Bitmaps/menu/ScreenSelectStyleexplanation.bmp", RGB(0,0,0)); 
	modeSelect[0].SetTopLeft(150,300);
	modeSelect[1].SetTopLeft(350,300);
	specialMode[0].SetTopLeft(170,300);
	specialMode[1].SetTopLeft(370,300);
	infoSelect[0].SetTopLeft(275,135);
	infoSelect[1].SetTopLeft(275,135);
	infoSelect[2].SetTopLeft(20,100);
	infoSelect[3].SetTopLeft(20,100);
	infoSelect[4].SetTopLeft(235,140);
	infoSelect[5].SetTopLeft(235,140);
	infoSelect[6].SetTopLeft(20,130);
	infoSelect[7].SetTopLeft(20,130);
	infoSelect[8].SetTopLeft(270,107);
	////////////////////////////////////

	// 背景圖 /////////////////////
	for( int p = 0 ; p < 7 ; p++ )
	{
		CMovingBitmap bmp ;
		switch(p)
		{
			case 0:
				bmp.LoadBitmap("Bitmaps/Header/header_left.bmp" ,RGB(0,0,0)) ;
				bmp.SetTopLeft(0,0) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 1:
				bmp.LoadBitmap("Bitmaps/Header/header_right.bmp" ,RGB(0,0,0)) ;
				bmp.SetTopLeft(300,0) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 2:
				bmp.LoadBitmap("Bitmaps/Header/header_top.bmp" ,RGB(255,255,255)) ;
				bmp.SetTopLeft(0,0) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 3:
				bmp.LoadBitmap("Bitmaps/Header/timeremain.bmp" ,RGB(255,255,255)) ;
				bmp.SetTopLeft(550,10) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 4:
				bmp.LoadBitmap("Bitmaps/Header/footer.bmp" ,RGB(255,255,255)) ;
				bmp.SetTopLeft(0,440) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 5:
				bmp.LoadBitmap("Bitmaps/menu/modeback1.bmp" ,RGB(0,0,0)) ;
				bmp.SetTopLeft(0,100) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 6:
				bmp.LoadBitmap("Bitmaps/menu/modeback2.bmp" ,RGB(0,0,0)) ;
				bmp.SetTopLeft(15,15) ;
				backgroundBMP.push_back(bmp) ;
				break ;
		}
	}
	
	selectMode[0].AddBitmap("Bitmaps/menu/modeSelected1.bmp",RGB(255,255,255));
	selectMode[0].AddBitmap("Bitmaps/menu/modeSelected2.bmp",RGB(255,255,255));
	selectMode[0].SetTopLeft(150,300);
	selectMode[0].SetDelayCount(20);
	selectMode[1].AddBitmap("Bitmaps/menu/specialSelected1.bmp",RGB(255,255,255));
	selectMode[1].AddBitmap("Bitmaps/menu/specialSelected2.bmp",RGB(255,255,255));
	selectMode[1].SetTopLeft(170,300);
	selectMode[1].SetDelayCount(20);

	instruction[0].LoadBitmap("Bitmaps/instruction1.bmp",RGB(0,0,0));
	instruction[1].LoadBitmap("Bitmaps/help.bmp",RGB(0,0,0));
	instruction[2].LoadBitmap("Bitmaps/cheat.bmp",RGB(0,0,0));
	instruction[0].SetTopLeft(81,400);
	instruction[1].SetTopLeft(81,400);
	instruction[2].SetTopLeft(0,380);

	for (int i = 1 ; i < 9; i++)
	{
		char str[80];				
		sprintf(str, "Bitmaps/help/help%d.bmp", i );
		help[i-1].LoadBitmap(str) ;
		help[i-1].SetTopLeft(0,0) ;
	}
	
	isHelping = false ;

	////////////////////////////////////

	timer.LoadBitmap();
	timer.SetInteger(99) ;
	timer.SetTopLeft(565,20);
}

void CGameStateInit::OnBeginState()
{
	processCounter = 0 ;
	songIndex = 0 ;
	selectIndex = 1 ;
	timerCounter = 100*60 ;
	helpCounter = 0 ;
	game->gameStateTable[GAME_STATE_RUN]->SetSpecialMode(false);
}

void CGameStateInit::SetPlayers( int index )
{
	game->gameStateTable[GAME_STATE_RUN]->SetPlayers( index ) ;
	game->gameStateTable[GAME_STATE_OVER]->SetPlayers( index ) ;
}

void CGameStateInit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	const char KEY_LEFT  = 0x25; // keyboard左箭頭
	const char KEY_UP    = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN  = 0x28; // keyboard下箭頭
	const char KEY_SPACE = ' ';
	const char KEY_ENTER = 0xD;
	const char KEY_ESC = 0x1B;
	const char KEY_H = 'H';
	
	if (nChar == KEY_DOWN || nChar == 'X')
	{
		if( songIndex == 1 )
		{
			songIndex = 0 ;
		}
		else if( songIndex == 0  )
		{
			songIndex = 1 ;
		}

	}
	if (nChar == KEY_UP || nChar == 'C')
	{
		songIndex = songIndex == 1?0:1;
		//if(  songIndex == 1  )
		//{
		//	songIndex = 0 ;
		//}
		//else if( songIndex == 0 )
		//{
		//	songIndex = 1 ;
		//}

    }

	if (nChar == KEY_RIGHT )
	{
		if( isHelping && helpCounter < 7 ) helpCounter++ ;
		if( processCounter == 1 || processCounter == 2 )
		{
			if( selectIndex == 2 )
			{
				selectIndex = 1 ;
			}
			else if( selectIndex == 1  )
			{
				selectIndex = 2 ;
			}
		}

	}
	if (nChar == KEY_LEFT  )
	{
		if( isHelping && helpCounter > 0 ) helpCounter-- ;
		if( processCounter == 1 || processCounter == 2 )
		{
			if( selectIndex == 2 )
			{
				selectIndex = 1 ;
			}
			else if( selectIndex == 1  )
			{
				selectIndex = 2 ;
			}
		}

	}
	
	if (nChar == KEY_H && processCounter == 0 )
	{
		isHelping = !isHelping ;
		if( !isHelping )
		{
			CAudio::Instance()->Stop( AUDIO_HELP );
			helpCounter = 0 ;
		}
		else
		{
			CAudio::Instance()->Play( AUDIO_HELP , true );
		}
	}
	
	if (nChar == KEY_ENTER && songIndex != 1 && !isHelping )
	{
		if( processCounter == 1 )
		{
			SetPlayers( selectIndex ) ;
			if( selectIndex == 1 )
			{
				CAudio::Instance()->Stop( AUDIO_MENU1 );
				GotoGameState(GAME_STATE_MENU);
			}
			processCounter++ ;
			selectIndex = 1 ;
			timerCounter = 100*60 ;
		}
		else if( processCounter == 2 )
		{
			if( selectIndex == 2 ) 
			{
				game->gameStateTable[GAME_STATE_RUN]->SetSpecialMode(true) ;
			}
			else
			{
				game->gameStateTable[GAME_STATE_RUN]->SetSpecialMode(false);
			}
			CAudio::Instance()->Stop( AUDIO_MENU1 );
			GotoGameState(GAME_STATE_MENU);	
		}
		else{
			CAudio::Instance()->Play( AUDIO_MENU1 , true);
			processCounter++ ;
		}
	}


	if (nChar == KEY_ENTER && songIndex == 1 && !isHelping )								// Demo 關閉遊戲的方法
	PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE,0,0);	// 關閉遊戲


	if (nChar == KEY_ESC && !isHelping )
	{
		if( processCounter == 1 || processCounter == 2 )
		{
			CAudio::Instance()->Stop( AUDIO_MENU1 );
			if( processCounter == 1 )
			{
				processCounter-- ;
			}
			else
			{
				processCounter-=2 ;
			}
			selectIndex = 1 ;
		}
	}

}
void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_ESC = 27;
	const char KEY_SPACE = ' ';

}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point)
{

}

void CGameStateInit::OnShow()
{

	switch(processCounter){
		case 0 :
			background[0].ShowBitmap();
			backgroundSelect[0].ShowBitmap();
			backgroundSelect[1].ShowBitmap();
			backgroundSelect[songIndex+2].ShowBitmap();
			instruction[1].ShowBitmap();
			instruction[2].ShowBitmap();
			if( isHelping ) help[helpCounter].ShowBitmap();
			break ;
		case 1 :
			background[1].ShowBitmap();
			for( int i = 0 ; i < 7 ; i++ )
			{
				backgroundBMP[i].ShowBitmap();
			}
			infoSelect[8].ShowBitmap();
			timer.ShowBitmap();
			modeSelect[0].ShowBitmap();
			modeSelect[1].ShowBitmap();
			selectMode[0].OnShow();
			infoSelect[selectIndex-1].ShowBitmap();
			infoSelect[selectIndex+1].ShowBitmap();
			instruction[0].ShowBitmap();
			break ;
		case 2 :
			background[1].ShowBitmap();
			for( int i = 0 ; i < 7 ; i++ )
			{
				backgroundBMP[i].ShowBitmap();
			}
			infoSelect[8].ShowBitmap();
			timer.ShowBitmap();
			specialMode[0].ShowBitmap();
			specialMode[1].ShowBitmap();
			selectMode[1].OnShow();
			if( processCounter == 2 )infoSelect[selectIndex+3].ShowBitmap();
			if( processCounter == 2 )infoSelect[selectIndex+5].ShowBitmap();
			instruction[0].ShowBitmap();
			break ;		
	}


	/*
	CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CFont f,*fp;
	f.CreatePointFont(160,"Times New Roman");	// 產生 font f; 160表示16 point的字
	fp=pDC->SelectObject(&f);					// 選用 font f
	char str[80] ;
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255,255,0));
//	pDC->TextOut(120,320,"IIDX Beatmania Design.");
	pDC->SetBkMode(0); // 使字體背景透明
	sprintf(str, "%d ", selectIndex );
	pDC->TextOut(400,70,str);

	pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	*/
}								

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame *g)
: CGameState(g)
{
}

void CGameStateOver::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_SPACE = ' ';
	const char KEY_ENTER = 0xD;
	
	if (nChar == KEY_ENTER)
	{
		CAudio::Instance()->Stop(AUDIO_RESULT) ;
		game->gameStateTable[GAME_STATE_RUN]->ClearMusic();
		if( allRecords2[0].isDead == true )
		{
			GotoGameState(GAME_STATE_INIT);
		}
		else{
			GotoGameState(GAME_STATE_MENU);
		}
	}
}
void CGameStateOver::OnMove()
{	
	if( animationCounter == 0 )
	{
		if (counter > 0)
		{
			timer.SetInteger(counter/60) ;
			counter-- ;
		}
		else{
			CAudio::Instance()->Stop(AUDIO_RESULT) ;
			game->gameStateTable[GAME_STATE_RUN]->ClearMusic();
			if( allRecords2[0].isDead == true )
			{
				GotoGameState(GAME_STATE_INIT);
			}
			else
			{
				GotoGameState(GAME_STATE_MENU);
			}
		}
	}
	else
	{
		selected.OnMove();
		animationCounter-- ;
	}
}

void CGameStateOver::SetRecord(ScoreCount tempRecords , int player )
{
	allRecords2[player] = tempRecords ;
}

void CGameStateOver::SetPlayers( int index )
{
	player = index ;
}

void CGameStateOver::OnBeginState()
{
	counter =  30 * 60; // 30 seconds
	timer.SetInteger(counter/60);

	SetRecord( (game->GetScoreInfo(0)) , 0 ) ;
	if( player == 2 )SetRecord( (game->GetScoreInfo(1)) , 1 ) ;

	for(int i = 0 ; i < player ; i++ )
	{
		allRecords2[i].scoreBMP.SetInteger((int)allRecords2[i].getScore()) ;
		allRecords2[i].scoreBMP.SetTopLeft(20+(i*400), 305);
		check[i] = allRecords2[i].checkGrade();
	}

	for( int i = 0 ; i < player ; i++ )
	{
		allRecords2[i].judge[0].SetInteger(allRecords2[i].perfectTimes);
		allRecords2[i].judge[0].ChangeDigit(3);
		allRecords2[i].judge[0].SetTopLeft(195+(i*178) ,125);
		allRecords2[i].judge[1].SetInteger(allRecords2[i].goodTimes);
		allRecords2[i].judge[1].ChangeDigit(3);
		allRecords2[i].judge[1].SetTopLeft(195+(i*178) ,163);
		allRecords2[i].judge[2].SetInteger(allRecords2[i].missTimes);
		allRecords2[i].judge[2].ChangeDigit(3);
		allRecords2[i].judge[2].SetTopLeft(195+(i*178) , 201);
		allRecords2[i].judge[3].SetInteger(allRecords2[i].maxCombo);
		allRecords2[i].judge[3].ChangeDigit(3);
		allRecords2[i].judge[3].SetTopLeft(130+(i*310) , 352);
	}
	CAudio::Instance()->Play(AUDIO_RESULT,true);
	animationCounter = 3*60 ;
	selected.Reset();
}


void CGameStateOver::OnInit()
{

	background[0].LoadBitmap("Bitmaps/Background.bmp") ;
	background[1].LoadBitmap("Bitmaps/black.bmp") ;
	background[0].SetTopLeft(0,0);
	background[1].SetTopLeft(0,0) ;

	winner[0].LoadBitmap("Bitmaps/grade/win.bmp" , RGB(0,0,0)) ;
	winner[1].LoadBitmap("Bitmaps/grade/lose.bmp" , RGB(0,0,0)) ;
	winner[2].LoadBitmap("Bitmaps/grade/draw.bmp" , RGB(0,0,0)) ;

	failed.LoadBitmap("Bitmaps/failed.bmp" , RGB(0,0,0));
    fullCombo.LoadBitmap("Bitmaps/fullcombo.bmp" , RGB(255,255,255));

	CAudio::Instance()->Load(AUDIO_RESULT,  "Sounds/result.mp3");	
	CAudio::Instance()->Load(AUDIO_GAMEOVER,  "Sounds/GameOvermusic.mp3");	

	timer.LoadBitmap();
	timer.SetTopLeft(565,20);

	for (int i = 4 ; i > 0; i--)
	{
		char str[80];				
		sprintf(str, "Bitmaps/loadbar%d.bmp", i );
		selected.AddBitmap(str,RGB(0,0,0));
	}
	selected.SetTopLeft(0,220);
	selected.SetDelayCount(60);

	for( int p = 0 ; p < 11 ; p++ )
	{
		CMovingBitmap bmp ;
		switch(p)
		{
			case 0:
				bmp.LoadBitmap("Bitmaps/Header/header_left.bmp" ,RGB(0,0,0)) ;
				bmp.SetTopLeft(0,0) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 1:
				bmp.LoadBitmap("Bitmaps/Header/header_right.bmp" ,RGB(0,0,0)) ;
				bmp.SetTopLeft(300,0) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 2:
				bmp.LoadBitmap("Bitmaps/Header/header_top.bmp" ,RGB(255,255,255)) ;
				bmp.SetTopLeft(0,0) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 3:
				bmp.LoadBitmap("Bitmaps/Header/timeremain.bmp" ,RGB(255,255,255)) ;
				bmp.SetTopLeft(550,10) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 4:
				bmp.LoadBitmap("Bitmaps/Header/footer.bmp" ,RGB(255,255,255)) ;
				bmp.SetTopLeft(0,440) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 5:
				bmp.LoadBitmap("Bitmaps/result.bmp" ,RGB(0,0,0)) ;
				bmp.SetTopLeft(190,120) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 6:
				bmp.LoadBitmap("Bitmaps/scoreBar.bmp" ,RGB(255,255,255)) ;
				bmp.SetTopLeft(180,300) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 7:
				bmp.LoadBitmap("Bitmaps/gradeFramePlayer1.bmp" ,RGB(0,0,0)) ;
				bmp.SetTopLeft(0,80) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 8:
				bmp.LoadBitmap("Bitmaps/gradeFramePlayer2.bmp" ,RGB(0,0,0)) ;
				bmp.SetTopLeft(448,80) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 9:
				bmp.LoadBitmap("Bitmaps/maxComboPlayer1.bmp" ,RGB(255,255,255)) ;
				bmp.SetTopLeft(30,350) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 10:
				bmp.LoadBitmap("Bitmaps/maxComboPlayer2.bmp" ,RGB(255,255,255)) ;
				bmp.SetTopLeft(435,350) ;
				backgroundBMP.push_back(bmp) ;
				break ;
		}
	}

	
	for(int i = 0 ; i < 2 ; i++ )
	{
		for( int j = 0 ; j < 7 ; j++ )
		{
			char str[80];				
			sprintf(str, "Bitmaps/grade/%d.bmp", j );
			gradeBMP[i][j].LoadBitmap(str, RGB(0,0,0) ) ;
			gradeBMP[i][j].SetTopLeft(30+(i*460),125) ;
		}
		check[i] = 0 ;
	}
	
	instruction.LoadBitmap("Bitmaps/instruction3.bmp",RGB(0,0,0));
	instruction.SetTopLeft(80,401);

}

void CGameStateOver::OnShow()
{

	background[0].ShowBitmap();

	if( animationCounter > 0 ) 
	{
		background[1].ShowBitmap();
		selected.OnShow();
		if( allRecords2[0].isDead )
		{
			failed.SetTopLeft( 180 ,300 ) ;
			failed.ShowBitmap();
		}
		if( allRecords2[1].isDead && player == 2)
		{
			failed.SetTopLeft( 180 ,300 ) ;
			failed.ShowBitmap();
		}
	}
	else
	{
		for(int i = 0 ; i < (int)backgroundBMP.size() ; i++ )
		{ 
			backgroundBMP[i].ShowBitmap();
		}
	
		if( allRecords2[0].missTimes == 0 )
		{
			fullCombo.SetTopLeft( 0 ,325 ) ;
			fullCombo.ShowBitmap();
		}
		if( allRecords2[1].missTimes == 0 && player == 2)
		{
			fullCombo.SetTopLeft( 400 ,325 ) ;
			fullCombo.ShowBitmap();
		}
	
		for(int j = 0 ; j < player ; j++ )
		{
			for(int i = 0 ; i < 4 ; i++ )
			{ 
				allRecords2[j].judge[i].ShowBitmap();
			}
		
			if( allRecords2[0].isDead == true  )
			{
				gradeBMP[j][6].ShowBitmap();
			}
			else{
				gradeBMP[j][check[j]].ShowBitmap() ;
			}
			allRecords2[j].scoreBMP.ShowBitmap();
		
		}
		if( player == 2 )
		{
			if( allRecords2[0].getScore() > allRecords2[1].getScore() ) 
			{
				winner[0].SetTopLeft( 50 , 245 ) ;
				winner[1].SetTopLeft( 470 , 245 ) ;
				winner[0].ShowBitmap() ;
				winner[1].ShowBitmap() ;
			}
			else if( allRecords2[1].getScore() > allRecords2[0].getScore() )
			{
				winner[1].SetTopLeft( 50 , 245 ) ;
				winner[0].SetTopLeft( 470 , 245 ) ;
				winner[0].ShowBitmap() ;
				winner[1].ShowBitmap() ;
			}
			else
			{
				winner[2].SetTopLeft( 266 , 245 ) ;
				winner[2].ShowBitmap() ;
			}

		}
		instruction.ShowBitmap();
		timer.ShowBitmap();
	}


	/*
	CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CFont f,*fp;
	f.CreatePointFont(100,"Times New Roman");	// 產生 font f; 160表示16 point的字
	fp=pDC->SelectObject(&f);					// 選用 font f
	pDC->SetBkMode(0);
	pDC->SetTextColor(RGB(255,255,0));
	char str[80];								// Demo 數字對字串的轉換
	
	sprintf(str, "perfect : %d", allRecords2[0].perfectTimes );
	pDC->TextOut(20,410,str);
	sprintf(str, "goode: %d", allRecords2[0].goodTimes );
	pDC->TextOut(20,440,str);
	sprintf(str, "miss : %d", allRecords2[0].missTimes );
	pDC->TextOut(20,470,str);
	
	sprintf(str, "grade : %d", check[0]  );
	pDC->TextOut(200,400,str);
	if( player == 2 )
	{
		sprintf(str, "grade : %d", check[1]  );
		pDC->TextOut(280,400,str);
	}
	
	pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
	CDDraw::ReleaseBackCDC();	// 放掉 Back Plain 的 CDC
	*/	
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame *g)
: CGameState(g) , delay(10)
{

}

CGameStateRun::~CGameStateRun()
{
	//ClearMusic();

}

void CGameStateRun::OnBeginState()
{
	for(int i = 0 ; i < num ; i++ )
		music[i].readFile( songPath.c_str() ) ;
	for(int i = 0 ; i < num ; i++ )
	{
		allRecords[i].initialize();
		OverCounter = (int)( music[i].GetTotalTime()*60)+(1*60) ;
	}
	for(int j = 0 ; j < num ; j++ )
	{
		for(int i = 0 ; i < 4 ; i++ )
		{
			keyMatchFlag[i] = 0 ;
			holdFlag[i] = true;
			holdRowFlag[j][i] = 0;
			SPflag[j][i] = false;
		}

		endRow[j] = startRow[j] = judgeRow[j] = 0;
		musicIsStart = false;
		holdRow[j] = 0;
	}
	for(int j = 0 ; j < num ; j++ )
	{
		allRecords[j].scoreBMP.SetInteger(0);
	}

	noteTime = timeGetTime() + ( 8 * 600 ) ;
	judgeTime = noteTime + (int)(4 * 60000 / music[0].GetBpms());
	judge[0] = judge[1] = -1 ;
	counter = 60 ; // 判定貼圖出現 1 秒後消失
	animationCounter[0] = animationCounter[1] = 3*60 ;
	ready.Reset();
	clear.Reset();
	mineFlag[0] = mineFlag[1] = SPfrost[0] = SPfrost[1] = SPshining[0] = SPshining[1] = false;
	frostCount[0] = frostCount[1] = shiningCount[0] = shiningCount[1] = 0;
}
void CGameStateRun::SetSongIndex( int index )							// 移動遊戲元素
{
	songIndex = index ;
}

bool CGameStateRun::judgeChanged( int last , int now )							// 移動遊戲元素
{
	if( last != now ) return true ;
	return false ;
}

void CGameStateRun::OnMove()							// 移動遊戲元素
{
	if( animationCounter[0] == 0 )
	{
		if( num == 2 ) firstPlace.OnMove();
		if( OverCounter < 3*60 ) 
		{
			if( !clear.IsFinalBitmap() ) clear.OnMove();
			animationCounter[1] = 0 ;
		}

		if( counter > 0 ) counter-- ; // 顯示判定圖案時間
		for(int i = 0 ; i < num ; i++ )
		{
			if( judge[i] != -1 && counter > 0 )
			{
				judgeBMP[i][judge[i]].OnMove(); // 貼判定
			}
			allRecords[i].scoreBMP.SetInteger((int)allRecords[i].getScore());
			allRecords[i].scoreBMP.SetTopLeft(25+(i*385),453); // 設定貼圖位置(分數)
			allRecords[i].comboBMP.SetInteger((int)allRecords[i].tempMaxCombo);
			allRecords[i].comboBMP.SetTopLeft(180+(i*300),200); // 設定貼圖位置(COMBO)
		}


		background.SetTopLeft(60 ,-background.Height());
		if( OverCounter < 0 ){
			CAudio::Instance()->Pause();
			for(int j = 0 ; j < num ; j++ )
			{
				allRecords1[j] = allRecords[j];
			}
			GotoGameState(GAME_STATE_OVER);
		}

		background.SetTopLeft(background.Left(),background.Top()+1);
	
		now = timeGetTime() ;
		if((now >= noteTime + (int)(4 * 60000 / music[0].GetBpms() + music[0].GetOffset() * 1000)) && !musicIsStart)
		{
			CAudio::Instance()->Play(songIndex+1, false);			// 撥放 
			musicIsStart = true;
		}
		for(int j = 0 ; j < num ; j++ )
		{
		
			if((now-judgeTime)  > music[j].NoteRow[judgeRow[j]].GetTime() && judgeRow[j] < music[j].GetRowCount() && musicIsStart)
			{
				for(int i=0;i<4;i++)
				{
					if(music[j].NoteRow[startRow[j]+holdRow[j]].CheckNoteType(i)==2 || music[j].NoteRow[startRow[j]+holdRow[j]].CheckNoteType(i)==4)
						holdRowFlag[j][i] = startRow[j];
				}
				judgeRow[j]++;	
			}
			if((now-judgeTime - 200)  >music[j].NoteRow[startRow[j]+holdRow[j]].GetTime() && startRow[j] < music[j].GetRowCount() && musicIsStart)
			{
				for(int i = 0 ; i < 4 ; i++ )
				{
					if(music[j].NoteRow[startRow[j]+holdRow[j]].CheckNoteType(i)==1)
					{
						music[j].NoteRow[startRow[j]+holdRow[j]].NoteClear(i);
						/////////////////////
						judge[j] = 2 ;
						counter = 30 ;
						/////////////////////
						allRecords[j].setIsMiss(true) ;
						allRecords[j].setMiss() ;
						if( !isInvincible ) allRecords[j].loseLife();
						allRecords[j].recordMaxCombo(comboUnbreak) ;
						allRecords[j].setIsMiss(false) ;
						for(int k=0;k<3;k++)
							SPflag[j][k] = false;
					}
					else if(music[j].NoteRow[startRow[j]+holdRow[j]].CheckNoteType(i)==2 || music[j].NoteRow[startRow[j]+holdRow[j]].CheckNoteType(i)==4)
					{
						holdFlag[i] = false;						//false表示進入holdCursor
						if(!keyMatchFlag[i])
						{
							/////////////////////
							judge[j] = 2 ;
							counter = 30 ;
							/////////////////////
							allRecords[j].setIsMiss(true) ;
							allRecords[j].setMiss() ;
							if( !isInvincible ) allRecords[j].loseLife();
							allRecords[j].recordMaxCombo(comboUnbreak) ;
							allRecords[j].setIsMiss(false) ;
							for(int k=0;k<3;k++)
							SPflag[j][k] = false;
						}
					}
					else if(music[j].NoteRow[startRow[j]+holdRow[j]].CheckNoteType(i)==3)
					{
						holdFlag[i] = true;
					}
					else if(music[j].NoteRow[startRow[j]+holdRow[j]].CheckNoteType(i)==5)
					{
						allRecords[j].setTotalScore(500);
						allRecords[j].setPerfect();
						if( allRecords[j].getLife() < 100 )
						{
							allRecords[j].gainLife();
						}
						allRecords[j].recordMaxCombo(comboUnbreak) ;
					}
				}
				if(holdFlag[0] && holdFlag[1] && holdFlag[2] && holdFlag[3])
				{
					startRow[j] = startRow[j] + holdRow[j] + 1;
					holdRow[j] = 0;
				}
				else
					holdRow[j]++;
			}
			if((now-noteTime) > music[j].NoteRow[endRow[j]].GetTime() && endRow[j] < music[j].GetRowCount())	
				endRow[j]++;
			for( int i = startRow[j]; i < endRow[j] ; i++ )
			{
				music[j].NoteRow[i].OnMove(music[j].GetBpms());
			}
			if(isSpecialMode == true)
			{
				for( int i=0;i<3;i++)
				{
					if(allRecords[j].tempMaxCombo >= 10+i*10 && SPflag[j][i] == false)
					{
						music[j].NoteRow[endRow[j]].setSP(1+i);
						SPflag[j][i] = true;
					}
				}
			}
			if(frostCount[j] > 0)
			{
				frostCount[j]++;
				if(frostCount[j]== 270 || frostCount[j]== 280 || frostCount[j]== 290)
					SPfrost[j] = false;
				else if(frostCount[j]== 275 || frostCount[j]== 285 || frostCount[j]== 295)
					SPfrost[j] = true;
				else if(frostCount[j]== 300)
				{
					SPfrost[j] = false;
					frostCount[j] = 0;
				}
			}
			if(shiningCount[j] > 0)
			{
				shiningCount[j]++;
				if((shiningCount[j] == 300))
				{
					SPshining[j] = false;
					shiningCount[j] = 0;
				}
				else if((shiningCount[j]/10)%2 == 0)
					SPshining[j] = true;
				else if((shiningCount[j]/10)%2 == 1)
					SPshining[j] = false;
			}
		}
		switch( num ){
			case 1 :
			if( allRecords[0].getLife() <= 0 )
			{
				for(int j=0;j<num;j++)
				{
					allRecords[j].setDead() ;
				}
				CAudio::Instance()->Play(AUDIO_DIE);
				for(int j=0;j<num;j++)
				{
					allRecords1[j] = allRecords[j];
				}
				CAudio::Instance()->Stop(songIndex+1);
			
				GotoGameState(GAME_STATE_OVER);
			}
			break ;
		
			case 2:
			if( allRecords[0].getLife() <= 0 || allRecords[1].getLife() <= 0 )
			{
				for(int j=0;j<num;j++)
				{
					allRecords[j].setDead() ;
				}
				CAudio::Instance()->Play(AUDIO_DIE);
				for(int j=0;j<num;j++)
				{
					allRecords1[j] = allRecords[j];
				}
				CAudio::Instance()->Stop(songIndex+1);
			
				GotoGameState(GAME_STATE_OVER);
			}
			break;
		}

		OverCounter-- ;
		PercentageCounter = (int)((OverCounter*100 / ( music[0].GetTotalTime()*60))) ;
		for(int i=0;i<num;i++)
		{
			right[i].OnMove();
			left[i].OnMove();
			up[i].OnMove();
			down[i].OnMove();
			SPcursor[i].OnMove();
		}
	}
	else
	{
		if( !ready.IsFinalBitmap() ) ready.OnMove() ;
		animationCounter[0]-- ;
	}
}

CAnimation CGameStateRun::judgeBMP[2][3];
CMovingBitmap  CGameStateRun::combo[2] ;
bool CGameStateRun::isBmpLoaded = false ;

void CGameStateRun::OnInit()  								// 遊戲的初值及圖形設定
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
	//
	// 開始載入資料
	//

	background.LoadBitmap("Bitmaps/Background.bmp") ;
	frost.LoadBitmap("Bitmaps/frost.bmp",RGB(0,0,0));

	ready.AddBitmap("Bitmaps/ready/ready1.bmp",RGB(255,255,255));
	ready.AddBitmap("Bitmaps/ready/ready2.bmp",RGB(255,255,255));
	ready.AddBitmap("Bitmaps/ready/ready3.bmp",RGB(255,255,255));
	ready.AddBitmap("Bitmaps/ready/ready4.bmp",RGB(255,255,255));
	ready.AddBitmap("Bitmaps/ready/ready5.bmp",RGB(255,255,255));
	ready.AddBitmap("Bitmaps/ready/ready6.bmp",RGB(255,255,255));
	ready.AddBitmap("Bitmaps/ready/ready7.bmp",RGB(255,255,255));
	ready.AddBitmap("Bitmaps/ready/ready8.bmp",RGB(255,255,255));
	ready.AddBitmap("Bitmaps/ready/ready9.bmp",RGB(255,255,255));
	ready.SetTopLeft(103,0) ;
	ready.SetDelayCount(5) ;

	clear.AddBitmap("Bitmaps/clear/clear1.bmp",RGB(255,255,255));
	clear.AddBitmap("Bitmaps/clear/clear2.bmp",RGB(255,255,255));
	clear.AddBitmap("Bitmaps/clear/clear3.bmp",RGB(255,255,255));
	clear.AddBitmap("Bitmaps/clear/clear4.bmp",RGB(255,255,255));
	clear.AddBitmap("Bitmaps/clear/clear5.bmp",RGB(255,255,255));
	clear.AddBitmap("Bitmaps/clear/clear6.bmp",RGB(255,255,255));
	clear.AddBitmap("Bitmaps/clear/clear7.bmp",RGB(255,255,255));
	clear.AddBitmap("Bitmaps/clear/clear8.bmp",RGB(255,255,255));
	clear.AddBitmap("Bitmaps/clear/clear9.bmp",RGB(255,255,255));
	clear.SetTopLeft(28,0) ;
	clear.SetDelayCount(5) ;

	for(int i=0 ; i < 2 ; i++ )
	{
		allRecords[i].scoreBMP.LoadBitmap();
		allRecords[i].comboBMP.LoadBitmap();

		left[i].AddBitmap("Bitmaps/cursor/left.bmp",RGB(255,255,255));
		left[i].AddBitmap("Bitmaps/cursor/left2.bmp",RGB(255,255,255));
		down[i].AddBitmap("Bitmaps/cursor/down.bmp",RGB(255,255,255));
		down[i].AddBitmap("Bitmaps/cursor/down2.bmp",RGB(255,255,255));
		up[i].AddBitmap("Bitmaps/cursor/up.bmp",RGB(255,255,255));
		up[i].AddBitmap("Bitmaps/cursor/up2.bmp",RGB(255,255,255));
		right[i].AddBitmap("Bitmaps/cursor/right.bmp",RGB(255,255,255));
		right[i].AddBitmap("Bitmaps/cursor/right2.bmp",RGB(255,255,255));
		SPcursor[i].AddBitmap("Bitmaps/cursor/SPcursor1.bmp",RGB(255,255,255));
		SPcursor[i].AddBitmap("Bitmaps/cursor/SPcursor.bmp",RGB(255,255,255));
		combo[i].LoadBitmap("Bitmaps/judgement/combo.bmp",RGB(0,0,0));
		combo[i].SetTopLeft(180+(i*300),230);

		for( int j = 0 ; j < 3 ; j++ )
		{
			if( j == 0 )
			{
				judgeBMP[i][0].AddBitmap("Bitmaps/judgement/fantastic.bmp", RGB(0,0,0) ) ;
				judgeBMP[i][0].AddBitmap("Bitmaps/judgement/fantastic1.bmp", RGB(0,0,0) ) ;
				judgeBMP[i][0].AddBitmap("Bitmaps/judgement/fantastic2.bmp", RGB(0,0,0) ) ;
				judgeBMP[i][0].SetTopLeft(50+(i*350),280) ;
				judgeBMP[i][0].SetDelayCount(5);
			}
			else if( j == 1 )
			{
				judgeBMP[i][1].AddBitmap("Bitmaps/judgement/good.bmp", RGB(0,0,0) ) ;
				judgeBMP[i][1].AddBitmap("Bitmaps/judgement/good1.bmp", RGB(0,0,0) ) ;
				judgeBMP[i][1].AddBitmap("Bitmaps/judgement/good2.bmp", RGB(0,0,0) ) ;
				judgeBMP[i][1].SetTopLeft(50+(i*350),280) ;	
				judgeBMP[i][1].SetDelayCount(5);
			}
			else if( j == 2 )
			{
				judgeBMP[i][2].AddBitmap("Bitmaps/judgement/poor.bmp", RGB(0,0,0) ) ;
				judgeBMP[i][2].AddBitmap("Bitmaps/judgement/poor1.bmp", RGB(0,0,0) ) ;
				judgeBMP[i][2].AddBitmap("Bitmaps/judgement/poor2.bmp", RGB(0,0,0) ) ;
				judgeBMP[i][2].SetTopLeft(50+(i*350),280) ;	
				judgeBMP[i][2].SetDelayCount(5);
			}
			if( !isBmpLoaded ) isBmpLoaded = true ;
		}
	}

	for(int i = 0 ; i < 4 ; i++ )
	{
		keyMatch[i].LoadBitmap("Bitmaps/match.bmp"); // 
		keyMatch[i].SetTopLeft(60 + i*130,400);
	}

	for(int i = 0 ; i < 2 ; i++ )
	{
		left[i].SetTopLeft(0 + i*384,60);
		down[i].SetTopLeft(64 + i*384,60);
		up[i].SetTopLeft(128 + i*384,60);
		right[i].SetTopLeft(192+ i*384,60);
		SPcursor[i].SetTopLeft(256 + i*64,60);
	}
	for( int p = 0 ; p < 10 ; p++ )
	{
		CMovingBitmap bmp ;
		switch(p)
		{
			case 0:
				bmp.LoadBitmap("Bitmaps/regular/lifeleft.bmp" ,RGB(255,255,255)) ;
				bmp.SetTopLeft(0,27) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 1:
				bmp.LoadBitmap("Bitmaps/regular/liferight.bmp" ,RGB(255,255,255)) ;
				bmp.SetTopLeft(361,27) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 2:
				bmp.LoadBitmap("Bitmaps/regular/pointleft.bmp" ,RGB(0,0,0)) ;
				bmp.SetTopLeft(0,440) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 3:
				bmp.LoadBitmap("Bitmaps/regular/pointright.bmp" ,RGB(0,0,0)) ;
				bmp.SetTopLeft(384,440) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 4:
				bmp.LoadBitmap("Bitmaps/regular/regular.bmp" ,RGB(255,255,255)) ;
				bmp.SetTopLeft(248,438) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 5:
				bmp.LoadBitmap("Bitmaps/regular/stage.bmp" ,RGB(0,0,0)) ;
				bmp.SetTopLeft(245,0) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 6:
				bmp.LoadBitmap("Bitmaps/regular/upleft.bmp" ,RGB(0,0,0)) ;
				bmp.SetTopLeft(0,0) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 7:
				bmp.LoadBitmap("Bitmaps/regular/upright.bmp" ,RGB(0,0,0)) ;
				bmp.SetTopLeft(395,0) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 8:
				bmp.LoadBitmap("Bitmaps/eventstage.bmp" ,RGB(0,0,0)) ;
				bmp.SetTopLeft(283,10) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 9:
				bmp.LoadBitmap("Bitmaps/scroll.bmp" ,RGB(0,0,0)) ;
				bmp.SetTopLeft(75,42) ;
				backgroundBMP.push_back(bmp) ;
				break ;
		}
	}

	for (int i = 1 ; i < 7; i++)
	{
		char str[80];				
		sprintf(str, "Bitmaps/grade/place%d.bmp", i );
		firstPlace.AddBitmap(str,RGB(0,0,0));
	}
	firstPlace.SetDelayCount(5) ;

	counter = 0 ;
	judge[0] = judge[1] = 0 ; // 判定圖案 0.fantastic 1.good 2.poor
	isSpecialMode = false ;
	isInvincible = false ;
	comboUnbreak = false ;
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	const char KEY_LEFT  = 0x25; // keyboard左箭頭
	const char KEY_UP    = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN  = 0x28; // keyboard下箭頭
	const char KEY[2][4] = {{'Z','X','C','V'},{'U','I','O','P'}};
	const char KEY2[2] = {'B','Y'};
	const char KEY_SPACE = ' ';
	const char KEY_ESC = 0x1B;
	const char KEY_F1 = 0x70 ; // F1 鍵 無敵狀態
	const char KEY_F2 = 0x71 ; //F2 鍵 血量補滿
	const char KEY_F3 = 0x72 ; //F3 鍵 直接進結算畫面
	const char KEY_F4 = 0x73 ; //F4 鍵
	int time = timeGetTime();
	int k=3;

	if(isSpecialMode == true)
	{
		for(int j=0;j<num;j++)
		{
			if(nChar == KEY2[j] && music[0].GetRowCount()- judgeRow[0] > 3)
			{
				if((music[j].NoteRow[judgeRow[j]].GetTime() - (time-judgeTime)) < 200 && (music[j].NoteRow[judgeRow[j]].CheckNoteType(4) > 5))
				{
					switch(music[j].NoteRow[judgeRow[j]].CheckNoteType(4))
					{
						case 6:
							CAudio::Instance()->Play( AUDIO_MINE2);
							do
							{
								for(int i=0;i<4;i++)
								{
									if(music[1-j].NoteRow[judgeRow[1-j]+k].CheckNoteType(i)==1)
									{
										music[1-j].NoteRow[judgeRow[1-j]+k].setMine(i);
										mineFlag[j] = true;
									}
									else
										k++;
								}
							}
							while(mineFlag[j] == false);
							mineFlag[j] = false;
							break;
						case 7:
							SPfrost[1-j]=true;
							frostCount[1-j] = 1;
							CAudio::Instance()->Play( AUDIO_FROST);
							break;
						case 8:
							SPshining[1-j]=true;
							shiningCount[1-j] = 1;
							CAudio::Instance()->Play( AUDIO_SHINING);
							break;
					}
				}
			}
		}
	}
	for(int j=0;j<num;j++)
	{
		for(int i=0;i<4;i++)
		{
			if (nChar == KEY[j][i])
			{
				keyMatchFlag[i] = true ;
				if((music[j].NoteRow[judgeRow[j]].GetTime() - (time-judgeTime)) < 50 && (music[j].NoteRow[judgeRow[j]].CheckNoteType(i) == 1 || music[j].NoteRow[judgeRow[j]].CheckNoteType(i) == 2))
				{
					allRecords[j].setTotalScore(500);
					allRecords[j].setPerfect();
					/////////////////////
					judge[j] = 0 ;
					counter = 30 ;
					/////////////////////
					if( allRecords[j].getLife() < 100 )
					{
						allRecords[j].gainLife();
					}
					allRecords[j].recordMaxCombo(comboUnbreak ) ;
					if(music[j].NoteRow[judgeRow[j]].CheckNoteType(i)==1)
						music[j].NoteRow[judgeRow[j]].NoteClear(i);
				}
				else if((music[j].NoteRow[judgeRow[j]].GetTime() - (time-judgeTime)) < 200 && (music[j].NoteRow[judgeRow[j]].CheckNoteType(i) == 1 || music[j].NoteRow[judgeRow[j]].CheckNoteType(i) == 2))
				{
					allRecords[j].setTotalScore(300) ;
					allRecords[j].setGood();
					/////////////////////
					judge[j] = 1 ;
					counter = 30 ;
					/////////////////////
					if( allRecords[j].getLife() < 100 )
					{
						allRecords[j].gainLife();
					}
					allRecords[j].recordMaxCombo(comboUnbreak ) ;
					if(music[j].NoteRow[judgeRow[j]].CheckNoteType(i)==1)
						music[j].NoteRow[judgeRow[j]].NoteClear(i);
				}
				else if((music[j].NoteRow[judgeRow[j]].GetTime() - (time-judgeTime)) < 200 && (music[j].NoteRow[judgeRow[j]].CheckNoteType(i) == 5))
				{
					CAudio::Instance()->Play( AUDIO_MINE );
					allRecords[j].setIsMiss(true) ;
					allRecords[j].setMiss() ;
					if( !isInvincible )
					{
						allRecords[j].loseLife();
						allRecords[j].loseLife();
						allRecords[j].loseLife();
					}
					allRecords[j].recordMaxCombo(comboUnbreak ) ;
					allRecords[j].setIsMiss(false) ;
					music[j].NoteRow[judgeRow[j]].NoteClear(i);
					for(int k=0;k<3;k++)
						SPflag[j][k] = false;
				}
				else if(judgeRow > 0)
				{
					if(((time-judgeTime) -music[j].NoteRow[judgeRow[j]-1].GetTime()) < 50 && (music[j].NoteRow[judgeRow[j]-1].CheckNoteType(i) == 1 || music[j].NoteRow[judgeRow[j]-1].CheckNoteType(i) == 2))
					{
						allRecords[j].setTotalScore(500);
						allRecords[j].setPerfect();
						/////////////////////
						judge[j] = 0 ;
						counter = 30 ;
						/////////////////////
						if( allRecords[j].getLife() < 100 )
						{
							allRecords[j].gainLife();
						}
						allRecords[j].recordMaxCombo(comboUnbreak ) ;
						if(music[j].NoteRow[judgeRow[j]-1].CheckNoteType(i)==1)
							music[j].NoteRow[judgeRow[j]-1].NoteClear(i);
					}
					else if(((time-judgeTime) -music[j].NoteRow[judgeRow[j]-1].GetTime()) < 200 && (music[j].NoteRow[judgeRow[j]-1].CheckNoteType(i) == 1 || music[j].NoteRow[judgeRow[j]-1].CheckNoteType(i) == 2))
					{
						allRecords[j].setTotalScore(300);
						allRecords[j].setPerfect();
					/////////////////////
						judge[j] = 0 ;
						counter = 30 ;
					/////////////////////
						if( allRecords[j].getLife() < 100 )
						{
							allRecords[j].gainLife();
						}
						allRecords[j].recordMaxCombo(comboUnbreak ) ;
						if(music[j].NoteRow[judgeRow[j]-1].CheckNoteType(i)==1)
							music[j].NoteRow[judgeRow[j]-1].NoteClear(i);
					}
					else if(((time-judgeTime) -music[j].NoteRow[judgeRow[j]-1].GetTime()) < 200 && (music[j].NoteRow[judgeRow[j]-1].CheckNoteType(i) == 5))
					{
						CAudio::Instance()->Play( AUDIO_MINE );
						allRecords[j].setIsMiss(true) ;
						allRecords[j].setMiss() ;
						if( !isInvincible )
						{
							allRecords[j].loseLife();
							allRecords[j].loseLife();
							allRecords[j].loseLife();
						}
						allRecords[j].recordMaxCombo(comboUnbreak ) ;
						allRecords[j].setIsMiss(false) ;
						music[j].NoteRow[judgeRow[j]-1].NoteClear(i);
						for(int k=0;k<3;k++)
							SPflag[j][k] = false;
					}
				}
			}
		}
	}
	if (nChar == KEY_F1 )
	{
		isInvincible = !isInvincible ;
	}
	if (nChar == KEY_F2 )
	{
		while( allRecords[0].getLife() <= 100)
		{
			allRecords[0].gainLife() ;
			if( num == 2 ) allRecords[1].gainLife() ;
		}
	}
	if (nChar == KEY_F3 )
	{
		OverCounter = 1 ;
	}
	if( nChar == KEY_F4 )
	{
		comboUnbreak = !comboUnbreak ;
	}
	if (nChar == KEY_ESC )
	{
		CAudio::Instance()->Stop(num+1);
//		for(int i = 0 ; i < num ; i++ ) music[i].DeleteRow();
		ClearMusic();
		
		GotoGameState(GAME_STATE_MENU);
	}

}

void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; // keyboard左箭頭
	const char KEY_UP    = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN  = 0x28; // keyboard下箭頭
	const char KEY[2][4] = {{'Z','X','C','V'},{'U','I','O','P'}};
	
	for(int j=0;j<num;j++)
	{
		for(int i=0;i<4;i++)
		{
			if (nChar == KEY[j][i])
			{
				keyMatchFlag[i] = false ;
				if(!holdFlag[i] && music[j].NoteRow[holdRowFlag[j][i]].CheckNoteType(i) == 2)
				{
					music[j].NoteRow[holdRowFlag[j][i]].NoteClear(i);
					allRecords[0].setIsMiss(true) ;
					allRecords[0].setMiss() ;
					////////////////////
					judge[j] = 2 ;
					counter = 30 ;
					/////////////////////

					if( !isInvincible ) allRecords[0].loseLife();
					allRecords[0].recordMaxCombo(comboUnbreak ) ;
					allRecords[0].setIsMiss(false) ;
				}
			}
		}
	}
}

ScoreCount CGameStateRun::GetRecordInfo( int index )
{
	return allRecords1[index] ;
}



void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
	
}

void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	
}

void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
	
}

void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	
}

void CGameStateRun::SetSongPath( string name )	
{
	songPath = name ;
}

void CGameStateRun::SetPlayers( int index )
{
	num = index ;
}

void CGameStateRun::ClearMusic()
{
	for(int i = 0 ; i < num ; i++ ){
	    music[i].DeleteRow();
	}	
}

void CGameStateRun::SetSpecialMode( bool open )
{
	if( open )
	{
		isSpecialMode = true ;
		return ;
	}
	isSpecialMode = false ;

}

void CGameStateRun::ShowLifeProgress( int percent , int position )
{
	const int bar_width = 275 ;
	const int bar_height = 10;
	const int x1 = 3+(position*362) ;
	const int x2 = x1 + bar_width;
	const int y1 = 33 ;
	const int y2 = y1 + bar_height ;
	const int pen_width = bar_height ;
	const int progress_x1 = x1 + pen_width;
	const int progress_x2 = progress_x1 + percent * (bar_width-2*pen_width) / 100;
	const int progress_x2_end = x2 - pen_width;
	const int progress_y1 = y1 + pen_width;
	const int progress_y2 = y2 - pen_width;

	CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CPen *pp, p(PS_NULL, 0, RGB(0,0,0));		// 清除pen
	pp = pDC->SelectObject(&p);

	CBrush b1(DEFAULT_BG_COLOR);				// 畫黑色 progrss中心
	pDC->SelectObject(&b1);
	pDC->Rectangle(progress_x1,progress_y1,progress_x2_end,progress_y2);

	CBrush b2(RGB(255,255,0));					// 畫黃色 progrss進度
	pDC->SelectObject(&b2);
	pDC->Rectangle(progress_x1,progress_y1,progress_x2,progress_y2);

	pDC->SelectObject(pp);						// 釋放 pen
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}

void CGameStateRun::ShowTimeProgress( int percent )
{

    const int tempPercent = 100-percent ;
	const int bar_width = 465;
	const int bar_height = 10;
	const int x1 = 75 ;
	const int x2 = x1 + bar_width;
	const int y1 = 42 ;
	const int pen_width = bar_height ;
	const int progress_x1 = x1 + pen_width;
	const int progress_x2 = progress_x1 + tempPercent * (bar_width-2*pen_width) / 100;
	const int progress_x2_end = x2 - pen_width;

	backgroundBMP[9].SetTopLeft( progress_x2 , y1 ) ;
}

void CGameStateRun::OnShow()
{

	background.SetTopLeft(0,0);
	background.ShowBitmap();

	for(int j = 0 ; j < num ; j++ )
	{
		right[j].OnShow();
		left[j].OnShow();
		up[j].OnShow();
		down[j].OnShow();
		if(isSpecialMode == true)
		{
			SPcursor[j].OnShow();
			if(SPshining[j] == false)
			for(int i = endRow[j] ; i >= startRow[j] ; i-- ){
				music[j].NoteRow[i].OnShow(j);
			}
		}
		else
		{
			for(int i = endRow[j] ; i >= startRow[j] ; i-- ){
				music[j].NoteRow[i].OnShow(j);
			}
		}
		if( allRecords[j].tempMaxCombo > 1 ) 
		{
			combo[j].ShowBitmap();
			allRecords[j].comboBMP.ShowBitmap();
		}
		ShowLifeProgress( (int)allRecords[j].getLife() , j ) ;
		ShowTimeProgress( PercentageCounter ) ;
		if(SPfrost[j] == true)
		{
			frost.SetTopLeft(j*320,45);
			frost.ShowBitmap();
		}
	}
	for(int i = 0 ; i < (int)backgroundBMP.size() ; i++ ){
		backgroundBMP[i].ShowBitmap();
	}
	for(int j = 0 ; j < num ; j++)
	{
		allRecords[j].scoreBMP.ShowBitmap();
		if( judge[j] != -1 && counter > 0 )
		{
			judgeBMP[j][judge[j]].OnShow(); // 貼判定
		}
	}
	
	if( num == 2 )
	{
		if( allRecords[0].getScore() > allRecords[1].getScore() ) 
		{
			firstPlace.SetTopLeft( 200 , 300 ) ;
			firstPlace.OnShow() ;

		}
		else if( allRecords[1].getScore() > allRecords[0].getScore() )
		{
			firstPlace.SetTopLeft( 470 , 300 ) ;
			firstPlace.OnShow() ;
		}
	}
	if( animationCounter[0] > 0 ) ready.OnShow();
	if( animationCounter[1] == 0 ) clear.OnShow();

	if( isInvincible || comboUnbreak )
	{
		CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		CFont f,*fp;
		f.CreatePointFont(80,"Times New Roman");	// 產生 font f; 160表示16 point的字
		fp=pDC->SelectObject(&f);					// 選用 font f
		pDC->SetBkColor(RGB(0,0,0));
		pDC->SetBkMode(0);
		pDC->SetTextColor(RGB(255,255,0));
		char str[80];								// Demo 數字對字串的轉換

		/////////// 
		if( isInvincible )
		{
			sprintf(str, "已開啟無敵模式" );
			pDC->TextOut(280,447,str);
		}
		if( comboUnbreak )
		{
			sprintf(str, "Combo 數不中斷");
			pDC->TextOut(280,457,str);
		}
		//////////
		pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
	}	

	//allRecords.showScore();

}   


/////////////////////////////////////////////////////////////////////////////
// Menu
/////////////////////////////////////////////////////////////////////////////

CGameStateMenu::CGameStateMenu(CGame *g)
: CGameState(g) 
{
}

CGameStateMenu::~CGameStateMenu()
{

}

void CGameStateMenu::OnBeginState()
{
	timerCounter = 100*60 ; // 選歌時間為30秒
	animationCounter = 4*60 ; // 選歌時間為30秒
	selected.Reset();
	isSelected = false ;
	keyMatchFlagUp = keyMatchFlagDown = keyMatchFlagLeft = keyMatchFlagRight = 0 ;
	CAudio::Instance()->Play(songIndex+1,false);
	SetBrowseCounter();

}

void CGameStateMenu::OnMove()							// 移動遊戲元素
{
	if( isSelected == true && animationCounter > 0 )
	{
		selected.OnMove();
		animationCounter-- ; 
	}
	if( animationCounter == 0 ) GotoGameState(GAME_STATE_RUN);

	background[0].SetTopLeft(0 ,0);

	musicSelect.OnMove();
	musicSelect.SetTopLeft(section_x-12,section_y-10);

	if( timerCounter > 0 )
	{
		timer.SetInteger(timerCounter/60);
		timerCounter--;
	}
	if( timerCounter == 0 )
	{
		SetSongIndex( songIndex ) ;
		game->gameStateTable[GAME_STATE_RUN]->SetSongPath( GetSongPath(GetSongName(songIndex)).c_str() ) ;
		CAudio::Instance()->Stop( songIndex+1);
		CAudio::Instance()->Stop( AUDIO_MENU );
		if( !isSelected )
		{
			isSelected = true ;
			CAudio::Instance()->Play( AUDIO_SELECT , false);	
		}
	}

	if( playBGM == true )
	{
		CAudio::Instance()->Play(AUDIO_MENU,true);
		playBGM = false ;
	}

	if( songChanged == true )
	{
		CAudio::Instance()->Stop(tempSongIndex+1);
		CAudio::Instance()->Stop(AUDIO_MENU);
		CAudio::Instance()->Play(songIndex+1,false);
		SetBrowseCounter();
		songChanged = false ;
	}
	else if( browseCounter >= 0 )
	{
		browseCounter-- ;
		if( browseCounter == 0 )
		{
			CAudio::Instance()->Stop(songIndex+1);
			if( !isSelected ) playBGM = true ;
		}
	}

}

CMovingBitmap CGameStateMenu::selectback[7] ;
bool CGameStateMenu::isBmpLoaded = false ;

void CGameStateMenu::OnInit()  								// 遊戲的初值及圖形設定
{
	//
	// 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
	//     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
	//
	ShowInitProgress(33);

	background[0].LoadBitmap("Bitmaps/Background.bmp", RGB(0,0,0));
	background[1].LoadBitmap("Bitmaps/black.bmp");
	timer.LoadBitmap();
	timer.SetInteger(99) ;
	timer.SetTopLeft(565,20);

	musicSelect.AddBitmap("Bitmaps/MusicWheelhighlight.bmp", RGB(0,0,0));
	for (int i = 1 ; i < 4; i++)
	{
		char str[80];				
		sprintf(str, "Bitmaps/MusicWheelhighlight%d.bmp", i );
		musicSelect.AddBitmap(str,RGB(0,0,0));
	}
	musicSelect.SetDelayCount(5);

	for (int i = 4 ; i > 0; i--)
	{
		char str[80];				
		sprintf(str, "Bitmaps/loadbar%d.bmp", i );
		selected.AddBitmap(str,RGB(0,0,0));
	}
	selected.SetDelayCount(60);
	selected.SetTopLeft(0,220);

	instruction.LoadBitmap("Bitmaps/instruction2.bmp",RGB(0,0,0));
	instruction.SetTopLeft(80,401);

	playBGM = false ;

	
	for(int i = 0 ; i < 7 ; i++ )
	{
		selectback[i].LoadBitmap("Bitmaps/Music section.bmp" ,RGB(0,0,0)) ;
		if( !isBmpLoaded ) isBmpLoaded = true ;
		selectback[i].SetTopLeft(300,80+(i*50)) ;
	}

	CAudio::Instance()->Load(AUDIO_SELECT,  "Sounds/select.mp3");	// 載入編號1
	CAudio::Instance()->Load(AUDIO_MUSIC1,  "Songs/concon/concon.mp3");	//
	CAudio::Instance()->Load(AUDIO_MUSIC2,  "Songs/I'm_So_Happy/i'm_so_happy.mp3");	//
	CAudio::Instance()->Load(AUDIO_MUSIC3,  "Songs/SigSig/SigSig.mp3");	//
	CAudio::Instance()->Load(AUDIO_MUSIC4,  "Songs/Elisha/Elisha.mp3");	//	

	CAudio::Instance()->Load(AUDIO_MENU,  "Sounds/selectmusic.mp3");	
	CAudio::Instance()->Load(AUDIO_DIE,  "Sounds/die.mp3");	

	songChanged = false ;
	songNum = 0 ; 
	GetAllSongs();
	SetBrowseCounter() ; // 預覽歌曲15秒

	for( int p = 0 ; p < songNum ; p++ )
	{
		CMovingBitmap title ;
		char str[80];								// Demo 數字對字串的轉換
	    sprintf(str, "Songs/%s/title.bmp" , allName[p].c_str());
		title.LoadBitmap(str) ;
		title.SetTopLeft(80,100) ;
		vecTitle.push_back(title) ;
	}
	for( int p = 0 ; p < 6 ; p++ )
	{
		CMovingBitmap bmp ;
		switch(p)
		{
			case 0:
				bmp.LoadBitmap("Bitmaps/Header/header_left.bmp" ,RGB(0,0,0)) ;
				bmp.SetTopLeft(0,0) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 1:
				bmp.LoadBitmap("Bitmaps/Header/header_right.bmp" ,RGB(0,0,0)) ;
				bmp.SetTopLeft(300,0) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 2:
				bmp.LoadBitmap("Bitmaps/Header/header_top.bmp" ,RGB(255,255,255)) ;
				bmp.SetTopLeft(0,0) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 3:
				bmp.LoadBitmap("Bitmaps/Header/timeremain.bmp" ,RGB(255,255,255)) ;
				bmp.SetTopLeft(550,10) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 4:
				bmp.LoadBitmap("Bitmaps/Header/footer.bmp" ,RGB(255,255,255)) ;
				bmp.SetTopLeft(0,440) ;
				backgroundBMP.push_back(bmp) ;
				break ;
			case 5:
				bmp.LoadBitmap("Bitmaps/menu/titlebanner.bmp" ,RGB(0,0,0)) ;
				bmp.SetTopLeft(75,89) ;
				backgroundBMP.push_back(bmp) ;
				break ;
		}
	}

	//
	// 完成部分Loading動作，提高進度
	//
	//
	section_x = 300 ;
	section_y = 80 ;
	songIndex = 0 ;

	ShowInitProgress(50);
}

void CGameStateMenu::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{

	const char KEY_LEFT  = 0x25; // keyboard左箭頭
	const char KEY_UP    = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN  = 0x28; // keyboard下箭頭
	const char KEY_SPACE = ' ';
	const char KEY_ENTER = 0xD;
	const char KEY_ESC = 0x1B;
	
	if (nChar == KEY_DOWN || nChar == 'X')
	{
		tempSongIndex = songIndex ; 
		if( section_y == 80+(50*7) || section_y == 80+(50*(songNum-1)) ) section_y = 80-50 ;
		section_y += 50 ;

		if( songIndex == songNum-1 )
		{
			songIndex = 0 ;
		}
		else if( songIndex >= 0  )
		{
			songIndex += 1 ;
		}

		songChanged = true ;
	}
	if (nChar == KEY_UP || nChar == 'C')
	{
		tempSongIndex = songIndex ; 
		if( section_y == 80  ) section_y = 80+(50*(songNum)) ;
			section_y -= 50 ;

		if(  songIndex == 0  )
		{
			songIndex = songNum-1 ;
		}
		else if( songIndex < songNum )
		{
			songIndex -= 1 ;
		}

		songChanged = true ;
    }
	if (nChar == KEY_ENTER)
	{
        SetSongIndex( songIndex ) ;
		game->gameStateTable[GAME_STATE_RUN]->SetSongPath( GetSongPath(GetSongName(songIndex)).c_str() ) ;
		CAudio::Instance()->Stop( songIndex+1 );
		CAudio::Instance()->Stop( AUDIO_MENU );
		if( !isSelected )
		{
			isSelected = true ;
			CAudio::Instance()->Play(AUDIO_SELECT, false);
		}

	}
	if (nChar == KEY_RIGHT || nChar == 'V')
	{
		
	}
	if (nChar == KEY_LEFT || nChar == 'Z')
	{
		
	}

	if (nChar == KEY_ESC )
	{
		CAudio::Instance()->Stop( songIndex+1 );
		CAudio::Instance()->Stop( AUDIO_MENU );
		CAudio::Instance()->Stop( AUDIO_SELECT );
		if( isSelected == true )
		{
			GotoGameState(GAME_STATE_MENU);
		}else{
			GotoGameState(GAME_STATE_INIT);
		}
	}
}
void CGameStateMenu::SetSongIndex(int index)
{
	game->gameStateTable[GAME_STATE_RUN]->SetSongIndex(index);
}
string CGameStateMenu::GetSongName(int index)
{
	string buff ;
	buff = allName[index] ;
	return buff;
}
void CGameStateMenu::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	const char KEY_LEFT  = 0x25; // keyboard左箭頭
	const char KEY_UP    = 0x26; // keyboard上箭頭
	const char KEY_RIGHT = 0x27; // keyboard右箭頭
	const char KEY_DOWN  = 0x28; // keyboard下箭頭
	const char KEY_SPACE = ' ';
	const char KEY_ENTER = 0xD;

	if (nChar == KEY_DOWN || nChar == 'X'){
		keyMatchFlagDown = false ;
	}
	if (nChar == KEY_UP || nChar == 'C'){
		keyMatchFlagUp = false ;
    }
	if (nChar == KEY_RIGHT || nChar == 'V'){
		keyMatchFlagRight = false ;
	}
	if (nChar == KEY_LEFT || nChar == 'Z'){
		keyMatchFlagLeft = false ;
	}	

}

void CGameStateMenu::OnLButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
	
}

void CGameStateMenu::OnLButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	
}

void CGameStateMenu::OnRButtonDown(UINT nFlags, CPoint point)  // 處理滑鼠的動作
{
	
}

void CGameStateMenu::OnRButtonUp(UINT nFlags, CPoint point)	// 處理滑鼠的動作
{
	
}

int CGameStateMenu::GetSongNum()
{
	return songNum ;
}

void CGameStateMenu::GetAllSongs( )	// 處理滑鼠的動作
{
	ifstream fin ;
    fin.open("Songs/infomation.txt") ; //讀檔
	char buff[256] ;
	string buffString ;
	while( fin >> buff )
	{		
		string stringBuff ; //暫存字串2
		buffString = buff ;
		for( unsigned int i = 0 ; i < buffString.size()-1 ; i++ )
		{
			if( buffString[i] != ';') 
			{
				if( buffString[i] == '_' )
				{
					tempString1 += " " ;
				}
				else
				{
					tempString1 += buffString[i] ;
				}
			}
			tempString += buffString[i] ;
		}	
		allName.push_back(tempString);
		allName1.push_back(tempString1);
		songNum += 1 ;
		tempString.clear();
		tempString1.clear();
	}
	fin.close();	
}

string CGameStateMenu::GetSongPath( string name )	// 處理滑鼠的動作
{
	string path ;
	path = "Songs/" + name + "/" + name + ".txt" ;
	return path ;
}

void CGameStateMenu::ShowList()
{
	CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CFont f,*fp;
	f.CreatePointFont(200,"Arial");	// 產生 font f; 160表示16 point的字
	fp=pDC->SelectObject(&f);					// 選用 font f
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255,255,0));
	char str[80];		// Demo 數字對字串的轉換
	/*
	sprintf(str, "Select Music ");
	pDC->TextOut( 40 , 40 ,str);
	*/
	pDC->SetBkMode(0); // 使字體背景透明

	for( int k = 0 ; k < songNum ; k++ )
	{
		sprintf(str, "%s" , allName1[k].c_str() );
		pDC->TextOut( 380 , 80+(50*k) ,str);
	}

	pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
	CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC

}

int CGameStateMenu::GetSongIndex()
{
	return songIndex ;
}

void CGameStateMenu::SetBrowseCounter()
{
	browseCounter = 15*60 ;
}

void CGameStateMenu::OnShow()
{

	background[0].ShowBitmap();

	vecTitle[GetSongIndex()].ShowBitmap();
	for( int i = 0 ; i < 6 ; i++ )
	{
		backgroundBMP[i].ShowBitmap();
	}

	for( int i = 0 ; i < (int)vecTitle.size() ; i++ )
	{
		selectback[i].ShowBitmap();
	}
	musicSelect.OnShow();
	

	timer.ShowBitmap();
	instruction.ShowBitmap();
	 //Test Index
	/*
	CDC *pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
	CFont f,*fp;
	f.CreatePointFont(200,"Times New Roman");	// 產生 font f; 160表示16 point的字
	fp=pDC->SelectObject(&f);					// 選用 font f
	pDC->SetBkColor(RGB(0,0,0));
	pDC->SetTextColor(RGB(255,255,0));
	char str[80];								// Demo 數字對字串的轉換
	sprintf(str, "%d " , songIndex);
	pDC->TextOut( 40 , 100 ,str);
	
	sprintf(str, "%s " , GetSongPath(GetSongName(songIndex)).c_str());
	pDC->TextOut( 40 , 100 ,str);
	
	pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
	CDDraw::ReleaseBackCDC();	
	*/
	
    ShowList() ;
	if( isSelected )
	{
		background[1].ShowBitmap(); 	
		selected.OnShow();
	}
}

}