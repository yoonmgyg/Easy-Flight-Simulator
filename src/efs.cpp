#include <iostream>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>
#include <memory>

#include "geocoder.h"
#include <matplot/matplot.h>

using namespace std;

class City
{
private:
  double latitude;
  double longitude;
  string name;
public:
  City(string geocode);
  double getLat();
  double getLng();
  string getNam();  
};

City::City(string geocode, string name) {
  this->latitude = stod(geocode.substr(geocode.find("lat") + 5, geocode.find("lat") + 9));
  cout << this->latitude << endl;
  this->longitude = stod(geocode.substr(geocode.find("lng") + 5, geocode.find("lng") + 9));
  cout << this->longitude << endl;
  this->name = name;
}

double City::getLat() {
  return this->latitude;
}

double City::getLng() {
  return this->longitude;
}

string City::getNam() {
  return this->name;
}

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
  string geocode =  g->geocode("MCO");
  City* orlando = new City(geocode);
  string geocodeTwo = g->geocode("JFK");
  City* nyc = new City(geocodeTwo);
  
  matplot::geoplot(std::vector{orlando->getLat(), nyc->getLat()},
		   std::vector{orlando->getLng(), nyc->getLng()}, "g-*");
  matplot::geolimits({25, 50},
		     {-130, -65});
  matplot::text(nyc->getLng(), nyc->getLat(), nyc->getNam());
  matplot::text(orlando->getLng(), orlando->getLat(), orlando->getNam());
  matplot::show();
  
  return 0;
}
