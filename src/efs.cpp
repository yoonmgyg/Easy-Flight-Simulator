#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <memory>

#include "geocoder.h"

using namespace std;

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

void Graph::insertEdge(string from, string to, int weight)
{
    graph[from].push_back(make_pair(to, weight));

    if (graph.find(to) == graph.end())
        graph[to] = {};

    numVertices++;
}

bool Graph::isEdge(string from, string to)
{
    for (int i = 0; i < graph[from].size(); i++)
    {
        if (graph[from].at(i).first == to)
            return true;
    }
    return false;
}

int Graph::getWeight(string from, string to)
{
    for (int i = 0; i < graph[from].size(); i++)
    {
        if (graph[from].at(i).first == to)
            return graph[from].at(i).second;
    }
    return 0;
}

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

void Graph::printGraph()
{
    for (std::map<string, vector<pair<string, int>>>::iterator it = graph.begin(); it != graph.end(); ++it)
    {
        cout << it->first;

        for (int i = 0; i < it->second.size(); i++)
        {
            cout << " " << it->second.at(i).first;
        }
        cout << "\n";
    }
}

int main()
{
  Graph myGraph;

  ifstream fin("Flight_Data.csv");

  string Airline, Flight_Number, Origin_Airport, Destination, Distance, temp;

  getline(fin, temp, '\n');
  while (fin.good())
    {
      getline(fin, Airline, ',');
      getline(fin, Flight_Number, ',');
      getline(fin, Origin_Airport, ',');
      getline(fin, Destination, ',');
      getline(fin, Distance, '\n');

      myGraph.insertEdge(Origin_Airport, Destination, stoi(Distance));
    }

  if (myGraph.isEdge("ANC", "SEA"))
    cout << "YES!" << endl;
  else
    cout << "NO!" << endl;
  unique_ptr<Geocoder>g(new Geocoder("a131785eb02d42349e3a14ba0dc5b471"));
  cout << g->geocode("MCO") << endl;
  return 0;
}
