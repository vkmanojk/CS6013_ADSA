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

// Function to get the height of a node.
int getHeight(Node* root) {
    if (root == NULL)
        return 0;

    return root->height;
}

// Function to perform a right rotation.
Node* rotateRight(Node* root) {
    Node* curr = root->left;
    Node* temp = curr->right;

    curr->right = root;
    root->left = temp;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    curr->height = 1 + max(getHeight(curr->left), getHeight(curr->right));

    return curr;
}

// Function to perform a left rotation.
Node* rotateLeft(Node* root) {
    Node* curr = root->right;
    Node* temp = curr->left;

    curr->left = root;
    root->right = temp;

    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    curr->height = 1 + max(getHeight(curr->left), getHeight(curr->right));

    return curr;
}

// Function to insert a key into the AVL tree without duplicates.
Node* insert(Node* root, int key) {
    // Case 1. Empty subtree, create node.
    if (root == NULL) {
        Node* newNode = new Node;
        newNode->key = key;
        newNode->height = 1;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    // Case 2. Traverse and insert.
    if (key < root->key)
        root->left = insert(root->left, key);
    else if (key > root->key)
        root->right = insert(root->right, key);
    else
        return root; // Prevent duplicate keys.

    // Case 3. Update height and balance.
    root->height = 1 + max(getHeight(root->left), getHeight(root->right));
    int balance = getHeight(root->left) - getHeight(root->right);

    // Case 4. Perform rotations if unbalanced.
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

// Function to collect all descendant nodes at distance k.
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

// Function to find the target and collect nodes at distance k across the tree.
int findDistance(Node* root, int target, int k, vector<int>& result) {
    if (root == NULL)
        return -1;

    // Case 1. Target found, search its descendants.
    if (root->key == target) {
        collectDown(root, k, result);
        return 0;
    }

    // Case 2. Search left subtree.
    int leftDistance = findDistance(root->left, target, k, result);

    if (leftDistance != -1) {
        int distance = leftDistance + 1;

        if (distance == k)
            result.push_back(root->key);
        else
            collectDown(root->right, k - distance - 1, result);

        return distance;
    }

    // Case 3. Search right subtree.
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

// Function to free dynamically allocated tree memory.
void freeTree(Node* root) {
    if (root == NULL)
        return;
    
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main() {
    int n;
    cin >> n;

    Node* root = NULL;

    for (int i = 0; i < n; ++i) {
        int key;
        cin >> key;
        root = insert(root, key);
    }

    int target, k;
    cin >> target >> k;

    vector<int> result;
    findDistance(root, target, k, result);

    if (result.empty()) {
        cout << "EMPTY\n";
        freeTree(root);
        return 0;
    }

    sort(result.begin(), result.end());
    for (int i = 0; i < (int)result.size(); ++i) {
        if (i > 0)
            cout << ' ';
        cout << result[i];
    }
    cout << endl;
    freeTree(root);
    return 0;
}