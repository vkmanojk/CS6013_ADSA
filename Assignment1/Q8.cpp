#include <iostream>
using namespace std;

struct AVLNode {
    int key;
    int height;
    struct AVLNode* left;
    struct AVLNode* right;
    struct AVLNode* parent;
};

struct RBNode {
    int key;
    bool color;
    struct RBNode* left;
    struct RBNode* right;
    struct RBNode* parent;
};

int getHeight(AVLNode* root) {
    if (root == NULL)
        return 0;

    return root->height;
}

void updateHeight(AVLNode* root) {
    int leftHeight = getHeight(root->left);
    int rightHeight = getHeight(root->right);

    if (leftHeight > rightHeight)
        root->height = leftHeight + 1;
    else
        root->height = rightHeight + 1;
}

int getBalance(AVLNode* root) {
    if (root == NULL)
        return 0;

    return getHeight(root->left) - getHeight(root->right);
}

AVLNode* rotateRight(AVLNode* root) {
    AVLNode* temp = root->left;
    AVLNode* parent = root->parent;

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

AVLNode* rotateLeft(AVLNode* root) {
    AVLNode* temp = root->right;
    AVLNode* parent = root->parent;

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

AVLNode* insertAVL(AVLNode* root, int key) {
    AVLNode* temp = root;
    AVLNode* parent = NULL;

    while (temp != NULL) {
        parent = temp;

        if (key < temp->key)
            temp = temp->left;
        else if (key > temp->key)
            temp = temp->right;
        else
            return root;
    }

    AVLNode* newNode = new AVLNode;

    newNode->key = key;
    newNode->height = 1;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = parent;

    if (parent == NULL)
        return newNode;

    if (key < parent->key)
        parent->left = newNode;
    else
        parent->right = newNode;

    temp = parent;

    while (temp != NULL) {
        updateHeight(temp);

        int balance = getBalance(temp);

        if (balance > 1 || balance < -1) {
            if (balance > 1 && key < temp->left->key) {
                AVLNode* newRoot = rotateRight(temp);

                if (newRoot->parent == NULL)
                    root = newRoot;

                break;
            }

            if (balance < -1 && key > temp->right->key) {
                AVLNode* newRoot = rotateLeft(temp);

                if (newRoot->parent == NULL)
                    root = newRoot;

                break;
            }

            if (balance > 1 && key > temp->left->key) {
                rotateLeft(temp->left);

                AVLNode* newRoot = rotateRight(temp);

                if (newRoot->parent == NULL)
                    root = newRoot;

                break;
            }

            if (balance < -1 && key < temp->right->key) {
                rotateRight(temp->right);

                AVLNode* newRoot = rotateLeft(temp);

                if (newRoot->parent == NULL)
                    root = newRoot;

                break;
            }
        }

        temp = temp->parent;
    }

    return root;
}

AVLNode* deleteAVL(AVLNode* root, int key) {
    AVLNode* temp = root;

    while (temp != NULL && temp->key != key) {
        if (key < temp->key)
            temp = temp->left;
        else
            temp = temp->right;
    }

    if (temp == NULL)
        return root;

    if (temp->left != NULL && temp->right != NULL) {
        AVLNode* successor = temp->right;

        while (successor->left != NULL)
            successor = successor->left;

        temp->key = successor->key;
        temp = successor;
    }

    AVLNode* child;

    if (temp->left != NULL)
        child = temp->left;
    else
        child = temp->right;

    AVLNode* parent = temp->parent;

    if (parent == NULL) {
        root = child;

        if (child != NULL)
            child->parent = NULL;

        delete temp;
        return root;
    }

    if (temp == parent->left)
        parent->left = child;
    else
        parent->right = child;

    if (child != NULL)
        child->parent = parent;

    delete temp;

    temp = parent;

    while (temp != NULL) {
        updateHeight(temp);

        int balance = getBalance(temp);

        if (balance > 1) {
            AVLNode* y = temp->left;

            if (getHeight(y->left) >= getHeight(y->right)) {
                AVLNode* newRoot = rotateRight(temp);

                if (newRoot->parent == NULL)
                    root = newRoot;

                temp = newRoot->parent;
            }
            else {
                rotateLeft(y);

                AVLNode* newRoot = rotateRight(temp);

                if (newRoot->parent == NULL)
                    root = newRoot;

                temp = newRoot->parent;
            }
        }
        else if (balance < -1) {
            AVLNode* y = temp->right;

            if (getHeight(y->right) >= getHeight(y->left)) {
                AVLNode* newRoot = rotateLeft(temp);

                if (newRoot->parent == NULL)
                    root = newRoot;

                temp = newRoot->parent;
            }
            else {
                rotateRight(y);

                AVLNode* newRoot = rotateLeft(temp);

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

bool isRed(RBNode* root) {
    if (root == NULL)
        return false;

    return root->color;
}

bool isBlack(RBNode* root) {
    if (root == NULL)
        return true;

    return !root->color;
}

void leftRotate(RBNode*& root, RBNode* temp) {
    RBNode* child = temp->right;

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

void rightRotate(RBNode*& root, RBNode* temp) {
    RBNode* child = temp->left;

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

void insertRBUpdate(RBNode*& root, RBNode* x) {
    while (x != root && isRed(x->parent)) {
        RBNode* b = x->parent;
        RBNode* a = b->parent;

        if (b == a->left) {
            RBNode* c = a->right;

            if (isRed(c)) {
                b->color = false;
                c->color = false;
                a->color = true;
                x = a;
            }
            else {
                if (x == b->right) {
                    x = b;
                    leftRotate(root, x);
                    b = x->parent;
                    a = b->parent;
                }

                b->color = false;
                a->color = true;
                rightRotate(root, a);
            }
        }
        else {
            RBNode* c = a->left;

            if (isRed(c)) {
                b->color = false;
                c->color = false;
                a->color = true;
                x = a;
            }
            else {
                if (x == b->left) {
                    x = b;
                    rightRotate(root, x);
                    b = x->parent;
                    a = b->parent;
                }

                b->color = false;
                a->color = true;
                leftRotate(root, a);
            }
        }
    }

    root->color = false;
}

RBNode* insertRB(RBNode* root, int key) {
    RBNode* temp = root;
    RBNode* parent = NULL;

    while (temp != NULL) {
        parent = temp;

        if (key < temp->key)
            temp = temp->left;
        else if (key > temp->key)
            temp = temp->right;
        else
            return root;
    }

    RBNode* newNode = new RBNode;

    newNode->key = key;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = parent;
    newNode->color = true;

    if (parent == NULL) {
        newNode->color = false;
        return newNode;
    }

    if (key < parent->key)
        parent->left = newNode;
    else
        parent->right = newNode;

    insertRBUpdate(root, newNode);

    return root;
}

RBNode* minimum(RBNode* root) {
    RBNode* temp = root;

    while (temp->left != NULL)
        temp = temp->left;

    return temp;
}

void deleteRBUpdate(RBNode*& root, RBNode* x, RBNode* parent) {
    while (x != root && parent != NULL) {
        if (x == parent->left) {
            RBNode* b = parent->right;

            if (isRed(b)) {
                b->color = false;
                parent->color = true;
                leftRotate(root, parent);
                b = parent->right;
            }

            RBNode* c = NULL;
            RBNode* d = NULL;

            if (b != NULL) {
                c = b->left;
                d = b->right;
            }

            if (isBlack(c) && isBlack(d)) {
                if (b != NULL)
                    b->color = true;

                if (isRed(parent)) {
                    parent->color = false;
                    break;
                }

                x = parent;
                parent = x->parent;
            }
            else {
                if (isBlack(d)) {
                    if (c != NULL)
                        c->color = false;

                    if (b != NULL)
                        b->color = true;

                    rightRotate(root, b);
                    b = parent->right;

                    if (b != NULL) {
                        c = b->left;
                        d = b->right;
                    }
                }

                if (b != NULL)
                    b->color = parent->color;

                parent->color = false;

                if (d != NULL)
                    d->color = false;

                leftRotate(root, parent);
                break;
            }
        }
        else {
            RBNode* b = parent->left;

            if (isRed(b)) {
                b->color = false;
                parent->color = true;
                rightRotate(root, parent);
                b = parent->left;
            }

            RBNode* c = NULL;
            RBNode* d = NULL;

            if (b != NULL) {
                c = b->left;
                d = b->right;
            }

            if (isBlack(c) && isBlack(d)) {
                if (b != NULL)
                    b->color = true;

                if (isRed(parent)) {
                    parent->color = false;
                    break;
                }

                x = parent;
                parent = x->parent;
            }
            else {
                if (isBlack(c)) {
                    if (d != NULL)
                        d->color = false;

                    if (b != NULL)
                        b->color = true;

                    leftRotate(root, b);
                    b = parent->left;

                    if (b != NULL) {
                        c = b->left;
                        d = b->right;
                    }
                }

                if (b != NULL)
                    b->color = parent->color;

                parent->color = false;

                if (c != NULL)
                    c->color = false;

                rightRotate(root, parent);
                break;
            }
        }
    }

    if (x != NULL)
        x->color = false;

    if (root != NULL)
        root->color = false;
}

RBNode* deleteRB(RBNode* root, int key) {
    RBNode* temp = root;

    while (temp != NULL && temp->key != key) {
        if (key < temp->key)
            temp = temp->left;
        else
            temp = temp->right;
    }

    if (temp == NULL)
        return root;

    if (temp->left != NULL && temp->right != NULL) {
        RBNode* successor = minimum(temp->right);
        temp->key = successor->key;
        temp = successor;
    }

    RBNode* child;

    if (temp->left != NULL)
        child = temp->left;
    else
        child = temp->right;

    RBNode* parent = temp->parent;
    bool deletedColor = temp->color;

    if (parent == NULL) {
        root = child;

        if (child != NULL)
            child->parent = NULL;

        delete temp;

        if (root != NULL)
            root->color = false;

        return root;
    }

    if (temp == parent->left)
        parent->left = child;
    else
        parent->right = child;

    if (child != NULL)
        child->parent = parent;

    delete temp;

    if (!deletedColor)
        deleteRBUpdate(root, child, parent);

    if (root != NULL)
        root->color = false;

    return root;
}

int countIgnored(AVLNode* avl, RBNode* rb) {
    if (avl == NULL)
        return 0;

    int count = 0;

    if (rb == NULL || avl->key != rb->key)
        count++;

    count += countIgnored(avl->left, rb == NULL ? NULL : rb->left);
    count += countIgnored(avl->right, rb == NULL ? NULL : rb->right);

    return count;
}

int main() {
    AVLNode* avlRoot = NULL;
    RBNode* rbRoot = NULL;

    int Q;
    cin >> Q;

    for (int i = 0; i < Q; ++i) {
        char operation;
        int key;

        cin >> operation >> key;

        if (operation == 'I') {
            avlRoot = insertAVL(avlRoot, key);
            if (avlRoot != NULL)
                avlRoot->parent = NULL;

            rbRoot = insertRB(rbRoot, key);
            if (rbRoot != NULL)
                rbRoot->parent = NULL;
        }
        else {
            avlRoot = deleteAVL(avlRoot, key);
            if (avlRoot != NULL)
                avlRoot->parent = NULL;

            rbRoot = deleteRB(rbRoot, key);
            if (rbRoot != NULL)
                rbRoot->parent = NULL;
        }

        int ignored = countIgnored(avlRoot, rbRoot);

        if (ignored == 0)
            cout << "MATCH ";
        else
            cout << "NO MATCH ";

        cout << ignored << " ";

        if (avlRoot == NULL)
            cout << "EMPTY";
        else
            cout << avlRoot->key;

        cout << " ";

        if (rbRoot == NULL)
            cout << "EMPTY";
        else
            cout << rbRoot->key;

        cout << '\n';
    }

    return 0;
}