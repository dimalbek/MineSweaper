#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;
using namespace chrono;


void Clear()
{
    system("CLS");
}

void Input(int in[], int sz)
{
    bool input = true;
    while (input)
    {
        cout << "Выберите ряд поля которое хотите открыть (1-" << sz << "):\n";
        cin >> in[0];
        if (in[0] > 0 && in[1] <= sz)
            input = false;
        else
            cout << "НЕПРАВИЛЬНЫЙ ВВОД. Введите число от 1 до " << sz << '\n';
    }
    input = true;
    while (input)
    {
        cout << "Выберите столбец поля которое хотите открыть (1-" << sz << "):\n";
        cin >> in[1];
        if (in[1] > 0 && in[1] <= sz)
            input = false;
        else
            cout << "НЕПРАВИЛЬНЫЙ ВВОД. Введите число от 1 до " << sz << '\n';
    }
    in[0]--;
    in[1]--;
    return;
}

void Generate(int sz, int in[], vector<vector<int>>& p)
{
    bool b;
    int count = 0;
    srand(time(nullptr));
    while (b)
    {
        int rRaw = rand() % sz;
        int rCol = rand() % sz;
        if (rRaw == in[0] && rCol == in[1] || p[rRaw][rCol] == -1)
            continue;
        p[rRaw][rCol] = -1;
        count++;
        if (count == 8 && sz == 8)
            b = false;
        if (count == 30 && sz == 16)
            b = false;
    }

    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    
    for (int i = 0; i < sz; i++) 
    {
        for (int j = 0; j < sz; j++) 
        {
            if (p[i][j] == -1)
                continue;
            
            int cMines = 0;
            for (int k = 0; k < 8; k++) 
            {
                int newRaw = i + dx[k];
                int newCol = j + dy[k];
                if (newRaw >= 0 && newRaw < sz && newCol >= 0 && newCol < sz) 
                {
                    if (p[newRaw][newCol] == -1) 
                        cMines++;
                }
            }
            
            p[i][j] = cMines; 
        }
    }
    return;
}

void Display(const vector<vector<int>>& p, const vector<vector<bool>>& opened, int sz, duration<int> Timer)
{
    for (int i = 0; i < sz; i++)
    {
        for (int j = 0; j < sz; j++)
        {
            if (opened[i][j])
            {
                if (p[i][j] == -1)
                    cout << "M ";
                else
                    cout << p[i][j] << " ";
            }
            else
            {
                cout << ". ";
            }
        }
        cout << "\n";
    }
    if (sz == 8)
        cout << "Количество мин: 8\n";
    else 
        cout << "Количество мин: 30\n";

    int rTime = 8 * 60 - Timer.count();
    int min = rTime / 60;
    int sec = rTime % 60;
    cout <<  "Осталось времени: " << min << " минут " << sec << "секунд\n\n";
}

void Expand(int raw, int col, int sz, vector<vector<int>>& p, vector<vector<bool>>& opened)
{
    if (p[raw][col] == 0 && !opened[raw][col])
    {
        opened[raw][col] = true;
        int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};
        for (int i = 0; i < 8; i++)
        {
            int newRaw = raw + dx[i];
            int newCol = col + dy[i];
            if (newRaw >= 0 && newRaw < sz && newCol >= 0 && newCol < sz)
            {   
                Expand(newRaw, newCol, sz, p, opened);
            }
        }
    }
}



bool CheckWin(const vector<vector<int>>& p, const vector<vector<bool>>& opened, int sz)
{
    for (size_t i = 0; i < sz; i++)
    {
        for (size_t j = 0; j < sz; j++)
        {
            if (p[i][j] != -1 && !opened[i][j])
            {
                return false; 
            }
        }
    }
    return true; 
}


int main()
{
    bool replay = true;
    while(replay)
    {
        int dif;
        bool incor_dif = true;
        while(incor_dif)
        {
            cout << "Выберите сложность (Введите цифру 1 или 2):\n1.Легкий\n2.Средний\n";
            cin >> dif;
            if (dif == 1 || dif == 2)
                incor_dif = false;
            else
                cout << "НЕПРАВИЛЬНЫЙ ВВОД. Введите цифру 1 или 2\n";
        }
        int sz;
        if (dif == 1)
            sz = 8;
        else 
            sz = 16;

        int in[2] = {0, 0};
        vector<vector<bool>> opened(sz, vector<bool>(sz, false));
        Input (in, sz);
        vector<vector<int>> pole(sz, vector<int>(sz, 0));
        Generate(sz, in, pole);
        // for(int ii = 0; ii < sz; ii++)
        // {
        //     for (int jj = 0; jj < sz; jj++)
        //     {
        //         cout << pole[ii][jj] << ' ';
        //     }
        //     cout << '\n';
        // }
        auto startTime = system_clock::now();
        bool isWin = false;
        while(!isWin)
        {
            Expand(in[0], in[1], sz, pole, opened);
            Clear();
            opened[in[0]][in[1]] = true;
            auto currentTime = system_clock::now();
            auto rTime = duration_cast<seconds>(currentTime - startTime);
            Display(pole, opened, sz, rTime);
            Input (in, sz);
            if (pole[in[0]][in[1]] == -1)
            {
                cout << "ВЫ ПОПАЛИ ПОД МИНУ!! ВЫ ПРОИГРАЛИ!!\n";
                break;
            }
            isWin = CheckWin(pole, opened, sz);
            if (isWin)
            {
                cout << "ВЫ ВЫИГРАЛИ!!!\n";
            }
            if (rTime.count() >= 8 * 60)
            {
                cout << "ВРЕМЯ ВЫШЛО!! ВЫ ПРОИГРАЛИ\n";
            }
        }
        int rep;
        bool irep = true;
        while(irep)
        {
            cout << "\nХотите сыграть снова? Введите 1/2\n1.ДА\n2.НЕТ\n";
            cin >> rep;
            if (rep == 1 || rep == 2)
            {
                irep = false;
                if(rep == 2)
                    replay = false;
            }
            else
                cout << "НЕПРАВИЛЬНЫЙ ВВОД. Введите цифру 1 или 2\n";
        }
        Clear();
    }
    cout << "\n\nСпасибо за внимание. Надеюсь втретимся на Инкубаторе!!!";
    return 0;
}