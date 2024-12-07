#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdlib> 
#include <vector>
#include "library.h"

using namespace std;

struct Coordinate
{
    double latitude;
    double longitude;

	Coordinate(double lat = 0.0, double lon = 0.0)
    {
        latitude = lat;
        longitude = lon;
    }
};

struct Road
{
    string rN;
    string rT;
    int inters1;
    int inters2;
    int dist;

    Road(string rn, string rt, int i1, int i2, double distr)
    {
        rN = rn;
        rT = rt;
        inters1 = i1;
        inters2 = i2;
        dist = (int)(distr * 10000);
    }
};

struct inters
{
    double longitude, latitude, dist_c;
    string state, name;
    vector<Road*> roads;
    int dTotal;
    inters* prevN;
    bool inQ;
    int index;

    inters(string s, string n, double lat, double lon, double d)
    {
        state = s;
        name = n;
        latitude = lat;
        longitude = lon;
        dist_c = d;
        dTotal = 99999999;
        prevN = NULL;
        inQ = false;
    }

    void addRoad(Road*& road)
    {
        roads.push_back(road);
    }

    void print()
    {
        cout << "Closest Intersection Details:" << "\n";
        cout << "Place Name: " << name << ", State: " << state << "\n";
        cout << "Longitude: " << longitude << ", Latitude: " << latitude << "\n";
        cout << "Distance to Intersection: " << dist_c << " units" << "\n\n";
    }

    void printroad()
    {
        for (size_t i = 0; i < roads.size(); ++i)
        {
            Road* road = roads[i];
            cout << i + 1 << ". " << road->rN << " connects "
                 << road->inters1 << " and " << road->inters2
                 << " with distance: " << road->dist << "\n";
        }
    }
};

struct Place
{
    int population, code, roadCode;
    string state, name;
    double area, latitude, longitude, distance;

    Place(int c, string s, string n, int p, double a, double lat, double lon, int r, double d)
    {
        code = c;
        state = s;
        name = n;
        population = p;
        area = a;
        latitude = lat;
        longitude = lon;
        roadCode = r;
        distance = d;
    }

    void print()
    {
        cout << "Found: " << name << " in " << state << "\n";
        cout << "Code: " << code << " Population: " << population << "\n";
        cout << "Area: " << area << " Latitude: " << latitude << "\n";
        cout << "Longitude: " << longitude << " Road Code: " << roadCode << "\n";
        cout << "Distance: " << distance << "\n\n";
    }
};

struct Tile
{
    double lat_min, lat_max;
    double lon_min, lon_max;
    string file_name;
};

vector<Tile> tiles;

struct Entry
{
    Place* place;
    Entry* next;

    Entry(Place* p, Entry* n = NULL)
    {
        place = p;
        next = n;
    }
};

class PriorityQueue
{
protected:
    vector<inters*> queue;

public:
    PriorityQueue() {}

    void enqueue(inters* node)
    {
        queue.push_back(node);
        int i = queue.size() - 1;
        while (i > 0 && queue[i]->dTotal < queue[(i - 1) / 2]->dTotal)
        {
            swap(queue[i], queue[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    inters* dequeue()
    {
        if (isEmpty())
        {
            return NULL;
        }
        inters* topNode = queue[0];
        queue[0] = queue.back();
        queue.pop_back();

        int i = 0, size = queue.size();
        while (i < size)
        {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;

            if (left < size && queue[left]->dTotal < queue[smallest]->dTotal)
            {
                smallest = left;
            }
            if (right < size && queue[right]->dTotal < queue[smallest]->dTotal)
            {
                smallest = right;
            }

            if (smallest != i)
            {
                swap(queue[i], queue[smallest]);
                i = smallest;
            }
            else
            {
                break;
            }
        }
        return topNode;
    }

    bool isEmpty()
    {
        return queue.empty();
    }

    int getSize()
    {
        return queue.size();
    }
};

class DQ
{
protected:
    vector<inters*> queue;
    int front;

public:
    DQ()
    {
        front = 0;
    }

    void enqueue(inters* node)
    {
        queue.push_back(node);
    }

    inters* dequeue()
    {
        if (isEmpty())
        {
            return NULL;
        }

        inters* frontNode = queue[front];
        front++;

        if (front > queue.size() / 2)
        {
            queue.erase(queue.begin(), queue.begin() + front);
            front = 0;
        }
        return frontNode;
    }

    bool isEmpty()
    {
        return front >= queue.size();
    }

    int getSize()
    {
        return queue.size() - front;
    }
};

class Travelmanager
{
protected:
    vector<inters*>& intersects;

public:
    Travelmanager(vector<inters*>& inters) : intersects(inters)
    {
    }

    inters* closestI(Place* start)
    {
        int rc = start->roadCode;
        return intersects[rc];
    }

    void listavi(inters* closeI)
    {
        cout << "You are at intersection: " << closeI->name << "\n";
        cout << "Available roads:\n";

        for (size_t i = 0; i < closeI->roads.size(); ++i)
        {
            Road* road = closeI->roads[i];
            cout << i + 1 << ". " << road->rN << " connects "
                 << road->inters1 << " and " << road->inters2
                 << " with distance: " << road->dist << "\n";
        }
    }

    void travelm(Place* start)
    {
        inters* closeI = closestI(start);
        while (true)
        {
            listavi(closeI);
            string input;
            cout << "Enter the road code to travel or 'q'/'Q' to stop: ";
            cin >> input;

            if (input == "Q" || input == "q")
            {
                cout << "Ending travel...\n";
                return;
            }

            int rC = stoi(input) - 1;

            if (rC >= 0 && rC < closeI->roads.size())
            {
                Road* sR = closeI->roads[rC];

                if (closeI == intersects[sR->inters1])
                {
                    closeI = intersects[sR->inters2];
                }
                else
                {
                    closeI = intersects[sR->inters1];
                }
            }
            else
            {
                cout << "Invalid choice. Try again.\n";
            }
        }
    }

    void reset()
    {
        for (int i = 0; i < intersects.size(); i++)
        {
            inters* intersection = intersects[i];
            intersection->dTotal = 99999999;
            intersection->prevN = NULL;
            intersection->inQ = false;
        }
    }

    Tile findSmallestTile(double start_lat, double start_lon, double dest_lat, double dest_lon)
    {
        Tile smallestTile;
        bool found = false;
        double smallestDifference = -1; 

        for (int i = 0; i < tiles.size(); ++i)
        {
            const Tile& tile = tiles[i];


            if (start_lat <= tile.lat_max && start_lat >= tile.lat_min &&
                start_lon >= tile.lon_min && start_lon <= tile.lon_max &&
                dest_lat <= tile.lat_max && dest_lat >= tile.lat_min &&
                dest_lon >= tile.lon_min && dest_lon <= tile.lon_max)
            {
                double latDiff = tile.lat_max - tile.lat_min;
                double lonDiff = tile.lon_max - tile.lon_min;
                double tileDifference = latDiff + lonDiff;

               
                if (!found || tileDifference < smallestDifference)
                {
                    smallestDifference = tileDifference;
                    smallestTile = tile;
                    found = true;
                }
            }
        }

        if (!found)
        {
            throw runtime_error("No suitable map tile found");
        }

        return smallestTile;
    }

    void drawMap(const string& file_name, const vector<Coordinate>& route, const Tile& tile)
    {
        ifstream file(file_name, ios::binary);
        if (!file)
        {
            cerr << "Unable to open file " << file_name << endl;
            exit(1);
        }

        string dummy_row;
        getline(file, dummy_row);
        int rows = 0, cols = 0;
        istringstream iss(dummy_row);
        string temp;

        while (iss >> temp)
        {
            if (temp == "rows")
            {
                iss >> rows;
            }
            else if (temp == "columns")
            {
                iss >> cols;
            }
        }

        file.seekg(2 * cols, ios::beg);

        make_window(cols, rows);

        signed short int* elevation_row = new signed short int[cols];

        for (int i = 0; i < rows; ++i)
        {
            file.read((char*)(elevation_row), cols * sizeof(short));
            if (file.fail())
            {
                cerr << "Error reading elevation data from row " << i << endl;
                exit(1);
            }

 
            for (int j = 0; j < cols; ++j)
            {

                short elevation = elevation_row[j];
                int pixel_color;
                if (elevation < 0)
                {
                    pixel_color = RGB(0, 0, 255);
                }
                else
                {
 
                    int green_intensity = min(255, 150 + elevation / 10); 
                    int red_intensity = max(0, elevation / 20);           
                    pixel_color = RGB(red_intensity, green_intensity, 0);
                }

                set_pixel_color(j, i, pixel_color);
            }
        }

        set_pen_color(RGB(255, 0, 0)); 
        set_pen_width(2); 


        if (!route.empty())
        {
            double lat_range = tile.lat_max - tile.lat_min;
            double lon_range = tile.lon_max - tile.lon_min;

            double x_start = (route[0].longitude - tile.lon_min) / lon_range * cols;
            double y_start = (tile.lat_max - route[0].latitude) / lat_range * rows;
            move_to(x_start, y_start);

            for (size_t i = 1; i < route.size() - 1; ++i)
            {
                double x = (route[i].longitude - tile.lon_min) / lon_range * cols;
                double y = (tile.lat_max - route[i].latitude) / lat_range * rows;
                draw_to(x, y);
            }
        }

        wait_for_mouse_click();
    }

    void dij(Place* start, Place* end)
    {
        PriorityQueue pq;
        inters* startI = closestI(start);
        inters* endI = closestI(end);
        startI->dTotal = 0;
        pq.enqueue(startI);

        while (!pq.isEmpty())
        {
            inters* current = pq.dequeue();
            if (current == endI)
            {
                break;
            }

            for (int i = 0; i < current->roads.size(); i++)
            {
                Road* road = current->roads[i];
                inters* neighbor;
                if (current == intersects[road->inters1])
                {
                    neighbor = intersects[road->inters2];
                }
                else
                {
                    neighbor = intersects[road->inters1];
                }

                if (neighbor->dTotal > current->dTotal + road->dist)
                {
                    neighbor->dTotal = current->dTotal + road->dist;
                    neighbor->prevN = current;
                    if (!neighbor->inQ)
                    {
                        neighbor->inQ = true;
                        pq.enqueue(neighbor);
                    }
                }
            }
        }

        vector<Coordinate> route;
        PrintPath(endI, route);

        double start_lat = start->latitude;
        double start_lon = start->longitude;
        double dest_lat = end->latitude;
        double dest_lon = end->longitude;

        Tile tile = findSmallestTile(start_lat, start_lon, dest_lat, dest_lon);
        drawMap("C:\\Users\\johna\\Documents\\all\\" + tile.file_name, route, tile);
    }

    void shortP(Place*& startP, Place*& endP)
    {
        inters* startI = closestI(startP);
        inters* endI = closestI(endP);

        startI->dTotal = 0;
        startI->inQ = true;

        DQ queue;
        queue.enqueue(startI);

        bool dR = false;

        while (!queue.isEmpty() || !dR)
        {
            inters* current = queue.dequeue();
            current->inQ = false;

            if (current == endI)
            {
                dR = true;
            }

            for (int i = 0; i < current->roads.size(); i++)
            {
                Road* road = current->roads[i];
                inters* neighbor;

                if (current == intersects[road->inters1])
                {
                    neighbor = intersects[road->inters2];
                }
                else
                {
                    neighbor = intersects[road->inters1];
                }

                double newDist = current->dTotal + road->dist;

                if (newDist < neighbor->dTotal)
                {
                    neighbor->dTotal = newDist;
                    neighbor->prevN = current;

                    if (!neighbor->inQ)
                    {
                        queue.enqueue(neighbor);
                        neighbor->inQ = true;
                    }
                }
            }
        }

        if (dR)
        {
            cout << "Reached the destination!\n";
            reconPath(endI);
        }
        else
        {
            cout << "No path found between " << startP->name << " and " << endP->name << ".\n";
        }
    }

    void reconPath(inters*& endI)
    {
        vector<inters*> path;
        for (inters* current = endI; current != NULL; current = current->prevN)
        {
            path.push_back(current);
        }
        int n = 0;
        for (int i = path.size() - 1; i >= 0; i--)
        {
            cout << path[i]->name;
            if (i > 0)
            {
                inters* next = path[i - 1];
                Road* road = NULL;

                for (int j = 0; j < path[i]->roads.size(); j++)
                {
                    Road* tempRoad = path[i]->roads[j];
                    if (tempRoad->dist + path[i]->dTotal == next->dTotal)
                    {
                        road = tempRoad;
                        break;
                    }
                }

                if (road != NULL)
                {
                    if (n == 3)
                    {
                        cout << "\n";
                        n = 0;
                    }
                    cout << " -- " << road->dist + path[i]->dTotal << "  total miles -> ";
                    n++;
                }
            }
        }
        cout << "\n" << endI->dTotal << "\n\n";
        cout << "\n";
    }

    void PrintPath(inters*& endI, vector<Coordinate>& route)
    {
        vector<inters*> path;
        for (inters* current = endI; current != NULL; current = current->prevN)
        {
            path.push_back(current);
        }

        inters* startI = path.back();
        cout << "Starting point is intersection #" << startI->index
             << ", " << startI->dist_c << " miles from "
             << startI->name << ", " << startI->state << "\n";
        cout << "   where ";

        for (int i = 0; i < startI->roads.size(); i++)
        {
            cout << startI->roads[i]->rN;
            if (i < startI->roads.size() - 1)
            {
                cout << " and ";
            }
        }
        cout << " meet\n";

        double cumulativeDistance = 0;
        string lastRoad = "";
        for (int i = path.size() - 1; i > 0; i--)
        {
            inters* from = path[i];
            inters* to = path[i - 1];
            Road* road = NULL;
            for (int j = 0; j < from->roads.size(); j++)
            {
                Road* tempRoad = from->roads[j];
                int neighborIdx;

                if (tempRoad->inters1 == from->index)
                {
                    neighborIdx = tempRoad->inters2;
                }
                else
                {
                    neighborIdx = tempRoad->inters1;
                }

                if (to->index == neighborIdx)
                {
                    road = tempRoad;
                    break;
                }
            }

            if (road)
            {
                if (lastRoad == road->rN)
                {
                    cumulativeDistance += (double)(road->dist) / 10000;
                }
                else
                {
                    if (!lastRoad.empty())
                    {
                        cout << "take " << lastRoad << " for " << cumulativeDistance
                             << " miles to intersection #" << from->index << ", "
                             << from->dist_c << " miles from "
                             << from->name << ", " << from->state << "\n";
                    }
                    lastRoad = road->rN;
                    cumulativeDistance = (double)(road->dist) / 10000;
                }
            }

            Coordinate coord(from->longitude, from->latitude);
            route.push_back(coord);
        }

        if (!lastRoad.empty())
        {
            cout << "then " << lastRoad << " for " << cumulativeDistance
                 << " miles to intersection #" << endI->index << ", "
                 << endI->dist_c << " miles from "
                 << endI->name << ", " << endI->state << "\n";
        }

        cout << "Destination is intersection #" << endI->index << ", "
             << endI->dist_c << " miles from "
             << endI->name << ", " << endI->state << "\n";
        cout << "   where ";

        for (int i = 0; i < endI->roads.size(); i++)
        {
            cout << endI->roads[i]->rN;
            if (i < endI->roads.size() - 1)
            {
                cout << " and ";
            }
        }
        cout << " meet\n";

        cout << "Total " << ((double)endI->dTotal / 10000) << " miles\n\n";

        Coordinate coord;
        coord.latitude = endI->latitude;
        coord.longitude = endI->longitude;
        route.push_back(coord);
    }
};
    
class HashTable
{
protected:
    Entry** table;
    int count, cap;

    unsigned int hash(const string& s)
    {
        unsigned int v = 92439827;
        for (int i = 0; i < s.length(); i++)
        {
            v = v * 169 + s[i];
        }
        return v;
    }

    void resize()
    {
        int newsize = 4 + cap * 3;
        Entry** newTable = new Entry*[newsize];

        for (int i = 0; i < newsize; i++)
        {
            newTable[i] = NULL;
        }

        for (int i = 0; i < cap; i++)
        {
            Entry* current = table[i];
            while (current != NULL)
            {
                Entry* next = current->next;
                int newIndex = hash(current->place->name) % newsize;

                current->next = newTable[newIndex];
                newTable[newIndex] = current;

                current = next;
            }
        }
        delete[] table;
        table = newTable;
        cap = newsize;
    }

public:
    HashTable()
    {
        count = 0;
        cap = 0;
        table = new Entry*[cap];
        for (int i = 0; i < cap; i++)
        {
            table[i] = NULL;
        }
    }

    ~HashTable()
    {
        for (int i = 0; i < cap - 1; i++)
        {
            Entry* current = table[i];
            while (current != NULL)
            {
                Entry* next = current->next;
                delete current->place;
                delete current;
                current = next;
            }
            table[i] = NULL;
        }
        delete[] table;
        table = NULL;
    }

    void insert(Place* place)
    {
        if (count * 4 >= cap * 3)
        {
            resize();
        }
        unsigned int index = hash(place->name) % cap;
        table[index] = new Entry(place, table[index]);
        count++;
    }

    Place* find(const string& name, const string& state)
    {
        unsigned int index = hash(name) % cap;
        Entry* current = table[index];
        while (current != NULL)
        {
            if (current->place->name == name && current->place->state == state)
            {
                return current->place;
            }
            current = current->next;
        }
        return NULL;
    }

    vector<Place*> findAll(const string& name)
    {
        vector<Place*> results;
        unsigned int index = hash(name) % cap;
        Entry* current = table[index];
        while (current != NULL)
        {
            if (current->place->name == name)
            {
                results.push_back(current->place);
            }
            current = current->next;
        }
        return results;
    }
};

string trim(const string& str)
{
    int start = 0;
    while (start < str.length() && isspace(str[start]))
    {
        start++;
    }

    int end = str.length() - 1;
    while (end >= start && isspace(str[end]))
    {
        end--;
    }
    return str.substr(start, end - start + 1);
}

void parsePlace(const string& line, HashTable& ht)
{
    int code = strtol(line.substr(0, 8).c_str(), NULL, 10);
    string state = line.substr(8, 2);
    string name = trim(line.substr(10, 30));
    int population = strtol(line.substr(59, 7).c_str(), NULL, 10);
    double area = strtod(line.substr(70, 10).c_str(), NULL);
    double latitude = strtod(line.substr(80, 10).c_str(), NULL);
    double longitude = strtod(line.substr(90, 11).c_str(), NULL);
    int roadCode = strtol(line.substr(101, 5).c_str(), NULL, 10);
    double distance = strtod(line.substr(106, 8).c_str(), NULL);
    Place* place = new Place(code, state, name, population, area, latitude, longitude, roadCode, distance);
    ht.insert(place);
}

void parseInters(const string& line, vector<inters*>& intersects, int index)
{
    double longitude = strtod(line.substr(0, 10).c_str(), NULL);
    double latitude = strtod(line.substr(10, 10).c_str(), NULL);
    double distance = strtod(line.substr(20, 8).c_str(), NULL);
    string state = line.substr(28, 2);
    string placeName = trim(line.substr(31));

    inters* intern = new inters(state, placeName, longitude, latitude, distance);
    intern->index = index;
    intersects.push_back(intern);
}

void start(HashTable& ht, vector<inters*>& intersects)
{
    ifstream fin("C:\\Users\\johna\\Documents\\named-places.txt");
    if (fin.fail())
    {
        cout << "File not found\n";
        exit(1);
    }
    string line;
    while (getline(fin, line))
    {
        parsePlace(line, ht);
    }
    fin.close();

    ifstream fin_1("C:\\Users\\johna\\Documents\\intersections.txt");
    if (fin_1.fail())
    {
        cout << "File not found\n";
        exit(1);
    }

    line = "";
    int indexer = 0;
    while (getline(fin_1, line))
    {
        parseInters(line, intersects, indexer);
        indexer += 1;
    }
    fin_1.close();

    ifstream fin_2("C:\\Users\\johna\\Documents\\connections.txt");
    if (fin_2.fail())
    {
        cout << "File not found\n";
        exit(1);
    }

    line = "";
    while (getline(fin_2, line))
    {
        istringstream iss(line);
        string rN;
        string rT;
        int inters1, inters2;
        double dist;
        iss >> rN >> rT >> inters1 >> inters2 >> dist;
        if (rN == "?")
        {
            rN = "unamed";
        }
        if (iss)
        {
            Road* road = new Road(rN, rT, inters1, inters2, dist);
            intersects[inters1]->addRoad(road);
            intersects[inters2]->addRoad(road);
        }
    }
    fin_2.close();
}

vector<Tile> parseCov(const string& file_path)
{
    vector<Tile> map_tiles;
    ifstream file(file_path);
    string line;

    while (getline(file, line))
    {
        istringstream iss(line);
        Tile tile;
        iss >> tile.lat_max >> tile.lat_min >> tile.lon_min >> tile.lon_max >> tile.file_name;
        map_tiles.push_back(tile);
    }

    return map_tiles;
}

void InitMapTiles()
{
    string coverage_file = "C:\\Users\\johna\\Documents\\coverage.txt";
    tiles = parseCov(coverage_file);
}

void user(istream& in, HashTable& ht, vector<inters*>& intersects)
{
    string command, name, state;
    while (true)
    {
        in >> command;
        if (in.fail())
        {
            break;
        }
        if (command == "N")
        {
            getline(in, name);
            name = trim(name);
            vector<Place*> places = ht.findAll(name);

            if (places.empty())
            {
                cout << "No places found with the name " << name << "\n\n";
            }
            else
            {
                for (int i = 0; i < places.size(); i++)
                {
                    places[i]->print();
                }
            }
            break;
        }
        else if (command == "S")
        {
            getline(in, name);
            name = trim(name);

            if (name.length() < 3)
            {
                cout << "Invalid input. Please provide both place name and state abbreviation.\n\n";
                break;
            }

            state = name.substr(name.length() - 2);
            name = name.substr(0, name.length() - 2);

            name = trim(name);
            Place* p = ht.find(name, state);

            if (p == NULL)
            {
                cout << "No such place exists\n\n";
            }
            else
            {
                p->print();
                intersects[p->roadCode]->print();
            }
            break;
        }
        else if (command == "travel")
        {
            getline(in, name);
            name = trim(name);

            if (name.length() < 3)
            {
                cout << "Invalid input. Please provide both place name and state abbreviation.\n\n";
                break;
            }

            state = name.substr(name.length() - 2);
            name = name.substr(0, name.length() - 2);

            name = trim(name);
            Place* p = ht.find(name, state);

            if (p != NULL)
            {
                Travelmanager manager(intersects);
                manager.travelm(p);
            }
            else
            {
                cout << "Place does Not Exist\n\n";
            }
            break;
        }
        else if (command == "short")
        {
            string sName, eName;
            cout << "Enter the start place name and state: ";
            getline(cin, sName);
            sName = trim(sName);

            cout << "Enter the destination place name and state: ";
            getline(cin, eName);
            eName = trim(eName);

            string sState = sName.substr(sName.length() - 2);
            sName = sName.substr(0, sName.length() - 2);
            sName = trim(sName);

            string eState = eName.substr(eName.length() - 2);
            eName = eName.substr(0, eName.length() - 2);
            eName = trim(eName);

            Place* s = ht.find(sName, sState);
            Place* e = ht.find(eName, eState);

            if (s == NULL || e == NULL)
            {
                cout << "One or both places not found.\n";
            }
            else
            {
                Travelmanager manager2(intersects);
                manager2.shortP(s, e);
                manager2.reset();
            }
            break;
        }
        else if (command == "Dij")
        {
            cout << "starting place: ";
            in.ignore();
            getline(cin, name);
            name = trim(name);

            vector<Place*> startPlaces = ht.findAll(name);
            if (startPlaces.empty())
            {
                cout << "No places found with the name " << name << "\n\n";
                continue;
            }

            cout << startPlaces.size() << " possibilities: ";
            for (int i = 0; i < startPlaces.size(); i++)
            {
                cout << startPlaces[i]->state;
                if (i < startPlaces.size() - 1)
                {
                    cout << " ";
                }
            }
            cout << "\nstate: ";
            getline(cin, state);
            state = trim(state);

            Place* startPlace = NULL;
            for (int i = 0; i < startPlaces.size(); i++)
            {
                if (startPlaces[i]->state == state)
                {
                    startPlace = startPlaces[i];
                    break;
                }
            }

            if (!startPlace)
            {
                cout << "Invalid state selection.\n";
                continue;
            }

            cout << "Place is " << startPlace->name << ", " << startPlace->state
                 << ", pop " << startPlace->population << " ("
                 << startPlace->latitude << ", " << startPlace->longitude
                 << "), closest intersection " << startPlace->roadCode << "\n\n";

            cout << "destination: ";
            getline(cin, name);
            name = trim(name);

            vector<Place*> endPlaces = ht.findAll(name);
            if (endPlaces.empty())
            {
                cout << "No places found with the name " << name << "\n\n";
                continue;
            }

            cout << endPlaces.size() << " possibilities: ";
            for (int i = 0; i < endPlaces.size(); i++)
            {
                cout << endPlaces[i]->state;
                if (i < endPlaces.size() - 1)
                {
                    cout << " ";
                }
            }
            cout << "\nstate: ";
            getline(cin, state);
            state = trim(state);

            Place* endPlace = NULL;
            for (int i = 0; i < endPlaces.size(); i++)
            {
                if (endPlaces[i]->state == state)
                {
                    endPlace = endPlaces[i];
                    break;
                }
            }

            if (!endPlace)
            {
                cout << "Invalid state selection.\n";
                continue;
            }

            cout << "Place is " << endPlace->name << ", " << endPlace->state
                 << ", pop " << endPlace->population << " ("
                 << endPlace->latitude << ", " << endPlace->longitude
                 << "), closest intersection " << endPlace->roadCode << "\n\n";

            Travelmanager manager3(intersects);
            manager3.dij(startPlace, endPlace);
            manager3.reset();
            break;
        }
    }
}

int main()
{
    HashTable ht;
    vector<inters*> Intersections;
    InitMapTiles();
    start(ht, Intersections);

    while (true)
    {
        string line;
        cout << "Enter command: " << "\n";

        getline(cin, line);
        if (line == "Q" || line == "q")
        {
            cout << "Program Exited\n";
            break;
        }

        istringstream in(line);

        while (true)
        {
            try
            {
                user(in, ht, Intersections);
            }
            catch (exception e)
            {
                cout << "Something went scary";
            }
            break;
        }
    }

    for (int i = 0; i < Intersections.size(); i++)
    {
        delete Intersections[i];
    }

    return 0;
}