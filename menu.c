#include <stdio.h>
#include <stdlib.h>
#include "menu.h"
#include "fileoperations.h"
#include "accountmanagement.h"
#include "transactions.h"
#include "sorting.h"
#include "colors.h"

void displayMenu(Account accounts[], int *count) {
    int choice;

    do {
        printf(BLUE "========================================\n" RESET);
        printf(YELLOW BOLD " ACCOUNT OPERATIONS\n" RESET);
        printf(WHITE " 1.  Query Account\n" RESET);
        printf(WHITE " 2.  Advanced Search\n" RESET);
        printf(WHITE " 3.  Add Account\n" RESET);
        printf(WHITE " 4.  Delete Account\n" RESET);
        printf(WHITE " 5.  Delete Multiple Accounts\n" RESET);
        printf(WHITE " 6.  Modify Account\n" RESET);
        printf(WHITE " 7.  Change Account Status\n" RESET);

        printf(BLUE "========================================\n" RESET);

        printf(YELLOW BOLD " TRANSACTION OPERATIONS\n" RESET);
        printf(WHITE " 8.  Deposit\n" RESET);
        printf(WHITE " 9.  Withdraw\n" RESET);
        printf(WHITE " 10.  Transfer\n" RESET);

        printf(BLUE "========================================\n" RESET);

        printf(YELLOW BOLD " REPORTS & DISPLAY\n" RESET);
        printf(WHITE " 11. Transaction Report\n" RESET);
        printf(WHITE " 12. Print All Accounts (Sorted)\n" RESET);

        printf(BLUE "========================================\n" RESET);
        printf(RED BOLD " 0.  Exit System\n" RESET);
        printf(BLUE "========================================\n" RESET);

        printf(CYAN "Enter your choice (0-12): " RESET);

        if (scanf("%d", &choice) != 1) {
            printf(RED "\nInvalid input! Please enter a number.\n" RESET);
            while (getchar() != '\n');
            printf(GREEN "Press Enter to continue..." RESET);
            getchar();
            continue;
        }

        switch (choice) {
            case 1:
                querySearch(accounts, *count);
                break;

            case 2:
                advancedSearch(accounts, *count);
                break;

            case 3:
                addAccount(accounts, count);
                break;

            case 4:
                deleteAccount(accounts, count);
                break;

            case 5:
                deleteMultiple(accounts, count);
                break;

            case 6:
                modifyAccount(accounts, *count);
                break;

            case 7:
                changeStatus(accounts, *count);
                break;

            case 8: {
                char acc[20];
                printf(CYAN "\nAccount number: " RESET);
                scanf("%s", acc);
                deposit(accounts, *count, acc);
                break;
            }

            case 9: {
                withdraw(accounts, *count);
                break;
            }

            case 10: {
                char s[20], r[20];
                printf(CYAN "\nSender account: " RESET);
                scanf("%s", s);
                printf(CYAN "Receiver account: " RESET);
                scanf("%s", r);
                transfer(accounts, *count, s, r);
                break;
            }

            case 11: {
                char acc[20];
                printf(CYAN "\nAccount number: " RESET);
                scanf("%s", acc);
                showReport(acc);
                break;
            }

            case 12:
                printAllAccounts(accounts, *count);
                break;

            case 0:
                exitProgram(accounts, *count);
                break;

            default:
                printf(RED "\nInvalid choice! Please try again.\n" RESET);
        }
    } while (1);
}

void exitProgram(Account accounts[], int count) {
    int choice;

    printf(BLUE BOLD "\n========================================\n" RESET);
    printf(RED BOLD "Are you sure you want to exit?\n" RESET);
    printf(GREEN "1. Yes, Exit\n" RESET);
    printf(YELLOW "2. No, Return to Menu\n" RESET);
    printf(BLUE "========================================\n" RESET);
    printf(CYAN "Enter choice: " RESET);

    scanf("%d", &choice);

    if (choice == 1) {
        saveAccounts(accounts, count);
        printf(GREEN BOLD "\n========================================\n" RESET);
        printf(GREEN "All data saved successfully.\n" RESET);
        printf(GREEN "Thank you for using Bank Management System.\n" RESET);
        printf(GREEN "Goodbye!\n" RESET);
        printf(GREEN BOLD "========================================\n" RESET);
        exit(0);
    }
    else if (choice == 2) {
        printf(YELLOW "\nExit cancelled. Returning to main menu...\n" RESET);
    }
    else {
        printf(RED "\nInvalid choice. Returning to main menu...\n" RESET);
    }
}
