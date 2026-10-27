#include <iostream>

using namespace std;

enum Color { RED, BLACK };

struct Node {
    long long key;
    Color color;
    int size;
    Node *left, *right, *parent;

    Node(long long val) {
        key = val;
        color = RED;
        size = 1;
        left = nullptr;
        right = nullptr;
        parent = nullptr;
    }
};

Node* root = nullptr;

int get_size(Node* u) {
    return u ? u->size : 0;
}

void update_size(Node* u) {
    if(u)
        u->size = 1+get_size(u->left)+get_size(u->right);
}

void update_ancestors(Node* u) {
    while(u) {
        update_size(u);
        u = u->parent;
    }
}

bool is_red(Node* u) {
    return (u != nullptr && u->color == RED);
}

// Left rotation preserving subtree sizes
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
    update_size(x);
    update_size(y);
}

// Right rotation preserving subtree sizes
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
    update_size(x);
    update_size(y);
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

// Fix Red-Black properties after insertion
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
        }
        else {
            bool a_is_left = (a == b->left);
            bool k_is_left = (k == a->left);
            if(a_is_left == k_is_left) {
                rotate_up(a);
                a->color = BLACK;
                b->color = RED;
            }
            else {
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

// Fix Red-Black properties after deletion
void fix_deletion(Node* u, Node* a) {
    while(u != root && a != nullptr) {
        bool u_is_right = (u == a->right);
        Node* b = u_is_right ? a->left : a->right;
        if(u == nullptr) {
            b = (a->left != nullptr) ? a->left : a->right;
            u_is_right = (b == a->left);
        }

        if(is_red(a)) {
            Node* inner_c = u_is_right ? b->right : b->left;
            Node* outer_c = u_is_right ? b->left : b->right;
            if(!is_red(inner_c) && !is_red(outer_c)) {
                a->color = BLACK;
                b->color = RED;
                return;
            }
            else {
                if(is_red(inner_c)) {
                    Node* c = inner_c;
                    rotate_up(c);
                    rotate_up(c);
                    c->color = RED;
                    a->color = BLACK;
                    b->color = BLACK;
                }
                else {
                    Node* c = outer_c;
                    rotate_up(b);
                    b->color = RED;
                    a->color = BLACK;
                    c->color = BLACK;
                }
                return;
            }
        }
        else {
            if(!is_red(b)) {
                Node* inner_c = u_is_right ? b->right : b->left;
                Node* outer_c = u_is_right ? b->left : b->right;
                if(!is_red(inner_c) && !is_red(outer_c)) {
                    b->color = RED;
                    u = a;
                    a = u->parent;
                }
                else {
                    if(is_red(inner_c)) {
                        Node* c = inner_c;
                        rotate_up(c);
                        rotate_up(c);
                        c->color = BLACK;
                        b->color = BLACK;
                        a->color = BLACK;
                    }
                    else {
                        Node* c = outer_c;
                        rotate_up(b);
                        b->color = BLACK;
                        a->color = BLACK;
                        c->color = BLACK;
                    }
                    return;
                }
            }
            else {
                Node* c = u_is_right ? b->right : b->left;
                Node* outer_d = u_is_right ? c->left : c->right;
                Node* inner_d = u_is_right ? c->right : c->left;
                if(!is_red(outer_d) && !is_red(inner_d)) {
                    rotate_up(b);
                    b->color = BLACK;
                    a->color = BLACK;
                    c->color = RED;
                    return;
                }
                else {
                    if(!is_red(outer_d)) {
                        rotate_up(inner_d);
                        inner_d->color = BLACK;
                        c->color = RED;
                        outer_d = c;
                        c = inner_d;
                    }
                    Node* d = outer_d;
                    rotate_up(c);
                    rotate_up(c);
                    c->color = BLACK;
                    b->color = RED;
                    a->color = BLACK;
                    d->color = BLACK;
                    return;
                }
            }
        }
    }
    if(root)
        root->color = BLACK;
}

Node* search_node(long long key) {
    Node* curr = root;
    while(curr) {
        if(key == curr->key)
            return curr;
        curr = (key < curr->key) ? curr->left : curr->right;
    }
    return nullptr;
}

// Insert key into Red-Black tree
bool insert_key(long long key) {
    Node* p = nullptr;
    Node* curr = root;
    while(curr) {
        p = curr;
        if(key == curr->key)
            return false;
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

    update_ancestors(p);
    fix_insert(k);
    return true;
}

// Delete key from Red-Black tree
bool delete_key(long long key) {
    Node* z = search_node(key);
    if(!z)
        return false;

    if(z->left && z->right) {
        Node* pred = z->left;
        while(pred->right)
            pred = pred->right;
        z->key = pred->key;
        z = pred;
    }

    Node* child = z->left ? z->left : z->right;
    Node* p = z->parent;

    if(child != nullptr) {
        child->parent = p;
        if(!p)
            root = child;
        else if(z == p->left)
            p->left = child;
        else
            p->right = child;
        child->color = BLACK;
        update_ancestors(p);
        delete z;
        return true;
    }

    if(z == root) {
        root = nullptr;
        delete z;
        return true;
    }

    if(z->color == RED) {
        if(z == p->left)
            p->left = nullptr;
        else
            p->right = nullptr;
        update_ancestors(p);
        delete z;
        return true;
    }

    if(z == p->left)
        p->left = nullptr;
    else
        p->right = nullptr;

    update_ancestors(p);
    delete z;
    fix_deletion(nullptr, p);
    return true;
}

// Find k-th smallest element using order statistics
long long kth_smallest(int k) {
    Node* curr = root;
    while(curr) {
        int left_sz = get_size(curr->left);
        if(k == left_sz+1)
            return curr->key;
        else if(k <= left_sz)
            curr = curr->left;
        else {
            k -= (left_sz+1);
            curr = curr->right;
        }
    }
    return -1;
}

// Find rank of x (number of keys strictly smaller than x)
int rank_of(long long x) {
    int rank = 0;
    Node* curr = root;
    while(curr) {
        if(x <= curr->key)
            curr = curr->left;
        else {
            rank += 1+get_size(curr->left);
            curr = curr->right;
        }
    }
    return rank;
}

int main() {
    int n;
    cin>>n;

    for(int i = 0; i < n; ++i) {
        long long x;
        cin>>x;
        insert_key(x);
    }

    int q;
    cin>>q;

    while(q--) {
        int type;
        long long x;
        cin>>type>>x;
        switch(type) {
            case 1:
                if(insert_key(x))
                    cout<<x<<" is inserted\n";
                else
                    cout<<x<<" is already present. So no need to insert\n";
                break;
            case 2:
                if(delete_key(x))
                    cout<<x<<" is deleted\n";
                else
                    cout<<x<<" is not present. So it can not be deleted\n";
                break;
            case 3:
                cout<<kth_smallest(static_cast<int>(x))<<"\n";
                break;
            case 4:
                cout<<rank_of(x)<<"\n";
                break;
        }
    }
    return 0;
}
