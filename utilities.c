#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>
#include "utilities.h"
#include "colors.h"

int isValidMobile11(const char *mobile) {
    if (strlen(mobile) != 11)
        return 0;

    for (int i = 0; i < 11; i++) {
        if (!isdigit(mobile[i]))
            return 0;
    }
    return 1;
}

int isValidEmail(char email[]) {
    char *at = strchr(email, '@');
    char *dot = strchr(email, '.');

    if (!at || !dot || at > dot)
        return 0;

    if (at == email)
        return 0;

    if (strlen(dot) < 3)
        return 0;

    return 1;
}

int isNumeric(char str[]) {
    if (str[0] == '\0')
        return 0;

    for (int i = 0; str[i]!='\0'; i++) {
        if (!isdigit(str[i]))
            return 0;
    }
    return 1;
}

void getCurrentDate(Date *d) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    d->day   = tm.tm_mday;
    d->month = tm.tm_mon + 1;
    d->year  = tm.tm_year + 1900;
}

int statusToInt(const char *status) {
    if (!status)
        return 0;

    while (isspace(*status))
        status++;

    char tmp[20];
    strncpy(tmp, status, sizeof(tmp) - 1);
    tmp[sizeof(tmp) - 1] = '\0';

    tmp[strcspn(tmp, "\r\n")] = '\0';

    if (strcmp(tmp, "active") == 0)
        return 1;

    return 0;
}


int Accountplace(Account accounts[], int count, const char *accNum) {
    for (int i = 0; i < count; i++) {
        if (strcmp(accounts[i].accountNumber, accNum) == 0)
            return i;
    }
    return -1;
}

int confirmSave(void) {
    int choice;

    while (1) {
        printf(YELLOW "\nConfirm changes:\n" RESET);
        printf("1. Save (Confirm)\n");
        printf("2. Discard\n");
        printf("Choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf(RED "Invalid input!\n" RESET);
            continue;
        }

        if (choice == 1) return 1;
        if (choice == 2) return 0;

        printf(RED "Please choose 1 or 2 only.\n" RESET);
    }
}


void printAccount(Account a) {
    printf(MAGENTA "\n========================================\n" RESET);
    printf(CYAN "Account Number : " RESET "%s\n", a.accountNumber);
    printf(CYAN "Name           : " RESET "%s\n", a.name);
    printf(CYAN "Email          : " RESET "%s\n", a.email);
    printf(CYAN "Balance        : " RESET GREEN "%.2f\n" RESET, a.balance);
    printf(CYAN "Mobile         : " RESET "%s\n", a.mobile);
    printf(CYAN "Date Opened    : " RESET "%02d-%d\n", a.opened.month, a.opened.year);
    printf(CYAN "Status         : " RESET "%s\n", a.active ? GREEN "Active" RESET : RED "Inactive" RESET);
    printf(MAGENTA "========================================\n" RESET);
}
