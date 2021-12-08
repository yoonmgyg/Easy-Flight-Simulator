#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <chrono>

using namespace std;
using namespace chrono;

typedef high_resolution_clock Clock;

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
    cout << "Sime Elapsed: " << duration_cast<nanoseconds>(t2 - t1).count()
        << " nanoseconds" << endl;

    //===================== INSERT ANY VALUES YOU WANT TO TEST HERE ======================//
    bool Continue = 1;
    while (Continue == 1)
    {
        cout << "Enter the airport you wish to Depart (i.e. MCO - Orlando International" << endl;
        string from;
        cin >> from;
        cout << "Enter the airport you wish to travel to: (i.e. DFW - Dallas Fort Worth International" << endl;
        string to;
        cin >> to;

        if (myGraph.isEdge(from, to))
            cout << "YES!" << endl;
        else
            cout << "NO!" << endl;
        cout << "\n" << "Would you like to enter another trip?" << endl;
        cout << "Enter 1 to continue" << endl;
        cout << "Enter 0 to quit" << endl;
        cin >> Continue;
    }
    return 0;
}
