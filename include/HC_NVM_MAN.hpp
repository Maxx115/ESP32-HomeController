/*
*
*
*
*
*
*/

#ifndef HC_NVM_MAN
#define HC_NVM_MAN

#include "self_arduino.hpp"
#include <nvs_flash.h>
#include <nvs.h>

#define WIFI_NVM "SSID-Password"
#define AP_SSID_NVM "AP_SSID"
#define AP_PASSWORD_NVM "AP_Password"
#define SSID_NVM "SSID"
#define PASSWORD_NVM "Password"

void initNVM(void);

void NVM_write_int(int toStoreInt, nvs_handle nvmHandle);

int NVM_read_int(nvs_handle nvmHandle);

void nvm_write_string(String storageSpace, String storageKey, String toStoreStr);

String nvm_read_string(String storageSpace, String storageKey);

void nvm_write_test(int my_int);

int nvm_read_test(void);

bool nvm_erase_namespace(String storageSpace);

void listKeysInNamespace(const char* namespace_name);

bool nvm_contains(String storageSpace, String storageKey);

#endif /* HC_NVM_MAN */