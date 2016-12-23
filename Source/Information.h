#include "StdAfx.h"
#include "Row.h"
using namespace std;

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// �q����T
	/////////////////////////////////////////////////////////////////////////////

	class Information {
	public:
		void readFile(string file); // true �ɮצs�b false �ɮפ��s�b
		void initialize();
		void OnShow();
		void DeleteRow();
		void SetTimeRate();
		void GetRowCountFromFile(string fileName); // ���o�`�C��

		double GetBpms();
		double GetOffset();
		double FormatToDouble(string  tempString); // �N�r���ഫ�� double
		double GetTotalTime();

		int GetRowCount(); // ���o�`�C��
		int GetTempCursor(int); // ���o���� row �����

		string GetPath();
		Row *NoteRow;
		Information operator = (Information&);

	private:
		//    Row *NoteRow ;
		string artist;
		string title;
		string path;
		string difficult;

		double bpms;
		double offset;
		double totalTime;

		int periodCount; // ���Z����
		int condition; // 1.title 2.artist 3.music 4.bpms 5.offset 6. totlaTime 7.difficult 8.rowCount
		int *tempCursor;
		int rowCount; // �`�C��
	};
}
