#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <sstream>
#include <iterator>
#include <map>

using namespace std;

class Graph
{
    int N;
    
    list<int> *adj;
    
    void DFSUtil(int v, bool visited[], int roomCash[], int dynamic[]);
    
    public:
        Graph(int N);
        
        void addEdge(int v, int w);
        void DFS(int v, int roomCash[], int dynamic[]);
};

Graph::Graph(int N)
{
    this->N = N;
    adj = new list<int>[N];
}

void Graph::addEdge(int v, int w)
{
    adj[v].push_back(w);
}

void Graph::DFSUtil(int v, bool visited[], int roomCash[], int dynamic[])
{
    visited[v] = true;
    //cout << v << " ";
    
    list<int>::iterator i;
    for (i = adj[v].begin(); i !=  adj[v].end(); ++i)
        if (!visited[*i] || dynamic[*i] < dynamic[v] + roomCash[*i]){
            dynamic[*i] = dynamic[v] + roomCash[*i];
            DFSUtil(*i, visited, roomCash, dynamic);
        }
}

void Graph::DFS(int v, int roomCash[], int dynamic[])
{
    bool *visited = new bool[N];
    for (int i = 0; i < N; ++i)
        visited[i] = false;

    DFSUtil(v, visited, roomCash, dynamic);
}

int main()
{
    int N;
    cin >> N; cin.ignore();
    int *roomCash = new int[N+1];
    int *dynamic = new int[N+1];
    Graph g(N+1);
    vector<int> tokens;
    for (int i = 0; i < N; i++) {
        string room;
        getline(cin, room);
        istringstream rm(room);
        string n;
        while (rm >> n){
            if (n == "E")
                tokens.push_back(N);
            else
                tokens.push_back(stoi(n));
        }
        g.addEdge(tokens[0], tokens[2]);
        g.addEdge(tokens[0], tokens[3]);
        roomCash[tokens[0]] = tokens[1];
        tokens.clear();
    }
    
    dynamic[0] = roomCash[0];
    g.DFS(0, roomCash, dynamic);
    int big = -1;
    for (int i = 0; i < N; ++i)
        if(big < dynamic[i])
            big = dynamic[i];
            
    cout << big << endl;
}