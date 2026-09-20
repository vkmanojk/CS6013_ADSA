#include <iostream>
#include <queue>

using namespace std;

bool flag;
bool first;

struct node {
    int data;
    int height;
    struct node* left;
    struct node* right;
    struct node* parent;
};

// Function to get the height of a node.
int getHeight(node* root) {
    if (root == NULL)
        return 0;

    return root->height;
}

// Function to update the height of a node.
void updateHeight(node* root) {
    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);

    if (leftHeight > rightHeight)
        root->height = leftHeight + 1;
    else
        root->height = rightHeight + 1;
}

// Function to get the balance factor of a node.
int getBalance(node* root) {
    if (root == NULL)
        return 0;

    return getHeight(root->left) - getHeight(root->right);
}

// Function to perform a right rotation.
node* rotateRight(node* root) {
    node* temp = root->left;
    node* parent = root->parent;

    root->left = temp->right;

    if (temp->right != NULL)
        temp->right->parent = root;

    temp->right = root;
    root->parent = temp;

    temp->parent = parent;

    if (parent != NULL) {
        if (parent->left == root)
            parent->left = temp;
        else
            parent->right = temp;
    }

    updateHeight(root);
    updateHeight(temp);

    return temp;
}

// Function to perform a left rotation.
node* rotateLeft(node* root) {
    node* temp = root->right;
    node* parent = root->parent;

    root->right = temp->left;

    if (temp->left != NULL)
        temp->left->parent = root;

    temp->left = root;
    root->parent = temp;

    temp->parent = parent;

    if (parent != NULL) {
        if (parent->left == root)
            parent->left = temp;
        else
            parent->right = temp;
    }

    updateHeight(root);
    updateHeight(temp);

    return temp;
}

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

// Function to search for a key in the AVL tree.
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

// Function to insert a key into the AVL tree.
node* insert(node* root, int key) {
    node* temp = root;
    node* parent = NULL;

    // Find the appropriate position as in a BST.
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

    // Create the new node.
    node* newNode = new node;

    newNode->data = key;
    newNode->height = 1;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = parent;

    // Insert into an empty tree.
    if (parent == NULL)
        return newNode;

    // Attach the new node to its parent.
    if (key < parent->data)
        parent->left = newNode;
    else
        parent->right = newNode;

    // Traverse from the inserted node towards the root.
    temp = parent;

    while (temp != NULL) {
        updateHeight(temp);

        int balance = getBalance(temp);

        // First unbalanced node found.
        if (balance > 1 || balance < -1) {

            // Left-Left case.
            if (balance > 1 && key < temp->left->data) {
                node* newRoot = rotateRight(temp);

                if (newRoot->parent == NULL)
                    root = newRoot;

                break;
            }

            // Right-Right case.
            if (balance < -1 && key > temp->right->data) {
                node* newRoot = rotateLeft(temp);

                if (newRoot->parent == NULL)
                    root = newRoot;

                break;
            }

            // Left-Right case.
            if (balance > 1 && key > temp->left->data) {
                rotateLeft(temp->left);

                node* newRoot = rotateRight(temp);

                if (newRoot->parent == NULL)
                    root = newRoot;

                break;
            }

            // Right-Left case.
            if (balance < -1 && key < temp->right->data) {
                rotateRight(temp->right);

                node* newRoot = rotateLeft(temp);

                if (newRoot->parent == NULL)
                    root = newRoot;

                break;
            }
        }

        temp = temp->parent;
    }

    return root;
}

// Function to delete a key from the AVL tree.
node* deleteNode(node* root, int key) {
    node* temp = root;

    // Find the node as in a BST.
    while (temp != NULL && temp->data != key) {
        if (key < temp->data)
            temp = temp->left;
        else
            temp = temp->right;
    }

    // Key not found.
    if (temp == NULL)
        return root;

    flag = true;

    // If the node has two children, replace it with its successor.
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

    node* parent = temp->parent;

    // Delete the root.
    if (parent == NULL) {
        root = child;

        if (child != NULL)
            child->parent = NULL;

        delete temp;
        return root;
    }

    // Replace the node by its child.
    if (temp == parent->left)
        parent->left = child;
    else
        parent->right = child;

    if (child != NULL)
        child->parent = parent;

    delete temp;

    // Traverse from the deleted node's parent towards the root.
    temp = parent;

    while (temp != NULL) {
        updateHeight(temp);

        int balance = getBalance(temp);

        // Left heavy.
        if (balance > 1) {
            node* y = temp->left;

            // Left-Left case.
            if (getHeight(y->left) >= getHeight(y->right)) {
                node* newRoot = rotateRight(temp);

                if (newRoot->parent == NULL)
                    root = newRoot;

                temp = newRoot->parent;
            }

            // Left-Right case.
            else {
                rotateLeft(y);

                node* newRoot = rotateRight(temp);

                if (newRoot->parent == NULL)
                    root = newRoot;

                temp = newRoot->parent;
            }
        }

        // Right heavy.
        else if (balance < -1) {
            node* y = temp->right;

            // Right-Right case.
            if (getHeight(y->right) >= getHeight(y->left)) {
                node* newRoot = rotateLeft(temp);

                if (newRoot->parent == NULL)
                    root = newRoot;

                temp = newRoot->parent;
            }

            // Right-Left case.
            else {
                rotateRight(y);

                node* newRoot = rotateLeft(temp);

                if (newRoot->parent == NULL)
                    root = newRoot;

                temp = newRoot->parent;
            }
        }

        else {
            temp = temp->parent;
        }
    }

    return root;
}


int main() {
    node *root = NULL, *keyNode;
    int N, S, I, D, key;

    cin >> N;

    for (int i = 0; i < N; ++i) {
        cin >> key;

        flag = false;
        root = insert(root, key);

        if (root != NULL)
            root->parent = NULL;
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

        if (root != NULL)
            root->parent = NULL;

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

        if (root != NULL)
            root->parent = NULL;

        if (!flag)
            cout << "Key not found\n";
        else
            displayTraversals(root);
    }

    return 0;
}