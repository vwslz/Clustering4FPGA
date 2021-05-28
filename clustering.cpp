#include <iostream>
#include <cmath>
#define MAX 8
struct Node {
    int id;
    int layer_id;
    int phi_index;
    int z_index;
};
struct Node temp;
void printNode(struct Node input) {
    printf("\n(%d, %d, %d, %d) ", input.id, input.layer_id, input.phi_index, input.z_index);
}
void printIntArray(int* input) {
    for(int i = 0; i < MAX;i++) {
        printf("%d ", input[i]);
    }
}
void printNodeArray(struct Node* input) {
    for(int i = 0; i < MAX;i++) {
        printNode(input[i]);
    }
}
void printNodeMatrix(struct Node** input) {
    for(int i = 0; i < MAX; i++) {
        printf("----");
        printNodeArray(input[i]);
    }
}
void printIntMatrix(int input[MAX][MAX]) {
    for(int i = 0; i < MAX; i++) {
        printf("\n");
        for(int j = 0; j < MAX; j++) {
            printf("%d ", input[i][j]);
        }
    }
}
void copyNode(struct Node* nodeTo, struct Node nodeFrom) {
   nodeTo->id = nodeFrom.id;
   nodeTo->layer_id = nodeFrom.layer_id;
   nodeTo->phi_index = nodeFrom.phi_index;
   nodeTo->z_index = nodeFrom.z_index;
}
void swapNode(struct Node* nodes, int i, int j) {
  copyNode(&temp, nodes[i]);
  copyNode(&(nodes[i]), nodes[j]);
  copyNode(&(nodes[j]), temp);
}
void getAdjancyMatrix(int adjMatrix[MAX][MAX], struct Node* input) {
    for (int i = 0; i < MAX; i++) {
        struct Node node_iter_i;
        copyNode(&node_iter_i, input[i]);
        struct Node node_iter_j;
        for (int j = 0; j < MAX; j++) {
            if (i == j)
                continue;
            copyNode(&node_iter_j, input[j]);
            if (node_iter_i.layer_id == node_iter_j.layer_id
                && abs(node_iter_i.phi_index - node_iter_j.phi_index) <= 2
                && abs(node_iter_i.z_index - node_iter_j.z_index) <= 2) {
                adjMatrix[i][j] = 1;
            }
        }
    }
}

void getAdjancyList(int adjList[MAX][MAX], int adjMatrix[MAX][MAX], struct Node* input) {
    int adjListIndex[MAX] = {0,0,0,0,0,0,0,0};
    for (int i = 0; i < MAX; i++) {
        for (int j = i; j < MAX; j++) {
            if (adjMatrix[i][j] == 1)
                adjList[i][adjListIndex[i]++] = j;
        }
    }
}

// O(|E|)
void getCluster(struct Node output[MAX], struct Node input[MAX], int adjList[MAX][MAX], int marked[MAX]) {
    int ptr_cluster = 0;
    int ptr_next = 0;
    int cluster_id = 0;
    struct Node node_iter;

    while (ptr_cluster < MAX) {
        if (marked[ptr_cluster] == -1) {
            
            cluster_id = ptr_cluster;
            marked[ptr_cluster] = ptr_cluster;
            copyNode(&(output[ptr_next++]), input[ptr_cluster]);
        }
        
        for (int i = 0; i < MAX ; i++) {
            int idx = adjList[ptr_cluster][i];
            if (idx == -1)
                continue;
                
            if (marked[idx] == -1) {
                marked[idx] = cluster_id;
                copyNode(&(output[ptr_next++]), input[idx]);
            }
        }
        ptr_cluster++;
    }

}

int main() {
    struct Node input[MAX] = {
        {.id=0, .layer_id=0, .phi_index=0, .z_index=0},
        {.id=1, .layer_id=0, .phi_index=1, .z_index=1},
        {.id=2, .layer_id=0, .phi_index=3, .z_index=3},
        {.id=3, .layer_id=0, .phi_index=10, .z_index=8},
        {.id=4, .layer_id=0, .phi_index=4, .z_index=4},
        {.id=5, .layer_id=0, .phi_index=11, .z_index=18},
        {.id=6, .layer_id=0, .phi_index=5, .z_index=5},
        {.id=7, .layer_id=0, .phi_index=12, .z_index=10},
    };
    
    printf("\nInput: ");
    printNodeArray(input);
    int adjMatrix[MAX][MAX] = {
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0}
    };
    int adjList[MAX][MAX] = {
        {-1,-1,-1,-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1,-1,-1},
        {-1,-1,-1,-1,-1,-1,-1,-1},
    };
    getAdjancyMatrix(adjMatrix, input);
    printf("\nGet Adjancy Matrix: ");
    printIntMatrix(adjMatrix);
    getAdjancyList(adjList, adjMatrix, input);
    printf("\nGet Adjancy List: ");
    printIntMatrix(adjList);
    struct Node output[MAX];
    int marked[MAX] = {-1,-1,-1,-1,-1,-1,-1,-1};
    getCluster(output, input, adjList, marked);
    printf("\n\nOutput: ");
    printNodeArray(output);
    printf("\nCluster ID:");
    printIntArray(marked);
    return 0;
}
