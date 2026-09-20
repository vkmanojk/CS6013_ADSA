#include <iostream>
#include <queue>

using namespace std;

bool flag;
bool first;

struct node {
    int data;
    struct node* left;
    struct node* right;
    struct node* parent;
    bool color;
    int black_depth;
};

// Function to check whether a node is red.
bool isRed(node* root) {
    if (root == NULL)
        return false;

    return root->color == true;
}

// Function to check whether a node is black.
bool isBlack(node* root) {
    if (root == NULL)
        return true;

    return root->color == false;
}

// Function to update black depths.
void updateBlackDepth(node* root, int depth) {
    if (root == NULL)
        return;

    root->black_depth = depth;

    int newDepth = depth;

    if (root->color == false)
        newDepth++;

    updateBlackDepth(root->left, newDepth);
    updateBlackDepth(root->right, newDepth);
}

// Function to perform a left rotation.
void leftRotate(node*& root, node* temp) {
    node* child = temp->right;

    temp->right = child->left;

    if (child->left != NULL)
        child->left->parent = temp;

    child->parent = temp->parent;

    if (temp->parent == NULL)
        root = child;
    else if (temp == temp->parent->left)
        temp->parent->left = child;
    else
        temp->parent->right = child;

    child->left = temp;
    temp->parent = child;
}

// Function to perform a right rotation.
void rightRotate(node*& root, node* temp) {
    node* child = temp->left;

    temp->left = child->right;

    if (child->right != NULL)
        child->right->parent = temp;

    child->parent = temp->parent;

    if (temp->parent == NULL)
        root = child;
    else if (temp == temp->parent->left)
        temp->parent->left = child;
    else
        temp->parent->right = child;

    child->right = temp;
    temp->parent = child;
}

// Function to print inorder traversal.
void printInOrder(node* root) {
    if (root == NULL)
        return;

    printInOrder(root->left);

    if (!first)
        cout<<" ";

    cout<<root->data;

    if (root->color == true)
        cout<<"(R)";
    else
        cout<<"(B)";

    first = false;

    printInOrder(root->right);
}

// Function to print preorder traversal.
void printPreOrder(node* root) {
    if (root == NULL)
        return;

    if (!first)
        cout<<" ";

    cout<<root->data;

    if (root->color == true)
        cout<<"(R)";
    else
        cout<<"(B)";

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
                cout<<" ";

            cout<<temp->data;

            if (temp->color == true)
                cout<<"(R)";
            else
                cout<<"(B)";

            if (temp->left != NULL)
                q.push(temp->left);

            if (temp->right != NULL)
                q.push(temp->right);
        }

        cout<<endl;
    }
}

// Function to print all traversals.
void displayTraversals(node* root) {
    cout<<"INORDER: ";
    first = true;
    printInOrder(root);

    cout<<"\nPREORDER: ";
    first = true;
    printPreOrder(root);

    cout<<"\nLEVELORDER:\n";
    printLevelOrder(root);

    cout<<"END\n";
}

// Function to search for a key in the Red-Black tree.
node* search(node* root, int key) {
    node* temp = root;

    while (temp != NULL) {
        if (temp->data == key) {
            cout<<"Key found\n";
            return temp;
        }
        else if (key < temp->data)
            temp = temp->left;
        else
            temp = temp->right;
    }

    cout<<"Key not found\n";
    return NULL;
}

// Function to restore Red-Black properties after insertion.
void insertNodeUpdate(node*& root, node* x) {

    while (x != root && isRed(x->parent)) {

        node* b = x->parent;
        node* a = b->parent;

        // b is the left child of a.
        if (b == a->left) {

            node* c = a->right;

            if (isRed(c)) {
                b->color = false;
                c->color = false;
                a->color = true;

                x = a;
            }

            else {

                // x is the right child of b.
                if (x == b->right) {
                    x = b;
                    leftRotate(root, x);

                    b = x->parent;
                    a = b->parent;
                }

                // x is the left child of b.
                b->color = false;
                a->color = true;

                rightRotate(root, a);
            }
        }

        // b is the right child of a.
        else {

            node* c = a->left;

            if (isRed(c)) {
                b->color = false;
                c->color = false;
                a->color = true;

                x = a;
            }

            else {

                // x is the left child of b.
                if (x == b->left) {
                    x = b;
                    rightRotate(root, x);

                    b = x->parent;
                    a = b->parent;
                }

                // x is the right child of b.
                b->color = false;
                a->color = true;

                leftRotate(root, a);
            }
        }
    }

    root->color = false;
}

// Function to insert a key into the Red-Black tree.
node* insert(node* root, int key) {
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
    newNode->color = true;
    newNode->black_depth = 0;

    if (parent == NULL) {
        newNode->color = false;
        return newNode;
    }

    if (key < parent->data)
        parent->left = newNode;
    else
        parent->right = newNode;

    insertNodeUpdate(root, newNode);

    return root;
}

// Function to find the minimum node.
node* minimum(node* root) {
    node* temp = root;
    while (temp->left != NULL)
        temp = temp->left;
    return temp;
}

// Function to replace one subtree with another.
void replaceSubTree(node*& root, node* temp, node* child) {
    if (temp->parent == NULL)
        root = child;
    else if (temp == temp->parent->left)
        temp->parent->left = child;
    else
        temp->parent->right = child;

    if (child != NULL)
        child->parent = temp->parent;
}

// Function to restore Red-Black properties after deletion.
void deleteNodeUpdate(node*& root, node* x, node* a) {
    // Add isBlack(x) condition to prevent erroneous entry when x is RED
    while (x != root && isBlack(x)) {

        if (a == NULL)
            break;

        // x is the left child of a.
        if (x == a->left) {

            node* b = a->right;

            // b is red.
            if (isRed(b)) {
                b->color = false;
                a->color = true;

                leftRotate(root, a);

                b = a->right;
            }

            node* c = NULL;
            node* d = NULL;

            if (b != NULL) {
                c = b->left;
                d = b->right;
            }
            // b is black and both its children are black.
            if (isBlack(c) && isBlack(d)) {
                if (b != NULL)
                    b->color = true;
                // If a is red, make it black and stop.
                if (isRed(a)) {
                    a->color = false;
                    break;
                }
                // Continue with the parent.
                x = a;
                a = x->parent;
            }

            else {

                // b has a red left child and black right child.
                if (isBlack(d)) {
                    if (c != NULL)
                        c->color = false;
                    b->color = true;
                    rightRotate(root, b);
                    b = a->right;
                    if (b != NULL) {
                        c = b->left;
                        d = b->right;
                    }
                }
                // b has a red right child.
                b->color = a->color;
                a->color = false;
                if (d != NULL)
                    d->color = false;
                leftRotate(root, a);
                break;
            }
        }

        // x is the right child of a.
        else {
            node* b = a->left;
            // b is red.
            if (isRed(b)) {
                b->color = false;
                a->color = true;

                rightRotate(root, a);

                b = a->left;
            }

            node* c = NULL;
            node* d = NULL;
            if (b != NULL) {
                c = b->left;
                d = b->right;
            }

            // b is black and both its children are black.
            if (isBlack(c) && isBlack(d)) {
                if (b != NULL)
                    b->color = true;
                // If a is red, make it black and stop.
                if (isRed(a)) {
                    a->color = false;
                    break;
                }
                // Continue with the parent.
                x = a;
                a = x->parent;
            }
            else {
                // b has a red right child and black left child.
                if (isBlack(c)) {
                    if (d != NULL)
                        d->color = false;
                    b->color = true;
                    leftRotate(root, b);
                    b = a->left;
                    if (b != NULL) {
                        c = b->left;
                        d = b->right;
                    }
                }
                // b has a red left child.
                b->color = a->color;
                a->color = false;
                if (c != NULL)
                    c->color = false;
                rightRotate(root, a);

                break;
            }
        }
    }
    if (x != NULL)
        x->color = false;
    if (root != NULL)
        root->color = false;
}

// Function to delete a key from the Red-Black tree.
node* deleteNode(node* root, int key) {
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

    /*
       Perform deletion as in a BST. If the node has two children,
       replace its value by the value of its inorder successor.
    */

    if (temp->left != NULL && temp->right != NULL) {
        node* x = minimum(temp->right);
        temp->data = x->data;
        temp = x;
    }

    node* child;

    if (temp->left != NULL)
        child = temp->left;
    else
        child = temp->right;

    node* a = temp->parent;
    bool deletedColor = temp->color;

    // Delete the root.
    if (a == NULL) {
        root = child;
        if (child != NULL)
            child->parent = NULL;
        delete temp;
        if (root != NULL)
            root->color = false;
        return root;
    }

    // Replace temp by its child.
    if (temp == a->left)
        a->left = child;
    else
        a->right = child;
    if (child != NULL)
        child->parent = a;

    delete temp;

    /*
       If the deleted node was black, restore the
       Red-Black properties using the deletion cases.
    */
    if (deletedColor == false)
        deleteNodeUpdate(root, child, a);
    if (root != NULL)
        root->color = false;
    return root;
}

// Function to free dynamically allocated tree memory.
void freeTree(node* root) {
    if (root == NULL)
        return;
    
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}


int main() {
    node *root = NULL, *keyNode;
    int N, S, I, D, key;

    cin>>N;
    for (int i = 0; i < N; ++i) {
        cin>>key;
        flag = false;
        root = insert(root, key);
        if (root != NULL)
            root->parent = NULL;
        updateBlackDepth(root, 0);
    }
    cout<<"INITIAL TREE\n";
    displayTraversals(root);

    cin>>S;
    for (int i = 0; i < S; ++i) {
        cin>>key;
        cout<<"SEARCH " << key << ": ";
        keyNode = search(root, key);
    }

    cin>>I;
    for (int i = 0; i < I; ++i) {
        cin>>key;
        cout<<"INSERT " << key << endl;
        flag = false;
        root = insert(root, key);
        if (root != NULL)
            root->parent = NULL;
        updateBlackDepth(root, 0);
        if (flag)
            cout<<"Duplicate key\n";
        else
            displayTraversals(root);
    }

    cin>>D;
    for (int i = 0; i < D; ++i) {
        cin>>key;
        cout<<"DELETE " << key << endl;
        flag = false;
        root = deleteNode(root, key);
        if (root != NULL)
            root->parent = NULL;
        updateBlackDepth(root, 0);
        if (!flag)
            cout<<"Key not found\n";
        else
            displayTraversals(root);
    }

    freeTree(root);
    return 0;
}