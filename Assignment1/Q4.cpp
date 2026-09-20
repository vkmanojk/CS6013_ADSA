#include <iostream>
#include <queue>
#include <stack>

using namespace std;

bool flag;
bool first;

struct node {
    int keys[3];
    int keyCount;
    bool isLeaf;
    struct node* children[4];
    struct node* parent;
};

// Function to print inorder traversal.
void printInOrder(node* root) {
    if (root == NULL)
        return;

    for (int i = 0; i < root->keyCount; ++i) {
        if (!root->isLeaf)
            printInOrder(root->children[i]);

        if (!first)
            cout << " ";

        cout << root->keys[i];
        first = false;
    }

    if (!root->isLeaf)
        printInOrder(root->children[root->keyCount]);
}

// Function to print level-order traversal.
void printLevelOrder(node* root) {
    if (root == NULL)
        return;

    queue<node*> q;
    q.push(root);

    while (!q.empty()) {
        int levelSize = q.size();
        bool firstNode = true;

        for (int i = 0; i < levelSize; ++i) {
            node* temp = q.front();
            q.pop();

            if (!firstNode)
                cout << " ";

            cout << "[";

            for (int j = 0; j < temp->keyCount; ++j) {
                if (j > 0)
                    cout << "|";

                cout << temp->keys[j];
            }

            cout << "]";

            firstNode = false;

            if (!temp->isLeaf) {
                for (int j = 0; j <= temp->keyCount; ++j)
                    q.push(temp->children[j]);
            }
        }

        cout << endl;
    }
}

// Function to print all traversals.
void displayTraversals(node* root) {
    cout << "INORDER: ";
    first = true;
    printInOrder(root);

    cout << "\nLEVELORDER:\n";
    printLevelOrder(root);
}

// Function to search for a key in the 2-4 tree.
node* search(node* root, int key) {
    node* temp = root;

    while (temp != NULL) {
        int i = 0;

        while (i < temp->keyCount && key > temp->keys[i])
            ++i;

        if (i < temp->keyCount && key == temp->keys[i]) {
            cout << "Key found\n";
            return temp;
        }

        if (temp->isLeaf)
            break;

        temp = temp->children[i];
    }

    cout << "Key not found\n";
    return NULL;
}

// Function to insert a key into the 2-4 tree.
node* insert(node* root, int key) {
    node* temp = root;

    // Check for a duplicate key.
    while (temp != NULL) {
        int i = 0;

        while (i < temp->keyCount && key > temp->keys[i])
            ++i;

        if (i < temp->keyCount && key == temp->keys[i]) {
            flag = true;
            return root;
        }

        if (temp->isLeaf)
            break;

        temp = temp->children[i];
    }

    // Create the first node.
    if (root == NULL) {
        root = new node;

        root->keyCount = 1;
        root->keys[0] = key;
        root->isLeaf = true;
        root->parent = NULL;

        for (int i = 0; i < 4; ++i)
            root->children[i] = NULL;

        return root;
    }

    // Split the root if it is full.
    if (root->keyCount == 3) {
        node* temp = root;
        node* right = new node;
        node* newRoot = new node;

        right->keyCount = 1;
        right->keys[0] = temp->keys[2];
        right->isLeaf = temp->isLeaf;
        right->parent = newRoot;

        for (int i = 0; i < 4; ++i)
            right->children[i] = NULL;

        if (!temp->isLeaf) {
            right->children[0] = temp->children[2];
            right->children[1] = temp->children[3];

            if (right->children[0] != NULL)
                right->children[0]->parent = right;

            if (right->children[1] != NULL)
                right->children[1]->parent = right;

            temp->children[2] = NULL;
            temp->children[3] = NULL;
        }

        newRoot->keyCount = 1;
        newRoot->keys[0] = temp->keys[1];
        newRoot->isLeaf = false;
        newRoot->children[0] = temp;
        newRoot->children[1] = right;
        newRoot->children[2] = NULL;
        newRoot->children[3] = NULL;
        newRoot->parent = NULL;

        temp->keyCount = 1;
        temp->parent = newRoot;

        root = newRoot;
    }

    temp = root;

    // Split full children before descending into them.
    while (!temp->isLeaf) {
        int i = 0;

        while (i < temp->keyCount && key > temp->keys[i])
            ++i;

        if (temp->children[i]->keyCount == 3) {
            node* child = temp->children[i];
            node* right = new node;

            right->keyCount = 1;
            right->keys[0] = child->keys[2];
            right->isLeaf = child->isLeaf;
            right->parent = temp;

            for (int j = 0; j < 4; ++j)
                right->children[j] = NULL;

            if (!child->isLeaf) {
                right->children[0] = child->children[2];
                right->children[1] = child->children[3];

                if (right->children[0] != NULL)
                    right->children[0]->parent = right;

                if (right->children[1] != NULL)
                    right->children[1]->parent = right;

                child->children[2] = NULL;
                child->children[3] = NULL;
            }

            int middle = child->keys[1];

            child->keyCount = 1;

            for (int j = temp->keyCount; j >= i + 1; --j)
                temp->children[j + 1] = temp->children[j];

            temp->children[i + 1] = right;

            for (int j = temp->keyCount - 1; j >= i; --j)
                temp->keys[j + 1] = temp->keys[j];

            temp->keys[i] = middle;
            temp->keyCount++;

            if (key == temp->keys[i]) {
                flag = true;
                return root;
            }

            if (key > temp->keys[i])
                ++i;
        }

        temp = temp->children[i];
    }

    // Insert into the non-full leaf.
    int i = temp->keyCount - 1;

    while (i >= 0 && key < temp->keys[i]) {
        temp->keys[i + 1] = temp->keys[i];
        --i;
    }

    temp->keys[i + 1] = key;
    temp->keyCount++;

    return root;
}

// Function to delete a key from the 2-4 tree.
node* deleteKey(node* root, int key) {
    node* temp = root;

    // Check whether the key exists.
    while (temp != NULL) {
        int i = 0;

        while (i < temp->keyCount && key > temp->keys[i])
            ++i;

        if (i < temp->keyCount && key == temp->keys[i])
            break;

        if (temp->isLeaf)
            return root;

        temp = temp->children[i];
    }

    if (temp == NULL)
        return root;

    flag = true;
    temp = root;

    while (temp != NULL) {
        int i = 0;

        while (i < temp->keyCount && key > temp->keys[i])
            ++i;

        // Key is present in this node.
        if (i < temp->keyCount && key == temp->keys[i]) {

            // Delete directly from a leaf.
            if (temp->isLeaf) {
                for (int j = i; j < temp->keyCount - 1; ++j)
                    temp->keys[j] = temp->keys[j + 1];

                temp->keyCount--;

                if (temp == root && temp->keyCount == 0) {
                    delete temp;
                    root = NULL;
                }

                return root;
            }

            // Use the predecessor if the left child has at least two keys.
            if (temp->children[i]->keyCount >= 2) {
                node* x = temp->children[i];

                while (!x->isLeaf)
                    x = x->children[x->keyCount];

                int predecessor = x->keys[x->keyCount - 1];

                temp->keys[i] = predecessor;
                key = predecessor;
                temp = temp->children[i];

                continue;
            }

            // Use the successor if the right child has at least two keys.
            if (temp->children[i + 1]->keyCount >= 2) {
                node* x = temp->children[i + 1];

                while (!x->isLeaf)
                    x = x->children[0];

                int successor = x->keys[0];

                temp->keys[i] = successor;
                key = successor;
                temp = temp->children[i + 1];

                continue;
            }

            // Merge the key with its two adjacent children.
            node* left = temp->children[i];
            node* right = temp->children[i + 1];

            int count = left->keyCount;

            left->keys[count] = temp->keys[i];
            count++;

            for (int j = 0; j < right->keyCount; ++j)
                left->keys[count + j] = right->keys[j];

            if (!left->isLeaf) {
                for (int j = 0; j <= right->keyCount; ++j) {
                    left->children[left->keyCount + 1 + j] =
                        right->children[j];

                    if (left->children[left->keyCount + 1 + j] != NULL)
                        left->children[left->keyCount + 1 + j]->parent = left;
                }
            }

            left->keyCount += right->keyCount + 1;

            for (int j = i; j < temp->keyCount - 1; ++j)
                temp->keys[j] = temp->keys[j + 1];

            for (int j = i + 1; j < temp->keyCount; ++j)
                temp->children[j] = temp->children[j + 1];

            temp->keyCount--;

            // Remove the unused child pointer.
            temp->children[temp->keyCount + 1] = NULL;

            right->parent = NULL;
            delete right;

            if (temp == root && temp->keyCount == 0) {
                root = left;
                left->parent = NULL;
                delete temp;
            }

            temp = left;
            continue;
        }

        // Key is not in this node.
        node* child = temp->children[i];

        // Make sure the child has at least two keys before descending.
        if (child->keyCount == 1) {

            // Borrow from the left sibling.
            if (i > 0 && temp->children[i - 1]->keyCount >= 2) {
                node* left = temp->children[i - 1];

                int oldChildKeyCount = child->keyCount;
                int oldLeftKeyCount = left->keyCount;

                for (int j = oldChildKeyCount; j > 0; --j)
                    child->keys[j] = child->keys[j - 1];

                child->keys[0] = temp->keys[i - 1];

                if (!child->isLeaf) {
                    for (int j = oldChildKeyCount + 1; j > 0; --j)
                        child->children[j] = child->children[j - 1];

                    child->children[0] =
                        left->children[oldLeftKeyCount];

                    if (child->children[0] != NULL)
                        child->children[0]->parent = child;

                    // The old last child position of left is now unused.
                    left->children[oldLeftKeyCount] = NULL;
                }

                temp->keys[i - 1] =
                    left->keys[oldLeftKeyCount - 1];

                left->keyCount--;
                child->keyCount++;
            }

            // Otherwise borrow from the right sibling.
            else if (i < temp->keyCount &&
                     temp->children[i + 1]->keyCount >= 2) {

                node* right = temp->children[i + 1];

                int oldChildKeyCount = child->keyCount;
                int oldRightKeyCount = right->keyCount;

                child->keys[oldChildKeyCount] = temp->keys[i];

                if (!child->isLeaf) {
                    child->children[oldChildKeyCount + 1] =
                        right->children[0];

                    if (child->children[oldChildKeyCount + 1] != NULL)
                        child->children[oldChildKeyCount + 1]->parent = child;
                }

                temp->keys[i] = right->keys[0];

                for (int j = 0; j < oldRightKeyCount - 1; ++j)
                    right->keys[j] = right->keys[j + 1];

                if (!right->isLeaf) {
                    for (int j = 0; j < oldRightKeyCount; ++j)
                        right->children[j] = right->children[j + 1];

                    // Last child position is now unused.
                    right->children[oldRightKeyCount] = NULL;
                }

                right->keyCount--;
                child->keyCount++;
            }

            // Otherwise merge with the left sibling.
            else if (i > 0) {
                node* right = temp->children[i];
                node* sibling = temp->children[i - 1];

                int oldParentKeyCount = temp->keyCount;
                int oldSiblingKeyCount = sibling->keyCount;

                int count = oldSiblingKeyCount;

                sibling->keys[count] = temp->keys[i - 1];
                count++;

                for (int j = 0; j < right->keyCount; ++j)
                    sibling->keys[count + j] = right->keys[j];

                if (!sibling->isLeaf) {
                    for (int j = 0; j <= right->keyCount; ++j) {
                        sibling->children[oldSiblingKeyCount + 1 + j] =
                            right->children[j];

                        if (sibling->children[oldSiblingKeyCount + 1 + j]
                            != NULL)
                            sibling->children[oldSiblingKeyCount + 1 + j]
                                ->parent = sibling;
                    }
                }

                sibling->keyCount += right->keyCount + 1;

                for (int j = i - 1; j < oldParentKeyCount - 1; ++j)
                    temp->keys[j] = temp->keys[j + 1];

                for (int j = i; j < oldParentKeyCount; ++j)
                    temp->children[j] = temp->children[j + 1];

                temp->keyCount--;

                // Last child position is now unused.
                temp->children[oldParentKeyCount] = NULL;

                right->parent = NULL;
                delete right;

                if (temp == root && temp->keyCount == 0) {
                    root = sibling;
                    sibling->parent = NULL;
                    delete temp;
                    temp = root;
                }

                child = sibling;
            }

            // Otherwise merge with the right sibling.
            else {
                node* left = temp->children[i];
                node* right = temp->children[i + 1];

                int oldParentKeyCount = temp->keyCount;
                int oldLeftKeyCount = left->keyCount;

                int count = oldLeftKeyCount;

                left->keys[count] = temp->keys[i];
                count++;

                for (int j = 0; j < right->keyCount; ++j)
                    left->keys[count + j] = right->keys[j];

                if (!left->isLeaf) {
                    for (int j = 0; j <= right->keyCount; ++j) {
                        left->children[oldLeftKeyCount + 1 + j] =
                            right->children[j];

                        if (left->children[oldLeftKeyCount + 1 + j] != NULL)
                            left->children[oldLeftKeyCount + 1 + j]
                                ->parent = left;
                    }
                }

                left->keyCount += right->keyCount + 1;

                for (int j = i; j < oldParentKeyCount - 1; ++j)
                    temp->keys[j] = temp->keys[j + 1];

                for (int j = i + 1; j < oldParentKeyCount; ++j)
                    temp->children[j] = temp->children[j + 1];

                temp->keyCount--;

                // Last child position is now unused.
                temp->children[oldParentKeyCount] = NULL;

                right->parent = NULL;
                delete right;

                if (temp == root && temp->keyCount == 0) {
                    root = left;
                    left->parent = NULL;
                    delete temp;
                    temp = root;
                }

                child = left;
            }
        }

        temp = child;
    }

    return root;
}

// Function to validate the 2-4 tree.
bool validate(node* root) {
    if (root == NULL)
        return true;

    if (root->parent != NULL)
        return false;

    queue<node*> q;
    queue<int> depths;
    queue<long long> lower;
    queue<long long> upper;

    q.push(root);
    depths.push(0);
    lower.push(-2147483649LL);
    upper.push(2147483648LL);

    int leafDepth = -1;

    while (!q.empty()) {
        node* temp = q.front();
        q.pop();

        int depth = depths.front();
        depths.pop();

        long long low = lower.front();
        lower.pop();

        long long high = upper.front();
        upper.pop();

        // Every node must contain one to three keys.
        if (temp->keyCount < 1 || temp->keyCount > 3)
            return false;

        // Keys inside a node must be strictly increasing.
        for (int i = 0; i < temp->keyCount; ++i) {

            if (temp->keys[i] <= low || temp->keys[i] >= high)
                return false;

            if (i > 0 && temp->keys[i - 1] >= temp->keys[i])
                return false;
        }

        if (temp->isLeaf) {

            // A leaf cannot have any children.
            for (int i = 0; i < 4; ++i) {
                if (temp->children[i] != NULL)
                    return false;
            }

            // All leaves must be at the same depth.
            if (leafDepth == -1)
                leafDepth = depth;
            else if (leafDepth != depth)
                return false;
        }

        else {

            // An internal node with k keys has exactly k+1 children.
            for (int i = 0; i <= temp->keyCount; ++i) {

                if (temp->children[i] == NULL)
                    return false;

                if (temp->children[i]->parent != temp)
                    return false;

                long long childLow = low;
                long long childHigh = high;

                if (i > 0)
                    childLow = temp->keys[i - 1];

                if (i < temp->keyCount)
                    childHigh = temp->keys[i];

                q.push(temp->children[i]);
                depths.push(depth + 1);
                lower.push(childLow);
                upper.push(childHigh);
            }

            // Unused child positions must be NULL.
            for (int i = temp->keyCount + 1; i < 4; ++i) {
                if (temp->children[i] != NULL)
                    return false;
            }
        }
    }

    return true;
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
    cout << "END\n";

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

        if (flag) {
            cout << "Duplicate Key\n";
        }
        else {
            displayTraversals(root);

            if (validate(root))
                cout << "VALID\n";
            else
                cout << "INVALID\n";

            cout << "END\n";
        }
    }

    cin >> D;

    for (int i = 0; i < D; ++i) {
        cin >> key;

        cout << "DELETE " << key << endl;

        flag = false;
        root = deleteKey(root, key);

        if (root != NULL)
            root->parent = NULL;

        if (!flag) {
            cout << "Key not found\n";
        }
        else {
            displayTraversals(root);

            if (validate(root))
                cout << "VALID\n";
            else
                cout << "INVALID\n";

            cout << "END\n";
        }
    }

    return 0;
}