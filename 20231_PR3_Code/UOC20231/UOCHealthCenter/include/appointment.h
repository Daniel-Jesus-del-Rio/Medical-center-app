#ifndef __APPOINTMENT__H
#define __APPOINTMENT__H
#include "date.h"
#include "person.h"
#include "vaccine.h"

// Type that stores a vaccination appointment data
typedef struct _tAppointment {    
    // Timestamp of the vaccination appointment
    tDateTime timestamp;
    // Person
    tPerson* person;
    // Vaccine
    tVaccine* vaccine;
} tAppointment;


// Type that stores a list of vaccination appointments
typedef struct _tAppointmentData {    
    // Vaccination appointments
    tAppointment* elems;
    // Number of elements
    int count;    
} tAppointmentData;

// Initializes a vaccination appointment data list
void appointmentData_init(tAppointmentData* list);

// Insert a new vaccination appointment
void appointmentData_insert(tAppointmentData* list, tDateTime timestamp, tVaccine* vaccine, tPerson* person);

// Remove a vaccination appointment
void appointmentData_remove(tAppointmentData* list, tDateTime timestamp, tPerson* person);

// Release a vaccination appointment data list
void appointmentData_free(tAppointmentData* list);

// Find the first instance of a vaccination appointment for given person
int appointmentData_findByPerson(tAppointmentData list, tPerson* person, int start_pos);

// Find the first appointment for given date and time
tAppointment * appointmentData_findByDate(tAppointmentData list, tDateTime timestamp, int start_pos);

// Returns a copy of the list of appointments sorted alphabetically by the person's name. If a person has more than one scheduled appointment,
// these will also be sorted by date in ascending order.
tAppointmentData appointmentData_orderByName(tAppointmentData data);

// Returns a copy of the list of appointments sorted alphabetically by the vaccine name. If a vaccine has more than one scheduled appointment,
// these will also be sorted by date in ascending order
tAppointmentData appointmentData_orderByVaccine (tAppointmentData data);

// AUX

// Creates and returns a copy of the provided list of appointments.
tAppointmentData appointmentData_copy(const tAppointmentData *data);

// Checks if two tAppointment structures are equal.
int appointment_isEqual(tAppointment a, tAppointment b);

// Checks if two tAppointmentData structures are equal.
int appointmentData_isEqual(tAppointmentData a, tAppointmentData b);

// Prints the details of a tAppointmentData structure.
void appointmentData_print(const tAppointmentData *data);

// Prints the details of a single tAppointment structure.
void appointment_print(const tAppointment *appointment);
void insert_position(tAppointmentData* list, tAppointment appointment, int p);
void delete_position(tAppointmentData* list, int p);
#endif // __APPOINTMENT__H
