#ifndef __CENTER_H__
#define __CENTER_H__

#include "stock.h"
#include "appointment.h"


// Health center
typedef struct _tHealthCenter {    
    char* hcc;
    tVaccineStockData stock;   
    //////////////////////////////////
    // Ex PR3 3a
	tAppointmentData appointments;
    /////////////////////////////////
   
 
} tHealthCenter;

// Health center list node
typedef struct _tHealthCenterNode {    
    tHealthCenter elem;
    struct _tHealthCenterNode *next;
} tHealthCenterNode;

// Health center list node
typedef struct _tHealthCenterList {    
    tHealthCenterNode* first;
    int count;
} tHealthCenterList;


// Initialize a center
void center_init(tHealthCenter* center, const char* hcc);

// Release a center's data
void center_free(tHealthCenter* center);

// Initialize a list of centers
void centerList_init(tHealthCenterList* list);

// Release a list of centers
void centerList_free(tHealthCenterList* list);

// Insert a new center
void centerList_insert(tHealthCenterList* list, const char* hcc);

// Find a center
tHealthCenter* centerList_find(tHealthCenterList* list, const char* hcc);

// Check if a hcc is valid
bool check_hcc(const char* hcc);

// Return a list of Centers with more than 0 doses available for that date
tHealthCenterList centerList_withStock(tHealthCenterList* centerList, tDate date, tVaccine* vaccine) ;

// Return the center code with more unit Doses avilables for that date
char* centerList_biggerStock(tHealthCenterList* centerList, tDate date, tVaccine* vaccine);

#endif // __CENTER_H__
