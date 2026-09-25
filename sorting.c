#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "sorting.h"
#include "utilities.h"
#include "colors.h"

int compareDates(Date d1, Date d2) {
    if (d1.year != d2.year)
        return d1.year - d2.year;
    if (d1.month != d2.month)
        return d1.month - d2.month;
    return d1.day - d2.day;
}

void sortByName(Account accounts[], int count) {
    Account temp;

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {

            char name1[100], name2[100];
            strcpy(name1, accounts[j].name);
            strcpy(name2, accounts[j + 1].name);

            for (int k = 0; name1[k] != '\0'; k++)
                name1[k] = tolower(name1[k]);
            for (int k = 0; name2[k] != '\0'; k++)
                name2[k] = tolower(name2[k]);

            if (strcmp(name1, name2) > 0) {
                temp = accounts[j];
                accounts[j] = accounts[j + 1];
                accounts[j + 1] = temp;
            }
        }
    }
}

void sortByBalance(Account accounts[], int count) {
    Account temp;

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (accounts[j].balance < accounts[j + 1].balance) {
                temp = accounts[j];
                accounts[j] = accounts[j + 1];
                accounts[j + 1] = temp;
            }
        }
    }
}

void sortByDate(Account accounts[], int count) {
    Account temp;

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (compareDates(accounts[j].opened, accounts[j + 1].opened) > 0) {
                temp = accounts[j];
                accounts[j] = accounts[j + 1];
                accounts[j + 1] = temp;
            }
        }
    }
}

void sortByStatus(Account accounts[], int count) {
    Account temp;

    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (accounts[j].active < accounts[j + 1].active) {
                temp = accounts[j];
                accounts[j] = accounts[j + 1];
                accounts[j + 1] = temp;
            }
        }
    }
}

void printAllAccounts(Account accounts[], int count) {
    if (count == 0) {
        printf(RED "\nNo accounts found in the system.\n" RESET);
        return;
    }

    printf(BLUE BOLD "\n========================================\n" RESET);
    printf(CYAN BOLD "       PRINT ALL ACCOUNTS\n" RESET);
    printf(BLUE BOLD "========================================\n" RESET);

    int choice;
    Account tempAccounts[MAX];
    for (int i = 0; i < count; i++)
        tempAccounts[i] = accounts[i];

    while (1) {
        printf(YELLOW "Sort by:\n" RESET);
        printf(WHITE "1. Name (A-Z)\n" RESET);
        printf(WHITE "2. Balance (Highest to Lowest)\n" RESET);
        printf(WHITE "3. Date Opened (Oldest to Newest)\n" RESET);
        printf(WHITE "4. Status (Active first)\n" RESET);
        printf(WHITE "5. No sorting (print as is)\n" RESET);
        printf(WHITE "0. Cancel\n" RESET);

        printf(CYAN "Enter your choice (0-5): " RESET);

        if (scanf("%d", &choice) != 1) {
            printf(RED "Invalid input! Please enter a number between 0 and 5.\n" RESET);
            while (getchar() != '\n');
            continue;
        }

        if (choice == 0) {
            printf(YELLOW "Operation cancelled.\n" RESET);
            return;
        }

        if (choice < 1 || choice > 5) {
            printf(RED "Invalid choice! Please try again.\n" RESET);
            continue;
        }

        break;
    }

    switch (choice) {
        case 1:
            sortByName(tempAccounts, count);
            printf(GREEN "\n--- Sorted by Name (A-Z) ---\n" RESET);
            break;
        case 2:
            sortByBalance(tempAccounts, count);
            printf(GREEN "\n--- Sorted by Balance (Highest to Lowest) ---\n" RESET);
            break;
        case 3:
            sortByDate(tempAccounts, count);
            printf(GREEN "\n--- Sorted by Date Opened (Oldest to Newest) ---\n" RESET);
            break;
        case 4:
            sortByStatus(tempAccounts, count);
            printf(GREEN "\n--- Sorted by Status (Active first) ---\n" RESET);
            break;
        case 5:
            printf(YELLOW "\n--- Accounts (No sorting) ---\n" RESET);
            break;
    }

    printf(CYAN "\nTotal Accounts: %d\n" RESET, count);

    for (int i = 0; i < count; i++) {
        printf(BLUE "\n========================================\n" RESET);
        printf(YELLOW "Account Number : " WHITE "%s\n", tempAccounts[i].accountNumber);
        printf(YELLOW "Name           : " WHITE "%s\n", tempAccounts[i].name);
        printf(YELLOW "Email          : " WHITE "%s\n", tempAccounts[i].email);
        printf(YELLOW "Balance        : " WHITE "%.2f\n", tempAccounts[i].balance);
        printf(YELLOW "Mobile         : " WHITE "%s\n", tempAccounts[i].mobile);
        printf(YELLOW "Date Opened    : " WHITE "%02d-%d\n",
               tempAccounts[i].opened.month, tempAccounts[i].opened.year);
        printf(YELLOW "Status         : " WHITE "%s\n",
               tempAccounts[i].active ? "Active" : "Inactive");
        printf(BLUE "========================================\n" RESET);
    }

    printf(GREEN "\nEnd of list. Total: %d account(s)\n" RESET, count);
}
