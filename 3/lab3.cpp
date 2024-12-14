#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <fstream>
#include <unordered_set>
#include <set>

using namespace std;

class Graph {
    int V;  // Количество вершин
public:
    vector<list<int>> adj;  // Список смежности 

    Graph(int V) : V(V) {
        adj.resize(V);
    }

    void addEdge(int v, int w) {
        adj[v].push_back(w);
        adj[w].push_back(v);
    }

    void findCyclesFromNode(int v, vector<bool>& visited, set<vector<int>>& allCycles) {
        stack<pair<int, int>> s;  // Стек для хранения пар (вершина, родитель)
        s.push({ v, -1 });       // Начинаем с текущей вершины и её родителя

        // Массив для хранения родителей
        vector<int> parentArray(V, -1);

        while (!s.empty()) {
            pair<int, int> currentPair = s.top(); // Получаем пару из стека
            s.pop(); // Удаляем пару из стека

            int curVertex = currentPair.first; // Извлекаем текущую вершину
            int curParent = currentPair.second; // Извлекаем родительскую вершину

            if (!visited[curVertex]) {
                visited[curVertex] = true;  // Помечаем вершину как посещённую

                for (int neighbor : adj[curVertex]) {
                    if (!visited[neighbor]) {
                        // Если сосед не посещён, добавляем его в стек
                        s.push({ neighbor, curVertex });
                        parentArray[neighbor] = curVertex; // Записываем родителя
                    }
                    else if (neighbor != curParent) {
                        // Если сосед посещён и не является родителем, найден цикл
                        vector<int> cycle;
                        // Восстанавливаем цикл, начиная с текущего узла, двигаясь по родителям
                        int current = curVertex;
                        while (current != -1) {
                            cycle.push_back(current);
                            if (current == neighbor) {
                                break; // Если достигли начальной точки цикла
                            }
                            current = parentArray[current]; // Переходим к родителю
                        }
                        cycle.push_back(curVertex);  // Добавляем текущую вершину в цикл
                        allCycles.insert(cycle);
                    }
                }
            }
        }
    }

    // Основной метод для поиска всех уникальных циклов
    void findAllCycles(set<vector<int>>& allCycles) {
        vector<bool> visited(V, false);

        for (int v = 0; v < V; ++v) {
            if (!visited[v]) {
                findCyclesFromNode(v, visited, allCycles);
            }
        }

        // Вывод всех циклов
        /*for (const auto& cycle : allCycles) {
            for (int node : cycle) {
                cout << node << " ";
            }
            cout << endl;
        }*/
    }

    bool checkSubcyclicity(int& nonCyclicU, int& nonCyclicV) {
        // Попытка добавить ребро между каждой парой несмежных вершин
        for (int u = 0; u < V; ++u) {
            unordered_set<int> neighbors(adj[u].begin(), adj[u].end());
            for (int v = 0; v < V; ++v) {
                if (u != v && neighbors.find(v) == neighbors.end()) {
                    adj[u].push_back(v);
                    adj[v].push_back(u);

                    set<vector<int>> allCycles;
                    findAllCycles(allCycles);

                    adj[u].pop_back();
                    adj[v].pop_back();

                    if (allCycles.size() != 1) {
                        nonCyclicU = u;
                        nonCyclicV = v;
                        return false;
                    }
                }
            }
        }
        return true;
    }

    bool checkTree(set<vector<int>>& allCycles, int& nonCyclicU, int& nonCyclicV) {
        bool isTree = true;

        findAllCycles(allCycles);
        if (allCycles.size() != 0) { // ацикличность
            isTree = false;
        }

        if (!checkSubcyclicity(nonCyclicU, nonCyclicV)) { // субцикличность
            isTree = false;
        }

        int edgeCount = 0;
        for (int i = 0; i < V; ++i)
            edgeCount += adj[i].size();
        edgeCount /= 2;
        if (edgeCount != V - 1) { // древочисленность
            isTree = false;
        }

        return isTree;
    }
};

Graph ReadFile(const string& filename, int& V) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open the file!" << endl;
        exit(1);
    }

    file >> V;
    Graph g(V);

    int v, w;
    while (file >> v >> w) {
        g.addEdge(v, w);
    }

    file.close();
    return g;
}

int main() {
    setlocale(LC_CTYPE, "Russian");
    int V = 0;
    Graph g = ReadFile("input.txt", V);
    if (g.adj.size() == 0) {
        cout << "Пустой граф" << endl;
        return -1;
    }

    set<vector<int>> allCycles;
    int nonCyclicU = -1, nonCyclicV = -1;

    ofstream outputFile("output.txt");

    if (g.checkTree(allCycles, nonCyclicU, nonCyclicV)) {
        cout << "Граф является деревом и древочисленный.\n";
        outputFile << "Граф является деревом и древочисленный.\n";
    }
    else {
        if (!allCycles.empty()) {
            cout << "Нарушена ацикличность. Найдены циклы: ";
            outputFile << "Нарушена ацикличность. Найдены циклы: ";
            for (const auto& cycle : allCycles) {
                for (int node : cycle) {
                    cout << node << " ";
                    outputFile << node << " ";
                }
                cout << endl;
                outputFile << endl;
            }
        }
        else {
            cout << "Ацикличность не нарушена.\n";
            outputFile << "Ацикличность не нарушена.\n";
        }
        if (nonCyclicU != -1 && nonCyclicV != -1) {
            cout << "Нарушена субцикличность для ребра: (" << nonCyclicU << ", " << nonCyclicV << ")\n";
            outputFile << "Нарушена субцикличность для ребра: (" << nonCyclicU << ", " << nonCyclicV << ")\n";
        }
        else {
            cout << "Субцикличность не нарушена\n";
            outputFile << "Субцикличность не нарушена\n";
        }

        int edgeCount = 0;
        for (int i = 0; i < V; ++i) {
            edgeCount += g.adj[i].size();
        }
        edgeCount /= 2;
        if (edgeCount != V - 1) {
            cout << "Граф не является древочисленным, так как количество рёбер (" << edgeCount << ") не равно V-1 (" << V - 1 << ").\n";
            outputFile << "Граф не является древочисленным, так как количество рёбер (" << edgeCount << ") не равно V-1 (" << V - 1 << ").\n";
        }
        else {
            cout << "Граф является древочисленным, так как количество рёбер (" << edgeCount << ") равно V-1 (" << V - 1 << ").\n";
            outputFile << "Граф является древочисленным, так как количество рёбер (" << edgeCount << ") равно V-1 (" << V - 1 << ").\n";
        }
    }

    outputFile.close();
    return 0;
}