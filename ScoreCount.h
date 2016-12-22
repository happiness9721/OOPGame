#pragma once
#include <iostream>
#include <fstream>

namespace game_framework {
	using namespace std;
	/////////////////////////////////////////////////////////////////////////////
	// 記錄成績  
	/////////////////////////////////////////////////////////////////////////////

	class ScoreCount {
	public:
		void initialize();
		void recordMaxCombo(bool);
		void showScore();
		void showFinal();   // 結算成績
		void setPerfect();
		void setGood();
		void setMiss();
		void setTotalScore(int);
		void setIsMiss(bool);
		void IsFullCombo();
		void loseLife();
		void gainLife();
		void clearVectors();
		int  checkGrade();

		bool setDead();
		bool isDead;

		double getLife();
		double getScore();

		int tempMaxCombo;
		int missTimes;
		int perfectTimes; // 500分
		int goodTimes; // 300 分
		int maxCombo;

		ScoreCount operator = (ScoreCount&);
		CInteger scoreBMP;
		CInteger judge[3];
		CIntegerCombo comboBMP;

	protected:
		int grade;
		double totalScore;
		double life;
		bool isMiss;
	};
}