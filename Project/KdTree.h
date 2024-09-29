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

// function to create KD-Tree
TreeNode* CreateTree(vector<vector<float>>& Points, int depth);


// function to print KD-Tree
void bfs(TreeNode* root);

// to find nearest neighbour
TreeNode* findNearestNeighbour(TreeNode* root , const vector<float>& target);

#endif