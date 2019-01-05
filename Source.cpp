#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <memory>
#include <Windows.h>
#include <locale>
#include <cctype>
#include <algorithm>

using namespace std;

void Mem();

int main(int argc, char ** argv[])
{
	{
		setlocale(LC_ALL, "Russian");
		const char* SEPARATORS = { "',.\/ !:;*&^%$#@()<> - + = _" };

		char * cFileName = nullptr;
		int fileNameLen = 0, fileLen = 0, buffLen = 0;

		if (argc < 2)
		{
			cout << "Pls, enter file name in commond line and restart program" << endl;
			system("pause");

			return 0;
		}
		else
		{
			fileNameLen = strlen((char *)argv[1]);
			cFileName = new char[fileNameLen + 1]{};
			memcpy(cFileName, argv[1], fileNameLen);
		}

		FILE *f;
		errno_t err = fopen_s(&f, cFileName, "rb");

		if (f == NULL)
		{
			cout << "Error open file. Pls check name and restart program" << endl;
			
			system("pause");
			return 0;
		}

		fseek(f, 0, SEEK_END);

		fileLen = ftell(f);
		buffLen = fileLen;
		char  *cBuffText = nullptr;
		int totalLen = 0; //readed and added data
		bool isBuffSizeOk = false; 
		while (isBuffSizeOk != true) //looking for optimal size for our buffer 
		{
			try
			{
				isBuffSizeOk = true;
				cBuffText = new char[buffLen];
			}
			catch (std::bad_alloc&)
			{
				if (cBuffText != nullptr)
				{
					delete[] cBuffText;
					cBuffText = nullptr;
				}
				isBuffSizeOk = false;
				buffLen /= 2;
			}
		}
	
		map<string, int> mapDictionary;
		char *pTocken, *pBuffForTocken, *pContext;
		string strTemp;
		
		fseek(f, 0, SEEK_SET);
		int nReadedBytes = fread(cBuffText, sizeof(char), buffLen, f);
		pTocken = strtok_s(cBuffText, SEPARATORS, &pContext);

		cout << "Start to read file. Wait pls..." << endl;
		while (totalLen < fileLen) 
		{
		    //loop for looking word, convert him to lower-registr and added to map, if it isn't already there 
			while (pTocken != NULL)
			{
				strTemp = pTocken;
				
				//cheking - is first symbol in current tocken - one of separators
					//cout << "Looking for spaces in firts symbol" << endl;
					//if (strTemp[0] == ' ')//SEPARATORS[i])
					//{
					//	string strForExchange = strTemp;
					//	for (int i = 0; i < strTemp.length()-1; ++i)
					//	{
					//		strForExchange[i] = strTemp[i + 1];
					//	}
					//	strTemp = strForExchange;
					//}
				
				auto it = mapDictionary.find(pTocken);
				if (it != mapDictionary.end())
				{
					(*it).second++;
				}
				else
				{
					transform(strTemp.begin(), strTemp.end(), strTemp.begin(), tolower);			
					mapDictionary.insert(pair<string, int>(strTemp, 1));
				}

				pTocken = strtok_s(NULL, SEPARATORS, &pContext);
			}

			totalLen += nReadedBytes;
			if (totalLen += buffLen > fileLen)
				buffLen = fileLen - totalLen;
			fseek(f, totalLen, SEEK_CUR);
			nReadedBytes = fread(cBuffText, sizeof(char), buffLen, f);
		}

		//Open file to write
		ofstream out;
		out.open("out.txt");

		out << "Words count:" << endl;
		map<string, int>::iterator it;

		//Write to file
		for (it = mapDictionary.begin(); it != mapDictionary.end();it++)
		{
			out << (*it).first << " = " << (*it).second << endl;
		}

		if (cFileName != 0)
		{
			delete[] cFileName;
			cFileName = nullptr;
		}
		if (cBuffText != nullptr)
		{
			delete[] cBuffText;
			cBuffText = nullptr;
		}
	}

	Mem(); //chering mem leaks
	system("pause");
	return 0;
}

void Mem()
{
	if (_CrtDumpMemoryLeaks())
		cout << "Mem leaks!" << endl;
	else
		cout << "Mem ok :-)" << endl;
}