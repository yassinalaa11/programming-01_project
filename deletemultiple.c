#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "deletemultiple.h"
#include "fileoperations.h"
#include "utilities.h"
#include "colors.h"

int daysDifference(Date opened) {
    time_t now = time(NULL);
    struct tm *current = localtime(&now);

    int currentYear  = current->tm_year + 1900;
    int currentMonth = current->tm_mon + 1;
    int currentDay   = current->tm_mday;

    int diffYears   = currentYear - opened.year;
    int diffMonths  = currentMonth - opened.month;
    int diffDays    = currentDay - opened.day;

    return diffYears * 365 + diffMonths * 30 + diffDays;
}

void deleteMultiple(Account accounts[], int *count) {
    int choice;

    printf(CYAN "\n==============================================\n" RESET);
    printf(YELLOW "       DELETE MULTIPLE ACCOUNTS\n" RESET);
    printf(CYAN "==============================================\n" RESET);
    printf("1) Delete By Date (YYYY-MM)\n");
    printf("2) Delete Inactive Accounts (>90 days & balance = 0)\n");
    printf("0) Back\n");
    printf("\nEnter your choice: " CYAN);
    scanf("%d", &choice);
    printf(RESET);

    switch (choice) {
        case 1: deleteByDate(accounts, count); break;
        case 2: deleteInactive(accounts, count); break;
        case 0: return;
        default: printf(RED "Invalid choice.\n" RESET);
    }
}

void deleteByDate(Account accounts[], int *count) {
    int year, month, deleted = 0;

    printf("\nEnter date (YYYY-MM): " CYAN);
    scanf("%d-%d", &year, &month);
    printf(RESET);

    Account *backup = malloc((*count) * sizeof(Account));
    if (!backup) {
        printf(RED "Memory allocation failed.\n" RESET);
        return;
    }

    for(int i = 0; i < *count; i++)
        backup[i] = accounts[i];

    int oldCount = *count;

    for (int i = 0; i < *count; i++) {
        if (accounts[i].opened.year == year && accounts[i].opened.month == month) {

            for (int j = i; j < *count - 1; j++)
                accounts[j] = accounts[j + 1];

            (*count)--; deleted++; i--;
        }
    }

    if (deleted == 0) {
        printf(YELLOW "No accounts created on this date.\n" RESET);
        free(backup);
        return;
    }

    printf(GREEN "%d account(s) found.\n" RESET, deleted);

    if (confirmSave()) {
        saveAccounts(accounts, *count);

        for (int i = 0; i < oldCount; i++) {
            if (backup[i].opened.year == year && backup[i].opened.month == month) {
                char file1[50], file2[50];
                sprintf(file1, "%s.txt", backup[i].accountNumber);
                sprintf(file2, "%s_withdraw.txt", backup[i].accountNumber);
                remove(file1);
                remove(file2);
            }
        }

        printf(GREEN "Changes saved.\n" RESET);
    } else {
        for(int i = 0; i < oldCount; i++)
            accounts[i] = backup[i];
        *count = oldCount;
        printf(YELLOW "Changes discarded.\n" RESET);
    }

    free(backup);
}

void deleteInactive(Account accounts[], int *count) {
    int deleted = 0;

    Account *backup = malloc((*count) * sizeof(Account));
    if (!backup) {
        printf(RED "Memory allocation failed.\n" RESET);
        return;
    }

    for(int i = 0; i < *count; i++)
        backup[i] = accounts[i];

    int oldCount = *count;

    for (int i = 0; i < *count; i++) {
        if (accounts[i].active == 0 &&
            accounts[i].balance == 0 &&
            daysDifference(accounts[i].opened) > 90) {

            for (int j = i; j < *count - 1; j++)
                accounts[j] = accounts[j + 1];

            (*count)--; deleted++; i--;
        }
    }

    if (deleted == 0) {
        printf(YELLOW "No inactive zero-balance accounts older than 90 days.\n" RESET);
        free(backup);
        return;
    }

    printf(GREEN "%d account(s) found.\n" RESET, deleted);

    if (confirmSave()) {
        saveAccounts(accounts, *count);

        for (int i = 0; i < oldCount; i++) {
            if (backup[i].active == 0 &&
                backup[i].balance == 0 &&
                daysDifference(backup[i].opened) > 90) {

                char file1[50], file2[50];
                sprintf(file1, "%s.txt", backup[i].accountNumber);
                sprintf(file2, "%s_withdraw.txt", backup[i].accountNumber);
                remove(file1);
                remove(file2);
            }
        }

        printf(GREEN "Changes saved.\n" RESET);
    } else {
        for(int i = 0; i < oldCount; i++)
            accounts[i] = backup[i];
        *count = oldCount;
        printf(YELLOW "Changes discarded.\n" RESET);
    }

    free(backup);
}
