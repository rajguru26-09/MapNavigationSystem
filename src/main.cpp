#include "Graph.h"
#include "NavigationSystem.h"
#include "Utils.h"
#include <iostream>
#include <string>
#include <limits>

void printBanner() {
    std::cout << "\n" << Colors::BOLD_CYAN;
    std::cout << "╔═══════════════════════════════════════════════════════════════════╗\n";
    std::cout << "║            SMART CAMPUS MAP NAVIGATION SYSTEM                     ║\n";
    std::cout << "║       Data Structures & Algorithms (DSA) Mini Project             ║\n";
    std::cout << "║               Second Year CSE - Academic Year 2026                ║\n";
    std::cout << "╚═══════════════════════════════════════════════════════════════════╝\n";
    std::cout << Colors::RESET;
}

void printMenu() {
    std::cout << "\n" << Colors::BOLD << "------------------------ MAIN NAVIGATION MENU -----------------------" << Colors::RESET << "\n";
    std::cout << "  [" << Colors::GREEN << "1" << Colors::RESET << "] View All Campus Locations & Landmarks\n";
    std::cout << "  [" << Colors::GREEN << "2" << Colors::RESET << "] Find Shortest Route " << Colors::BOLD_YELLOW << "(Dijkstra's Algorithm - Min Distance)" << Colors::RESET << "\n";
    std::cout << "  [" << Colors::GREEN << "3" << Colors::RESET << "] Find Fewest Hops Route " << Colors::BOLD_YELLOW << "(BFS Algorithm - Fewest Turns)" << Colors::RESET << "\n";
    std::cout << "  [" << Colors::GREEN << "4" << Colors::RESET << "] Compare Both Routes " << Colors::BOLD_CYAN << "(Dijkstra vs. BFS Algorithmic Analysis)" << Colors::RESET << "\n";
    std::cout << "  [" << Colors::GREEN << "5" << Colors::RESET << "] Explore Reachable Locations " << Colors::DIM << "(Breadth-First Traversal)" << Colors::RESET << "\n";
    std::cout << "  [" << Colors::GREEN << "6" << Colors::RESET << "] Search for a Location or Department\n";
    std::cout << "  [" << Colors::GREEN << "7" << Colors::RESET << "] Add a New Campus Location (Dynamic Vertex Insertion)\n";
    std::cout << "  [" << Colors::GREEN << "8" << Colors::RESET << "] Add a New Road / Pathway (Dynamic Edge Insertion)\n";
    std::cout << "  [" << Colors::GREEN << "9" << Colors::RESET << "] View Full Graph Adjacency List Structure\n";
    std::cout << "  [" << Colors::GREEN << "10" << Colors::RESET << "] Save Current Map to File\n";
    std::cout << "  [" << Colors::GREEN << "11" << Colors::RESET << "] Reload Map from File\n";
    std::cout << "  [" << Colors::RED   << "0" << Colors::RESET << "] Exit Navigation System\n";
    std::cout << Colors::BOLD << "---------------------------------------------------------------------" << Colors::RESET << "\n";
    std::cout << Colors::BOLD_CYAN << "Enter your choice [0-11]: " << Colors::RESET;
}

// Helper to resolve user input to a valid vertex ID (by index or by name)
int promptForLocation(const Graph& graph, const std::string& promptText) {
    while (true) {
        std::cout << promptText;
        std::string input;
        if (!std::getline(std::cin, input)) return -1;
        input = Utils::trim(input);
        if (input.empty()) continue;

        // Check if numeric ID
        bool isNumber = true;
        for (char c : input) {
            if (!std::isdigit(c)) {
                isNumber = false;
                break;
            }
        }

        if (isNumber) {
            int id = std::stoi(input);
            if (id >= 0 && id < graph.getVertexCount()) {
                return id;
            }
            std::cout << Colors::RED << " [!] Error: Location ID " << id << " is out of range (0 to "
                      << graph.getVertexCount() - 1 << "). Try again.\n" << Colors::RESET;
            continue;
        }

        // Try lookup by name
        int id = graph.getVertexIndex(input);
        if (id != -1) {
            return id;
        }

        std::cout << Colors::RED << " [!] Location \"" << input << "\" not recognized. Type ID or full name.\n" << Colors::RESET;
        std::cout << Colors::YELLOW << " Tip: Type 1 in main menu to list all valid locations.\n" << Colors::RESET;
    }
}

void loadDefaultMapFallback(Graph& graph) {
    // If data/campus_map.txt not found, initialize hardcoded default graph
    int mg = graph.addVertex("Main Gate", "Campus primary entrance and security gate");
    int ab = graph.addVertex("Admin Block", "Central administrative offices and admissions");
    int cl = graph.addVertex("Central Library", "Research library and digital reference halls");
    int cs = graph.addVertex("Computer Science Dept", "Turing labs, AI lab, and servers");
    int me = graph.addVertex("Mechanical Dept", "Workshops and CAD/CAM modeling");
    int ee = graph.addVertex("Electrical Dept", "Microcontrollers and circuits lab");
    int sc = graph.addVertex("Student Center & Canteen", "Cafeteria, food stalls, and club room");
    int au = graph.addVertex("Auditorium", "Amphitheater for seminars and college fests");
    int sp = graph.addVertex("Sports Complex", "Athletics stadium, football field, gym");
    int bh = graph.addVertex("Boys Hostel", "Hostel blocks A & B");
    int gh = graph.addVertex("Girls Hostel", "Hostel blocks C & D");
    int hc = graph.addVertex("Health Center", "24/7 campus health clinic and dispensary");
    int th = graph.addVertex("Tech Incubation Hub", "Startup incubator and makerspace");

    graph.addEdge(mg, ab, 250, "Central Boulevard");
    graph.addEdge(mg, hc, 180, "West Perimeter Road");
    graph.addEdge(ab, cl, 200, "University Avenue");
    graph.addEdge(ab, au, 320, "Ceremony Way");
    graph.addEdge(ab, hc, 210, "Medical Drive");
    graph.addEdge(cl, cs, 150, "Tech Corridor");
    graph.addEdge(cl, sc, 220, "Scholar Walkway");
    graph.addEdge(cl, hc, 290, "Wellness Path");
    graph.addEdge(cs, ee, 180, "Innovation Way");
    graph.addEdge(cs, th, 130, "Silicon Alley");
    graph.addEdge(me, ee, 160, "Engineering Lane");
    graph.addEdge(me, sp, 350, "South Ring Road");
    graph.addEdge(ee, ab, 310, "Faculty Promenade");
    graph.addEdge(sc, th, 210, "Startup Street");
    graph.addEdge(sc, bh, 300, "North Pathway");
    graph.addEdge(sc, gh, 280, "East Pathway");
    graph.addEdge(au, sc, 260, "Festival Mall");
    graph.addEdge(sp, bh, 250, "Stadium Road");
    graph.addEdge(bh, gh, 400, "Hostel Link Road");
}

int main() {
    printBanner();

    Graph graph;
    std::string defaultDataFile = "data/campus_map.txt";

    // Attempt to load default dataset
    if (graph.loadFromFile(defaultDataFile)) {
        std::cout << Colors::GREEN << "[✓] Campus map successfully loaded from \""
                  << defaultDataFile << "\" (" << graph.getVertexCount() << " locations, "
                  << graph.getEdgeCount() << " road segments)." << Colors::RESET << "\n";
    } else {
        std::cout << Colors::YELLOW << "[!] Could not open \"" << defaultDataFile
                  << "\". Initializing default hardcoded campus network..." << Colors::RESET << "\n";
        loadDefaultMapFallback(graph);
        std::cout << Colors::GREEN << "[✓] Initialized " << graph.getVertexCount()
                  << " locations and " << graph.getEdgeCount() << " road connections." << Colors::RESET << "\n";
    }

    NavigationSystem nav(graph);

    bool running = true;
    while (running) {
        printMenu();
        std::string choiceStr;
        if (!std::getline(std::cin, choiceStr)) break;
        choiceStr = Utils::trim(choiceStr);

        if (choiceStr == "1") {
            graph.displayLocations();
        } else if (choiceStr == "2") {
            std::cout << "\n" << Colors::BOLD << "--- Find Shortest Route (Dijkstra's Algorithm) ---" << Colors::RESET << "\n";
            int src = promptForLocation(graph, "Enter Starting Location (ID or Name): ");
            int dest = promptForLocation(graph, "Enter Destination Location (ID or Name): ");

            PathResult res = nav.findShortestRoute(src, dest);
            nav.printRouteSummary(res, "Dijkstra's Shortest Path Algorithm");
            if (res.found) {
                nav.printAsciiRouteMap(res);
                nav.printTurnByTurnDirections(res);
            }
        } else if (choiceStr == "3") {
            std::cout << "\n" << Colors::BOLD << "--- Find Fewest Hops Route (BFS Algorithm) ---" << Colors::RESET << "\n";
            int src = promptForLocation(graph, "Enter Starting Location (ID or Name): ");
            int dest = promptForLocation(graph, "Enter Destination Location (ID or Name): ");

            PathResult res = nav.findFewestHopsRoute(src, dest);
            nav.printRouteSummary(res, "Breadth-First Search (Fewest Intersections)");
            if (res.found) {
                nav.printAsciiRouteMap(res);
                nav.printTurnByTurnDirections(res);
            }
        } else if (choiceStr == "4") {
            std::cout << "\n" << Colors::BOLD_CYAN << "--- Algorithmic Comparison: Dijkstra (Weighted) vs. BFS (Unweighted) ---" << Colors::RESET << "\n";
            int src = promptForLocation(graph, "Enter Starting Location (ID or Name): ");
            int dest = promptForLocation(graph, "Enter Destination Location (ID or Name): ");

            PathResult dRes = nav.findShortestRoute(src, dest);
            PathResult bRes = nav.findFewestHopsRoute(src, dest);

            if (!dRes.found || !bRes.found) {
                std::cout << Colors::RED << "[!] No connected route exists between these locations.\n" << Colors::RESET;
            } else {
                std::cout << "\n" << Colors::BOLD << "=========================== COMPARISON MATRIX ===========================" << Colors::RESET << "\n";
                std::cout << std::left << std::setw(28) << "Metric"
                          << std::setw(24) << "Dijkstra's (Min Distance)"
                          << std::setw(24) << "BFS (Fewest Hops)" << "\n";
                std::cout << Colors::DIM << "------------------------------------------------------------------------" << Colors::RESET << "\n";

                std::cout << std::left << std::setw(28) << "Total Distance"
                          << Colors::BOLD_GREEN << std::setw(24) << Utils::formatDistance(dRes.totalDistance) << Colors::RESET
                          << std::setw(24) << Utils::formatDistance(bRes.totalDistance) << "\n";

                std::cout << std::left << std::setw(28) << "Road Segments (Hops)"
                          << std::setw(24) << dRes.totalHops
                          << Colors::BOLD_YELLOW << std::setw(24) << bRes.totalHops << Colors::RESET << "\n";

                std::cout << std::left << std::setw(28) << "Walking Time (~5 km/h)"
                          << Colors::CYAN << std::setw(24) << Utils::formatTime(dRes.walkingTimeMin) << Colors::RESET
                          << std::setw(24) << Utils::formatTime(bRes.walkingTimeMin) << "\n";

                std::cout << std::left << std::setw(28) << "Campus Shuttle (~30 km/h)"
                          << Colors::CYAN << std::setw(24) << Utils::formatTime(dRes.drivingTimeMin) << Colors::RESET
                          << std::setw(24) << Utils::formatTime(bRes.drivingTimeMin) << "\n";

                std::cout << std::left << std::setw(28) << "Time Complexity"
                          << std::setw(24) << "O((V + E) log V)"
                          << std::setw(24) << "O(V + E)" << "\n";

                std::cout << Colors::DIM << "------------------------------------------------------------------------" << Colors::RESET << "\n";
                if (dRes.totalDistance < bRes.totalDistance) {
                    double saved = bRes.totalDistance - dRes.totalDistance;
                    std::cout << Colors::BOLD_GREEN << " [*] Dijkstra saves " << Utils::formatDistance(saved)
                              << " (" << std::fixed << std::setprecision(1) << (saved / bRes.totalDistance * 100.0)
                              << "%) distance compared to BFS route." << Colors::RESET << "\n";
                } else if (dRes.totalHops > bRes.totalHops) {
                    std::cout << Colors::BOLD_YELLOW << " [*] BFS route has " << (dRes.totalHops - bRes.totalHops)
                              << " fewer turn(s), but distance is identical or longer." << Colors::RESET << "\n";
                } else {
                    std::cout << Colors::BOLD << " [*] Both algorithms produced an identical route for this pair.\n" << Colors::RESET;
                }
            }
        } else if (choiceStr == "5") {
            std::cout << "\n" << Colors::BOLD << "--- Explore All Reachable Locations (Graph Traversal) ---" << Colors::RESET << "\n";
            int src = promptForLocation(graph, "Enter Starting Location (ID or Name): ");
            std::vector<int> reachable = nav.findReachableLocations(src);

            std::cout << "\n" << Colors::BOLD_GREEN << "Reachable locations from [" << graph.getVertexName(src)
                      << "] (" << reachable.size() << " total):" << Colors::RESET << "\n";
            for (size_t i = 0; i < reachable.size(); ++i) {
                int id = reachable[i];
                std::cout << "  " << (i + 1) << ". [" << id << "] " << graph.getVertexName(id) << "\n";
            }
        } else if (choiceStr == "6") {
            std::cout << "\nEnter keyword to search (e.g., 'hostel', 'lab', 'sports'): ";
            std::string q;
            if (std::getline(std::cin, q)) {
                std::vector<int> matches = nav.searchLocations(q);
                if (matches.empty()) {
                    std::cout << Colors::YELLOW << "No locations found matching \"" << q << "\".\n" << Colors::RESET;
                } else {
                    std::cout << Colors::GREEN << "\nFound " << matches.size() << " matching location(s):\n" << Colors::RESET;
                    for (int id : matches) {
                        const auto& v = graph.getVertex(id);
                        std::cout << "  • [" << v.id << "] " << Colors::BOLD << v.name << Colors::RESET
                                  << " - " << v.description << "\n";
                    }
                }
            }
        } else if (choiceStr == "7") {
            std::cout << "\n" << Colors::BOLD << "--- Add a New Campus Location ---" << Colors::RESET << "\n";
            std::cout << "Enter Location Name: ";
            std::string name;
            std::getline(std::cin, name);
            name = Utils::trim(name);

            if (name.empty()) {
                std::cout << Colors::RED << "[!] Location name cannot be empty.\n" << Colors::RESET;
            } else if (graph.hasVertex(name)) {
                std::cout << Colors::YELLOW << "[!] Location \"" << name << "\" already exists.\n" << Colors::RESET;
            } else {
                std::cout << "Enter Location Description: ";
                std::string desc;
                std::getline(std::cin, desc);
                int newId = graph.addVertex(name, desc);
                std::cout << Colors::BOLD_GREEN << "[✓] Successfully added \"" << name
                          << "\" with ID [" << newId << "].\n" << Colors::RESET;
            }
        } else if (choiceStr == "8") {
            std::cout << "\n" << Colors::BOLD << "--- Add a New Road / Pathway Connection ---" << Colors::RESET << "\n";
            int u = promptForLocation(graph, "Enter First Location (ID or Name): ");
            int v = promptForLocation(graph, "Enter Second Location (ID or Name): ");

            if (u == v) {
                std::cout << Colors::RED << "[!] Self-loops not permitted in this navigation network.\n" << Colors::RESET;
            } else {
                std::cout << "Enter Road/Pathway Name (e.g., 'Alumni Walk'): ";
                std::string rName;
                std::getline(std::cin, rName);
                rName = Utils::trim(rName);
                if (rName.empty()) rName = "Pathway";

                std::cout << "Enter Distance in meters (e.g., 220): ";
                std::string distStr;
                std::getline(std::cin, distStr);
                try {
                    double dist = std::stod(distStr);
                    if (dist <= 0) {
                        std::cout << Colors::RED << "[!] Distance must be a positive number.\n" << Colors::RESET;
                    } else {
                        graph.addEdge(u, v, dist, rName, true);
                        std::cout << Colors::BOLD_GREEN << "[✓] Road \"" << rName << "\" ("
                                  << Utils::formatDistance(dist) << ") between ["
                                  << graph.getVertexName(u) << "] and ["
                                  << graph.getVertexName(v) << "] successfully added.\n" << Colors::RESET;
                    }
                } catch (...) {
                    std::cout << Colors::RED << "[!] Invalid distance entered.\n" << Colors::RESET;
                }
            }
        } else if (choiceStr == "9") {
            graph.displayAdjacencyList();
        } else if (choiceStr == "10") {
            std::cout << "\nEnter file path to save (press Enter for default \"" << defaultDataFile << "\"): ";
            std::string savePath;
            std::getline(std::cin, savePath);
            savePath = Utils::trim(savePath);
            if (savePath.empty()) savePath = defaultDataFile;

            if (graph.saveToFile(savePath)) {
                std::cout << Colors::BOLD_GREEN << "[✓] Map data successfully saved to \"" << savePath << "\".\n" << Colors::RESET;
            } else {
                std::cout << Colors::RED << "[!] Failed to write to file \"" << savePath << "\".\n" << Colors::RESET;
            }
        } else if (choiceStr == "11") {
            std::cout << "\nEnter file path to reload (press Enter for default \"" << defaultDataFile << "\"): ";
            std::string loadPath;
            std::getline(std::cin, loadPath);
            loadPath = Utils::trim(loadPath);
            if (loadPath.empty()) loadPath = defaultDataFile;

            if (graph.loadFromFile(loadPath)) {
                std::cout << Colors::BOLD_GREEN << "[✓] Map successfully reloaded from \"" << loadPath << "\".\n" << Colors::RESET;
            } else {
                std::cout << Colors::RED << "[!] Failed to open file \"" << loadPath << "\".\n" << Colors::RESET;
            }
        } else if (choiceStr == "0") {
            std::cout << Colors::BOLD_CYAN << "\nThank you for using the Smart Campus Map Navigation System. Goodbye!\n\n" << Colors::RESET;
            running = false;
        } else {
            std::cout << Colors::RED << "[!] Invalid option \"" << choiceStr << "\". Please enter a number between 0 and 11.\n" << Colors::RESET;
        }
    }

    return 0;
}
