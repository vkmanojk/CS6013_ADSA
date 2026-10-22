#include <iostream>
using namespace std;

enum Color { RED, BLACK };

struct Node {
    long long key;
    Color color;
    int sz;
    Node *left, *right, *parent;

    Node(long long k) {
        key = k;
        color = RED;
        sz = 1;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
};

Node* root = nullptr; // Global root pointer

int getSize(Node* u) {
    return u ? u->sz : 0;
}

void updateSize(Node* u) {
    if (u) {
        u->sz = 1 + getSize(u->left) + getSize(u->right);
    }
}

void updateAncestorsSize(Node* u) {
    while (u) {
        updateSize(u);
        u = u->parent;
    }
}

bool isRed(Node* u) {
    return u != nullptr && u->color == RED; // External nodes (nullptr) are Black
}

void rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;

    y->parent = x->parent;
    if (!x->parent) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;

    updateSize(x);
    updateSize(y);
}

void rotateRight(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right) y->right->parent = x;

    y->parent = x->parent;
    if (!x->parent) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->right = x;
    x->parent = y;

    updateSize(x);
    updateSize(y);
}

void rotateUp(Node* childNode) {
    Node* p = childNode->parent;
    if (!p) return;
    if (childNode == p->left) {
        rotateRight(p);
    } else {
        rotateLeft(p);
    }
}

void fixInsert(Node* k) {
    while (k != root && isRed(k->parent)) {
        Node* a = k->parent;
        Node* b = a->parent;
        Node* c = (a == b->left) ? b->right : b->left;

        if (isRed(c)) {
            a->color = BLACK;
            c->color = BLACK;
            b->color = RED;
            k = b;
        } else {
            bool aIsLeft = (a == b->left);
            bool kIsLeft = (k == a->left);

            if (aIsLeft == kIsLeft) {
                rotateUp(a);
                a->color = BLACK;
                b->color = RED;
            } else {
                rotateUp(k);
                rotateUp(k);
                k->color = BLACK;
                b->color = RED;
            }
            break;
        }
    }
    if (root) root->color = BLACK;
}

void fixDeletion(Node* u, Node* a) {
    while (u != root && a != nullptr) {
        bool uIsRight = (u == a->right);
        Node* b = uIsRight ? a->left : a->right;
        if (u == nullptr) {
            b = (a->left != nullptr) ? a->left : a->right;
            uIsRight = (b == a->left);
        }

        if (isRed(a)) {
            Node* innerC = uIsRight ? b->right : b->left;
            Node* outerC = uIsRight ? b->left : b->right;

            if (!isRed(innerC) && !isRed(outerC)) {
                a->color = BLACK;
                b->color = RED;
                return;
            } else {
                if (isRed(innerC)) {
                    Node* c = innerC;
                    rotateUp(c);
                    rotateUp(c);
                    c->color = RED;
                    a->color = BLACK;
                    b->color = BLACK;
                } else {
                    Node* c = outerC;
                    rotateUp(b);
                    b->color = RED;
                    a->color = BLACK;
                    c->color = BLACK;
                }
                return;
            }
        } else {
            if (!isRed(b)) {
                Node* innerC = uIsRight ? b->right : b->left;
                Node* outerC = uIsRight ? b->left : b->right;

                if (!isRed(innerC) && !isRed(outerC)) {
                    b->color = RED;
                    u = a;
                    a = u->parent;
                } else {
                    if (isRed(innerC)) {
                        Node* c = innerC;
                        rotateUp(c);
                        rotateUp(c);
                        c->color = BLACK;
                        b->color = BLACK;
                        a->color = BLACK;
                    } else {
                        Node* c = outerC;
                        rotateUp(b);
                        b->color = BLACK;
                        a->color = BLACK;
                        c->color = BLACK;
                    }
                    return;
                }
            } else {
                Node* c = uIsRight ? b->right : b->left;
                Node* outerD = uIsRight ? c->left : c->right;
                Node* innerD = uIsRight ? c->right : c->left;

                if (!isRed(outerD) && !isRed(innerD)) {
                    rotateUp(b);
                    b->color = BLACK;
                    a->color = BLACK;
                    c->color = RED;
                    return;
                } else {
                    if (!isRed(outerD)) {
                        rotateUp(innerD);
                        innerD->color = BLACK;
                        c->color = RED;
                        outerD = c;
                        c = innerD;
                    }
                    Node* d = outerD;
                    rotateUp(c);
                    rotateUp(c);
                    c->color = BLACK;
                    b->color = RED;
                    a->color = BLACK;
                    d->color = BLACK;
                    return;
                }
            }
        }
    }
    if (root) root->color = BLACK;
}

Node* searchNode(long long key) {
    Node* curr = root;
    while (curr) {
        if (key == curr->key) return curr;
        curr = (key < curr->key) ? curr->left : curr->right;
    }
    return nullptr;
}

bool insertKey(long long key) {
    Node* parent = nullptr;
    Node* curr = root;

    while (curr) {
        parent = curr;
        if (key == curr->key) {
            return false;
        } else if (key < curr->key) {
            curr = curr->left;
        } else {
            curr = curr->right;
        }
    }

    Node* k = new Node(key);
    k->parent = parent;

    if (!parent) {
        root = k;
    } else if (key < parent->key) {
        parent->left = k;
    } else {
        parent->right = k;
    }

    updateAncestorsSize(parent);
    fixInsert(k);
    return true;
}

bool eraseKey(long long key) {
    Node* z = searchNode(key);
    if (!z) return false; 

    if (z->left && z->right) {
        Node* pred = z->left;
        while (pred->right) {
            pred = pred->right;
        }
        z->key = pred->key;
        z = pred;
    }

    Node* child = z->left ? z->left : z->right;
    Node* a = z->parent;

    if (child != nullptr) {
        child->parent = a;
        if (!a) {
            root = child;
        } else if (z == a->left) {
            a->left = child;
        } else {
            a->right = child;
        }
        child->color = BLACK;
        updateAncestorsSize(a);
        delete z;
        return true;
    }

    if (z == root) {
        root = nullptr;
        delete z;
        return true;
    }

    if (z->color == RED) {
        if (z == a->left) a->left = nullptr;
        else a->right = nullptr;
        updateAncestorsSize(a);
        delete z;
        return true;
    }

    if (z == a->left) a->left = nullptr;
    else a->right = nullptr;
    updateAncestorsSize(a);
    delete z;

    fixDeletion(nullptr, a);
    return true;
}

long long kthSmallest(int k) {
    Node* curr = root;
    while (curr) {
        int leftSz = getSize(curr->left);
        if (k == leftSz + 1) {
            return curr->key;
        } else if (k <= leftSz) {
            curr = curr->left;
        } else {
            k -= (leftSz + 1);
            curr = curr->right;
        }
    }
    return -1;
}

int rankOf(long long x) {
    int rank = 0;
    Node* curr = root;
    while (curr) {
        if (x <= curr->key) {
            curr = curr->left;
        } else {
            rank += 1 + getSize(curr->left);
            curr = curr->right;
        }
    }
    return rank;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    for (int i = 0; i < n; ++i) {
        long long x;
        cin >> x;
        insertKey(x);
    }

    int q;
    if (!(cin >> q)) return 0;

    while (q--) {
        int type;
        long long x;
        cin >> type >> x;

        if (type == 1) {
            if (insertKey(x)) {
                cout << x << " is inserted\n";
            } else {
                cout << x << " is already present. So no need to insert\n";
            }
        } else if (type == 2) {
            if (eraseKey(x)) {
                cout << x << " is deleted\n";
            } else {
                cout << x << " is not present. So it can not be deleted\n";
            }
        } else if (type == 3) {
            cout << kthSmallest(static_cast<int>(x)) << "\n";
        } else if (type == 4) {
            cout << rankOf(x) << "\n";
        }
    }

    return 0;
}