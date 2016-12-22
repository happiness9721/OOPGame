/*
 * gamelib.cpp: ���ɮ��x�s�䴩�C��������class��implementation
 * Copyright (C) 2002-2012 Woei-Kae Chen <wkc@csie.ntut.edu.tw>
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

 * Known Problems:
 *   2005-9-29
 *      1. VC++.net is unable to add new event handlers for game (this is due
 *         to incompatibility of MFC files between VC++6.0 and VC++.net).
 *         Work around: I have added MOUSEMOVE handlers by using VC++6.0. Other
 *           event handlers are likely unnecessary.
 *      2. When ENABLE_GAME_PAUSE is false, it is possible for a minimized
 *           game to continue playing sounds.
 *         
 *
 * History:
 *	 1999-09-24 V2.2
 *		1. Support playback of WAVE files with DirectSound.
 *		2. Use OnIdle to control the flow of the game.
 *	 2002-02-23 v3.0 (fullscreen)
 *		1. Support fullscreen mode.
 *		2. Support playback of both MIDI and WAVE files with DirectMusic.
 *		3. Fix surface lost bugs by restoring surfaces.
 *		4. Move CGame and CBall classes into mygame.cpp.
 *		5. Add CAnimation Class.
 *   2002-03-04 V3.1
 *      1. Add CMovingBitmap::ShowBitmap(CMovingBitmap &) to enable
 *         the operation of blitting from one bitmap into another bitmap.
 *		2. Fix a CheckDDError() bug (incorrect number of errors).
 *      3. Revise delay codes for CSpecialEffect::Delay() and
 *		   CSpecialEffect::DelayFromSetCurrentTime().
 *		4. Support ENABLE_AUDIO flag.
 *   2004-03-01 V4.0
 *      1. Enhance CAudio to support pause and resume. 
 *   2004-03-08 V4.1
 *      1. Add KeyDown & KeyUp handler and remove auto-repeat for key
 *         down.
 *      2. Fix a surface lost bug due to suspend of windows.
 *      3. The Game Engine is now closer to a framework.
 *   2005-07-28 V4.2
 *      1  GAME_ASSERT is used to enforce correctness of operations for
 *         CMovingBitamp (make sure bitmap is loaded first).
 *      2. Change the constructor of CAudio to eliminate the compiling
 *         error with VC++.net.
 *      3. Make SurfaceID unsigned to eliminate warning with VC++.net.
 *   2005-09-08
 *      1. Fix a bug that handles CLR_INVALID incorrectly during
 *         SetColorKey(), BltBitmapToBack(), and BltBitmapToBitmap().
 *      2. Eliminate the use of CSpecialEffect::Abort() from BltBitmapToBack.
 *         Use GAME_ASSERT instead.
 *   2005-09-20 V4.2Beta1.
 *   2005-09-29 V4.2Beta2.
 *      1. Add MOUSEMOVE Handler for CGame and CGameState.
 *      2. Add _TRACE preprocessor flag for VC++.net.
 *   2006-02-08 V4.2
 *      1. Fix bugs: make CAnimation::Top() and CAnimation::Left() return y and x.
 *      2. Enhance CAnimation to support SetDelayCount(), Reset(), and IsFinalBitmap().
 *      3. Remove CAnimation::GetLocation() and CMovingBitmap::GetLocation().
 *      4. Bitmap coordinate can no longer be set by CMovingBitmap::LoadBitmap();
 *         defauts to (0,0).
 *   2006-09-09 V4.3
 *      1. Rename Move() and Show() as OnMove and OnShow() to emphasize that they are
 *         event driven.
 *      2. Fix bug: audio is now correctly recovered after a sleep or suspension of windows.
 *      3. Support ENABLE_GAME_PAUSE.
 *   2008-02-15 V4.4
 *      1. Add setup project for Visual studio 2005.
 *      2. Support bitmap scaling when ShowBitmap(scale) is called.
 *      3. Add namespace game_framework.
 *      4. Make the class CGame a singleton so that MFC can access it easily.
 *      5. Support loading of bitmap from bmp file.
 *      6. Support ShowInitProgress(percent) to display loading progress. 
 *   2010-02-23 V4.5
 *      1. Remove #define INITGUID to work with VS2008
 *   2010-03-23 V4.6
 *      1. Fix bug: when AUDIO device is not available, CGame::OnInit() returned too early.
 *      2. Rewrite CAudio with MCI commands to eliminate dependency with DirectMusic.
 *      3. Supprt MP3 audio playback.
 *   2012-03-21 V4.7
 *      1. Add SetCapture in CGameView::OnLButtonDown() and RelaseCapture in 
 *         CGameView::OnLButtonUp() so that Mouse Events (OnLButtonUp and OnMouseMove)
 *         will be called even if the cursor is outside of the window.
 *   2012-05-13 V4.8
 *      1. Change release mode WINVER to 0x500 so that release mode can be correctly complied
 *         with VS 2010 (project->Game Properties->C/C++->Preprocessor Definitions->WINVER=0x500).
 *      2. Add comments to CGameState::ShowInitProgress() - remind students not to copy
 *         	CDDraw::BltBackToPrimary().
 *      3. Fix Ctrl-Q display bug. Add Invalidate() at the end of CGameView::OnKillFocus.
 *      4. Move CAudio header and implementations into new audio.h and audio.cpp files.
 *      5. Use a thread to execute all MCI commands so that when a sound is played, the
 *         main game thread is not slowed down by MCI commands. According to students, the
 *         slow down is more obvious in Win7 64 bit version; in WinXP, the slow down cannot
 *         be observed.
 *      6. Remove UnitTest folder and configurations.
 *      7. Move header and cpp files into Source sub-directory.
 *   2016-02-26 V4.9
 *      1.Fixed program crash in change display mode to fullscreen
*/

//#define	 INITGUID
#include "stdafx.h"
#include "game.h"
#include "MainFrm.h"
#include "Resource.h"
#include <mmsystem.h>
#include "../CDDraw.h"
#include <ddraw.h>
#include <direct.h>
#include <string.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"


namespace game_framework {

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����U�ت��A��Base class(�O�@��abstract class)
/////////////////////////////////////////////////////////////////////////////

CGameState::CGameState(CGame *g)
{
	game = g; 	// �]�wgame��pointer
}

void CGameState::GotoGameState(int state)
{
	game->SetGameState(state);
}

ScoreCount CGameState::GetRecordInfo(int index)
{
	return allRecords[index];
}

void CGameState::ShowInitProgress(int percent)
{
	if (!SHOW_LOAD_PROGRESS)
		return;
	const int bar_width = SIZE_X * 2 / 3;
	const int bar_height = SIZE_Y / 20;
	const int x1 = (SIZE_X - bar_width) / 2;
	const int x2 = x1 + bar_width;
	const int y1 = (SIZE_Y - bar_height) / 2;
	const int y2 = y1 + bar_height;
	const int pen_width = bar_height / 8;
	const int progress_x1 = x1 + pen_width;
	const int progress_x2 = progress_x1 + percent * (bar_width-2*pen_width) / 100;
	const int progress_x2_end = x2 - pen_width;
	const int progress_y1 = y1 + pen_width;
	const int progress_y2 = y2 - pen_width;

	CDDraw::BltBackColor(DEFAULT_BG_COLOR);		// �N Back Plain ��W�w�]���C��
	CMovingBitmap loading;						// �K�Wloading�ϥ�
	loading.LoadBitmap(IDB_LOADING, RGB(0,0,0));
	loading.SetTopLeft((SIZE_X - loading.Width())/2, y1 - 2 * loading.Height());
	loading.ShowBitmap();
	//
	// �H�U��CDC���Ϊk
	//
	CDC *pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
	CPen *pp, p(PS_NULL, 0, RGB(0,0,0));		// �M��pen
	pp = pDC->SelectObject(&p);

	CBrush *pb, b(RGB(0,255,0));				// �e��� progress��
	pb = pDC->SelectObject(&b);
	pDC->Rectangle(x1,y1,x2,y2);				

	CBrush b1(DEFAULT_BG_COLOR);				// �e�¦� progrss����
	pDC->SelectObject(&b1);
	pDC->Rectangle(progress_x1,progress_y1,progress_x2_end,progress_y2);

	CBrush b2(RGB(255,255,0));					// �e���� progrss�i��
	pDC->SelectObject(&b2);
	pDC->Rectangle(progress_x1,progress_y1,progress_x2,progress_y2);

	pDC->SelectObject(pp);						// ���� pen
	pDC->SelectObject(pb);						// ���� brush
	CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
	//
	// �p�G�O�O���a��Ψ�CDC���ܡA���n�ۥH�U�o��A�_�h�ù��|�{�{
	//
	CDDraw::BltBackToPrimary();					// �N Back Plain �K��ù�
}

void CGameState::OnDraw() // Template Method
{
	OnShow();
}

void CGameState::OnCycle() // Template Method
{
	OnMove();
	OnShow();
}

/////////////////////////////////////////////////////////////////////////////
// CGame: Game Class
// �o��class�O�C����facade�A�OMFC�P�U�ӹC�����A�����١A�p�G���W�[�δ��
// �C�����A���ܡA�i�H���κ޳o��class�������P��@�C
/////////////////////////////////////////////////////////////////////////////

CGame CGame::instance;

CGame::CGame()
	: NUM_GAME_STATES(4)
{
	running = true;
	suspended = false;
	gameStateTable[GAME_STATE_INIT] = new CGameStateInit(this);
	gameStateTable[GAME_STATE_MENU] = new CGameStateMenu(this);
	gameStateTable[GAME_STATE_RUN] = new CGameStateRun(this);
	gameStateTable[GAME_STATE_OVER] = new CGameStateOver(this);
	gameState = NULL;
}

CGame::~CGame()
{
	for (int i = 0; i < NUM_GAME_STATES; i++)
		delete gameStateTable[i];
}

CGame *CGame::Instance()
{
	return &instance;
}

bool CGame::IsRunning()
{
	return running;
}

void CGame::OnDraw()
{
	CDDraw::BltBackColor(DEFAULT_BG_COLOR);	// �N Back Plain ���
	gameState->OnDraw();					// ��ܹC�������C�Ӥ���
	if (!running) {
		//
		// �p�G�b�Ȱ����A�A�h���Ctrl-Q...
		//
		CMovingBitmap bmp;
		bmp.LoadBitmap(IDB_CONTINUE);
		bmp.SetTopLeft(0,0);
		bmp.ShowBitmap();
	}
	CDDraw::BltBackToPrimary();				// �N Back Plain �K��ù�
}

void  CGame::OnFilePause()
{
	if (ENABLE_GAME_PAUSE) {
		if (running)
			CAudio::Instance()->Pause();
		else
			CAudio::Instance()->Resume();
		running = !running;
	} else {
		CAudio::Instance()->Resume();
		running = true;
	}
}

bool CGame::OnIdle()  // �ק�\�ण�n�ק�OnIdle()�A�����ק�OnMove()��OnShow()
{
	if (suspended) {
		running = false;
		suspended = false;
	}
	//
	// ����C���O�_�Ȱ�
	//
	if (!running)
		return false;
	//
	// �H�U�O�C�����D�j��
	//
	CDDraw::BltBackColor(DEFAULT_BG_COLOR);	// �N Back Plain ��W�w�]���C��
	gameState->OnCycle();
	CDDraw::BltBackToPrimary();				// �N Back Plain �K��ù�
	//
	// �H�U���{������C���i�檺�t�סA�`�N�ƶ��G
	// 1. ��Debug mode�i�H�˵��C�@���j��ᱼ���ɶ��A�O���ɶ���t�C
	// 2. �q�W�����}OnIdle()�ܦ��A�ɶ��w��33ms�A���i�R���A��ɶ����i�C��t�C
	//
	if (SHOW_GAME_CYCLE_TIME)
		TRACE("Ellipse time for the %d th cycle=%d \n", CSpecialEffect::GetCurrentTimeCount(),CSpecialEffect::GetEllipseTime());
	CSpecialEffect::DelayFromSetCurrentTime(GAME_CYCLE_TIME);
	CSpecialEffect::SetCurrentTime();	// �]�w���}OnIdle()���ɶ�
	return true;
}

void CGame::OnInit()	// OnInit() �u�b�{���@�}�l�ɰ���@��
{
	//
	// �Ұʶü�
	//
	srand((unsigned)time(NULL));
	//
	// �}��DirectXø�Ϥ���
	//
	CDDraw::Init(SIZE_X, SIZE_Y);							// �]�w�C���ѪR��
	//
	// �}��DirectX���Ĥ���
	//
	if (!CAudio::Instance()->Open())						// �}�ҭ��Ĥ���
		AfxMessageBox("Audio Interface Failed (muted)");	// �L���Ĥ���
	//
	// Switch to the first state
	//
	gameState = gameStateTable[GAME_STATE_INIT];
	gameState->OnBeginState();
	CSpecialEffect::SetCurrentTime();
	running = true;
}

void CGame::OnInitStates()
{
	//
	// �I�s�C�Ӫ��A��OnInitialUpdate
	//
	for (int i = 0; i < NUM_GAME_STATES; i++)
		gameStateTable[i]->OnInit();
}

void CGame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (running)
		if ((nFlags & 0x4000) == 0) // �h��auto repeat
			gameState->OnKeyDown(nChar, nRepCnt, nFlags);
#ifdef _UNITTEST					// invike unit test if _UNITTEST is defined
	void runTest();
	if (nChar == 'T')
		runTest();
#endif
}

void CGame::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (running)
		gameState->OnKeyUp(nChar, nRepCnt, nFlags);
}

void CGame::OnKillFocus()
{
	CAudio::Instance()->Pause();
	if (ENABLE_GAME_PAUSE)
		running = false;
	else if (CDDraw::IsFullScreen())
		running = false;
}

void CGame::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (running)
		gameState->OnLButtonDown(nFlags, point);
}

void CGame::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (running)
		gameState->OnRButtonDown(nFlags, point);
}

void CGame::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (running)
		gameState->OnLButtonUp(nFlags, point);
}

void CGame::OnMouseMove(UINT nFlags, CPoint point)
{
	if (running)
		gameState->OnMouseMove(nFlags, point);
}

void CGame::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (running)
		gameState->OnRButtonUp(nFlags, point);
}

void CGame::OnResume()
{
	//
	// Note: the resume message is not synchronized with the other messages
	//
}

void CGame::OnSetFocus()
{
	if (!ENABLE_GAME_PAUSE) {
		CAudio::Instance()->Resume();
		running = true;
	}
}

void CGame::OnSuspend()
{
	//
	// Note: the suspend message is not synchronized with the other messages
	//
	suspended = true;
	CAudio::Instance()->SetPowerResume();
}

void CGame::SetGameState(int state)
{
	ASSERT(state >=0 && state < NUM_GAME_STATES);
	gameState = gameStateTable[state];
	gameState->OnBeginState();
	OnDraw();
	CSpecialEffect::SetCurrentTime();
	running = true;
}

ScoreCount CGame::GetScoreInfo(int index) {
	return gameStateTable[GAME_STATE_RUN]->GetRecordInfo(index);
}

/////////////////////////////////////////////////////////////////////////////
// CSpecialEffect: Specail Effect functions
// �@�몺�C���ä��ݪ����ާ@�o�Ӫ���A�]���i�H�������L����
/////////////////////////////////////////////////////////////////////////////

DWORD CSpecialEffect::ctime=0;
int   CSpecialEffect::ctimeCount=0;

void CSpecialEffect::Delay(DWORD ms)
{
	DWORD ctime = timeGetTime();
	int waitms;
	waitms = ms - (timeGetTime() - ctime);
	if (waitms > 0)
		Sleep(waitms);
}

void CSpecialEffect::DelayFromSetCurrentTime(DWORD ms)
{
	int waitms;
	waitms = ms - (timeGetTime() - ctime);
	if (waitms > 0)
		Sleep(waitms);
}

void CSpecialEffect::SetCurrentTime()
{
	ctime = timeGetTime();
	ctimeCount++;
}

DWORD CSpecialEffect::GetEllipseTime()
{
	return timeGetTime()-ctime;
}

int CSpecialEffect::GetCurrentTimeCount()
{
	return ctimeCount;
}