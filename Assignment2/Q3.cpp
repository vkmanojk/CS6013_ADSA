#include <iostream>
using namespace std;

enum Color { RED, BLACK };

struct Node {
    long long key;
    Color color;
    Node *left, *right, *parent;

    Node(long long k) {
        key = k;
        color = RED;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
};

Node* root = nullptr;

bool isRed(Node* u) {
    return (u != nullptr && u->color == RED);
}

void rotateLeft(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if (y->left) y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent) 
        root = y;
    else if (x == x->parent->left) 
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void rotateRight(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if (y->right) y->right->parent = x;
    y->parent = x->parent;
    if (!x->parent) 
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->right = x;
    x->parent = y;
}

void rotateUp(Node* childNode) {
    Node* p = childNode->parent;
    if (!p) return;
    if (childNode == p->left) 
        rotateRight(p);
    else 
        rotateLeft(p);
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
    if (root) 
        root->color = BLACK;
}

void insertKey(long long key) {
    Node* parent = nullptr;
    Node* curr = root;
    while (curr) {
        parent = curr;
        if (key == curr->key) 
            return; // Distinct keys only, ignore duplicates
        else if (key < curr->key) 
            curr = curr->left;
        else 
            curr = curr->right;
    }
    
    Node* k = new Node(key);
    k->parent = parent;
    
    if (!parent)
        root = k;
    else if (key < parent->key)
        parent->left = k;
    else
        parent->right = k;
        
    fixInsert(k);
}

void rbEnumerate(Node* x, long long a, long long b, bool& found) {
    if (!x) return;
    
    // If current key is strictly greater than 'a', valid keys might exist in the left subtree.
    if (x->key > a) {
        rbEnumerate(x->left, a, b, found);
    }
    
    // If current key is within the range [a, b], process it.
    if (x->key >= a && x->key <= b) {
        if (found) cout << " ";
        cout << x->key;
        found = true;
    }
    
    // If current key is strictly less than 'b', valid keys might exist in the right subtree.
    if (x->key < b) {
        rbEnumerate(x->right, a, b, found);
    }
}

int main() {
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
        long long a, b;
        cin >> a >> b;
        
        bool found = false;
        rbEnumerate(root, a, b, found);
        
        if (!found) {
            cout << "EMPTY";
        }
        cout << "\n";
    }
    
    return 0;
}