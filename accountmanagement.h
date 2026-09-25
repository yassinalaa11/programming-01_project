#ifndef ACCOUNTMANAGEMENT_H
#define ACCOUNTMANAGEMENT_H

#include "structures.h"

void querySearch(Account accounts[], int count);
void advancedSearch(Account accounts[], int count);
void addAccount(Account accounts[], int *count);
void deleteAccount(Account accounts[], int *count);
void modifyAccount(Account accounts[], int count);
void changeStatus(Account accounts[], int count);

#endif
