#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <map>
#include <queue>
#include <vector>
#include <string>
#include <chrono>
#include <unordered_map>

using namespace std;
using namespace chrono;

typedef high_resolution_clock Clock;
#define INF 0x3f3f3f3f

//===== GRAPH ADJACENCY LIST IMPLEMENTATION =====//
class Graph
{
private:
    map<string, vector<pair<string, int>>> graph;
    int numVertices = 0;
public:
    void insertEdge(string from, string to, int weight);
    bool isEdge(string from, string to);
    int getWeight(string from, string to);
    vector<string> getAdjacent(string vertex);
    void printGraph();

    // graphs
    bool genericBFS(string from, string to);
    pair<vector<string>, int> detDijkstras(string from, string to);
};

// insert an edge between origin and destination, including distance between
void Graph::insertEdge(string from, string to, int weight)
{
    graph[from].push_back(make_pair(to, weight));

    if (graph.find(to) == graph.end())
        graph[to] = {};

    numVertices++;
}

// returns true if there is a flight from one airport to another
bool Graph::isEdge(string from, string to)
{
    for (int i = 0; i < graph[from].size(); i++)
    {
        if (graph[from].at(i).first == to)
            return true;
    }
    return false;
}

// returns the distance between two airports
int Graph::getWeight(string from, string to)
{
    for (int i = 0; i < graph[from].size(); i++)
    {
        if (graph[from].at(i).first == to)
            return graph[from].at(i).second;
    }
    return 0;
}

// returns a vector of all airports wich can be reach from an origin airport
vector<string> Graph::getAdjacent(string vertex)
{
    vector<string> adjacentVertices;
    for (int i = 0; i < graph[vertex].size(); i++)
    {
        adjacentVertices.push_back(graph[vertex].at(i).first);
    }
    sort(adjacentVertices.begin(), adjacentVertices.end());
    return adjacentVertices;
}

// prints all adjacent vertices (in ascending order) for each airport
void Graph::printGraph()
{
    for (std::map<string, vector<pair<string, int>>>::iterator it = graph.begin(); it != graph.end(); ++it)
    {
        cout << it->first;

        for (int i = 0; i < it->second.size(); i++)
        {
            cout << " " << it->second.at(i).first;
        }
        cout << "\n" << "\n" << "\n" << "\n";
    }
}

bool Graph::genericBFS(string from, string to)
{
    
    //bool *visited = new bool[graph.size()];
    map<string, bool> visited;
    for(auto itr = graph.begin(); itr != graph.end(); ++itr)
    {
        visited.emplace(itr->first, false);
    }
    queue<string> q;
    unordered_map<string, string> predecessor;
    // current node and then successor
    predecessor.insert(make_pair(from, "NULL"));

    visited.find(from)->second = true;
    q.push(from);
    int counter = 0;
    while(!q.empty())
    {
        from = q.front();
        q.pop();

        if(from == to)
        {
            //cout << "successfully found" << endl;
            return true;
        }

        vector<string> adjacents = getAdjacent(from);
        for(int i = 0; i < adjacents.size(); i++)
        {
            predecessor.insert(make_pair(adjacents.at(i), from));
            if(visited.find(adjacents.at(i))->second == false)
            {
                visited.find(adjacents.at(i))->second = true;
                q.push(adjacents.at(i));
            }
        }
        
    }
   return false;
}

//typedef pair<string, int> ports;
pair<vector<string>, int> Graph::detDijkstras(string from, string to)
{
    priority_queue<pair<string, int>, vector<pair<string, int>>, greater<pair<string, int>>> pq;

      unordered_map<string, pair<string, int>> dist;   

    for(auto it = graph.begin(); it != graph.end(); ++it)
    {
        dist.insert(make_pair(it->first, make_pair("NULL", INF)));
    }

    pq.push(make_pair(from, 0));
    dist.find(from)->second.second = 0;

    while(!pq.empty())
    {
        string u = pq.top().first;
        pq.pop();
        auto it = graph.find(u);
        for(int i = 0; i < it->second.size(); i++)
        {
            string v = it->second.at(i).first;
            int weight = it->second.at(i).second;

            if(dist.find(v)->second.second > dist.find(u)->second.second + weight)
            {
                dist.find(v)->second.second = dist.find(u)->second.second + weight;
                  pq.push(make_pair(v, dist.find(v)->second.second));
                  dist.find(v)->second.first = u;

            }


        }
        
    }

   pair<vector<string>, int> data;
   auto find = dist.find(to);
   int counter = 0;
   counter += find->second.second;
   data.first.push_back(find->first);
   while(find != dist.end())
   {
       if(find->second.first != "NULL")
       {
           find = dist.find(find->second.first);
           data.first.push_back(find->first);
           counter += find->second.second;
       }
       else
       {
           break;
       }
   }
   // cout << "dist: " << counter << endl;
    
    data.second = counter;

 /*  for(int i = data.first.size() - 1; i >= 0; --i)
   {
       cout << data.first.at(i) << endl;
   }*/

   return data;
}

int main()
{
    Graph myGraph;
    auto t1 = Clock::now();

    ifstream fin("Flight_Data.csv");

    string Airline, Flight_Number, Origin_Airport, Destination, Distance, temp;

    cout << "Initializing Data..." << endl;

    getline(fin, temp, '\n'); // clearing the title string from csv
    while (fin.good())
    {
        getline(fin, Airline, ',');
        getline(fin, Flight_Number, ',');
        getline(fin, Origin_Airport, ',');
        getline(fin, Destination, ',');
        getline(fin, Distance, '\n');

        myGraph.insertEdge(Origin_Airport, Destination, stoi(Distance));
    }

    auto t2 = Clock::now();
    cout << "Time Elapsed: " << duration_cast<nanoseconds>(t2 - t1).count()
        << " nanoseconds" << endl;

    //===================== INSERT ANY VALUES YOU WANT TO TEST HERE ======================//
    bool Continue = 1;
    while (Continue == 1)
    {
        cout << "Enter the airport you wish to Depart (i.e. MCO - Orlando International)" << endl;
        string from;
        cin >> from;
        cout << "Enter the airport you wish to travel to: (i.e. DFW - Dallas Fort Worth International)" << endl;
        string to;
        cin >> to;

        if(myGraph.genericBFS(from, to) == false)
        {
            cout << "No such flight from " << from << " to " << to << " exists." << endl;
        }
        else
        {
            cout << "Such flight from " << from << " to " << to << " exists." << endl;
            pair<vector<string>, int> data = myGraph.detDijkstras(from, to);
            for(int i = data.first.size() - 1; i >= 0; i--)
            {
                if (i == 0)
                    cout << data.first.at(i) << endl;
                else
                    cout << data.first.at(i) << " -> ";
            }
            cout << "Distance from " << from << " to " << to << " is " << data.second << " miles!" << endl;
        }
            

        cout << "\n" << "Would you like to enter another trip?" << endl;
        cout << "Enter 1 to continue" << endl;
        cout << "Enter 0 to quit" << endl;
        cin >> Continue;
    }
    return 0;
}