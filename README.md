# 🗺️ Smart Campus Map Navigation System

[![C++17](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://en.wikipedia.org/wiki/C%2B%2B17)
[![Build Status](https://img.shields.io/badge/Build-Passing-brightgreen.svg)]()
[![Course](https://img.shields.io/badge/Course-Data%20Structures%20%26%20Algorithms-orange.svg)]()
[![Level](https://img.shields.io/badge/Academic%20Level-Second%20Year%20CSE-purple.svg)]()
[![License](https://img.shields.io/badge/License-MIT-green.svg)]()

A high-performance, lightweight, and modular **Campus Map Navigation System** built in **C++17** for Second Year Computer Science & Engineering (DSA Mini Project). The system models geographical landmarks and roads using an **Adjacency List Graph**, implementing both **Dijkstra's Shortest Path Algorithm** (for minimum physical distance) and **Breadth-First Search (BFS)** (for minimum intersections/hops).

---

## 📌 Features

- **Optimal Shortest Route (Dijkstra's Algorithm)**:
  - Uses a **Binary Min-Heap (`std::priority_queue`)** to achieve $O((V + E) \log V)$ time complexity.
  - Computes exact distance in meters/kilometers.
- **Fewest Turns Route (Breadth-First Search - BFS)**:
  - Discovers paths with the minimum number of intersections/hops in $O(V + E)$ time.
- **Algorithmic Comparison Matrix**:
  - Live side-by-side benchmark of **Dijkstra vs. BFS** showing distance saved vs. hop count tradeoffs.
- **Multi-Modal Travel Time Estimation**:
  - Automatically calculates realistic travel duration for **Walking (5 km/h)**, **Cycling (15 km/h)**, and **Campus Shuttle / Driving (30 km/h)**.
- **Turn-by-Turn Directions & ASCII Trail**:
  - Clear, readable navigation instructions with segment distances and cumulative totals.
- **Dynamic Graph Mutability**:
  - Add new campus buildings/landmarks and new connecting pathways at runtime without restarting.
- **Fast Landmark Search & Traversal**:
  - Case-insensitive substring search for buildings and BFS reachability exploration.
- **Data Persistence**:
  - Loads and saves map datasets from structured text files (`data/campus_map.txt`).
- **Zero External GUI/Library Dependencies**:
  - 100% self-contained standard C++17 with colored ANSI terminal output.

---

## 📂 Project Architecture

```
MapNavigationSystem/
├── include/
│   ├── Graph.h             # Graph ADT (Adjacency List, Vertex, Edge structures)
│   ├── NavigationSystem.h  # Dijkstra, BFS, and navigation engine declarations
│   └── Utils.h             # ANSI styling, string parsers, distance/time formatters
├── src/
│   ├── Graph.cpp           # Adjacency list logic, dynamic updates & file I/O
│   ├── NavigationSystem.cpp# Dijkstra, BFS, and direction formatting implementations
│   └── main.cpp            # Interactive menu-driven CLI application
├── data/
│   └── campus_map.txt      # Default campus network dataset (13 locations, 19 roads)
├── Makefile                # Automated GNU makefile
├── CMakeLists.txt          # Modern CMake configuration
├── run.sh                  # One-click compile & execute script
└── README.md               # Project documentation
```

---

## 🧠 Data Structures & Algorithms Used

| Component / Task | Data Structure / Algorithm | Time Complexity | Space Complexity |
| :--- | :--- | :--- | :--- |
| **Graph Representation** | Adjacency List (`std::vector<std::vector<Edge>>`) | $O(1)$ edge insert | $O(V + E)$ |
| **Location Lookup** | Hash Table (`std::unordered_map<std::string, int>`) | $O(1)$ average | $O(V)$ |
| **Shortest Route** | Dijkstra's Algorithm with Min-Priority Queue | $O((V + E) \log V)$ | $O(V)$ |
| **Fewest Turns Route**| Breadth-First Search (Queue) | $O(V + E)$ | $O(V)$ |
| **Reachability Check** | Breadth-First Traversal | $O(V + E)$ | $O(V)$ |
| **Landmark Search** | Substring Matching | $O(V \cdot L)$ | $O(K)$ |

---

## ⚡ Quick Start & Compilation

### Option 1: Using the Bash Script (Fastest)
```bash
git clone https://github.com/rajguru26-09/MapNavigationSystem.git
cd MapNavigationSystem
./run.sh
```

### Option 2: Using GNU Make
```bash
make
./bin/map_navigator
```

### Option 3: Using Direct `g++` Command
```bash
g++ -std=c++17 -Wall -Wextra -O2 -Iinclude src/main.cpp src/Graph.cpp src/NavigationSystem.cpp -o map_navigator
./map_navigator
```

### Option 4: Using CMake
```bash
cmake -B build
cmake --build build
./build/map_navigator
```

---

## 🖥️ Sample Run & Output

### 1. Shortest Route Calculation (Dijkstra)
```
=================================================================
  ROUTE FOUND (Dijkstra's Shortest Path Algorithm)
=================================================================
 • Origin:         Main Gate
 • Destination:    Sports Complex
 • Total Distance: 1.07 km
 • Road Segments:  4 segments (5 landmarks)
-----------------------------------------------------------------
 Estimated Travel Times:
   🚶 Walking  (~5 km/h):   12 min 50 sec
   🚲 Cycling  (~15 km/h):  4 min 16 sec
   🚗 Shuttle  (~30 km/h):  2 min 8 sec
=================================================================

>>> Route Map Trail <<<
[Main Gate]
    │
    ▼  (Central Boulevard, 250 m)
[Admin Block]
    │
    ▼  (Faculty Promenade, 310 m)
[Electrical Dept]
    │
    ▼  (Engineering Lane, 160 m)
[Mechanical Dept]
    │
    ▼  (South Ring Road, 350 m)
[Sports Complex]

>>> Step-by-Step Navigation Directions <<<
-----------------------------------------------------------------
 [START] Depart from Main Gate
   Step 1: Follow "Central Boulevard" for 250 m ➔ Reach Admin Block (Cumulative: 250 m)
   Step 2: Follow "Faculty Promenade" for 310 m ➔ Reach Electrical Dept (Cumulative: 560 m)
   Step 3: Follow "Engineering Lane" for 160 m ➔ Reach Mechanical Dept (Cumulative: 720 m)
   Step 4: Follow "South Ring Road" for 350 m ➔ Reach Sports Complex (Cumulative: 1.07 km)
 [ARRIVE] You have reached your destination: Sports Complex!
-----------------------------------------------------------------
```

### 2. Algorithmic Comparison (Dijkstra vs. BFS)
```
=========================== COMPARISON MATRIX ===========================
Metric                      Dijkstra's (Min Distance)   BFS (Fewest Hops)
------------------------------------------------------------------------
Total Distance              1.07 km                     1.38 km
Road Segments (Hops)        4                           4
Walking Time (~5 km/h)      12 min 50 sec               16 min 33 sec
Campus Shuttle (~30 km/h)   2 min 8 sec                 2 min 45 sec
Time Complexity             O((V + E) log V)            O(V + E)
------------------------------------------------------------------------
 [*] Dijkstra saves 310 m (22.5%) distance compared to BFS route.
```

---

## 👥 Author

- **Student**: Second Year B.Tech Computer Science & Engineering
- **GitHub**: [@rajguru26-09](https://github.com/rajguru26-09)
- **Course**: Data Structures & Algorithms (DSA) Practical

---

## 📜 License

This project is licensed under the MIT License - open for educational and academic use.
