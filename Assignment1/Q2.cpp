#include<iostream>

using namespace std;

struct node {
    int data;
    int height;
    struct node* left;
    struct node* right;
    struct node* parent;
};