#include <iostream>
#include <vector>
#include <list>
#include <limits>
#include <sstream>
#include <fstream>

using namespace std;

class Graph {
private:
    int vertices = 100; // Количество вершин в графе
    vector<list<pair<int, int>>> adjList; // Список смежности для хранения рёбер и их весов
public:
    Graph(int V) : vertices(V), adjList(V) {}

    void addEdge(int src, int dest, int weight) {

        // Добавляем ребро в список смежности для исходной вершины
        adjList[src].emplace_back(dest, weight);

        // Добавляем ребро в список смежности для конечной вершины
        adjList[dest].emplace_back(src, weight);
    }

    void removeEdge(int src, int dest) {
        adjList[src].remove_if([&](const pair<int, int> &edge) { return edge.first == dest; });
        adjList[dest].remove_if([&](const pair<int, int> &edge) { return edge.first == src; });
    }

    void printGraph() {
        for (int i = 0; i < vertices; ++i) {
            for (auto &edge: adjList[i]) {
                cout << "(" << i << ", " << edge.first << ", " << edge.second << ") ";
            }
        }
        cout << endl;
    }
    void changeEdgeWeight(int src, int dest, int newWeight) {
        // Ищем ребро в списке смежности для исходной вершины
        auto itSrc = find_if(adjList[src].begin(), adjList[src].end(), [&](const pair<int, int>& edge) {
            return edge.first == dest;
        });

        // Если ребро найдено, обновляем его вес
        if (itSrc != adjList[src].end()) {
            itSrc->second = newWeight;
        }

        // Ищем ребро в списке смежности для конечной вершины
        auto itDest = find_if(adjList[dest].begin(), adjList[dest].end(), [&](const pair<int, int>& edge) {
            return edge.first == src;
        });

        // Если ребро найдено, обновляем его вес
        if (itDest != adjList[dest].end()) {
            itDest->second = newWeight;
        }
    }
    // Рекурсивный метод для обхода в глубину (Depth-First Search)
    void dfs(int start, int end, vector<bool> &visited, vector<int> &path, vector<int> &minWeight) {
        visited[start] = true;
        path.push_back(start);

        if (start == end) {
            int totalWeight = 0;
            for (size_t i = 0; i < path.size() - 1; ++i) {
                for (auto &edge: adjList[path[i]]) {
                    if (edge.first == path[i + 1]) {
                        totalWeight += edge.second;
                        break;
                    }
                }
            }

            if (totalWeight < minWeight[end]) {
                minWeight[end] = totalWeight;
            }
            cout << "(";
            for (size_t i = 0; i < path.size() - 1; ++i) {
                cout << path[i] << ", ";
            }

        } else {
            for (auto &neighbor: adjList[start]) {
                if (!visited[neighbor.first]) {
                    dfs(neighbor.first, end, visited, path, minWeight);
                }
            }
        }

        visited[start] = false;
        path.pop_back();
    }

    // Метод для поиска кратчайшего пути между вершинами start и end
    void findShortestPath(int start, int end) {
        if (start < 0 || start >= vertices || end < 0 || end >= vertices) {
            cout << "NO, NO, NOWAY" << endl;
            return;
        }

        vector<bool> visited(vertices, false);
        vector<int> path, minWeight(vertices, numeric_limits<int>::max());

        dfs(start, end, visited, path, minWeight);

        if (minWeight[end] != numeric_limits<int>::max()) {
            cout << end << ", " << minWeight[end] << ")" << endl;
        } else {
            cout << "NO, NO, NOWAY" << endl;
        }
    }

    void readGraphFromFile(const string &filename) {
        ifstream file(filename);
        if (file.is_open()) {
            int edges;
            int n;
            file >> n >> edges;

            adjList.resize(vertices);

            for (int i = 0; i < edges; ++i) {
                int src, dest, weight;
                file >> src >> dest >> weight;
                addEdge(src, dest, weight);
            }

            file.close();
        } else {
            cout << "NO, NO, NOWAY" << filename << endl;
        }
    }

    void writeGraphToCSV(const string &filename) {
        ofstream file(filename);
        if (file.is_open()) {
            // Записываем количество вершин и рёбер в начало файла
            file << vertices+1 << " " << countEdges() << "\n";
            for (int i = 0; i < vertices; ++i) {
                for (const auto &edge: adjList[i]) {
                    file << i << " " << edge.first << " " << edge.second << endl;
                }
            }

            file.close();
        } else {
            cout << "NO, NO, NOWAY" << filename << endl;
        }
    }

    int countEdges() const {
        int edges = 0;
        for (int i = 0; i < vertices; ++i) {
            edges += adjList[i].size();
        }
        // Каждое ребро учтено дважды в списке смежности (для каждой вершины), поэтому делим на 2
        return edges / 2;
    }
};

int main() {
    Graph graph(99);
    graph.readGraphFromFile("/Users/aleksey/CLionProjects/untitled1/input.txt");
    //cout << "Введите команды в формате add (3, 4, 8), remove (3, 4), findShortestPath (1,3) или 'exit' для выхода." << endl;

    string command;
    getline(cin, command);


    stringstream ss(command);
    string cmd;
    ss >> cmd;

    if (cmd == "add") {
        int src, dest, weight;
        char dummy;
        ss >> dummy >> src >> dummy >> dest >> dummy >> weight;
        graph.addEdge(src, dest, weight);
        graph.printGraph();
        graph.writeGraphToCSV("/Users/aleksey/CLionProjects/untitled1/input.txt");
    } else if (cmd == "remove") {
        int src, dest;
        char dummy;
        ss >> dummy >> src >> dummy >> dest;
        graph.removeEdge(src, dest);
        graph.printGraph();
        graph.writeGraphToCSV("/Users/aleksey/CLionProjects/untitled1/input.txt");
    } else if (cmd == "changeEdgeWeight") {
        int src, dest, weight;
        char dummy;
        ss >> dummy >> src >> dummy >> dest >> dummy >> weight;
        graph.changeEdgeWeight(src, dest, weight);
        graph.printGraph();
        graph.writeGraphToCSV("/Users/aleksey/CLionProjects/untitled1/input.txt");
    } else if (cmd == "findShortestPath") {
        int start, end;
        char dummy;
        ss >> dummy >> start >> dummy >> end;
        graph.findShortestPath(start, end);
    } else {
        cout << "NO, NO, NOWAY" << cmd << endl;
    }

    return 0;
}