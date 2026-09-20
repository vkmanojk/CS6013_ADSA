#include <iostream>
using namespace std;

struct Node {
    int key;
    int subtreeSize;
    struct Node* left;
    struct Node* right;
};

int getSize(Node* root) {
    if (root == NULL)
        return 0;

    return root->subtreeSize;
}

Node* insert(Node* root, int key) {
    if (root == NULL) {
        Node* newNode = new Node;
        newNode->key = key;
        newNode->subtreeSize = 1;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    if (key < root->key)
        root->left = insert(root->left, key);
    else if (key > root->key)
        root->right = insert(root->right, key);
    else
        return root;

    root->subtreeSize =
        1 + getSize(root->left) + getSize(root->right);

    return root;
}

Node* minimum(Node* root) {
    Node* temp = root;

    while (temp->left != NULL)
        temp = temp->left;

    return temp;
}

Node* deleteNode(Node* root, int key) {
    if (root == NULL)
        return NULL;

    if (key < root->key) {
        root->left = deleteNode(root->left, key);
    }
    else if (key > root->key) {
        root->right = deleteNode(root->right, key);
    }
    else {
        if (root->left == NULL) {
            Node* temp = root->right;
            delete root;
            return temp;
        }

        if (root->right == NULL) {
            Node* temp = root->left;
            delete root;
            return temp;
        }

        Node* temp = minimum(root->right);
        root->key = temp->key;
        root->right = deleteNode(root->right, temp->key);
    }

    root->subtreeSize =
        1 + getSize(root->left) + getSize(root->right);

    return root;
}

Node* kthSmallest(Node* root, int k) {
    if (root == NULL)
        return NULL;

    int leftSize = getSize(root->left);

    if (k == leftSize + 1)
        return root;

    if (k <= leftSize)
        return kthSmallest(root->left, k);

    return kthSmallest(root->right, k - leftSize - 1);
}

int main() {
    Node* root = NULL;

    int Q;
    cin >> Q;

    for (int i = 0; i < Q; ++i) {
        char operation;
        int x;

        cin >> operation >> x;

        if (operation == 'I') {
            root = insert(root, x);
        }
        else if (operation == 'D') {
            root = deleteNode(root, x);
        }
        else if (operation == 'K') {
            if (x <= 0 || x > getSize(root)) {
                cout << "INVALID\n";
            }
            else {
                Node* result = kthSmallest(root, x);
                cout << result->key << '\n';
            }
        }
    }

    return 0;
}