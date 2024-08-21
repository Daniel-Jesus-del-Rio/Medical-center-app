#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "test_pr2.h"
#include "api.h"

// Run all tests for PR1
bool run_pr2(tTestSuite* test_suite, const char* input) {
    bool ok = true;
    tTestSection* section = NULL;

    assert(test_suite != NULL);

    testSuite_addSection(test_suite, "PR2", "Tests for PR2 exercices");

    section = testSuite_getSection(test_suite, "PR2");
    assert(section != NULL);

    ok = run_pr2_ex1(section, input);
    ok = run_pr2_ex2(section, input) && ok;
    ok = run_pr2_ex3(section, input) && ok;

    return ok;
}

// Run all tests for Exercice 1 of PR1
bool run_pr2_ex1(tTestSection* test_section, const char* input) {
    tDate date;
    int i;
    tVaccineStockData stock;
    tVaccineDailyStock* pDailyStockNode;
    tVaccine vModerna, vPfizer, vAAAA;
    bool passed = true;
    bool failed = false;
    bool fail_all = false;

    // Initialize sample vaccines
    vPfizer.name = "PFIZER";
    vPfizer.days = 21;
    vPfizer.required = 2;
    vPfizer.unitDoses = 5;
    vModerna.name = "MODERNA";
    vModerna.days = 0;
    vModerna.required = 1;
    vModerna.unitDoses = 2;
    vAAAA.name = "AAAAA";
    vAAAA.days = 27;
    vAAAA.required = 3;
    vAAAA.unitDoses = 3;
    /////////////////////////////
    /////  PR2 EX1 TEST 1  //////
    /////////////////////////////    
    start_test(test_section, "PR2_EX1_1", "Initialize Stock data structure");

    // Set incorrect value
    stock.count = -34;

    // Initialize the data    
    stockList_init(&stock);
    if (stock.count != 0 || stock.first != NULL) {
        failed = true;
        passed = false;
        fail_all = true;
    }
    end_test(test_section, "PR2_EX1_1", !failed);

    /////////////////////////////
    /////  PR2 EX1 TEST 2  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX1_2", "Add vaccine lot to empty stock");

    if (!fail_all) {
        // Add the vaccine lot
        date_parse(&date, "02/01/2022");

        stockList_update(&stock, date, &vModerna, 50);

        // Check result
        if (stock.count != 1) {
            failed = true;
        } else if (stock.first->count != 1 || stock.first == NULL ) {
            failed = true;
        } else if (date_cmp(stock.first->day, date) != 0) {
            failed = true;
        } else if (stock.first->count != 1 || stock.first->first == NULL ) {
            failed = true;
        } else if (stock.first->first->elem.vaccine != &vModerna || stock.first->first->elem.doses != 50 ) {
            failed = true;        
        }
        
        if (failed) {
            fail_all = true;
            passed = false;
        }
    }
    end_test(test_section, "PR2_EX1_2", !failed);


    /////////////////////////////
    /////  PR2 EX1 TEST 3  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX1_3", "Add vaccine lot with days step");

    if (!fail_all) {
        // Add the vaccine lot
        date_parse(&date, "05/01/2022");
        stockList_update(&stock, date, &vModerna, 10);
        //printVaccineStockData(&stock);
        // Check result
        if (stock.count != 4) {
            failed = true;
        } else {
            date_parse(&date, "02/01/2022");            
            pDailyStockNode = stock.first;



            for (i = 0; i < 4 && !failed; i++) {
                date.day = i + 2;
                if (pDailyStockNode == NULL) {
                    failed = true;
                } else {
                    if (date_cmp(pDailyStockNode->day, date) != 0 || pDailyStockNode->count != 1 || pDailyStockNode->first == NULL) {
                        failed = true;
                    } else if (i<3 && pDailyStockNode->first->elem.doses != 50) {
                        failed = true;                        
                    } else if (i==3 && pDailyStockNode->first->elem.doses != 60) {
                        failed = true;
                    }
                }
                pDailyStockNode = pDailyStockNode->next;
            }
        }

        if (failed) {
            fail_all = true;
            passed = false;
        }
    }
    end_test(test_section, "PR2_EX1_3", !failed);

    /////////////////////////////
    /////  PR2 EX1 TEST 4  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX1_4", "Add vaccine before previous stocks");

    if (!fail_all) {
        // Add the vaccine lot
        date_parse(&date, "01/01/2022");
        stockList_update(&stock, date, &vModerna, 2);

        // Check result
        if (stock.count != 5) {
            failed = true;
        } else {
            date_parse(&date, "01/01/2022");            
            pDailyStockNode = stock.first;
            for (i = 0; i < 5 && !failed; i++) {
                date.day = i + 1;
                if (pDailyStockNode == NULL) {
                    failed = true;
                } else {
                    if (date_cmp(pDailyStockNode->day, date) != 0 || pDailyStockNode->count != 1 || pDailyStockNode->first == NULL) {
                        failed = true;
                    } else if (i<1 && pDailyStockNode->first->elem.doses != 2) {
                        failed = true;                        
                    } else if (i>0 && i<4 && pDailyStockNode->first->elem.doses != 52) {
                        failed = true;                        
                    } else if (i==4 && pDailyStockNode->first->elem.doses != 62) {
                        failed = true;
                    }
                }
                pDailyStockNode = pDailyStockNode->next;
            }
        }

        if (failed) {
            fail_all = true;
            passed = false;
        }
    }
    end_test(test_section, "PR2_EX1_4", !failed);

    /////////////////////////////
    /////  PR2 EX1 TEST 5  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX1_5", "Add other vaccine");

    if (!fail_all) {
        // Add the vaccine lot
        date_parse(&date, "03/01/2022");
        stockList_update(&stock, date, &vPfizer, 30);

        // Check result
        if (stock.count != 5) {
            failed = true;
        } else {
            date_parse(&date, "01/01/2022");            
            pDailyStockNode = stock.first;
            for (i = 0; i < 5 && !failed; i++) {
                date.day = i + 1;
                if (pDailyStockNode == NULL) {
                    failed = true;
                } else {
                    if (date_cmp(pDailyStockNode->day, date) != 0 || pDailyStockNode->first == NULL) {
                        failed = true;
                    } else if (i<1 && pDailyStockNode->first->elem.doses != 2) {
                        failed = true;                        
                    } else if (i>0 && i<4 && pDailyStockNode->first->elem.doses != 52) {
                        failed = true;            
                    } else if (i>=2 && pDailyStockNode->count != 2) {
                        failed = true;
                    } else if (i<2 && pDailyStockNode->count != 1) {
                        failed = true;                    
                    } else if (i==4 && pDailyStockNode->first->elem.doses != 62) {
                        failed = true;
                    }
                }
                pDailyStockNode = pDailyStockNode->next;
            }
        }

        if (failed) {
            fail_all = true;
            passed = false;
        }
    }
    end_test(test_section, "PR2_EX1_5", !failed);

    /////////////////////////////
    /////  PR2 EX1 TEST 6  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX1_6", "Decrease the number of vaccines reverting last test");

    if (!fail_all) {
        // Add the vaccine lot
        date_parse(&date, "03/01/2022");
        stockList_update(&stock, date, &vPfizer, -30);

        // Check result
        if (stock.count != 5) {
            failed = true;
        } else {
            date_parse(&date, "01/01/2022");            
            pDailyStockNode = stock.first;
            for (i = 0; i < 5 && !failed; i++) {
                date.day = i + 1;
                if (pDailyStockNode == NULL) {
                    failed = true;
                } else {
                    if (date_cmp(pDailyStockNode->day, date) != 0 || pDailyStockNode->count != 1 || pDailyStockNode->first == NULL) {
                        failed = true;
                    } else if (i<1 && pDailyStockNode->first->elem.doses != 2) {
                        failed = true;                        
                    } else if (i>0 && i<4 && pDailyStockNode->first->elem.doses != 52) {
                        failed = true;                        
                    } else if (i==4 && pDailyStockNode->first->elem.doses != 62) {
                        failed = true;
                    }
                }
                pDailyStockNode = pDailyStockNode->next;
            }
        }

        if (failed) {
            fail_all = true;
            passed = false;
        }
    }
    end_test(test_section, "PR2_EX1_6", !failed);

    /////////////////////////////
    /////  PR2 EX1 TEST 7  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX1_7", "Decrease the number of vaccines removing stocks in the middle");

    if (!fail_all) {
        // Add the vaccine lot
        date_parse(&date, "03/01/2022");
        stockList_update(&stock, date, &vModerna, -52);

        // Check result
        if (stock.count != 5) {
            failed = true;
        } else {
            date_parse(&date, "01/01/2022");            
            pDailyStockNode = stock.first;
            for (i = 0; i < 5 && !failed; i++) {
                date.day = i + 1;
                if (pDailyStockNode == NULL) {
                    failed = true;
                } else {
                    if (date_cmp(pDailyStockNode->day, date) != 0) {
                        failed = true;
                    } else if((i<2 || i >3) && (pDailyStockNode->count != 1 || pDailyStockNode->first == NULL)) {
                        failed = true;                    
                    } else if (i<1 && pDailyStockNode->first->elem.doses != 2 ) {
                        failed = true;                        
                    } else if (i>0 && i<2 && pDailyStockNode->first->elem.doses != 52) {
                        failed = true;                        
                    } else if (i>=2 && i<4 && pDailyStockNode->count != 0) {
                        failed = true;                        
                    } else if (i==4 && pDailyStockNode->first->elem.doses != 10) {
                        failed = true;
                    }
                }
                pDailyStockNode = pDailyStockNode->next;
            }
        }

        if (failed) {
            fail_all = true;
            passed = false;
        }
    }
    end_test(test_section, "PR2_EX1_7", !failed);

    /////////////////////////////
    /////  PR2 EX1 TEST 8  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX1_8", "Request the number of doses");

    if (!fail_all) {
        // Parse first date                
        date_parse(&date, "01/01/2022");
        if (stockList_getUnitDoses(&stock, date, &vPfizer) != 0) {
            failed = true;
        } else if (stockList_getUnitDoses(&stock, date, &vModerna) != 4) {
            failed = true;
        }
        // Parse other date                
        date_parse(&date, "02/01/2022");
        if (stockList_getUnitDoses(&stock, date, &vPfizer) != 0) {
            failed = true;
        } else if (stockList_getUnitDoses(&stock, date, &vModerna) != 104) {
            failed = true;
        }
        // Parse other date                
        date_parse(&date, "03/01/2022");
        if (stockList_getUnitDoses(&stock, date, &vPfizer) != 0) {
            failed = true;
        } else if (stockList_getUnitDoses(&stock, date, &vModerna) != 0) {
            failed = true;
        }
        // Parse non existing date from before the stock range
        date_parse(&date, "30/12/2021");
        if (stockList_getUnitDoses(&stock, date, &vPfizer) != 0) {
            failed = true;
        } else if (stockList_getUnitDoses(&stock, date, &vModerna) != 0) {
            failed = true;
        }

        // Parse non existing date from after the stock range
        date_parse(&date, "30/01/2022");
        if (stockList_getUnitDoses(&stock, date, &vPfizer) != 0) {
            failed = true;
        } else if (stockList_getUnitDoses(&stock, date, &vModerna) != 20) {
            failed = true;
        }


        if (failed) {
            fail_all = true;
            passed = false;
        }
    }
    end_test(test_section, "PR2_EX1_8", !failed);

    /////////////////////////////
    /////  PR2 EX1 TEST 9  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX1_9", "Add a new vaccine to check stock order");

    if (!fail_all) {
        // Add the vaccine lot
        date_parse(&date, "01/01/2022");
        stockList_update(&stock, date, &vAAAA, 12);

        // Check result
        if (stock.count != 5) {
            failed = true;
        } else if (stock.first->count != 2) {
            failed = true;
        } else if (stock.first->first->elem.vaccine != &vAAAA) {
            failed = true;
        } else if (stock.first->first->next->elem.vaccine != &vModerna) {
            failed = true;
        }

        if (failed) {
            fail_all = true;
            passed = false;
        }
    }
    end_test(test_section, "PR2_EX1_9", !failed);

    // Release used memory
    stockList_free(&stock);

    return passed;
}

// Run all tests for Exercice 2 of PR2
bool run_pr2_ex2(tTestSection* test_section, const char* input) {

    tDate dateWithStock;
    tVaccine vModerna;
    tVaccine vPfizer;
    tVaccine vaccineToCheck;

    tHealthCenterList newList;


    char* hcc;
    bool passed = true;
    bool failed = false;
    bool fail_all = false;

    // Initialize sample vaccines    
    vModerna.name = "MODERNA";
    vModerna.days = 0;
    vModerna.required = 1;
    vModerna.unitDoses = 2;

    // Initialize sample vaccines    
    vPfizer.name = "PFIZER";
    vPfizer.days = 0;
    vPfizer.required = 1;
    vPfizer.unitDoses = 5;

    /*******************************************************************************/
    /* Example of possibly test to verify the example functions from the statement */
    /*******************************************************************************/

    /*
    tHealthCenter *pCenter;
    tHealthCenter center;
    tHealthCenterList list;
    tDate date;

    /////////////////////////////
    /////  PR2 EX2 TEST 1  //////
    /////////////////////////////
    start_test(test_section, "PR2_EX2_1", "Initialize a center");

    // Set incorrect value
    center.stock.count = -34;

    // Initialize the data
    center_init(&center, "08016");
    if (center.stock.count != 0 || strcmp(center.hcc, "08016") != 0) {
        failed = true;
        passed = false;
        fail_all = true;
    }
    end_test(test_section, "PR2_EX2_1", !failed);

    /////////////////////////////
    /////  PR2 EX2 TEST 2  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX2_2", "Remove center data");

    if (!fail_all) {
        // Add the vaccine lot
        date_parse(&date, "01/01/2022");
        stockList_update(&(center.stock), date, &vModerna, 12);

        // Clean center data
        center_free(&center);
        if (center.hcc != NULL) {
            failed = true;
        } else if (center.stock.count != 0) {
            failed = true;
        } else if (center.stock.first != NULL) {
            failed = true;
        }

        if (failed) {
            fail_all = true;
            passed = false;
        }
    }
    end_test(test_section, "PR2_EX2_2", !failed);

    /////////////////////////////
    /////  PR2 EX2 TEST 3  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX2_3", "Initialize list of health centers");

    if (!fail_all) {
        // Set incorrect value
        list.count = -34;

        // Initialize the data
        centerList_init(&list);
        if (list.count != 0 || list.first != NULL) {
            failed = true;
            passed = false;
            fail_all = true;
        }
    }
    end_test(test_section, "PR2_EX2_3", !failed);

    /////////////////////////////
    /////  PR2 EX2 TEST 4  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX2_4", "Insert a health center to an empty list");

    if (!fail_all) {

        // Initialize the data
        centerList_init(&list);

        // Insert a new center
        centerList_insert(&list, "EE08004");


        if (list.count != 1 || list.first == NULL || strcmp(list.first->elem.hcc, "EE08004") != 0) {
            failed = true;
            passed = false;
            fail_all = true;
        }
    }
    end_test(test_section, "PR2_EX2_4", !failed);

    /////////////////////////////
    /////  PR2 EX2 TEST 5  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX2_5", "Test hcc sorting");

    if (!fail_all) {
        // Insert a new center
        centerList_insert(&list, "ES08005");

        if (list.count != 2 || list.first == NULL || strcmp(list.first->elem.hcc, "EE08004") != 0) {
            failed = true;
            passed = false;
            fail_all = true;
        } else if (list.first->next == NULL || strcmp(list.first->next->elem.hcc, "ES08005") != 0) {
            failed = true;
            passed = false;
            fail_all = true;
        }

        // Insert a new center
        centerList_insert(&list, "CA08001");

        if (list.count != 3 || list.first == NULL || strcmp(list.first->elem.hcc, "CA08001") != 0) {
            failed = true;
            passed = false;
            fail_all = true;
        } else if (list.first->next == NULL || strcmp(list.first->next->elem.hcc, "EE08004") != 0) {
            failed = true;
            passed = false;
            fail_all = true;
        } else if (list.first->next->next == NULL || strcmp(list.first->next->next->elem.hcc, "ES08005") != 0) {
            failed = true;
            passed = false;
            fail_all = true;
        }
    }
    end_test(test_section, "PR2_EX2_5", !failed);

    /////////////////////////////
    /////  PR2 EX2 TEST 6  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX2_6", "Find an existing health center by hcc");

    if (!fail_all) {

        // Initialize the data
        pCenter = centerList_find(&list, "CA08001");
        if (pCenter == NULL || strcmp(pCenter->hcc, "CA08001") != 0) {
            failed = true;
            passed = false;
            fail_all = true;
        }
    }
    end_test(test_section, "PR2_EX2_6", !failed);

    /////////////////////////////
    /////  PR2 EX2 TEST 7  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX2_7", "Find a non existing health center by hcc");

    if (!fail_all) {

        // Initialize the data
        pCenter = centerList_find(&list, "08500");
        if (pCenter != NULL) {
            failed = true;
            passed = false;
            fail_all = true;
        }
    }
    end_test(test_section, "PR2_EX2_7", !failed);





    /////////////////////////////
    /////  PR2 EX2 TEST 8 //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX2_8", "Release a list of health centers");

    if (!fail_all) {

        // Initialize the data
        centerList_free(&list);
        if (list.count != 0 || list.first != NULL) {
            failed = true;
            passed = false;
            fail_all = true;
        }
    }
    end_test(test_section, "PR2_EX2_8", !failed);

    */

    /////////////////////////////
    /////  PR2 EX2 TEST 1  //////
    /////////////////////////////

    failed = fail_all;
    start_test(test_section, "PR2_EX2_1", "Test centerList_withStock");

    if (!fail_all) {
        // Initialize a health center with available stock for the given date
        tHealthCenter* centerWithStock;


        centerList_init(&newList);
        // Add the health center with available stock to the list of centers 
        centerList_insert(&newList, "CA08001");
        centerList_insert(&newList, "MD08002");
        centerList_insert(&newList, "FC08003");
        centerList_insert(&newList, "EE08004");
        centerList_insert(&newList, "ES08005");

        centerWithStock = centerList_find(&newList, "CA08001");

        if (centerWithStock == NULL) {
            failed = true;
            passed = false;
            fail_all = true;
        }
        else {
            date_parse(&dateWithStock, "01/01/2022");
            stockList_update(&(centerWithStock->stock), dateWithStock, &vModerna, 2);

            centerWithStock = centerList_find(&newList, "MD08002");

            if (centerWithStock == NULL) {
                failed = true;
                passed = false;
                fail_all = true;
            }
            else {
                date_parse(&dateWithStock, "11/01/2023");
                stockList_update(&(centerWithStock->stock), dateWithStock, &vPfizer, 3);

                centerWithStock = centerList_find(&newList, "FC08003");

                if (centerWithStock == NULL) {
                    failed = true;
                    passed = false;
                    fail_all = true;
                }
                else {
                    date_parse(&dateWithStock, "01/01/2022");
                    stockList_update(&(centerWithStock->stock), dateWithStock, &vModerna, 10);

                    centerWithStock = centerList_find(&newList, "EE08004");

                    if (centerWithStock == NULL) {
                        failed = true;
                        passed = false;
                        fail_all = true;
                    }
                    else {
                        date_parse(&dateWithStock, "11/01/2023");
                        stockList_update(&(centerWithStock->stock), dateWithStock, &vPfizer, 50);

                        centerWithStock = centerList_find(&newList, "ES08005");

                        if (centerWithStock == NULL) {
                            failed = true;
                            passed = false;
                            fail_all = true;
                        }
                        else {
                            date_parse(&dateWithStock, "01/01/2022");
                            stockList_update(&(centerWithStock->stock), dateWithStock, &vModerna, 5);
                        }
                    }
                }
            }
        }

        // Call centerList_withStock to obtain centers with stock for the specified date
        if (!fail_all) {
            vaccineToCheck.name = "MODERNA";
            vaccineToCheck.days = 0;
            vaccineToCheck.required = 1;
            vaccineToCheck.unitDoses = 2;


            date_parse(&dateWithStock, "01/01/2022");
            tHealthCenterList centersWithStock = centerList_withStock(&newList, dateWithStock, &vaccineToCheck);


            centerWithStock = centerList_find(&centersWithStock, "CA08001");

            if (centersWithStock.count != 3) {
                failed = true;
                passed = false;
                fail_all = true;
            }

            if (centerWithStock == NULL || strcmp(centerWithStock->hcc, "CA08001") != 0) {
                failed = true;
                passed = false;
                fail_all = true;
            }


            centerWithStock = centerList_find(&centersWithStock, "FC08003");

            if (centerWithStock == NULL || strcmp(centerWithStock->hcc, "FC08003") != 0) {
                failed = true;
                passed = false;
                fail_all = true;
            }

            centerWithStock = centerList_find(&centersWithStock, "ES08005");

            if (centerWithStock == NULL || strcmp(centerWithStock->hcc, "ES08005") != 0) {
                failed = true;
                passed = false;
                fail_all = true;
            }

            vaccineToCheck.name = "PFIZER";
            vaccineToCheck.days = 0;
            vaccineToCheck.required = 1;
            vaccineToCheck.unitDoses = 2;

            date_parse(&dateWithStock, "11/01/2023");

            centerList_free(&centersWithStock);

            centersWithStock = centerList_withStock(&newList, dateWithStock, &vaccineToCheck);

            if (centersWithStock.count != 2) {
                failed = true;
                passed = false;
                fail_all = true;
            }

            centerWithStock = centerList_find(&centersWithStock, "MD08002");

            if (centerWithStock == NULL || strcmp(centerWithStock->hcc, "MD08002") != 0) {
                failed = true;
                passed = false;
                fail_all = true;
            }


            centerWithStock = centerList_find(&centersWithStock, "EE08004");

            if (centerWithStock == NULL || strcmp(centerWithStock->hcc, "EE08004") != 0) {
                failed = true;
                passed = false;
                fail_all = true;
            }

            centerList_free(&centersWithStock);
        }
    }

    end_test(test_section, "PR2_EX2_1", !failed);


    /////////////////////////////
    /////  PR2 EX2 TEST 2 //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX2_2", "Test centerList_biggerStock");

    if (!fail_all) {

        date_parse(&dateWithStock, "01/01/2022");
        vaccineToCheck.name = "MODERNA";

        hcc = centerList_biggerStock(&newList, dateWithStock, &vaccineToCheck);


        if (hcc == NULL) {
            failed = true;
            passed = false;
        }

        else if (strcmp(hcc, "FC08003") != 0) {
            failed = true;
            passed = false;
            fail_all = true;
        }

        vaccineToCheck.name = "PFIZER";

        date_parse(&dateWithStock, "11/01/2023");

        hcc = centerList_biggerStock(&newList, dateWithStock, &vaccineToCheck);

        if (hcc == NULL) {
            failed = true;
            passed = false;
        }

        else if (strcmp(hcc, "EE08004") != 0) {
            failed = true;
            passed = false;
            fail_all = true;
        }
    }

    end_test(test_section, "PR2_EX2_2", !failed);

    centerList_free(&newList);

    return passed;
}

// Run all tests for Exercice 3 of PR2
bool run_pr2_ex3(tTestSection* test_section, const char* input) {
    tApiData data;
    tApiError error;
    int nLots;
    int nVaccines;
    int nPeople;
    int nCenters;
    bool passed = true;
    bool failed = false;
    bool fail_all = false;

    /////////////////////////////
    /////  PR2 EX3 TEST 1  //////
    /////////////////////////////
    failed = false;
    start_test(test_section, "PR2_EX3_1", "Initialize the API data");
    // Initialize the data    
    error = api_initData(&data);
    if (error != E_SUCCESS) {
        passed = false;
        failed = true;
        fail_all = true;
    } else if (api_centersCount(data) != 0) {
        passed = false; 
        failed = true;
        fail_all = true;
    }

    end_test(test_section, "PR2_EX3_1", !failed);


    /////////////////////////////
    /////  PR2 EX3 TEST 2  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX3_2", "Load API data");
    if (!fail_all) {
        error = api_loadData(&data, input, true);
        nVaccines = api_vaccineCount(data);
        nLots = api_vaccineLotsCount(data);
        nPeople = api_populationCount(data);

        if (error != E_SUCCESS || nVaccines != 2 || nLots != 5 || nPeople != 2) {
            passed = false;
            failed = true;
            //fail_all = true;
        }
    }
    end_test(test_section, "PR2_EX3_2", !failed);

    /////////////////////////////
    /////  PR2 EX3 TEST 3  //////
    /////////////////////////////
    failed = fail_all;
    start_test(test_section, "PR2_EX3_3", "Obtain number of centers in the API");
    if (!fail_all) {
        nCenters = api_centersCount(data);
        if (nCenters != 2) {
            passed = false;
            failed = true;
            fail_all = true;
        }
    }
    end_test(test_section, "PR2_EX3_3", !failed);

    // Release all data
    api_freeData(&data);

    return passed;
}
