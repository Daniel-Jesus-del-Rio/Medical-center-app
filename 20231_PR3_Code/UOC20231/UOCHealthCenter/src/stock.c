#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stock.h"

// Initialize a stock list
void stockList_init(tVaccineStockData* list) {
    // PR2 Ex 1a
    assert(list != NULL);

    list->count = 0;
    list->first = NULL;
    /////////////////
}

// Modify the doses of a certain vaccine
void stockList_update(tVaccineStockData* list, tDate date, tVaccine* vaccine, int doses) {
    // PR2 Ex 1b
    tVaccineDailyStock* pNode;
    tVaccineDailyStock* last;

    assert(list != NULL);

    // If the list is empty, just add a new element
    if (list->count == 0) {
        // Create the new element
        list->first = (tVaccineDailyStock*)malloc(sizeof(tVaccineDailyStock));
        assert(list->first != NULL);
        last = list->first;
        list->count = 1;
        // Initialize the new element
        dailyStock_init(list->first, date);
        assert(list->first->next == NULL);
        // Update the number of doses
        dailyStock_update(list->first, vaccine, doses);
    }
    else {
        last = getLastVaccineStockData(list);
        // Search for the starting date
        if (date_cmp(list->first->day, date) > 0) {
            // Date is before other dates. Add missing elements at left
            stockList_expandLeft(list, date);
            // Set start position for update the first element
            pNode = list->first;
        }
        else if (date_cmp(last->day, date) > 0) {
            // Date is in the range of dates we already have. We will start updating on provided date.
            pNode = stockList_find(list, date);
            assert(pNode != NULL);
        }
        else {
            // Date is after the end date. We will extend current stock up to the date and then update recursivelly
            stockList_expandRight(list, date);
            // Set start position for update the last element
            last = getLastVaccineStockData(list);
            pNode = last;
        }


        // Update all days from start position        
        while (pNode != NULL) {
            dailyStock_update(pNode, vaccine, doses);
            pNode = pNode->next;
        }
    }

    // Once updated, remove empty elements
    stockList_purge(list);

    /////////////////
}

// Get the number of doses for a certain vaccine and date
int stockList_getUnitDoses(tVaccineStockData* list, tDate date, tVaccine* vaccine) {
    // PR2 Ex 1c
    tVaccineDailyStock* pNode = NULL;
    tVaccineDailyStock* last;
    int numDoses = 0;
    assert(list != NULL);

    pNode = stockList_find(list, date);
    last = getLastVaccineStockData(list);
    if (pNode != NULL) {
        numDoses = stockNode_getDoses(pNode->first, vaccine);
    }
    else if (date_cmp(last->day, date) < 0) {

        numDoses = stockNode_getDoses(last->first, vaccine);
    }

    return numDoses * vaccine->unitDoses;
    /////////////////
    // return -10000;
}

// Release a list of stocks
void stockList_free(tVaccineStockData* list) {

    tVaccineDailyStock* pNode;

    assert(list != NULL);

    pNode = list->first;

    while (pNode != NULL) {
        list->first = pNode->next;
        dailyStock_free(pNode);
        free(pNode);
        list->count--;

        pNode = list->first;
    }
    list->first = NULL;
    /////////////////
}

/////////////////////////////////////////
///// AUX Methods: Top-down design //////
/////////////////////////////////////////

// Initialize a stock element
void stock_init(tVaccineStock* stock, tVaccine* vaccine, int doses) {
    assert(stock != NULL);
    stock->doses = doses;
    stock->vaccine = vaccine;
}

// Get the number of doses for a given vaccine
int stockNode_getDoses(tVaccineStockNode* stock, tVaccine* vaccine) {
    int numDoses = 0;

    // Recursive trival cases: Empty list or current vaccine name is after target vaccine name (data is sorted by vaccine name)
    if (stock == NULL || strcmp(stock->elem.vaccine->name, vaccine->name) > 0) {
        numDoses = 0;
    }
    else if (strcmp(stock->elem.vaccine->name, vaccine->name) == 0) {
        // Recursion trivial case: We are on the vaccine node
        numDoses = stock->elem.doses;
    }
    else {
        // Recursive call
        numDoses = stockNode_getDoses(stock->next, vaccine);
    }

    return numDoses;
}

// Initialize a stock node element
void stockNode_init(tVaccineStockNode* node, tVaccine* vaccine, int doses) {
    assert(node != NULL);
    stock_init(&(node->elem), vaccine, doses);
    node->next = NULL;
}

// Initialize a daily stock element
void dailyStock_init(tVaccineDailyStock* stock, tDate date) {
    assert(stock != NULL);
    stock->count = 0;
    stock->first = NULL;
    stock->next = NULL;
    stock->day = date;
}

// Remove a daily stock element data
void dailyStock_free(tVaccineDailyStock* stock) {
    tVaccineStockNode* pNode;

    assert(stock != NULL);

    pNode = stock->first;

    while (pNode != NULL) {
        stock->first = pNode->next;
        free(pNode);
        pNode = stock->first;
    }

    stock->count = 0;
    stock->first = NULL;
}

// Find a vaccine node for a given daily stock
tVaccineStockNode* dailyStock_find(tVaccineDailyStock* stock, tVaccine* vaccine) {
    tVaccineStockNode* pNode;

    assert(stock != NULL);

    pNode = stock->first;

    while (pNode != NULL && pNode->elem.vaccine != vaccine) {
        pNode = pNode->next;
    }

    return pNode;
}

// Remove all vaccine entries with no doses
void dailyStock_purge(tVaccineDailyStock* stock) {
    tVaccineStockNode* pNode;
    tVaccineStockNode* pAux;

    assert(stock != NULL);

    pNode = stock->first;
    pAux = NULL;

    while (pNode != NULL) {
        if (pNode->elem.doses == 0) {
            if (pAux != NULL) {
                pAux->next = pNode->next;
                free(pNode);
                pNode = pAux->next;
            }
            else {
                stock->first = pNode->next;
                free(pNode);
                pNode = stock->first;
                pAux = NULL;
            }
            stock->count--;
        }
        else {
            pAux = pNode;
            pNode = pNode->next;
        }
    }
}

// Update the number of doses for a given vaccine
void dailyStock_update(tVaccineDailyStock* stock, tVaccine* vaccine, int doses) {
    tVaccineStockNode* pNode;
    tVaccineStockNode* pAux;

    assert(stock != NULL);

    // Search for a node for given vaccine
    pNode = dailyStock_find(stock, vaccine);

    if (pNode != NULL) {
        // Just modify the number of doses
        pNode->elem.doses += doses;
    }
    else if (stock->count == 0) {
        // If the node is empty, just add a new element
        stock->first = (tVaccineStockNode*)malloc(sizeof(tVaccineStockNode));
        assert(stock->first != NULL);
        stock_init(&(stock->first->elem), vaccine, doses);
        stock->first->next = NULL;
        stock->count++;
    }
    else if (strcmp(stock->first->elem.vaccine->name, vaccine->name) > 0) {
        // Insert as first element
        pNode = stock->first;
        stock->first = (tVaccineStockNode*)malloc(sizeof(tVaccineStockNode));
        assert(stock->first != NULL);
        stock_init(&(stock->first->elem), vaccine, doses);
        stock->first->next = pNode;
        stock->count++;
    }
    else {
        // Search for insertion point
        pNode = stock->first;
        while (pNode->next != NULL && strcmp(pNode->next->elem.vaccine->name, vaccine->name) < 0) {
            pNode = pNode->next;
        }
        pAux = pNode->next;
        pNode->next = (tVaccineStockNode*)malloc(sizeof(tVaccineStockNode));
        assert(pNode->next != NULL);
        stock_init(&(pNode->next->elem), vaccine, doses);
        pNode->next->next = pAux;
        stock->count++;
    }
}

// Copy the contents from source to destination
void dailyStock_copy(tVaccineDailyStock* src, tVaccineDailyStock* dst) {
    tVaccineStockNode* pNode;

    // Ensure destination is initialized
    dailyStock_init(dst, dst->day);

    // Add vaccines from soruce to destination
    pNode = src->first;
    while (pNode != NULL) {
        dailyStock_update(dst, pNode->elem.vaccine, pNode->elem.doses);
        pNode = pNode->next;
    }
}

// Find the stock for a given date
tVaccineDailyStock* stockList_find(tVaccineStockData* list, tDate date) {
    tVaccineDailyStock* pNode;
    tVaccineDailyStock* pDate;
    tVaccineDailyStock* last;
    assert(list != NULL);

    // Find the node
    pDate = NULL;
    last = getLastVaccineStockData(list);
    if (list->count > 0 && date_cmp(list->first->day, date) <= 0 && date_cmp(last->day, date) >= 0) {
        pNode = list->first;
        while (pNode != NULL && pDate == NULL) {
            if (date_cmp(pNode->day, date) == 0) {
                // Point current node
                pDate = pNode;
            }
            pNode = pNode->next;
        }
    }

    return pDate;
}

// Remove entries with no data on the start and end of the list
void stockList_purge(tVaccineStockData* list) {
    tVaccineDailyStock* pNode;
    tVaccineDailyStock* pAux;
    bool start;

    assert(list != NULL);

    pNode = list->first;
    pAux = NULL;

    start = true;
    while (pNode != NULL) {
        // Remove empty stocks
        dailyStock_purge(pNode);

        // If node has no data...
        if (pNode->count == 0) {
            if (start) {
                // If we are on the first element, remove the element
                list->first = pNode->next;
                dailyStock_free(pNode);
                free(pNode);
                list->count--;
                pNode = list->first;
            }
            else {
                // On other nodes just move to the next node
                pNode = pNode->next;
                start = false;
            }

        }
        else {
            // Store the last node with data
            pAux = pNode;
            pNode = pNode->next;
            start = false;
        }
    }

    // Remove empty nodes at the end
    if (pAux != NULL) {
        pNode = pAux->next;
        while (pNode != NULL) {
            pAux->next = pNode->next;
            dailyStock_free(pNode);
            free(pNode);
            list->count--;
            pNode = pAux->next;
        }
    }
}


// Extend the list adding empty day cells on left
void stockList_expandLeft(tVaccineStockData* list, tDate date) {
    tDate today;
    tVaccineDailyStock* pAux;

    // Select the day previous to the first one
    today = list->first->day;
    date_addDay(&today, -1);

    // Iterate up to the top left date
    while (date_cmp(today, date) >= 0) {
        // Store current element
        pAux = list->first;
        // Add an element at first position
        list->first = (tVaccineDailyStock*)malloc(sizeof(tVaccineDailyStock));
        assert(list->first != NULL);
        list->count++;
        // Initialize the new element
        dailyStock_init(list->first, today);
        // Set old first element as next element
        list->first->next = pAux;
        // Decrement the date
        date_addDay(&today, -1);
    }
}

// Extend the list to the right with the data of the last position
void stockList_expandRight(tVaccineStockData* list, tDate date) {
    tDate today;
    tVaccineDailyStock* last = getLastVaccineStockData(list);
    // Select as today the day after the last day
    today = last->day;
    date_addDay(&today, 1);
    // Iterate for all dates up to the top right date
    while (date_cmp(today, date) <= 0) {
        // Add an element at first position
        last->next = (tVaccineDailyStock*)malloc(sizeof(tVaccineDailyStock));
        assert(last->next != NULL);
        list->count++;
        // Initialize the new element
        dailyStock_init(last->next, today);
        // Copy the contents from old last element
        dailyStock_copy(last, last->next);
        last = last->next;
        // Increment the date
        date_addDay(&today, 1);
    }
}

// Get the stock of last day with stocks
tVaccineDailyStock* getLastVaccineStockData(tVaccineStockData* list) {
    if (list == NULL || list->count == 0) {
        return NULL; // La lista está vacía o es nula
    }

    tVaccineDailyStock* currentNode = list->first;

    if (currentNode == NULL) {
        return NULL;
    }

    // Itera a través de la lista hasta llegar al último elemento
    while (currentNode->next != NULL) {
        currentNode = currentNode->next;
    }

    return currentNode;
}

// Print on the screen the information of a Stock data
void printVaccineStockData(tVaccineStockData* list) {
    if (list == NULL || list->count == 0) {
        printf("lista vacia\n");
    }

    else {

        tVaccineDailyStock* currentNode = list->first;
        printf("lista dias:\n");
        // Itera a través de la lista hasta llegar al último elemento
        do {

            printf("%d/%d/%d\n", currentNode->day.day, currentNode->day.month, currentNode->day.year);
            printVaccineStockNode(currentNode->first);
            currentNode = currentNode->next;

        } while (currentNode != NULL);

    }
}


// Print on the screen the information of a Node of a list of vaccine stocks
void printVaccineStockNode(tVaccineStockNode* list) {
    if (list == NULL) {
        printf("día vacio\n");
    }

    else {

        tVaccineStockNode* currentNode = list;
        printf("vacunas:\n");
        // Itera a través de la lista hasta llegar al último elemento
        do {

            printf("\t%d %s\n", currentNode->elem.doses, currentNode->elem.vaccine->name);
            currentNode = currentNode->next;

        } while (currentNode != NULL);
    }
}

// Get the number of doses for a certain vaccine and date
int stockList_getDoses(tVaccineStockData* list, tDate date, tVaccine* vaccine) {
    // PR2 Ex 1c
    tVaccineDailyStock* pNode = NULL;
    tVaccineDailyStock* last = getLastVaccineStockData(list);
    int numDoses = 0;
    assert(list != NULL);

    pNode = stockList_find(list, date);

    if (pNode != NULL) {
        numDoses = stockNode_getDoses(pNode->first, vaccine);
    }
    else if (date_cmp(last->day, date) < 0) {
        numDoses = stockNode_getDoses(last->first, vaccine);
    }

    return numDoses;
    /////////////////
    // return -10000;
}
