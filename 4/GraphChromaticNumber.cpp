#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <list>

using namespace std;

class Graph {
private:
    int numVertices;
    vector<list<int>> adjacencyList; 
public:
    Graph(int V) : numVertices(V) {
        adjacencyList.resize(V);
    }

    void addEdge(int u, int v) {
        adjacencyList[u].push_back(v);
        adjacencyList[v].push_back(u);
    }

    const list<int>& getNeighbors(int v) const {
        return adjacencyList[v];
    }

    int getNumVertices() const {
        return numVertices;
    }
};

Graph ReadFile(const string filename, int& V) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Could not open the file!" << endl;
        exit(1);
    }

    file >> V;
    if (V == 0) {
        cerr << "������ ����!" << endl;
        exit(1);
    }

    Graph graph(V);

    int v, w;
    while (file >> v >> w) {
        graph.addEdge(v, w);
    }

    file.close();
    return graph;
}

// ������� ��� ���������� ������ �� �������� �������
vector<int> sortVerticesByDegree(const Graph& graph) {
    int numVertices = graph.getNumVertices();
    vector<pair<int, int>> degreeAndIndex(numVertices);

    for (int i = 0; i < numVertices; i++) {
        degreeAndIndex.push_back(make_pair(graph.getNeighbors(i).size(), i));
    }

    sort(degreeAndIndex.begin(), degreeAndIndex.end(), greater<>()); // �� ��������

    // ���������� �������� ������
    vector<int> sortedVertices;
    for (const auto& p : degreeAndIndex) {
        sortedVertices.push_back(p.second);
    }

    return sortedVertices;
}

// ������� ���������� �������������� ����� �����
int welshPowellAlgorithm(const Graph& graph, vector<int>& colors) {
    int n = graph.getNumVertices();
    vector<int> sortedVertices = sortVerticesByDegree(graph);

    int currentColor = 0;

    for (int v : sortedVertices) {
        if (colors[v] == -1) { // ���� ������� ��� �� ��������, ����������� ������� ����
            colors[v] = currentColor;

            // ������������ ��������� ������� ���� ������, ���� ��� �� ��������
            for (int u : sortedVertices) {
                if (colors[u] == -1) {
                    bool canColor = true;
                    for (int neighbor : graph.getNeighbors(u)) {
                        if (colors[neighbor] == currentColor) { // �������� �������
                            canColor = false;
                            break;
                        }
                    }

                    if (canColor) {
                        colors[u] = currentColor; // ������������ �������
                    }
                }
            }

            currentColor++;
        }
    }

    return currentColor;
}

void GetResult(const Graph& graph, int V) {
    ofstream outputFile("output.txt");

    vector<int> colors(V, -1);
    int chromaticNumber = welshPowellAlgorithm(graph, colors);

    cout << "������������� ����� (�����������): " << chromaticNumber << endl;
    outputFile << "������������� ����� (�����������): " << chromaticNumber << "\n";

    cout << "����� ������:" << endl;
    outputFile << "����� ������:" << "\n";
    for (int i = 0; i < V; ++i) {
        cout << "������� " << i + 1 << ": ���� " << colors[i] << endl;
        outputFile << "������� " << i << ": ���� " << colors[i] << "\n";
    }

    outputFile.close();
}

int main() {
    setlocale(LC_CTYPE, "Russian");
    int V = 0;
    Graph graph = ReadFile("input.txt", V);

    GetResult(graph, V);

    return 0;
}