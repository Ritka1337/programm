#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include "json.hpp"
using namespace std;
using json = nlohmann::json;

void error(int a)
{
    cout << "Файл не открылся или не был найден" << endl;
    exit(0);

}

class D
{
public:
    virtual void shifr(string path_text, string path_shifr, string path_key) = 0;
    virtual void deshifr(string path_shifr, string path_text, string path_key) = 0;
    virtual void keygen(string path_alph, string path_key) = 0;
};

//Замена
class replacement : public D
{
public:
    void keygen(string path_alph, string path_key) override
    {
        srand(time(0));
        json alph;
        json key = { {"alg_type", "replace"}, {"key",{}} };
        string str_alph;

        ifstream file_a(path_alph);
        if (!file_a.is_open())
        {
            cout << "Файл не открылся или не найден" << endl;
            exit(0);
        }

        file_a >> alph;
        file_a.close();

        auto mass = alph.find("alp");
        if (mass != alph.end())
        {
            json inf_alph = alph.at("alp");
            json::iterator pointer = inf_alph.begin();
            vector <int> proverka(inf_alph.size(), -1);
            bool may;

            for (int i = 0; i < inf_alph.size(); i++)
            {
                string bykva = *pointer;
                str_alph.push_back(bykva[0]);
                pointer++;
            }

            string buf;
            for (int i = 0; i < str_alph.size(); i++)
            {
                int x = rand() % str_alph.size();
                may = true;
                for (int j = 0; j < proverka.size(); j++)
                {
                    if (proverka[j] == -1)
                    {
                        may = true;
                        break;
                    }
                    if (proverka[j] == x)
                    {
                        i--;
                        may = false;
                        break;
                    }
                }
                if (may == true)
                {
                    proverka[i] = x;
                    buf.push_back(str_alph[x]);
                    key.at("key").push_back({ str_alph.substr(i, 1), buf });
                    buf.clear();
                }
            }
            ofstream file_k(path_key);
            file_k << key;
            file_k.close();
        }
    }
    void shifr(string path_text, string path_shifr, string path_key) override
    {
        ifstream file_k(path_key);
        if (!file_k.is_open())
        {
            error(0);
        }
        json key;
        file_k >> key;
        file_k.close();
        if (key.at("alg_type") != "replace")
        {
            cout << "Ключ не подходит" << endl;
            exit(0);
        }

        fstream file_text(path_text);
        if (!file_text.is_open())
        {
            error(0);
        }

        ofstream file_shifr(path_shifr);

        string f_str;
        while (!file_text.eof())
        {
            string str;
            getline(file_text, str);
            f_str += str;
        }

        string str1;
        string str2;
        string str3;

        for (int i = 0; i < f_str.size(); i++)
        {
            for (int j = 0; j < key.at("key").size(); j++)
            {
                str1 = key.at("key").at(j).at(0);
                if (f_str[i] == str1[0])
                {
                    str2 = key.at("key").at(j).at(1);
                    str3.push_back(str2[0]);
                }
            }
        }

        file_shifr << str3;
        str1.clear();
        str2.clear();
        str3.clear();

        file_text.close();
        file_shifr.close();

    }
    void deshifr(string path_shifr, string path_text, string path_key) override
    {
        ifstream file_k(path_key);
        if (!file_k.is_open())
        {
            error(0);
        }
        json key;
        file_k >> key;
        file_k.close();
        if (key.at("alg_type") != "replace")
        {
            cout << "Ключ не подходит" << endl;
            exit(0);
        }

        fstream file_shifr(path_shifr);
        if (!file_shifr.is_open())
        {
            error(0);
        }

        ofstream file_text(path_text);

        string f_str;
        while (!file_shifr.eof())
        {
            string str;
            getline(file_shifr, str);
            f_str += str;
        }

        string str1;
        string str2;
        string str3;

        for (int i = 0; i < f_str.size(); i++)
        {
            for (int j = 0; j < key.at("key").size(); j++)
            {
                str1 = key.at("key").at(j).at(1);
                if (f_str[i] == str1[0])
                {
                    str2 = key.at("key").at(j).at(0);
                    str3.push_back(str2[0]);
                }
            }
        }

        file_text << str3;
        str1.clear();
        str2.clear();
        str3.clear();

        file_text.close();
        file_shifr.close();
    }
};

//Перестановка
class transposition :public D
{
public:
    void keygen(string empty, string path_key) override
    {
        srand(time(0));
        json key = { {"alg_type", "rearrangement"}, {"key",{{"block_value",{}},{"block_size",{}}}} };

        int block_size;
        string bsize;
        cout << "Введите размер блока (число, не больше пятизначного): ";
        cin.ignore(255, '\n');
        getline(cin, bsize);
        if (cin.fail() || bsize.size() > 5)
        {
            cout << "Невозможно!" << endl;
            exit(0);
        }
        for (int i = 0; i < bsize.size(); i++)
        {
            if (bsize[i] < '0' || bsize[i]>'9' || bsize[i] == ' ')
            {
                cout << "Невозможно!" << endl;
                exit(0);
            }
        }
        block_size = stoi(bsize);
        key.at("key").at("block_size") = block_size;

        vector <int> proverka;
        int elem;
        bool may = true;

        for (int i = 0; i < block_size; i++)
        {
            elem = rand() % block_size;
            if (proverka.size() == 0)
            {
                key.at("key").at("block_value").push_back(elem);
                proverka.push_back(elem);
                continue;
            }
            for (int j = 0; j < proverka.size(); j++)
            {
                if (proverka[j] == elem)
                {
                    i--;
                    break;
                }
                if (j == proverka.size() - 1 && proverka[j] != elem)
                {
                    may = true;
                    proverka.push_back(elem);
                    key.at("key").at("block_value").push_back(elem);
                    break;
                }
            }
        }
        proverka.clear();
        ofstream file_key(path_key);
        file_key << key;
        file_key.close();
    }
    void shifr(string path_text, string path_shifr, string path_key) override
    {
        srand(time(0));
        ifstream file_key(path_key);
        if (!file_key.is_open())
        {
            error(0);
        }
        json key;
        file_key >> key;
        file_key.close();
        if (key.at("alg_type") != "rearrangement")
        {
            cout << "Ключ не подходит" << endl;
            exit(0);
        }

        fstream file_text(path_text);
        if (!file_text.is_open())
        {
            error(0);
        }

        string f_str;
        while (!file_text.eof())
        {
            string str;
            getline(file_text, str);
            f_str += str;
        }

        string result = f_str;
        
        vector <int> value = key.at("key").at("block_value");
        int elem;

        for (int i = 0; i < result.size(); i++)
        {
            for (int j = 0; j < value.size(); j++)
            {
                if (f_str.size() < value.size())
                {
                    while (f_str.size() != value.size())
                    {
                        elem = rand() % f_str.size();
                        f_str.push_back(f_str[elem]);
                        result.push_back(f_str[elem]);
                    }
                    j--;
                }
                else
                {
                    result[i] = f_str[value[j]];
                    i++;
                }
            }
            for (int k = 0; k < value.size(); k++)
            {
                if (f_str.size() != 0)
                    f_str.erase(f_str.begin());
            }
            i--;
        }

        ofstream file_shifr(path_shifr);
        file_shifr << result;
        file_text.close();
        file_shifr.close();
    }
    void deshifr(string path_shifr, string path_text, string path_key) override
    {
        ifstream file_key(path_key);
        if (!file_key.is_open())
        {
            error(0);
        }
        json key;
        file_key >> key;
        file_key.close();
        if (key.at("alg_type") != "rearrangement")
        {
            cout << "Ключ не подходит" << endl;
            exit(0);
        }

        fstream file_shifr(path_shifr);
        if (!file_shifr.is_open())
        {
            error(0);
        }

        string f_str;
        while (!file_shifr.eof())
        {
            string str;
            getline(file_shifr, str);
            f_str += str;
        }

        vector<int> value = key.at("key").at("block_value");
        string result;
        string buf(value.size(), '0');

        for (int i = 0; i < f_str.size(); i++)
        {
            for (int j = 0; j < value.size(); j++)
            {
                buf[value[j]] = f_str[i];
                i++;
            }
            result += buf;
            for (int k = 0; k < value.size(); k++)
            {
                if (f_str.size() != 0)
                    f_str.erase(f_str.begin());
                if (f_str.size() == 0)
                {
                    i = result.size() + 1;
                    break;
                }
            }
            i = -1;
        }

        ofstream file_text(path_text);
        file_text << result;
        file_text.close();
        file_shifr.close();
    }
};

//Гаммирование
class gamming :public D
{
public:
    void keygen(string path_alph, string path_key) override
    {
        srand(time(0));
        json alph;
        json key = { {"alg_type", "gamming"}, {"key",{{"alph",{}} , {"block_size",{}} ,{"block_value",{}}}} };

        ifstream file_alph(path_alph);
        if (!file_alph.is_open())
        {
            cout << "Файл не открылся или не найден" << endl;
            exit(0);
        }
        file_alph >> alph;
        file_alph.close();

        int block_size;
        string bsize;
        cout << "Введите размер блока(число, не больше пятизначного): ";
        cin.ignore(255, '\n');
        getline(cin, bsize);
        if (cin.fail() || bsize.size() > 5)
        {
            cout << "Невозможно" << endl;
            exit(0);
        }
        for (int i = 0; i < bsize.size(); i++)
        {
            if (bsize[i] < '0' || bsize[i]>'9' || bsize[i] == ' ')
            {
                cout << "Невозможно" << endl;
                exit(0);
            }
        }
        block_size = stoi(bsize);
        key.at("key").at("block_size") = block_size;

        auto mass = alph.find("alp");
        if (mass != alph.end())
        {
            json inf_alph = alph.at("alp");
            json::iterator pointer = inf_alph.begin();
            string buf;

            for (int i = 0; i < inf_alph.size(); i++)
            {
                buf = *pointer;
                key.at("key").at("alph").push_back(json::array({ buf, i }));
                pointer++;
            }
            buf.clear();
            for (int i = 0; i < block_size; i++)
            {
                int x = rand() % 25;
                key.at("key").at("block_value").push_back(x);
            }
            ofstream file_key(path_key);
            file_key << key;
            file_key.close();
        }
    }
    void shifr(string path_text, string path_shifr, string path_key) override
    {
        ifstream fkey(path_key);
        if (!fkey.is_open())
            error(0);
        json key;
        fkey >> key;
        fkey.close();
        if (key.at("alg_type") != "gamming")
        {
            cout << "Ключ не подходит" << endl;
            exit(0);
        }

        fstream file_text(path_text);
        if (!file_text.is_open())
        {
            error(0);
        }

        string f_str;
        while (!file_text.eof())
        {
            string str;
            getline(file_text, str);
            f_str += str;
        }

        int size = key.at("key").at("block_size");
        int alph_size = key.at("key").at("alph").size();
        vector<int> value = key.at("key").at("block_value");

        string elem;
        string result;
        int m_value = 0;
        for (int i = 0; i < f_str.size(); i++)
        {
            for (int j = 0; j < alph_size; j++)
            {
                string buf = key.at("key").at("alph").at(j).at(0);
                if (f_str[i] == buf[0] && f_str.size() != 0)
                {
                    if (m_value == value.size())
                    {
                        m_value = 0;
                    }

                    int m_letter;
                    int a = key.at("key").at("alph").at(j).at(1);
                    int b = value[m_value];

                    m_letter = (a + b) % alph_size;
                    elem = key.at("key").at("alph").at(m_letter).at(0);
                    result += elem;
                    elem.clear();
                    m_value++;
                    break;
                }
                else if (f_str.size() == 0)
                {
                    i = f_str.size();
                    break;
                }
            }
        }

        ofstream file_shifr(path_shifr);
        file_shifr << result;
        file_text.close();
        file_shifr.close();
    }
    void deshifr(string path_shifr, string path_text, string path_key) override
    {
        ifstream file_key(path_key);
        if (!file_key.is_open())
        {
            error(0);
        }
        json key;
        file_key >> key;
        file_key.close();
        if (key.at("alg_type") != "gamming")
        {
            cout << "Ключ не подходит" << endl;
            exit(0);
        }

        fstream file_shifr(path_shifr);
        if (!file_shifr.is_open())
            error(0);

        string f_str;
        while (!file_shifr.eof())
        {
            string str;
            getline(file_shifr, str);
            f_str += str;
        }

        int size = key.at("key").at("block_size");
        int alph_size = key.at("key").at("alph").size();
        vector <int> value = key.at("key").at("block_value");

        string elem;
        string buf;
        string result;
        int m_value = 0;
        int m_letter;
        for (int i = 0; i < f_str.size(); i++)
        {
            for (int j = 0; j < alph_size; j++)
            {
                buf = key.at("key").at("alph").at(j).at(0);
                if (f_str[i] == buf[0] && f_str.size() != 0)
                {
                    if (m_value == value.size())
                    {
                        m_value = 0;
                    }

                    int x = value[m_value];
                    m_letter = j - x;
                    if (m_letter < 0)
                    {
                        m_letter += alph_size;
                    }

                    elem = key.at("key").at("alph").at(m_letter).at(0);
                    result += elem;
                    elem.clear();
                    m_value++;
                    break;
                }
                else if (f_str.size() == 0)
                {
                    i = f_str.size();
                    break;
                }
            }
        }
        ofstream file_text(path_text);
        file_text << result;
        file_shifr.close();
        file_text.close();
    }
};

int main()
{
    setlocale(LC_ALL, "rus");
    int a;
    bool err = false;
    while (1)
    {
        if (err == true)
        {
            cout << endl << "Неизвестная команда!" << endl << endl;
            err = false;
            cin.clear();
            system("pause");
            return 0;
        }


        cout << "Главное меню:" << endl << endl << "1) Зашифровать " << endl << "2) Дешифровать" << endl << "3) Сгенерировать ключ" << endl << "4) Выход" << endl << endl;
        cout << "Ваш выбор: ";
        cin >> a;

        system("cls");
        if (a == 1) //ШИФРАЦИЯ
        {
            a = 0;
            cout << "Метод:" << endl << endl << "1) Замена " << endl << "2) Перестановка " << endl << "3) Гаммирование " << endl << endl;

            cin >> a;
            system("cls");

            if (a == 1) //Замена
            {
                a = 0;
                cout << "Замена" << endl << endl;
                string path_text;
                string pt;
                string path_shifr;
                string ps;
                string path_key;
                string pk;
                string namef;


                cout << "Введите путь к папке, содержащей файл с текстом: ";
                cin >> pt;
                cout << "Введите имя файла, содержащего текст: ";
                cin >> namef;
                path_text = pt + "\\" + namef + ".txt";

                cout << "Введите путь к папке, в которую запишется шифротекст:  ";
                cin >> ps;
                cout << "Введите имя файла, содержащего шифротекст: ";
                cin >> namef;

                path_shifr = ps + "\\" + namef + ".encrypt";


                cout << "Ведите путь к папке, содержащей ключ: ";
                cin >> pk;
                cout << "Введите имя файла с ключом: ";
                cin >> namef;
                path_key = pk + "\\" + namef + ".key";



                replacement s;
                s.shifr(path_text, path_shifr, path_key);
                cout << endl << "Завершено" << endl;
                system("pause");
                continue;

            }
            else if (a == 2) //Перестановка
            {
                a = 0;
                cout << "Перестановка" << endl << endl;
                string path_text;
                string pt;
                string path_shifr;
                string ps;
                string path_key;
                string pk;
                string namef;


                cout << "Введите путь к папке, содержащей файл с текстом: ";
                cin >> pt;
                cout << "Введите имя файла, содержащего текст: ";
                cin >> namef;
                path_text = pt + "\\" + namef + ".txt";

                cout << "Введите путь к папке, в которую запишется шифротекст:  ";
                cin >> ps;
                cout << "Введите имя файла, содержащего шифротекст: ";
                cin >> namef;

                path_shifr = ps + "\\" + namef + ".encrypt";

                cout << "Введите путь к папке, содержащей ключ: ";
                cin >> pk;
                cout << "Введите имя файла с ключом: ";
                cin >> namef;
                path_key = pk + "\\" + namef +".key";




                transposition s;
                s.shifr(path_text, path_shifr, path_key);
                cout << endl << "Завершено" << endl;
                system("pause");
                continue;
            }
            else if (a == 3) //Гаммирование
            {
                a = 0;
                cout << "Гаммирование" << endl << endl;
                string path_text;
                string pt;
                string path_shifr;
                string ps;
                string path_key;
                string pk;
                string namef;


                cout << "Введите путь к папке, содержащей файл с текстом: ";
                cin >> pt;
                cout << "Введите имя файла, содержащего текст: ";
                cin >> namef;
                path_text = pt + "\\" + namef + ".txt";

                cout << "Введите путь к папке, в которую запишется шифротекст:  ";
                cin >> ps;
                cout << "Введите имя файла, содержащего шифротекст: ";
                cin >> namef;

                path_shifr = ps + "\\" + namef + ".encrypt";

                cout << "Введите путь к папке, содержащей ключ: ";
                cin >> pk;
                cout << "Введите имя файла с ключом: ";
                cin >> namef;
                path_key = pk + "\\" + namef + ".key";



                gamming s;
                s.shifr(path_text, path_shifr, path_key);
                cout << endl << "Завершено" << endl;
                system("pause");
                continue;
            }
            else
            {
                err = true;
                system("pause");
                continue;

            }

        }
        else if (a == 2) //ДЕШИФРАЦИЯ
        {
            a = 0;
            cout << "Метод:" << endl << endl << "1) Замена " << endl << "2) Перестановка " << endl << "3) Гаммирование " << endl << endl;

            cin >> a;
            system("cls");

            if (a == 1) //Замена
            {
                a = 0;
                cout << "Замена" << endl << endl;
                string path_shifr;
                string ps;
                string path_text;
                string pt;
                string path_key;
                string pk;
                string namef;


                cout << "Введите путь к папке, содержащей файл с шифром: ";
                cin >> ps;
                cout << "Введите имя файла с шифром: ";
                cin >> namef;
                path_shifr = ps + "\\" + namef + ".encrypt";

                cout << "Введите путь к папке, в которую запишется текст:  ";
                cin >> pt;
                cout << "Введите имя файла, куда запишется текст: ";
                cin >> namef;

                path_text = pt + "\\" + namef + ".text";

                cout << "Введите путь к папке, содержащей ключ: ";
                cin >> pk;
                cout << "Введите имя ключа: ";
                cin >> namef;
                path_key = pk + "\\" + namef + ".key";


                replacement res;
                res.deshifr(path_shifr, path_text, path_key);
                cout << endl << "Завершено" << endl;
                system("pause");
                continue;

            }
            else if (a == 2) //Перестановка
            {
                a = 0;
                cout << "Перестановка" << endl << endl;
                string path_text;
                string pt;
                string path_shifr;
                string ps;
                string path_key;
                string pk;
                string namef;


                cout << "Введите путь к папке, содержащей файл с шифром: ";
                cin >> ps;
                cout << "Введите имя файла с шифром: ";
                cin >> namef;
                path_shifr = ps + "\\" + namef + ".encrypt";

                cout << "Введите путь к папке, в которую запишется текст:  ";
                cin >> pt;
                cout << "Введите имя файла с текстом: ";
                cin >> namef;
                path_text = pt + "\\" + namef + ".text";


                cout << "Введите путь к папке, содержащей ключ: ";
                cin >> pk;
                cout << "Введите имя файла с ключом: ";
                cin >> namef;
                path_key = pk + "\\" + namef + ".key";



                transposition s;
                s.deshifr(path_shifr, path_text, path_key);
                cout << endl << "Завершено" << endl;
                system("pause");
                continue;
            }
            else if (a == 3) //Гаммирование
            {
                a = 0;
                cout << "Гаммирование" << endl << endl;
                string path_text;
                string pt;
                string path_shifr;
                string ps;
                string path_key;
                string pk;
                string namef;


                cout << "Введите путь к папке, содержащей файл с шифром: ";
                cin >> ps;
                cout << "Введите имя файла с шифром: ";
                cin >> namef;
                path_shifr = ps + "\\" + namef + ".encrypt";

                cout << "Введите путь к папке, в которую запишется текст:  ";
                cin >> pt;
                cout << "Введите имя файла с текстом: ";
                cin >> namef;
                path_text = pt + "\\" + namef + ".text";


                cout << "Введите путь к папке, содержащей ключ: ";
                cin >> pk;
                cout << "Введите имя файла с ключом: ";
                cin >> namef;
                path_key = pk + "\\" + namef + ".key";



                gamming s;
                s.deshifr(path_shifr, path_text, path_key);
                cout << endl << "Завершено" << endl;
                system("pause");
                continue;
            }
            else
            {
                err = true;
                system("pause");
                continue;
            }

        }
        else if (a == 3) //ГЕН КЛЮЧА
        {
            a = 0;
            cout << "Сгенерировать ключ " << endl << endl;
            cout << "1) Замена " << endl << "2) Перестановка" << endl << "3) Гаммирование" << endl << endl;

            cin >> a;
            system("cls");

            if (a == 1) //Замена
            {
                a = 0;
                cout << "Сгенерировать ключ для Замены" << endl << endl;
                string pa;
                string path_alph;
                string pk;
                string path_key;
                string namef;

                cout << "Введите путь к папке, содержащей алфавит: ";
                cin >> pa;
                cout << "Введите название файла с алфавитом: ";
                cin >> namef;

                path_alph = pa + "\\" + namef + ".alph";

                cout << "Введите путь к папке, содержащей ключ: ";
                cin >> pk;
                cout << "Введите имя файла для ключа: ";
                cin >> namef;


                path_key = pk + "\\" + namef + ".key";



                replacement r;
                r.keygen(path_alph, path_key);
                cout << endl << "Завершено" << endl;
                system("pause");
                continue;
            }
            else if (a == 2) //Перестановка
            {
                a = 0;
                cout << "Сгенерировать ключ для Перестановки" << endl << endl;
                string path_key;
                string pk;
                string empty;
                string namef;


                cout << "Введите путь к папке, содержащей ключ: ";
                cin >> pk;
                cout << "Введите имя файла для ключа: ";
                cin >> namef;


                path_key = pk + "\\" + namef + ".key";



                transposition s;
                s.keygen(empty, path_key);
                cout << endl << "Завершено" << endl;
                system("pause");
                continue;

            }
            else if (a == 3) //Гаммирование
            {
                a = 0;
                cout << "Сгенерировать ключ для Гаммирования" << endl << endl;
                string path_alph;
                string pa;
                string path_key;
                string pk;
                string namef;


                cout << "Введите путь к папке, содержащей алфавит: ";
                cin >> pa;
                cout << "Введите имя с алфавитом: ";
                cin >> namef;

                path_alph = pa + "\\" + namef + ".alph";

                cout << " Введите путь к папке, содержащей ключ : ";
                cin >> pk;
                cout << "Введите имя файла для ключа: ";
                cin >> namef;


                path_key = pk + "\\" + namef + ".key";



                gamming s;
                s.keygen(path_alph, path_key);
                cout << endl << "Завершено" << endl;
                system("pause");
                continue;
            }
            else
            {
                err = true;
                continue;
            }

        }
        else if (a == 4)
        {
            a = 0;
            system("pause");
            return 0;
        }
        else
        {
            err = true;
            system("pause");
            continue;
        }

    }
}