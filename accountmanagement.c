#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "accountmanagement.h"
#include "utilities.h"
#include "fileoperations.h"
#include "colors.h"

void querySearch(Account accounts[], int count)
{
    char accNum[20];
    int found = 0;
    while (1)
    {
        printf(CYAN "\nEnter Account Number (0 to cancel): " RESET);
        scanf("%s", accNum);
        if (strcmp(accNum, "0") == 0)
            return;
        if (!isNumeric(accNum))
            printf(RED "Warning: Account number must contain only digits.\n" RESET);
        else
            break;
    }

    for (int i = 0; i < count; i++)
    {
        if (strcmp(accounts[i].accountNumber, accNum) == 0)
        {
            printf(BLUE BOLD "\n========================================\n" RESET);
            printf(YELLOW "Account Number : " WHITE "%s\n", accounts[i].accountNumber);
            printf(YELLOW "Name           : " WHITE "%s\n", accounts[i].name);
            printf(YELLOW "Email          : " WHITE "%s\n", accounts[i].email);
            printf(YELLOW "Balance        : " WHITE "%.2f\n", accounts[i].balance);
            printf(YELLOW "Mobile         : " WHITE "%s\n", accounts[i].mobile);
            printf(YELLOW "Date Opened    : " WHITE "%02d-%d\n",
                   accounts[i].opened.month, accounts[i].opened.year);
            printf(YELLOW "Status         : " WHITE "%s\n",
                   accounts[i].active ? "Active" : "Inactive");
            printf(BLUE BOLD "========================================\n" RESET);
            found = 1;
            break;
        }
    }

    if (!found)
        printf(RED "\nAccount not found.\n" RESET);
}

void advancedSearch(Account accounts[], int count)
{
    char keyword[100];
    int found = 0;

    while (1)
    {
        printf(CYAN "\nEnter name keyword (0 to cancel): " RESET);
        getchar();
        fgets(keyword, sizeof(keyword), stdin);
        keyword[strcspn(keyword, "\n")] = '\0';

        if (strcmp(keyword, "0") == 0) return;
        if (strlen(keyword) == 0)
        {
            printf(RED "Keyword cannot be empty.\n" RESET);
        }
        else
        {
            break;
        }
    }

    for (int i = 0; keyword[i]; i++)
        keyword[i] = tolower(keyword[i]);

    printf(YELLOW BOLD "\nSearch Results:\n" RESET);

    for (int i = 0; i < count; i++)
    {
        char nameCopy[100];
        strcpy(nameCopy, accounts[i].name);

        for (int j = 0; nameCopy[j]; j++)
            nameCopy[j] = tolower(nameCopy[j]);

        if (strstr(nameCopy, keyword))
        {
            printf(BLUE "\n========================================\n" RESET);
            printf("Account Number : %s\n", accounts[i].accountNumber);
            printf("Name           : %s\n", accounts[i].name);
            printf("Email          : %s\n", accounts[i].email);
            printf("Balance        : %.2f\n", accounts[i].balance);
            printf("Mobile         : %s\n", accounts[i].mobile);
            printf("Date Opened    : %02d-%d\n",
                   accounts[i].opened.month, accounts[i].opened.year);
            printf("Status         : %s\n",
                   accounts[i].active ? "Active" : "Inactive");
            printf(BLUE "========================================\n" RESET);
            found = 1;
        }
    }

    if (!found)
        printf(RED "\nNo matching accounts found.\n" RESET);
}

void addAccount(Account acc[], int *count)
{
    Account a;

    if (*count >= MAX)
    {
        printf(RED "Error: Maximum account limit reached!\n" RESET);
        return;
    }

    printf(YELLOW BOLD "\n--- Add New Account ---\n" RESET);

    while (1)
    {
        printf(CYAN "Account Number (0 to cancel): " RESET);
        scanf("%s", a.accountNumber);

        if (strcmp(a.accountNumber, "0") == 0) return;

        if (!isNumeric(a.accountNumber))
        {
            printf(RED "Account number must contain only digits.\n" RESET);
            continue;
        }

        if (strlen(a.accountNumber) != 10)
        {
            printf(RED "Account number must be exactly 10 digits.\n" RESET);
            continue;
        }

        int exists = 0;
        for (int i = 0; i < *count; i++)
        {
            if (strcmp(acc[i].accountNumber, a.accountNumber) == 0)
            {
                exists = 1;
                break;
            }
        }
        if (exists)
        {
            printf(RED "Account number already exists!\n" RESET);
        }
        else
        {
            break;
        }
    }

    printf(CYAN "Name: " RESET);
    scanf(" %[^\n]", a.name);

    do
    {
        printf(CYAN "Email (contain @ and .com): " RESET);
        scanf("%s", a.email);
        if (!isValidEmail(a.email))
            printf(RED "Invalid email format.\n" RESET);
    }
    while (!isValidEmail(a.email));

    do
    {
        printf(CYAN "Mobile (11 digits): " RESET);
        scanf("%s", a.mobile);
        if (!isValidMobile11(a.mobile))
            printf(RED "Mobile must be exactly 11 digits.\n" RESET);
    }
    while (!isValidMobile11(a.mobile));

    char balanceInput[50];

    do
    {
        printf(CYAN "Initial Balance: " RESET);
        scanf("%s", balanceInput);

        if (!isNumeric(balanceInput))
        {
            printf(RED "Balance must contain only digits.\n" RESET);
            continue;
        }

        a.balance = atof(balanceInput);
        if (a.balance < 0)
            printf(RED "Balance cannot be negative.\n" RESET);

    } while (!isNumeric(balanceInput) || a.balance < 0);

    getCurrentDate(&a.opened);
    a.active = 1;

    acc[*count] = a;
    (*count)++;

    if (confirmSave())
    {
        saveAccounts(acc, *count);
        printf(GREEN "Account added and saved successfully.\n" RESET);
    }
    else
    {
        (*count)--;
        printf(RED "Changes discarded. Account not saved.\n" RESET);
    }
}

void deleteAccount(Account acc[], int *count)
{
    char num[20];

    while (1)
    {
        printf(CYAN "\nAccount Number to delete (0 to cancel): " RESET);
        scanf("%s", num);
        if (strcmp(num, "0") == 0) return;

        int found = -1;
        for (int i = 0; i < *count; i++)
        {
            if (strcmp(acc[i].accountNumber, num) == 0)
            {
                found = i;
                break;
            }
        }

        if (found == -1) {
            printf(RED "Account not found.\n" RESET);
            continue;
        }

        if (acc[found].balance != 0)
        {
            printf(RED "Cannot delete account with balance %.2f\n" RESET, acc[found].balance);
            return;
        }

        char file1[50], file2[50];
        sprintf(file1, "%s.txt", num);
        sprintf(file2, "%s_withdraw.txt", num);

        printf(YELLOW "\nConfirm deletion of account %s ?\n" RESET, num);
        printf("1. Save (Confirm)\n");
        printf("2. Cancel\n");
        printf("Choice: ");

        int choice;
        scanf("%d", &choice);

        if (choice == 1)
        {
            for (int j = found; j < *count - 1; j++)
                acc[j] = acc[j + 1];
            (*count)--;

            saveAccounts(acc, *count);

            remove(file1);
            remove(file2);

            printf(GREEN "Account deleted successfully.\n" RESET);
        }
        else
        {
            printf(YELLOW "Deletion cancelled. No changes saved.\n" RESET);
        }

        return;
    }
}

void modifyAccount(Account acc[], int count)
{
    char num[20];

    while (1)
    {
        printf(CYAN "\nAccount Number to modify (0 to cancel): " RESET);
        scanf("%s", num);
        if (strcmp(num, "0") == 0) return;

        int found = -1;
        for (int i = 0; i < count; i++)
        {
            if (strcmp(acc[i].accountNumber, num) == 0)
            {
                found = i;

                char oldName[50], oldEmail[50], oldMobile[20];
                strcpy(oldName,   acc[i].name);
                strcpy(oldEmail,  acc[i].email);
                strcpy(oldMobile, acc[i].mobile);

                char temp[50];

                printf(CYAN "Current Name: %s\nEnter new Name (or 0 to skip): " RESET, acc[i].name);
                scanf(" %[^\n]", temp);
                if (strcmp(temp, "0") != 0)
                    strcpy(acc[i].name, temp);

                while (1)
                {
                    printf(CYAN "Current Email: %s\nEnter new Email (or 0 to skip): " RESET, acc[i].email);
                    scanf("%s", temp);

                    if (strcmp(temp, "0") == 0) break;

                    if (!isValidEmail(temp))
                        printf(RED "Invalid email format(contain @ and .com).\n" RESET);
                    else {
                        strcpy(acc[i].email, temp);
                        break;
                    }
                }

                while (1)
                {
                    printf(CYAN "Current Mobile: %s\nEnter new Mobile (11 digits) (or 0 to skip): " RESET, acc[i].mobile);
                    scanf("%s", temp);

                    if (strcmp(temp, "0") == 0) break;

                    if (!isValidMobile11(temp))
                        printf(RED "Mobile must be exactly 11 digits.\n" RESET);
                    else {
                        strcpy(acc[i].mobile, temp);
                        break;
                    }
                }

                if (confirmSave())
                {
                    saveAccounts(acc, count);
                    printf(GREEN "Account details modified and saved successfully.\n" RESET);
                }
                else
                {
                    strcpy(acc[i].name,   oldName);
                    strcpy(acc[i].email,  oldEmail);
                    strcpy(acc[i].mobile, oldMobile);

                    printf(YELLOW "Changes discarded. Account not modified.\n" RESET);
                }

                return;
            }
        }

        if (found == -1)
            printf(RED "Account not found.\n" RESET);
    }
}


void changeStatus(Account accounts[], int count)
{
    char accNum[20];
    int choice;

    while (1)
    {
        printf(CYAN "\nAccount Number to change status (0 to cancel): " RESET);
        scanf("%s", accNum);
        if (strcmp(accNum, "0") == 0) return;

        int found = 0;
        for (int i = 0; i < count; i++)
        {
            if (strcmp(accounts[i].accountNumber, accNum) == 0)
            {
                found = 1;

                printf(YELLOW "\nCurrent Status: %s\n" RESET,
                       accounts[i].active ? "Active" : "Inactive");

                printf("1. Toggle Status\n2. Cancel\nChoice: ");
                if (scanf("%d", &choice) != 1)
                {
                    while (getchar() != '\n');
                    printf(RED "Invalid input.\n" RESET);
                    return;
                }

                if (choice == 1)
                {
                    int oldStatus = accounts[i].active;

                    accounts[i].active = !accounts[i].active;

                    if (confirmSave())
                    {
                        saveAccounts(accounts, count);
                        printf(GREEN "Status updated and saved successfully.\n" RESET);
                    }
                    else
                    {
                        accounts[i].active = oldStatus;
                        printf(RED "Changes discarded. Status not updated.\n" RESET);
                    }
                }

                return;
            }
        }

        if (!found)
            printf(RED "Account not found.\n" RESET);
    }
}
