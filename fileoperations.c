#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "fileoperations.h"
#include "utilities.h"
#include "colors.h"

#define MAX_ACCOUNTS 1000
#define MAX_LINE 500

int login(void) {
    char username[50], password[50];
    char fileUser[50], filePass[50];
    FILE *fp;

    while (1) {
        printf(CYAN BOLD "\nEnter Username & Password: " RESET);
        scanf("%s", username);
        scanf("%s", password);

        fp = fopen("users.txt", "r");
        if (!fp) {
            printf(YELLOW "Warning: users.txt not found. Creating default user.\n" RESET);
            fp = fopen("users.txt", "w");
            if (fp) {
                fprintf(fp, "admin admin123\n");
                fclose(fp);
            }
            if (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0) {
                return 1;
            } else {
                printf(RED "Login failed! Invalid username or password.\n" RESET);
                continue;
            }
        }

        int success = 0;
        while (fscanf(fp, "%s %s", fileUser, filePass) != EOF) {
            if (strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0) {
                success = 1;
                break;
            }
        }
        fclose(fp);

        if (success) {
            return 1;
        } else {
            printf(RED "Login failed! Invalid username or password.\n" RESET);
        }
    }
}

void loadAccounts(Account accounts[], int *count) {
    FILE *fp = fopen("accounts.txt", "r");
    char line[512];

    *count = 0;

    if (!fp) {
        printf("accounts.txt not found!\n");
        return;
    }

    while (fgets(line, sizeof(line), fp) && *count < MAX) {

        char *token;
        char status[20];

        token = strtok(line, ",");
        if (!token) continue;
        strcpy(accounts[*count].accountNumber, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(accounts[*count].name, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(accounts[*count].email, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        accounts[*count].balance = atof(token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(accounts[*count].mobile, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        sscanf(token, "%d-%d",
               &accounts[*count].opened.month,
               &accounts[*count].opened.year);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(status, token);

        accounts[*count].active = statusToInt(status);

        (*count)++;
    }

    fclose(fp);
}

void saveAccounts(Account accounts[], int count) {
    FILE *fp = fopen("accounts.txt", "w");

    if (!fp) {
        printf("Error saving accounts!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%s,%s,%s,%.2lf,%s,%02d-%d, %s\n",
                accounts[i].accountNumber,
                accounts[i].name,
                accounts[i].email,
                accounts[i].balance,
                accounts[i].mobile,
                accounts[i].opened.month,
                accounts[i].opened.year,
                accounts[i].active ? "active" : "inactive");
    }

    fclose(fp);
}
