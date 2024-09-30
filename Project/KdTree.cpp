#include "KdTree.h"
#include<vector>
#include<iostream>
#include<queue>
#include <cmath>
#include<algorithm>
using namespace std;

TreeNode* CreateTree(vector<vector<float>>& Points, int depth){


    if(Points.empty())return nullptr;
    int n = Points[0].size();
    int dim = depth%n;

    // sort according to the current dimension
    sort(Points.begin(), Points.end(), [dim](const vector<float>&a, const vector<float>&b){
        return a[dim]<b[dim];
    });

    int median = Points.size()/2;
    vector<vector<float>> left(Points.begin(), Points.begin()+median);
    vector<vector<float>> right(Points.begin()+median+1, Points.end());

    TreeNode* node = new TreeNode(Points[median]);
    node->left = CreateTree(left, depth+1);
    node->right = CreateTree(right, depth+1);

    return node;
}


void bfs(TreeNode* root){
    if(root==nullptr)cerr<<"root not found"<<endl;
    queue<TreeNode*>q;
    q.push(root);
    while(!q.empty()){
        int n = q.size();
        for(int i=0;i<n;i++){
            TreeNode* temp = q.front();
            q.pop();
            cout<<"{";
            for(auto it: temp->point){
                cout<<it<<" ";
            }cout<<"} ";
            if(temp->left!=nullptr)q.push(temp->left);
            if(temp->right!=nullptr)q.push(temp->right);
        }cout<<endl;
    }

}

float squareDistance(const vector<float>& p1 , const vector<float>& p2) {
    float distance = 0;
    if(p1.size()!=p2.size())return 0;
    for(int i=0;i < p1.size();i++) distance +=pow((p1[i] - p2[i]), 2);
    return distance;
}

void findNearest(TreeNode* root, const vector<float>& target, int depth, TreeNode*& bestNode, float& bestDistance) {
    if(root == nullptr) return;
    int n = target.size();
    int dim = depth % n;

    float distance = squareDistance(root->point , target);

    if(distance < bestDistance) {
    bestDistance = distance;
    bestNode = root;
    }

    TreeNode* nextBranch = nullptr;
    TreeNode* otherBranch = nullptr;
    if(target[dim] < root->point[dim]) {
        nextBranch = root->left;
        otherBranch = root->right;
    } 
    else {
    nextBranch = root->right;
    otherBranch = root->left;
    }

    findNearest(nextBranch, target , depth+1, bestNode, bestDistance);

    float planeDistance  = pow(target[dim] - root->point[dim], 2);
    if(planeDistance  < bestDistance) {
        findNearest(otherBranch , target, depth+1, bestNode, bestDistance);
    }
}

TreeNode* findNearestNeighbour(TreeNode* root , const vector<float>& target) {
    if(root == nullptr) return nullptr;

    TreeNode*bestNode = nullptr;
    float bestDistance = numeric_limits<float>::max();

    findNearest(root, target, 0, bestNode, bestDistance);
    return bestNode;
}















