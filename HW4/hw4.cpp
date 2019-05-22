#include <iostream>
#include <fstream>
#include <iomanip>
#include <set>
using namespace std;

/************************************
*  Note that in general using global
*  variables is a bad habit.
*  I use globals here because I don't 
*  want to scare you with "double***"
*************************************/

int m; // Column number (x)
int n; // Row    number (y)
double **v;  // n*(m+1) 2D-array
double **h;  // (n+1)*m 2D-array

// For internal use only. Do not really use them directly
double *real_v;
double *real_h;

double **dist;

struct cell
{
    int32_t x, y;
    double dist;
    cell(const int32_t &x, const int32_t &y, double d) : x(x), y(y), dist(d) {}
};

// operator to compare 2 cells
bool operator<(const cell& lhs, const cell& rhs) 
{ 
    if (lhs.dist == rhs.dist) 
    { 
        if (lhs.x != rhs.x) 
            return (lhs.x < rhs.x); 
        else
            return (lhs.y < rhs.y); 
    } 
    return (lhs.dist < rhs.dist); 
}

bool isInBoundary(const int32_t &y, const int32_t &x)
{
    return (y >= 0 && y <= n && x >= 0 && x <= m);
}

// extract weight information from h,v matrix
inline double weight(const int32_t &y, const int32_t &x, const int8_t &dy, const int8_t &dx)
{
    // Left
    if(dx == -1 && dy == 0)
        return h[y][x - 1];

    // Right 
    else if (dx == 1 && dy == 0)
        return h[y][x];

    // Up
    else if (dx == 0 && dy == -1)
        return v[y - 1][x];

    // Down
    else if (dx == 0 && dy == 1)
        return v[y][x];

    else 
        return 0.0;
}


bool readParameters()
{
    ifstream ifs("input/problem1/input1", ifstream::binary);

    if(!ifs.is_open())
        return false;

    ifs.read((char*)&m, sizeof(int));
    ifs.read((char*)&n, sizeof(int));

    cout << "(m,n) = (" << m << ", " << n << endl;

    real_v = new double[n*(m+1)];
    real_h = new double[(n+1)*m];

    ifs.read((char*)real_v, sizeof(double)*n*(m+1));
    ifs.read((char*)real_h, sizeof(double)*(n+1)*m);

    v = new double*[n];
    for (int i=0; i<n; ++i){
        v[i] = &(real_v[i*(m+1)]);
		
	}
    h = new double*[n+1];
    for (int i=0; i<n+1; ++i){
        h[i] = &(real_h[i*m]);
	}

    ifs.close();

    return true;
}

void release()
{
    delete []v;
    delete []h;

    delete []real_v;
    delete []real_h;

    for (int32_t i = 0; i < n + 1; ++i)
    {
        delete dist[i];
    }

    delete[] dist;
}

void printGrid()
{
    cout << "Matrix v" << endl;
    for (int32_t i = 0; i < n; ++i)
    {
        cout << "[" << i << "] ";
        for (int32_t j = 0; j <= m; ++j)
        {
            cout << setiosflags(ios::fixed) << setprecision(4) << v[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
    cout << "Matrix h" << endl;
    for (int32_t i = 0; i <= n; ++i)
    {
        cout << "[" << i << "] ";
        for (int32_t j = 0; j < m; ++j)
        {
            cout << setiosflags(ios::fixed) << setprecision(4) << h[i][j] << " ";
        }
        cout << endl;
    }
}

void printDist()
{
    if (dist == NULL) // empty
        return;
    cout << "Dist" << endl;
    for (int32_t i = 0; i < n + 1; ++i)
    {
        cout << "[" << i << "] ";
        for (int32_t j = 0; j < m + 1; ++j)
        {
            if (dist[i][j] > 1000.0)
                cout << "Inf ";
            else
                cout << dist[i][j] << " ";
        }
        cout << endl;
    }
}

inline void printDir(const uint8_t &d)
{
    switch (d)
            {
            case 0:
                cout << "l";
                break;
            case 1:
                cout << "d";
                break;
            case 2:
                cout << "r";
                break;
            case 3:
                cout << "u";
                break;
            default:
                break;
            }
}

// Dijkstra's algorithm
void findShortest()
{
    dist = new double*[n + 1];
    for(int32_t i = 0; i < n + 1; ++i)
        dist[i] = new double[m + 1];

    int8_t dx[] = {-1, 0, 1, 0}; 
    int8_t dy[] = { 0, 1, 0,-1};
    //              L  D  R  U

    set<cell> st;
    st.insert(cell(0, 0, 0.0));

    // initialize distance matrix with distance infinity
    for (int32_t i = 0; i < n + 1; ++i)
        for (int32_t j = 0; j < m + 1; ++j)
            dist[i][j] = __DBL_MAX__;

    dist[0][0] = 0.0;

    while(!st.empty())
    {
        cell u = *st.begin();
        st.erase(st.begin());

        for (uint8_t i = 0; i < 4; ++i)
        {

            int32_t x = u.x + dx[i];
            int32_t y = u.y + dy[i];

            if (!isInBoundary(y, x))
                continue;

            //cout << "MOV->[" << x << ", " << y << "]" << endl;

            if (dist[y][x] > dist[u.y][u.x] + weight(u.y, u.x, dy[i], dx[i]))
            {
                printDir(i);

                //if (dist[y][x] != __DBL_MAX__)
                //    st.erase(st.find(cell(y, x, dist[y][x])));
                if (dist[y][x] != __DBL_MAX__) {
                    auto res = st.find(cell(y, x, dist[y][x]));
                    if (res != st.end())
                        st.erase(res);
                }

                dist[y][x] = dist[u.y][u.x] + weight(u.y, u.x, dy[i], dx[i]);
                st.insert(cell(y, x, dist[y][x]));
                //cout << "ADD[" << x << "," << y << "] = " << weight(u.y, u.x, dy[i], dx[i]) << endl;
                //cout << "DIST["<< x << ", " << y << "] = " << dist[y][x] << endl;
            }
        }
    }

    cout << endl << dist[n][m] << endl;
}

int main()
{
    if(!readParameters())
    {
        cerr << "[ERROR] File reading failed!" << endl;
        return 0;
    }
    /***************************
     *
     *   your code/functions here
     *
     ***************************/
    printGrid();
    findShortest();
    printDist();

    release();
    return 0;
}