#include <string.h>
#include <assert.h>
#include <stdlib.h>
#include "appointment.h"
#include <stdio.h>
#include "error.h"

// Initializes a vaccination appointment data list
void appointmentData_init(tAppointmentData* list) {
    //////////////////////////////////
    // Ex PR3 1a
	list->count = 0;
	list->elems = NULL;
    /////////////////////////////////

}
void insert_position(tAppointmentData* list, tAppointment appointment, int p){
	int i;
	if(p >= 0 && p <= list->count){
		if(list->count == 0){ // is empty
			list->elems = (tAppointment*) malloc(sizeof(tAppointment));
			list->elems[0].timestamp = appointment.timestamp;
			list->elems[0].person = appointment.person;
			list->elems[0].vaccine = appointment.vaccine;
			list->count++;
		}
		else{
			list->elems = (tAppointment*) realloc(list->elems, (list->count+1) * sizeof(tAppointment));
			list->count++;
			for(i = list->count-1; i >= p; i--){
				list->elems[i].timestamp = list->elems[i-1].timestamp;
				list->elems[i].person = list->elems[i-1].person;
				list->elems[i].vaccine = list->elems[i-1].vaccine;
			}
			list->elems[p].timestamp = appointment.timestamp;
			list->elems[p].person = appointment.person;
			list->elems[p].vaccine = appointment.vaccine;
		}
	}
}
// Insert a new vaccination appointment
void appointmentData_insert(tAppointmentData* list, tDateTime timestamp, tVaccine* vaccine, tPerson* person) {
    //////////////////////////////////
    // Ex PR3 1b
	int i; bool placed = false;
	tAppointment appointment;
	appointment.timestamp = timestamp;
	appointment.person = person;
	appointment.vaccine = vaccine;
	if(list->count == 0){
		insert_position(list, appointment, 0);
	}
	else{ // search place
		for(i = 0; i < list->count && !placed; i++){
			if((dateTime_cmp(timestamp, list->elems[i].timestamp) == 0 && 
				strcmp(person->document, list->elems[i].person->document) < 0) || dateTime_cmp(timestamp, list->elems[i].timestamp) < 0 ){ // same date but different docunment
				insert_position(list, appointment, i);
				placed = true;
			}
		}
		if(!placed){
			insert_position(list, appointment, list->count);
		}
	}
    /////////////////////////////////

}
void delete_position(tAppointmentData* list, int p){
	int i;
	if(p >= 0 && p <= list->count){
		for(i = p; i < list->count-1; i++){
			list->elems[i].timestamp = list->elems[i+1].timestamp;
				list->elems[i].person = list->elems[i+1].person;
				list->elems[i].vaccine = list->elems[i+1].vaccine;
		}
		list->elems = (tAppointment*) realloc(list->elems, (list->count-1) * sizeof(tAppointment));
		list->count--;
	}
}
// Remove a vaccination appointment
void appointmentData_remove(tAppointmentData* list, tDateTime timestamp, tPerson* person) {
    //////////////////////////////////
    // Ex PR3 1c
	int i; bool deleted = false;

	for(i = 0; i < list->count && !deleted; i++){
		if(dateTime_cmp(timestamp, list->elems[i].timestamp) == 0 && 
			strcmp(person->document, list->elems[i].person->document) == 0){ // same date but different docunment
			delete_position(list, i);
			deleted = true;
		}
	}
    /////////////////////////////////

}

// Find the first appointment for given person
int appointmentData_findByPerson(tAppointmentData list, tPerson* person, int start_pos) {
    //////////////////////////////////
    // Ex PR3 2a
	int i;
	if(start_pos < 0 || start_pos >= list.count){
		return E_PERSON_NOT_FOUND;
	}
	for(i=start_pos; i < list.count; i++){
		if(strcmp(person->document, list.elems[i].person->document) == 0){
			return i;
		}
	}
	
    /////////////////////////////////

    return E_NOT_IMPLEMENTED;
}

// Find the first appointment for given date and time
tAppointment* appointmentData_findByDate(tAppointmentData list, tDateTime timestamp, int start_pos) {
    //////////////////////////////////
    // Ex PR3 2b
	if(start_pos < 0 || start_pos >= list.count || list.elems == NULL){
		return NULL;
	}
	else if(dateTime_cmp(timestamp, list.elems[start_pos].timestamp) == 0){
			return &(list.elems[start_pos]);
	}
	else{
		return appointmentData_findByDate(list, timestamp, start_pos+1);
	}	
    /////////////////////////////////

    //return NULL;
}


// Returns a copy of the list of appointments sorted alphabetically by the person's name. If a person has more than one scheduled appointment,
// these will also be sorted by date in ascending order.
tAppointmentData appointmentData_orderByName(tAppointmentData data) {
    //////////////////////////////////
    // Ex PR3 2c
    ///////////////////////////////// 

    // Avoid compilation errors and warnings with temporal code
    tAppointmentData orderedData;
    appointmentData_init(&orderedData);
    return orderedData;
}


// Returns a copy of the list of appointments sorted alphabetically by the vaccine name. If a vaccine has more than one scheduled appointment,
// these will also be sorted by date in ascending order
tAppointmentData appointmentData_orderByVaccine(tAppointmentData data) {
    //////////////////////////////////
    // Ex PR3 2d
    /////////////////////////////////	  

    // Avoid compilation errors and warnings with temporal code
    tAppointmentData orderedData;
    appointmentData_init(&orderedData);
    return orderedData;
}

// Release a vaccination appointment data list
void appointmentData_free(tAppointmentData* list) {
    //////////////////////////////////
    // Ex PR3 1e
    /////////////////////////////////   
}

// AUXILIARY METHODS

// Creates and returns a copy of the provided list of appointments.
tAppointmentData appointmentData_copy(const tAppointmentData* data) {

    assert(data != NULL);

    tAppointmentData orderedData;
    orderedData.count = data->count;
    orderedData.elems = malloc(sizeof(tAppointment) * data->count);

    if (orderedData.elems == NULL) {
        orderedData.count = 0;
        return orderedData;
    }

    for (int i = 0; i < data->count; i++) {
        orderedData.elems[i] = data->elems[i];
        // If tAppointment contains pointers to other data, you may need
        // make deep copies of that data here
    }

    return orderedData;
}

// Checks if two tAppointmentData structures are equal.
int appointment_isEqual(tAppointment a, tAppointment b) {


    if (a.timestamp.date.year != b.timestamp.date.year ||
        a.timestamp.date.month != b.timestamp.date.month ||
        a.timestamp.date.day != b.timestamp.date.day) {
        return 0;
    }


    if (a.timestamp.time.hour != b.timestamp.time.hour ||
        a.timestamp.time.minutes != b.timestamp.time.minutes) {
        return 0;
    }

    if (strcmp(a.person->document, b.person->document) != 0) {
        return 0;
    }


    return 1;
}

// Checks if two tAppointmentData structures are equal.
int appointmentData_isEqual(tAppointmentData a, tAppointmentData b) {
    if (a.count != b.count) {
        return 0;
    }

    for (int i = 0; i < a.count; i++) {

        if (!appointment_isEqual(a.elems[i], b.elems[i])) {
            return 0;
        }

    }

    return 1;
}

// Prints the details of a tAppointmentData structure.
void appointmentData_print(const tAppointmentData* data) {

    for (int i = 0; i < data->count; i++) {
        appointment_print(&data->elems[i]);
        printf("\n");
    }
}

// Prints the details of a single tAppointment structure.
void appointment_print(const tAppointment* appointment) {
    if (appointment == NULL) {
        printf("Appointment is NULL.\n");
        return;
    }


    if (appointment->person != NULL) {
        printf("Person: %s %s, Document: %s\n",
            appointment->person->name,
            appointment->person->surname,
            appointment->person->document);
    }
    else {
        printf("Person information is not available.\n");
    }


    printf("\tDate: %02d/%02d/%04d, Time: %02d:%02d\n",
        appointment->timestamp.date.day,
        appointment->timestamp.date.month,
        appointment->timestamp.date.year,
        appointment->timestamp.time.hour,
        appointment->timestamp.time.minutes);


    if (appointment->vaccine != NULL) {
        printf("\tVaccine: %s, Required Doses: %d\n",
            appointment->vaccine->name,
            appointment->vaccine->required);
    }
    else {
        printf("Vaccine information is not available.\n");
    }
}
