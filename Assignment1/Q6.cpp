#include <iostream>
#include <vector>
#include <queue>
#include <string>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
};

struct Info {
    bool valid;
    long long sum;
    long long minimum;
    long long maximum;
};

Info findMax(Node* root, long long& answer) {
    if (root == NULL)
        return {true, 0, 0, 0};

    Info left = findMax(root->left, answer);
    Info right = findMax(root->right, answer);

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

        if (sum > answer)
            answer = sum;

        return {true, sum, minimum, maximum};
    }

    return {false, 0, 0, 0};
}

int main() {
    int M;
    cin >> M;

    vector<string> tokens(M);

    for (int i = 0; i < M; ++i)
        cin>>tokens[i];

    if (M == 0 || tokens[0] == "null") {
        cout<<0<<'\n';
        return 0;
    }

    Node* root = new Node;
    root->data = stoi(tokens[0]);
    root->left = NULL;
    root->right = NULL;

    queue<Node*> q;
    q.push(root);

    int index = 1;

    while (!q.empty() && index < M) {
        Node* current = q.front();
        q.pop();

        if (index < M && tokens[index] != "null") {
            current->left = new Node;
            current->left->data = stoi(tokens[index]);
            current->left->left = NULL;
            current->left->right = NULL;
            q.push(current->left);
        }

        index++;

        if (index < M && tokens[index] != "null") {
            current->right = new Node;
            current->right->data = stoi(tokens[index]);
            current->right->left = NULL;
            current->right->right = NULL;
            q.push(current->right);
        }

        index++;
    }

    long long answer = 0;

    findMax(root, answer);

    cout<<answer<<'\n';

    return 0;
}