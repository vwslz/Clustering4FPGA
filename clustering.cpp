
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

void display(struct Node* input, int* conn) {
    for(int i = 0; i < MAX;i++) {
        printNode(input[i]);
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

void getCluster(struct Node* nodes, int* status) {
    // printf("\n----Get Cluster----");
    
    printf("\nStatus:");
    printIntArray(status);
    int ptr_cluster = 0;
    int ptr_next = 0;
    struct Node node_iter;
    
    
    while (ptr_cluster < MAX) {
        printf("\n---------------------");
        copyNode(&node_iter, nodes[ptr_cluster]);
        printf("\nFind neighbours for %d at %d", node_iter.id, ptr_cluster);
        if (status[node_iter.id] == -1) {
            printf("\nMark %d as begin of cluster", node_iter.id);
            status[node_iter.id] = 1;
            ptr_next++;
        }
        printNode(node_iter);
        
        for (int i = ptr_next; i < MAX ; i++) {
            struct Node neighbour_iter;
            copyNode(&neighbour_iter, nodes[i]);
            // printf("\n Check neighbours at %d", i);
            // printNode(neighbour_iter);
            if (status[neighbour_iter.id] == -1
                && node_iter.layer_id == neighbour_iter.layer_id
                && abs(node_iter.phi_index - neighbour_iter.phi_index) <= 2
                && abs(node_iter.z_index - neighbour_iter.z_index) <= 2) {
                printf("\n--> %d add to %d", neighbour_iter.id, ptr_next);
                
                status[neighbour_iter.id] = 0;
                swapNode(nodes, i, ptr_next++);
            }
            if (ptr_next == MAX)
                return;
        }
        
        printf("\nStatus: ");
        printIntArray(status);
        ptr_cluster++;
        printf("\nindexes: ");
        printf("\n%d %d:", ptr_cluster, ptr_next);
    }

}

int main() {
    struct Node input[] = {
        {.id=0, .layer_id=0, .phi_index=1675, .z_index=3266},
        {.id=1, .layer_id=0, .phi_index=1675, .z_index=3267},
        {.id=2, .layer_id=0, .phi_index=1227, .z_index=4582},
        {.id=3, .layer_id=0, .phi_index=4120, .z_index=3450},
        {.id=4, .layer_id=0, .phi_index=1676, .z_index=3266},
        {.id=5, .layer_id=0, .phi_index=1226, .z_index=4581},
        {.id=6, .layer_id=0, .phi_index=1227, .z_index=4586},
        {.id=7, .layer_id=0, .phi_index=1227, .z_index=4581},
    };
    
    // -1, not used, 0, begin of a cluster, 1 used
    int status[MAX] = {-1, -1, -1, -1, -1, -1, -1, -1}; 
    
    printf("\nInput: ");
    display(input, status);

    getCluster(input, status);
    printf("\n\nOutput: ");
    display(input, status);
    
    printf("\Status: ");
    printIntArray(status);
    return 0;
}