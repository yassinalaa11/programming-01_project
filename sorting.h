#ifndef SORTING_H
#define SORTING_H

#include "structures.h"

void sortByName(Account accounts[], int count);
void sortByBalance(Account accounts[], int count);
void sortByDate(Account accounts[], int count);
void sortByStatus(Account accounts[], int count);
void printAllAccounts(Account accounts[], int count);

#endif
