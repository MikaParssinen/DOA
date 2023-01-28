#include "list.h"
#include <stdlib.h>
#include <assert.h>

/*Det Šr helt tillŒtet att lŠgga till egna hjŠlpfunktioner men de befintliga funktionerna fŒr inte Šndras*/

/*Hjalpfunktion till add
  Allokerar minne for en ny nod
  om allokeringen lyckades initieras data samt pekare (pekare initieras till NULL).
  Den nya noden (eller NULL) returneras.*/
static struct node* createListNode(const Data data)
{
    //Glom inte att testa sa att allokeringen lyckades innan du initierar noden
    struct node *nyNode = (struct node*)calloc(1, sizeof(struct node));
    if(nyNode!=NULL){
        (*nyNode).data = data;
        
        return nyNode;
    }
    printf("ERROR cant allocate memmory:1");
    
    return NULL; //Ersatt med ratt returvarde
}

/*Returnera en tom lista - funktionen ar fardig*/
List createEmptyList(void)
{
    return NULL;
}


/*Ar listan tom?
  Returnerar 1 om den Šr tom, annars 0*/
int isEmpty(const List list)
{
    
    if(list == NULL)
        return 1;
    
    return 0; //ersatt med ratt returvarde
}

/*Lagg till nod forst i listan*/
/*Postcondition: Det nya datat ligger forst i listan (testa med assert)*/
void addFirst(List *list, const Data data)
{
    
    struct node *nyNode = createListNode(data);
    if(nyNode != NULL){
        if(*list == NULL)
        {
            *list = nyNode; //Huvud
            assert(data== (*list)->data);
            return;
        }
    }
    else
    {
        printf("ERROR CANT MAKE NODE 1");
        exit(0);
    }
    struct node *temp = *list;
    //Anropa createListNode for att skapa den nya noden
    //Glom inte att testa att den nya noden faktiskt kunde skapas/tilldelas minne innan du fortsatter
    //Tank pa att listan kan vara tom nar en ny nod laggs till
    *list = nyNode; //Nytt huvud
    
   
    (*nyNode).next = temp; // lŠnkar till den nya fšrsta noden.
    nyNode->next->previous = nyNode;
    assert(data == (*list)->data);
}

/*Lagg till nod sist i listan
  Tips, nar du hittat ratt plats kan du anvanda funktionen addFirst for att lagga till*/
void addLast(List *list, const Data data)
{
    struct node *nyNode= createListNode(data);
    if(nyNode != NULL) {
        if(isEmpty(*list))
        {
            *list =nyNode;
            assert((*list)->data == data);
            return;
        }
        //loop fšr att veta att nŠr sista next noden Šr null.
        struct node *sistaNode;
        for(sistaNode = *list; sistaNode->next != NULL; sistaNode = sistaNode->next)
            
            ;
        sistaNode->next = nyNode; //lŠnkar sista noden med den nya sista noden.
        nyNode->previous = sistaNode;
        assert(getLastElement(*list) == data);
    }
}

/*Ta bort forsta noden i listan
  Precondition: listan ar inte tom (testa med assert)
  Noden ska lankas ur och minnet frigoras, resten av listan ska finnas kvar*/
void removeFirst(List *list)
{
    
    assert(!isEmpty(*list));
    //Glom inte att frigora minnet for den nod som lankas ur listan.
    //Tank pa att listans huvud efter bortlankningen maste peka pa den nod som nu ar forst.
    
    if((*list)->next == NULL)
    {
        free(*list);
        *list = NULL;
        return;
    }
    //Gšr en temp nod pekare
    struct node *temp = *list;
    
    *list = (*list)->next; //Nytt huvud
    
    free(temp); //Tar bort temp noden.
    (*list)->previous = NULL;
    
}

/*Ta bort sista noden i listan
  Precondition: listan ar inte tom (testa med assert)t*/
void removeLast(List *list)
{
    
    assert(!isEmpty(*list));
    
    if((*list)->next == NULL)
    {
        free(*list);
        *list =NULL;
        return;
    }

    struct node *nastSistNode;
    for(nastSistNode = *list; nastSistNode->next->next != NULL; nastSistNode = nastSistNode->next)
        
        ;
    
    free(nastSistNode->next);
    nastSistNode->next = NULL;
           
    //Glom inte att frigora minnet for den nod som lankas ur listan.
    //Tank pa att den nod som nu ar sist inte pekar nagonstans, dess pekare maste nollstallas
}

/*Ta bort data ur listan (forsta forekomsten)
  Returnera 1 om datat finns, annars 0
  Tips, nar du hittar ratt nod kan du anvanda en av de ovanstaende funktionerna for att ta bort noden*/
int removeElement(List *list, const Data data)
{
    if(isEmpty(*list)) // Om listan ar tom
        return 0;

    
    struct node *removeNode;
    for(removeNode = *list; removeNode->data != data && removeNode->next != NULL; removeNode = removeNode->next)
        
        ;
    
    if(removeNode->data != data)
        return 0;
    
    if(removeNode->previous != NULL)
        removeNode->previous->next = removeNode->next;
    
    else
        *list = removeNode->next;
    
    if(removeNode->next != NULL)
        removeNode->next->previous = removeNode->previous;
    
    free(removeNode);
    
    return 1; //Ersatt med ratt returvarde
}

/*Finns data i listan?
  Om datat finns returneras 1, annars 0
  Tank pa att listan kan vara tom*/
int search(const List list, const Data data)
{
    if (list == NULL)
        return 0;
    
    struct node *nodeToFind;
    for (nodeToFind = list; nodeToFind->data != data && nodeToFind->next != NULL; nodeToFind = nodeToFind->next)
        ;
    if (nodeToFind->data == data)
        return 1;
    
    return 0; //Ersatt med ratt returvarde
}

/*Returnera antalet noder i listan*/
int numberOfNodesInList(const List list)
{
    int count = 0;
    
    for (struct node *node = list; node != NULL; node = node->next)
        count++;
    
    return count;

}

/*Ta bort alla noder ur listan
  Glom inte att frigora minnet
  Postcondition: Listan ar tom, *list Šr NULL (testa med assert)*/
void clearList(List *list)
{
   if (isEmpty(*list))
       return;
    
    if ((*list)->next == NULL)
    {
        free(*list);
        *list=NULL;
        assert(isEmpty(*list));
        return;
    }
    
    struct node *temp;
    for (temp = (*list)->next; temp->next != NULL; temp = temp->next)
        free(temp->previous);
    
    //Vi sluta pŒ sista noden men tog inte bort nŠst sista noden
    //tar bort sista och nŠstsista noden
    free(temp->previous);
    free(temp);
    *list=NULL;
    
    
    assert(isEmpty(*list));
}

/*Skriv ut listan
  Vid anropet kan man ange stdout som argument 2 for att skriva ut pŒ skarmen.
  Anvanda fprintf for att skriva ut.
  Den har typen av utskriftfunktion blir mer generell da man kan valja att skriva ut till skarmen eller till fil.*/
void printList(const List list, FILE *textfile)
{
    if(list!=NULL){
        fprintf(textfile, "%s","[");
        struct node *node;
        for (node = list; node->next != NULL; node = node->next){
            fprintf(textfile, "%d, ", node->data);
        }
        fprintf(textfile, "%d]\r\n",node->data);
    }
}

/*Returnera forsta datat i listan
  Precondition: listan ar inte tom (testa med assert)*/
Data getFirstElement(const List list)
{
    assert(!isEmpty(list));
    return list->data;
}

/*Returnera sista datat i listan
  Precondition: listan ar inte tom (testa med assert)*/
Data getLastElement(const List list)
{
    assert(!isEmpty(list));
    
    struct node *SistaNode;
    
    for(SistaNode = list; SistaNode->next != NULL; SistaNode = SistaNode->next)
        ;
   
    return SistaNode->data;
}
