﻿#include<iostream>
#include<string>
#include<fstream>
#include <vector>
#include<filesystem>
using namespace std;
namespace fs = std::filesystem;

int main()
{
	setlocale(LC_ALL, "ru");
	string device;
	cout << "Название устройства ";
	cin >> device;

	if (cin.fail())
	{
		std::cout << "Ошибка";
		return 0;
	}
	cout << endl << "\tДата\t\t-Показания" << endl << endl;
	vector <string> logs;
	for (auto& p : filesystem::directory_iterator("C:/Users/79089/Desktop/logs"))
	{
		logs.push_back(p.path().string());
	}
	for (int i = 0; i < logs.size(); i++)
	{
		auto path = logs[i];
		auto file = fstream(path);
		string line;

		while (!file.eof())
		{
			auto res = line.find(':');
			string name_result = line.substr(res + 1, line.size() - res - 1);
			res = name_result.find(':');
			string name = name_result.substr(0, res);

			if (device == name && line.find("garbage") == -1)
			{
				auto res = line.find(':');
				string sec = line.substr(0, res);

				res = name_result.find(':');
				string result = name_result.substr(res + 1, line.size() - res - 1);

				auto d = path.find('(');
				auto b = path.find(')');
				string date = path.substr(d + 1, b - d - 1);
				cout << date << "-0";
				cout << sec << "\t" << "-" << result << endl;
			}
			line = "";
			getline(file, line);
		}
		file.close();
	}
}
