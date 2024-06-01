#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTEX 10

bool VER[MAX_VERTEX] = {false, }; // ����� ���� �����ϴ��� ���θ� �����ϴ� �迭

struct Node { // ��� ����ü
    int vertex; // ����� ��
    struct Node* next; // ���� ��带 ����Ű�� ������
};

struct Graph { // �׷��� ����ü
    struct Node* adjLists[MAX_VERTEX]; // ����� �迭
    int visited[MAX_VERTEX]; // �湮 ���θ� �����ϴ� �迭
};

struct Node *createNode(int v); // ��� ���� �Լ�
struct Graph *createGraph(); // �׷��� ���� �Լ�
void insertVertex(struct Graph* graph, int v); // ��� ���� �Լ�
void addEdge(struct Graph* graph, int src, int dest); // ���� �߰� �Լ�
void sortAdjLists(struct Graph* graph); // ���� ����Ʈ ���� �Լ�
void printGraph(struct Graph* graph); // �׷��� ��� �Լ�
void DFS(struct Graph* graph, int vertex); // ���� �켱 Ž�� �Լ�
void BFS(struct Graph* graph, int startVertex); // �ʺ� �켱 Ž�� �Լ�
void resetVisited(struct Graph* graph); // �湮 ���� �ʱ�ȭ �Լ�

int main() {
    struct Graph* graph; // �׷��� ����ü ������
    char command; // ��ɾ�
    int v1, v2; // ��� ��

    do {
        printf("----------------------------------------------------------------\n");
        printf("                         Graph Searches                         \n");
        printf("----------------------------------------------------------------\n");
        printf(" Initialize Graph \t= z\n");
        printf(" Insert Vertex   \t= v \tInsert Edge = e\n");
        printf(" Depth First Search \t= d \tBreath First Search = b\n");
        printf(" Print Graph        \t= p \tQuit = q\n");
        printf("----------------------------------------------------------------\n");

        printf("Command = ");
        scanf(" %c", &command);

        switch(command) { // �Է¿� ���� �Լ� ȣ��
        case 'z':
        case 'Z':
            printf(" --- Initialize Graph\n");
            graph = createGraph();
            break;
        case 'v':
        case 'V':
            printf(" --- Insert Vertex\n");
            scanf("%d", &v1);
            insertVertex(graph, v1);
            break;
        case 'e':
        case 'E':
            printf(" --- Insert Edge\n");
            scanf("%d %d", &v1, &v2);
            addEdge(graph, v1, v2);
            sortAdjLists(graph);
            break;
        case 'd':
        case 'D':
            printf(" --- Depth First Search(DFS)\n");
            scanf("%d", &v1);
            resetVisited(graph);
            DFS(graph, v1);
            break;
        case 'b':
        case 'B':
            printf(" --- Breath First Search(BFS)\n");
            scanf("%d", &v1);
            resetVisited(graph);
            BFS(graph, v1);
            break;
        case 'p':
        case 'P':
            printf(" --- Print Graph\n");
            printGraph(graph);
            break;
        case 'q':
        case 'Q':
            printf("Quit\n");
            break;
        default: // �߸��� �Է��� �� �й��� �̸� ���
            printf("---------------------- 2023041018 ������ -----------------------\n");
            break;
        }
    } while (command != 'q' && command != 'Q'); // q�� �Է��ϸ� ����
    return 1;
}

struct Node* createNode(int v) { // ��� ���� �Լ�
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node)); // ��� ũ�⸸ŭ �޸� �Ҵ�
    newNode->vertex = v; // ����� �� ����
    newNode->next = NULL; // ���� ��带 NULL�� ����
    return newNode; // ������ ��� ��ȯ
}

struct Graph* createGraph() { // �׷��� ���� �Լ�
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph)); // �׷��� ũ�⸸ŭ �޸� �Ҵ�
    for (int i = 0; i < MAX_VERTEX; i++) { // �׷����� ������ NULL�� �ʱ�ȭ
        graph->adjLists[i] = NULL; // ��带 NULL�� �ʱ�ȭ
        graph->visited[i] = 0; // �湮 ���θ� 0���� �ʱ�ȭ
    }
    return graph; // ������ �׷��� ��ȯ
}

void insertVertex(struct Graph* graph, int v) { // ��� ���� �Լ�
    if (v >= MAX_VERTEX || v < 0) { // ����� ���� ������ �����
        printf("Error: Vertex number out of range.\n"); // ���� �޽��� ���
        return; // �Լ� ����
    }
    else VER[v] = true; // ����� ���� ���� ���� ������ ��带 ����
    printf("Vertex %d inserted.\n", v); // ��� ���� �޽��� ���
}

void addEdge(struct Graph* graph, int src, int dest) { // ���� �߰� �Լ�
    if (src >= MAX_VERTEX || dest >= MAX_VERTEX || src < 0 || dest < 0) { // ����� ���� ������ �����
        printf("Error: Vertex number out of range.\n"); // ���� �޽��� ���
        return; // �Լ� ����
    }
    if (!VER[src] || !VER[dest]) { // ����� ���� �������� ������
        printf("Error: Vertex does not exist.\n"); // ���� �޽��� ���
        return; // �Լ� ����
    }
    struct Node* newNode = createNode(dest); // ���ο� ��� ����
    newNode->next = graph->adjLists[src]; // ���ο� ��带 src ����� ���� ���� ����
    graph->adjLists[src] = newNode; // src ��带 ���ο� ���� ����

    newNode = createNode(src); // ���ο� ��� ����
    newNode->next = graph->adjLists[dest]; // ���ο� ��带 dest ����� ���� ���� ����
    graph->adjLists[dest] = newNode; // dest ��带 ���ο� ���� ����
}

void sortAdjLists(struct Graph* graph) { // ���� ����Ʈ ���� �Լ�
    for (int i = 0; i < MAX_VERTEX; i++) { // ��� ��忡 ����
        if (graph->adjLists[i] != NULL) { // ��尡 NULL�� �ƴϸ�
            struct Node* sorted = NULL; // ���ĵ� ���
            struct Node* current = graph->adjLists[i]; // ����� ���� ��带 ����Ű�� ������
            while (current != NULL) { // ��尡 NULL�� �ƴ� ������ (��尡 ���� ������)
                struct Node* next = current->next; // ���� ��带 ����Ű�� ������
                if (sorted == NULL || sorted->vertex >= current->vertex) { // ���ĵ� ��尡 NULL�̰ų� ���ĵ� ����� ���� �� ũ��
                    current->next = sorted; // ��带 ���ĵ� ���� ����
                    sorted = current; // ��带 ���ĵ� ���� ����
                } else { // ���ĵ� ����� ���� �� ������
                    struct Node* temp = sorted; // ���ĵ� ��带 ����Ű�� ������
                    while (temp->next != NULL && temp->next->vertex < current->vertex) temp = temp->next; 
                    // ���ĵ� ����� ���� ��尡 NULL�� �ƴϰ� ���ĵ� ����� ���� ����� ���� �� ���� ������ �̵�
                    current->next = temp->next; // ��带 ���ĵ� ����� ���� ���� ����
                    temp->next = current; // ��带 ���ĵ� ���� ����
                }
                current = next; // ���� ���� �̵�
            }
            graph->adjLists[i] = sorted; // ��带 ���ĵ� ���� ����
        }
    }
}

void printGraph(struct Graph* graph) { // �׷��� ��� �Լ�
    if (graph == NULL) { // �׷����� �ʱ�ȭ���� �ʾ�����
        printf("Error: Graph is not initialized.\n"); // ���� �޽��� ���
        return; // �Լ� ����
    }
    for (int v = 0; v < MAX_VERTEX; v++) { // ��� ��忡 ����
        struct Node* temp = graph->adjLists[v]; // ����� ���� ��带 ����Ű�� ������
        printf("Vertex %d -> ", v); // ����� �� ���
        while (temp) { // ��尡 NULL�� �ƴ� ������
            printf("%d   ", temp->vertex); // ����� �� ���
            temp = temp->next; // ���� ���� �̵�
        }
        printf("\n"); // �ٹٲ�
    }
}

void DFS(struct Graph* graph, int vertex) { // ���� �켱 Ž�� �Լ�
    struct Node* adjList = graph->adjLists[vertex]; // ����� ���� ��带 ����Ű�� ������
    struct Node* temp = adjList; // ����� ���� ��带 ����Ű�� ������

    graph->visited[vertex] = 1; // ��� �湮 ���θ� 1�� ����
    printf("Visited %d \n", vertex); // ��� �湮 �޽��� ���

    while (temp != NULL) { // ��尡 NULL�� �ƴ� ������
        int connectedVertex = temp->vertex; // ����� ��
        if (graph->visited[connectedVertex] == 0) DFS(graph, connectedVertex); // ��尡 �湮���� �ʾ����� ��� ȣ��
        temp = temp->next; // ���� ���� �̵�
    }
}

void BFS(struct Graph* graph, int startVertex) { // �ʺ� �켱 Ž�� �Լ�
    int queue[MAX_VERTEX]; // ť �迭
    int front = 0; // ť�� ��
    int rear = 0; // ť�� ��

    graph->visited[startVertex] = 1; // ���� ��� �湮 ���θ� 1�� ����
    queue[rear++] = startVertex; // ���� ��带 ť�� ����

    while (front < rear) { // ť�� ������� ���� ������
        int currentVertex = queue[front++]; // ť�� �տ� �ִ� ��带 ������
        printf("Visited %d\n", currentVertex); // ��� �湮 �޽��� ���
        struct Node* temp = graph->adjLists[currentVertex]; // ����� ���� ��带 ����Ű�� ������

        while (temp) { // ��尡 NULL�� �ƴ� ������
            int adjVertex = temp->vertex; // ����� ��
            if (graph->visited[adjVertex] == 0) { // ��尡 �湮���� �ʾ�����
                queue[rear++] = adjVertex; // ť�� ��� ����
                graph->visited[adjVertex] = 1; // ��� �湮 ���θ� 1�� ����
            }
            temp = temp->next; // ���� ���� �̵�
        }
    }
}