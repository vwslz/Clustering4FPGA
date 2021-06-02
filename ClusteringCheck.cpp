#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
#define NUM_FILE 1000
#define PATH_TEST "tests/"

struct Node {
    int id;
    int layer_id;
    int phi_index;
    int z_index;
};
struct Node temp;

int num_hits = 0;

void printNode(struct Node input) {
    printf("\n(%d, %d, %d, %d) ", input.id, input.layer_id, input.phi_index, input.z_index);
}
void printIntArray(int* input, int n) {
    for(int i = 0; i < n;i++) {
        printf("%d ", input[i]);
    }
}
void printNodeArray(struct Node* input, int n) {
    for(int i = 0; i < n;i++) {
        printNode(input[i]);
    }
}

void printIntMatrix(int** input, int n_col, int n_row) {
    for(int i = 0; i < n_row; i++) {
        printf("\n");
        for(int j = 0; j < n_row; j++) {
            printf("%d ", input[i][j]);
        }
    }
}

void printSummary(struct Node* input, struct Node* output, int** adj, int* marked, int num_hits) {
    printf("\nInput: ");
    printNodeArray(input, num_hits);
    printf("\nOutput: ");
    printNodeArray(output, num_hits);
    printf("\nAdj: ");
    printIntMatrix(adj, num_hits, num_hits);
    printf("\nMarked: ");
    printIntArray(marked, num_hits);
}

void copyNode(struct Node* nodeTo, struct Node nodeFrom) {
   nodeTo->id = nodeFrom.id;
   nodeTo->layer_id = nodeFrom.layer_id;
   nodeTo->phi_index = nodeFrom.phi_index;
   nodeTo->z_index = nodeFrom.z_index;
}

void getAdjancyMatrix(int** adjMatrix, struct Node* input, int n) {
    for (int i = 0; i < n; i++) {
        struct Node node_iter_i;
        copyNode(&node_iter_i, input[i]);
        struct Node node_iter_j;
        for (int j = 0; j < n; j++) {
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
void getAdjancyList(int** adjMatrix, struct Node* input, int n) {
    int adjListIndex;
    for (int i = 0; i < n; i++) {
        adjListIndex = 0;
        for (int j = 0; j < n; j++) {
            if (adjMatrix[i][j] == 1)
                adjMatrix[i][adjListIndex++] = j;
        }
        for (int j = adjListIndex; j < n; j++) {
             adjMatrix[i][j] = -1;
        }
    }
}

int appendNeighbour(struct Node* output, struct Node* input, int** adjList, int* marked, int i_node, int i_next, int num_hits) {
    for (int i = 0; i < num_hits ; i++) {
        int idx = adjList[i_node][i];
        if (idx == -1)
            break;
        if (marked[idx] == -1) {
            marked[idx] = marked[i_node];
            // printf("\n - Add neighbour %d at %d", idx, i_next);
            copyNode(&(output[i_next++]), input[idx]);
        }
    }
    return i_next;
}

void getCluster(struct Node* output, struct Node* input, int** adjList, int* marked, int num_hits) {
    int ptr_cluster = 0;
    int ptr_cluster_queue = 0;
    int ptr_next = 0;
    while (ptr_cluster < num_hits) {
        // printf("\n Loop at %d", ptr_cluster);
        if (marked[ptr_cluster] == -1) {
            // printf("\n - Node %d is cluster parent", ptr_cluster);
            marked[ptr_cluster] = ptr_cluster;
            copyNode(&(output[ptr_next++]), input[ptr_cluster]);
            ptr_next = appendNeighbour(output, input, adjList, marked, ptr_cluster, ptr_next, num_hits);
            // printf("\n");
            // printIntArray(marked);
            ptr_cluster_queue = ptr_cluster + 1;
            // printf("\n - %d", ptr_cluster_queue);
            while (ptr_cluster_queue < num_hits && output[ptr_cluster_queue].id != -1) {
                // printf("\n - Iteraste on Node %d", output[ptr_cluster_queue].id);
                ptr_next = appendNeighbour(output, input, adjList, marked, output[ptr_cluster_queue].id, ptr_next, num_hits);
                ptr_cluster_queue++;
                // printf("\n");
                // printIntArray(marked);
            }
        }
        ptr_cluster++;
    }
}

void compare(int* expected, int* marked) {
    
    for (int i = 0; i < num_hits; i++) {
        if (expected[i] != marked[i])
            printf("\nProblem Exist!");
    }
}

void getInts(int* addr_ints, string s, string delimiter) {
    size_t pos = 0;
    string token;
    
    int i_res = 0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        addr_ints[i_res++] = atoi(token.c_str());
        s.erase(0, pos + delimiter.length());
    }
    addr_ints[i_res++] = atoi(s.c_str());
}

void getStrs(string* addr_strs, string s, string delimiter) {
    size_t pos = 0;
    string token;
    
    int i_res = 0;
    int tuple[3];
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        addr_strs[i_res++] = token.c_str();
        s.erase(0, pos + delimiter.length());
    }
    addr_strs[i_res++] = s.c_str();
}

void loadTest(int i) {
    string line;
    ifstream myfile;
    int num_hits;

    string filename_in = PATH_TEST + to_string(i) + "_in.data";
    string filename_out = PATH_TEST + to_string(i) + "_out.data";
    string filename_n = PATH_TEST + to_string(i) + "_n.data";
    myfile.open(filename_n); 

    if (myfile.is_open()) 
    {
        getline (myfile,line);
        num_hits = atoi(line.c_str());
        myfile.close();
        myfile.clear();
    }else cout << "can't open the output file"; 

    string input_str[num_hits];
    int keys[num_hits][3];
    int expected[num_hits];

    myfile.open(filename_in); 
    if (myfile.is_open()) 
    {
        getline (myfile,line);
        getStrs(input_str, line, ";");
        myfile.close();
        myfile.clear();
    }else cout << "can't open the input file"; 
    myfile.open(filename_out); 
    if (myfile.is_open()) 
    {
        getline (myfile,line);
        getInts(expected, line, ",");
        myfile.close();
        myfile.clear();
    }else cout << "can't open the output file"; 
    
    int input_iter[3];
    for (int j = 0; j < num_hits; j++) {
        string line = input_str[j];
        getInts(input_iter, line, ",");
        keys[j][0] = input_iter[0];
        keys[j][1] = input_iter[1];
        keys[j][2] = input_iter[2];
        // printf("\n%d %d %d", input[j][0], input[j][1], input[j][2]);
    }

    /**
    / Initialization
    **/
    // num_hits = 8;
    // int keys[8][3] = {{0, 0, 0}, {0, 3, 3}, {0, 1, 1}, {0, 10, 8}, {0, 5, 5}, {0, 11, 18}, {0, 7, 7}, {0, 12, 10}};
    struct Node input[num_hits];
    struct Node output[num_hits];
    int** adj= new int*[num_hits];
    int marked[num_hits];
    // initialize(keys, input, output, adj, marked);
    for (int i = 0; i < num_hits; i++) {
        input[i].id = i;
        input[i].layer_id = keys[i][0];
        input[i].phi_index = keys[i][1];
        input[i].z_index = keys[i][2];
        output[i].id = -1;
        output[i].layer_id = -1;
        output[i].phi_index = 0;
        output[i].z_index = 0;
        marked[i] = -1;
        adj[i] = new int[num_hits];
        for (int j = 0; j < num_hits; j++) {
            adj[i][j] = 0;
        }
    }

    getAdjancyMatrix(adj, input, num_hits);
    getAdjancyList(adj, input, num_hits);
    
    // printSummary(input, output, adj, marked, num_hits);
    
    getCluster(output, input, adj, marked, num_hits);
    
    // printf("\nCluster ID:");
    // printIntArray(marked, num_hits);
    
    compare(expected, marked);
}

int main ()
{
    int* keys; 
    int* cluster_ids;
    int num_hits;
    string d_int = ",";
    string d_tuple = ";";

    for (int i = 0; i < 1000; i++) {
        loadTest(i);
    }

    return 0;
}
