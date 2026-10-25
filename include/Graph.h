#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <vector>
#include <unordered_map>

// Represents a directed/undirected edge in the map graph
struct Edge {
    int to;                 // Destination vertex ID
    double distance;        // Weight: distance in meters
    std::string roadName;   // Name of road/pathway
};

// Represents a location/vertex in the map
struct Vertex {
    int id;                 // Unique sequential ID
    std::string name;       // Landmark/building name
    std::string description;// Short description
};

class Graph {
private:
    std::vector<Vertex> vertices;                     // List of all vertices
    std::vector<std::vector<Edge>> adj;              // Adjacency list
    std::unordered_map<std::string, int> nameToIndex; // Case-insensitive name lookup
    int edgeCount;                                    // Count of unique connections

    std::string normalizeKey(const std::string& key) const;

public:
    Graph();

    // Add a new vertex (location) to the graph. Returns its new index.
    int addVertex(const std::string& name, const std::string& description = "");

    // Add an edge (road) between two vertices using their IDs
    bool addEdge(int u, int v, double distance, const std::string& roadName = "Pathway", bool bidirectional = true);

    // Add an edge using vertex names
    bool addEdge(const std::string& uName, const std::string& vName, double distance, const std::string& roadName = "Pathway", bool bidirectional = true);

    // Lookups and getters
    int getVertexIndex(const std::string& name) const;
    std::string getVertexName(int id) const;
    const Vertex& getVertex(int id) const;
    const std::vector<Edge>& getNeighbors(int u) const;
    int getVertexCount() const;
    int getEdgeCount() const;
    bool hasVertex(const std::string& name) const;
    const std::vector<Vertex>& getAllVertices() const;

    // Display helpers
    void displayLocations() const;
    void displayAdjacencyList() const;

    // File I/O
    bool loadFromFile(const std::string& filepath);
    bool saveToFile(const std::string& filepath) const;
    void clear();
};

#endif // GRAPH_H
