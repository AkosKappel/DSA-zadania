#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    int height;
    struct Node* left;
    struct Node* right;
}Node;

//Funkcia na pridavanie novych prvkov do stromu
Node* addNewNode(int data) {
    Node* newNode = (Node*)calloc(1, sizeof(Node));
    newNode->data = data;
    newNode->height = 0;
    newNode->left = newNode->right = NULL;
    return newNode;
}

//Funkcia na ziskanie vacsej hodnoty spomedzi dvoch cisel
int max(int num1, int num2) {
    return num1 >= num2 ? num1 : num2;
}

//Vypocet faktoru balancovania
int getBalanceFactor(Node* root) {
    if (root->left == NULL && root->right == NULL)
        return 0;
    else if (root->left == NULL && root->right != NULL)
        return -(root->right->height + 1);
    else if (root->left != NULL && root->right == NULL)
        return root->left->height + 1;
    else
        return root->left->height - root->right->height;
}

//Vypocet vysky prvku v strome
int getHeight(Node* root) {
    if (root == NULL)
        return -1;
    else
        return root->height;
}

//Aktualizacia vysky stromu po pridani a rotovani
void updateHeight(Node* root) {
    if (root->left == NULL && root->right == NULL)
        root->height = 0;
    else if (root->left == NULL && root->right != NULL)
        root->height = 1 + root->right->height;
    else if (root->left != NULL && root->right == NULL)
        root->height = 1 + root->left->height;
    else
        root->height = 1 + max(root->left->height, root->right->height);
}

//Rotacia doprava
Node* rotateRight(Node* root) {
    Node* newRoot = root->left;
    root->left = newRoot->right;
    newRoot->right = root;
    updateHeight(root);
    updateHeight(newRoot);
    return newRoot;
}

//Rotacia dolava
Node* rotateLeft(Node* root) {
    Node* newRoot = root->right;
    root->right = newRoot->left;
    newRoot->left = root;
    updateHeight(root);
    updateHeight(newRoot);
    return newRoot;
}

//Dvojita rotacia dolava
Node* rotateRightLeft(Node* root) {
    root->right = rotateRight(root->right);
    root = rotateLeft(root);
    return root;
}

//Dvojita rotacia doprava
Node* rotateLeftRight(Node* root) {
    root->left = rotateLeft(root->left);
    root = rotateRight(root);
    return root;
}

//Pridavanie prvku do stromu
void insert_AVL(Node** root, int data) {
    if (*root == NULL) {
        *root = addNewNode(data);
        return;
    }
    else if ((*root)->data >= data)
        insert_AVL(&((*root)->left), data);
    else
        insert_AVL(&((*root)->right), data);

    updateHeight(*root);
    int balanceFactor = getBalanceFactor(*root);

    if (balanceFactor > 1) {
        if (getHeight((*root)->left->left) >= getHeight((*root)->left->right))
            *root = rotateRight(*root);
        else
            *root = rotateLeftRight(*root);
    }
    if (balanceFactor < -1) {
        if (getHeight((*root)->right->right) >= getHeight((*root)->right->left))
            *root = rotateLeft(*root);
        else
            *root = rotateRightLeft(*root);
    }
}

//Vyhladanie prvku
Node* search_AVL(int data, Node* root) {
    if (root == NULL)
        return NULL;
    else if (root->data == data)
        return root;
    else if (root->data >= data)
        return search_AVL(data, root->left);
    else
        return search_AVL(data, root->right);
}

//Ziskanie najmensej hodnoty
int getMin(Node* current) {
    if (current == NULL)
        return 0;
    while (current->left != NULL) {
        current = current->left;
    }
    return current->data;
}

//Ziskanie najvacsej hodnoty
int getMax(Node* current) {
    if (current == NULL)
        return 0;
    while (current->right != NULL) {
        current = current->right;
    }
    return current->data;
}

//Funkcia na skontrolovanie, ci je strom binarny
int isBinarySearchTree(Node* root, int min, int max) {
    if (root == NULL)
        return 1;
    if (root->data >= min && root->data <= max
        && isBinarySearchTree(root->left, min, root->data)
        && isBinarySearchTree(root->right, root->data, max))
        return 1;
    else
        return 0;
}

//Vypis stromu (sluzi iba na kontrolu a funguje najlepsie ak ma strom malo prvkov)
//Funkcia je prevzata z https://www.geeksforgeeks.org/print-binary-tree-2-dimensions/
void printTree(Node* root, int space) {
    if (root == NULL)
        return;

    space += 6;
    printTree(root->right, space);

    for (int i = 6; i < space; i++)
        printf(" ");
    printf("(%4d|%2d)\n", root->data, root->height);

    printTree(root->left, space);
}

