#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <limits>
using namespace std;

// Structure for a binary tree node.
struct node {
    int data;
    node* left;
    node* right;
};

// Structure to hold BST validation details for each subtree.
struct Info {
    bool valid;
    long long sum;
    long long minimum;
    long long maximum;
};

// Function to recursively find the maximum sum of any valid BST subtree.
Info findMax(node* root, long long& answer) {
    // Base case: An empty tree is a valid BST with sum 0.
    if (root == NULL)
        return {true, 0, 0, 0};

    Info left = findMax(root->left, answer);
    Info right = findMax(root->right, answer);

    // Check if the curr subtree is a valid BST.
    if (left.valid && right.valid &&
        (root->left == NULL || left.maximum < root->data) &&
        (root->right == NULL || root->data < right.minimum)) {

        long long sum = left.sum + root->data + right.sum;
        long long minimum = root->data;
        long long maximum = root->data;

        if (root->left != NULL)
            minimum = left.minimum;

        if (root->right != NULL)
            maximum = right.maximum;

        // Update the maximum sum found so far.
        if (sum > answer)
            answer = sum;

        return {true, sum, minimum, maximum};
    }

    // Return false if the curr subtree violates BST properties.
    return {false, 0, 0, 0};
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
    int M;
    cin>>M;
    if (M == 0) {
        cout << 0 << '\n';
        return 0;
    }

    vector<string> tokens(M);

    for (int i = 0; i < M; ++i)
        cin>>tokens[i];

    if (tokens[0] == "null") {
        cout<<0<<endl;
        return 0;
    }

    // Initialize the root node.
    node* root = new node;
    root->data = stoi(tokens[0]);
    root->left = NULL;
    root->right = NULL;

    queue<node*> q;
    q.push(root);

    int index = 1;

    // Construct the tree using level-order deserialization.
    while (!q.empty() && index < M) {
        node* curr = q.front();
        q.pop();

        if (index < M && tokens[index] != "null") {
            curr->left = new node;
            curr->left->data = stoi(tokens[index]);
            curr->left->left = NULL;
            curr->left->right = NULL;
            q.push(curr->left);
        }

        index++;

        if (index < M && tokens[index] != "null") {
            curr->right = new node;
            curr->right->data = stoi(tokens[index]);
            curr->right->left = NULL;
            curr->right->right = NULL;
            q.push(curr->right);
        }

        index++;
    }

    long long answer = std::numeric_limits<long long>::min();

    findMax(root, answer);

    // Answer is 0 if none have a positive contribution
    if (answer < 0) answer = 0;

    cout<<answer<<'\n';

    freeTree(root);
    return 0;
}