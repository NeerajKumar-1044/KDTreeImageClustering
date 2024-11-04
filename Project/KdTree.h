#ifndef KDTREE
#define KDTREE
#include <vector>
#include <iostream>
#include <queue>
#include <cmath>
#include <algorithm>

using namespace std;

class Cluster {
public:
    vector<float> centroid;
    vector<Cluster*> members;
    float distance;

    explicit Cluster(vector<float> val, vector<Cluster*> m = {}, float dis = 0.0)
        : centroid(val), members(m), distance(dis) {}
};

class TreeNode {
public:
    Cluster* point;
    TreeNode* left;
    TreeNode* right;

    explicit TreeNode(Cluster* cluster) : point(cluster), left(nullptr), right(nullptr) {}
};

TreeNode* CreateTree(vector<Cluster*>& clusters, int depth = 0);
Cluster* findNearestNeighbour(TreeNode* root, const vector<float>& target);

void bfs(TreeNode* root);
bool DeleteKdNode(TreeNode* &root, const vector<float>& target, int depth);
void InsertKdNode(TreeNode* root, Cluster* cluster);

#endif
