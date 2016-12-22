#include <iostream>
#include <afxwin.h>
#include "ScoreCount.h"

namespace game_framework {

	using namespace std;

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

	ScoreCount ScoreCount::operator=(ScoreCount &)
	{
		return ScoreCount();
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
}