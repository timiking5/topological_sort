#include <iostream>
#include <stdlib.h>

using namespace std;
/*
Насколько я понял, задача на работу с графами.
Суть же заключается в том, чтобы сделать топологическую сортировку.
Т.к. имеем дело с графами, я полагаю, что я могу оставить за собой
право выбора представления графа - будь то матрица смежности или список смежности.
На мой взгляд, в данном случае список смежности подойдёт лучше.
*/

struct adjNode {
    int value;
    adjNode* next;
    adjNode* prev;
    adjNode(int val) {
        value = val;
        next = nullptr;
        prev = nullptr;
    }
    
}; // Структура для реализации списка смежности.

struct adjList {
    adjNode* head;
    adjNode* tail;
public:
    adjList() {
        head = nullptr;
        tail = nullptr;
    }
    void addNode(int val) {
        adjNode* new_node = new adjNode(val);
        new_node->next = nullptr;
        if (head == nullptr) {
            head = new_node;
        }
        else {
            adjNode* tmp = head;
            head = new_node;
            tmp->prev = new_node;
            new_node->next = tmp;
        }
    }
    int getLength() {
        if (head == nullptr) {
            return 0;
        }
        else {
            int length = 0;
            adjNode * tmp = head;
            while (tmp != nullptr) {
                length++;
                tmp = tmp->next;
            }
            return length;
        }
    }
};

struct Vertex {
    int value;
    char color;  // для поиска в глубину
    int d;  // discroverd - время открытия
    int f;  // finished - время закрытия
    Vertex * parent;  // условный предок вершины в направленном графе
    adjList * Adj;  // название не очень, конечно, но мне оно кажется понятнее, чем "Edges"
    Vertex(int val) {
        parent = nullptr;
        Adj = new adjList();
        value = val;
        d = -1;
        f = -1;
        color = 'U';  // unsigned
    }
    void addEdge(int vertex_val) {
        Adj->addNode(vertex_val);
    }
};


class Graph {
private:
    int time;  // нужно для DFS
    int n;  // кол-во вершин
    Vertex * Vertexes;
    
public:
    Graph(int vertexes_num) {  // где vertexes_num- количество вершин
        time = 0;
        if (vertexes_num > 0) {
            n = vertexes_num;
            Vertexes = (Vertex*)malloc(sizeof(Vertex) * n);
            if (Vertexes) {
                for (int i = 0; i < n; i++) {

                    Vertexes[i] = Vertex(i);
                }
            }
        }
        else {
            n = 0;
            Vertexes = nullptr;
            //  тут можно что-то придумать, но сейчас это не основная задача
        }
    }

    void AddEdge(int u, int v) {
        Vertexes[u].addEdge(v);
    }

    void CycleVertexes() {
        for (int i = 0; i < n; i++)
            cout << Vertexes[i].value << " (" << Vertexes[i].d << " ," << Vertexes[i].f << ") " 
            << Vertexes[i].Adj->getLength() << " --> " << Vertexes[i].parent << endl;  // чтобы посмотреть, что всё работает
    }

    void DFS() {
        for (int i = 0; i < n; i++) {
            Vertexes[i].color = 'W';  // White
            Vertexes[i].parent = NULL;  // инициализируем аттрибуты вершин для нового DFS
        }
        time = 0;
        
        for (int i = 0; i < n; i++) {
            if (Vertexes[i].color == 'W') {
                cout << "We are going to vertex number " << i << " in DFS\n";
                DFSVisit(Vertexes[i]);
            }
        }
    }

    void DFSVisit(Vertex &u) {
        time++;
        u.d = time;  // Вершина была открыта на ходу под номером time
        u.color = 'G';  // Стала серой (её обнаружили один раз, её могут обнаружить второй раз, если есть цикл)
        cout << "We made vertex " << u.value << " grey\n";
        int n = u.Adj->getLength();
        if (n > 0) {
            adjNode* tmp = u.Adj->head;
            for (int i = 0; i < n; i++) {
                if (Vertexes[tmp->value].color == 'W') {
                    Vertexes[tmp->value].parent = &u;
                    DFSVisit(Vertexes[tmp->value]);
                }
                tmp = tmp->next;
            }
        }
        u.color = 'B';  // Чёрная
        time++;
        u.f = time;  // закрыли вершину, обозначив её цветов и обозначив её номер(шаг) закрытия
    }
    
};


class BuildGraph;

class Builder {
public:
    explicit Builder(size_t num_threads);

    void execute(const BuildGraph& build_graph, size_t target_id);
};

int main()
{
    int n = 9;
    Graph test = Graph(n);
    cout << "Let's asd go\n";

    test.AddEdge(0, 1);
    test.AddEdge(0, 2);
    test.AddEdge(1, 2);
    test.AddEdge(3, 2);
    test.AddEdge(1, 5);
    test.AddEdge(6, 5);
    test.AddEdge(6, 7);
    test.AddEdge(7, 8);
    test.AddEdge(5, 8);
    test.DFS();
    test.CycleVertexes();
    
}
