#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "test_pr3.h"
#include "api.h"

// Run all tests for PR1
bool run_pr3(tTestSuite* test_suite, const char* input) {
    bool ok = true;
    tTestSection* section = NULL;

    assert(test_suite != NULL);

    testSuite_addSection(test_suite, "PR3", "Tests for PR3 exercices");

    section = testSuite_getSection(test_suite, "PR3");
    assert(section != NULL);

    ok = run_pr3_ex1(section, input);
    ok = run_pr3_ex2(section, input) && ok;
    ok = run_pr3_ex3(section, input) && ok;

    return ok;
}

// Run all tests for Exercice 1 of PR3
bool run_pr3_ex1(tTestSection* test_section, const char* input) {
    tPerson person1, person2;
    tVaccine vModerna, vPfizer;
    tCSVEntry entry;
    tAppointmentData data;
    tDateTime dt1, dt2;
    bool passed = true;
    bool failed = false;
    bool fail_all = false;


    // Initialize sample persons
    csv_initEntry(&entry);
    person_init(&person1);
    csv_parseEntry(&entry, "87654321K;John;Smith;john.smith@example.com;My street, 25;08001;30/12/1980", "PERSON");
    person_parse(&person1, entry);
    csv_freeEntry(&entry);

    csv_initEntry(&entry);
    person_init(&person2);
    csv_parseEntry(&entry, "98765432J;Jane;Doe;jane.doe@example.com;Her street, 5;08500;12/01/1995", "PERSON");
    person_parse(&person2, entry);
    csv_freeEntry(&entry);

    // Initialize sample vaccines
    vPfizer.name = "PFIZER";
    vPfizer.days = 21;
    vPfizer.required = 2;
    vModerna.name = "MODERNA";
    vModerna.days = 0;
    vModerna.required = 1;

    /////////////////////////////
    /////  PR3 EX1 TEST 1  //////
    /////////////////////////////    
    start_test(test_section, "PR3_EX1_1", "Initialize appointment data");
    appointmentData_init(&data);
    if (data.count != 0 || data.elems != NULL) {
        failed = true;
        fail_all = true;
        passed = false;
    }
    end_test(test_section, "PR3_EX1_1", !failed);

    /////////////////////////////
    /////  PR3 EX1 TEST 2  //////
    /////////////////////////////    
    failed = fail_all;
    start_test(test_section, "PR3_EX1_2", "Insert a vaccination appointment");
    if (!fail_all) {
        dateTime_parse(&dt1, "03/05/2022", "15:25");
        appointmentData_insert(&data, dt1, &vPfizer, &person1);
        if (data.count != 1 || data.elems[0].person != &person1 || data.elems[0].vaccine != &vPfizer || !dateTime_equals(data.elems[0].timestamp, dt1)) {
            failed = true;
            fail_all = true;
            passed = false;
        }
    }
    end_test(test_section, "PR3_EX1_2", !failed);

    /////////////////////////////
    /////  PR3 EX1 TEST 3  //////
    /////////////////////////////    
    failed = fail_all;
    start_test(test_section, "PR3_EX1_3", "Insert an earlier second vaccination appointment");
    if (!fail_all) {
        dateTime_parse(&dt2, "03/05/2022", "15:24");
        appointmentData_insert(&data, dt2, &vPfizer, &person2);
        if (data.count != 2 || data.elems[0].person != &person2 || data.elems[0].vaccine != &vPfizer || !dateTime_equals(data.elems[0].timestamp, dt2)) {
            failed = true;
            fail_all = true;
            passed = false;
        }
    }
    end_test(test_section, "PR3_EX1_3", !failed);

    /////////////////////////////
    /////  PR3 EX1 TEST 4  //////
    /////////////////////////////    
    failed = fail_all;
    start_test(test_section, "PR3_EX1_4", "Insert another vaccination appointment");
    if (!fail_all) {
        dateTime_parse(&dt2, "03/05/2022", "15:24");
        appointmentData_insert(&data, dt2, &vModerna, &person1);
        if (data.count != 3 || data.elems[0].person != &person1 || data.elems[0].vaccine != &vModerna || !dateTime_equals(data.elems[0].timestamp, dt2)) {
            failed = true;
            fail_all = true;
            passed = false;
        }
    }
    end_test(test_section, "PR3_EX1_4", !failed);

    /////////////////////////////
    /////  PR3 EX1 TEST 5  //////
    /////////////////////////////    
    failed = fail_all;
    start_test(test_section, "PR3_EX1_5", "Remove last inserted vaccination appointment");
    if (!fail_all) {
        dateTime_parse(&dt2, "03/05/2022", "15:24");
        appointmentData_remove(&data, dt2, &person1);
        if (data.count != 2 || data.elems[0].person != &person2 || data.elems[0].vaccine != &vPfizer || !dateTime_equals(data.elems[0].timestamp, dt2)) {
            failed = true;
            fail_all = true;
            passed = false;
        }
    }
    end_test(test_section, "PR3_EX1_5", !failed);

    /////////////////////////////
    /////  PR3 EX1 TEST 6  //////
    /////////////////////////////    
    failed = fail_all;
    start_test(test_section, "PR3_EX1_6", "Remove previously removed vaccination appointment");
    if (!fail_all) {
        dateTime_parse(&dt2, "03/05/2022", "15:24");
        appointmentData_remove(&data, dt2, &person1);
        if (data.count != 2 || data.elems[0].person != &person2 || data.elems[0].vaccine != &vPfizer || !dateTime_equals(data.elems[0].timestamp, dt2)) {
            failed = true;
            fail_all = true;
            passed = false;
        }
    }
    end_test(test_section, "PR3_EX1_6", !failed);

    /////////////////////////////
    /////  PR3 EX1 TEST 7  //////
    /////////////////////////////    
    failed = fail_all;
    start_test(test_section, "PR3_EX1_7", "Remove first added appointment");
    if (!fail_all) {
        dateTime_parse(&dt1, "03/05/2022", "15:25");
        appointmentData_remove(&data, dt1, &person1);
        if (data.count != 1 || data.elems[0].person != &person2 || data.elems[0].vaccine != &vPfizer || !dateTime_equals(data.elems[0].timestamp, dt2)) {
            failed = true;
            fail_all = true;
            passed = false;
        }
    }
    end_test(test_section, "PR3_EX1_7", !failed);


    failed = fail_all;
    start_test(test_section, "PR3_EX1_8", "Free all appointments");
    if (!fail_all) {
        appointmentData_free(&data);
        if (person1.document == NULL || strcmp(person1.document, "87654321K") != 0) {
            failed = true;
            fail_all = true;
            passed = false;
        }
    }
    end_test(test_section, "PR3_EX1_8", !failed);

    // Release appointment data
    appointmentData_free(&data);
    person_free(&person1);
    person_free(&person2);

    return passed;
}

// Run all tests for Exercice 2 of PR3
bool run_pr3_ex2(tTestSection* test_section, const char* input) {
    tAppointmentData  data, copy;
    //tApiError error;
    tDateTime dtNotExist;
    tPerson dummyPerson;
    tVaccine vModerna, vPfizer;
    tAppointment* appointmentPointer;
    tAppointmentData orderedDataByName, orderedDataByVaccine;
    tCSVEntry entry;

    int pos;
    bool passed = true;
    bool failed = false;
    bool fail_all = false;

    const int NUM_APPOINTMENTS = 9;

    tPerson persons[NUM_APPOINTMENTS];
    tDateTime dates[NUM_APPOINTMENTS];

    const char* personData[] = {
        "87654321K;John;Smith;john.smith@example.com;My street, 25;08001;30/12/1980",
        "34567890N;Charlie;Brown;charlie.brown@example.com;Charlie's street, 55;30003;09/11/1975",
        "98765432J;Jane;Doe;jane.doe@example.com;Her street, 5;08500;12/01/1995",
        "12345678L;Alice;Johnson ;alice.johnson@example.com;Alice's street, 10;10001;15/04/1990",
        "34567890N;Charlie;Brown;charlie.brown@example.com;Charlie's street, 55;30003;09/11/1975",
        "23456789M;Bob;Williams;bob.williams@example.com;Bob's street, 42;20002;22/07/1985",
        "45678901O;Diana;Davis;diana.davis@example.com;Diana's street, 73;40004;03/03/1988",
        "56789012P;Ethan;Miller;ethan.miller@example.com;Ethan's street, 89;50005;17/05/1992",
        "34567890N;Charlie;Brown;charlie.brown@example.com;Charlie's street, 55;30003;09/11/1975",

    };

    const char* dateData[] = { "01/01/2022", "25/11/2023", "13/05/2022",	"05/06/2022", "30/07/2023",	"06/06/2022", "07/08/2022", "07/12/2022", "25/01/2023" };

    const char* timeData[] = { "10:00"		, "15:25"		, "15:24"		,"09:30"	, "10:00"	  ,	"11:15"		, "14:45"	, "16:20", "17:00" };


    csv_initEntry(&entry);
    person_init(&dummyPerson);
    csv_parseEntry(&entry, "00000001A;Dummy;Dummy;dummy.dummy@example.com;Dummy street, 1;00001;12/08/2023", "PERSON");
    person_parse(&dummyPerson, entry);
    csv_freeEntry(&entry);

    // Initialize sample vaccines
    vPfizer.name = "PFIZER";
    vPfizer.days = 21;
    vPfizer.required = 2;
    vModerna.name = "MODERNA";
    vModerna.days = 0;
    vModerna.required = 1;


    // Initialize the data    
    appointmentData_init(&data);
    if (data.count != 0 || data.elems != NULL) {
        failed = true;
        fail_all = true;
        passed = false;
    }


    // Initialize sample persons and dates
    for (int i = 0; i < NUM_APPOINTMENTS; i++) {
        csv_initEntry(&entry);
        person_init(&persons[i]);
        csv_parseEntry(&entry, personData[i], "PERSON");
        person_parse(&persons[i], entry);
        csv_freeEntry(&entry);

        dateTime_parse(&dates[i], dateData[i], timeData[i]);
        if (i % 2 == 0) {
            appointmentData_insert(&data, dates[i], &vPfizer, &persons[i]);
        }
        else {
            appointmentData_insert(&data, dates[i], &vModerna, &persons[i]);
        }


    }


    /////////////////////////////
    /////  PR3 EX2 TEST 1  //////
    /////////////////////////////    
    failed = fail_all;
    start_test(test_section, "PR3_EX2_1", "Search for the vaccine appointment of a person who exists.");
    if (!fail_all) {
        for (int i = 0; i < NUM_APPOINTMENTS; i++) {
            pos = appointmentData_findByPerson(data, &persons[i], 0);
            if (pos == E_NOT_IMPLEMENTED) {
                failed = true;
                fail_all = true;
                passed = false;
            }
            else if (pos != E_PERSON_NOT_FOUND && (strcmp(data.elems[pos].person->document, persons[i].document) != 0)) {
                failed = true;
                fail_all = true;
                passed = false;
            }
        }
    }
    end_test(test_section, "PR3_EX2_1", !failed);

    /////////////////////////////
    /////  PR3 EX1 TEST 2  //////
    /////////////////////////////    
    failed = fail_all;
    start_test(test_section, "PR3_EX2_2", "Search for the appointment of a person who does not exist");
    if (!fail_all) {
        pos = appointmentData_findByPerson(data, &dummyPerson, 0);
        if (pos != E_PERSON_NOT_FOUND) {
            failed = true;
            fail_all = true;
            passed = false;
        }
    }
    end_test(test_section, "PR3_EX2_2", !failed);

    /////////////////////////////
    /////  PR3 EX1 TEST 3  //////
    /////////////////////////////    
    failed = fail_all;
    start_test(test_section, "PR3_EX2_3", "Search for the vaccine appointment with invalid start position");
    if (!fail_all) {
        pos = appointmentData_findByPerson(data, &persons[0], 1);
        if (pos != E_PERSON_NOT_FOUND) {
            failed = true;
            fail_all = true;
            passed = false;
        }
    }
    end_test(test_section, "PR3_EX2_3", !failed);

    /////////////////////////////
    /////  PR3 EX2 TEST 4  //////
    /////////////////////////////    
    failed = fail_all;
    start_test(test_section, "PR3_EX2_4", "Search for a vaccine appointment on a date that exists.");
    if (!fail_all) {

        for (int i = 0; i < NUM_APPOINTMENTS; i++) {
            appointmentPointer = appointmentData_findByDate(data, dates[i], 0);
            if (appointmentPointer != NULL) {
                if ((dateTime_cmp(appointmentPointer->timestamp, dates[i]) != 0)) {
                    failed = true;
                    fail_all = true;
                    passed = false;

                }
            }

            else {
                failed = true;
                fail_all = true;
                passed = false;
            }
        }

    }
    end_test(test_section, "PR3_EX2_4", !failed);

    /////////////////////////////
    /////  PR3 EX2 TEST 5  //////
    /////////////////////////////    
    failed = fail_all;
    start_test(test_section, "PR3_EX2_5", "Search for a vaccine appointment on a date that not exists.");

    dateTime_parse(&dtNotExist, "08/12/2023", "12:55");

    if (!fail_all) {
        appointmentPointer = appointmentData_findByDate(data, dtNotExist, 0);
        if (appointmentPointer != NULL) {
            failed = true;
            fail_all = true;
            passed = false;
        }

    }
    end_test(test_section, "PR3_EX2_5", !failed);

    /////////////////////////////
    /////  PR3 EX2 TEST 6  //////
    /////////////////////////////    
    failed = fail_all;
    start_test(test_section, "PR3_EX2_6", "Sort appointments by the name of the person. Check data has not changed");
    if (!fail_all) {

        copy = appointmentData_copy(&data);

        orderedDataByName = appointmentData_orderByName(data);

        // Check if data is preserved		
        if (!appointmentData_isEqual(copy, data)) {
            failed = true;
            passed = false;
        }


        appointmentData_free(&copy);
    }
    end_test(test_section, "PR3_EX2_6", !failed);

    /////////////////////////////
    /////  PR3 EX2 TEST 7  //////
    /////////////////////////////    
    failed = fail_all;
    start_test(test_section, "PR3_EX2_7", "Sort appointments by the name of the person. Verify the order");
    if (!fail_all) {


        // check if data is ordered			

        bool isOrdered = appointment_isEqual(data.elems[0], orderedDataByName.elems[8]) &&
            appointment_isEqual(data.elems[1], orderedDataByName.elems[7]) &&
            appointment_isEqual(data.elems[2], orderedDataByName.elems[0]) &&
            appointment_isEqual(data.elems[3], orderedDataByName.elems[1]) &&
            appointment_isEqual(data.elems[4], orderedDataByName.elems[5]) &&
            appointment_isEqual(data.elems[5], orderedDataByName.elems[6]) &&
            appointment_isEqual(data.elems[6], orderedDataByName.elems[2]) &&
            appointment_isEqual(data.elems[7], orderedDataByName.elems[3]) &&
            appointment_isEqual(data.elems[8], orderedDataByName.elems[4]);


        if (!isOrdered) {
            failed = true;
            passed = false;
        }



    }

    end_test(test_section, "PR3_EX2_7", !failed);


    /////////////////////////////
    /////  PR3 EX2 TEST 8  //////
    /////////////////////////////    
    failed = fail_all;
    start_test(test_section, "PR3_EX2_8", "Sort appointments by the name of the vaccine. Check data has not changed");
    if (!fail_all) {

        copy = appointmentData_copy(&data);

        orderedDataByVaccine = appointmentData_orderByVaccine(data);

        // Check if data is preserved		
        if (!appointmentData_isEqual(copy, data)) {
            failed = true;
            passed = false;
        }


        appointmentData_free(&copy);


    }
    end_test(test_section, "PR3_EX2_8", !failed);

    /////////////////////////////
    /////  PR3 EX2 TEST 8  //////
    /////////////////////////////    
    failed = fail_all;
    start_test(test_section, "PR3_EX2_9", "Sort appointments by the name of the vaccine. Verify the order.");
    if (!fail_all) {

        // check if data is ordered			

        bool isOrdered = appointment_isEqual(data.elems[0], orderedDataByVaccine.elems[4]) &&
            appointment_isEqual(data.elems[1], orderedDataByVaccine.elems[5]) &&
            appointment_isEqual(data.elems[2], orderedDataByVaccine.elems[0]) &&
            appointment_isEqual(data.elems[3], orderedDataByVaccine.elems[1]) &&
            appointment_isEqual(data.elems[4], orderedDataByVaccine.elems[6]) &&
            appointment_isEqual(data.elems[5], orderedDataByVaccine.elems[2]) &&
            appointment_isEqual(data.elems[6], orderedDataByVaccine.elems[7]) &&
            appointment_isEqual(data.elems[7], orderedDataByVaccine.elems[8]) &&
            appointment_isEqual(data.elems[8], orderedDataByVaccine.elems[3]);


        if (!isOrdered) {
            failed = true;
            passed = false;
        }

    }

    appointmentData_free(&copy);


    end_test(test_section, "PR3_EX2_9", !failed);

    appointmentData_free(&orderedDataByVaccine);
    appointmentData_free(&orderedDataByName);
    appointmentData_free(&data);

    for (int i = 0; i < NUM_APPOINTMENTS; i++) {
        person_free(&persons[i]);
    }
    person_free(&dummyPerson);

    return passed;
}


// Run all tests for Exercice 3 of PR3
bool run_pr3_ex3(tTestSection* test_section, const char* input) {
    tApiData data;
    tApiError error;
    tDateTime dt1;
    bool passed = true;
    bool failed = false;
    bool fail_all = false;

    // Initialize the data    
    error = api_initData(&data);
    if (error != E_SUCCESS) {
        passed = false;
        failed = true;
        fail_all = true;
    }

    // Load the test data    
    if (!fail_all) {
        error = api_loadData(&data, input, true);
        if (error != E_SUCCESS) {
            passed = false;
            failed = true;
            fail_all = true;
        }
    }

    /////////////////////////////
    /////  PR3 EX3 TEST 1  //////
    /////////////////////////////    
    failed = fail_all;
    start_test(test_section, "PR3_EX3_1", "Add an appointment for a non-existing person");
    if (!fail_all) {
        dateTime_parse(&dt1, "03/05/2022", "15:25");
        error = api_addAppointment(&data, "08500", "67654329J", "MODERNA", dt1);
        if (error != E_PERSON_NOT_FOUND) {
            failed = true;
            fail_all = true;
            passed = false;
        }
    }
    end_test(test_section, "PR3_EX3_1", !failed);

    /////////////////////////////
    /////  PR3 EX3 TEST 2  //////
    /////////////////////////////    
    failed = fail_all;
    start_test(test_section, "PR3_EX3_2", "Add an appointment for a non-existing vaccine");
    if (!fail_all) {
        dateTime_parse(&dt1, "03/05/2022", "15:25");
        error = api_addAppointment(&data, "08500", "98765432J", "NON-EXITING", dt1);
        if (error != E_VACCINE_NOT_FOUND) {
            failed = true;
            fail_all = true;
            passed = false;
        }
    }
    end_test(test_section, "PR3_EX3_2", !failed);

    /////////////////////////////
    /////  PR3 EX3 TEST 3  //////
    /////////////////////////////    
    failed = fail_all;
    start_test(test_section, "PR3_EX3_3", "Add an appointment for a non-existing center");
    if (!fail_all) {
        dateTime_parse(&dt1, "03/05/2022", "15:25");
        error = api_addAppointment(&data, "08509", "98765432J", "MODERNA", dt1);
        if (error != E_HEALTH_CENTER_NOT_FOUND) {
            failed = true;
            fail_all = true;
            passed = false;
        }
    }
    end_test(test_section, "PR3_EX3_3", !failed);


    // Release all data
    api_freeData(&data);

    return passed;
}

