#include <bits/stdc++.h>
using namespace std;

struct Node {
    int node_id;
    int x, y, R, E, P;
    int F;
};

const int NUM_CLUSTERS = 16;
const int GRID_SIZE = 20;

int num_nodes;
vector<Node> nodes;
vector<vector<Node>> clusters;
unordered_map<int, Node> clusterHeads;

// Function prototypes
bool read_input_file();
double random_double(double min, double max);
vector<Node> generate_random_nodes(int num_nodes);
int assign_cluster_id(const Node& node);
int distance_sqr(int x1, int y1, int x2, int y2);
vector<pair<Node, int>> find_neighbors(const Node& source, const vector<Node>& nodes, int range);
bool dfs(Node node, int destination_id, stack<int>& curr_route, vector<int>& visited);
stack<int> perform_nearest_neighbor_routing(const vector<Node>& nodes, int source_id, int destination_id);
void print_nodes(const vector<Node>& nodes);
void find_and_set_cluster_heads();
void print_cluster_info();

int main() {
    clusters.resize(NUM_CLUSTERS);
	srand(static_cast<unsigned>(time(nullptr)));
	
	
    string mode;

    cout << endl << "Enter Mode (User/Random): ";
    cin >> mode;

    transform(mode.begin(), mode.end(), mode.begin(), ::tolower);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << endl;

    if (mode == "random") {
        int num_nodes = rand() % 91 + 10;
        nodes = generate_random_nodes(num_nodes);
    } else if (mode == "user") {
        read_input_file();
    } else {
        cout << "Please choose from these options: User / Random only" << endl;
        return 0;
    }

    cout << "Input nodes : " << endl;
    print_nodes(nodes);

    for (Node& node : nodes) {
        int cluster_id = assign_cluster_id(node);
        clusters[cluster_id].push_back(node);
    }

    find_and_set_cluster_heads();

    print_cluster_info();
	// Open the "network.txt" file for writing (or create it if it doesn't exist)
		ofstream networkFile("network.txt", ios::out | ios::trunc);

		if (!networkFile) {
			cerr << "Error: Could not open 'network.txt' for writing" << endl;
		}
		
		// Writing into Network.txt file
		networkFile << "Input Nodes :  " <<endl;
		networkFile << nodes.size() << endl;
		for(auto node : nodes){
			networkFile << node.x << " " << node.y << " " << node.R << " " << node.E << " " << node.P << endl;
		}
		networkFile << endl;
		networkFile << endl;
		
		networkFile << "Cluster Head Nodes : " << endl;
		for (int cluster_id = 0; cluster_id < NUM_CLUSTERS; ++cluster_id) {
			if(clusters[cluster_id].size() != 0)
			networkFile << "Cluster " << cluster_id << "'s Head Node is at : (" << clusterHeads[cluster_id].x  << "," << clusterHeads[cluster_id].y << ")" << endl;	
		}
		networkFile << endl;
		networkFile << endl;
		
		// Print the cluster assignment
		networkFile << "Cluster and its residency : " << endl;;
		for (int i = 0; i < NUM_CLUSTERS; ++i) {
			
			if(clusters[i].size()!=0){
				networkFile << "Cluster " << i << " has these nodes(node_Id's) : ";
				for(auto node : clusters[i]){
					networkFile << node.node_id << "  ";
				}
				networkFile << endl;
			}
		}
		networkFile << endl;
		

		// Close the file
		networkFile.close();
	

    do {
        int source_id, destination_id;
        cout << endl << "Enter valid source node ID: ";
        cin >> source_id;

        if (source_id >= nodes.size()) {
            cout << endl << "Source node not found." << endl;
            continue;
        }

        cout << "Enter valid destination node ID: ";
        cin >> destination_id;

        if (destination_id >= nodes.size()) {
            cout << endl << "Destination node not found." << endl;
            continue;
        }

        stack<int> route_stk = perform_nearest_neighbor_routing(nodes, source_id, destination_id);

        if (route_stk.empty()) {
            cout << endl << "There is no path to send data packets from Node " << source_id << " to Node " << destination_id << endl;
        } else {
            cout << endl << "Routing from Node " << source_id << " to Node " << destination_id << ":" << endl;
            vector<int> route;

            while (!route_stk.empty()) {
                int node_id = route_stk.top();
                route_stk.pop();
                route.push_back(node_id);
            }

            reverse(route.begin(), route.end());

            for (const int node_id : route) {
                if (node_id == destination_id) break;
                cout << node_id << " -> ";
            }

            cout << destination_id << endl;
        }

        string preference;
        cout << endl << "Do you want to quit (Yes(Y,Q)/No(other than Y,Q))? : ";
        cin >> preference;

        transform(preference.begin(), preference.end(), preference.begin(), ::tolower);
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (preference == "yes" || preference == "y" || preference == "q" || preference == "quit") {
            break;
        }

    } while (true);

    return 0;
}

bool read_input_file() {
    ifstream inputFile("input.txt");
    if (!inputFile) {
        cerr << "Error: Could not open input file 'input.txt'" << endl;
        return false;
    }

    inputFile >> num_nodes;
    nodes.resize(num_nodes);

    for (int i = 0; i < num_nodes; ++i) {
        nodes[i].node_id = i;
        inputFile >> nodes[i].x >> nodes[i].y >> nodes[i].R >> nodes[i].E >> nodes[i].P;
        nodes[i].F = (0.4 * nodes[i].R) + (0.4 * nodes[i].E) + (0.2 * nodes[i].P);
    }

    inputFile.close();
    return true;
}

double random_double(double min, double max) {
    return min + static_cast<double>(rand()) / static_cast<double>(RAND_MAX / (max - min));
}

vector<Node> generate_random_nodes(int num_nodes) {
    vector<Node> random_nodes;
    for (int i = 0; i < num_nodes; ++i) {
        int x = rand() % (GRID_SIZE + 1);
        int y = rand() % (GRID_SIZE + 1);
        int R = rand() % 8 + 1;
        int E = rand() % 100 + 1;
        int P = rand() % 100 + 1;
        int F = (0.4 * R) + (0.4 * E) + (0.2 * P);
        Node node = {i, x, y, R, E, P, F};
        random_nodes.push_back(node);
    }
    return random_nodes;
}

int assign_cluster_id(const Node& node) {
    int cluster_id = -1;

    // Manually assign cluster IDs based on conditions
    if (node.x >= 0 && node.x < 5 && node.y >= 0 && node.y < 5) {
        cluster_id = 0;
    }
    else if (node.x >= 5 && node.x < 10 && node.y >= 0 && node.y < 5) {
        cluster_id = 1;
    }
    else if (node.x >= 10 && node.x < 15 && node.y >= 0 && node.y < 5) {
        cluster_id = 2;
    }
    else if (node.x >= 15 && node.x <= 20 && node.y >= 0 && node.y < 5) {
        cluster_id = 3;
    }
    else if (node.x >= 0 && node.x < 5 && node.y >= 5 && node.y < 10) {
        cluster_id = 4;
    }
    else if (node.x >= 5 && node.x < 10 && node.y >= 5 && node.y < 10) {
        cluster_id = 5;
    }
    else if (node.x >= 10 && node.x < 15 && node.y >= 5 && node.y < 10) {
        cluster_id = 6;
    }
    else if (node.x >= 15 && node.x <= 20 && node.y >= 5 && node.y < 10) {
        cluster_id = 7;
    }
    else if (node.x >= 0 && node.x < 5 && node.y >= 10 && node.y < 15) {
        cluster_id = 8;
    }
    else if (node.x >= 5 && node.x < 10 && node.y >= 10 && node.y < 15) {
        cluster_id = 9;
    }
    else if (node.x >= 10 && node.x < 15 && node.y >= 10 && node.y < 15) {
        cluster_id = 10;
    }
    else if (node.x >= 15 && node.x <= 20 && node.y >= 10 && node.y < 15) {
        cluster_id = 11;
    }
    else if (node.x >= 0 && node.x < 5 && node.y >= 15 && node.y <= 20) {
        cluster_id = 12;
    }
    else if (node.x >= 5 && node.x < 10 && node.y >= 15 && node.y <= 20) {
        cluster_id = 13;
    }
    else if (node.x >= 10 && node.x < 15 && node.y >= 15 && node.y <= 20) {
        cluster_id = 14;
    }
    else if (node.x >= 15 && node.x <= 20 && node.y >= 15 && node.y <= 20) {
        cluster_id = 15;
    }

    return cluster_id;
}

int distance_sqr(int x1, int y1, int x2, int y2) {
    return ((x2 - x1) * (x2 - x1)) + ((y2 - y1) * (y2 - y1));
}

vector<pair<Node, int>> find_neighbors(const Node& source, const vector<Node>& nodes, int range) {
    vector<pair<Node, int>> neighbors;

    for (const Node& node : nodes) {
        if (node.node_id != source.node_id) {
            int dist = distance_sqr(source.x, source.y, node.x, node.y);
            if (dist <= range * range) {
                neighbors.push_back({node, dist});
            }
        }
    }

    sort(neighbors.begin(), neighbors.end(), [](const pair<Node, int>& a, const pair<Node, int>& b) {
        return a.second < b.second;
    });

    return neighbors;
}

bool dfs(Node node, int destination_id, stack<int>& curr_route, vector<int>& visited) {
    curr_route.push(node.node_id);
    if (node.node_id == destination_id) return true;
    visited[node.node_id] = 1;

    vector<pair<Node, int>> nearest_neighbors = find_neighbors(node, nodes, node.R);

    for (auto node_pair : nearest_neighbors) {
        Node curr_node = node_pair.first;
        if (visited[curr_node.node_id] == 0) {
            bool res = dfs(curr_node, destination_id, curr_route, visited);
            if (res) return res;
        }
    }
    curr_route.pop();
    return false;
}

stack<int> perform_nearest_neighbor_routing(const vector<Node>& nodes, int source_id, int destination_id) {
    stack<int> curr_route;
    vector<int> visited_nodes(nodes.size(), 0);

    Node source_node = nodes[source_id];
    Node destination_node = nodes[destination_id];

    dfs(source_node, destination_id, curr_route, visited_nodes);

    return curr_route;
}

void print_nodes(const vector<Node>& nodes) {
    for (const Node& node : nodes) {
        cout << "Node ID: " << node.node_id << ", (" << node.x << ", " << node.y << "), R=" << node.R
             << ", E=" << node.E << ", P=" << node.P << ", F=" << node.F << endl;
    }
}

void find_and_set_cluster_heads() {
    for (int cluster_id = 0; cluster_id < NUM_CLUSTERS; ++cluster_id) {
        vector<Node>& cluster = clusters[cluster_id];

        if (!cluster.empty()) {
            int max_f = -1;
            for (auto node : cluster) {
                if (node.F > max_f) {
                    max_f = node.F;
                }
            }
            vector<Node> ties;

            for (auto node : cluster) {
                if (node.F == max_f) {
                    ties.push_back(node);
                }
            }

            if (ties.size() > 1) {
                int cluster_center_x = ((ties[0].x) / 5) * 5 + 2.5;
                int cluster_center_y = ((ties[0].y) / 5) * 5 + 2.5;

                int min_distance = INT_MAX;
                Node min_dist_node;
                for (auto node : ties) {
                    if (distance_sqr(cluster_center_x, cluster_center_y, node.x, node.y) < min_distance)
                        min_dist_node = node;
                }

                clusterHeads[cluster_id] = min_dist_node;
            } else {
                clusterHeads[cluster_id] = ties[0];
            }
        }
    }
}

void print_cluster_info() {
    cout << endl << "Cluster Heads and their location: " << endl;
    for (int cluster_id = 0; cluster_id < NUM_CLUSTERS; ++cluster_id) {
        if (clusters[cluster_id].size() != 0)
            cout << "Cluster " << cluster_id << "'s Head Node is at : (" << clusterHeads[cluster_id].x << ","
                 << clusterHeads[cluster_id].y << ")" << endl;
    }

    cout << endl << "Cluster assignments : " << endl;
    for (int i = 0; i < NUM_CLUSTERS; ++i) {
        if (clusters[i].size() != 0) {
            cout << "Cluster " << i << " has these nodes(node_Id's) : ";
            for (auto node : clusters[i]) {
                cout << node.node_id << "  ";
            }
            cout << endl;
        }
    }
	
}
