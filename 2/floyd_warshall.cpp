#include <iostream>
#include <stack>
#include <vector>
#include <climits>
#include <fstream>
#include <algorithm>

using namespace std;

const int INF = 999;

// ������� ��� ������ �����
vector<vector<int>> ReadFile(const string filename) {
    ifstream inputFile(filename);

    if (!inputFile.is_open()) {
        throw runtime_error("Unable to open the file: " + filename);
    }

    int numberOfVert;
    inputFile >> numberOfVert;

    vector<vector<int>> matrix(numberOfVert, vector<int>(numberOfVert));

    //��������� ������� ����� �����
    for (int i = 0; i < numberOfVert; i++) {
        for (int j = 0; j < numberOfVert; j++) {
            inputFile >> matrix[i][j];
            if (matrix[i][j] > INF) {
                throw overflow_error("Overflow the permissible weight value!");
            }
        }
    }

    inputFile.close();

    return matrix;
}

// ������� ��� ������ ���� �� `i` � `j`
void printPath(const vector<vector<int>>& path, int i, int j, ofstream& file) {
    stack<int> stack;

    // ������� ��� ������������� ������� � ����
    while (path[i][j] != i) {
        stack.push(path[i][j]);
        j = path[i][j];
    }

    // ���� ���� �� ����, ��������� ��������
    while (!stack.empty()) {
        int temp = stack.top();
        stack.pop();
        cout << temp << " -> ";
        file << temp << " -> ";
    }
}

void printAllPath(vector<vector<int>> const& path, ofstream& outputFile) {
    int n = path.size();

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j && path[i][j] != -1) {
                cout << "The shortest path from " << i << " to " << j << " { " << i << "-> ";
                outputFile << "The shortest path from " << i << " to " << j << " { " << i << "-> ";
                printPath(path, i, j, outputFile);
                cout << j << " }" << endl;
                outputFile << j << " }" << endl;
            }
            else if (path[i][j] == -1) {
                cout << "There is no path from " << i << " to " << j << endl;
                outputFile << "There is no path from " << i << " to " << j << "\n";
            }
        }
    }
}

// ������� ��� ������ �������
void printMatrix(vector<vector<int>> matrix, ofstream& outputFile) {
    int numberOfVert = matrix.size();
    for (int i = 0; i < numberOfVert; i++) {
        for (int j = 0; j < numberOfVert; j++) {
            if (matrix[i][j] == INF) {
                cout << "INF" << "\t";
                outputFile << "INF" << "\t";
            }
            else {
                cout << matrix[i][j] << "\t";
                outputFile << matrix[i][j] << "\t";
            }
        }
        outputFile << "\n";
        cout << endl;
    }
    outputFile << "\n";
    cout << endl;
}

// ������� ��� ������� ��������� ������-��������
void floydWarshall(vector<vector<int>> const& Matrix, string const filename) {
    ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        throw runtime_error("Unable to open the file: " + filename);
    }

    // ����������� ������� ��������� `adjMatrix`
    int n = Matrix.size();

    // ������� �������
    if (n == 0) {
        return;
    }

    // distance ��������� ���������� ��������� (���)
    vector<vector<int>> distance(n, vector<int>(n));
    // path ��������� ��� ����
    vector<vector<int>> path(n, vector<int>(n));

    // �������������� distance � path
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            distance[i][j] = Matrix[i][j];

            if (i == j) {
                path[i][j] = 0;
            }
            else if (distance[i][j] != INF) {
                path[i][j] = i;
            }
            else {
                path[i][j] = -1;
            }
        }
    }

    // �������� ���� ������-��������
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // ���� k � ������������� ������� � ���������� ���� �� `i` � `j`, �� ������� �������� distance[i][j]
                if (distance[i][k] != INF && distance[k][j] != INF && (distance[i][k] + distance[k][j] < distance[i][j])) {
                    distance[i][j] = distance[i][k] + distance[k][j];
                    path[i][j] = path[k][j];
                }
            }

            // ���� ������������ �������� ���������� ��������������, graph �������� ���� �������������� ����
            if (distance[i][i] < 0) {
                cout << "Negative-weight cycle found!";
                return;
            }
        }
    }

    // ������� ���������� ���� ����� ����� ������ ������ � ���� �������
    printMatrix(distance, outputFile);
    // ������� �� ����� ��� ���� � �����
    printAllPath(path, outputFile);

    outputFile.close();
}

int main() {
    vector<vector<int>> Matrix = ReadFile("input.txt");

    // ������ ��������� ������-��������
    floydWarshall(Matrix, "output.txt");

    return 0;
}