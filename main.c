#include <stdio.h>
#include <stdlib.h>

struct node
{
    int key;
    struct node *left;
    struct node *right;
};

struct node *newNode(int item)
{
    struct node *temp = (struct node *)malloc(sizeof(struct node));
    temp->key = item;
    temp->left = temp->right = NULL;
    return temp;
}

void inorder(struct node *root)
{
    if (root != NULL)
    {
        inorder(root->left);
        printf("%d \n", root->key);
        inorder(root->right);
    }
}

struct node *insert(struct node *node, int key)
{
    if (node == NULL)
        return newNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);

    return node;
}

struct node *minValueNode(struct node *node)
{
    struct node *current = node;

    while (current && current->left != NULL)
        current = current->left;

    return current;
}

struct node *deleteNode(struct node *root, int key)
{
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);

    else
    {
        if (root->left == NULL)
        {
            struct node *temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL)
        {
            struct node *temp = root->left;
            free(root);
            return temp;
        }

        struct node *temp = minValueNode(root->right);

        root->key = temp->key;

        root->right = deleteNode(root->right, temp->key);
    }
    return root;
}

int height(struct node *root) {
    if (root == NULL)
        return -1;
    else {
        int leftHeight = height(root->left);
        int rightHeight = height(root->right);
        return (leftHeight > rightHeight) ? leftHeight + 1 : rightHeight + 1;
    }
}

void balanceFactor(struct node *root) {
    if (root != NULL) {
        int bf = height(root->right) - height(root->left);
        printf("bal(%d) = %d\n", root->key, bf);
        if (bf > 1 || bf < -1)
            printf("bal(%d) = %d (AVL violation!)\n", root->key, bf);
        balanceFactor(root->left);
        balanceFactor(root->right);
    }
}

int isAVL(struct node *root) {
    if (root == NULL)
        return 1;

    int lh = height(root->left);
    int rh = height(root->right);
    if (abs(lh - rh) > 1)
        return 0;

    return isAVL(root->left) && isAVL(root->right);
}

void calculateStatistics(struct node *root, int *min, int *max, int *sum, int *count) {
    if (root != NULL) {
        *min = (root->key < *min) ? root->key : *min;
        *max = (root->key > *max) ? root->key : *max;
        *sum += root->key;
        *count += 1;
        calculateStatistics(root->left, min, max, sum, count);
        calculateStatistics(root->right, min, max, sum, count);
    }
}

int main(int argc, char *argv[]) {
    // Öffnen Sie die Eingabedatei "testData_one.txt" im Lesemodus
    FILE *fp = fopen("testData_one.txt", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return 1;
    }

    struct node *root = NULL;
    int key;
    // Lesen Sie die Schlüssel aus der Datei und fügen Sie sie in den Baum ein
    while (fscanf(fp, "%d", &key) == 1) {
        root = insert(root, key);
    }
    fclose(fp);

    // Führen Sie die Funktionen zur Überprüfung des AVL-Baums und zur Berechnung der Statistiken aus
    balanceFactor(root);
    if (isAVL(root))
        printf("AVL: yes\n");
    else
        printf("AVL: no\n");

    int min = root->key, max = root->key, sum = 0, count = 0;
    calculateStatistics(root, &min, &max, &sum, &count);
    printf("min: %d, max: %d, avg: %.1f\n", min, max, (float)sum / count);

    return 0;
}
