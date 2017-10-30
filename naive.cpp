#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <stdlib.h>
#include <vector>

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

long findNextNode(unordered_map<long, unordered_set<long>> neighbours, long curNode, long prevNode) {
    unordered_set<long> curNeighbours = neighbours[curNode];
    if (prevNode == -1) {
        unordered_set<long>::iterator itr = curNeighbours.begin();
        int incur = rand() % curNeighbours.size();
        advance(itr, incur);
        return *itr;
    } else {
        unordered_set<long> prevNeighbours = neighbours[prevNode];

    }
}

long** generateWalks(unordered_map<long, unordered_set<long>> neighbours, int walkPerNode, int walkLength) {
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
                int nextNode = findNextNode(neighbours, curNode, prevNode);
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

    long** walks = generateWalks(neighbours, walkPerNode, walkLength);
    printWalks(walks, neighbours.size(), walkPerNode, walkLength);
    // clean up
    for (int i = 0; i < walkPerNode * neighbours.size(); i++) {
        delete[] walks[i];
    }
    delete[] walks;
}





