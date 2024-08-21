#include <stdio.h>
#include <assert.h>
#include "csv.h"
#include "api.h"

#include <string.h>
#include "person.h"
#include "vaccine.h"


#define FILE_READ_BUFFER_SIZE 2048

// Get the API version information
const char* api_version() {
    return "UOC PP 20231";
}

// Load data from a CSV file. If reset is true, remove previous data
tApiError api_loadData(tApiData* data, const char* filename, bool reset) {
    tApiError error;
    FILE* fin;
    char buffer[FILE_READ_BUFFER_SIZE];
    tCSVEntry entry;

    // Check input data
    assert(data != NULL);
    assert(filename != NULL);

    // Reset current data    
    if (reset) {
        // Remove previous information
        error = api_freeData(data);
        if (error != E_SUCCESS) {
            return error;
        }

        // Initialize the data
        error = api_initData(data);
        if (error != E_SUCCESS) {
            return error;
        }
    }

    // Open the input file
    fin = fopen(filename, "r");
    if (fin == NULL) {
        return E_FILE_NOT_FOUND;
    }

    // Read file line by line
    while (fgets(buffer, FILE_READ_BUFFER_SIZE, fin)) {
        // Remove new line character     
        buffer[strcspn(buffer, "\n\r")] = '\0';

        csv_initEntry(&entry);
        csv_parseEntry(&entry, buffer, NULL);
        // Add this new entry to the api Data
        error = api_addDataEntry(data, entry);
        if (error != E_SUCCESS) {
            return error;
        }
        csv_freeEntry(&entry);
    }

    fclose(fin);

    return E_SUCCESS;
}

// Initialize the data structure
tApiError api_initData(tApiData* data) {
    //////////////////////////////////
    // Ex PR1 2b
    /////////////////////////////////
    // Check input data structure
    assert(data != NULL);

    // Initialize data structures
    population_init(&(data->population));
    vaccineList_init(&(data->vaccines));
    vaccineLotData_init(&(data->vaccineLots));

    //////////////////////////////////
    // Ex PR1 2b
    /////////////////////////////////
    centerList_init(&(data->centers));
    /////////////////////////////////



    return E_SUCCESS;

    /////////////////////////////////
    // return E_NOT_IMPLEMENTED;
}

// Add a new vaccines lot
tApiError api_addVaccineLot(tApiData* data, tCSVEntry entry) {
    //////////////////////////////////
    // Ex PR1 2c
    /////////////////////////////////
    tVaccine vaccine;
    tVaccineLot lot;
    tVaccine* pVaccine;

    //////////////////////////////////
    // Ex PR2 3c
    /////////////////////////////////
    tHealthCenter* pCenter;
    /////////////////////////////////

    // Check input data structure
    assert(data != NULL);

    // Check the entry type
    if (strcmp(csv_getType(&entry), "VACCINE_LOT") != 0) {
        return E_INVALID_ENTRY_TYPE;
    }

    // Check the number of fields
    if (csv_numFields(entry) != 8) {
        return E_INVALID_ENTRY_FORMAT;
    }

    // Parse the entry
    vaccineLot_parse(&vaccine, &lot, entry);

    // Check HCC code
    if (!check_hcc(lot.hcc)) {
        vaccine_free(&vaccine);
        vaccineLot_free(&lot);
        return E_INVALID_HEALTH_CENTER_CODE;
    }


    // Check if vaccine exists
    pVaccine = vaccineList_find(data->vaccines, vaccine.name);
    if (pVaccine == NULL) {
        // Add the vaccine
        vaccineList_insert(&(data->vaccines), vaccine);
        pVaccine = vaccineList_find(data->vaccines, vaccine.name);
    }
    assert(pVaccine != NULL);

    // Assign this vaccine to the lot
    lot.vaccine = pVaccine;

    // Add the lot to the data
    vaccineLotData_add(&(data->vaccineLots), lot);


    //////////////////////////////////
    // Ex PR2 3c
    /////////////////////////////////
    pCenter = centerList_find(&(data->centers), lot.hcc);
    if (pCenter == NULL) {
        centerList_insert(&(data->centers), lot.hcc);
        pCenter = centerList_find(&(data->centers), lot.hcc);
    }
    stockList_update(&(pCenter->stock), lot.timestamp.date, lot.vaccine, lot.doses);
    /////////////////////////////////


    // Release temporal data
    vaccine_free(&vaccine);
    vaccineLot_free(&lot);

    return E_SUCCESS;

    /////////////////////////////////

    //return E_NOT_IMPLEMENTED;
}

// Get the number of persons registered on the application
int api_populationCount(tApiData data) {
    //////////////////////////////////
    // Ex PR1 2d
    /////////////////////////////////
    return population_len(data.population);
    /////////////////////////////////
    //return -1;
}

// Get the number of vaccines registered on the application
int api_vaccineCount(tApiData data) {
    //////////////////////////////////
    // Ex PR1 2d
    /////////////////////////////////
    return vaccineList_len(data.vaccines);
    /////////////////////////////////
    //return -1;
}

// Get the number of vaccine lots registered on the application
int api_vaccineLotsCount(tApiData data) {
    //////////////////////////////////
    // Ex PR1 2d
    /////////////////////////////////
    return vaccineLotData_len(data.vaccineLots);
    /////////////////////////////////
    //return -1;
}


// Free all used memory
tApiError api_freeData(tApiData* data) {
    //////////////////////////////////
    // Ex PR1 2e
    /////////////////////////////////
    population_free(&(data->population));
    vaccineLotData_free(&(data->vaccineLots));
    vaccineList_free(&(data->vaccines));

    //////////////////////////////////
    // Ex PR2 3d
    /////////////////////////////////
    centerList_free(&(data->centers));
    /////////////////////////////////

    return E_SUCCESS;
    /////////////////////////////////
    //return E_NOT_IMPLEMENTED;
}


// Add a new entry
tApiError api_addDataEntry(tApiData* data, tCSVEntry entry) {
    //////////////////////////////////
    // Ex PR1 2f
    /////////////////////////////////
    tPerson person;

    assert(data != NULL);

    // Initialize the person object
    person_init(&person);

    if (strcmp(csv_getType(&entry), "PERSON") == 0) {
        // Check the number of fields
        if (csv_numFields(entry) != 7) {
            return E_INVALID_ENTRY_FORMAT;
        }
        // Parse the data
        person_parse(&person, entry);

        // Check if this person already exists
        if (population_find(data->population, person.document) >= 0) {
            // Release person object
            person_free(&person);
            return E_DUPLICATED_PERSON;
        }

        // Add the new person
        population_add(&(data->population), person);

        // Release person object
        person_free(&person);

    }
    else if (strcmp(csv_getType(&entry), "VACCINE_LOT") == 0) {
        return api_addVaccineLot(data, entry);
    }
    else {
        return E_INVALID_ENTRY_TYPE;
    }
    return E_SUCCESS;
    /////////////////////////////////
    //return E_NOT_IMPLEMENTED;
}

// Get vaccine data
tApiError api_getVaccine(tApiData data, const char* name, tCSVEntry* entry) {
    //////////////////////////////////
    // Ex PR1 3a
    /////////////////////////////////
    char buffer[2048];
    tVaccine* vaccine = NULL;

    assert(name != NULL);
    assert(entry != NULL);

    // Search vaccine
    vaccine = vaccineList_find(data.vaccines, name);

    if (vaccine == NULL) {
        return E_VACCINE_NOT_FOUND;
    }

    // Print data in the buffer
    sprintf(buffer, "%s;%d;%d", vaccine->name, vaccine->required, vaccine->days);

    // Initialize the output structure
    csv_initEntry(entry);
    csv_parseEntry(entry, buffer, "VACCINE");

    return E_SUCCESS;

    /////////////////////////////////
    //return E_NOT_IMPLEMENTED; 
}

// Get vaccine lot data
tApiError api_getVaccineLot(tApiData data, const char* hcc, const char* vaccine, tDateTime timestamp, tCSVEntry* entry) {
    //////////////////////////////////
    // Ex PR1 3b
    /////////////////////////////////
    char buffer[2048];
    int idx;

    assert(hcc != NULL);
    assert(vaccine != NULL);
    assert(entry != NULL);

    // Check HCC code
    if (!check_hcc(hcc)) {
        return E_INVALID_HEALTH_CENTER_CODE;
    }


    // Search vaccine
    idx = vaccineLotData_find(data.vaccineLots, hcc, vaccine, timestamp);

    if (idx < 0) {
        return E_LOT_NOT_FOUND;
    }

    // Print data in the buffer
    sprintf(buffer, "%02d/%02d/%04d;%02d:%02d;%s;%s;%d;%d;%d",
        data.vaccineLots.elems[idx].timestamp.date.day, data.vaccineLots.elems[idx].timestamp.date.month, data.vaccineLots.elems[idx].timestamp.date.year,
        data.vaccineLots.elems[idx].timestamp.time.hour, data.vaccineLots.elems[idx].timestamp.time.minutes,
        data.vaccineLots.elems[idx].hcc,
        data.vaccineLots.elems[idx].vaccine->name, data.vaccineLots.elems[idx].vaccine->required, data.vaccineLots.elems[idx].vaccine->days,
        data.vaccineLots.elems[idx].doses
    );

    // Initialize the output structure
    csv_initEntry(entry);
    csv_parseEntry(entry, buffer, "VACCINE_LOT");

    return E_SUCCESS;


    /////////////////////////////////
    //return E_NOT_IMPLEMENTED; 
}

// Get registered vaccines
tApiError api_getVaccines(tApiData data, tCSVData* vaccines) {
    //////////////////////////////////
    // Ex PR1 3c
    /////////////////////////////////
    char buffer[2048];
    tVaccineNode* pNode = NULL;

    csv_init(vaccines);

    pNode = data.vaccines.first;
    while (pNode != NULL) {
        sprintf(buffer, "%s;%d;%d", pNode->vaccine.name, pNode->vaccine.required, pNode->vaccine.days);
        csv_addStrEntry(vaccines, buffer, "VACCINE");
        pNode = pNode->next;
    }

    return E_SUCCESS;
    /////////////////////////////////
    //return E_NOT_IMPLEMENTED; 
}

// Get vaccine lots
tApiError api_getVaccineLots(tApiData data, tCSVData* lots) {
    //////////////////////////////////
    // Ex PR1 3d
    /////////////////////////////////
    char buffer[2048];
    int idx;

    csv_init(lots);
    for (idx = 0; idx < data.vaccineLots.count; idx++) {
        sprintf(buffer, "%02d/%02d/%04d;%02d:%02d;%s;%s;%d;%d;%d",
            data.vaccineLots.elems[idx].timestamp.date.day, data.vaccineLots.elems[idx].timestamp.date.month, data.vaccineLots.elems[idx].timestamp.date.year,
            data.vaccineLots.elems[idx].timestamp.time.hour, data.vaccineLots.elems[idx].timestamp.time.minutes,
            data.vaccineLots.elems[idx].hcc,
            data.vaccineLots.elems[idx].vaccine->name, data.vaccineLots.elems[idx].vaccine->required, data.vaccineLots.elems[idx].vaccine->days,
            data.vaccineLots.elems[idx].doses
        );
        csv_addStrEntry(lots, buffer, "VACCINE_LOT");
    }

    return E_SUCCESS;

    /////////////////////////////////
    //return E_NOT_IMPLEMENTED; 
}

// Get the number of health centers registered on the application
int api_centersCount(tApiData data) {
    //////////////////////////////////
    // Ex PR2 3e

    return data.centers.count;

    /////////////////////////////////
    // return -1;
}


// Add a new vaccination appointment
tApiError api_addAppointment(tApiData* data, const char* hcc, const char* document, const char* vaccine, tDateTime timestamp) {
    //////////////////////////////////
    // Ex PR3 3c
	
    int count=0;   
	int indexperson = -1;
        
    tVaccine *Vaccine_N = NULL;
    tHealthCenter *Center_N;
	tPerson *Person_N = NULL;
        
    // PRECONDITIONS
    assert(data != NULL);
    assert(document != NULL);
	assert(hcc != NULL);
	assert(vaccine != NULL);
    
    //USING AUX METHOD FOR FINDING VACCINE
    Vaccine_N = vaccineList_find(data->vaccines, vaccine);    
    if (Vaccine_N == NULL) {
        return E_VACCINE_NOT_FOUND;
    }
	//USING AUX METHOD FOR FINDING PERSON
	indexperson = population_find(data->population, document);
    if (indexperson < 0) {
        return E_PERSON_NOT_FOUND;
    }
	
    Person_N = &(data->population.elems[indexperson]);
	//FIND CENTER 
	Center_N = centerList_find(&(data->centers), hcc);
    if (Center_N == NULL) {
        return E_HEALTH_CENTER_NOT_FOUND;
    }
    
    //ADDING APPOINTMENT
    
	while (count < Vaccine_N->required) {
		appointmentData_insert(&(Center_N->appointments), timestamp, Vaccine_N, Person_N);
		dateTime_addDay(&timestamp, Vaccine_N->days);
		count++;
	}
    
    return E_SUCCESS;
    /////////////////////////////////

}
