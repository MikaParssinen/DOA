#define _CRT_SECURE_NO_WARNINGS // Behovs for vissa funktioner i visual studio
#include "HashTable.h"
#include "Bucket.h"
#include<assert.h>
#include<stdlib.h>
#include<stdio.h>

// Anvands for att markera en ledig plats i Hashtabellen


/* Denna funktion tar en nyckel och returnerar ett hash-index
dvs ett index till arrayen som Šr Hashtabellen */
static int hash(Key key, int tablesize)
{
    
    return key%tablesize;
}

/*Leta framŒt enligt principen šppen adressering
 Antalet krockar returneras via pekaren col i parameterlistan*/
static int linearProbe(const HashTable* htable, Key key, unsigned int *col)
{
    int keyHash = hash(key, getSize(htable));
    *col = 0;
    while(1){
        if(htable->table[keyHash].key == key || htable->table[keyHash].key == UNUSED){
            return keyHash;
        }
        keyHash = (keyHash + 1) % getSize(htable);
        (*col)++;
        if(*col > getSize(htable)){
            printf("Hashtable is filled!\n");
            return -1;
        }
    }
    printf("Error in linear probe\n");
    return -1;
}

/*Allokera minne fšr hashtabellen*/
HashTable createHashTable(unsigned int size)
{
    HashTable htable;
    htable.size = size;
    
    struct Bucket *tmp = (struct Bucket*)malloc(sizeof(struct Bucket) * size);
    if(tmp != NULL){
        htable.table = tmp;
        for(int i=0; i < size; i++){
            htable.table[i].key = UNUSED;
        }
        
        return htable;
    }
    else{
        printf("Allocation faild! (code 1)");
        exit(0);
    }
    

}

/* Satter in paret {key,data} i Hashtabellen, om en nyckel redan finns ska vardet uppdateras */
/* Returnerar antalet krockar (som rŠknas i linearProbe() )*/
unsigned int insertElement(HashTable* htable, const Key key, const Value value)
{
	// Postcondition: det finns ett element for key i tabellen (anvand lookup() for att verifiera)
    struct Bucket bucket;
    bucket.key = key;
    bucket.value = value;
    unsigned int col;
    int index = linearProbe(htable, key, &col);
    
    if(index != -1){
        htable->table[index] = bucket;
    }
    
    assert(lookup(htable, key) != NULL);
    return col;
                            
}

/* Tar bort datat med nyckel "key" */
void deleteElement(HashTable* htable, const Key key)
{
    unsigned int col;
    int index = linearProbe(htable, key, &col);
    if(index != -1){
        htable->table[index].key = UNUSED;
        while(1){
            index = (index + 1) % getSize(htable);
            if(htable->table[index].key){
                Key keyTemp = htable->table[index].key;
                htable->table[index].key = UNUSED;
                insertElement(htable, keyTemp, htable->table[index].value);
            }
            else{
                return;
            }
        }
    }
        
    assert(NULL == lookup(htable, key));
}

/* Returnerar en pekare till vardet som key ar associerat med eller NULL om ingen sadan nyckel finns */
const Value* lookup(const HashTable* htable, const Key key)
{
    unsigned int col;
    int index = linearProbe(htable, key, &col);
    if(index != -1){
        if(htable->table[index].key != UNUSED)
            return &(htable->table[index].value);
    }
    return NULL;

}


/* Tommer Hashtabellen */
void freeHashTable(HashTable* htable)
{
    free(htable->table);
    htable->table = NULL;
    htable->size = 0;
    
}

/* Ger storleken av Hashtabellen */
unsigned int getSize(const HashTable* htable)
{
    return htable->size;
}

/* Denna for att ni enkelt ska kunna visualisera en Hashtabell */
void printHashTable(const HashTable* htable)
{
    for(int i = 0; i < getSize(htable); i++){
        printPerson(&(htable->table[i].value), i);
    }
}
