#ifndef NAVIGATION_SYSTEM_H
#define NAVIGATION_SYSTEM_H

#include "Graph.h"
#include <vector>
#include <string>

// Structure to store route calculation results
struct PathResult {
    bool found;
    std::vector<int> path;                // Sequence of vertex IDs
    double totalDistance;                 // Total distance in meters
    int totalHops;                        // Number of road segments
    std::vector<std::string> roadNames;   // Road names along path
    std::vector<double> legDistances;     // Distance of each segment
    double walkingTimeMin;                // Walking time at ~5 km/h
    double cyclingTimeMin;                // Cycling time at ~15 km/h
    double drivingTimeMin;                // Driving/shuttle time at ~30 km/h

    PathResult() : found(false), totalDistance(0.0), totalHops(0),
                   walkingTimeMin(0.0), cyclingTimeMin(0.0), drivingTimeMin(0.0) {}
};

class NavigationSystem {
private:
    const Graph& graph;

    // Helper to calculate travel times based on speeds
    void calculateTravelTimes(PathResult& result) const;

public:
    explicit NavigationSystem(const Graph& g);

    // Dijkstra's Algorithm: Finds path with minimum total distance
    PathResult findShortestRoute(int src, int dest) const;
    PathResult findShortestRoute(const std::string& srcName, const std::string& destName) const;

    // Breadth-First Search (BFS): Finds path with minimum number of hops/turns
    PathResult findFewestHopsRoute(int src, int dest) const;
    PathResult findFewestHopsRoute(const std::string& srcName, const std::string& destName) const;

    // Graph Traversal: Finds all reachable locations from a given source
    std::vector<int> findReachableLocations(int src) const;

    // Search: Find location IDs matching query in name or description
    std::vector<int> searchLocations(const std::string& query) const;

    // Pretty printing navigation directions
    void printRouteSummary(const PathResult& result, const std::string& algorithmName) const;
    void printTurnByTurnDirections(const PathResult& result) const;
    void printAsciiRouteMap(const PathResult& result) const;
};

#endif // NAVIGATION_SYSTEM_H
