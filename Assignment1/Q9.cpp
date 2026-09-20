#include <iostream>
using namespace std;

// Structure for a BST node augmented with subtree size.
struct Node {
    int key;
    int subtreeSize;
    struct Node* left;
    struct Node* right;
};

// Function to get the size of a subtree.
int getSize(Node* root) {
    if (root == NULL)
        return 0;

    return root->subtreeSize;
}

// Function to insert a key and update subtree sizes.
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
        return root; // Prevent duplicate keys.

    // Update size based on the augmented left and right subtrees.
    root->subtreeSize =
        1 + getSize(root->left) + getSize(root->right);

    return root;
}

// Function to find the minimum key in a subtree.
Node* minimum(Node* root) {
    Node* temp = root;

    while (temp->left != NULL)
        temp = temp->left;

    return temp;
}

// Function to delete a key and update subtree sizes.
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
        // Node with only one child or no child.
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

        // Node with two children: get the inorder successor.
        Node* temp = minimum(root->right);
        root->key = temp->key;
        root->right = deleteNode(root->right, temp->key);
    }

    // Update size based on the augmented left and right subtrees.
    root->subtreeSize =
        1 + getSize(root->left) + getSize(root->right);

    return root;
}

// Function to find the Kth smallest element using augmented subtree sizes.
Node* kthSmallest(Node* root, int k) {
    if (root == NULL)
        return NULL;

    int leftSize = getSize(root->left);

    // If the left subtree has exactly k-1 nodes, root is the kth smallest.
    if (k == leftSize + 1)
        return root;

    // If the left subtree has k or more nodes, the target is in the left subtree.
    if (k <= leftSize)
        return kthSmallest(root->left, k);

    // Otherwise, the target is in the right subtree.
    return kthSmallest(root->right, k - leftSize - 1);
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
    Node* root = NULL;

    int Q;
    cin >> Q;

    for (int i = 0; i < Q; ++i) {
        char operation;
        int x;

        cin >> operation >> x;
        if (operation == 'I') 
            root = insert(root, x);
        
        else if (operation == 'D') 
            root = deleteNode(root, x);
        
        else if (operation == 'K') {
            // Validate bounds before traversing
            if (x <= 0 || x > getSize(root))
                cout << "INVALID\n";
            
            else {
                Node* result = kthSmallest(root, x);
                cout << result->key << '\n';
            }
        }
    }

    freeTree(root);
    return 0;
}