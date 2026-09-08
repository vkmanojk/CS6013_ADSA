#include<iostream>
#include<queue>

using namespace std;

bool flag;

struct node {
    int data;
    struct node* left;
    struct node* right;
    struct node* parent;
};

void printInOrder(node* root) {
    if(root == NULL)
        return;
    node* temp = root;
    printInOrder(temp->left);
    cout<<temp->data<<" ";
    printInOrder(temp->right);
}

void printPreOrder(node* root) {
    if(root == NULL)
        return;
    node* temp = root;
    cout<<temp->data<<" ";
    printPreOrder(temp->left);
    printPreOrder(temp->right);
}

void printLevelOrder(node* root) {
    if(root == NULL)
        return;
    queue<node*> q;
    q.push(root);
    node* temp;
    while(!q.empty()) {
        int levelSize = q.size();
        for(int i = 0 ; i < levelSize ; ++i) {
            temp = q.front();
            q.pop();
            cout<<temp->data<<" ";
            if(temp->left != NULL)
                q.push(temp->left);
            if(temp->right != NULL)
                q.push(temp->right);
        }
        cout<<endl;
    }
}

void displayTraversals(node* root) {
    cout<<"INORDER:\t"; printInOrder(root);
    cout<<"\nPREORDER:\t"; printPreOrder(root);
    cout<<"\nLEVELORDER:\n";
    printLevelOrder(root);
    cout<<"END\n";
}

node* insert(node* root, int key) {
    node *newNode = new node;
    newNode->data = key;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->parent = NULL;

    if(root == NULL) {
        root = newNode;
        return root;
    }
    node* temp = root;
    node* parent = NULL;

    while(temp != NULL) {
        parent = temp;
        if(key < temp -> data)
            temp = temp->left;
        else if (key > temp -> data)
            temp = temp->right;
        else {
            flag = true;
            return root;
        }
    }

    newNode->parent = parent;
    if(key < parent->data)
        parent->left = newNode;
    else
        parent->right = newNode;

    return root;
}

node* search(node* root, int key) {
    node* temp = root;
    while(temp!=NULL) {
        if(temp->data == key) {
            cout<<"Key found\n";
            return temp;
        }
        else if(key < temp->data) 
            temp = temp->left;
        else
            temp = temp->right;
    }
    cout<<"Key not found\n";
    return NULL;
}

node* deleteNode(node* root, int key) {
    node* temp = root;
    while (temp != NULL && temp->data != key) {
        if (key < temp->data)
            temp = temp->left;
        else
            temp = temp->right;
    }
    if (temp == NULL)
        return root;
    flag = true;
    if (temp->left != NULL && temp->right != NULL) {
        node* successor = temp->right;
        while (successor->left != NULL)
            successor = successor->left;

        temp->data = successor->data;
        temp = successor;
    }

    node* child = (temp->left != NULL) ? temp->left : temp->right;

    if (temp->parent == NULL) 
        root = child;
    else if (temp == temp->parent->left) 
        temp->parent->left = child;
    else 
        temp->parent->right = child;
    
    if (child != NULL)
        child->parent = temp->parent;
    delete temp;

    return root;
}



int main() {
    node *root = NULL, *keyNode;
    int N, S, I, D, key;
    cin>>N;
    for(int i = 0; i < N; ++i) {
        cin>>key;
        root = insert(root, key);
    }
    cout<<"INITIAL TREE\n";
    displayTraversals(root);

    cin>>S;
    for(int i = 0; i < S; ++i) {
        cin>>key;
        cout<<"SEARCH "<<key<<": ";
        keyNode = search(root, key);
    }

    cin>>I;
    for(int i = 0; i < I; ++i) {
        cin>>key;
        cout<<"INSERT "<<key<<endl;
        flag = false;
        root = insert(root, key);
        if(flag) cout<<"Duplicate Key"<<endl;
        else displayTraversals(root);
    }

    cin>>D;
    for(int i = 0; i < D; ++i) {
        cin>>key;
        cout<<"DELETE "<<key<<endl;
        flag = false;
        root = deleteNode(root, key);
        if(!flag)
            cout<<"Key not found" << endl;
        else
            displayTraversals(root);
    }
    return 0;
}