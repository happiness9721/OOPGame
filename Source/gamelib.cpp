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
#include <ddraw.h>
#include <direct.h>
#include <string.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"

namespace game_framework {

/////////////////////////////////////////////////////////////////////////////
// CAnimation: ���Ѱʵe����O
// 1. �n���o���I�s(�B��)��U�د�O�A���O�i�H�����U�C���{���O����N��
// 2. �ۤv�g��B��CMovingBitmap���{���ɡA�i�H�ѦҤU�C�{�����g�k
// 3. �p�G�ݭn�ק���X�RCAnimation���\��ɡA�Х��~�өΪ����������覡�A�̦n
//    ���n������CAnimation�C
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

int CAnimation::GetCurrentBitmapNumber()
{
	return bmp_counter;
}

int CAnimation::Height()
{
	GAME_ASSERT(bmp.size() != 0,"CAnimation: Bitmaps must be loaded first.");
	return bmp_iter->Height();
}

bool CAnimation::IsFinalBitmap()
{
	GAME_ASSERT(bmp.size() != 0,"CAnimation: Bitmaps must be loaded first.");
	return bmp_counter == (bmp.size()-1);
}

int CAnimation::Left()
{
	GAME_ASSERT(bmp.size() != 0,"CAnimation: Bitmaps must be loaded first.");
	return x;
}

void CAnimation::OnMove()
{
	GAME_ASSERT(bmp.size() != 0,"CAnimation: Bitmaps must be loaded first.");
	if (--delay_counter <= 0)  {
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
	GAME_ASSERT(bmp.size() != 0,"CAnimation: Bitmaps must be loaded first.");
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
	GAME_ASSERT(bmp.size() != 0,"CAnimation: Bitmaps must be loaded before they are shown.");
	bmp_iter->SetTopLeft(x,y);
	bmp_iter->ShowBitmap();
}

int CAnimation::Top()
{
	GAME_ASSERT(bmp.size() != 0,"CAnimation: Bitmaps must be loaded first.");
	return y;
}

int CAnimation::Width()
{
	GAME_ASSERT(bmp.size() != 0,"CAnimation: Bitmaps must be loaded first.");
	return bmp_iter->Width();
}

/////////////////////////////////////////////////////////////////////////////
// CInteger: �o��class������ܾ�ƹϧΪ���O
// 1. �n���o���I�s(�B��)��U�د�O�A���O�i�H�����U�C���{���O����N��
// 2. �ۤv�g��B��CMovingBitmap���{���ɡA�i�H�ѦҤU�C�{�����g�k
/////////////////////////////////////////////////////////////////////////////

CMovingBitmap CInteger::digit[11];
bool CInteger::isBmpLoaded = false;
CInteger::CInteger(int digits)
: NUMDIGITS(digits)
{
	isBmpLoaded = false;
}

void CInteger::Add(int x)
{
	n += x;
}

int CInteger::GetInteger()
{
	return n;
}

void CInteger::ChangeDigit(int index)
{
	NUMDIGITS = index;
}

void CInteger::LoadBitmap()
{
	//
	// digit[i]��class varibale�A�ҥH�����קK����LoadBitmap
	//
	if (!isBmpLoaded) {
		int d[11]={IDB_0,IDB_1,IDB_2,IDB_3,IDB_4,IDB_5,IDB_6,IDB_7,IDB_8,IDB_9,IDB_MINUS};
		for (int i=0; i < 11; i++)
			digit[i].LoadBitmap(d[i],RGB(255,255,255));
		isBmpLoaded = true;
	}
}

void CInteger::SetInteger(int i)
{
	n = i;
}

void CInteger::SetTopLeft(int nx, int ny)		// �N�ʵe�����W���y�в��� (x,y)
{
	x = nx; y = ny;
}

void CInteger::ShowBitmap()
{
	GAME_ASSERT(isBmpLoaded, "CInteger: �Х�����LoadBitmap�A�M��~��ShowBitmap");
	int nx;		// ����ܦ�ƪ� x �y��
	int MSB;	// �̥���(�t�Ÿ�)����ƪ��ƭ�
	if (n >= 0) {
		MSB = n;
		nx = x+digit[0].Width()*(NUMDIGITS-1);
	} else {
		MSB = -n;
		nx = x+digit[0].Width()*NUMDIGITS;
	}
	for (int i=0; i < NUMDIGITS; i++) {
		int d = MSB % 10;
		MSB /= 10;
		digit[d].SetTopLeft(nx, y);
		digit[d].ShowBitmap();
		nx -= digit[d].Width();
	}
	if (n < 0) { // �p�G�p��0�A�h��ܭt��
		digit[10].SetTopLeft(nx, y);
		digit[10].ShowBitmap();
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMovingBitmap: Moving Bitmap class
// �o��class���ѥi�H���ʪ��ϧ�
// �n���o���I�s(�B��)��U�د�O�A���O�i�H�����U�C���{���O����N��
/////////////////////////////////////////////////////////////////////////////

CMovingBitmap::CMovingBitmap()
{
	isBitmapLoaded = false;
}

int CMovingBitmap::Height()
{
	GAME_ASSERT(isBitmapLoaded,"A bitmap must be loaded before Height() is called !!!");
	return location.bottom - location.top;
}

int CMovingBitmap::Left()
{
	GAME_ASSERT(isBitmapLoaded,"A bitmap must be loaded before Left() is called !!!");
	return location.left;
}

void CMovingBitmap::LoadBitmap(int IDB_BITMAP, COLORREF color)
{
	const int nx = 0;
	const int ny = 0;
	GAME_ASSERT(!isBitmapLoaded,"A bitmap has been loaded. You can not load another bitmap !!!");
	CBitmap bitmap;
	BOOL rval = bitmap.LoadBitmap(IDB_BITMAP);
	GAME_ASSERT(rval,"Load bitmap failed !!! Please check bitmap ID (IDB_XXX).");
	BITMAP bitmapSize;
	bitmap.GetBitmap(&bitmapSize);
	location.left = nx; location.top = ny;
	location.right = nx+bitmapSize.bmWidth;
	location.bottom = ny+bitmapSize.bmHeight;
	SurfaceID = CDDraw::RegisterBitmap(IDB_BITMAP, color);
	isBitmapLoaded = true;
}

void CMovingBitmap::LoadBitmap(char *filename, COLORREF color)
{
	const int nx = 0;
	const int ny = 0;
	GAME_ASSERT(!isBitmapLoaded,"A bitmap has been loaded. You can not load another bitmap !!!");
	HBITMAP hbitmap = (HBITMAP)LoadImage(NULL,filename,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	if (hbitmap == NULL) {
		char error_msg[300];
		sprintf(error_msg, "Loading bitmap from file \"%s\" failed !!!", filename);
		GAME_ASSERT(false, error_msg);
	}
	CBitmap *bmp = CBitmap::FromHandle(hbitmap ); // memory will be deleted automatically
	BITMAP bitmapSize;
	bmp->GetBitmap(&bitmapSize);
	location.left = nx; location.top = ny;
	location.right = nx+bitmapSize.bmWidth;
	location.bottom = ny+bitmapSize.bmHeight;
	SurfaceID = CDDraw::RegisterBitmap(filename, color);
	isBitmapLoaded = true;
}

void CMovingBitmap::SetTopLeft(int x, int y)
{
	GAME_ASSERT(isBitmapLoaded,"A bitmap must be loaded before SetTopLeft() is called !!!");
	int dx = location.left - x;
	int dy = location.top - y;
	location.left = x;
	location.top = y;
	location.right -= dx;
	location.bottom -= dy;
}

void CMovingBitmap::ShowBitmap()
{
	GAME_ASSERT(isBitmapLoaded,"A bitmap must be loaded before ShowBitmap() is called !!!");
	CDDraw::BltBitmapToBack(SurfaceID,location.left,location.top);
}

void CMovingBitmap::ShowBitmap(double factor)
{
	GAME_ASSERT(isBitmapLoaded,"A bitmap must be loaded before ShowBitmap() is called !!!");
	CDDraw::BltBitmapToBack(SurfaceID,location.left,location.top,factor);
}

void CMovingBitmap::ShowBitmap(CMovingBitmap &bm)
{
	GAME_ASSERT(isBitmapLoaded,"A bitmap must be loaded before ShowBitmap() is called !!!");
	GAME_ASSERT(bm.isBitmapLoaded,"A bitmap must be loaded before ShowBitmap() is called !!!");
	CDDraw::BltBitmapToBitmap(SurfaceID, bm.SurfaceID, location.left,location.top);
}

int CMovingBitmap::Top()
{
	GAME_ASSERT(isBitmapLoaded,"A bitmap must be loaded before Top() is called !!!");
	return location.top;
}

int CMovingBitmap::Width()
{
	GAME_ASSERT(isBitmapLoaded,"A bitmap must be loaded before Width() is called !!!");
	return location.right - location.left;
}

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

/////////////////////////////////////////////////////////////////////////////
// CDDraw: Direct Draw Object
// �o��class�|�إ�DirectDraw����A�H���Ѩ�Lclass�ϥ�
// �o��class�������{�����O�C����ø�Ϥ����A�i�H�������L����
/////////////////////////////////////////////////////////////////////////////

HDC							CDDraw::hdc;
CDC							CDDraw::cdc;
CView						*CDDraw::pCView;
LPDIRECTDRAW2				CDDraw::lpDD;
LPDIRECTDRAWCLIPPER			CDDraw::lpClipperPrimary;   
LPDIRECTDRAWCLIPPER			CDDraw::lpClipperBack;   
LPDIRECTDRAWSURFACE			CDDraw::lpDDSPrimary;
LPDIRECTDRAWSURFACE			CDDraw::lpDDSBack;
HRESULT						CDDraw::ddrval;
bool						CDDraw::fullscreen;
int							CDDraw::size_x, CDDraw::size_y;
CDDraw						CDDraw::ddraw;
vector<int>					CDDraw::BitmapID;
vector<string>				CDDraw::BitmapName;
vector<CRect>				CDDraw::BitmapRect;
vector<COLORREF>			CDDraw::BitmapColorKey;
vector<LPDIRECTDRAWSURFACE>	CDDraw::lpDDS;

CDDraw::CDDraw()
{
	pCView=NULL;
	lpClipperPrimary=lpClipperBack=NULL;
	lpDDSPrimary=lpDDSBack=NULL;
	fullscreen = OPEN_AS_FULLSCREEN;
	lpDD=NULL;
}

CDDraw::~CDDraw()
{
	ReleaseSurface();
	if (lpDD)
		lpDD->Release();
	pCView=NULL; lpDD=NULL;
	lpClipperPrimary=lpClipperBack=NULL;
	lpDDSPrimary=lpDDSBack=NULL;
	TRACE("~CDDraw()\n");
}

void CDDraw::BltBackColor(DWORD color)
{
	if (lpDDSBack) {
		if (lpDDSBack->IsLost())
			RestoreSurface();
		DDBLTFX ddbltfx;
		ddbltfx.dwSize = sizeof(ddbltfx);
		ddbltfx.dwFillColor = MatchColorKey(lpDDSBack,color);
		ddrval = lpDDSBack->Blt(NULL,NULL,NULL,DDBLT_COLORFILL|DDBLT_WAIT,&ddbltfx);
		CheckDDFail("BltBackColor: blitting failed");
	}
}

void CDDraw::BltBackToPrimary()
{
	if (!pCView || !lpDDSPrimary || !lpDDSBack)
		return;
	if (fullscreen) {
		CRect ClientRect;
		GetClientRect(ClientRect);
		if (lpDDSBack->IsLost())
			RestoreSurface();
		if (lpDDSPrimary->IsLost())
			RestoreSurface();
		ddrval = lpDDSPrimary->Blt(ClientRect, lpDDSBack, ClientRect, DDBLTFAST_WAIT, NULL);
		CheckDDFail("Blt Back to primary failed");
	} else {
		if (lpDDSBack->IsLost())
			RestoreSurface();
		if (lpDDSPrimary->IsLost())
			RestoreSurface();
		CRect ClientRect;
		GetClientRect(ClientRect);
		CRect WindowRect;
		pCView->GetWindowRect(&WindowRect);
		WindowRect.right = WindowRect.left + size_x;
		WindowRect.bottom = WindowRect.top + size_y;
		ddrval = lpDDSPrimary->Blt(WindowRect, lpDDSBack, ClientRect, DDBLTFAST_WAIT, NULL);
		CheckDDFail("Blt Back to primary failed");
	}
}

void CDDraw::BltBitmapToBack(unsigned SurfaceID, int x, int y)
{
	GAME_ASSERT(lpDDSBack && (SurfaceID < lpDDS.size()) && lpDDS[SurfaceID], "Internal Error: Incorrect SurfaceID in BltBitmapToBack");
	CRect TargetRect;
	TargetRect.left = x;
	TargetRect.top = y;
	TargetRect.right = x + BitmapRect[SurfaceID].right-BitmapRect[SurfaceID].left;
	TargetRect.bottom = y + BitmapRect[SurfaceID].bottom-BitmapRect[SurfaceID].top;
	int blt_flag;
	if (BitmapColorKey[SurfaceID] != CLR_INVALID)
		blt_flag = DDBLT_WAIT | DDBLT_KEYSRC;
	else
		blt_flag = DDBLT_WAIT;
	if (lpDDSBack->IsLost())
		RestoreSurface();
	if (lpDDS[SurfaceID]->IsLost())
		RestoreSurface();
	ddrval = lpDDSBack->Blt(TargetRect, lpDDS[SurfaceID],NULL, blt_flag, NULL);
	CheckDDFail("Blt Bitmap to Back Failed");
}

void CDDraw::BltBitmapToBack(unsigned SurfaceID, int x, int y, double factor)
{
	GAME_ASSERT(lpDDSBack && (SurfaceID < lpDDS.size()) && lpDDS[SurfaceID], "Internal Error: Incorrect SurfaceID in BltBitmapToBack");
	CRect TargetRect;
	TargetRect.left = x;
	TargetRect.top = y;
	TargetRect.right = x + (int) ((BitmapRect[SurfaceID].right-BitmapRect[SurfaceID].left)*factor);
	TargetRect.bottom = y + (int) ((BitmapRect[SurfaceID].bottom-BitmapRect[SurfaceID].top)*factor);
	int blt_flag;
	if (BitmapColorKey[SurfaceID] != CLR_INVALID)
		blt_flag = DDBLT_WAIT | DDBLT_KEYSRC;
	else
		blt_flag = DDBLT_WAIT;
	if (lpDDSBack->IsLost())
		RestoreSurface();
	if (lpDDS[SurfaceID]->IsLost())
		RestoreSurface();
	ddrval = lpDDSBack->Blt(TargetRect, lpDDS[SurfaceID],NULL, blt_flag, NULL);
	CheckDDFail("Blt Bitmap to Back Failed");
}

void CDDraw::BltBitmapToBitmap(unsigned SourceID, unsigned TargetID, int x, int y)
{
	GAME_ASSERT((SourceID < lpDDS.size()) && (TargetID < lpDDS.size()) && (SourceID != TargetID), "Internal Error: Incorrect SourceID in BltBitmapToBitmap");
	int blt_flag;
	if (BitmapColorKey[SourceID] != CLR_INVALID)
		blt_flag = DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY;
	else
		blt_flag = DDBLTFAST_WAIT;
	if (lpDDS[SourceID]->IsLost())
		RestoreSurface();
	if (lpDDS[TargetID]->IsLost())
		RestoreSurface();
	ddrval = lpDDS[TargetID]->BltFast(x, y, lpDDS[SourceID], NULL, blt_flag );
	CheckDDFail("Blt Bitmap to Back Failed");
}

bool CDDraw::CreateSurface()
{
	//
	// Since all surfaces are created and loaded, this operation can be very slow.
	//
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
	ReleaseSurface();
	if (fullscreen) {
		if (!CreateSurfaceFullScreen())
			return false;
	} else {
		if (!CreateSurfaceWindowed())
			return false;
	}
	for (unsigned i = 0; i < lpDDS.size(); i++) {
		if (BitmapID[i] != -1) // from resource
			LoadBitmap(i, BitmapID[i]);
		else
			LoadBitmap(i, (char *) BitmapName[i].c_str()); // from file
		SetColorKey(i, BitmapColorKey[i]);
	}
	return true;
}

bool CDDraw::CreateSurfaceFullScreen()
{
    ddrval = lpDD->SetCooperativeLevel(AfxGetMainWnd()->m_hWnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN);
    CheckDDFail("Can not SetCooperativeLevel Exclusive");
    ddrval = lpDD->SetDisplayMode(size_x, size_y, 32, 0, 0);
	if (ddrval != DD_OK) {
	    ddrval = lpDD->SetCooperativeLevel(AfxGetMainWnd()->m_hWnd, DDSCL_NORMAL);
		CheckDDFail("Can not SetCooperativeLevel Normal");
		return false;
	}
	CheckDDFail("SetDisplayMode FAILED");

	DDSURFACEDESC ddsd;
	ZeroMemory(&ddsd,sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    ddrval = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
	CheckDDFail("Create Primary Surface failed");

	// Create clippers for the primary and back surfaces
    // ddrval = lpDD->CreateClipper(0, &lpClipperPrimary, NULL);
	// CheckDDFail("Create Primay Surface Clipper FAILED"); 
    ddrval = lpDD->CreateClipper(0, &lpClipperBack, NULL);
	CheckDDFail("Create Back Surface Clipper FAILED"); 

	typedef struct {
		RGNDATAHEADER hdr;
		RECT rgndata[4]; 
    } CLIPLIST, *LPCLIPLIST; 
	CLIPLIST    ClipList; 
	RECT rc;
	SetRect(&rc, 0, 0, size_x, size_y);  
    ClipList.hdr.dwSize = sizeof(RGNDATAHEADER); 
    ClipList.hdr.iType = RDH_RECTANGLES;
	ClipList.hdr.nCount = 1; 
    ClipList.hdr.nRgnSize = 0; 
    memcpy(&ClipList.hdr.rcBound, &rc, sizeof(RECT)); 
    memcpy(&ClipList.rgndata, &rc, sizeof(RECT)); 
	ddrval = lpClipperBack->SetClipList((LPRGNDATA)&ClipList,0); 
	CheckDDFail("SetHWnd FAILED"); 

	// Create Back (Secondary) Surface
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwHeight = size_y; ddsd.dwWidth = size_x;
    ddrval = lpDD->CreateSurface( &ddsd, &lpDDSBack, NULL );
	CheckDDFail("Can not create back plain");

	// In fullscreen mode, the clipper for primary surafce is fixed to screen size
	ddrval = lpDDSPrimary->SetClipper(lpClipperBack);
	CheckDDFail("SetClipper FAILED");  
    ddrval = lpDDSBack->SetClipper(lpClipperBack);
	CheckDDFail("SetClipper FAILED");  
	lpClipperBack->Release();

	BltBackColor(RGB(0,0,0));
	BltBackToPrimary();
	return true;
}

bool CDDraw::CreateSurfaceWindowed()
{
    ddrval = lpDD->SetCooperativeLevel(AfxGetMainWnd()->m_hWnd, DDSCL_NORMAL);
    CheckDDFail("Can not SetCooperativeLevel ");

	DDSURFACEDESC ddsd;
	ZeroMemory(&ddsd,sizeof(ddsd));
    ddsd.dwSize = sizeof(ddsd);
    ddsd.dwFlags = DDSD_CAPS;
    ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
    ddrval = lpDD->CreateSurface( &ddsd, &lpDDSPrimary, NULL );
	CheckDDFail("Create Primary Surface failed");

	// Create clippers for the primary and back surfaces
    ddrval = lpDD->CreateClipper(0, &lpClipperPrimary, NULL);
	CheckDDFail("Create Primay Surface Clipper FAILED"); 
    ddrval = lpDD->CreateClipper(0, &lpClipperBack, NULL);
	CheckDDFail("Create Back Surface Clipper FAILED"); 

    ddrval = lpClipperPrimary->SetHWnd(0, AfxGetMainWnd()->m_hWnd);
	CheckDDFail("Primary Surface SetHWnd FAILED"); 

	typedef struct {
		RGNDATAHEADER hdr;
		RECT rgndata[4]; 
    } CLIPLIST, *LPCLIPLIST; 
	CLIPLIST    ClipList; 
	RECT rc;
	SetRect(&rc, 0, 0, size_x, size_y);  
    ClipList.hdr.dwSize = sizeof(RGNDATAHEADER); 
    ClipList.hdr.iType = RDH_RECTANGLES;
	ClipList.hdr.nCount = 1; 
    ClipList.hdr.nRgnSize = 0; 
    memcpy(&ClipList.hdr.rcBound, &rc, sizeof(RECT)); 
    memcpy(&ClipList.rgndata, &rc, sizeof(RECT)); 
	ddrval = lpClipperBack->SetClipList((LPRGNDATA)&ClipList,0); 
	CheckDDFail("SetHWnd FAILED"); 

	// Create Back (Secondary) Surface
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
    ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwHeight = size_y; ddsd.dwWidth = size_x;
    ddrval = lpDD->CreateSurface( &ddsd, &lpDDSBack, NULL );
	CheckDDFail("Can not create back plain");

	ddrval = lpDDSPrimary->SetClipper(lpClipperPrimary);
	CheckDDFail("SetClipper FAILED");  
	lpClipperPrimary->Release();
    ddrval = lpDDSBack->SetClipper(lpClipperBack);
	CheckDDFail("SetClipper FAILED");  
	lpClipperBack->Release();

	BltBackColor(RGB(0,0,0));
	return true;
}

void CDDraw::GetClientRect(CRect &r)
{
	r = CRect(0,0,size_x,size_y);
}

void CDDraw::Init(int sx, int sy)
{
	// set target screen size
	size_x = sx, size_y = sy;
	// init lpDD
	LPDIRECTDRAW lpDD0;
    ddrval = DirectDrawCreate(NULL, &lpDD0, NULL);
	CheckDDFail("DDraw create failed");
	ddrval = lpDD0->QueryInterface(IID_IDirectDraw2, (void **) &lpDD);
	CheckDDFail("DDraw surface 2 create failed");
	// init pCView
	POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();
	CDocTemplate *doc_t = AfxGetApp()->GetNextDocTemplate(pos);
	pos = doc_t->GetFirstDocPosition();
	CDocument *doc = doc_t->GetNextDoc(pos);
	pos = doc->GetFirstViewPosition();
	pCView = doc->GetNextView(pos);
	// init surfaces
	SetFullScreen(fullscreen);
}

bool CDDraw::IsFullScreen()
{
	return fullscreen;
}

void CDDraw::LoadBitmap(int i, int IDB_BITMAP)
{
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_BITMAP);
	CDC mDC;
	mDC.CreateCompatibleDC(NULL);
	CBitmap* pOldBitmap = mDC.SelectObject(&bitmap);
	BITMAP bitmapSize;
	bitmap.GetBitmap(&bitmapSize);
	DDSURFACEDESC ddsd;
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof( ddsd );
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	BitmapRect[i].bottom = ddsd.dwHeight = bitmapSize.bmHeight;
	BitmapRect[i].right = ddsd.dwWidth = bitmapSize.bmWidth;
	ddrval = lpDD->CreateSurface(&ddsd, &lpDDS[i], NULL);
	CheckDDFail("Create Bitmap Surface Failed");
	HDC hdc;
    ddrval= lpDDS[i]->GetDC(&hdc);
	CheckDDFail("Get surface HDC failed");
	CDC cdc;
	cdc.Attach(hdc);
	cdc.BitBlt(0,0,bitmapSize.bmWidth,bitmapSize.bmHeight, &mDC,0,0,SRCCOPY);
	cdc.Detach();
	lpDDS[i]->ReleaseDC(hdc);
	// avoid memory leak
	// According to spec, mDC should delete itself automatically.  However,
	// it appears that we have to do it explictly.
	mDC.SelectObject(&pOldBitmap);	
	mDC.DeleteDC();
	bitmap.DeleteObject();
}

void CDDraw::LoadBitmap(int i, char *filename)
{

	HBITMAP hbitmap = (HBITMAP)LoadImage(NULL,filename,IMAGE_BITMAP,0,0,LR_LOADFROMFILE);
	GAME_ASSERT(hbitmap != NULL,"Load bitmap failed !!! Please check bitmap ID (IDB_XXX).");
	CBitmap *bmp = CBitmap::FromHandle(hbitmap ); // will be deleted automatically
	CDC mDC;
	mDC.CreateCompatibleDC(NULL);
	CBitmap* pOldBitmap = mDC.SelectObject(bmp);
	BITMAP bitmapSize;
	bmp->GetBitmap(&bitmapSize);
	DDSURFACEDESC ddsd;
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof( ddsd );
	ddsd.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
	BitmapRect[i].bottom = ddsd.dwHeight = bitmapSize.bmHeight;
	BitmapRect[i].right = ddsd.dwWidth = bitmapSize.bmWidth;
	ddrval = lpDD->CreateSurface(&ddsd, &lpDDS[i], NULL);
	CheckDDFail("Create Bitmap Surface Failed");
	HDC hdc;
    ddrval= lpDDS[i]->GetDC(&hdc);
	CheckDDFail("Get surface HDC failed");
	CDC cdc;
	cdc.Attach(hdc);
	cdc.BitBlt(0,0,bitmapSize.bmWidth,bitmapSize.bmHeight, &mDC,0,0,SRCCOPY);
	cdc.Detach();
	lpDDS[i]->ReleaseDC(hdc);
	// avoid memory leak
	// According to spec, mDC should delete itself automatically.  However,
	// it appears that we have to do it explictly.
	mDC.SelectObject(&pOldBitmap);	
	mDC.DeleteDC();
	bmp->DeleteObject();
}

DWORD CDDraw::MatchColorKey(LPDIRECTDRAWSURFACE lpDDSurface, COLORREF color)
{
    DDSURFACEDESC ddsd;
	HDC hdc;
	HRESULT hres;
	COLORREF rgbT= CLR_INVALID;
	DWORD dw=CLR_INVALID,mask=(DWORD)~0;
	if (lpDDSurface && color != CLR_INVALID) {
		if (lpDDSurface->GetDC(&hdc) == DD_OK) {
			rgbT = GetPixel(hdc, 0, 0);             // save (0,0) pixel value
			SetPixel(hdc, 0, 0, color);             // set our value
			lpDDSurface->ReleaseDC(hdc);
		}
		ddsd.dwSize = sizeof(ddsd);
		while ((hres = lpDDSurface->Lock(NULL, &ddsd, 0, NULL)) == DDERR_WASSTILLDRAWING);
		if (hres == DD_OK) {
			dw = *(DWORD *)ddsd.lpSurface;          // get (0,0) data
			if (ddsd.ddpfPixelFormat.dwRGBBitCount < 32)
				mask = (1 << ddsd.ddpfPixelFormat.dwRGBBitCount)-1;
			dw &= mask;								// mask it to bpp
			lpDDSurface->Unlock(NULL);
		}
		if (lpDDSurface->GetDC(&hdc) == DD_OK)	{
			SetPixel(hdc, 0, 0, rgbT);				// restore (0,0) pixel value
			lpDDSurface->ReleaseDC(hdc);
		}
	}
	return dw;
}

CDC* CDDraw::GetBackCDC()
{
	if (lpDDSBack->IsLost())
		RestoreSurface();
    ddrval= lpDDSBack->GetDC(&hdc);
	CheckDDFail("Get back surface HDC failed");
	cdc.Attach(hdc);
	return &cdc;
}

int CDDraw::RegisterBitmap(int IDB_BITMAP, COLORREF ColorKey)
{
	unsigned i;
	for (i = 0; i < lpDDS.size(); i++)
		if (BitmapID[i] == IDB_BITMAP)
			return i;
	//
	// Enlarge the size of vectors
	//
	BitmapID.push_back(IDB_BITMAP);
	BitmapName.push_back("");
	BitmapColorKey.push_back(ColorKey);
	BitmapRect.push_back(CRect(0,0,0,0));
	lpDDS.push_back(NULL);
	LoadBitmap(i, IDB_BITMAP);
	SetColorKey(i, ColorKey);
	return i;
}
	
int CDDraw::RegisterBitmap(char *filename, COLORREF ColorKey)
{
	unsigned i;
	for (i = 0; i < lpDDS.size(); i++)
		if (BitmapName[i].compare(filename) == 0)
			return i;
	//
	// Enlarge the size of vectors
	//
	BitmapID.push_back(-1);
	BitmapName.push_back(filename);
	BitmapColorKey.push_back(ColorKey);
	BitmapRect.push_back(CRect(0,0,0,0));
	lpDDS.push_back(NULL);
	LoadBitmap(i, filename);
	SetColorKey(i, ColorKey);
	return i;
}

void CDDraw::ReleaseBackCDC()
{
	cdc.Detach();
	ddrval = lpDDSBack->ReleaseDC(hdc);
	CheckDDFail("Release back HDC failed");
}

void CDDraw::ReleaseSurface()
{
    if (lpDD)
    {
		for (unsigned i = 0; i < lpDDS.size(); i++)
			if (lpDDS[i]) {
				lpDDS[i]->Release();
				lpDDS[i] = NULL;
			}
        if (lpDDSBack)
        {
            lpDDSBack->Release();
            lpDDSBack = NULL;
        }
        if (lpDDSPrimary)
        {
            lpDDSPrimary->Release();
            lpDDSPrimary = NULL;
        }
	}
}

void CDDraw::RestoreSurface()
{
	//
	// Since all surfaces are restored and reloaded, this operation can be very slow.
	//
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
    if (lpDD != NULL)
    {
		CreateSurface();
		while (lpDDSBack->IsLost() || lpDDSPrimary->IsLost()) {
			Sleep(100);
			CreateSurface();
		}
    }
}

void CDDraw::SetColorKey(unsigned SurfaceID, COLORREF color)
{
	if (color != CLR_INVALID) {
		DDCOLORKEY ddck;
		ddck.dwColorSpaceLowValue  = MatchColorKey(lpDDS[SurfaceID], color);
		ddck.dwColorSpaceHighValue = ddck.dwColorSpaceLowValue;
		ddrval = lpDDS[SurfaceID]->SetColorKey(DDCKEY_SRCBLT, &ddck);
		CheckDDFail("Can not Set Color Key");
	}
}

bool CDDraw::SetFullScreen(bool isFullScreen)
{
	fullscreen = isFullScreen;
	return CreateSurface();
}

void CDDraw::CheckDDFail(char *s)
{
	if (ddrval != DD_OK) {
		TRACE("Error Code: %d (%s)\n", ddrval, s);
		//
		// For some unknown reason, in Win98, ddrval can be changed
		//	 after AfxMessageBox() is called, resulting an unknown
		//	 error message.
		//
		AfxMessageBox(s);
		static int ErrorCode[] = {
			DDERR_ALREADYINITIALIZED          ,DDERR_BLTFASTCANTCLIP             ,DDERR_CANNOTATTACHSURFACE         ,DDERR_CANNOTDETACHSURFACE         ,
			DDERR_CANTCREATEDC                ,DDERR_CANTDUPLICATE               ,DDERR_CANTLOCKSURFACE             ,DDERR_CANTPAGELOCK                ,
			DDERR_CANTPAGEUNLOCK              ,DDERR_CLIPPERISUSINGHWND          ,DDERR_COLORKEYNOTSET              ,DDERR_CURRENTLYNOTAVAIL           ,
			DDERR_DCALREADYCREATED            ,DDERR_DIRECTDRAWALREADYCREATED    ,DDERR_EXCEPTION                   ,DDERR_EXCLUSIVEMODEALREADYSET     ,
			DDERR_GENERIC                     ,DDERR_HEIGHTALIGN                 ,DDERR_HWNDALREADYSET              ,DDERR_HWNDSUBCLASSED              ,
			DDERR_IMPLICITLYCREATED           ,DDERR_INCOMPATIBLEPRIMARY         ,DDERR_INVALIDCAPS                 ,DDERR_INVALIDCLIPLIST             ,
			DDERR_INVALIDDIRECTDRAWGUID       ,DDERR_INVALIDMODE                 ,DDERR_INVALIDOBJECT               ,DDERR_INVALIDPARAMS               ,
			DDERR_INVALIDPIXELFORMAT          ,DDERR_INVALIDPOSITION             ,DDERR_INVALIDRECT                 ,DDERR_INVALIDSURFACETYPE          ,
			DDERR_LOCKEDSURFACES              ,DDERR_NO3D                        ,DDERR_NOALPHAHW                   ,DDERR_NOBLTHW                     ,
			DDERR_NOCLIPLIST                  ,DDERR_NOCLIPPERATTACHED           ,DDERR_NOCOLORCONVHW               ,DDERR_NOCOLORKEY                  ,
			DDERR_NOCOLORKEYHW                ,DDERR_NOCOOPERATIVELEVELSET       ,DDERR_NODC                        ,DDERR_NODDROPSHW                  ,
			DDERR_NODIRECTDRAWHW              ,DDERR_NODIRECTDRAWSUPPORT         ,DDERR_NOEMULATION                 ,DDERR_NOEXCLUSIVEMODE             ,
			DDERR_NOFLIPHW                    ,DDERR_NOGDI                       ,DDERR_NOHWND                      ,DDERR_NOMIPMAPHW                  ,
			DDERR_NOMIRRORHW                  ,DDERR_NOOVERLAYDEST               ,DDERR_NOOVERLAYHW                 ,DDERR_NOPALETTEATTACHED           ,
			DDERR_NOPALETTEHW                 ,DDERR_NORASTEROPHW                ,DDERR_NOROTATIONHW                ,DDERR_NOSTRETCHHW                 ,
			DDERR_NOT4BITCOLOR                ,DDERR_NOT4BITCOLORINDEX           ,DDERR_NOT8BITCOLOR                ,DDERR_NOTAOVERLAYSURFACE          ,
			DDERR_NOTEXTUREHW                 ,DDERR_NOTFLIPPABLE                ,DDERR_NOTFOUND                    ,DDERR_NOTINITIALIZED              ,
			DDERR_NOTLOCKED                   ,DDERR_NOTPAGELOCKED               ,DDERR_NOTPALETTIZED               ,DDERR_NOVSYNCHW                   ,
			DDERR_NOZBUFFERHW                 ,DDERR_NOZOVERLAYHW                ,DDERR_OUTOFCAPS                   ,DDERR_OUTOFMEMORY                 ,
			DDERR_OUTOFVIDEOMEMORY            ,DDERR_OVERLAYCANTCLIP             ,DDERR_OVERLAYCOLORKEYONLYONEACTIVE,DDERR_OVERLAYNOTVISIBLE           ,
			DDERR_PALETTEBUSY                 ,DDERR_PRIMARYSURFACEALREADYEXISTS ,DDERR_REGIONTOOSMALL              ,DDERR_SURFACEALREADYATTACHED      ,
			DDERR_SURFACEALREADYDEPENDENT     ,DDERR_SURFACEBUSY                 ,DDERR_SURFACEISOBSCURED           ,DDERR_SURFACELOST                 ,
			DDERR_SURFACENOTATTACHED          ,DDERR_TOOBIGHEIGHT                ,DDERR_TOOBIGSIZE                  ,DDERR_TOOBIGWIDTH                 ,
			DDERR_UNSUPPORTED                 ,DDERR_UNSUPPORTEDFORMAT           ,DDERR_UNSUPPORTEDMASK             ,DDERR_UNSUPPORTEDMODE             ,
			DDERR_VERTICALBLANKINPROGRESS     ,DDERR_WASSTILLDRAWING             ,DDERR_WRONGMODE                   ,DDERR_XALIGN
		};
		static char *ErrorMsg[] = {
			"DDERR_ALREADYINITIALIZED          ","DDERR_BLTFASTCANTCLIP             ","DDERR_CANNOTATTACHSURFACE         ","DDERR_CANNOTDETACHSURFACE         ",
			"DDERR_CANTCREATEDC                ","DDERR_CANTDUPLICATE               ","DDERR_CANTLOCKSURFACE             ","DDERR_CANTPAGELOCK                ",
			"DDERR_CANTPAGEUNLOCK              ","DDERR_CLIPPERISUSINGHWND          ","DDERR_COLORKEYNOTSET              ","DDERR_CURRENTLYNOTAVAIL           ",
			"DDERR_DCALREADYCREATED            ","DDERR_DIRECTDRAWALREADYCREATED    ","DDERR_EXCEPTION                   ","DDERR_EXCLUSIVEMODEALREADYSET     ",
			"DDERR_GENERIC                     ","DDERR_HEIGHTALIGN                 ","DDERR_HWNDALREADYSET              ","DDERR_HWNDSUBCLASSED              ",
			"DDERR_IMPLICITLYCREATED           ","DDERR_INCOMPATIBLEPRIMARY         ","DDERR_INVALIDCAPS                 ","DDERR_INVALIDCLIPLIST             ",
			"DDERR_INVALIDDIRECTDRAWGUID       ","DDERR_INVALIDMODE                 ","DDERR_INVALIDOBJECT               ","DDERR_INVALIDPARAMS               ",
			"DDERR_INVALIDPIXELFORMAT          ","DDERR_INVALIDPOSITION             ","DDERR_INVALIDRECT                 ","DDERR_INVALIDSURFACETYPE          ",
			"DDERR_LOCKEDSURFACES              ","DDERR_NO3D                        ","DDERR_NOALPHAHW                   ","DDERR_NOBLTHW                     ",
			"DDERR_NOCLIPLIST                  ","DDERR_NOCLIPPERATTACHED           ","DDERR_NOCOLORCONVHW               ","DDERR_NOCOLORKEY                  ",
			"DDERR_NOCOLORKEYHW                ","DDERR_NOCOOPERATIVELEVELSET       ","DDERR_NODC                        ","DDERR_NODDROPSHW                  ",
			"DDERR_NODIRECTDRAWHW              ","DDERR_NODIRECTDRAWSUPPORT         ","DDERR_NOEMULATION                 ","DDERR_NOEXCLUSIVEMODE             ",
			"DDERR_NOFLIPHW                    ","DDERR_NOGDI                       ","DDERR_NOHWND                      ","DDERR_NOMIPMAPHW                  ",
			"DDERR_NOMIRRORHW                  ","DDERR_NOOVERLAYDEST               ","DDERR_NOOVERLAYHW                 ","DDERR_NOPALETTEATTACHED           ",
			"DDERR_NOPALETTEHW                 ","DDERR_NORASTEROPHW                ","DDERR_NOROTATIONHW                ","DDERR_NOSTRETCHHW                 ",
			"DDERR_NOT4BITCOLOR                ","DDERR_NOT4BITCOLORINDEX           ","DDERR_NOT8BITCOLOR                ","DDERR_NOTAOVERLAYSURFACE          ",
			"DDERR_NOTEXTUREHW                 ","DDERR_NOTFLIPPABLE                ","DDERR_NOTFOUND                    ","DDERR_NOTINITIALIZED              ",
			"DDERR_NOTLOCKED                   ","DDERR_NOTPAGELOCKED               ","DDERR_NOTPALETTIZED               ","DDERR_NOVSYNCHW                   ",
			"DDERR_NOZBUFFERHW                 ","DDERR_NOZOVERLAYHW                ","DDERR_OUTOFCAPS                   ","DDERR_OUTOFMEMORY                 ",
			"DDERR_OUTOFVIDEOMEMORY            ","DDERR_OVERLAYCANTCLIP             ","DDERR_OVERLAYCOLORKEYONLYONEACTIVE","DDERR_OVERLAYNOTVISIBLE           ",
			"DDERR_PALETTEBUSY                 ","DDERR_PRIMARYSURFACEALREADYEXISTS ","DDERR_REGIONTOOSMALL              ","DDERR_SURFACEALREADYATTACHED      ",
			"DDERR_SURFACEALREADYDEPENDENT     ","DDERR_SURFACEBUSY                 ","DDERR_SURFACEISOBSCURED           ","DDERR_SURFACELOST                 ",
			"DDERR_SURFACENOTATTACHED          ","DDERR_TOOBIGHEIGHT                ","DDERR_TOOBIGSIZE                  ","DDERR_TOOBIGWIDTH                 ",
			"DDERR_UNSUPPORTED                 ","DDERR_UNSUPPORTEDFORMAT           ","DDERR_UNSUPPORTEDMASK             ","DDERR_UNSUPPORTEDMODE             ",
			"DDERR_VERTICALBLANKINPROGRESS     ","DDERR_WASSTILLDRAWING             ","DDERR_WRONGMODE                   ","DDERR_XALIGN                      "
		};
		for (int i = 0; i < sizeof(ErrorCode)/sizeof(int); i++)
			if (ddrval == ErrorCode[i])
				GAME_ASSERT(0, ErrorMsg[i]);
		GAME_ASSERT(0, "Direct Draw Failed due to unknown error code !!!");
	}
}

CMovingBitmap CIntegerTimer::digit[10];
bool CIntegerTimer::isBmpLoaded = false;
CIntegerTimer::CIntegerTimer(int digits)
	: NUMDIGITS(digits)
{
	//	isBmpLoaded = false;
}

void CIntegerTimer::Add(int x)
{
	n += x;
}

int CIntegerTimer::GetInteger()
{
	return n;
}

void CIntegerTimer::LoadBitmap()
{
	//
	// digit[i]��class varibale�A�ҥH�����קK����LoadBitmap
	//
	if (!isBmpLoaded) {
		for (int i = 0; i < 10; i++)
		{
			char str[80];
			sprintf(str, "Bitmaps/numbers/timer/%d.bmp", i);
			digit[i].LoadBitmap(str, RGB(255, 255, 255));
		}
		isBmpLoaded = true;
	}
}

void CIntegerTimer::SetInteger(int i)
{
	n = i;
}

void CIntegerTimer::SetTopLeft(int nx, int ny)		// �N�ʵe�����W���y�в��� (x,y)
{
	x = nx; y = ny;
}

void CIntegerTimer::ShowBitmap()
{
	GAME_ASSERT(isBmpLoaded, "CInteger: �Х�����LoadBitmap�A�M��~��ShowBitmap");
	int nx;		// ����ܦ�ƪ� x �y��
	int MSB;	// �̥���(�t�Ÿ�)����ƪ��ƭ�
	CString s;
	s.Format("%d", n);
	int num = s.GetLength();
	if (n >= 0) {
		MSB = n;
		nx = x + digit[0].Width()*(num - 1);
	}
	else {
		MSB = -n;
		nx = x + digit[0].Width()*num;
	}
	for (int i = 0; i < num; i++) {
		int d = MSB % 10;
		MSB /= 10;
		digit[d].SetTopLeft(nx, y);
		digit[d].ShowBitmap();
		nx -= digit[d].Width();
	}
	if (n < 0) { // �p�G�p��0�A�h��ܭt��
		digit[10].SetTopLeft(nx, y);
		digit[10].ShowBitmap();
	}
}

CMovingBitmap CIntegerCombo::digit[10];
bool CIntegerCombo::isBmpLoaded = false;
CIntegerCombo::CIntegerCombo(int digits)
	: NUMDIGITS(digits)
{
	//	isBmpLoaded = false;
}

void CIntegerCombo::Add(int x)
{
	n += x;
}

int CIntegerCombo::GetInteger()
{
	return n;
}

void CIntegerCombo::LoadBitmap()
{
	//
	// digit[i]��class varibale�A�ҥH�����קK����LoadBitmap
	//
	if (!isBmpLoaded) {
		for (int i = 0; i < 10; i++)
		{
			char str[80];
			sprintf(str, "Bitmaps/numbers/combo/%d.bmp", i);
			digit[i].LoadBitmap(str, RGB(255, 255, 255));
		}
		isBmpLoaded = true;
	}
}

void CIntegerCombo::SetInteger(int i)
{
	n = i;
}

void CIntegerCombo::SetTopLeft(int nx, int ny)		// �N�ʵe�����W���y�в��� (x,y)
{
	x = nx; y = ny;
}

void CIntegerCombo::ShowBitmap()
{
	GAME_ASSERT(isBmpLoaded, "CInteger: �Х�����LoadBitmap�A�M��~��ShowBitmap");
	int nx;		// ����ܦ�ƪ� x �y��
	int MSB;	// �̥���(�t�Ÿ�)����ƪ��ƭ�
	CString s;
	s.Format("%d", n);
	int num = s.GetLength();
	if (n >= 0) {
		MSB = n;
		nx = x + digit[0].Width()*(num - 1);
	}
	else {
		MSB = -n;
		nx = x + digit[0].Width()*num;
	}
	for (int i = 0; i < num; i++) {
		int d = MSB % 10;
		MSB /= 10;
		digit[d].SetTopLeft(nx, y);
		digit[d].ShowBitmap();
		nx -= digit[d].Width();
	}
	if (n < 0) { // �p�G�p��0�A�h��ܭt��
		digit[10].SetTopLeft(nx, y);
		digit[10].ShowBitmap();
	}
}

//////////////////////////////////////////////////////////////////////////////
// ���Z�O�� 
///////////////////////////////////////////////////////////////////////////////

void ScoreCount::initialize()
{
	totalScore = 0;
	perfectTimes = 0;
	goodTimes = 0;
	missTimes = 0;
	maxCombo = 0;
	tempMaxCombo = 0;
	isMiss = false;
	life = 60;
	isDead = false;
	grade = 0;

	scoreBMP.LoadBitmap();

	for (int i = 0; i < 4; i++)
	{
		judge[i].LoadBitmap();
	}


}
void ScoreCount::clearVectors()
{

}
void ScoreCount::loseLife()
{
	life -= 5;
}
void ScoreCount::gainLife()
{
	life += 2;
}
double ScoreCount::getLife()
{
	return life;
}
bool ScoreCount::setDead()
{
	isDead = true;
	return isDead;
}
ScoreCount ScoreCount::operator =(ScoreCount &records)
{
	totalScore = records.totalScore;
	perfectTimes = records.perfectTimes;
	goodTimes = records.goodTimes;
	missTimes = records.missTimes;
	maxCombo = records.maxCombo;
	tempMaxCombo = records.tempMaxCombo;
	isDead = records.isDead;
	return *this;
}
void ScoreCount::recordMaxCombo(bool isActivate)
{
	if (isMiss == true && !isActivate) {
		if (tempMaxCombo > maxCombo) {
			maxCombo = tempMaxCombo;
		}
		tempMaxCombo = 0;
	}
	else {
		tempMaxCombo += 1;
	}
}
void ScoreCount::IsFullCombo()
{
	maxCombo = tempMaxCombo;
}

double ScoreCount::getScore()
{
	return totalScore;
}
void ScoreCount::showScore()
{

	CDC *pDCp = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
	CFont fp, *fpp;
	fp.CreatePointFont(120, "Times New Roman");	// ���� font f; 160���16 point���r
	fpp = pDCp->SelectObject(&fp);					// ��� font f
													//pDCp->SetBkColor(RGB(0,0,0));
	pDCp->SetBkMode(0);
	pDCp->SetTextColor(RGB(255, 255, 0));
	//char strp[80];								// Demo �Ʀr��r�ꪺ�ഫ
	/*
	sprintf(strp, "Score %.lf", totalScore );
	pDCp->TextOut(50,60,strp);
	sprintf(strp, "Life %.lf", life );
	pDCp->TextOut(50,60,strp);
	sprintf(strp, "judgeBMP.size %d", judgeBMP.size() );
	pDCp->TextOut(50,100,strp);
	sprintf(strp, "combo.size %d", comboNum.size() );
	pDCp->TextOut(50,100,strp);
	sprintf(strp, "score.size %d", scoreNum.size() );
	pDCp->TextOut(50,140,strp);
	sprintf(strp, "Perfect %d", perfectTimes );
	pDCp->TextOut(470,20,strp);
	sprintf(strp, "Good %d", goodTimes );
	pDCp->TextOut(470,40,strp);
	sprintf(strp, "Miss %d", missTimes );
	pDCp->TextOut(470,60,strp);
	sprintf(strp, "Max Combo %d", maxCombo );
	pDCp->TextOut(470,80,strp);

	if( tempMaxCombo > 1 )
	{
	sprintf(strp, "%d", tempMaxCombo );
	pDCp->TextOut(550,160,strp);
	}
	*/
	pDCp->SelectObject(fpp);						// �� font f (�d�U���n�|�F��)
	CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC

}

void ScoreCount::showFinal()
{

	CDC *pDCp = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
	CFont fp, *fpp;
	fp.CreatePointFont(100, "Times New Roman");	// ���� font f; 160���16 point���r
	fpp = pDCp->SelectObject(&fp);					// ��� font f
	pDCp->SetBkColor(RGB(0, 0, 0));
	//pDCp->SetBkMode(0);
	pDCp->SetTextColor(RGB(255, 255, 0));
	char strp[80];								// Demo �Ʀr��r�ꪺ�ഫ

	sprintf(strp, "grade %d", grade);
	pDCp->TextOut(0, 40, strp);
	/*
	sprintf(strp, "Perfect %d", perfectTimes );
	pDCp->TextOut(0,70,strp);
	sprintf(strp, "Good %d", goodTimes );
	pDCp->TextOut(0,100,strp);
	sprintf(strp, "Miss %d", missTimes );
	pDCp->TextOut(0,130,strp);
	sprintf(strp, "Max Combo %d", maxCombo );
	pDCp->TextOut(0,160,strp);
	/*
	if( missTimes == 0 )
	{
	sprintf(strp, "Full Combo !!" );
	pDCp->TextOut(240,190,strp);
	}
	if( isDead == true )
	{
	sprintf(strp, "Stage Failed !!" );
	pDCp->TextOut(240,220,strp);
	}
	*/
	pDCp->SelectObject(fpp);						// �� font f (�d�U���n�|�F��)
	CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC

}

void ScoreCount::setPerfect()
{
	perfectTimes += 1;
}
void ScoreCount::setGood()
{
	goodTimes += 1;
}
void ScoreCount::setMiss()
{
	missTimes += 1;
}
void ScoreCount::setTotalScore(int i)
{
	totalScore += i;
}
void ScoreCount::setIsMiss(bool miss)
{
	isMiss = miss;
}
int ScoreCount::checkGrade()
{
	int totalNote = missTimes + perfectTimes + goodTimes;
	double maxPoint = totalNote * 500;

	if ((perfectTimes * 500 + goodTimes * 300) / maxPoint == 1.0)
	{
		grade = 0;
	}
	else if (((perfectTimes * 500 + goodTimes * 300) / maxPoint) > 0.901)
	{
		grade = 1;
	}
	else if (((perfectTimes * 500 + goodTimes * 300) / maxPoint) > 0.801)
	{
		grade = 2;
	}
	else if (((perfectTimes * 500 + goodTimes * 300) / maxPoint) > 0.701)
	{
		grade = 3;
	}
	else if (((perfectTimes * 500 + goodTimes * 300) / maxPoint) > 0.601)
	{
		grade = 4;
	}
	else if (((perfectTimes * 500 + goodTimes * 300) / maxPoint) >= 0.501)
	{
		grade = 5;
	}
	else
	{
		grade = 6;
	}
	return grade;
}

//////////////////////////////////////////////////////////////////////////////
// ��� 
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
		for (int i = 1; i<17; i++)
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
	dy = (double)(420 / (4 * 60 * 60 / bpms));           //�`���ʮɶ���8��beat���j�A�b8��beat���j���e�N�n���note
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




/////////////////////////////////////////////////////////////////////////////
// �C���
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
	for (int i = 0; i<5; i++)
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
	for (int i = 0; i<5; i++)
	{
		CursorNote[i].OnShow(NoteType[i], i, player);
	}
}
void Row::OnMove(double bpms) {
	for (int i = 0; i<5; i++)
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

/////////////////////////////////////////////////////////////////////////////
// �q����T
/////////////////////////////////////////////////////////////////////////////

double Infomation::FormatToDouble(string tempString) {
	int findFloat = 0;
	double value = 0;
	int isNagative = 0;

	if (tempString[0] == '-') isNagative = 1;
	for (unsigned int j = 0; j < tempString.size(); j++)
		findFloat = tempString.find('.');

	if (findFloat == -1) {
		for (unsigned int j = 0 + isNagative; j < tempString.size(); j++)
		{
			int bbuff = (tempString[j] - 48); // ���o�Ʀr
			value += bbuff * pow(10.0, (double)(tempString.size() - j - 1));
		}
	}
	else {
		for (unsigned int j = 0 + isNagative; j < tempString.size(); j++)
		{
			if (j >= (unsigned int)findFloat)
			{
				if (tempString[j] == '.') {
					value += 0.0;
				}
				else {
					int bbuff = (tempString[j] - 48); // ���o�Ʀr
					value += bbuff * pow(0.1, (double)(j - findFloat));
				}
			}
			else {
				int bbuff = (tempString[j] - 48); // ���o�Ʀr
				value += bbuff * pow(10.0, (double)(findFloat - j - 1));
			}
		}
	}
	if (isNagative == 1) value = -value; // �P�_�O�_���t��
	return value;
}

void Infomation::initialize()
{
	artist = "";
	title = "";
	path = "";
	difficult = "";
	bpms = 0.0;
	offset = 0.0;
	totalTime = 0.0;
	rowCount = 0;
}

void Infomation::readFile(string fileName)
{
	int holdRowIndex[4] = { 0,0,0,0 };
	int NowNote = 0;
	condition = 1; // ���p�N��
	ifstream fin;
	fin.open(fileName); //Ū��
	int rowCounter = 0;
	int dotCounter = 0;
	periodCount = 0;
	bpms = 0;
	offset = 0;
	totalTime = 0;
	rowCount = 0;
	string buffString; // �Ȧs�r��
	int k = 0; //�Ȧscount

	char buff[256];
	int i = 0;

	if (fin.fail())
	{
		//	return false ;
	}
	while (fin >> buff)
	{
		string stringBuff; //�Ȧs�r��2
		buffString = buff;
		if (buffString[0] == '#')
		{
			for (unsigned int j = (buffString.find(':') + 1); j < buffString.size(); j++)
			{
				if (buffString[j] != ';')
					stringBuff += buffString[j];
			}
			switch (condition)
			{
			case 1:
				title = stringBuff;
				break;
			case 2:
				artist = stringBuff;
				break;
			case 3:
				path = stringBuff;
				break;
			case 4:
				bpms = FormatToDouble(stringBuff);
				break;
			case 5:
				offset = FormatToDouble(stringBuff);
				break;
			case 6:
				totalTime = FormatToDouble(stringBuff);
				break;
			case 7:
				difficult = stringBuff;
				break;
			case 8:
				rowCount = (int)FormatToDouble(stringBuff);
				NoteRow = new Row[rowCount];
				break;
			}
			condition += 1;
		}
		else {
			if (buff[0] != ',' && buff[0] != ';')
			{
				NoteRow[rowCounter].initialize((int)FormatToDouble(buffString));
				rowCounter += 1;
				k += 1;
			}
			else if (buff[0] == ',' || buff[0] == ';') {
				for (int i = 0; i < k; i++)
				{
					NoteRow[i + rowCounter - k].SetTime((int)((dotCounter * 4 * 60000 / bpms) + (i * 4 * 60000) / (bpms * k)));
					for (int j = 0; j<4; j++)
					{
						if (NoteRow[i + rowCounter - k].CheckNoteType(j) == 2)
							holdRowIndex[j] = NowNote;
						if (NoteRow[i + rowCounter - k].CheckNoteType(j) == 3)
						{
							NoteRow[holdRowIndex[j]].setY(NoteRow[i + rowCounter - k].getY(j), j);
						}
					}
					NowNote++;
				}
				k = 0;
				dotCounter += 1;
			}

		}
	}
	fin.close();

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

double Infomation::GetTotalTime()
{
	return totalTime;
}
string Infomation::GetPath()
{
	return path;
}
/*
void Infomation::initialize()
{
double i = ( (totalTime) + ((bpms/60)+1) ) ;
NoteRow = new Row [(int)i] ;
}
*/
double Infomation::GetBpms()
{
	return bpms;
}

double Infomation::GetOffset()
{
	return offset;
}
void Infomation::OnShow() {
	CDC *pDCpp = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC 
	CFont fpp, *fppp;
	fpp.CreatePointFont(160, "Times New Roman");	// ���� font f; 160���16 point���r
	fppp = pDCpp->SelectObject(&fpp);					// ��� font f
	pDCpp->SetBkColor(RGB(0, 0, 0));
	pDCpp->SetTextColor(RGB(255, 255, 0));
	char buff[256];
	sprintf(buff, "Artist: %s", artist.c_str());
	pDCpp->TextOut(50, 50, buff);
	sprintf(buff, "Title: %s", title.c_str());
	pDCpp->TextOut(50, 70, buff);
	sprintf(buff, "Path: %s", path.c_str());
	pDCpp->TextOut(50, 90, buff);
	sprintf(buff, "Difficult: %s", difficult.c_str());
	pDCpp->TextOut(50, 110, buff);
	sprintf(buff, "Bpms: %.2f", bpms);
	pDCpp->TextOut(50, 130, buff);
	sprintf(buff, "Offset: %.2f", offset);
	pDCpp->TextOut(50, 150, buff);
	sprintf(buff, "TotalTime: %.2f", totalTime);
	pDCpp->TextOut(50, 170, buff);
	sprintf(buff, "RowCount: %d", rowCount);
	pDCpp->TextOut(50, 190, buff);
	pDCpp->SelectObject(fppp);						// �� font f (�d�U���n�|�F��)
	CDDraw::ReleaseBackCDC();
}
int Infomation::GetTempCursor(int index) {
	return tempCursor[index];
}
int Infomation::GetRowCount() {
	return rowCount;
}
void Infomation::DeleteRow() {
	delete[] NoteRow;
}

Infomation Infomation::operator =(Infomation &music)
{
	artist = music.artist;
	title = music.title;
	path = music.path;
	bpms = music.bpms;
	offset = music.offset;
	totalTime = music.totalTime;
	difficult = music.difficult;
	rowCount = music.rowCount;
	return *this;
}

void Infomation::GetRowCountFromFile(string fileName) {
	ifstream fin;
	fin.open(fileName);
	int rowCount = 0;
	char buffer[256];
	string buffString; // �Ȧs�r��
	while (!fin.eof())
	{
		string stringBuff; //�Ȧs�r��2
		fin >> buffer;
		buffString = buffer;
		if (buffString.find("#ROWCOUNT:") == 0)
		{
			for (unsigned int j = (buffString.find(':') + 1); j < buffString.size(); j++)
			{
				if (buffString[j] != ';')
					stringBuff += buffString[j];
			}
			rowCount = (int)FormatToDouble(stringBuff);
		}

	}
	fin.close();
}
}