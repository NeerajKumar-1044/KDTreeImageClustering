#ifndef KDTREE
#define KDTREE
#include<vector>

using namespace std;

class TreeNode{
    public:
    vector<float> point;
    TreeNode *left;
    TreeNode *right;
    public:
    explicit TreeNode(const vector<float> &arr): point(arr), left(nullptr), right(nullptr) {}
};

TreeNode* CreateTree(vector<vector<float>>& Points, int depth);

void bfs(TreeNode* root);

#endif