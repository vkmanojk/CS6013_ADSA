#include <iostream>
#include <queue>

using namespace std;

bool flag, first;

struct node {
    int data;
    struct node* left;
    struct node* right;
    struct node* parent;
};

// Function to print inorder traversal.
// Function to print inorder traversal.
void printInOrder(node* root) {
    if (root == NULL)
        return;

    printInOrder(root->left);

    if (!first)
        cout << " ";

    cout << root->data;
    first = false;

    printInOrder(root->right);
}

// Function to print preorder traversal.
void printPreOrder(node* root) {
    if (root == NULL)
        return;

    if (!first)
        cout << " ";

    cout << root->data;
    first = false;

    printPreOrder(root->left);
    printPreOrder(root->right);
}

// Function to print level-order traversal.
void printLevelOrder(node* root) {
    if (root == NULL)
        return;
    queue<node*> q;
    q.push(root);
    while (!q.empty()) {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; ++i) {
            node* temp = q.front();
            q.pop();

            if (i > 0)
                cout << " ";

            cout << temp->data;

            if (temp->left != NULL)
                q.push(temp->left);

            if (temp->right != NULL)
                q.push(temp->right);
        }
        cout << endl;
    }
}

// Function to print all traversals.
void displayTraversals(node* root) {
    cout << "INORDER: ";
    first = true;
    printInOrder(root);

    cout << "\nPREORDER: ";
    first = true;
    printPreOrder(root);

    cout << "\nLEVELORDER:\n";
    printLevelOrder(root);
    cout << "END\n";
}

// Function to insert a key into the BST.
node* insert(node* root, int key) {
    flag = false;

    node* temp = root;
    node* parent = NULL;

    while (temp != NULL) {
        parent = temp;
        if (key < temp->data)
            temp = temp->left;
        else if (key > temp->data)
            temp = temp->right;
        else {
            flag = true;
            return root;
        }
    }

    node* newNode = new node;
    newNode->data = key;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = parent;

    if (parent == NULL)
        return newNode;

    if (key < parent->data)
        parent->left = newNode;
    else
        parent->right = newNode;

    return root;
}

// Function to search for a key in the BST.
node* search(node* root, int key) {
    node* temp = root;

    while (temp != NULL) {
        if (temp->data == key) {
            cout << "Key found\n";
            return temp;
        }
        else if (key < temp->data)
            temp = temp->left;
        else
            temp = temp->right;
    }

    cout << "Key not found\n";
    return NULL;
}

// Function to delete a key from the BST.
node* deleteNode(node* root, int key) {
    flag = false;
    node* temp = root;

    while (temp != NULL && temp->data != key) {
        if (key < temp->data)
            temp = temp->left;
        else
            temp = temp->right;
    }

    if (temp == NULL)
        return root;
    flag = true;

    // If the node has two children, find its inorder successor.
    if (temp->left != NULL && temp->right != NULL) {
        node* successor = temp->right;
        while (successor->left != NULL)
            successor = successor->left;
        temp->data = successor->data;
        temp = successor;
    }

    // The node now has at most one child.
    node* child;

    if (temp->left != NULL)
        child = temp->left;
    else
        child = temp->right;

    if (temp->parent == NULL)
        root = child;
    else if (temp == temp->parent->left)
        temp->parent->left = child;
    else
        temp->parent->right = child;
    if (child != NULL)
        child->parent = temp->parent;
    delete temp;
    return root;
}


int main() {
    node *root = NULL, *keyNode;
    int N, S, I, D, key;

    cin >> N;
    for (int i = 0; i < N; ++i) {
        cin >> key;
        root = insert(root, key);
    }
    cout << "INITIAL TREE\n";
    displayTraversals(root);

    cin >> S;

    for (int i = 0; i < S; ++i) {
        cin >> key;
        cout << "SEARCH " << key << ": ";
        keyNode = search(root, key);
    }

    cin >> I;
    for (int i = 0; i < I; ++i) {
        cin >> key;
        cout << "INSERT " << key << endl;
        flag = false;
        root = insert(root, key);
        if (flag)
            cout << "Duplicate Key\n";
        else
            displayTraversals(root);
    }

    cin >> D;
    for (int i = 0; i < D; ++i) {
        cin >> key;
        cout << "DELETE " << key << endl;
        flag = false;
        root = deleteNode(root, key);
        if (!flag)
            cout << "Key not found\n";
        else
            displayTraversals(root);
    }

    return 0;
}