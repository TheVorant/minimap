# Landscape Route Mapping Project

## Overview

This project visualizes and analyzes landscapes along specified routes by processing geographical data. The program, written in C++, leverages several `.data` files to draw and simulate the terrain for given routes. It also includes tools for working with connections, intersections, and named places to create a cohesive map of interconnected regions. Sample inputs and outputs are provided in the repository for testing and demonstration purposes.

## Features

- **Terrain Visualization**: Draws the landscape for a specified route using provided `.data` files.
- **Geographical Analysis**: Processes connections, intersections, and named places to simulate navigation.
- **Route Navigation**: Implements both shortest path searches (using breadth-first and depth-first strategies) and route simulations based on available data.
- **Custom Route Management**: Supports user-defined road connections and intersections for flexible route planning.
- **Efficient Data Handling**: Uses specialized data structures (e.g., priority queues) for performance optimization.
- **Place Information Retrieval**: Provides detailed information for each place, including its population, area, latitude, longitude, and nearby roads.

## File Structure

### Folders

1. **allp# (formerly "all")**: Contains all `.data` files that represent geographical tiles for the landscape.

### Key Files

- `minimap.cpp`: The core C++ file implementing the landscape drawing and navigation logic. It defines data structures such as `Coordinate`, `Road`, `Place`, `inters` (intersections), and includes priority queue implementations for efficient route computations.
- `coverage.txt`: Lists the geographical coverage of `.data` files, specifying the latitude/longitude ranges and corresponding filenames.
- `named-places.txt`: Provides detailed information about places, including population, area, latitude, and longitude.
- `connections.txt`: Defines connections between geographical points, specifying road names and distances.
- `intersections.txt`: Contains data on intersections, including their coordinates, distances, and associated names.
- **Sample Display Outputs**: Demonstration file showcasing Dij command resulting diplays from `.dat`, files located in `sample.pdf`.

## How It Works

1. **Data Loading**:

   - Reads `.data` files from the `allp#` folder.
   - Loads additional information from `named-places.txt`, `connections.txt`, and `intersections.txt`.

2. **Landscape Rendering**:

   - Uses coordinates and elevation data to draw a visual representation of the landscape along a route.

3. **Route Navigation**:

   - Identifies the closest intersection to a starting place.
   - Lists available roads and their distances.
   - Simulates navigation based on user input.
   - Includes shortest path searches using breadth-first and depth-first strategies.

4. **Place Information**:

   - Retrieves detailed attributes of each place, such as population, geographical area, coordinates, and nearby roads.

## Getting Started

### Prerequisites

- A C++ compiler supporting C++11 or later.
- Basic understanding of data structures (priority queues, vectors).

### Setup

1. Clone the repository:
   ```bash
   git clone https://github.com/TheVorant/minimap.git
   cd minimap
   ```
2. Compile the program:
   ```bash
   g++ -o minimap minimap.cpp
   ```
3. Run the program:
   ```bash
   ./minimap
   ```

### Example Usage
1. Start the program.
2. Enter `S <place name> <state>` to search for a specific place by name and state abbreviation. For example:
   ```
   Enter command: S Miami FL
   Found: Miami in FL
   Code: 31245000 Population: 362470
   Area: 35.673 Latitude: 25.7877
   Longitude: -80.2241 Road Code: 28876
   Distance: 1.2844

   Closest Intersection Details:
   Place Name: Miami, State: FL
   Longitude: -80.2241, Latitude: 25.7877
   Distance to Intersection: 1.2844 units
   ```
3. Use `travel` followed by a place name and state to simulate traveling between places:
   ```
   Enter command: travel Miami FL
   You are at intersection: Miami
   Available roads:
   1. I-395 connects 28876 and 28874 with distance: 0.934
   2. I-95 connects 28876 and 28869 with distance: 1.43
   Enter the road code to travel or 'q'/'Q' to stop: 1
   ```
## Available Commands

- **`N <place name>`**: Search for all places matching the specified name.
- **`S <place name> <state>`**: Search for a specific place by its name and state abbreviation.
- **`travel <place name> <state>`**: Start a travel simulation from a specified place.
- **`short`**: Find the shortest path between two specified locations.
- **`Dij`**: Execute Dijkstra's algorithm for finding the shortest path and diplays route.
- **`Q` or `q`**: Quit the program.

## Contributions

Contributions are welcome! Please follow these steps:

1. Fork the repository.
2. Create a new branch:
   ```bash
   git checkout -b feature-branch
   ```
3. Commit changes:
   ```bash
   git commit -m "Add new feature"
   ```
4. Push to the branch:
   ```bash
   git push origin feature-branch
   ```
5. Create a pull request.

---

Feel free to explore the code, suggest improvements, or create new features! Happy mapping!

