#include <iostream>
#include <vector>
using namespace std;

// Custom BigInteger class to handle massive combinatorial results.
struct BigInteger {
    vector<int> digits;

    BigInteger(int value = 0) {
        if (value == 0)
            digits.push_back(0);

        while (value > 0) {
            digits.push_back(value % 1000000000);
            value /= 1000000000;
        }
    }

    void multiply(int value) {
        long long carry = 0;

        for (int i = 0; i < (int)digits.size(); i++) {
            long long current = 1LL * digits[i] * value + carry;
            digits[i] = current % 1000000000;
            carry = current / 1000000000;
        }

        while (carry > 0) {
            digits.push_back(carry % 1000000000);
            carry /= 1000000000;
        }
    }

    void divide(int value) {
        long long remainder = 0;

        for (int i = digits.size() - 1; i >= 0; i--) {
            long long current = remainder * 1000000000LL + digits[i];
            digits[i] = current / value;
            remainder = current % value;
        }

        while (digits.size() > 1 && digits.back() == 0)
            digits.pop_back();
    }

    BigInteger operator*(const BigInteger& other) {
        BigInteger result(0);
        result.digits.assign(digits.size() + other.digits.size(), 0);

        for (int i = 0; i < (int)digits.size(); i++) {
            long long carry = 0;

            for (int j = 0; j < (int)other.digits.size(); j++) {
                long long current = 1LL * digits[i] * other.digits[j]
                                  + result.digits[i + j]
                                  + carry;

                result.digits[i + j] = current % 1000000000;
                carry = current / 1000000000;
            }

            int j = other.digits.size();

            while (carry > 0) {
                long long current = 1LL * result.digits[i + j] + carry;
                result.digits[i + j] = current % 1000000000;
                carry = current / 1000000000;
                j++;
            }
        }

        while (result.digits.size() > 1 && result.digits.back() == 0)
            result.digits.pop_back();

        return result;
    }

    void subtractOne() {
        int i = 0;

        while (digits[i] == 0) {
            digits[i] = 999999999;
            i++;
        }

        digits[i]--;

        while (digits.size() > 1 && digits.back() == 0)
            digits.pop_back();
    }

    void print() {
        cout << digits.back();

        for (int i = digits.size() - 2; i >= 0; i--) {
            cout.width(9);
            cout.fill('0');
            cout << digits[i];
        }

        cout << endl;
    }
};

// BST Node augmented with subtreeSize to remove the need for manual counting.
struct Node {
    int key;
    int subtreeSize;
    Node* left;
    Node* right;
};

// Function to safely extract subtree size.
int getSize(Node* root) {
    if (root == NULL)
        return 0;
    return root->subtreeSize;
}

// Function to insert a key, reject duplicates, and update sizes.
Node* insert(Node* root, int key) {
    if (root == NULL) {
        Node* newNode = new Node;
        newNode->key = key;
        newNode->subtreeSize = 1;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    if (key < root->key)
        root->left = insert(root->left, key);
    else if (key > root->key)
        root->right = insert(root->right, key);
    else
        return root; // Properly prevent duplicate keys.

    root->subtreeSize = 1 + getSize(root->left) + getSize(root->right);
    return root;
}

// Function to calculate the combinatorial (n choose k).
BigInteger combination(int n, int k) {
    if (k > n - k)
        k = n - k;

    BigInteger result(1);

    for (int i = 1; i <= k; i++) {
        result.multiply(n - k + i);
        result.divide(i);
    }

    return result;
}

// Function to recursively calculate total valid permutations.
BigInteger countWays(Node* root) {
    if (root == NULL)
        return BigInteger(1);

    int leftSize = getSize(root->left);
    int rightSize = getSize(root->right);

    // Calculate ways to interleave the left and right subtrees.
    BigInteger ways = combination(leftSize + rightSize, leftSize);

    // Multiply by the internal permutations of each subtree.
    ways = ways * countWays(root->left);
    ways = ways * countWays(root->right);

    return ways;
}

// Function to deallocate the BST.
void freeTree(Node* root) {
    if (root == NULL)
        return;
    
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

int main() {
    int n;
    cin >> n;

    Node* root = NULL;

    for (int i = 0; i < n; i++) {
        int key;
        cin >> key;
        root = insert(root, key);
    }

    // Calculate total ways and subtract 1 to exclude the original array.
    BigInteger answer = countWays(root);
    answer.subtractOne();
    answer.print();

    freeTree(root);

    return 0;
}