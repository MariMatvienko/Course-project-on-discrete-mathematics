#include <iostream>
#include <fstream>
#include <string>
#include <codecvt> // ¤«ï locale() ¨ codecvt_utf8
#include <iomanip>
#include <vector>
using namespace std;

// Žá®¡¥­­®áâ¨ ¢ë¢®¤  ¢ ä ©«: ­¥®¡å®¤¨¬® ¨á¯®«ì§®¢ âì 'wstring' ¢¬¥áâ® 'string' ¨ ¤®¡ ¢«ïâì ¯¥à¥¤ â¥ªáâ®¬ L: L"’¥ªá¨".


// ‚ë¢®¤¨â ¯®á«¥¤®¢ â¥«ì­®áâì ¢¥àè¨­ ­ ©¤¥­­®£® ¯ãâ¨ ¢ ª®­á®«ì
void PrintPath(int* path, int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << " " << setw(3) << path[i] << " ";
    }
    cout << endl;
}

// ‚ë¢®¤¨â ¢áî â ¡«¨æã (¬ âà¨æã) â¥ªãé¥£® ¯®â®ª  ¢ ª®­á®«ì
void PrintFlow(int** flow, int size)
{
    cout << endl;
    for (int i = 0; i < size; i++)
    {
        PrintPath(flow[i], size);
    }
    cout << endl;
}

// Š®£¤  ¯ãâì ­ ©¤¥­, íâ  äã­ªæ¨ï ã¢¥«¨ç¨¢ ¥â ¯®â®ª ¯® ¢á¥¬ ¥£® à¥¡à ¬
void FoundedPath(int size, int** matrix, int** flow, int* path, int count)
{
    //cout << " ©¤¥­ ¯ãâì" << endl;
    //cout << "0 ->";
    for (int i = 0; i < count; i++)
    {
        cout << path[i] << " -> ";
    }
    cout << endl;
    int mx = -1;
    for (int i = 0; i < count - 1; i++)
    {
        int l = matrix[path[i]][path[i + 1]];
        if (l > mx)
            mx = l;
    }
    //cout << "Œ ªá¨¬ã¬: " << mx << endl;
    for (int i = 0; i < count - 1; i++)
    {
        flow[path[i]][path[i + 1]] += mx;
    }
    //PrintFlow(flow, size);
}

// à®¢¥àï¥â, ­¥ ¯®á¥é «¨ «¨ ¬ë ã¦¥ íâã ¢¥àè¨­ã ¢ â¥ªãé¥¬ ¯ãâ¨ (çâ®¡ë ­¥ å®¤¨âì ¯® ªàã£ã)
bool IsUnicVertex(int i, int* path, int count)
{
    for (int j = 0; j < count; j++)
        if (path[j] == i)
            return false;
    return true;
}

// count - íâ® ª®«¨ç¥áâ¢® àñ¡¥à ¢ ¯ãâ¨ 
// ˆé¥â ¯ãâ¨ ®â ­ ç «  ¤® ª®­æ , çâ®¡ë § ¯®«­¨âì ­ ç «ì­ë© ¯®â®ª
void SearchPath(int size, int** matrix, int** flow, int* path, int count, int cur)
{
    path[count] = cur;
    //printf("cur %d\n", cur);
    //cout << "cur: " << cur << " count: " << count << endl;
    if (cur == size - 1)
    {
        // ®¡à ¡®âª 
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

// ‡ ¯ãáª ¥â ¯à®æ¥áá ¯®áâà®¥­¨ï á ¬®£® ¯¥à¢®£® ¯®â®ª 
void BuildFirstFlow(int size, int** flow, int** matrix)
{
    int* path = new int[size];
    SearchPath(size, matrix, flow, path, 0, 0);
}

// false - ¢®§¢à é ¥âáï ¥áâì ­  ¯ãâ¨ ¥áâì ­ áëé¥­­ë¥ àñ¡à  ¨ ¯®â®ª ­¥«ì§ï ã¬¥­ìè¨âì. ‚ â ª®¬ á«ãç ¥ ¬ë ­ ©¤ñ¬ á«¥¤ãîé¨© ¯ãâì.
// true - §­ ç¨â ­  ¯ãâ¨ ­¥â ­ áëé¥­­ëå àñ¡¥à ¨ ¬ë ¬®¦¥¬ ã¬¥­ìè¨âì ¯®â®ª. ®á«¥ ã¬¥­ìè¥­¨ï ¯®â®ª  § ¯ãáª ¥¬ ¯®¨áª § ­®¢®.
// Š®£¤  ¯ãâì ­ ©¤¥­, íâ  äã­ªæ¨ï ã¬¥­ìè ¥â ¯®â®ª ¯® ­¥¬ã ­ áâ®«ìª®, ­ áª®«ìª® íâ® ¢®§¬®¦­®
bool FoundedPathForDecrease(int size, int** matrix, int** flow, int* path, int count)
{
    //cout << " ©¤¥­ ¯ãâì" << endl;
    //cout << "0 ->";
    for (int i = 0; i < count; i++)
    {
        cout << path[i] << " -> ";
    }
    cout << endl;
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
    //cout << "Œ ªá¨¬ã¬: " << mx << endl;
}

// count - íâ® ª®«¨ç¥áâ¢® àñ¡¥à ¢ ¯ãâ¨ 
// ˆé¥â ¯ãâ¨, ¯® ª®â®àë¬ ¬®¦­® ã¬¥­ìè¨âì ¯®â®ª, ­¥ ­ àãè ï ­¨¦­¨¥ £à ­¨æë
bool SearchPathForDecrease(int size, int** matrix, int** flow, int* path, int count, int cur)
{
    path[count] = cur;
    //printf("cur %d\n", cur);
    //cout << "cur: " << cur << " count: " << count << endl;
    if (cur == size - 1)
    {
        // ®¡à ¡®âª 
        if (FoundedPathForDecrease(size, matrix, flow, path, count + 1))
        {
            cout << "true in SearchPathForDecrease" << endl;
            return true; // ¯¥à¥§ ¯ãáâ¨âì ¯®¨áª
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
                return true; // áâã¯¥­ç âë© á¯ãáª ¯® áâ¥ªã ¢ë§®¢®¢
            }
        }
    return false; // ¯®«­ë© ¢ëå®¤ ¨§ à¥ªãàá¨¨: ¬ë § ª®­ç¨«¨
}


// ‡ ¯ãáª ¥â ¯à®æ¥áá ã¬¥­ìè¥­¨ï ¯®â®ª  ¤® ¬¨­¨¬ «ì­® ¢®§¬®¦­®£® §­ ç¥­¨ï
bool DecreaseFlow(int size, int** flow, int** matrix)
{
    int* path = new int[size];
    return SearchPathForDecrease(size, matrix, flow, path, 0, 0);
}

/* ”ã­ªæ¨ï § ¯®«­¥­¨ï ¯¥à¢®­ ç «ì­®£® ¯®â®ª :
¯¥à¥¤ ñ¬ ¢ äã­ªæ¨î ¤¢¥ ¬ âà¨æë: ¨§­ ç «ì­ ï ¬â à¨æ  á¬¥¦­®áâ¨ £à ä  ¨ ¯ãáâãî ¤«ï § ¯®«­¥­¨ï ¯®â®ª .
Œë ¤®«¦­ë ¨¤â¨ ¯®¨áª®¬ ¢ è¨à¨­ã á ¨á¯®«ì§®¢ ­¨¥¬ ®ç¥à¥¤¨.  ©â¨ ¯ãâì, ­ ©â¨ ­  ­ñ¬ ¬ ªá¨¬ «ì­®¥ §­ ç¥­¨¥ ¨§ ¢á¥å àñ¡¥à
¯®á«¥ ­ã¦­® ¯à¨¡ ¢¨âì ¯®â®ª ª ª ¦¤®¬ã à¥¡àã ¯ãâ¨ ­  íâ® §­ ç¥­¨¥. …á«¨ ­  ¯ãâ¨ ã¦¥ ¢á¥ àñ¡à  ­ áëé¥­­ë (ã¤®¢«¥â¢ àïîâ ãá«®¢¨î ® ­¨¦­¥© £à ­¨æ¥)
¯¥à¥å®¤¨¬ ª á«¥¤ãîé¥¬ã ¯ãâ¨. Š®£¤  ¢á¥ ¯ãâ¨ ¡ã¤ãâ ¯à®©¤¥­ë - ¯¥¢à®­ ç «ì­ë© ¯®â®ª £®â®¢.
*/

/* ”ã­ªæ¨ï ã¬¥­ìè¥­¨ï ¯®â®ª 
‘­®¢  ¯®¨áª ¢ è¨à¨­ã, ­ å®¤¨¬ ¯ãâì.  å®¤¨¬ ¤«ï ª ¦¤®£® à¥¡à :
à §­¨æã ¬¥¦¤ã ¥£® ¯®â®ª®¬ ¨ ­¨¦­¥© £à ­¨æ¥ (â¥ à §­¨æ  ç¨á¥« ¢ ¤¢ãå ¬ âà¨æ å ­  ®¤¨­ ¨ â¥å ¦¥ ¬¥áâ å)
§ ¯¨áë¢ ¥¬ íâ¨ §­ ç¥­¨ï ¢ ¢¥ªâ®à, çâ®¡ë ¯®á«¥ ­ ©â¨ ¬¨­¨¬ «ì­®¥ ¢ ­ñ¬ ç¨á«®. â® ç¨á«®, ­  ª®â®à®¥ ­  íâ®¬ ¯ãâ¨ ¬®¦­® ã¬¥­ìè¨âì ¯®â®ª ­  ¢á¥å àñ¡à å.
®á«¥ ã¬¥­ìè¥­¨ï ¯®â®ª  ¯à®¤®¦ ¥¬ ¨áª âì ¯ãâ¨, ª®£¤  ¯ãâ¨ § ª®­ç âáï, ¯®â®ª ¡ã¤¥â ¬¨­¨¬ «ì­ë¬.
*/
int main(int argc, char* argv[])
{
    // Input:    
    wifstream in(argv[1]);
    //ifstream in("D:\\test.txt");
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
            flow[i][j] = 0; // ¯ãáâ ï ¬ âà¨æ : ¯®â®ª  ¯®ª  ­¥â
        }
    }

    BuildFirstFlow(size, flow, matrix);
    while (DecreaseFlow(size, flow, matrix))
    {};
    PrintFlow(flow, size);
    int flow_sum = 0;
    for (int i = 0; i < size; i++)
        flow_sum += flow[0][i];
    cout << "Œ¨­¨¬ «ì­ë© ¯®â®ª: " << flow_sum << endl;
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
