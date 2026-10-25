#include "Graph.h"
#include "Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

Graph::Graph() : edgeCount(0) {}

std::string Graph::normalizeKey(const std::string& key) const {
    return Utils::toLower(Utils::trim(key));
}

int Graph::addVertex(const std::string& name, const std::string& description) {
    std::string cleanName = Utils::trim(name);
    std::string key = normalizeKey(cleanName);

    if (nameToIndex.find(key) != nameToIndex.end()) {
        return nameToIndex[key];
    }

    int id = static_cast<int>(vertices.size());
    vertices.push_back({id, cleanName, Utils::trim(description)});
    adj.push_back(std::vector<Edge>());
    nameToIndex[key] = id;
    return id;
}

bool Graph::addEdge(int u, int v, double distance, const std::string& roadName, bool bidirectional) {
    if (u < 0 || u >= static_cast<int>(vertices.size()) ||
        v < 0 || v >= static_cast<int>(vertices.size())) {
        return false;
    }
    if (u == v) return false;

    // Check if edge already exists to update or avoid duplicates
    for (auto& edge : adj[u]) {
        if (edge.to == v) {
            edge.distance = distance;
            edge.roadName = roadName;
            if (bidirectional) {
                for (auto& revEdge : adj[v]) {
                    if (revEdge.to == u) {
                        revEdge.distance = distance;
                        revEdge.roadName = roadName;
                    }
                }
            }
            return true;
        }
    }

    adj[u].push_back({v, distance, roadName});
    if (bidirectional) {
        adj[v].push_back({u, distance, roadName});
    }
    edgeCount++;
    return true;
}

bool Graph::addEdge(const std::string& uName, const std::string& vName, double distance, const std::string& roadName, bool bidirectional) {
    int u = getVertexIndex(uName);
    int v = getVertexIndex(vName);
    if (u == -1 || v == -1) return false;
    return addEdge(u, v, distance, roadName, bidirectional);
}

int Graph::getVertexIndex(const std::string& name) const {
    std::string key = normalizeKey(name);
    auto it = nameToIndex.find(key);
    if (it != nameToIndex.end()) {
        return it->second;
    }
    return -1;
}

std::string Graph::getVertexName(int id) const {
    if (id >= 0 && id < static_cast<int>(vertices.size())) {
        return vertices[id].name;
    }
    return "Unknown";
}

const Vertex& Graph::getVertex(int id) const {
    return vertices.at(id);
}

const std::vector<Edge>& Graph::getNeighbors(int u) const {
    return adj.at(u);
}

int Graph::getVertexCount() const {
    return static_cast<int>(vertices.size());
}

int Graph::getEdgeCount() const {
    return edgeCount;
}

bool Graph::hasVertex(const std::string& name) const {
    return nameToIndex.find(normalizeKey(name)) != nameToIndex.end();
}

const std::vector<Vertex>& Graph::getAllVertices() const {
    return vertices;
}

void Graph::displayLocations() const {
    std::cout << "\n" << Colors::BOLD << Colors::CYAN << "=== Campus Landmarks & Locations (" << vertices.size() << ") ===" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "--------------------------------------------------------------------------------" << Colors::RESET << "\n";
    std::cout << std::left << std::setw(6)  << "ID"
              << std::left << std::setw(28) << "Location Name"
              << std::left << "Description / Landmark Details" << "\n";
    std::cout << Colors::DIM << "--------------------------------------------------------------------------------" << Colors::RESET << "\n";

    for (const auto& v : vertices) {
        std::cout << std::left << std::setw(6)  << v.id
                  << Colors::BOLD_GREEN << std::left << std::setw(28) << v.name << Colors::RESET
                  << v.description << "\n";
    }
    std::cout << Colors::DIM << "--------------------------------------------------------------------------------" << Colors::RESET << "\n";
}

void Graph::displayAdjacencyList() const {
    std::cout << "\n" << Colors::BOLD << Colors::CYAN << "=== Road Network Adjacency List (Graph Structure) ===" << Colors::RESET << "\n";
    std::cout << Colors::DIM << "--------------------------------------------------------------------------------" << Colors::RESET << "\n";
    for (size_t i = 0; i < vertices.size(); ++i) {
        std::cout << Colors::BOLD_YELLOW << "[" << i << "] " << vertices[i].name << Colors::RESET << "\n";
        if (adj[i].empty()) {
            std::cout << "     " << Colors::RED << "(Isolated vertex - no connections)" << Colors::RESET << "\n";
        } else {
            for (const auto& edge : adj[i]) {
                std::cout << "     └──> " << Colors::GREEN << vertices[edge.to].name << Colors::RESET
                          << "  via \"" << edge.roadName << "\" ("
                          << Utils::formatDistance(edge.distance) << ")\n";
            }
        }
    }
    std::cout << Colors::DIM << "--------------------------------------------------------------------------------" << Colors::RESET << "\n";
}

bool Graph::loadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return false;
    }

    clear();
    std::string line;
    std::string currentSection = "";

    while (std::getline(file, line)) {
        line = Utils::trim(line);
        if (line.empty() || line[0] == '#') continue;

        if (line == "[LOCATIONS]") {
            currentSection = "LOCATIONS";
            continue;
        } else if (line == "[ROADS]") {
            currentSection = "ROADS";
            continue;
        }

        if (currentSection == "LOCATIONS") {
            std::stringstream ss(line);
            std::string name, desc;
            if (std::getline(ss, name, '|')) {
                std::getline(ss, desc, '|');
                addVertex(name, desc);
            }
        } else if (currentSection == "ROADS") {
            std::stringstream ss(line);
            std::string uName, vName, distStr, roadName;
            if (std::getline(ss, uName, '|') &&
                std::getline(ss, vName, '|') &&
                std::getline(ss, distStr, '|')) {
                std::getline(ss, roadName, '|');
                if (roadName.empty()) roadName = "Pathway";
                try {
                    double dist = std::stod(distStr);
                    int u = addVertex(uName);
                    int v = addVertex(vName);
                    addEdge(u, v, dist, roadName, true);
                } catch (...) {
                    // ignore malformed line
                }
            }
        }
    }

    file.close();
    return true;
}

bool Graph::saveToFile(const std::string& filepath) const {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        return false;
    }

    file << "# Map Navigation System - Graph Database\n\n";
    file << "[LOCATIONS]\n";
    for (const auto& v : vertices) {
        file << v.name << "|" << v.description << "\n";
    }

    file << "\n[ROADS]\n";
    // Write edges once per pair (since bidirectional)
    for (size_t u = 0; u < vertices.size(); ++u) {
        for (const auto& edge : adj[u]) {
            if (static_cast<size_t>(edge.to) > u) { // write only u < v to avoid duplicate bidirectional lines
                file << vertices[u].name << "|"
                     << vertices[edge.to].name << "|"
                     << edge.distance << "|"
                     << edge.roadName << "\n";
            }
        }
    }

    file.close();
    return true;
}

void Graph::clear() {
    vertices.clear();
    adj.clear();
    nameToIndex.clear();
    edgeCount = 0;
}
