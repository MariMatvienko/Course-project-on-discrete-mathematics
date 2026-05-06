#include <iostream>
#include <fstream>
#include <string>
#include <codecvt> // для locale() и codecvt_utf8
#include <iomanip>
#include <vector>
using namespace std;

// Особенности вывода в файл: необходимо использовать 'wstring' вместо 'string' и добавлять перед текстом L: L"Текси".


// Выводит последовательность вершин найденного пути в консоль
void PrintPath(int* path, int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << " " << setw(3) << path[i] << " ";
    }
    cout << endl;
}

// Выводит всю таблицу (матрицу) текущего потока в консоль
void PrintFlow(int** flow, int size)
{
    cout << endl;
    for (int i = 0; i < size; i++)
    {
        PrintPath(flow[i], size);
    }
    cout << endl;
}

// Когда путь найден, эта функция увеличивает поток по всем его ребрам
void FoundedPath(int size, int** matrix, int** flow, int* path, int count)
{
    //cout << "Найден путь" << endl;
    //cout << "0 ->";
    /*
    for (int i = 0; i < count; i++)
    {
        cout << path[i] << " -> ";
    }
    cout << endl;
    */
    int mx = -1;
    for (int i = 0; i < count - 1; i++)
    {
        int l = matrix[path[i]][path[i + 1]];
        if (l > mx)
            mx = l;
    }
    //cout << "Максимум: " << mx << endl;
    for (int i = 0; i < count - 1; i++)
    {
        flow[path[i]][path[i + 1]] += mx;
    }
    //PrintFlow(flow, size);
}

// Проверяет, не посещали ли мы уже эту вершину в текущем пути (чтобы не ходить по кругу)
bool IsUnicVertex(int i, int* path, int count)
{
    for (int j = 0; j < count; j++)
        if (path[j] == i)
            return false;
    return true;
}

// count - это количество рёбер в пути 
// Ищет пути от начала до конца, чтобы заполнить начальный поток
void SearchPath(int size, int** matrix, int** flow, int* path, int count, int cur)
{
    path[count] = cur;
    if (cur == size - 1)
    {
        // обработка
        FoundedPath(size, matrix, flow, path, count + 1);
    }

    for (int i = 0; i < size; i++)
        if (IsUnicVertex(i, path, count))
        {
            int edge = matrix[cur][i];
            if (edge == 0)
            {
                continue;
            }
            SearchPath(size, matrix, flow, path, count+1, i);
        }
}

// Запускает процесс построения самого первого потока
void BuildFirstFlow(int size, int** flow, int** matrix)
{
    int* path = new int[size];
    SearchPath(size, matrix, flow, path, 0, 0);
}

// false - возвращается есть на пути есть насыщенные рёбра и поток нельзя уменьшить. В таком случае мы найдём следующий путь.
// true - значит на пути нет насыщенных рёбер и мы можем уменьшить поток. После уменьшения потока запускаем поиск заново.
// Когда путь найден, эта функция уменьшает поток по нему настолько, насколько это возможно
bool FoundedPathForDecrease(int size, int** matrix, int** flow, int* path, int count)
{
    int mn = 1e9;
    for (int i = 0; i < count - 1; i++)
    {
        int l = matrix[path[i]][path[i + 1]];
        int r = flow[path[i]][path[i + 1]];
        if (r - l < mn)
            mn = r - l;
    }
    if (mn == 0)
    {
        return false;
    } else 
    {
        for (int i = 0; i < count - 1; i++)
        {
            flow[path[i]][path[i + 1]] -= mn;
        }
        //PrintFlow(flow, size);
        return true;
    }
    //cout << "Максимум: " << mx << endl;
}

// count - это количество рёбер в пути 
// Ищет пути, по которым можно уменьшить поток, не нарушая нижние границы
bool SearchPathForDecrease(int size, int** matrix, int** flow, int* path, int count, int cur)
{
    path[count] = cur;
    //cout << "cur: " << cur << " count: " << count << endl;
    if (cur == size - 1)
    {
        // обработка
        if (FoundedPathForDecrease(size, matrix, flow, path, count + 1))
        {
            //cout << "true in SearchPathForDecrease" << endl;
            return true; // перезапустить поиск
        }
    }
    for (int i = 0; i < size; i++)
        if (IsUnicVertex(i, path, count))
        {
            int edge = matrix[cur][i];
            if (edge == 0)
            {
                continue;
            }
            if (SearchPathForDecrease(size, matrix, flow, path, count+1, i))
            {
                //cout << "true in SearchPathForDecrease" << endl;
                return true; // ступенчатый спуск по стеку вызовов
            }
        }
    return false; // полный выход из рекурсии: мы закончили
}

// Обработка пути в остаточной сети
bool FoundedPathForResidual(int size, int** ResidualNetwork, int** matrix, int** flow, int* path, int count)
{
    int mn = 1e9;
    for (int i = 0; i < count - 1; i++)
    {
        if (ResidualNetwork[path[i]][path[i + 1]] < mn)
            mn = ResidualNetwork[path[i]][path[i + 1]];
    }
    if (mn == 0)
    {
        return false;
    } else 
    {
        for (int i = 0; i < count - 1; i++)
        {
            if (matrix[path[i]][path[i + 1]] != 0)
            {
                ResidualNetwork[path[i]][path[i + 1]] -= mn;
                flow[path[i]][path[i + 1]] -= mn;
            } else
            {
                ResidualNetwork[path[i]][path[i + 1]] += mn;
                flow[path[i + 1]][path[i]] += mn;
            }
        }
        //PrintFlow(flow, size);
        return true;
    }
    //cout << "Максимум: " << mx << endl;
}

// count - это количество рёбер в пути 
// Ищет пути, по которым можно уменьшить поток, не нарушая нижние границы
// Это функция поиска в глубину для обработки остаточной сети
bool SearchPathForResidual(int size, int** ResidualNetwork, int** matrix, int** flow, int* path, int count, int cur)
{
    path[count] = cur;
    //cout << "cur: " << cur << " count: " << count << endl;
    if (cur == size - 1)
    {
        // обработка
        if (FoundedPathForResidual(size, ResidualNetwork, matrix, flow, path, count + 1))
        {
            //cout << "true in SearchPathForDecrease" << endl;
            return true; // перезапустить поиск
        }
    }
    for (int i = 0; i < size; i++)
        if (IsUnicVertex(i, path, count))
        {
            if (ResidualNetwork[cur][i] == 0) continue;
            if (SearchPathForDecrease(size, matrix, flow, path, count+1, i))
            {
                //cout << "true in SearchPathForDecrease" << endl;
                return true; // ступенчатый спуск по стеку вызовов
            }
        }
    return false; // полный выход из рекурсии: мы закончили
}

// Построение остаточной сети
void ConstructionResidualNetwork(int size, int** ResidualNetwork, int** matrix, int** flow)
{
    
    for (int i = 0; i < size; ++i) 
    {
        ResidualNetwork[i] = new int[size];
        for (int j = 0; j < size; ++j) 
        {
            ResidualNetwork[i][j] = 0; // заполняем матрицу нулями, чтобы она не забилась мусорными значениями 
        }
    }
    for (int i = 0; i < size; ++i) 
    {
        for (int j = 0; j < size; ++j) 
        {
            if (ResidualNetwork[i][j] != 0 || (flow[i][j] == 0 && matrix[i][j] == 0)) continue; // если ребро, которое мы до этого не обрабаотывали, уже не пустое => мы его заполнили ранее при обработке насыщенного ребра (третье условие ниже)
            else if (flow[i][j] - matrix[i][j] > 0) // если ребро в потоке не насыщенное 
            {
                ResidualNetwork[i][j] = flow[i][j] - matrix[i][j];
            } else if (flow[i][j] - matrix[i][j] == 0) // если ребро насыщенное, нужно развернуть его, те записать значение в ResidualNetwork[j][i]
            {
                ResidualNetwork[j][i] = matrix[i][j];
            }
        }
    }
}

// Запускает процесс уменьшения потока до минимально возможного значения с помощью осттаочной сети
bool DecreaseFlowForResidual(int size, int** ResidualNetwork, int** flow, int** matrix)
{
    int* path = new int[size];
    return SearchPathForResidual(size, ResidualNetwork, matrix, flow, path, 0, 0);
}

// Запускает процесс уменьшения потока
bool DecreaseFlow(int size, int** flow, int** matrix)
{
    int* path = new int[size];
    return SearchPathForDecrease(size, matrix, flow, path, 0, 0);
}

/* Функция заполнения первоначального потока:
передаём в функцию две матрицы: изначальная мтарица смежности графа и пустую для заполнения потока.
Мы должны идти поиском в ширину с использованием очереди. Найти путь, найти на нём максимальное значение из всех рёбер
после нужно прибавить поток к каждому ребру пути на это значение. Если на пути уже все рёбра насыщенны (удовлетваряют условию о нижней границе)
переходим к следующему пути. Когда все пути будут пройдены - певроначальный поток готов.
*/

/* Функция уменьшения потока
Снова поиск в ширину, находим путь. Находим для каждого ребра:
разницу между его потоком и нижней границе (те разница чисел в двух матрицах на один и тех же местах)
записываем эти значения в вектор, чтобы после найти минимальное в нём число. Это число, на которое на этом пути можно уменьшить поток на всех рёбрах.
После уменьшения потока продожаем искать пути, когда пути закончатся, поток будет минимальным.
*/
int main(int argc, char* argv[])
{
    // Input:    
    wifstream in(argv[1]);
    //wifstream in("D:\\test.txt");
    int size = 0, i, j;
    in >> size;
    int** matrix = new int* [size];
    for (i = 0; i < size; i++)
    {
        matrix[i] = new int[size];
        for (j = 0; j < size; j++)
            in >> matrix[i][j];
    }

    vector<wstring> position;
    wstring line;
    while (getline(in, line))
    {
        position.push_back(line);
    }

    in.close();
    
    // My code:
    wstring text1 = L"<Text>\nWe construct a minimal flow based on a directed, weighted graph.\nVertex numbered 0 is taken as the source, and the sink is the vertex with the highest number.\nThe numbers assigned to the edges are treated as the minimum capacity of the respective edge.\n";//, colors = L"<Vertex_Colors>\n";
    
    int new_size = size;
    int** flow = new int*[size];
    for (int i = 0; i < size; ++i) 
    {
        flow[i] = new int[size];
        for (int j = 0; j < size; ++j) 
        {
            flow[i][j] = 0; // пустая матрица: потока пока нет
        }
    }

    BuildFirstFlow(size, flow, matrix);
    while (DecreaseFlow(size, flow, matrix))
    {};
    int** ResidualNetwork = new int*[size];
    ConstructionResidualNetwork(size, ResidualNetwork, matrix, flow);
    while (DecreaseFlowForResidual(size, ResidualNetwork, flow, matrix))
    {};
    int flow_sum = 0;
    for (int i = 0; i < size; i++)
        flow_sum += flow[0][i];
    //cout << "Минимальный поток: " << flow_sum << endl;
    wstring text2 = L"<Text>\nThe minimum flow is.\n";//, colors = L"<Vertex_Colors>\n";

    // Output:
    wofstream out;
    out.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t, 0x10ffff, generate_header>));
    out.open(argv[1]);
    //out.open("D:\\test_out.txt");
    out << new_size << endl;
    for (i = 0; i < new_size; i++)
    {
        for (j = 0; j < new_size; j++)
        {
            out << flow[i][j];
            if (j != new_size - 1)
                out << ' ';
        }
        out << endl;
    }

    out << text1;
    out << text2 << flow_sum;;

    out << "<Positions>";
    for (auto s:position)
    {
        out << s << endl;
    }
    

    out.close();
    
    delete[] matrix;
    delete[] flow;

    return 0;
}
