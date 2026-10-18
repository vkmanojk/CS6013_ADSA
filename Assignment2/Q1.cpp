#include <iostream>
#include <queue>

using namespace std;

int MAX, MIN;
struct BTreeNode {
    int *key;                    // allocate MAX entries
    int count;
    struct BTreeNode **children; // allocate MAX + 1 entries
};

// Allocate and initialize a new B-tree node
BTreeNode* createNode() {
    BTreeNode* node = new BTreeNode;
    node->key = new int[MAX];
    node->count = 0;
    node->children = new BTreeNode*[MAX + 1];
    for (int i = 0; i <= MAX; ++i) 
        node->children[i] = nullptr; //Unused children set as null
    return node;
}

bool isLeaf(BTreeNode* node) {
    return node->children[0] == nullptr;
}

// (a) search(root, key)
bool search(BTreeNode* root, int key) {
    if (root == nullptr) 
        return false;
    int i = 0;
    while (i < root->count && key > root->key[i]) 
        ++i;
    if (i < root->count && root->key[i] == key) 
        return true;
    if (isLeaf(root)) 
        return false;
    return search(root->children[i], key);
}

// (b) insert(root, key)
void splitChild(BTreeNode* x, int i) {
    BTreeNode* y = x->children[i];
    BTreeNode* z = createNode();
    z->count = MIN;

    // Copy the last MIN keys of y into z
    for (int j = 0; j < MIN; ++j) {
        z->key[j] = y->key[j + MIN + 1];
    }

    // If y is an internal node, copy the last MIN + 1 children of y into z
    if (!isLeaf(y)) {
        for (int j = 0; j <= MIN; ++j) {
            z->children[j] = y->children[j + MIN + 1];
            y->children[j + MIN + 1] = nullptr;
        }
    }

    int middleKey = y->key[MIN];
    y->count = MIN;

    // Shift children of x to make room for z
    for (int j = x->count; j >= i + 1; --j) 
        x->children[j + 1] = x->children[j];
    x->children[i + 1] = z;

    // Shift keys of x to promote middleKey
    for (int j = x->count - 1; j >= i; --j) 
        x->key[j + 1] = x->key[j];
    x->key[i] = middleKey;
    x->count++;
}

// Insert key k into a non-full node x
void insertNonFull(BTreeNode* x, int k) {
    int i = x->count - 1;

    if (isLeaf(x)) {
        while (i >= 0 && k < x->key[i]) {
            x->key[i + 1] = x->key[i];
            --i;
        }
        x->key[i + 1] = k;
        x->count++;
    } else {
        while (i >= 0 && k < x->key[i])
            i--;
        i++; // Child index to descend into

        // Split full child before descending into it
        if (x->children[i]->count == MAX) {
            splitChild(x, i);
            if (k > x->key[i]) {
                i++;
            }
        }
        insertNonFull(x->children[i], k);
    }
}

bool insert(BTreeNode*& root, int key) {
    if (search(root, key)) 
        return false;
    
    if (root == nullptr) {
        root = createNode();
        root->key[0] = key;
        root->count = 1;
        return true;
    }

    // Split a full root before insertion
    if (root->count == MAX) {
        BTreeNode* s = createNode();
        s->children[0] = root;
        splitChild(s, 0);
        root = s;
    }
    insertNonFull(root, key);
    return true;
}

// (c) deleteKey(root, key) - Top-down deletion
int getPredecessor(BTreeNode* y) {
    BTreeNode* cur = y;
    while (!isLeaf(cur)) {
        cur = cur->children[cur->count];
    }
    return cur->key[cur->count - 1];
}

int getSuccessor(BTreeNode* z) {
    BTreeNode* cur = z;
    while (!isLeaf(cur)) 
        cur = cur->children[0];
    return cur->key[0];
}

// Borrow a key from left sibling x->children[idx - 1] into x->children[idx]
void borrowFromLeft(BTreeNode* x, int idx) {
    BTreeNode* child = x->children[idx];
    BTreeNode* leftSib = x->children[idx - 1];

    for (int i = child->count - 1; i >= 0; --i) 
        child->key[i + 1] = child->key[i];

    if (!isLeaf(child)) {
        for (int i = child->count; i >= 0; --i) 
            child->children[i + 1] = child->children[i];
        child->children[0] = leftSib->children[leftSib->count];
        leftSib->children[leftSib->count] = nullptr;
    }

    child->key[0] = x->key[idx - 1];
    x->key[idx - 1] = leftSib->key[leftSib->count - 1];

    child->count++;
    leftSib->count--;
}

// Borrow a key from right sibling x->children[idx + 1] into x->children[idx]
void borrowFromRight(BTreeNode* x, int idx) {
    BTreeNode* child = x->children[idx];
    BTreeNode* rightSib = x->children[idx + 1];

    child->key[child->count] = x->key[idx];

    if (!isLeaf(child)) {
        child->children[child->count + 1] = rightSib->children[0];
    }

    x->key[idx] = rightSib->key[0];

    for (int i = 1; i < rightSib->count; ++i) {
        rightSib->key[i - 1] = rightSib->key[i];
    }

    if (!isLeaf(rightSib)) {
        for (int i = 1; i <= rightSib->count; ++i) {
            rightSib->children[i - 1] = rightSib->children[i];
        }
        rightSib->children[rightSib->count] = nullptr;
    }

    child->count++;
    rightSib->count--;
}

// Merge y = x->children[idx] and z = x->children[idx + 1] with separating key x->key[idx]
void mergeChildren(BTreeNode* x, int idx) {
    BTreeNode* y = x->children[idx];
    BTreeNode* z = x->children[idx + 1];

    y->key[MIN] = x->key[idx];

    for (int i = 0; i < MIN; ++i) {
        y->key[MIN + 1 + i] = z->key[i];
    }

    if (!isLeaf(y)) {
        for (int i = 0; i <= MIN; ++i) {
            y->children[MIN + 1 + i] = z->children[i];
        }
    }

    y->count = MAX;

    for (int i = idx + 1; i < x->count; ++i) {
        x->key[i - 1] = x->key[i];
    }

    for (int i = idx + 2; i <= x->count; ++i) {
        x->children[i - 1] = x->children[i];
    }
    x->children[x->count] = nullptr;
    x->count--;

    delete[] z->key;
    delete[] z->children;
    delete z;
}

void deleteFromNode(BTreeNode* x, int k) {
    int idx = 0;
    while (idx < x->count && x->key[idx] < k) {
        idx++;
    }

    // Key k is found in node x
    if (idx < x->count && x->key[idx] == k) {
        if (isLeaf(x)) {
            // Case 1: x is a leaf node
            for (int i = idx + 1; i < x->count; ++i) {
                x->key[i - 1] = x->key[i];
            }
            x->count--;
        } else {
            // Case 2: x is an internal node
            BTreeNode* y = x->children[idx];
            BTreeNode* z = x->children[idx + 1];

            if (y->count > MIN) {
                // Case 2a: left adjacent child y has > MIN keys
                int pred = getPredecessor(y);
                x->key[idx] = pred;
                deleteFromNode(y, pred);
            } else if (z->count > MIN) {
                // Case 2b: right adjacent child z has > MIN keys
                int succ = getSuccessor(z);
                x->key[idx] = succ;
                deleteFromNode(z, succ);
            } else {
                // Case 2c: both y and z have MIN keys; merge them with k
                mergeChildren(x, idx);
                deleteFromNode(y, k);
            }
        }
    } else {
        // Key k is in child x->children[idx]
        BTreeNode* child = x->children[idx];

        // Ensure child has > MIN keys before descending
        if (child->count == MIN) {
            if (idx > 0 && x->children[idx - 1]->count > MIN) {
                borrowFromLeft(x, idx);
            } else if (idx < x->count && x->children[idx + 1]->count > MIN) {
                borrowFromRight(x, idx);
            } else {
                // Neither sibling can lend: merge with right sibling if it exists, else left
                if (idx < x->count) {
                    mergeChildren(x, idx);
                    child = x->children[idx];
                } else {
                    mergeChildren(x, idx - 1);
                    child = x->children[idx - 1];
                }
            }
        }
        deleteFromNode(child, k);
    }
}

bool deleteKey(BTreeNode*& root, int key) {
    // Deletions of absent keys must leave the tree structure unchanged
    if (!search(root, key)) 
        return false;

    deleteFromNode(root, key);

    // Replace an empty internal root with its only child, or null if empty tree
    if (root->count == 0) {
        BTreeNode* oldRoot = root;
        if (isLeaf(root)) 
            root = nullptr;
        else 
            root = root->children[0];
        delete[] oldRoot->key;
        delete[] oldRoot->children;
        delete oldRoot;
    }

    return true;
}

// (d) Level-order display, one tree level per line
void levelOrderDisplay(BTreeNode* root) {
    if (root == nullptr) {
        cout << "EMPTY\nEND\n";
        return;
    }

    queue<BTreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = q.size();
        for (int i = 0; i < levelSize; ++i) {
            BTreeNode* curr = q.front();
            q.pop();

            if (i > 0) cout << " ";
            for (int j = 0; j < curr->count; ++j) {
                if (j > 0) cout << ",";
                cout << curr->key[j];
            }

            if (!isLeaf(curr)) {
                for (int j = 0; j <= curr->count; ++j) {
                    if (curr->children[j] != nullptr) {
                        q.push(curr->children[j]);
                    }
                }
            }
        }
        cout << "\n";
    }
    cout << "END\n";
}

int main() {
    int n;
    if (!(cin >> n >> MAX >> MIN)) 
        return 0;

    BTreeNode* root = nullptr;
    // Insert initial keys
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        insert(root, x);
    }

    int q;
    if (!(cin >> q)) 
        return 0;

    while (q--) {
        int type;
        cin >> type;
        if (type == 1) {
            int x;
            cin >> x;
            if (search(root, x))
                cout << x << " is present\n";
            else 
                cout << x << " is not present\n";
            
        } else if (type == 2) {
            int x;
            cin >> x;
            if (insert(root, x)) 
                cout << x << " is inserted\n";
            else 
                cout << x << " is already present\n";
            
        } else if (type == 3) {
            int x;
            cin >> x;
            if (deleteKey(root, x))
                cout << x << " is deleted\n";
            else 
                cout << x << " is not present. So it can not be deleted\n";
            
        } else if (type == 4) 
            levelOrderDisplay(root);
    }

    return 0;
}