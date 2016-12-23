#include "StdAfx.h"
#include "Row.h"
using namespace std;

namespace game_framework {
	/////////////////////////////////////////////////////////////////////////////
	// 歌曲資訊
	/////////////////////////////////////////////////////////////////////////////

	class Information {
	public:
		void readFile(string file); // true 檔案存在 false 檔案不存在
		void initialize();
		void OnShow();
		void DeleteRow();
		void SetTimeRate();
		void GetRowCountFromFile(string fileName); // 取得總列數

		double GetBpms();
		double GetOffset();
		double FormatToDouble(string  tempString); // 將字串轉換成 double
		double GetTotalTime();

		int GetRowCount(); // 取得總列數
		int GetTempCursor(int); // 取得產生 row 的整數

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

		int periodCount; // 間距次數
		int condition; // 1.title 2.artist 3.music 4.bpms 5.offset 6. totlaTime 7.difficult 8.rowCount
		int *tempCursor;
		int rowCount; // 總列數
	};
}
