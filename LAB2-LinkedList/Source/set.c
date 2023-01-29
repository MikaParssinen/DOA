#include "Set.h"
#include <assert.h>

/*Funktionen ar fardig*/
Set initializeSet(void)
{
	return createEmptyList();
}

/* Tips: tank pa att inte lagga till elementet om det redan finns i *set
   Postcondition: 'element' finns i set (tips: anvand isInSet() for att verifiera) */
void addToSet(Set* set, const Data element)
{
    if(isInSet(*set, element))
        return;
    
    addFirst(set, element);
    assert(isInSet(*set, element));
    
}

/* Postcondition: 'element' finns INTE i set (tips: anvand isInSet() for att verifiera) */
void removeFromSet(Set* set, const Data element)
{
    removeElement(set, element);
    assert(!isInSet(*set, element));
}


int isInSet(const Set set, const Data element)
{
    return search(set, element);
}

/* Anvands for testning och felsokning */
void printSet(const Set set, FILE *textfile)
{
    printList(set, textfile);
}

