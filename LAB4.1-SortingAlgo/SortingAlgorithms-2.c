#define _CRT_SECURE_NO_WARNINGS
#include "SortingAlgorithms.h"
#include "Statistics.h"
#include "Array.h"
#include <assert.h>
#include <string.h>

int isImplemented(SortingAlgorithm algorithm)
{
	switch (algorithm)
	{
     case BUBBLE_SORT:
  //   case INSERTION_SORT:
  //      case SELECTION_SORT:
 //       case QUICK_SORT:
//       case MERGE_SORT:
            return 1;
        default:
            return 0;
	}
}

/*Antalet byten i en och samma algoritm kan vara olika beroende på implementationen. Ibland ligger datat redan på rätt plats och då kan man välja att testa det och inte göra ett byte (vilket ger extra jämförelse) eller så kan man ändå göra ett byte (med sig själv). Följer man de algoritmer som vi gått igenom på föreläsningarna exakt så gör man en swap även på ett element som ligger på rätt plats
 
    När du analyserar det data som genereras (result.txt) så behöver du ha detta i åtanke */

/******************************************************************************************/
/* Era algoritmer har: */

//Lokala funktioner.

static void quickSortStart(ElementType* arrayToSort, int start, int end, Statistics* statistics);
static int quickSortPartition(ElementType* arrayToSort, int start, int end, Statistics* statistics);

static void bubbleSort(ElementType* arrayToSort, unsigned int size, Statistics* statistics)
{
    
    int n = size;
    do
    {
        
        //poppa en jamnfor och eventuellt byta plats.
        int nyttN = 0;
        for (int i = 1; lessThan(i, n, statistics); i++)
        {
            
            if(greaterThan(arrayToSort[i-1], arrayToSort[i], statistics)){
                swapElements(arrayToSort + (i - 1), arrayToSort + i, statistics);
                nyttN = i;
            }
        }
        n = nyttN;
        
    }while(lessThanOrEqualTo(1, n, statistics));
}

static void insertionSort(ElementType* arrayToSort, unsigned int size, Statistics* statistics)
{
    int i = 1;
    while(lessThan( i, size, statistics))
    {
        int j = i;
        while(greaterThan(j, 0, statistics) && greaterThan(arrayToSort[j-1],arrayToSort[j], statistics))
        {
            swapElements(arrayToSort + j, arrayToSort + (j-1), statistics);
            j--;
        }
        i++;
    }
    
}

static void selectionSort(ElementType* arrayToSort, unsigned int size, Statistics* statistics)
{
}

static void mergeSort(ElementType* arrayToSort, unsigned int size, Statistics* statistics)
{
}


static void quickSort(ElementType* arrayToSort, unsigned int size, Statistics* statistics)
{
    quickSortStart(arrayToSort, 0, size - 1, statistics);
}

static void quickSortStart(ElementType* arrayToSort, int start, int end, Statistics* statistics)
{
    if(start < end){
        int part = quickSortPartition(arrayToSort, start, end, statistics);
        quickSortStart(arrayToSort, start, part - 1, statistics);
        quickSortStart(arrayToSort, part + 1, end, statistics);
    }
}

static int quickSortPartition(ElementType* arrayToSort, int start, int end, Statistics* statistics)
{

    int pivot = start;

    while(lessThan(start, end, statistics)){

        //Kor tills element på index start är större eller lika med element på index pivot.
        //Tills element pa indexet start ar storre eller == element pa idexet pivot.
        while(lessThanOrEqualTo(arrayToSort[start], arrayToSort[pivot], statistics)){
            start++;
        }
        //Gor samma sak fast spegelvänt (baklänges tills element på index end är mindre än pivot)
        while(greaterThan(arrayToSort[end], arrayToSort[pivot], statistics)){
            end--;
        }

        //Om det ovan gått bra med en osorterad array, byt plasts på element på index start och end
        if(lessThan(start, end, statistics)){
            swapElements(&arrayToSort[start], &arrayToSort[end], statistics);
        }
    }

    //Flytta element på index pivot till rätt plats i arrayen
    swapElements(&arrayToSort[pivot], &arrayToSort[end], statistics);
    return end;
}




/******************************************************************************************/


char* getAlgorithmName(SortingAlgorithm algorithm)
{
	/* Ar inte strangen vi allokerar lokal for funktionen?
	   Nej, inte i detta fall. Strangkonstanter ar ett speciallfall i C */
	switch (algorithm)
	{
        case BUBBLE_SORT:	 return "  Bubble sort ";
        case SELECTION_SORT: return "Selection sort";
        case INSERTION_SORT: return "Insertion sort";
        case MERGE_SORT:	 return "  Merge sort  ";
        case QUICK_SORT:	 return "  Quick sort  ";
        default: assert(0 && "Ogiltig algoritm!"); return "";
	}
}

// Sorterar 'arrayToSort' med 'algorithmToUse'. Statistik for antal byten och jamforelser hamnar i *statistics
static void sortArray(ElementType* arrayToSort, unsigned int size, SortingAlgorithm algorithmToUse, Statistics* statistics)
{
	if(statistics != NULL)
		resetStatistics(statistics);

	switch (algorithmToUse)
	{
	case BUBBLE_SORT:	 bubbleSort(arrayToSort, size, statistics); break;
	case SELECTION_SORT: selectionSort(arrayToSort, size, statistics); break;
	case INSERTION_SORT: insertionSort(arrayToSort, size, statistics); break;
	case MERGE_SORT:	 mergeSort(arrayToSort, size, statistics); break;
	case QUICK_SORT:	 quickSort(arrayToSort, size, statistics); break;
	default:
		assert(0 && "Ogiltig algoritm!");
	}
}

// Forbereder arrayer for sortering genom att allokera minne for dem, samt intialisera dem
static void prepareArrays(SortingArray sortingArray[], SortingAlgorithm algorithm, const ElementType* arrays[], const unsigned int sizes[])
{
	assert(isImplemented(algorithm));

	int i;
	int totalArraySize;

	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		totalArraySize = sizeof(ElementType)*sizes[i];
		sortingArray[i].arrayToSort = malloc(totalArraySize);
		memcpy(sortingArray[i].arrayToSort, arrays[i], totalArraySize);

		sortingArray[i].algorithm = algorithm;
		sortingArray[i].arraySize = sizes[i];
		resetStatistics(&sortingArray[i].statistics);
	}
}

// Sorterar arrayerna
static void sortArrays(SortingArray toBeSorted[])
{
	int i;
	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		SortingArray* current = &toBeSorted[i];
		sortArray(current->arrayToSort, current->arraySize, current->algorithm, &current->statistics);
	
		if (!isSorted(current->arrayToSort, current->arraySize))
		{
			printf("Fel! Algoritmen %s har inte sorterat korrekt!\n", getAlgorithmName(current->algorithm));
			printf("Resultatet ‰r: \n");
			printArray(current->arrayToSort, current->arraySize, stdout);
			assert(0); // Aktiveras alltid
		}
	}
}

void printResult(SortingArray sortedArrays[], FILE* file)
{
	assert(file != NULL);

	int i;
	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		fprintf(file, "%4d element: ", sortedArrays[i].arraySize);
		printStatistics(&sortedArrays[i].statistics, file);
		fprintf(file, "\n");
	}
	fprintf(file, "\n");
}

void sortAndPrint(SortingArray sortingArray[], SortingAlgorithm algorithm, const ElementType* arrays[], unsigned int sizes[], FILE* file)
{
	assert(file != NULL);

	prepareArrays(sortingArray, algorithm, arrays, sizes);
	sortArrays(sortingArray);
	printResult(sortingArray, file);
}

void freeArray(SortingArray sortingArray[])
{
	int i;
	for (i = 0; i < NUMBER_OF_SIZES; i++)
	{
		if (sortingArray[i].arrayToSort != NULL)
			free(sortingArray[i].arrayToSort);
		sortingArray[i].arrayToSort = NULL;
		sortingArray[i].arraySize = 0;
		resetStatistics(&sortingArray[i].statistics);
	}
}
