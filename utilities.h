#ifndef UTILITIES_H
#define UTILITIES_H

#include "structures.h"

int isValidMobile11(const char *mobile);
int isValidEmail(char email[]);
int isNumeric(char str[]);
void getCurrentDate(Date *d);
int statusToInt(const char *status);
int Accountplace(Account accounts[], int count, const char *accNum);
void printAccount(Account a);
int confirmSave(void);

#endif
