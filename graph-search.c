#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTEX 10

bool VER[MAX_VERTEX] = {false, }; // 노드의 값이 존재하는지 여부를 저장하는 배열

struct Node { // 노드 구조체
    int vertex; // 노드의 값
    struct Node* next; // 다음 노드를 가리키는 포인터
};

struct Graph { // 그래프 구조체
    struct Node* adjLists[MAX_VERTEX]; // 노드의 배열
    int visited[MAX_VERTEX]; // 방문 여부를 저장하는 배열
};

struct Node *createNode(int v); // 노드 생성 함수
struct Graph *createGraph(); // 그래프 생성 함수
void insertVertex(struct Graph* graph, int v); // 노드 삽입 함수
void addEdge(struct Graph* graph, int src, int dest); // 간선 추가 함수
void sortAdjLists(struct Graph* graph); // 인접 리스트 정렬 함수
void printGraph(struct Graph* graph); // 그래프 출력 함수
void DFS(struct Graph* graph, int vertex); // 깊이 우선 탐색 함수
void BFS(struct Graph* graph, int startVertex); // 너비 우선 탐색 함수
void resetVisited(struct Graph* graph); // 방문 여부 초기화 함수

int main() {
    struct Graph* graph; // 그래프 구조체 포인터
    char command; // 명령어
    int v1, v2; // 노드 값

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

        switch(command) { // 입력에 따라 함수 호출
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
        default: // 잘못된 입력일 때 학번과 이름 출력
            printf("---------------------- 2023041018 김준후 -----------------------\n");
            break;
        }
    } while (command != 'q' && command != 'Q'); // q를 입력하면 종료
    return 1;
}

struct Node* createNode(int v) { // 노드 생성 함수
    struct Node* newNode = (struct Node*) malloc(sizeof(struct Node)); // 노드 크기만큼 메모리 할당
    newNode->vertex = v; // 노드의 값 설정
    newNode->next = NULL; // 다음 노드를 NULL로 설정
    return newNode; // 생성된 노드 반환
}

struct Graph* createGraph() { // 그래프 생성 함수
    struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph)); // 그래프 크기만큼 메모리 할당
    for (int i = 0; i < MAX_VERTEX; i++) { // 그래프의 노드들을 NULL로 초기화
        graph->adjLists[i] = NULL; // 노드를 NULL로 초기화
        graph->visited[i] = 0; // 방문 여부를 0으로 초기화
    }
    return graph; // 생성된 그래프 반환
}

void insertVertex(struct Graph* graph, int v) { // 노드 삽입 함수
    if (v >= MAX_VERTEX || v < 0) { // 노드의 값이 범위를 벗어나면
        printf("Error: Vertex number out of range.\n"); // 에러 메시지 출력
        return; // 함수 종료
    }
    else VER[v] = true; // 노드의 값이 범위 내에 있으면 노드를 삽입
    printf("Vertex %d inserted.\n", v); // 노드 삽입 메시지 출력
}

void addEdge(struct Graph* graph, int src, int dest) { // 간선 추가 함수
    if (src >= MAX_VERTEX || dest >= MAX_VERTEX || src < 0 || dest < 0) { // 노드의 값이 범위를 벗어나면
        printf("Error: Vertex number out of range.\n"); // 에러 메시지 출력
        return; // 함수 종료
    }
    if (!VER[src] || !VER[dest]) { // 노드의 값이 존재하지 않으면
        printf("Error: Vertex does not exist.\n"); // 에러 메시지 출력
        return; // 함수 종료
    }
    struct Node* newNode = createNode(dest); // 새로운 노드 생성
    newNode->next = graph->adjLists[src]; // 새로운 노드를 src 노드의 다음 노드로 설정
    graph->adjLists[src] = newNode; // src 노드를 새로운 노드로 설정

    newNode = createNode(src); // 새로운 노드 생성
    newNode->next = graph->adjLists[dest]; // 새로운 노드를 dest 노드의 다음 노드로 설정
    graph->adjLists[dest] = newNode; // dest 노드를 새로운 노드로 설정
}

void sortAdjLists(struct Graph* graph) { // 인접 리스트 정렬 함수
    for (int i = 0; i < MAX_VERTEX; i++) { // 모든 노드에 대해
        if (graph->adjLists[i] != NULL) { // 노드가 NULL이 아니면
            struct Node* sorted = NULL; // 정렬된 노드
            struct Node* current = graph->adjLists[i]; // 노드의 다음 노드를 가리키는 포인터
            while (current != NULL) { // 노드가 NULL이 아닐 때까지 (노드가 끝날 때까지)
                struct Node* next = current->next; // 다음 노드를 가리키는 포인터
                if (sorted == NULL || sorted->vertex >= current->vertex) { // 정렬된 노드가 NULL이거나 정렬된 노드의 값이 더 크면
                    current->next = sorted; // 노드를 정렬된 노드로 설정
                    sorted = current; // 노드를 정렬된 노드로 설정
                } else { // 정렬된 노드의 값이 더 작으면
                    struct Node* temp = sorted; // 정렬된 노드를 가리키는 포인터
                    while (temp->next != NULL && temp->next->vertex < current->vertex) temp = temp->next; 
                    // 정렬된 노드의 다음 노드가 NULL이 아니고 정렬된 노드의 다음 노드의 값이 더 작을 때까지 이동
                    current->next = temp->next; // 노드를 정렬된 노드의 다음 노드로 설정
                    temp->next = current; // 노드를 정렬된 노드로 설정
                }
                current = next; // 다음 노드로 이동
            }
            graph->adjLists[i] = sorted; // 노드를 정렬된 노드로 설정
        }
    }
}

void printGraph(struct Graph* graph) { // 그래프 출력 함수
    if (graph == NULL) { // 그래프가 초기화되지 않았으면
        printf("Error: Graph is not initialized.\n"); // 에러 메시지 출력
        return; // 함수 종료
    }
    for (int v = 0; v < MAX_VERTEX; v++) { // 모든 노드에 대해
        struct Node* temp = graph->adjLists[v]; // 노드의 다음 노드를 가리키는 포인터
        printf("Vertex %d -> ", v); // 노드의 값 출력
        while (temp) { // 노드가 NULL이 아닐 때까지
            printf("%d   ", temp->vertex); // 노드의 값 출력
            temp = temp->next; // 다음 노드로 이동
        }
        printf("\n"); // 줄바꿈
    }
}

void DFS(struct Graph* graph, int vertex) { // 깊이 우선 탐색 함수
    struct Node* adjList = graph->adjLists[vertex]; // 노드의 다음 노드를 가리키는 포인터
    struct Node* temp = adjList; // 노드의 다음 노드를 가리키는 포인터

    graph->visited[vertex] = 1; // 노드 방문 여부를 1로 설정
    printf("Visited %d \n", vertex); // 노드 방문 메시지 출력

    while (temp != NULL) { // 노드가 NULL이 아닐 때까지
        int connectedVertex = temp->vertex; // 노드의 값
        if (graph->visited[connectedVertex] == 0) DFS(graph, connectedVertex); // 노드가 방문되지 않았으면 재귀 호출
        temp = temp->next; // 다음 노드로 이동
    }
}

void BFS(struct Graph* graph, int startVertex) { // 너비 우선 탐색 함수
    int queue[MAX_VERTEX]; // 큐 배열
    int front = 0; // 큐의 앞
    int rear = 0; // 큐의 뒤

    graph->visited[startVertex] = 1; // 시작 노드 방문 여부를 1로 설정
    queue[rear++] = startVertex; // 시작 노드를 큐에 삽입

    while (front < rear) { // 큐가 비어있지 않을 때까지
        int currentVertex = queue[front++]; // 큐의 앞에 있는 노드를 가져옴
        printf("Visited %d\n", currentVertex); // 노드 방문 메시지 출력
        struct Node* temp = graph->adjLists[currentVertex]; // 노드의 다음 노드를 가리키는 포인터

        while (temp) { // 노드가 NULL이 아닐 때까지
            int adjVertex = temp->vertex; // 노드의 값
            if (graph->visited[adjVertex] == 0) { // 노드가 방문되지 않았으면
                queue[rear++] = adjVertex; // 큐에 노드 삽입
                graph->visited[adjVertex] = 1; // 노드 방문 여부를 1로 설정
            }
            temp = temp->next; // 다음 노드로 이동
        }
    }
}