#ifndef FILEOPERATIONS_H
#define FILEOPERATIONS_H

#include "structures.h"

int login(void);
void loadAccounts(Account accounts[], int *count);
void saveAccounts(Account accounts[], int count);

#endif
