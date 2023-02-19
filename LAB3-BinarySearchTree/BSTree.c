#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "BSTree.h"

int bigger(int a, int b);
void InOrderArr(BSTree tree, int *arr, int *count);
int CheckBalance( const BSTree tree);
int MinstITree(const BSTree tree);



/*
 Mitt satt att ga till vaga, skapa array i inorder, sedan borja i slutet av arrayn for att hitta de storsta talen. Addera de tillsammans och printa ut det.
 Varfor? Jag tyckte det var lattast sattet tankte pa funktionerna jag redan hade och att inorder ar minst till storst sa det funka bast.

 */

//Preconditions: none.
//Postconditions: Tree will not change in shape.
int sumBiggest(BSTree tree, int k){
    int n = numberOfNodes(tree), sum = 0;
    if(isEmpty(tree)){
        printf("Tree is empty exiting\n");
        return 0;
    }
    else if(n < k || k<=0){
        printf("Invalid value, enter a value smaller or equal to the tree size!\n");
        return 1;
    }

    int *arr = (int*)calloc(n, sizeof(int));
    if(arr == NULL){
        printf("Can't make memory\n");
    }
    else{
        int i = 0;
        InOrderArr(tree, arr, &i);
    }
    
    printf("You have choosen %d amount of the largest numbers to summarize\n", k);
    while(k != 0){
        n = n - 1;
        sum = sum + arr[n];
        printf("%d\n", arr[n]);
        k--;
    }
    printf("= \n");
    printf("%d \n", sum);
    return sum;
    
}
//TESTFUNKTION.
void test_sumBiggest(BSTree tree){
    printf("Starting test of sumBiggest:\n");
   //Testar pa tom trad
    assert(sumBiggest(tree, 2) == 0);
  
    //Insattning av element
    int arr[5] = {5,10,20,1,2}, i;
    for (i = 0; i < 5; i++)
    {
        insertSorted(&tree, arr[i]);
    }
    
    //Kollar om den kan rakna ratt.
    assert(sumBiggest(tree, 3) == 35);
    //Testar om det gar att rakna mer and vad det finns noder.
    assert(sumBiggest(tree, 10) == 1);
    
        
    printf("Completed! It passed the test.\n");
}



/* Skapar en tradnod med det givna datat genom att allokera minne for noden. Glom inte att initiera pekarna*/
static struct treeNode* createNode(int data)
{
    struct treeNode* resultat = (struct treeNode*)calloc(1,sizeof(struct treeNode)); //Skapar Tradet
    if(resultat != NULL) {
        resultat->left = NULL;
        resultat->right = NULL;
        resultat->data = data;
        return resultat; // Returnerar noden
    }
    else{
        printf("Memory allocation failed 1");
        return 0;
    }
    
  
}

/* Returnerar en dynamiskt allokerad array som innehaller tradets data sorterat */
static int* writeSortedToArray(const BSTree tree)
{
    //Antal noder som ska allokeras loser funktionen numberOfNodes
    int *arr = (int*)calloc(numberOfNodes(tree), sizeof(int));
    
    if(arr != NULL){
        int i = 0;
        InOrderArr(tree, arr, &i);
        return arr;
    }
    
    else{
        printf("Memory allocation failed 2");
        return 0;
    }
}

/* Bygger upp ett sorterat, balanserat trad fran en sorterad array */
static void buildTreeSortedFromArray(BSTree* tree, const int arr[], int size)
{
    /* Bygg rekursivt fran mitten.
       Mittenelementet i en delarray skapar rot i deltradet
       Vanster delarray bygger vanster deltrad
       Hoger delarray bygger hoger deltrad*/
  
    //Halverar for att fa halftet av size. floor ar en fuktion som avrundar ner 
    int halv = (int)floor(size/2);
    
    *tree = createNode(arr[halv]);
    if (*tree != NULL)
    {
        if (size > 2){
            buildTreeSortedFromArray(&(*tree)->left, arr, halv);
            buildTreeSortedFromArray(&(*tree)->right, arr + halv + 1, size - halv -1);
        }
        else if (size == 2)
            (*tree)->left = createNode(arr[0]);
        
    }
}


/* Implementation av tradet, funktionerna i interfacet */

/* Skapar ett tomt trad - denna funktion ar fardig */
BSTree emptyTree(void)
{
	return NULL;
}

/* Returnerar 1 ifall tradet ar tomt, 0 annars */
int isEmpty(const BSTree tree)
{
    if(tree == NULL)
        return 1;
    else
        return 0;

}

/* Satter in 'data' sorterat i *tree
 Post-condition: data finns i tradet*/
void insertSorted(BSTree* tree, int data)
{
   
    //Testar om tradet ar tomt om de tomt gor vi en root.
    if(isEmpty(*tree) == 1)
    {
        *tree = createNode(data);
        return;
    }
    
    //Om right data ar storre, testa till hoger.
    if(data > (*tree)->data)
    {
        //Om right ar tom allokera node till right.
        if ((*tree)->right == NULL){
            
            struct treeNode *nyNode = createNode(data);
            if(nyNode != NULL)
            {
                (*tree)->right = nyNode;
                assert(find((*tree)->right, (*tree)->right->data) == 1);
            }
        }
        
        //annars recursivt ga till hoger.
        else
        {
            return insertSorted(&((*tree)->right),data);
        }
    }
    //Samma som forgonde men at vanster
    if (data < (*tree)->data)
    {
        if((*tree)->left == NULL)
        {
            struct treeNode *nyNode = createNode(data);
            
            if(nyNode != NULL){
                (*tree)->left = nyNode;
                assert(find((*tree)->left, (*tree)->left->data) == 1);
            }
        }
        else
            return insertSorted(&((*tree)->left), data);
    }
    
    assert(find(*tree, data));

}

void printPreorder(const BSTree tree, FILE *textfile)
{
    //Preorder = besok, vanster, hoger.
    fprintf(textfile, "%d ", tree->data);
    if(tree->left != NULL)
        printPreorder(tree->left, textfile);
    
    if(tree->right != NULL)
        printPreorder(tree->right, textfile);
}

void printInorder(const BSTree tree, FILE *textfile)
{
    //Inorder = Vanster, besok, hoger.
    if(tree->left != NULL)
        printInorder(tree->left, textfile);
    
    fprintf(textfile, "%d", tree->data);

    if(tree->right != NULL)
        printInorder(tree->right,textfile);
}

void printPostorder(const BSTree tree, FILE *textfile)
{
    //Postorder = Vaster, hoger, besok.
    if(tree->left != NULL)
        printPostorder(tree->left, textfile);
    if(tree->right != NULL)
        printPostorder(tree->right, textfile);
    
    fprintf(textfile, "%d", tree->data);
    
    
}

/* Returnerar 1 om 'data' finns i tree, 0 annars */
int find(const BSTree tree, int data)
{
     if(isEmpty(tree) == 1){ //Kolla om listan ar tom.
        return 0;
    }
   
    if(tree->data == data) //Hitta nod med samma data.
        return 1;
  
    if(data < tree->data) //Om data ar mindra an datat i nuvarande nod, sa skickar vi naste nod till vanster.
        return find(tree->left, data);
    
    else
        return find(tree->right, data);
	
}

/* Tar bort 'data' fran tradet om det finns */
void removeElement(BSTree* tree, int data)
{
	
    //Testar om trŠdet Šr tomt
    if(isEmpty(*tree) == 1)
        return;
    
    //Om data mindre an aktuell nod, ga vanster.
    if ((*tree)->data > data) {
        removeElement(&(*tree)->left, data);
    }
    //Samma fast vanster
    else if((*tree)->data < data) {
        removeElement(&(*tree)->right, data);
    }
    
    else {
        //2 Barn. viktigt ordning
        if ((*tree)->left != NULL && (*tree)->right != NULL){
            //Hitta minsta
            int nodeToRemove = MinstITree((*tree)->right);
            //Ta bort nod
            removeElement(&(*tree)->right, nodeToRemove);
            //Satt node datan till removed node data.
            (*tree)->data = nodeToRemove;
        }
        //Ett barn till vanster
        else if ((*tree)->left != NULL)
        {
            //Ny pekare for att ta bort noden.
            struct treeNode *nodeToRemove = *tree;
            //Nuvarande nod till left.
            *tree = nodeToRemove->left;
            //Ta bort nod.
            free(nodeToRemove);
            nodeToRemove = NULL;
        }
        //Samma som forra men right.
        else if ((*tree)->right != NULL)
        {
            struct treeNode *nodeToRemove = *tree;
            
            (*tree) = nodeToRemove->right;
            
            free(nodeToRemove);
            nodeToRemove = NULL;
        }
        
        //Om det ar ett lov, ta bort direkt.
        else
        {
            free(*tree);
            *tree = NULL;
        }
    }
   
    assert(!find(*tree,data));
    
}

/* Returnerar hur manga noder som totalt finns i tradet */
int numberOfNodes(const BSTree tree)
{
    if(isEmpty(tree))
        return 0;
    
    return 1 + numberOfNodes(tree->left) + numberOfNodes(tree->right);
    //Ersatt med korrekt returvarde
}

/* Returnerar hur djupt tradet ar */
int depth(const BSTree tree)
{
    if(isEmpty(tree) == 1) //Om listan Šr tom sŒ return 0
        return 0;
    
    return bigger(depth(tree->left),depth(tree->right)) + 1;
}

/* Returnerar minimidjupet for tradet
   Se math.h for anvandbara funktioner*/
int minDepth(const BSTree tree)
{
    if(tree == NULL)
        return 0;
    else
        return (int)ceil(log2(numberOfNodes(tree)+1)); // Hmin = [log2N] + 1. Ceil avrundar upp till narmsta heltal.
}

/* Balansera tradet sa att depth(tree) == minDepth(tree) */
void balanceTree(BSTree* tree)
{
	
    if (isEmpty(*tree) == 1)
        return;
    //Om tradet ar balancerat sa behover vi inte gora om de
    if (CheckBalance(*tree))
        return;
    
    //Rakna antal noder
    int stl = numberOfNodes(*tree);
    //Skriv till array
    int *arr = writeSortedToArray(*tree);
    //Freea tradet
    freeTree(tree);
    //Bygg fran sorted.
    buildTreeSortedFromArray(tree, arr, stl);
    //freea arrayn
    free(arr);
    arr = NULL;
    
    assert(stl = numberOfNodes(*tree));
    assert(CheckBalance(*tree));
    
}
    

/* Tom tradet och frigor minnet for de olika noderna */
void freeTree(BSTree* tree)
{
    if(tree == NULL){
        return;
    }
    
    if((*tree)->left != NULL){
        freeTree(&(*tree)->left);
    }
    
    if((*tree)->right != NULL){
        freeTree(&(*tree)->right);
    }
    
    free(*tree);
    *tree = NULL;
    assert(isEmpty(*tree));
	// Post-condition: tradet ar tomt
}

int bigger(int a, int b){
    if(a>b)
        return a;
    else
        return b;
}

void InOrderArr(BSTree tree, int *arr, int *count)
{
    //Minst till storts maste man anvanda inorder(Vanster, besok, hoger)
    
    //Vanster rekursivt
    if(tree->left != NULL){
        InOrderArr(tree->left, arr, count);
    }
    //Add node data arr
    arr[(*count)++] = tree->data;

    //Hoger rekursivt
    if(tree->right != NULL){
        
        InOrderArr(tree->right, arr, count);
    }
    

}

int CheckBalance( const BSTree tree)
{
    return (depth(tree) == minDepth(tree));
}

int MinstITree(const BSTree tree)
{
    if (tree->left == NULL)
        return tree->data;
    
    return MinstITree(tree->left);
    
}

