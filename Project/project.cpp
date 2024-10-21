#include<iostream>
#include "KdTree.h"
#include <queue>
#include <algorithm>

using namespace std;

vector<float> CalculateCentroid(vector<float>&a, vector<float>&b){
    vector<float> centroid;
    for(int i=0; i<a.size(); i++){
        centroid.push_back((a[i]+b[i])/2);
    }
    return centroid;
}

Cluster* hierarchicalClustering(TreeNode* kdTree, vector<Cluster*> feature) {

    queue<Cluster*>q;
    for(auto it: feature) q.push(it);

    while(q.size()>1){
        Cluster* c1 = q.front();
        q.pop();
        if(DeleteKdNode(kdTree, c1->centroid, 0)){
            Cluster* c2 = findNearestNeighbour(kdTree, c1->centroid);
            if(!DeleteKdNode(kdTree, c2->centroid, 0))continue;
            vector<float> centroid = CalculateCentroid(c1->centroid, c2->centroid);
            Cluster* newCluster = new Cluster(centroid);
            newCluster->members.push_back(c1);
            newCluster->members.push_back(c2);
            q.push(newCluster);
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
    int numClusters = 3;
    // Cluster* cluster = hierarchicalClustering(kdTree, feature);
    bfs(kdTree);
    return 0;
}
