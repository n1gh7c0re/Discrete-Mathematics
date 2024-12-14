#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <fstream>
#include <unordered_set>
#include <set>

using namespace std;

class Graph {
    int V;  // ���������� ������
public:
    vector<list<int>> adj;  // ������ ��������� 

    Graph(int V) : V(V) {
        adj.resize(V);
    }

    void addEdge(int v, int w) {
        adj[v].push_back(w);
        adj[w].push_back(v);
    }

    void findCyclesFromNode(int v, vector<bool>& visited, set<vector<int>>& allCycles) {
        stack<pair<int, int>> s;  // ���� ��� �������� ��� (�������, ��������)
        s.push({ v, -1 });       // �������� � ������� ������� � � ��������

        // ������ ��� �������� ���������
        vector<int> parentArray(V, -1);

        while (!s.empty()) {
            pair<int, int> currentPair = s.top(); // �������� ���� �� �����
            s.pop(); // ������� ���� �� �����

            int curVertex = currentPair.first; // ��������� ������� �������
            int curParent = currentPair.second; // ��������� ������������ �������

            if (!visited[curVertex]) {
                visited[curVertex] = true;  // �������� ������� ��� ����������

                for (int neighbor : adj[curVertex]) {
                    if (!visited[neighbor]) {
                        // ���� ����� �� �������, ��������� ��� � ����
                        s.push({ neighbor, curVertex });
                        parentArray[neighbor] = curVertex; // ���������� ��������
                    }
                    else if (neighbor != curParent) {
                        // ���� ����� ������� � �� �������� ���������, ������ ����
                        vector<int> cycle;
                        // ��������������� ����, ������� � �������� ����, �������� �� ���������
                        int current = curVertex;
                        while (current != -1) {
                            cycle.push_back(current);
                            if (current == neighbor) {
                                break; // ���� �������� ��������� ����� �����
                            }
                            current = parentArray[current]; // ��������� � ��������
                        }
                        cycle.push_back(curVertex);  // ��������� ������� ������� � ����
                        allCycles.insert(cycle);
                    }
                }
            }
        }
    }

    // �������� ����� ��� ������ ���� ���������� ������
    void findAllCycles(set<vector<int>>& allCycles) {
        vector<bool> visited(V, false);

        for (int v = 0; v < V; ++v) {
            if (!visited[v]) {
                findCyclesFromNode(v, visited, allCycles);
            }
        }

        // ����� ���� ������
        /*for (const auto& cycle : allCycles) {
            for (int node : cycle) {
                cout << node << " ";
            }
            cout << endl;
        }*/
    }

    bool checkSubcyclicity(int& nonCyclicU, int& nonCyclicV) {
        // ������� �������� ����� ����� ������ ����� ��������� ������
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
        if (allCycles.size() != 0) { // ������������
            isTree = false;
        }

        if (!checkSubcyclicity(nonCyclicU, nonCyclicV)) { // ��������������
            isTree = false;
        }

        int edgeCount = 0;
        for (int i = 0; i < V; ++i)
            edgeCount += adj[i].size();
        edgeCount /= 2;
        if (edgeCount != V - 1) { // ����������������
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
        cout << "������ ����" << endl;
        return -1;
    }

    set<vector<int>> allCycles;
    int nonCyclicU = -1, nonCyclicV = -1;

    ofstream outputFile("output.txt");

    if (g.checkTree(allCycles, nonCyclicU, nonCyclicV)) {
        cout << "���� �������� ������� � ��������������.\n";
        outputFile << "���� �������� ������� � ��������������.\n";
    }
    else {
        if (!allCycles.empty()) {
            cout << "�������� ������������. ������� �����: ";
            outputFile << "�������� ������������. ������� �����: ";
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
            cout << "������������ �� ��������.\n";
            outputFile << "������������ �� ��������.\n";
        }
        if (nonCyclicU != -1 && nonCyclicV != -1) {
            cout << "�������� �������������� ��� �����: (" << nonCyclicU << ", " << nonCyclicV << ")\n";
            outputFile << "�������� �������������� ��� �����: (" << nonCyclicU << ", " << nonCyclicV << ")\n";
        }
        else {
            cout << "�������������� �� ��������\n";
            outputFile << "�������������� �� ��������\n";
        }

        int edgeCount = 0;
        for (int i = 0; i < V; ++i) {
            edgeCount += g.adj[i].size();
        }
        edgeCount /= 2;
        if (edgeCount != V - 1) {
            cout << "���� �� �������� ��������������, ��� ��� ���������� ���� (" << edgeCount << ") �� ����� V-1 (" << V - 1 << ").\n";
            outputFile << "���� �� �������� ��������������, ��� ��� ���������� ���� (" << edgeCount << ") �� ����� V-1 (" << V - 1 << ").\n";
        }
        else {
            cout << "���� �������� ��������������, ��� ��� ���������� ���� (" << edgeCount << ") ����� V-1 (" << V - 1 << ").\n";
            outputFile << "���� �������� ��������������, ��� ��� ���������� ���� (" << edgeCount << ") ����� V-1 (" << V - 1 << ").\n";
        }
    }

    outputFile.close();
    return 0;
}