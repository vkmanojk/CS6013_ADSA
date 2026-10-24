#include <iostream>
#include <queue>

using namespace std;

int MAX, MIN;

struct BTreeNode {
    int *key;
    int count;
    struct BTreeNode **children;
};

// Allocate and initialize a new B-tree node
BTreeNode* createNode() {
    BTreeNode* node = new BTreeNode;
    node->key = new int[MAX];
    node->count = 0;
    node->children = new BTreeNode*[MAX + 1];
    for (int i = 0; i <= MAX; ++i)
        node->children[i] = nullptr;
    return node;
}

bool isLeaf(BTreeNode* node) {
    return node->children[0] == nullptr;
}

// Search key in B-tree
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

// Split full child node y at index i
void splitChild(BTreeNode* x, int i) {
    BTreeNode* y = x->children[i];
    BTreeNode* z = createNode();
    z->count = MIN;

    for (int j = 0; j < MIN; ++j)
        z->key[j] = y->key[j + MIN + 1];

    if (!isLeaf(y)) {
        for (int j = 0; j <= MIN; ++j) {
            z->children[j] = y->children[j + MIN + 1];
            y->children[j + MIN + 1] = nullptr;
        }
    }

    int mid_key = y->key[MIN];
    y->count = MIN;

    for (int j = x->count; j >= i + 1; --j)
        x->children[j + 1] = x->children[j];
    x->children[i + 1] = z;

    for (int j = x->count - 1; j >= i; --j)
        x->key[j + 1] = x->key[j];
    x->key[i] = mid_key;
    x->count++;
}

// Top-down insert into non-full node
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
            --i;
        ++i;

        if (x->children[i]->count == MAX) {
            splitChild(x, i);
            if (k > x->key[i])
                ++i;
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

    if (root->count == MAX) {
        BTreeNode* s = createNode();
        s->children[0] = root;
        splitChild(s, 0);
        root = s;
    }
    insertNonFull(root, key);
    return true;
}

int getPredecessor(BTreeNode* y) {
    BTreeNode* cur = y;
    while (!isLeaf(cur))
        cur = cur->children[cur->count];
    return cur->key[cur->count - 1];
}

int getSuccessor(BTreeNode* z) {
    BTreeNode* cur = z;
    while (!isLeaf(cur))
        cur = cur->children[0];
    return cur->key[0];
}

void borrowFromLeft(BTreeNode* x, int idx) {
    BTreeNode* child = x->children[idx];
    BTreeNode* left_sib = x->children[idx - 1];

    for (int i = child->count - 1; i >= 0; --i)
        child->key[i + 1] = child->key[i];

    if (!isLeaf(child)) {
        for (int i = child->count; i >= 0; --i)
            child->children[i + 1] = child->children[i];
        child->children[0] = left_sib->children[left_sib->count];
        left_sib->children[left_sib->count] = nullptr;
    }

    child->key[0] = x->key[idx - 1];
    x->key[idx - 1] = left_sib->key[left_sib->count - 1];

    child->count++;
    left_sib->count--;
}

void borrowFromRight(BTreeNode* x, int idx) {
    BTreeNode* child = x->children[idx];
    BTreeNode* right_sib = x->children[idx + 1];

    child->key[child->count] = x->key[idx];

    if (!isLeaf(child))
        child->children[child->count + 1] = right_sib->children[0];

    x->key[idx] = right_sib->key[0];

    for (int i = 1; i < right_sib->count; ++i)
        right_sib->key[i - 1] = right_sib->key[i];

    if (!isLeaf(right_sib)) {
        for (int i = 1; i <= right_sib->count; ++i)
            right_sib->children[i - 1] = right_sib->children[i];
        right_sib->children[right_sib->count] = nullptr;
    }

    child->count++;
    right_sib->count--;
}

void mergeChildren(BTreeNode* x, int idx) {
    BTreeNode* y = x->children[idx];
    BTreeNode* z = x->children[idx + 1];

    y->key[MIN] = x->key[idx];

    for (int i = 0; i < MIN; ++i)
        y->key[MIN + 1 + i] = z->key[i];

    if (!isLeaf(y)) {
        for (int i = 0; i <= MIN; ++i)
            y->children[MIN + 1 + i] = z->children[i];
    }

    y->count = MAX;

    for (int i = idx + 1; i < x->count; ++i)
        x->key[i - 1] = x->key[i];

    for (int i = idx + 2; i <= x->count; ++i)
        x->children[i - 1] = x->children[i];

    x->children[x->count] = nullptr;
    x->count--;

    delete[] z->key;
    delete[] z->children;
    delete z;
}

void deleteFromNode(BTreeNode* x, int k) {
    int idx = 0;
    while (idx < x->count && x->key[idx] < k)
        ++idx;

    if (idx < x->count && x->key[idx] == k) {
        if (isLeaf(x)) {
            for (int i = idx + 1; i < x->count; ++i)
                x->key[i - 1] = x->key[i];
            x->count--;
        } else {
            BTreeNode* y = x->children[idx];
            BTreeNode* z = x->children[idx + 1];

            if (y->count > MIN) {
                int pred = getPredecessor(y);
                x->key[idx] = pred;
                deleteFromNode(y, pred);
            } else if (z->count > MIN) {
                int succ = getSuccessor(z);
                x->key[idx] = succ;
                deleteFromNode(z, succ);
            } else {
                mergeChildren(x, idx);
                deleteFromNode(y, k);
            }
        }
    } else {
        BTreeNode* child = x->children[idx];

        if (child->count == MIN) {
            if (idx > 0 && x->children[idx - 1]->count > MIN)
                borrowFromLeft(x, idx);
            else if (idx < x->count && x->children[idx + 1]->count > MIN)
                borrowFromRight(x, idx);
            else if (idx < x->count) {
                mergeChildren(x, idx);
                child = x->children[idx];
            } else {
                mergeChildren(x, idx - 1);
                child = x->children[idx - 1];
            }
        }
        deleteFromNode(child, k);
    }
}

bool deleteKey(BTreeNode*& root, int key) {
    if (!search(root, key))
        return false;

    deleteFromNode(root, key);

    if (root->count == 0) {
        BTreeNode* old_root = root;
        if (isLeaf(root))
            root = nullptr;
        else
            root = root->children[0];
        delete[] old_root->key;
        delete[] old_root->children;
        delete old_root;
    }

    return true;
}

void levelOrderDisplay(BTreeNode* root) {
    if (root == nullptr) {
        cout << "EMPTY\nEND\n";
        return;
    }

    queue<BTreeNode*> q;
    q.push(root);

    while (!q.empty()) {
        int level_sz = q.size();
        for (int i = 0; i < level_sz; ++i) {
            BTreeNode* curr = q.front();
            q.pop();

            if (i > 0)
                cout << " ";
            for (int j = 0; j < curr->count; ++j) {
                if (j > 0)
                    cout << ",";
                cout << curr->key[j];
            }

            if (!isLeaf(curr)) {
                for (int j = 0; j <= curr->count; ++j)
                    if (curr->children[j] != nullptr)
                        q.push(curr->children[j]);
            }
        }
        cout << "\n";
    }
    cout << "END\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n >> MAX >> MIN))
        return 0;

    BTreeNode* root = nullptr;
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
