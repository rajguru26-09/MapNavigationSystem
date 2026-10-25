#include "NavigationSystem.h"
#include "Utils.h"
#include <iostream>
#include <queue>
#include <limits>
#include <algorithm>
#include <iomanip>

const double INF = std::numeric_limits<double>::infinity();

NavigationSystem::NavigationSystem(const Graph& g) : graph(g) {}

void NavigationSystem::calculateTravelTimes(PathResult& result) const {
    // 5 km/h = 83.33 m/min (Walking)
    // 15 km/h = 250.0 m/min (Cycling)
    // 30 km/h = 500.0 m/min (Driving / Campus Shuttle)
    result.walkingTimeMin = result.totalDistance / (5000.0 / 60.0);
    result.cyclingTimeMin = result.totalDistance / (15000.0 / 60.0);
    result.drivingTimeMin = result.totalDistance / (30000.0 / 60.0);
}

PathResult NavigationSystem::findShortestRoute(int src, int dest) const {
    PathResult res;
    int n = graph.getVertexCount();
    if (src < 0 || src >= n || dest < 0 || dest >= n) {
        return res;
    }

    if (src == dest) {
        res.found = true;
        res.path = {src};
        res.totalDistance = 0.0;
        res.totalHops = 0;
        calculateTravelTimes(res);
        return res;
    }

    std::vector<double> dist(n, INF);
    std::vector<int> parent(n, -1);
    std::vector<std::string> parentRoad(n, "");
    std::vector<double> parentDist(n, 0.0);

    // Min-heap priority queue: stores pair<distance, vertex_id>
    using PII = std::pair<double, int>;
    std::priority_queue<PII, std::vector<PII>, std::greater<PII>> pq;

    dist[src] = 0.0;
    pq.push({0.0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();

        if (d > dist[u]) continue;
        if (u == dest) break; // Reached target destination

        for (const auto& edge : graph.getNeighbors(u)) {
            int v = edge.to;
            double weight = edge.distance;

            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                parentRoad[v] = edge.roadName;
                parentDist[v] = weight;
                pq.push({dist[v], v});
            }
        }
    }

    if (dist[dest] == INF) {
        res.found = false;
        return res;
    }

    // Reconstruct the path backwards
    std::vector<int> pathNodes;
    std::vector<std::string> roads;
    std::vector<double> legs;

    int curr = dest;
    while (curr != -1) {
        pathNodes.push_back(curr);
        if (parent[curr] != -1) {
            roads.push_back(parentRoad[curr]);
            legs.push_back(parentDist[curr]);
        }
        curr = parent[curr];
    }

    std::reverse(pathNodes.begin(), pathNodes.end());
    std::reverse(roads.begin(), roads.end());
    std::reverse(legs.begin(), legs.end());

    res.found = true;
    res.path = pathNodes;
    res.totalDistance = dist[dest];
    res.totalHops = static_cast<int>(roads.size());
    res.roadNames = roads;
    res.legDistances = legs;
    calculateTravelTimes(res);

    return res;
}

PathResult NavigationSystem::findShortestRoute(const std::string& srcName, const std::string& destName) const {
    int src = graph.getVertexIndex(srcName);
    int dest = graph.getVertexIndex(destName);
    if (src == -1 || dest == -1) {
        PathResult res;
        res.found = false;
        return res;
    }
    return findShortestRoute(src, dest);
}

PathResult NavigationSystem::findFewestHopsRoute(int src, int dest) const {
    PathResult res;
    int n = graph.getVertexCount();
    if (src < 0 || src >= n || dest < 0 || dest >= n) {
        return res;
    }

    if (src == dest) {
        res.found = true;
        res.path = {src};
        res.totalDistance = 0.0;
        res.totalHops = 0;
        calculateTravelTimes(res);
        return res;
    }

    std::vector<bool> visited(n, false);
    std::vector<int> parent(n, -1);
    std::vector<std::string> parentRoad(n, "");
    std::vector<double> parentDist(n, 0.0);

    std::queue<int> q;
    visited[src] = true;
    q.push(src);

    bool reached = false;
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        if (u == dest) {
            reached = true;
            break;
        }

        for (const auto& edge : graph.getNeighbors(u)) {
            int v = edge.to;
            if (!visited[v]) {
                visited[v] = true;
                parent[v] = u;
                parentRoad[v] = edge.roadName;
                parentDist[v] = edge.distance;
                q.push(v);
            }
        }
    }

    if (!reached) {
        res.found = false;
        return res;
    }

    // Reconstruct path
    std::vector<int> pathNodes;
    std::vector<std::string> roads;
    std::vector<double> legs;
    double totalDist = 0.0;

    int curr = dest;
    while (curr != -1) {
        pathNodes.push_back(curr);
        if (parent[curr] != -1) {
            roads.push_back(parentRoad[curr]);
            legs.push_back(parentDist[curr]);
            totalDist += parentDist[curr];
        }
        curr = parent[curr];
    }

    std::reverse(pathNodes.begin(), pathNodes.end());
    std::reverse(roads.begin(), roads.end());
    std::reverse(legs.begin(), legs.end());

    res.found = true;
    res.path = pathNodes;
    res.totalDistance = totalDist;
    res.totalHops = static_cast<int>(roads.size());
    res.roadNames = roads;
    res.legDistances = legs;
    calculateTravelTimes(res);

    return res;
}

PathResult NavigationSystem::findFewestHopsRoute(const std::string& srcName, const std::string& destName) const {
    int src = graph.getVertexIndex(srcName);
    int dest = graph.getVertexIndex(destName);
    if (src == -1 || dest == -1) {
        PathResult res;
        res.found = false;
        return res;
    }
    return findFewestHopsRoute(src, dest);
}

std::vector<int> NavigationSystem::findReachableLocations(int src) const {
    std::vector<int> reachable;
    int n = graph.getVertexCount();
    if (src < 0 || src >= n) return reachable;

    std::vector<bool> visited(n, false);
    std::queue<int> q;

    visited[src] = true;
    q.push(src);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        reachable.push_back(u);

        for (const auto& edge : graph.getNeighbors(u)) {
            if (!visited[edge.to]) {
                visited[edge.to] = true;
                q.push(edge.to);
            }
        }
    }

    return reachable;
}

std::vector<int> NavigationSystem::searchLocations(const std::string& query) const {
    std::vector<int> matches;
    std::string cleanQ = Utils::toLower(Utils::trim(query));
    if (cleanQ.empty()) return matches;

    const auto& all = graph.getAllVertices();
    for (const auto& v : all) {
        std::string nameLower = Utils::toLower(v.name);
        std::string descLower = Utils::toLower(v.description);
        if (nameLower.find(cleanQ) != std::string::npos ||
            descLower.find(cleanQ) != std::string::npos) {
            matches.push_back(v.id);
        }
    }
    return matches;
}

void NavigationSystem::printRouteSummary(const PathResult& result, const std::string& algorithmName) const {
    if (!result.found) {
        std::cout << Colors::RED << "\n[!] No navigation path exists between the selected locations." << Colors::RESET << "\n";
        return;
    }

    std::cout << "\n" << Colors::BOLD << Colors::GREEN << "=================================================================" << Colors::RESET << "\n";
    std::cout << Colors::BOLD_CYAN << "  ROUTE FOUND (" << algorithmName << ")" << Colors::RESET << "\n";
    std::cout << Colors::BOLD << Colors::GREEN << "=================================================================" << Colors::RESET << "\n";
    std::cout << " • Origin:         " << Colors::BOLD_YELLOW << graph.getVertexName(result.path.front()) << Colors::RESET << "\n";
    std::cout << " • Destination:    " << Colors::BOLD_YELLOW << graph.getVertexName(result.path.back()) << Colors::RESET << "\n";
    std::cout << " • Total Distance: " << Colors::BOLD_GREEN << Utils::formatDistance(result.totalDistance) << Colors::RESET << "\n";
    std::cout << " • Road Segments:  " << result.totalHops << " segments (" << result.path.size() << " landmarks)\n";
    std::cout << Colors::DIM << "-----------------------------------------------------------------" << Colors::RESET << "\n";
    std::cout << Colors::BOLD << " Estimated Travel Times:" << Colors::RESET << "\n";
    std::cout << "   🚶 Walking  (~5 km/h):   " << Colors::CYAN << Utils::formatTime(result.walkingTimeMin) << Colors::RESET << "\n";
    std::cout << "   🚲 Cycling  (~15 km/h):  " << Colors::CYAN << Utils::formatTime(result.cyclingTimeMin) << Colors::RESET << "\n";
    std::cout << "   🚗 Shuttle  (~30 km/h):  " << Colors::CYAN << Utils::formatTime(result.drivingTimeMin) << Colors::RESET << "\n";
    std::cout << Colors::BOLD << Colors::GREEN << "=================================================================" << Colors::RESET << "\n";
}

void NavigationSystem::printTurnByTurnDirections(const PathResult& result) const {
    if (!result.found || result.path.size() <= 1) return;

    std::cout << "\n" << Colors::BOLD << Colors::WHITE << ">>> Step-by-Step Navigation Directions <<<" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "-----------------------------------------------------------------" << Colors::RESET << "\n";

    double cumDist = 0.0;
    std::cout << Colors::BOLD_YELLOW << " [START] " << Colors::RESET
              << "Depart from " << Colors::BOLD << graph.getVertexName(result.path[0]) << Colors::RESET << "\n";

    for (size_t i = 0; i < result.roadNames.size(); ++i) {
        cumDist += result.legDistances[i];
        int nextNode = result.path[i + 1];

        std::cout << "   Step " << (i + 1) << ": Follow "
                  << Colors::CYAN << "\"" << result.roadNames[i] << "\"" << Colors::RESET
                  << " for " << Colors::GREEN << Utils::formatDistance(result.legDistances[i]) << Colors::RESET
                  << " ➔ Reach " << Colors::BOLD << graph.getVertexName(nextNode) << Colors::RESET
                  << Colors::DIM << " (Cumulative: " << Utils::formatDistance(cumDist) << ")" << Colors::RESET << "\n";
    }

    std::cout << Colors::BOLD_GREEN << " [ARRIVE] " << Colors::RESET
              << "You have reached your destination: "
              << Colors::BOLD << graph.getVertexName(result.path.back()) << Colors::RESET << "!\n";
    std::cout << Colors::DIM << "-----------------------------------------------------------------" << Colors::RESET << "\n";
}

void NavigationSystem::printAsciiRouteMap(const PathResult& result) const {
    if (!result.found || result.path.empty()) return;

    std::cout << "\n" << Colors::BOLD_CYAN << ">>> Route Map Trail <<<" << Colors::RESET << "\n";
    for (size_t i = 0; i < result.path.size(); ++i) {
        std::cout << Colors::BOLD_YELLOW << "[" << graph.getVertexName(result.path[i]) << "]" << Colors::RESET;
        if (i < result.roadNames.size()) {
            std::cout << "\n    │\n    ▼  " << Colors::DIM << "(" << result.roadNames[i]
                      << ", " << Utils::formatDistance(result.legDistances[i]) << ")" << Colors::RESET << "\n";
        }
    }
    std::cout << "\n";
}
