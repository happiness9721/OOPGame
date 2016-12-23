#include "StdAfx.h"
#include "Information.h"
#include <fstream>
#include "CDDraw.h"

using namespace std;

namespace game_framework {

	/////////////////////////////////////////////////////////////////////////////
	// 歌曲資訊
	/////////////////////////////////////////////////////////////////////////////

	double Information::FormatToDouble(string tempString) {
		int findFloat = 0;
		double value = 0;
		int isNagative = 0;

		if (tempString[0] == '-') isNagative = 1;
		for (unsigned int j = 0; j < tempString.size(); j++)
			findFloat = tempString.find('.');

		if (findFloat == -1) {
			for (unsigned int j = 0 + isNagative; j < tempString.size(); j++)
			{
				int bbuff = (tempString[j] - 48); // 取得數字
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
						int bbuff = (tempString[j] - 48); // 取得數字
						value += bbuff * pow(0.1, (double)(j - findFloat));
					}
				}
				else {
					int bbuff = (tempString[j] - 48); // 取得數字
					value += bbuff * pow(10.0, (double)(findFloat - j - 1));
				}
			}
		}
		if (isNagative == 1) value = -value; // 判斷是否為負數
		return value;
	}

	void Information::initialize()
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

	void Information::readFile(string fileName)
	{
		int holdRowIndex[4] = { 0,0,0,0 };
		int NowNote = 0;
		condition = 1; // 狀況代號
		ifstream fin;
		fin.open(fileName); //讀檔
		int rowCounter = 0;
		int dotCounter = 0;
		periodCount = 0;
		bpms = 0;
		offset = 0;
		totalTime = 0;
		rowCount = 0;
		string buffString; // 暫存字串
		int k = 0; //暫存count

		char buff[256];
		int i = 0;

		if (fin.fail())
		{
			//	return false ;
		}
		while (fin >> buff)
		{
			string stringBuff; //暫存字串2
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
						for (int j = 0; j < 4; j++)
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

	double Information::GetTotalTime()
	{
		return totalTime;
	}

	string Information::GetPath()
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

	double Information::GetBpms()
	{
		return bpms;
	}

	double Information::GetOffset()
	{
		return offset;
	}

	void Information::OnShow() {
		CDC *pDCpp = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC 
		CFont fpp, *fppp;
		fpp.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
		fppp = pDCpp->SelectObject(&fpp);					// 選用 font f
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
		pDCpp->SelectObject(fppp);						// 放掉 font f (千萬不要漏了放掉)
		CDDraw::ReleaseBackCDC();
	}

	int Information::GetTempCursor(int index) {
		return tempCursor[index];
	}

	int Information::GetRowCount() {
		return rowCount;
	}

	void Information::DeleteRow() {
		delete[] NoteRow;
	}

	Information Information::operator =(Information &music)
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

	void Information::GetRowCountFromFile(string fileName) {
		ifstream fin;
		fin.open(fileName);
		int rowCount = 0;
		char buffer[256];
		string buffString; // 暫存字串
		while (!fin.eof())
		{
			string stringBuff; //暫存字串2
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