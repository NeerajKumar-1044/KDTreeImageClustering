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

using namespace std;

vector<float> CalculateCentroid(vector<float>&a, vector<float>&b){
    vector<float> centroid;
    for(int i=0; i<a.size(); i++){
        centroid.push_back((a[i]+b[i])/2);
    }
    return centroid;
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
             print(newCluster);
             cout << endl;
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

    // vector<vector<float>> features;
    vector<vector<float>>features = {{1,9},{2,3},{4,1},{3,7},{5,4},{6,8},{7,2},{8,8},{7,9},{9,6}};

    // Build KD-Tree from features
    vector<Cluster*> feature;
    for(auto it: features){
        Cluster* temp = new Cluster(it);
        feature.push_back(temp);
    }
    TreeNode* kdTree = CreateTree(feature, 0);
    // Perform hierarchical clustering
    Cluster* cluster = hierarchicalClustering(kdTree, feature);
    // bfs(kdTree);
    // cout<<"KdTree after deletion"<<endl;
    // if(DeleteKdNode(kdTree, {8,8}, 0)) cout<<"Deleted"<<endl;
    // else cout<<"Not Deleted"<<endl;
    // bfs(kdTree);
    return 0;
}
