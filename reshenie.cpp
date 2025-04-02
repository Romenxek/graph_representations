#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <unordered_map>
#include <fstream>
#include <sstream>

using namespace std;

class Graph {

public:
    void readGraph(std::string fileName) 
    {
        ifstream inputFile(fileName);  
        char typeInput;
        string line;
        int number;

        inputFile >> typeInput; 

        if (typeInput == 'C') { typeRepr = 0;}
        else if (typeInput == 'L') { typeRepr = 1; }
        else if (typeInput == 'E') { typeRepr = 2; }

        inputFile >> cntV;

        if (typeRepr == 2) { inputFile >> cntE; }

        inputFile >> isOriented;
        inputFile >> isWeighted;

        if (typeRepr == 0) 
        {
            for (auto& row : adjacencyMatrix) { row.clear(); }  // Очистка предыдущих данных

            adjacencyMatrix.clear();

            for (int i = 0; i < cntV; i++) { adjacencyMatrix.push_back(vector<int>(cntV, 0)); }

            for (int i = 0; i < cntV; i++) 
            {
                for (int j = 0; j < cntV; j++) 
                {
                    inputFile >> number;
                    if (number != 0) { cntE++; }
                    adjacencyMatrix[i][j] = number;
                }

            }

        }

        else if (typeRepr == 1) 
        {
            for (auto& s : adjVerList) { s.clear(); }  // Очистка предыдущих данных

            adjVerList.clear();

            vector<int> numbers;

            getline(inputFile, line);

            for (int i = 0; i < cntV; i++) { adjVerList.emplace_back(); }

            for (int i = 0; i < cntV; i++) 
            {
                numbers.clear();

                getline(inputFile, line);

                istringstream iss(line);

                while (iss >> number) { numbers.push_back(number); }

                if (!isWeighted) {

                    for (int j : numbers) 
                    {
                        adjVerList[i].insert(make_pair(numbers[j] - 1, 1));
                        cntE++;
                    }

                }

                else {

                    for (int j = 0; j < numbers.size(); j += 2) 
                    {
                        adjVerList[i].insert(make_pair(numbers[j] - 1, numbers[j + 1]));

                        cntE++;
                    }
                }
            }
            cout << endl << cntE << endl;
        }

        else if (typeRepr == 2) 
        {
            int u, v;

            edgeList.clear();  

            for (int i = 0; i < cntE; i++) {

                inputFile >> u >> v; 

                u--; v--;  // Индексация с 0

                if (isWeighted) { inputFile >> number; }
                else { number = 1; }

                edgeList.push_back(make_tuple(u, v, number));
            }

            for (auto& i : edgeList) 
            {
                cout << get<0>(i) << " " << get<1>(i) << " " << get<2>(i) << endl;  // Выводим ребра

                break;  // Прерываем цикл после первого ребра
            }
        }
    }
    void addEdge(int from, int to, int weight = 1) 
    {
        from--;  // Индексация с 0

        to--;    // Индексация с 0

        cntE++;  // Увеличиваем количество рёбер

        if (typeRepr == 0) 
        {
            adjacencyMatrix[from][to] = weight;

            if (!isOriented) { adjacencyMatrix[to][from] = weight; }
        }
        else if (typeRepr == 1) 
        {
            adjVerList[from].insert(make_pair(to, weight));

            if (!isOriented) { adjVerList[to].insert(make_pair(from, weight)); }
        }
        else if (typeRepr == 2) { edgeList.push_back(make_tuple(from, to, weight)); }
    }
    void removeEdge(int from, int to) 
    {
        from--; to--;  // Индексация с 0

        if (typeRepr == 0) 
        {
            adjacencyMatrix[from][to] = 0;

            if (!isOriented) { adjacencyMatrix[to][from] = 0; }
        }
        else if (typeRepr == 1) 
        {
            for (const auto& pair : adjVerList[from]) 
            {
                if (pair.first == to) 
                {
                    adjVerList[from].erase(pair);
                    break;
                }
            }
            if (!isOriented) 
            {
                for (const auto& pair : adjVerList[to]) 
                {
                    if (pair.first == from) 
                    {
                        adjVerList[to].erase(pair);
                        break;

                    }
                }
            }
        }
        else if (typeRepr == 2) 
        {
            pair<int, int> eraseInd;

            eraseInd.first = -1; eraseInd.second = -1;

            for (int i = 0; i < cntE; i++) 
            {
                if (get<0>(edgeList[i]) == from && get<1>(edgeList[i]) == to) 
                { eraseInd.first = i; }
            }

            if (eraseInd.first != -1) { edgeList.erase(edgeList.begin() + eraseInd.first); }
        }

        cntE--;  // Уменьшаем количество рёбер
    }
    int changeEdge(int from, int to, int newWeight) {

        from--; to--;  // Индексация с 0

        int oldWeight;
        pair<int, int> edge;

        if (typeRepr == 0) 
        {
            oldWeight = adjacencyMatrix[from][to];

            adjacencyMatrix[from][to] = newWeight;

            if (!isOriented) { adjacencyMatrix[to][from] = newWeight; }
        }
        else if (typeRepr == 1) 
        {
            for (const auto& pair : adjVerList[from]) 
            {
                if (pair.first == to) 
                {
                    oldWeight = pair.second;
                    edge.first = pair.first;
                    edge.second = newWeight;

                    adjVerList[from].erase(pair);

                    break;
                }
            }

            adjVerList[from].insert(edge);

            if (!isOriented) 
            {
                for (const auto& pair : adjVerList[to]) 
                {
                    if (pair.first == from) 
                    {
                        edge.first = pair.first;
                        adjVerList[to].erase(pair);

                        break;
                    }
                }

                if (edge.first != to) { adjVerList[to].insert(edge); }
            }
        }
        else if (typeRepr == 2) 
        {
            for (int i = 0; i < cntE; i++) 
            {
                if (get<0>(edgeList[i]) == from && get<1>(edgeList[i]) == to) 
                {
                    oldWeight = get<2>(edgeList[i]);
                    edgeList[i] = make_tuple(get<0>(edgeList[i]), get<1>(edgeList[i]), newWeight);

                    break;
                }
            }
        }

        return oldWeight;  // Возвращаем старый вес
    }
    void transformToAdjList() {

        for (auto& s : adjVerList) { s.clear(); }

        adjVerList.clear();

        for (int i = 0; i < cntV; i++) { adjVerList.emplace_back(); }

        if (typeRepr == 0) 
        {
            for (int i = 0; i < cntV; i++) 
            {
                for (int j = 0; j < cntV; j++) 
                {
                    if (adjacencyMatrix[i][j] == 0) { continue; }

                    adjVerList[i].insert(make_pair(j, adjacencyMatrix[i][j]));
                }

            }

            for (auto& row : adjacencyMatrix) { row.clear(); }

            adjacencyMatrix.clear();
        }
        else if (typeRepr == 2) 
        {
            for (int i = 0; i < cntE; i++) 
            {
                adjVerList[get<0>(edgeList[i])].insert(make_pair(get<1>(edgeList[i]), get<2>(edgeList[i])));

                if (!isOriented) { adjVerList[get<1>(edgeList[i])].insert(make_pair(get<0>(edgeList[i]), get<2>(edgeList[i]))); }
            }
            edgeList.clear();
        }

        typeRepr = 1;
    }
    void transformToAdjMatrix() {

        for (auto& row : adjacencyMatrix) { row.clear(); }

        adjacencyMatrix.clear();

        for (int i = 0; i < cntV; i++) 
        {
            adjacencyMatrix.push_back(vector<int>(cntV, 0));  // Инициализация пустой матрицы
        }

        if (typeRepr == 1) 
        {
            for (size_t i = 0; i < adjVerList.size(); ++i) 
            {
                for (const auto& edge : adjVerList[i]) { adjacencyMatrix[i][edge.first] = edge.second; }
            }
            for (auto& s : adjVerList) { s.clear(); }

            adjVerList.clear();
        }
        else if (typeRepr == 2) 
        {
            for (int i = 0; i < cntE; i++) 
            {
                adjacencyMatrix[get<0>(edgeList[i])][get<1>(edgeList[i])] = get<2>(edgeList[i]);

                if (!isOriented) { adjacencyMatrix[get<1>(edgeList[i])][get<0>(edgeList[i])] = get<2>(edgeList[i]); }
            }
            edgeList.clear();
        }
        typeRepr = 0;  // Меняем тип представления на матрицу смежности
    }
    void transformToListOfEdges() 
    {
        edgeList.clear();

        if (typeRepr == 0) 
        {
            for (int i = 0; i < cntV; i++) 
            {
                for (int j = i + 1; j < cntV; j++) 
                {
                    if (adjacencyMatrix[i][j] == 0) { continue; }

                    edgeList.push_back(make_tuple(i, j, adjacencyMatrix[i][j]));

                    if (adjacencyMatrix[i][j] != adjacencyMatrix[j][i]) { edgeList.push_back(make_tuple(j, i, adjacencyMatrix[j][i])); }
                }
            }

            for (auto& row : adjacencyMatrix) { row.clear(); }

            adjacencyMatrix.clear();
        }
        else if (typeRepr == 1) 
        {
            pair<int, int> el;
            int j;
            vector<vector<bool>> flags;

            for (int i = 0; i < cntV; i++) { flags.push_back(vector<bool>(adjVerList[i].size(), false)); }

            for (int i = 0; i < cntV; i++) 
            {
                j = -1;

                for (const auto& edge : adjVerList[i]) 
                {
                    j++;

                    if (flags[i][j]) { continue; }

                    edgeList.push_back(make_tuple(i, edge.first, edge.second));

                    flags[i][j] = true;

                    auto reverseEdge = adjVerList[edge.first].find(make_pair(i, edge.second));

                    if (reverseEdge != adjVerList[edge.first].end()) {

                        flags[edge.first][distance(adjVerList[edge.first].begin(), reverseEdge)] = true;

                    }
                }
            }
            for (auto& s : adjVerList) { s.clear(); }

            adjVerList.clear();
        }
        typeRepr = 2;  // Меняем тип представления на список рёбер
    }
    void writeGraph(string fileName) 
    {
        ofstream outFile(fileName);

        if (!outFile) 
        {
            cout << "Ошибка открытия файла для записи." << endl;

            return;
        }

        if (typeRepr == 0) { outFile << "C "; }
        else if (typeRepr == 1) { outFile << "L "; }
        else if (typeRepr == 2) { outFile << "E "; }

        outFile << cntV;  // Записываем количество вершин

        if (typeRepr == 2) { outFile << " " << cntE; }  // Если список рёбер, записываем количество рёбер

        outFile << endl;

        outFile << (int)isOriented << " " << (int)isWeighted << endl;  // Записываем ориентацию и взвешенность графа

        if (typeRepr == 0) 
        {
            for (int i = 0; i < cntV; i++) 
            {
                for (int j = 0; j < cntV; j++) 
                {
                    outFile << adjacencyMatrix[j][i] << " ";
                }

                outFile << endl;
            }
        }
        else if (typeRepr == 1) 
        {
            for (int i = 0; i < cntV; i++) 
            {
                for (const auto& edge : adjVerList[i]) 
                {
                    outFile << edge.first << " "; 

                    if (isWeighted) { outFile << edge.second << " "; }
                }

                outFile << endl;
            }
        }
        else if (typeRepr == 2) 
        {
            for (int i = 0; i < cntE; i++) 
            {
                outFile << get<0>(edgeList[i]) + 1 << " " << get<1>(edgeList[i]) + 1;  // Запись списка рёбер

                if (isWeighted) { outFile << " " << get<2>(edgeList[i]); }

                outFile << endl;
            }
        }
    }

public:

    // Матрица смежности
    vector<vector<int>> adjacencyMatrix;

    // Список смежных вершин
    vector<set<pair<int, int>>> adjVerList;

    // Список рёбер
    vector<tuple<int, int, int>> edgeList;

    bool isOriented;  // Ориентирован ли граф

    bool isWeighted;  // Взвешенный ли граф

    int typeRepr = 0;  // Тип представления графа: 0 - матрица смежности, 1 - список смежности, 2 - список рёбер

    int cntV = 0;  // Количество вершин в графе

    int cntE = 0;  // Количество рёбер в графе

};
 
// Основная функция

int main() {

    Graph g;

    g.readGraph("input_1e3_1e5.txt");  // Чтение графа из файла

    g.transformToAdjMatrix();  // Преобразование графа в матрицу смежности

    g.transformToAdjList();  // Преобразование графа в список смежности

    g.transformToListOfEdges();  // Преобразование графа в список рёбер

    g.writeGraph("output.txt");  // Запись графа в файл

    return 0;  // Завершение программы
}
