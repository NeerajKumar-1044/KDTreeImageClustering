#include "KdTree.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <cmath>
#include <vector>

// this code is working fine
float squareDistance(const vector<float>& p1, const vector<float>& p2) {
    float distance = 0;
    if (p1.size() != p2.size()) return 0;
    for (int i = 0; i < p1.size(); i++) distance += pow((p1[i] - p2[i]), 2);
    return distance;
}


// this code is working fine
TreeNode* CreateTree(vector<Cluster*>& clusters, int depth) {
    if (clusters.empty()) return nullptr;
    int n = clusters[0]->centroid.size();
    int dim = depth % n;

    // Sort clusters based on their centroids in the current dimension
    sort(clusters.begin(), clusters.end(), [dim](Cluster* a, Cluster* b) {
        return a->centroid[dim] < b->centroid[dim];
    });

    int median = clusters.size() / 2;
    vector<Cluster*> left(clusters.begin(), clusters.begin() + median);
    vector<Cluster*> right(clusters.begin() + median + 1, clusters.end());

    TreeNode* node = new TreeNode(clusters[median]);
    node->left = CreateTree(left, depth + 1);
    node->right = CreateTree(right, depth + 1);

    return node;
}



void findNearest(TreeNode* root, const vector<float>& target, int depth, TreeNode*& bestNode, float& bestDistance) {
    if (root == nullptr) return;

    int n = target.size();
    int dim = depth % n;

    float distance = squareDistance(root->point->centroid, target);
    if (distance < bestDistance) {
        bestDistance = distance;
        bestNode = root;
    }

    TreeNode* nextBranch = nullptr;
    TreeNode* otherBranch = nullptr;

    if (target[dim] < root->point->centroid[dim]) {
        nextBranch = root->left;
        otherBranch = root->right;
    } else {
        nextBranch = root->right;
        otherBranch = root->left;
    }

    findNearest(nextBranch, target, depth + 1, bestNode, bestDistance);

    float planeDistance = pow(target[dim] - root->point->centroid[dim], 2);
    if (planeDistance < bestDistance) {
        findNearest(otherBranch, target, depth + 1, bestNode, bestDistance);
    }
}



Cluster* findNearestNeighbour(TreeNode* root, const vector<float>& target) {
    if (root == nullptr) return nullptr;
    TreeNode* bestNode = nullptr;
    float bestDistance = numeric_limits<float>::max();
    findNearest(root, target, 0, bestNode, bestDistance);
    return bestNode->point;
}


// this code is working fine
bool floatCompare(const vector<float>& a, const vector<float>& b) {
    for (int i = 0; i < a.size(); i++) {
        if (fabs(a[i] - b[i]) > 1e-5) return false;
    }
    return true;
}


// this code is working fine
TreeNode* findMinRec(TreeNode* &root, int d, int depth) {
    if (root == nullptr) return nullptr;

    int dim = root->point->centroid.size();
    int currentDim = depth % dim;

    if (currentDim == d) {
        if (root->left == nullptr) return root;
        return findMinRec(root->left, d, depth + 1);
    }

    TreeNode* leftMin = findMinRec(root->left, d, depth + 1);
    TreeNode* rightMin = findMinRec(root->right, d, depth + 1);

    TreeNode* minNode = root;
    if (leftMin && leftMin->point->centroid[d] < minNode->point->centroid[d]) minNode = leftMin;
    if (rightMin && rightMin->point->centroid[d] < minNode->point->centroid[d]) minNode = rightMin;

    return minNode;
}

// this code is working fine
bool DeleteKdNode(TreeNode* &root, const vector<float>& target, int depth=0) {
    if (root == nullptr) return false;

    int dim = target.size();
    int currentDim = depth % dim;


    if (floatCompare(root->point->centroid, target)) {

        if (root->right != nullptr){

            TreeNode* minNode = findMinRec(root->right, currentDim, depth + 1);
            root->point = minNode->point;
            return DeleteKdNode(root->right, minNode->point->centroid, depth + 1);
        }

        else if (root->left != nullptr) {
            TreeNode* minNode = findMinRec(root->left, currentDim, depth + 1);
            root->point = minNode->point;
            root->right = root->left;
            root->left = nullptr;
            return DeleteKdNode(root->right, minNode->point->centroid, depth + 1);
        }

        else {
            delete root;
            root = nullptr;
            return true;
        }
    }


    if (target[currentDim] < root->point->centroid[currentDim]) {
        return DeleteKdNode(root->left, target, depth + 1);
    } else {
        return DeleteKdNode(root->right, target, depth + 1);
    }
}



void InsertKdNode(TreeNode* root, Cluster* cluster) {
    if (root == nullptr) return;

    int dim = root->point->centroid.size();
    int depth = 0;
    TreeNode* current = root;

    while (current) {
        int currentDim = depth % dim;
        if (cluster->centroid[currentDim] < current->point->centroid[currentDim]) {
            if (current->left == nullptr) {
                current->left = new TreeNode(cluster);
                return;
            }
            current = current->left;
        } else {
            if (current->right == nullptr) {
                current->right = new TreeNode(cluster);
                return;
            }
            current = current->right;
        }
        depth++;
    }
}


// this code is working fine
void bfs(TreeNode* root) {
    if (root == nullptr) {
        cerr << "Root not found" << endl;
        return;
    }

    queue<TreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int n = q.size();
        for (int i = 0; i < n; i++) {
            TreeNode* temp = q.front();
            q.pop();

            cout << "{";
            for (float val : temp->point->centroid) {
                cout << val << " ";
            }
            cout << "} ";

            if (temp->left != nullptr) q.push(temp->left);
            if (temp->right != nullptr) q.push(temp->right);
        }
        cout << endl;
    }
}
