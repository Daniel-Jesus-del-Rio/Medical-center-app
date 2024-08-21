#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include <ctype.h>
#include "center.h"

// Initialize a center
void center_init(tHealthCenter* center, const char* hcc) {
       
    assert(center != NULL);
    
    // Allocate the memory for the hcc
    center->hcc = (char*) malloc(strlen(hcc) + 1);
    assert(center->hcc != NULL);
    
    // Copy the hcc
    strcpy(center->hcc, hcc);    
    
    // Initialize the stock
    stockList_init(&(center->stock));
	
	//////////////////////////////////
    // Ex PR3 3b
	appointmentData_init(&(center->appointments));
    /////////////////////////////////
    
   	
}


// Release a center's data
void center_free(tHealthCenter* center) {
       
    assert(center != NULL);
    
    // Release hcc memory
    if(center->hcc != NULL) {
        free(center->hcc);
        center->hcc = NULL;        
    }
    
    // Remove stock data
    stockList_free(&(center->stock));   

     //////////////////////////////////
    // Ex PR3 3b
	appointmentData_free(&(center->appointments));
    /////////////////////////////////

   

}

// Initialize a list of centers
void centerList_init(tHealthCenterList* list) {
       
    assert(list != NULL);
    
    list->count = 0;
    list->first = NULL;
}

// Release a list of centers
void centerList_free(tHealthCenterList* list) {
	
    tHealthCenterNode *pNode;
    tHealthCenterNode *pAux;
    
    assert(list != NULL);
    
    // Remove all elements in the list
    pNode = list->first;
    while(pNode != NULL) {
        center_free(&(pNode->elem));
        pAux = pNode;
        pNode = pNode->next;
        free(pAux);
    }
    
    list->count = 0;
    list->first = NULL;
}

// Insert a new center
void centerList_insert(tHealthCenterList* list, const char* hcc) {
    
    tHealthCenterNode *pNode;
    tHealthCenterNode *pAux;
    
    assert(list != NULL);
    
    // Check HCC code
    if (!check_hcc(hcc)) {
        // Do not add the center because parameters are wrong
        return ;
    }
    
    if (centerList_find(list, hcc) == NULL) {    
        
        // Check if insertion point is the first position
        if (list->first == NULL || strcmp(list->first->elem.hcc, hcc) > 0) {
            // Insert as initial position
            pAux = list->first;            
            list->first = (tHealthCenterNode*) malloc(sizeof(tHealthCenterNode));
            assert(list->first != NULL);
            list->first->next = pAux;
            center_init(&(list->first->elem), hcc);
        } else {        
            // Search insertion point
            pAux = list->first;
            pNode = pAux->next;            
            while(pNode != NULL && strcmp(pNode->elem.hcc, hcc) > 0) {                
                pAux = pNode;
                pNode = pNode->next;         
            }
            pAux->next = (tHealthCenterNode*) malloc(sizeof(tHealthCenterNode));
            assert(pAux->next != NULL);
            pAux->next->next = pNode;
            center_init(&(pAux->next->elem), hcc);
        }
        // Increase the number of elements
        list->count++;
    }
}

// Find a center
tHealthCenter* centerList_find(tHealthCenterList* list, const char* hcc) {
    
    tHealthCenterNode *pNode;    
    tHealthCenter *pCenter;    
    
    assert(list != NULL);
    
    // Search center with provided hcc
    pNode = list->first;
    pCenter = NULL;
    while(pNode != NULL && pCenter == NULL) {
        if(strcmp(hcc, pNode->elem.hcc) == 0) {
            pCenter = &(pNode->elem);
        }        
        pNode = pNode->next;        
    }
    
    return pCenter;
}

// Check if a hcc is valid
bool check_hcc(const char* hcc) {
    bool valid = true;
    int i;
    
    assert(hcc != NULL);
    
    // Check total lenght
    if (valid && strlen(hcc) != 7) {
        valid = false;
    }
    
    // Check initial values
    if (valid) {
        for (i=0; i<2; i++) {
            valid = valid && isalpha(hcc[i]) && isupper(hcc[i]);
        }
    }
    
    // Check numeric codes
    if (valid) {    
        for (i=2; i<7; i++) {
            valid = valid && isdigit(hcc[i]);
        }
    }
    
    return valid;    
}

// Return a list of Centers with more than 0 doses available for that date	
tHealthCenterList centerList_withStock(tHealthCenterList* centerList, tDate date, tVaccine* vaccine) {
		
	// PR2 Ex 2a
    tHealthCenterList centersWithStock;
    centerList_init(&centersWithStock); // Initizalice the center list with stock 
    
    tHealthCenterNode* currentNode = centerList->first;
    
    while (currentNode != NULL) {
        // Verify if the center has at least one available dose for the date and the vaccine provided 
		
        int availableDoses = stockList_getUnitDoses(&currentNode->elem.stock, date, vaccine);

        
        if (availableDoses > 0) {
            // Add the center to the center list with stock 
            centerList_insert(&centersWithStock, currentNode->elem.hcc);
        }
        
        currentNode = currentNode->next;
    }
    
    return centersWithStock;
}



// Return the center code with more unit Doses avilables for that date
char* centerList_biggerStock(tHealthCenterList* centerList, tDate date, tVaccine* vaccine){
	 // PR2 Ex 2b
    tHealthCenterList centersWithStock;
    centerList_init(&centersWithStock); // Initizalice the center list with stock 
	int maxDoses = 0;
    char * hcc ;    
   
   
    
    tHealthCenterNode* currentNode = centerList->first;
    
    while (currentNode != NULL) {
        // Verify if the center has at leart one available dose for the date and the vaccine provided 
		
        int availableDoses = stockList_getUnitDoses(&currentNode->elem.stock, date, vaccine);

        // Update center with maximum (bigger) dosis if necessary
        if (availableDoses > maxDoses) {
            maxDoses = availableDoses;
			hcc = currentNode->elem.hcc; 
        }
        
        currentNode = currentNode->next;
    }
    
    return hcc;
}