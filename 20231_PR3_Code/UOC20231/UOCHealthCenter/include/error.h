#ifndef __UOCHEALTHCENTER_ERROR__H
#define __UOCHEALTHCENTER_ERROR__H

// Define error codes
enum _tApiError
{
    E_SUCCESS = 0, // No error
    E_NOT_IMPLEMENTED = -1, // Called method is not implemented
    E_FILE_NOT_FOUND = -2, // File not found
    E_PERSON_NOT_FOUND = -3, // Person not found
    E_INVALID_ENTRY_TYPE = -4, // Invalid entry type
    E_INVALID_ENTRY_FORMAT = -5, // Invalid entry format
    E_DUPLICATED_PERSON = -6, // Duplicated person
    E_MEMORY_ERROR = -7, // Memory error
    E_VACCINE_NOT_FOUND = -8, // Vaccine not found
    E_HEALTH_CENTER_NOT_FOUND = -9, // Health Center not found
    E_INVALID_HEALTH_CENTER_CODE = -10, // Health Center code is not valid
    E_LOT_NOT_FOUND = -11, // Vaccine lot not found
	E_NO_VACCINES = -12, // No vaccines to allocate appointments.
};

// Define an error type
typedef enum _tApiError tApiError;

#endif // __UOCHEALTHCENTER_ERRORS__H