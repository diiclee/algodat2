#include <stdio.h>
#include <stdlib.h>

// Definition eines Knotens für den binären Baum
struct node
{
    int key;            // Schlüsselwert des Knotens
    struct node *left;  // Zeiger auf den linken Kindknoten
    struct node *right; // Zeiger auf den rechten Kindknoten
};

// Funktion zum Erstellen eines neuen Knotens mit einem bestimmten Schlüsselwert
struct node *newNode(int item)
{
    struct node *temp = (struct node *)malloc(sizeof(struct node)); // Speicher für den neuen Knoten zuweisen
    temp->key = item;               // Schlüsselwert setzen
    temp->left = temp->right = NULL; // Kindzeiger auf NULL initialisieren
    return temp;
}

// Inorder-Traversierung: Linkes Kind, Wurzel, Rechtes Kind
void inorder(struct node *root)
{
    if (root != NULL)
    {
        inorder(root->left);        // Linken Teilbaum traversieren
        printf("%d \n", root->key); // Wurzel ausgeben
        inorder(root->right);       // Rechten Teilbaum traversieren
    }
}

// Funktion zum Einfügen eines neuen Knotens in den Baum
struct node *insert(struct node *node, int key)
{
    // Falls der Baum leer ist, wird ein neuer Knoten erstellt und als Wurzel zurückgegeben
    if (node == NULL)
        return newNode(key);

    // Rekursiv nach dem richtigen Ort für den neuen Knoten suchen und einfügen
    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);

    return node;
}

// Funktion, um den Knoten mit dem kleinsten Schlüsselwert im gegebenen Baum zu finden
struct node *minValueNode(struct node *node)
{
    struct node *current = node;

    // Gehe nach links, bis das Ende des linken Teilbaums erreicht ist
    while (current && current->left != NULL)
        current = current->left;

    return current; // Rückgabe des Knotens mit dem kleinsten Schlüsselwert
}

// Funktion zum Löschen eines Knotens mit einem bestimmten Schlüsselwert im Baum
struct node *deleteNode(struct node *root, int key)
{
    if (root == NULL)
        return root;

    // Rekursiv den Knoten finden und löschen
    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else
    {
        // Knoten mit nur einem Kind oder ohne Kindern
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

        // Knoten mit beiden Kindern: den nächsten Knoten im Inorder-Nachfolger suchen und den aktuellen Knotenwert ersetzen
        struct node *temp = minValueNode(root->right);
        root->key = temp->key;
        root->right = deleteNode(root->right, temp->key);
    }
    return root;
}

// Funktion zur Berechnung der Höhe eines Baumes
int height(struct node *root)
{
    if (root == NULL)
        return -1; // Höhe eines leeren Baumes ist -1
    else
    {
        // Höhe des Baumes ist die maximale Höhe des linken oder rechten Teilbaums plus 1 (für die aktuelle Ebene)
        int leftHeight = height(root->left);
        int rightHeight = height(root->right);
        return (leftHeight > rightHeight) ? leftHeight + 1 : rightHeight + 1;
    }
}

// Funktion zur Ausgabe des Balancefaktors für jeden Knoten im Baum
void balanceFactor(struct node *root)
{
    if (root != NULL)
    {
        balanceFactor(root->right); // Rechten Teilbaum traversieren
        balanceFactor(root->left);  // Linken Teilbaum traversieren
        // Balancefaktor berechnen: Höhe des rechten Teilbaums - Höhe des linken Teilbaums
        int bf = height(root->right) - height(root->left);
        if (bf > 1 || bf < -1)
            printf("bal(%d) = %d (AVL violation!)\n", root->key, bf); // AVL-Verletzung melden
        else
            printf("bal(%d) = %d\n", root->key, bf); // Balancefaktor ausgeben
    }
}

// Funktion zur Überprüfung, ob der gegebene Baum ein AVL-Baum ist
int isAVL(struct node *root)
{
    if (root == NULL)
        return 1; // Leerer Baum ist AVL
    // Überprüfen, ob der Unterschied der Höhen der linken und rechten Teilbäume größer als 1 ist
    int lh = height(root->left);
    int rh = height(root->right);
    if (abs(lh - rh) > 1)
        return 0; // Nicht AVL
    // Rekursiv die AVL-Eigenschaft für alle Teilbäume überprüfen
    return isAVL(root->left) && isAVL(root->right);
}

// Funktion zur Berechnung von Statistiken wie Min, Max, Summe und Anzahl der Knoten im Baum
void calculateStatistics(struct node *root, int *min, int *max, int *sum, int *count)
{
    if (root != NULL)
    {
        // Min, Max, Summe und Anzahl aktualisieren
        *min = (root->key < *min) ? root->key : *min;
        *max = (root->key > *max) ? root->key : *max;
        *sum += root->key;
        *count += 1;
        // Rekursiv die Statistiken für die Teilbäume berechnen
        calculateStatistics(root->left, min, max, sum, count);
        calculateStatistics(root->right, min, max, sum, count);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "r");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }

    struct node *root = NULL;
    int key;
    // Einlesen der Schlüsselwerte aus einer Datei und Einfügen in den Baum
    while (fscanf(fp, "%d", &key) == 1)
    {
        root = insert(root, key);
    }
    fclose(fp);

    // Balancefaktoren für jeden Knoten im Baum ausgeben
    balanceFactor(root);
    // Überprüfen, ob der Baum ein AVL-Baum ist
    if (isAVL(root))
        printf("AVL: yes\n");
    else
        printf("AVL: no\n");

    // Statistiken wie Min, Max, Durchschnitt berechnen und ausgeben
    int min = root->key, max = root->key, sum = 0, count = 0;
    calculateStatistics(root, &min, &max, &sum, &count);
    printf("min: %d, max: %d, avg: %.1f\n", min, max, (float)sum / count);

    return 0;
}
