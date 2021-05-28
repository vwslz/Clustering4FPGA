
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

void getCluster(struct Node output[MAX], struct Node input[MAX], int adjList[MAX][MAX], int marked[MAX]) {
    
    int ptr_cluster = 0;
    int ptr_next = 0;
    
    while (ptr_cluster < MAX) {
        copyNode(&(output[ptr_next++]), input[ptr_cluster]);
        marked[ptr_cluster] = 1;
        for (int i = 0; i < MAX && adjList[ptr_cluster][i] > -1; i++) {
            if (marked[adjList[ptr_cluster][i]] == 0) {
                copyNode(&(output[ptr_next++]), input[adjList[ptr_cluster][i]]);
                marked[adjList[ptr_cluster][i]] = 1;
            }
        }
        while(marked[ptr_cluster] == 1)
            ptr_cluster++;
    }

}

int main() {
    struct Node input[MAX] = {
        {.id=0, .layer_id=0, .phi_index=1676, .z_index=3266},
        {.id=1, .layer_id=0, .phi_index=1227, .z_index=4582},
        {.id=2, .layer_id=0, .phi_index=4120, .z_index=3450},
        {.id=3, .layer_id=0, .phi_index=1226, .z_index=4581},
        {.id=4, .layer_id=0, .phi_index=1227, .z_index=4586},
        {.id=5, .layer_id=0, .phi_index=1675, .z_index=3266},
        {.id=6, .layer_id=0, .phi_index=1227, .z_index=4581},
        {.id=7, .layer_id=0, .phi_index=1675, .z_index=3267},
    };
    
    // -1, not used, 0, begin of a cluster, 1 used
    int status[MAX] = {-1, -1, -1, -1, -1, -1, -1, -1}; 
    
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
    int marked[MAX] = {0,0,0,0,0,0,0,0};
    getCluster(output, input, adjList, marked);
    printf("\n\nOutput: ");
    printNodeArray(output);
    
    return 0;
}
