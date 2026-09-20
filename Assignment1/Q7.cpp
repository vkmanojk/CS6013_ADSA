#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Node {
    int key;
    int height;
    struct Node* left;
    struct Node* right;
};

int getHeight(Node* root) {
    if (root == NULL)
        return 0;

    return root->height;
}

Node* rotateRight(Node* root) {
    Node* child = root->left;
    Node* temp = child->right;

    child->right = root;
    root->left = temp;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    child->height = 1 + max(getHeight(child->left), getHeight(child->right));

    return child;
}

Node* rotateLeft(Node* root) {
    Node* child = root->right;
    Node* temp = child->left;

    child->left = root;
    root->right = temp;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    child->height = 1 + max(getHeight(child->left), getHeight(child->right));

    return child;
}

Node* insert(Node* root, int key) {
    if (root == NULL) {
        Node* newNode = new Node;
        newNode->key = key;
        newNode->height = 1;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    if (key < root->key)
        root->left = insert(root->left, key);
    else
        root->right = insert(root->right, key);

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));

    int balance = getHeight(root->left) - getHeight(root->right);

    if (balance > 1 && key < root->left->key)
        return rotateRight(root);

    if (balance < -1 && key > root->right->key)
        return rotateLeft(root);

    if (balance > 1 && key > root->left->key) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    if (balance < -1 && key < root->right->key) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

void collectDown(Node* root, int k, vector<int>& result) {
    if (root == NULL || k < 0)
        return;

    if (k == 0) {
        result.push_back(root->key);
        return;
    }

    collectDown(root->left, k - 1, result);
    collectDown(root->right, k - 1, result);
}

int findDistance(Node* root, int target, int k, vector<int>& result) {
    if (root == NULL)
        return -1;

    if (root->key == target) {
        collectDown(root, k, result);
        return 0;
    }

    int leftDistance = findDistance(root->left, target, k, result);

    if (leftDistance != -1) {
        int distance = leftDistance + 1;

        if (distance == k)
            result.push_back(root->key);
        else
            collectDown(root->right, k - distance - 1, result);

        return distance;
    }

    int rightDistance = findDistance(root->right, target, k, result);

    if (rightDistance != -1) {
        int distance = rightDistance + 1;

        if (distance == k)
            result.push_back(root->key);
        else
            collectDown(root->left, k - distance - 1, result);

        return distance;
    }

    return -1;
}

int main() {
    int n;
    cin>>n;

    Node* root = NULL;

    for (int i = 0; i < n; ++i) {
        int key;
        cin>>key;
        root = insert(root, key);
    }

    int target, k;
    cin>>target>>k;

    vector<int> result;

    findDistance(root, target, k, result);

    if (result.empty()) {
        cout<<"EMPTY\n";
        return 0;
    }

    sort(result.begin(), result.end());

    for (int i = 0; i < (int)result.size(); ++i) {
        if (i > 0)
            cout<<' ';

        cout<<result[i];
    }
    cout<<endl;
    return 0;
}