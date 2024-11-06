#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <string>
#include <set>
#include <unordered_map>
#include "KdTree.cpp"
#include "KdTree.h"
#include <cmath> 
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

unordered_map<string, vector<float>> features;
set<string> final;
string target;
int num;

vector<float> CalculateCentroid(vector<float>&a, vector<float>&b){
    vector<float> centroid;
    for(int i=0; i<a.size(); i++){
        centroid.push_back((a[i]+b[i])/2);
    }
    return centroid;
}

bool areVectorsEqual(const vector<float>& a, const vector<float>& b, float epsilon = 1e-5) {
    if (a.size() != b.size()) return false;

    for (size_t i = 0; i < a.size(); ++i) {
        if (std::fabs(a[i] - b[i]) > epsilon) {
            return false;
        }
    }
    return true;
}

string getKeyFromValue(const vector<float>& value, float epsilon = 1e-5) {
    for (const auto& pair : features) {
        if (areVectorsEqual(pair.second, value, epsilon)) {
            return pair.first; 
        }
    }
    return ""; 
}

bool check(Cluster* cluster) {
    if (cluster->members.size() == 0) {
        return getKeyFromValue(cluster->centroid) == target;
    }
    for (auto it : cluster->members) {
       if(check(it)) return true;
    }
    return false;
}

void func(Cluster* cluster) {
    if (final.size()==num) return;
    if (cluster->members.size() == 0) {
        if(getKeyFromValue(cluster->centroid) != target){
        final.insert(getKeyFromValue(cluster->centroid));
        }
        return;
    }
    for (auto it : cluster->members) {
        func(it);
        if(final.size()==num) break;
    }
}

void print(Cluster* c, int depth = 0) {
    for (int i = 0; i < depth; ++i) {
        cout << "    "; // Add 4 spaces for each level of depth
    }

    if (c->members.empty()) {
        // Leaf cluster, print the centroid
        cout << "Centroid: ";
        for (float value : c->centroid) {
            cout << value << " ";
        }
        cout << endl;
    } else {
        cout << "centroid :" ;
        for (float value : c->centroid) {
            cout << value << " ";
        }
        cout << endl;
        cout << "Members :" << endl;
        // Print members with increased depth
        for (Cluster* member : c->members) {
            print(member, depth + 1);
        }
    }
}

void removeClusterFromQueue(queue<Cluster*>& q, Cluster* clusterToRemove) {
    queue<Cluster*> tempQueue;

    // Transfer elements to tempQueue, skipping the cluster to remove
    while (!q.empty()) {
        Cluster* currentCluster = q.front();
        q.pop();

        // Check if the current cluster is the one to remove
        if (currentCluster != clusterToRemove) {
            tempQueue.push(currentCluster);
        }
    }

    // Replace the original queue with the filtered one
    q = tempQueue;
}

Cluster* hierarchicalClustering(TreeNode* kdTree, vector<Cluster*> feature) {

    queue<Cluster*>q;
    for(auto it: feature) q.push(it);

    while(q.size()>1){
        Cluster* c1 = q.front();
        q.pop();
        if(DeleteKdNode(kdTree, c1->centroid, 0)){
            Cluster* c2 = findNearestNeighbour(kdTree, c1->centroid);
            removeClusterFromQueue(q, c2);
            if(!DeleteKdNode(kdTree, c2->centroid, 0))continue;
            vector<float> centroid = CalculateCentroid(c1->centroid, c2->centroid);
            Cluster* newCluster = new Cluster(centroid);
            newCluster->members.push_back(c1);
            newCluster->members.push_back(c2);
            q.push(newCluster);
            // print(newCluster);
            // cout << endl;
            if( check(newCluster)){
            func(newCluster);            
           }
            InsertKdNode(kdTree, newCluster);
        }
        else continue;
    }

    if(q.size()==1){
        return q.front();
    }
    return nullptr;
}

int main() {
ifstream inputFile("output.json");
    if (!inputFile.is_open()) {
        cerr << "Could not open the file: output.json" << endl;
        return -1;
    }
    
    json data;
    inputFile >> data;
   

    // Populate features map from JSON data
    for (auto it = data.begin(); it != data.end(); ++it) {
        string imageID = it.key();
        vector<float> featureVector = it.value().get<vector<float>>();
        features[imageID] = featureVector;
    }

    ifstream inputOutputFile("input.json");
    if (!inputOutputFile.is_open()) {
        cerr << "Could not open the file: input.json" << endl;
        return -1;
    }

    // Parse JSON data
    json ioData;
    inputOutputFile >> ioData;

    target = ioData["target"].get<string>();
    num = ioData["num"].get<int>();
    
    cout << "Target: " << target << endl;
    cout << "Num: " << num << endl;
    vector<Cluster*> feature;
    for(auto it:features){
        feature.push_back(new Cluster(it.second));
    }
    TreeNode* kdTree = CreateTree(feature, 0);
    Cluster* cluster = hierarchicalClustering(kdTree, feature);
    json jsonData;
    jsonData["numbers"] = final; 
    ofstream outFile("input.json");
    if (!outFile.is_open()) {
        cerr << "Could not open the file for writing" << endl;
        return -1;
    }

    outFile << jsonData.dump(4); // Pretty-print with 4-space indentation
    outFile.close();

    cout << "Array successfully stored in data.json" << endl;
    return 0;
}
