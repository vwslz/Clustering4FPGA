#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;
#define NUM_FILE 1000
#define PATH_TEST "tests/"

struct Node {
    int id;
    int layer_id;
    double phi_index_phi; // double phi
    double z_index_r; // double r
    double x; 
    double y; 
    double z;
    int n_nodes;
};

int num_hits = 0;
struct Node temp;

void printNode(struct Node input) {
    printf("\n(%d, %d, %.3f, %.3f, %.3f, %.3f, %.3f, %d) ", 
        input.id, input.layer_id, input.phi_index_phi, input.z_index_r,
        input.x, input.y, input.z, input.n_nodes
        );
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

void printNodesWithSelectedFeatures(struct Node* input, int n) {
    printf("\nr, phi, z, n_hits: ");
    for (int i = 0; i < n; i++) {
        printf("\n(%d, %.3f, %.3f, %.3f, %d) ", 
            input[i].id, input[i].z_index_r, input[i].phi_index_phi, input[i].z, input[i].n_nodes
        );
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
   nodeTo->phi_index_phi = nodeFrom.phi_index_phi;
   nodeTo->z_index_r = nodeFrom.z_index_r;
   nodeTo->x = nodeFrom.x;
   nodeTo->y = nodeFrom.y;
   nodeTo->z = nodeFrom.z;
   nodeTo->n_nodes = nodeFrom.n_nodes;
}

void swapNode(struct Node* nodes, int i, int j) {
  copyNode(&temp, nodes[i]);
  copyNode(&(nodes[i]), nodes[j]);
  copyNode(&(nodes[j]), temp);
}

void resetNodeIndex(struct Node* nodes, int n) {
  for (int i = 0; i < n; i++) {
      nodes[i].id = i;
  }
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
                && abs(node_iter_i.phi_index_phi - node_iter_j.phi_index_phi) <= 2
                && abs(node_iter_i.z_index_r - node_iter_j.z_index_r) <= 2) {
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

void getCluster(struct Node* output, int* root_index, struct Node* input, int** adjList, int* marked, int num_hits) {
    int ptr_cluster = 0;
    int ptr_cluster_queue = 0;
    int ptr_next = 0;
    int ptr_root = 0;
    while (ptr_cluster < num_hits) {
        // printf("\n Loop at %d", ptr_cluster);
        if (marked[ptr_cluster] == -1) {
            // printf("\n - Node %d is cluster parent", ptr_cluster);
            marked[ptr_cluster] = ptr_cluster;
            root_index[ptr_root++] = ptr_next;
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

int getNextRootIter(int* root_index, int i, int num_hits) {
    int res = root_index[i+1];
    if (res == -1)
        res = num_hits;
}

void initializeIntArray(int* input, int n, int init_val) {
    for (int i = 0; i < n; i++) {
        input[i] = init_val;
    }
}

int getMedianInArray(int* cnts, int num) {
    int idx_mid = num / 2;
    int idx = 0;
        
    while (idx_mid >= 0) {
        if (idx_mid < cnts[idx]) {
            return idx;
        }
        else {
            idx_mid -= cnts[idx++];
        }
    }
    printf("\nDid not find Medium...");
    return -1;
}

void getClusteredNodes(struct Node* output, int* root_index, int num_hits, int num_layer) {
    int root_iter = root_index[0];
    int root_iter_next = root_index[1];
    int node_iter = 0;

    int layer_cnt_in_cluster[num_layer];
    initializeIntArray(layer_cnt_in_cluster, num_layer, 0);

    int idx_mid_layer = 0;
    int idx_layer = 0;
    int num_nodes = 0;

    while (node_iter < num_hits) {
        if (node_iter == root_index[root_iter]) {
            // printf("\nAt %d (x, y, z): (%.3f %.3f %.3f)", root_iter, output[node_iter].x, output[node_iter].y, output[node_iter].z);
            output[root_iter].id = root_iter;
            output[root_iter].x = output[node_iter].x;
            output[root_iter].y = output[node_iter].y;
            output[root_iter].z = output[node_iter].z;
            output[root_iter].phi_index_phi = -1;
            output[root_iter].z_index_r = -1;
            layer_cnt_in_cluster[output[node_iter].layer_id]++;
        }
        else {
            // printf("\nAt %d add (x, y, z): (%.3f %.3f %.3f)", root_iter, output[node_iter].x, output[node_iter].y, output[node_iter].z);
            output[root_iter].x += output[node_iter].x;
            output[root_iter].y += output[node_iter].y;
            output[root_iter].z += output[node_iter].z;
            layer_cnt_in_cluster[output[node_iter].layer_id]++;
        }

        node_iter++;
        if (node_iter == root_index[root_iter+1]) {
            // printf("\nCurrently at root %d", root_iter);
            
            num_nodes = root_index[root_iter+1] - root_index[root_iter];
            output[root_iter].x /=  num_nodes;
            output[root_iter].y /=  num_nodes;
            output[root_iter].z /=  num_nodes;
            output[root_iter].n_nodes =  num_nodes;

            // printIntArray(layer_cnt_in_cluster, num_layer);

            idx_mid_layer = num_nodes / 2;
            idx_layer = 0;
            output[root_iter].layer_id = getMedianInArray(layer_cnt_in_cluster, num_nodes);
            initializeIntArray(layer_cnt_in_cluster, num_layer, 0);
            root_iter++;
        }
    }

    num_nodes = num_hits - root_index[root_iter];
    // printf("\nnum_nodes: %d", num_nodes);
    output[root_iter].x /=  num_nodes;
    output[root_iter].y /=  num_nodes;
    output[root_iter].z /=  num_nodes;
    output[root_iter].n_nodes =  num_nodes;

    // printf("\nCurrently at root %d", root_iter);
    // printf("\n");
    // printIntArray(layer_cnt_in_cluster, num_layer);
    idx_mid_layer = num_nodes / 2;
    idx_layer = 0;
    output[root_iter].layer_id = getMedianInArray(layer_cnt_in_cluster, num_nodes);

    for (int i = 0; i <= root_iter; i++) {
        // printf("\nx y r phi: %.3f %.3f %.3f %.3f", output[i].x, output[i].y, output[i].z_index_r, output[i].phi_index_phi);
        output[i].z_index_r = sqrt(output[i].x * output[i].x + output[i].y * output[i].y);
        output[i].phi_index_phi = atan2(output[i].y, output[i].x);
        // printf("\nx y r phi: %.3f %.3f %.3f %.3f", output[i].x, output[i].y, output[i].z_index_r, output[i].phi_index_phi);
    }
}

void sortByLayer(struct Node* input, int* layer_begin_idx, int* layer_cnt, int num_layers, int num_clusters) {
    for (int i = 0; i < num_clusters; i++) {
        layer_cnt[input[i].layer_id]++;
    }

    int layer_iter[num_layers];
    layer_iter[0] = 0;
    layer_begin_idx[0] = 0;
    layer_begin_idx[num_layers] = num_clusters;
    for (int i = 0; i < num_layers; i++) {
        if (i > 0)
            layer_begin_idx[i] = layer_begin_idx[i-1] + layer_cnt[i-1];

        layer_iter[i] = layer_begin_idx[i];
        for (int j = 0; j < layer_cnt[i]; j++) {
            if (input[layer_begin_idx[i] + j].layer_id != i)
                break;
            layer_iter[i]++;
        }
    }
    // printf("\n");
    // printIntArray(layer_iter, num_layers);
    // printf("\n");
    // printIntArray(layer_begin_idx, num_layers + 1);

    for (int i = 0; i < num_layers; i++) {
        for (int j = layer_iter[i]; j < layer_begin_idx[i + 1]; j++) {
            // printf("\nNode %d at layer %d: ", j, input[j].layer_id);
            while (input[j].layer_id != i) {
                // printf("\nswap %d %d", j, layer_iter[input[j].layer_id]);
                swapNode(input, j, layer_iter[input[j].layer_id]++);
            }
        }
    }
}

double getDphi(double phi_0, double phi_1) {
    double dphi = phi_1 - phi_0;
    if (dphi > M_PI)
        return dphi - 2 * M_PI;
    else if (dphi < -M_PI)
        return dphi + 2 * M_PI;
    else   
        return dphi;
}

int getSegments(int* edge_pair[2], struct Node* output, int* indexes_layers, int layer_0, int layer_1, double phi_slope_max, double z0_max, int edge_pair_iter) {
    for (int i = indexes_layers[layer_0]; i < indexes_layers[layer_0 + 1]; i++) {
        for (int j = indexes_layers[layer_1]; j < indexes_layers[layer_1 + 1]; j++) {
            double dphi = getDphi(output[i].phi_index_phi, output[j].phi_index_phi);
            double dz = output[j].z - output[i].z;
            double dr = output[j].z_index_r - output[i].z_index_r;
            double phi_slope = dphi / dr;
            double z0 = output[i].z - output[i].z_index_r * dz / dr;
            
            // printf("\ni j dphi dz dr phi_slope z0: %d %d %.3f %.3f %.3f %.3f %.3f", i, j, dphi, dz, dr, phi_slope, z0);
            
            if ((fabs(phi_slope) < phi_slope_max) && (fabs(z0) < z0_max)) {
                // if (layer_0 == 1 && layer_1 == 3 && fabs(phi_slope) > 0.09) {
                //     printf("\ni j phi_slope phi_slope_max z0: %d %d %f %.3f %.3f", i, j, fabs(phi_slope), phi_slope_max, fabs(z0));
                //     printf("\nr phi z: %.3f %.3f %.3f", output[i].z_index_r, output[i].phi_index_phi, output[i].z);
                //     printf("\nr phi z: %.3f %.3f %.3f", output[j].z_index_r, output[j].phi_index_phi, output[j].z);
                    
                // }
                // if (layer_0 == 1 && layer_1 == 3)
                //     printf("\ni j dphi dz dr phi_slope z0: %d %d %.3f %.3f %.3f %.3f %.3f", i, j, dphi, dz, dr, phi_slope, z0);
                edge_pair[edge_pair_iter][0] = i;
                edge_pair[edge_pair_iter][1] = j;
                edge_pair_iter++;
            }
        }
    }
    // printf("\n----> %d", edge_pair_iter);
    return edge_pair_iter;
}

int getEdgeIndex(int** edge_pair, struct Node* output, int* indexes_layers, int num_layers) {
    int edge_pair_iter = 0;
    for (int i = 0; i < num_layers * 2; i++) {
        for (int j = i + 1; j < ((int)(i/2) + 2) * 2 && j < num_layers * 2; j++) {
            // printf("\n%d %d", i, j);
            edge_pair_iter = getSegments(edge_pair, output, indexes_layers, i, j, 0.1, 200, edge_pair_iter);
        }
    }
    return edge_pair_iter;
}

void featureScale(struct Node* input, int n) {
    for (int i = 0; i < n; i++) {
        input[i].phi_index_phi /= M_PI;
        input[i].z_index_r /= (double)10;
        input[i].z /= (double)10;
    }
}

void compare(int* expected, int* marked) {
    for (int i = 0; i < num_hits; i++) {
        if (expected[i] != marked[i])
            printf("\nProblem Exist!");
    }
}

void getNums(int* addr_nums, string s, string delimiter) {
    size_t pos = 0;
    string token;
    
    int i_res = 0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        if (i_res>2)
            addr_nums[i_res++] = stod(token);
        else
            addr_nums[i_res++] = atoi(token.c_str());
        s.erase(0, pos + delimiter.length());
    }
    addr_nums[i_res++] = atoi(s.c_str());
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

void getNodes(Node* node, string s, string delimiter) {
    size_t pos = 0;
    string token;
    
    int i_res = 0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        if (i_res == 0)
            node->layer_id = atoi(token.c_str());
        else if (i_res == 1)
            node->phi_index_phi = atoi(token.c_str());
        else if (i_res == 2)
            node->z_index_r = atoi(token.c_str());
        else if (i_res == 3)
            node->x = stod(token);
        else if (i_res == 4)
            node->y = stod(token);
        else
            printf("Too many input for getNodes()");
        i_res++;
        s.erase(0, pos + delimiter.length());
    }
    node->z = stod(s);
}

void getStrs(string* addr_strs, string s, string delimiter) {
    size_t pos = 0;
    string token;
    
    int i_res = 0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        addr_strs[i_res++] = token.c_str();
        s.erase(0, pos + delimiter.length());
    }
    addr_strs[i_res++] = s.c_str();
}

void loadTest(int idx) {
    string line;
    ifstream myfile;
    int num_hits;
    int expected_num_edge_pairs;

    string filename_in = PATH_TEST + to_string(idx) + "_in.data";
    string filename_out = PATH_TEST + to_string(idx) + "_out.data";
    string filename_n = PATH_TEST + to_string(idx) + "_n.data";
    string filename_n_ep = PATH_TEST + to_string(idx) + "_n_ep.data";

    myfile.open(filename_n); 
    if (myfile.is_open()) 
    {
        getline (myfile,line);
        num_hits = atoi(line.c_str());
        myfile.close();
        myfile.clear();
    }else cout << "can't open the n file"; 

    myfile.open(filename_n_ep); 
    if (myfile.is_open()) 
    {
        getline (myfile,line);
        expected_num_edge_pairs = atoi(line.c_str());
        myfile.close();
        myfile.clear();
    }else cout << "can't open the nep file"; 

    string input_str[num_hits];
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

    /**
    / Initialization
    **/
    // num_hits = 8;
    struct Node input[num_hits];
    struct Node output[num_hits];
    int** adj = new int*[num_hits];
    int marked[num_hits];
    int rootIndex[num_hits];
    int num_layers = 3;
    int layer_cnt[num_layers * 2];
    int layer_begin_idx[num_layers * 2 + 1];
    // (2, 1000)
    int** edge_pair = new int*[1000];;
    // initialize(keys, input, output, adj, marked);
    for (int i = 0; i < num_hits; i++) {
        string line = input_str[i];
        getNodes(&(input[i]), line, ",");
        input[i].id = i;
        output[i].id = -1;
        output[i].layer_id = -1;
        output[i].phi_index_phi = 0;
        output[i].z_index_r = 0;
        output[i].x = 0;
        output[i].y = 0;
        output[i].z = 0;
        output[i].n_nodes = 0;
        marked[i] = -1;
        rootIndex[i] = -1;
        adj[i] = new int[num_hits];
        for (int j = 0; j < num_hits; j++) {
            adj[i][j] = 0;
        }
    }

    for (int i = 0; i < 1000; i++) {
        edge_pair[i] = new int[2];
        edge_pair[i][0] = 0;
        edge_pair[i][1] = 0;
    }

    // printNodeArray(input, num_hits);

    getAdjancyMatrix(adj, input, num_hits);
    getAdjancyList(adj, input, num_hits);
    // printSummary(input, output, adj, marked, num_hits);

    getCluster(output, rootIndex, input, adj, marked, num_hits);
    // printf("\nCluster ID:");
    // printNodeArray(output, num_hits);
    // printIntArray(marked, num_hits);
    // printf("\n");
    // printIntArray(rootIndex, num_hits);
    
    compare(expected, marked);

    int num_clusters = 0;
    for (int i = 0; i < num_hits; i++, num_clusters++) {
        if (rootIndex[i] == -1)
            break;
    }
    // printf("\nNumber of clusters: %d", num_clusters);
    
    getClusteredNodes(output, rootIndex, num_hits, num_layers*2);
    // printNodeArray(output, num_clusters);
    
    /**Two sublayers for each layer
    * The first sublayer can connect to the next sublayer
    * Both sublayers can connect to the sublayers in the next layer
    */
    for (int i = 0; i < num_layers*2; i++) {
        layer_cnt[i] = 0;
        layer_begin_idx[i] = 0;
    }
    layer_begin_idx[num_clusters] = 0;

    sortByLayer(output, layer_begin_idx, layer_cnt, num_layers*2, num_clusters);
    resetNodeIndex(output, num_clusters);

    // printNodeArray(output, num_clusters);
    // printf("\n");
    // printIntArray(layer_begin_idx, num_layers*2+1);

    int num_edge_pairs = getEdgeIndex(edge_pair, output, layer_begin_idx, num_layers);
    
    if (abs(num_edge_pairs - expected_num_edge_pairs) > 2)
        printf("\nCheck Event %d: %d v.s. %d", idx, num_edge_pairs, expected_num_edge_pairs);

    // Add number of nodes
    featureScale(output, num_clusters);
    // printNodesWithSelectedFeatures(output, num_clusters);
}

int main ()
{
    int* keys; 
    int* cluster_ids;
    int num_hits;
    string d_int = ",";
    string d_tuple = ";";

    printf("Begin Checking...");
    for (int i = 0; i < 1000; i++) {
        loadTest(i);
    }

    return 0;
}
