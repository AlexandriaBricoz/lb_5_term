#include <iostream>
#include <vector>
#include <list>
#include <limits>
#include <sstream>
#include <fstream>

using namespace std;

class Graph {
private:
    int vertices; // Количество вершин в графе
    vector<list<pair<int, int>>> adjList; // Список смежности для хранения рёбер и их весов

public:
    Graph(int V) : vertices(V), adjList(V) {}

    void addEdge(int src, int dest, int weight) {
        // Проверяем, если вершины src и dest выходят за пределы текущего количества вершин
        if (src >= vertices || dest >= vertices) {
            // Увеличиваем количество вершин до максимального из src и dest
            vertices = max(src, dest) + 1;
            // Ресайзим список смежности
            adjList.resize(vertices);
        }

        // Ищем существующее ребро между вершинами src и dest
        auto it = find_if(adjList[src].begin(), adjList[src].end(), [&](const pair<int, int>& edge) {
            return edge.first == dest;
        });

        if (it != adjList[src].end()) {
            // Ребро найдено, обновляем вес
            it->second = weight;

            // Теперь ищем ребро в обратном направлении (dest -> src)
            it = find_if(adjList[dest].begin(), adjList[dest].end(), [&](const pair<int, int>& edge) {
                return edge.first == src;
            });

            if (it != adjList[dest].end()) {
                // Ребро найдено, обновляем вес
                it->second = weight;
            } else {
                // Ребра в обратном направлении нет, добавляем его
                adjList[dest].emplace_back(make_pair(src, weight));
            }
        } else {
            // Ребра между вершинами нет, добавляем его
            adjList[src].emplace_back(make_pair(dest, weight));
            adjList[dest].emplace_back(make_pair(src, weight));
        }
    }


    void removeEdge(int src, int dest) {
        adjList[src].remove_if([&](const pair<int, int>& edge) { return edge.first == dest; });
        adjList[dest].remove_if([&](const pair<int, int>& edge) { return edge.first == src; });
    }

    void printGraph() {
        for (int i = 0; i < vertices; ++i) {
            for (const auto& edge : adjList[i]) {
                cout << "(" << i << ", " << edge.first << ", " << edge.second << ") ";
            }
        }
        cout << endl;
    }

    void dfs(int start, int end, vector<bool>& visited, vector<int>& path, vector<int>& minWeight) {
        visited[start] = true;
        path.push_back(start);

        if (start == end) {
            int totalWeight = 0;
            for (size_t i = 0; i < path.size() - 1; ++i) {
                for (const auto& edge : adjList[path[i]]) {
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
            for (const auto& neighbor : adjList[start]) {
                if (!visited[neighbor.first]) {
                    dfs(neighbor.first, end, visited, path, minWeight);
                }
            }
        }

        visited[start] = false;
        path.pop_back();
    }

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

    void readGraphFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            int edges;
            file >> vertices >> edges;

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

    void writeGraphToCSV(const string& filename) {
        ofstream file(filename);
        if (file.is_open()) {
            // Записываем количество вершин и рёбер в начало файла
            file << vertices << " " << countEdges() << "\n";
            for (int i = 0; i < vertices; ++i) {
                for (const auto& edge : adjList[i]) {
                    file << i << " " << edge.first << " " << edge.second << "\n";
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
        graph.writeGraphToCSV("/Users/aleksey/CLionProjects/untitled1/input.txt");
        graph.printGraph();
    } else if (cmd == "remove") {
        int src, dest;
        char dummy;
        ss >> dummy >> src >> dummy >> dest;
        graph.removeEdge(src, dest);
        graph.writeGraphToCSV("/Users/aleksey/CLionProjects/untitled1/input.txt");
        graph.printGraph();
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
