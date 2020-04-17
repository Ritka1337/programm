#include<iostream>
#include<string>
#include<fstream>
#include <vector>//подключаем директиву вектор
#include<filesystem>
#include <iterator>
namespace fs = std::filesystem;
using namespace std;

int main()
{
	setlocale(LC_ALL, "ru");
	
	cout << "Название устройства ";
	string device;
	cin >> device;
	if (cin.fail())
	{
		cout << "Ошибка";
		return 0;
	}

	vector <string> logs;

	for (auto& p : fs::directory_iterator("C:/Users/79089/Desktop/logs"))
	{

		logs.push_back(p.path().string());
	}

	cout << endl << "\tДата\t\t-Показания" << endl<< endl;

	for (int i = 0; i < logs.size(); i++) //метод size возвращает число символов в строке(другими словами её длину)
	{
		auto path = logs[i];

		auto file = fstream(path);
		string line;
		getline(file, line);
		while (!file.eof())
		{

			auto res = line.find(':');			
			string name_result = line.substr(res + 1, line.size() - res - 1);
			res = name_result.find(':');
			string name = name_result.substr(0, res);
			line = "";   //можно присваивать указателю любые значения. Это всего лишь строки, на которые он будет указывать нельзя изменять, используя этот указатель.
			getline(file, line);
			if (device == name)
			{

				auto res = line.find(':');
				string sec = line.substr(0, res);
				string name_result = line.substr(res + 1, line.size() - res - 1);
				res = name_result.find(':');
				string result = name_result.substr(res + 1, line.size() - res - 1);

				auto d = path.find('(');
				auto b = path.find(')');
				string date = path.substr(d + 1, b - d - 1);
				cout << date << "-0";
				cout << sec << "\t" << "-" << result << endl;
			}
			
		}
		cout << endl;
		file.close();
	}

}





