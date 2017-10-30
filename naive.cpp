#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <stdlib.h>
#include <vector>
#include <random>

using namespace std;

void printNeighbours(unordered_map<long, unordered_set<long>> neighbours) {
    for (auto& n : neighbours) {
        cout << n.first << ": ";
        for (auto& s : n.second) {
            cout << s << " ";
        }
        cout << endl;
    }
}

void printWalks(long** walks, int numNodes, int walkPerNode, int walkLength) {
    for (int i = 0; i < numNodes * walkPerNode; i++) {
        for (int j = 0; j < walkLength; j++) {
            cout << walks[i][j] << " ";
        }
        cout << endl;
    }
}

long findNextNode(unordered_map<long, unordered_set<long>> neighbours, long curNode, long prevNode, double p, double q) {
    default_random_engine generator;
    vector<long> curNeighbours;
    curNeighbours.insert(curNeighbours.end(), neighbours[curNode].begin(), neighbours[curNode].end());
    if (prevNode == -1) {
        return curNeighbours[rand() % curNeighbours.size()];
    } else {
        unordered_set<long> prevNeighbours = neighbours[prevNode];
        vector<double> weights(curNeighbours.size());
        for (int i = 0; i < weights.size(); i++) {
            long nextNode = curNeighbours[i];
            if (nextNode == prevNode) {
                weights[i] = 1.0 / p;
            } else if (prevNeighbours.find(nextNode) != prevNeighbours.end()) {
                weights[i] = 1.0;
            } else {
                weights[i] = 1.0 / q;
            }
        }
        discrete_distribution<int> dist(weights.begin(), weights.end());
        return curNeighbours[dist(generator)];
    }
}

long** generateWalks(unordered_map<long, unordered_set<long>> neighbours, int walkPerNode, int walkLength, double p, double q) {
    long** walks = new long*[walkPerNode * neighbours.size()];
    int counter = 0;
    for (auto& neighbour : neighbours) {
        long node = neighbour.first;
        for (int i = 0; i < walkPerNode; i++) {
            walks[counter + i] = new long[walkLength];
            walks[counter + i][0] = node;
            int curNode = node;
            int prevNode = -1;
            for (int j = 1; j < walkLength; j++) {
                int nextNode = findNextNode(neighbours, curNode, prevNode, p, q);
                walks[counter + i][j] = nextNode;
                prevNode = curNode;
                curNode = nextNode;
            }
        }
        counter += walkPerNode;
    }
    return walks;
}

int main(int argc, char* argv[]) {
    string fileName;
    int walkPerNode = 1;
    int walkLength = 10;
    double p = 1;
    double q = 2;

    if (argc == 2) {
        fileName = argv[1];
    } else {
        cerr << "Invalid argument, must provide path to edge list" << endl;
        return 1;
    }

    unordered_map<long, unordered_set<long>> neighbours = {};


    ifstream in(fileName);
    long s, e;
    while (in >> s >> e) {
        if (s == e) {
            cerr << "Loop edge is not supported" << endl;
            return 1;
        } else if (s <= 0 || e <= 0) {
            cerr << "node must be greater than or equal to 0" << endl;
            return 1;
        }
        if (neighbours.find(s) == neighbours.end()) {
            unordered_set<long> emptySet = {};
            neighbours[s] = emptySet;
        }
        if (neighbours.find(e) == neighbours.end()) {
            unordered_set<long> emptySet = {};
            neighbours[e] = emptySet;
        }
        neighbours[s].insert(e);
        neighbours[e].insert(s);
    }

    long** walks = generateWalks(neighbours, walkPerNode, walkLength, p, q);
    printWalks(walks, neighbours.size(), walkPerNode, walkLength);
    // clean up
    for (int i = 0; i < walkPerNode * neighbours.size(); i++) {
        delete[] walks[i];
    }
    delete[] walks;
}





