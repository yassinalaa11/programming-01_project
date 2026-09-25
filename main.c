#include <stdio.h>
#include <stdlib.h>
#include "structures.h"
#include "utilities.h"
#include "fileoperations.h"
#include "accountmanagement.h"
#include "transactions.h"
#include "sorting.h"
#include "menu.h"
#include "colors.h"
#include "welcome.h"

int main() {
    Account accounts[MAX];
    int count = 0;
    printMainTitle();
    if (!login())
        return 0;
    loadAccounts(accounts, &count);
    displayMenu(accounts, &count);
    return 0;
}
