#include<iostream>
#include "KdTree.h"

using namespace std;
// all kdtree functions are declared in KdTree.h header file and are implemented in KdTree.cpp file 

int main(){

    // main code here

    // testing  CreateTree function
    vector<vector<float>>ans = {{1,9},{2,3},{4,1},{3,7},{5,4},{6,8},{7,2},{8,8},{7,9},{9,6}};

    TreeNode* root = CreateTree(ans,0);
    bfs(root);
    // end

    // testing NearestNeighbour function
    vector<float> target = {2,4};
    TreeNode* node = findNearestNeighbour(root, target);
    
    cout<<"\nnearest element to target:- ";
    vector<float> nearest_element = node->point;
    for(auto it: nearest_element){
        cout<<it<<" ";
    }
    // end

    return 0;
}