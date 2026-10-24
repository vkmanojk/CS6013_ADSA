#include <iostream>

using namespace std;

enum Color { RED, BLACK };

struct Node {
    long long key;
    Color color;
    Node *left, *right, *parent;

    Node(long long val) {
        key = val;
        color = RED;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
};

Node* root = nullptr;

bool is_red(Node* u) {
    return (u != nullptr && u->color == RED);
}

void rotate_left(Node* x) {
    Node* y = x->right;
    x->right = y->left;
    if(y->left)
        y->left->parent = x;
    y->parent = x->parent;
    if(!x->parent)
        root = y;
    else if(x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

void rotate_right(Node* x) {
    Node* y = x->left;
    x->left = y->right;
    if(y->right)
        y->right->parent = x;
    y->parent = x->parent;
    if(!x->parent)
        root = y;
    else if(x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->right = x;
    x->parent = y;
}

void rotate_up(Node* child) {
    Node* p = child->parent;
    if(!p)
        return;
    if(child == p->left)
        rotate_right(p);
    else
        rotate_left(p);
}

void fix_insert(Node* k) {
    while(k != root && is_red(k->parent)) {
        Node* a = k->parent;
        Node* b = a->parent;
        Node* c = (a == b->left) ? b->right : b->left;

        if(is_red(c)) {
            a->color = BLACK;
            c->color = BLACK;
            b->color = RED;
            k = b;
        } else {
            bool a_is_left = (a == b->left);
            bool k_is_left = (k == a->left);
            if(a_is_left == k_is_left) {
                rotate_up(a);
                a->color = BLACK;
                b->color = RED;
            } else {
                rotate_up(k);
                rotate_up(k);
                k->color = BLACK;
                b->color = RED;
            }
            break;
        }
    }
    if(root)
        root->color = BLACK;
}

void insert_key(long long key) {
    Node* p = nullptr;
    Node* curr = root;
    while(curr) {
        p = curr;
        if(key == curr->key)
            return;
        else if(key < curr->key)
            curr = curr->left;
        else
            curr = curr->right;
    }

    Node* k = new Node(key);
    k->parent = p;

    if(!p)
        root = k;
    else if(key < p->key)
        p->left = k;
    else
        p->right = k;

    fix_insert(k);
}

// Range enumeration: visits nodes in [low, high] in increasing order
void rb_enumerate(Node* u, long long low, long long high, bool& printed_any) {
    if(!u)
        return;

    if(u->key > low)
        rb_enumerate(u->left, low, high, printed_any);

    if(u->key >= low && u->key <= high) {
        if(printed_any)
            cout<<" ";
        cout<<u->key;
        printed_any = true;
    }

    if(u->key < high)
        rb_enumerate(u->right, low, high, printed_any);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if(!(cin>>n))
        return 0;

    for(int i = 0; i < n; ++i) {
        long long x;
        cin>>x;
        insert_key(x);
    }

    int q;
    if(!(cin>>q))
        return 0;

    while(q--) {
        long long low, high;
        cin>>low>>high;

        bool printed_any = false;
        rb_enumerate(root, low, high, printed_any);

        if(!printed_any)
            cout<<"EMPTY";
        cout<<"\n";
    }

    return 0;
}
