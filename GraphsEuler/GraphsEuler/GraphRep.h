#pragma once

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
#include <queue>
#include <list>
#include <array>
#include <map>
#include <chrono>

class AdjacencyMatrix
{
private:
    std::vector< std::vector<bool> > data;

    int degree(int vertex) {
        int deg = 0;
        for (auto u = 0; u < data.size(); u++)
            deg += data[vertex][u];
        return deg;
    }

    void eulerianCycleDfs(int v, std::list<int>& cycle) {
        for (size_t u = 0; u < data.size(); u++) {
            if (data[v][u]) {
                data[v][u] = data[u][v] = false;
                eulerianCycleDfs(u, cycle);
            }
        }
        cycle.push_front(v);
    }

    bool contains(std::vector<int>& collection, int element) {
        for (auto test : collection)
            if (test == element)
                return true;
        return false;
    }

    bool hamiltonianCycleDfs(int v, std::vector<int>& visited) {
        if (contains(visited, v)) {
            if (visited.size() == data.size() && v == 0) {
                visited.push_back(v);
                return true;
            }
            return false;
        }

        visited.push_back(v);
        for (size_t u = 0; u < data.size(); u++) {
            if (data[v][u] && hamiltonianCycleDfs(u, visited)) {
                return true;
            }
        }

        visited.pop_back();
        return false;
    }

    bool isSafe(int v, std::vector<std::vector<bool>> graph, int path[], int pos)
    {
        if (graph[path[pos - 1]][v] == 0)
            return false;
        for (int i = 0; i < pos; i++)
            if (path[i] == v)
                return false;
        return true;
    }

    bool hamCycleUtil(std::vector<std::vector<bool>> graph, int path[], int pos)
    {
        int V = graph.size();
        if (pos == V)
        {
            if (graph[path[pos - 1]][path[0]] == 1)
                return true;
            else
                return false;
        }

        for (int v = 1; v < V; v++)
        {
            if (isSafe(v, graph, path, pos))
            {
                path[pos] = v;
                if (hamCycleUtil(graph, path, pos + 1) == true)
                    return true;
                path[pos] = -1;
            }
        }
        return false;
    }
    void printSolution(int path[], int V)
    {
        std::cout << "Solution Exists:";
        std::cout << " Following is one Hamiltonian Cycle \n" << std::endl;
        for (int i = 0; i < V; i++)
            std::cout << path[i] << "  ";
        std::cout << path[0] << std::endl;
    }
public:
    AdjacencyMatrix(std::vector< std::vector<bool> > _data) :
        data(_data) {}

    AdjacencyMatrix(std::string file_path)
    {
        std::ifstream input(file_path);
        if (!input.good())
        {
            std::cerr << "[error] (bad file) " << file_path << "\n";
            exit(1);
        }

        for (std::string line; getline(input, line);)
        {
            std::istringstream line_input(line);
            std::vector<bool> line_data;
            for (bool byte; line_input >> byte;)
                line_data.push_back(byte);
            data.push_back(line_data);
        }
    }

    AdjacencyMatrix(std::vector< std::vector<int> > list) {
        auto listData = list;
        std::vector< std::vector<bool> > tempData(listData.size(), std::vector<bool>(listData.size()));

        for (int i = 0; i < listData.size(); i++)
        {
            for (int y = 0; y < listData[i].size(); y++)
            {
                tempData[i][listData[i][y]] = true;
                tempData[listData[i][y]][i] = true;
            }
        }
        data = tempData;
    }

    std::vector< std::vector<bool> > getData() {
        return data;
    }

    void setData(std::vector< std::vector<bool> > _data) {
        data = _data;
    }

    bool is_edge(int u, int v) {
        return data[u][v] == true;
    }

    void print() {
        std::cout << "AdjacencyMatrix[\n";
        for (auto row : data)
        {
            std::cout << "  ";
            for (int col : row)
                std::cout << col << " ";
            std::cout << "\n";
        }
        std::cout << "]\n";
    }

  /*  int countBackedgesAdjMatrix()
    {
        auto adj_matrix = data;

        int backedges = 0;
        for (int row = 0; row < adj_matrix.size(); row++)
            for (int col = 0; col < adj_matrix[row].size(); col++)
                if (adj_matrix[row][col])
                    if (GraphUtils::checkForBackedge(row, col))
                        backedges += 1;

        if (backedges != 0)
            GraphUtils::cyclic_graphs += 1;
        return backedges;
    }*/

    bool isEulerian() {
        for (auto vertex = 0; vertex < data.size(); vertex++)
            if (degree(vertex) % 2 != 0)
                return false;
        return true;
    }

    void makeEulerian() {
        srand(time(NULL));

        for (int i = 0; i < data.size(); i++) {
            int deg = degree(i);

            if (deg % 2 != 0) {
                int x;

                do
                {
                    x = rand() % data.size();
                } while (degree(x) <= 2 || data[i][x]);

                if (data[i][x]) {
                    data[i][x] = false;
                    data[x][i] = false;
                }
                else {
                    data[i][x] = true;
                    data[x][i] = true;
                }
            }
        }
    }

    void makeCoherent() {
        srand(time(NULL));

        for (int i = 0; i < data.size(); i++) {
            int deg = degree(i);

            if (deg == 0) {
                int x;

                do
                {
                    x = rand() % data.size();
                } while (degree(x) <= 1 || data[i][x]);

                if (data[i][x]) {
                    data[i][x] = false;
                    data[x][i] = false;
                }
                else {
                    data[i][x] = true;
                    data[x][i] = true;
                }
            }
        }
    }

    bool isCoherent() {
        for (auto v = 0; v < data.size(); v++)
            if (degree(v) == 0)
                return false;
        return true;
    }

   std::vector<int> hamiltonianCycle() {
        std::vector<int> visited;
        if (hamiltonianCycleDfs(0, visited)) {
            return visited;
        }
        else {
            std::cout << "no hamilton cycle";
            return std::vector<int>();
        }
    }

    std::list<int> eulerianCycle() {
        auto tempData = data;
        std::list<int> cycle;
        eulerianCycleDfs(0, cycle);
        data = tempData;
        return cycle;
    }

    bool hamCycle(std::vector<std::vector<bool>> graph)
    {
        int V = graph.size();
        int* path = new int[V];
        for (int i = 0; i < V; i++)
            path[i] = -1;
        path[0] = 0;
        if (hamCycleUtil(graph, path, 1) == false)
        {
            std::cout << "\nSolution does not exist" << std::endl;
            return false;
        }
        printSolution(path, V);
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const AdjacencyMatrix& am);
};

std::ostream& operator<<(std::ostream& os, const AdjacencyMatrix& am)
{
    auto data = am.data;
    for (auto ver1 = 0; ver1 < am.data.size(); ++ver1)
    {
        for (auto ver2 = 0; ver2 < am.data.size(); ++ver2)
        {
            os << data[ver1][ver2] << " ";
        }
        os << "\n";
    }
    return os;
}

class AdjacencyList
{
private:
    std::vector< std::vector<int> > data;
    int hamiltonianCycle = 0;

public:

    std::vector<std::vector<int>> getData()
    {
        return data;
    }

    AdjacencyList(std::string file_path)
    {
        std::ifstream input(file_path);

        if (!input.good())
        {
            std::cerr << "[error] (bad file) " << file_path << "\n";
            exit(1);
        }

        for (std::string line; getline(input, line);)
        {
            std::istringstream line_input(line);
            std::vector<int> line_data;
            int nr;
            line_input >> nr;

            for (int n; line_input >> n;)
                line_data.push_back(n);
            data.push_back(line_data);
        }
    }

    AdjacencyList(AdjacencyMatrix& adj, bool coherent = false)
    {
        auto tempData = adj.getData();

        std::vector< std::vector<int> > create(tempData.size(), std::vector<int>());

        for (unsigned int y = 0; y < tempData.size(); y++)
        {
            for (unsigned int x = 0; x < tempData[y].size(); x++)
                if (tempData[y][x] && !coherent)
                    create[y].push_back(x);
                else if (tempData[y][x] && coherent)
                {
                    create[y].push_back(x);
                }
        }

        data = create;
    }

    void hamiltonianPaths(AdjacencyList& graph, int v, std::vector<bool>& visited, std::vector<int>& path, int n)
    {
        auto data = graph.getData();

        if (path.size() == n)
        {
            //printPath(path);

            //is cycle
            for (int i = 0; i < data[path[path.size() - 1]].size(); i++)
            {
                if (data[path[path.size() - 1]][i] == path[0])
                    hamiltonianCycle += 1;
            }

            return;
        }

        for (int w : data[v])
        {
            if (!visited[w])
            {
                visited[w] = true;
                path.push_back(w);

                hamiltonianPaths(graph, w, visited, path, n);

                visited[w] = false;
                path.pop_back();
            }
        }
    }

    int findHamiltonianCycles()
    {
        int n = data.size();
        for (int start = 0; start < n; start++)
        {
            std::vector<int> path;
            path.push_back(start);

            std::vector<bool> visited(n);
            visited[start] = true;

            hamiltonianPaths(*this, start, visited, path, n);
        }

        return hamiltonianCycle;
    }
};