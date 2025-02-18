#include <iostream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <cmath>
#include <string>

using namespace std;



bool is_goal(const string& board) {
    string goal = "ABCDEFGHIJKLMNO-";
    return board == goal;
}

void printPath(string s){
    for(int i = 0; i<16; i++){
        cout << s[i] << ' ';
        if(i % 4 == 3) cout << endl;
    }
    cout << endl;

}

//BFS code
vector<string> BFS(const string &startboard, int &expandedNodes) {
    if(is_goal(startboard)) {
        return {startboard};
    }

    queue<string> q;                    
    unordered_set<string> visited;            
    unordered_map<string, string> parent;    

    q.push(startboard);
    visited.insert(startboard);
    expandedNodes = 0;

    while(!q.empty()){
        string current = q.front();
        q.pop();
        int move[4] = {1, -1, 4, -4};
        expandedNodes++;
        int blank = current.find('-');
        for(int i = 0; i < 4; i++){
            if(blank + move[i] < 0 || blank + move[i] >= current.size()) continue;
            string next = current;
            swap(next[blank], next[blank + move[i]]);
            if(visited.find(next) == visited.end() ){
                visited.insert(next);
                parent[next] = current;

                if(is_goal(next)){
                    vector<string> path;
                    path.push_back(next);
                    string p = current;
                    while (p != startboard) {
                        path.push_back(p);
                        p = parent[p];
                    }
                    path.push_back(startboard);
                    reverse(path.begin(), path.end());
                    return path;
                }

                q.push(next);
            }
        }
    }
    return {};
}

//A* code
unordered_map<char, pair<int,int>> goalpos;

int heuristics(const string &state) {
    int distance = 0;
    for (int i = 0; i < 16; i++) {
        char c = state[i];
        if (c == '-') continue;
        int row = i / 4;
        int col = i % 4;
        auto &goal = goalpos[c];
        distance += abs(row - goal.first) + abs(col - goal.second);
    }
    return distance;
}

struct cmp{
    bool operator()(const pair<int, string> &a, const pair<int, string> &b) {
        return a.first > b.first;
    }
};

vector<string> AStar(const string &startboard, int &expandedNodes){
    if(is_goal(startboard)){
        return {startboard};
    }
    unordered_map<string, int> g;
    unordered_map<string, string> parent;
    unordered_set<string> visited;


    priority_queue<pair<int,string>, vector<pair<int,string>>, cmp> pq;

    g[startboard] = 0;
    int h = heuristics(startboard);
    pq.push({h, startboard});
    expandedNodes = 0;

    while(!pq.empty()){
        auto [f, current] = pq.top();
        pq.pop();
        if(visited.find(current) != visited.end()) continue;

        visited.insert(current);
        expandedNodes++;

        if(is_goal(current)){
            vector<string> path;
            path.push_back(current);
            while (parent.find(current) != parent.end()) {
                current = parent[current];
                path.push_back(current);
            }
            reverse(path.begin(), path.end());
            return path;
        }
        int move[4] = {1, -1, 4, -4};
        expandedNodes++;
        int blank = current.find('-');
        for(int i = 0; i < 4; i++){
             if(blank + move[i] < 0 || blank + move[i] >= current.size()) continue;
            string next = current;
            swap(next[blank], next[blank + move[i]]);
            if (visited.find(next) != visited.end()) {
                continue;
            }
            int nextG = g[current] + 1;
            if (!g.count(next) || nextG < g[next]) {
                parent[next] = current;
                g[next] = nextG;
                int nextH = heuristics(next);
                int nextF = nextG + nextH;
                pq.push({nextF, next});
            }
        }
    }

    return {};
}



int main() {
    string start_board = "";
    int expandedNodes = 0;
    cout << "Welcome to 16-Puzzle Solver";


    //Read File
    cout << "Enter the file name: ";
    string filename;
    cin >> filename;

    string folder = "./tested_files/";
    string filepath = folder + filename;
    ifstream infile(filepath);
    if(!infile){
        cerr << "Error: Could not open file " << filepath << endl;
        return 1;
    }

    string line;
    int row = 0;
    while (getline(infile, line) && row < 4) {
        istringstream iss(line);
        string token;
        int col = 0;
        while (iss >> token && col < 4) {
            start_board += token;
            col++;
        }
        row++;
    }
    infile.close();

    //Choice for Algorithms

    cout << "\nSelect the algorithm to solve the puzzle:" << endl;
    cout << "1. Uninformed Search (BFS)" << endl;
    cout << "2. Informed Search (A*)" << endl;
    cout << "Enter 1 or 2: ";
    int choice;
    cin >> choice;

    if(choice == 1){
        vector<string> result = BFS(start_board,  expandedNodes);
        if(result.empty()){
            cout << "No solution found.\n";
        }
        else{
            cout << "\nSolution path:\n";
            for (auto &state : result) {
                printPath(state);
            }

            cout << "Number of moves: " << result.size() - 1 << endl;
            cout << "Number of nodes expanded: " << expandedNodes << endl;
        }
    }
    else if(choice == 2) {
        vector<string> result = AStar(start_board,  expandedNodes);
        if(result.empty()){
            cout << "No solution found.\n";
        }
        else{    
            cout << "\nSolution path:\n";
            for (auto &state : result) {
                printPath(state);
            }

            cout << "Number of moves: " << result.size() - 1 << endl;
            cout << "Number of nodes expanded: " << expandedNodes << endl;
        }

    } 
    else {
        cout << "Nothing here! Exit =)))";

    }
    return 0;
}
