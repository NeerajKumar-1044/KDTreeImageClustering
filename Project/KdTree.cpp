#include "KdTree.h"
#include<vector>
#include<iostream>
#include<queue>

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
