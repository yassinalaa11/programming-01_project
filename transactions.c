#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "transactions.h"
#include "utilities.h"
#include "fileoperations.h"
#include "colors.h"

void logTransaction(const char *accNum, const char *type, double amount)
{
    char filename[30], lines[MAX_TRANS][LINE_LEN];
    int count = 0;
    sprintf(filename, "%s.txt", accNum);

    FILE *fp = fopen(filename, "r");
    if (fp)
    {
        while (fgets(lines[count], LINE_LEN, fp) && count < MAX_TRANS) count++;
        fclose(fp);
    }

    Date d;
    getCurrentDate(&d);
    char newLine[LINE_LEN];
    sprintf(newLine, "%s %.2f %02d-%02d-%d\n", type, amount, d.day, d.month, d.year);

    if (count == MAX_TRANS)
    {
        for (int i = 1; i < MAX_TRANS; i++) strcpy(lines[i - 1], lines[i]);
        count = MAX_TRANS - 1;
    }

    strcpy(lines[count], newLine);
    count++;

    fp = fopen(filename, "w");
    if (!fp)
    {
        printf(RED "Error: Cannot open transaction file.\n" RESET);
        return;
    }
    for (int i = 0; i < count; i++) fputs(lines[i], fp);
    fclose(fp);
}

double getDailyTotal(const char *accNum)
{
    FILE *fp;
    char filename[30], line[LINE_LEN];
    double total = 0;
    Date today;
    getCurrentDate(&today);
    sprintf(filename, "%s.txt", accNum);

    fp = fopen(filename, "r");
    if (!fp) return 0;

    while (fgets(line, sizeof(line), fp))
    {
        char type[20], date[20];
        double amount;

        if (sscanf(line, "%[^|]|%lf|%s", type, &amount, date) == 3)
        {
            int d, m, y;
            if (sscanf(date, "%d-%d-%d", &d, &m, &y) == 3)
            {
                if (d == today.day && m == today.month && y == today.year)
                {
                    if (strcmp(type, "Withdraw") == 0 || strcmp(type, "Transfer Out") == 0)
                    {
                        total += amount;
                    }
                }
            }
        }
    }
    fclose(fp);
    return total;
}

void deposit(Account accounts[], int count, const char *accNum)
{
    int place = Accountplace(accounts,count,accNum);
    double amount;
    if (place==-1)
    {
        printf(RED "Account not found.\n" RESET);
        return;
    }
    if (!accounts[place].active)
    {
        printf(YELLOW "Account inactive.\n" RESET);
        return;
    }
    printf(CYAN "Enter deposit amount: " RESET);
    if (scanf("%lf",&amount)!=1)
    {
        while(getchar()!='\n');
        printf(RED "Invalid input.\n" RESET);
        return;
    }
    if (amount<=0 || amount>MAX_TRANS_AMOUNT)
    {
        printf(RED "Deposit limit %.0f\n" RESET,(double)MAX_TRANS_AMOUNT);
        return;
    }
    double oldBalance = accounts[place].balance;
    accounts[place].balance+=amount;
    if (confirmSave())
    {
        logTransaction(accNum,"Deposit",amount);
        saveAccounts(accounts,count);
        printf(GREEN "Deposit successful. New balance: %.2f\n" RESET,accounts[place].balance);
    }
    else
    {
        accounts[place].balance=oldBalance;
        printf(RED "Deposit cancelled. Balance unchanged.\n" RESET);
    }
}

void logDailyWithdraw(const char *accNum, double amount) {
    char filename[40];
    sprintf(filename, "%s_withdraw.txt", accNum);

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    FILE *fp = fopen(filename, "a");
    if(!fp){
        printf("Error opening daily withdraw log.\n");
        return;
    }

    fprintf(fp, "%d-%02d-%02d %.2lf\n",
        t->tm_year + 1900,
        t->tm_mon + 1,
        t->tm_mday,
        amount
    );

    fclose(fp);
}


double getTodayWithdrawTotal(const char *accNum) {
    char filename[40];
    sprintf(filename, "%s_withdraw.txt", accNum);

    FILE *fp = fopen(filename, "r");
    if (!fp) return 0;

    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    int yearToday = t->tm_year + 1900;
    int monthToday = t->tm_mon + 1;
    int dayToday = t->tm_mday;

    double amount, total = 0;
    int y, m, d;

    while (fscanf(fp, "%d-%d-%d %lf", &y, &m, &d, &amount) == 4) {
        if (y == yearToday && m == monthToday && d == dayToday) {
            total += amount;
        }
    }

    fclose(fp);
    return total;
}


void withdraw(Account accounts[], int count) {
    char accNum[20];
    double amount;

    printf(CYAN "Enter account number: " RESET);
    scanf("%s", accNum);

    int index = Accountplace(accounts, count, accNum);
    if (index == -1) {
        printf(RED "Account not found.\n" RESET);
        return;
    }

    if (accounts[index].active == 0) {
        printf(YELLOW "Account is inactive. Withdrawal not allowed.\n" RESET);
        return;
    }

    printf(CYAN "Enter withdrawal amount: "RESET);
    scanf("%lf", &amount);

    if (amount > 10000) {
        printf(RED "Transaction limit exceeded. Maximum is 10,000 per transaction.\n" RESET);
        return;
    }

    if (amount > accounts[index].balance) {
        printf(YELLOW "Insufficient balance.\n" RESET);
        return;
    }

    double todayTotal = getTodayWithdrawTotal(accNum);

    if (todayTotal + amount > 50000) {
        printf(RED "Daily limit exceeded. Maximum daily limit is 50,000.\n" RESET);
        return;
    }

    printf(YELLOW "\nConfirm withdrawal of %.2lf from account %s?\n" RESET, amount, accNum);
    printf("1. Save (Confirm)\n");
    printf("2. Cancel\n");
    printf("Choice: ");

    int choice;
    if (scanf("%d", &choice) != 1 || choice != 1) {
        while (getchar() != '\n');
        printf(YELLOW "Withdrawal cancelled.\n" RESET);
        return;
    }

    accounts[index].balance -= amount;

    logDailyWithdraw(accNum, amount);

    logTransaction(accNum, "withdraw", amount);

    saveAccounts(accounts, count);

    printf(GREEN "Withdrawal successful. New balance: %.2lf\n" RESET, accounts[index].balance);
}

void transfer(Account accounts[], int count, const char *senderAcc, const char *receiverAcc)
{
    int s=Accountplace(accounts,count,senderAcc);
    int r=Accountplace(accounts,count,receiverAcc);
    double amount;
    if (s==-1 || r==-1)
    {
        printf(RED "Account not found.\n" RESET);
        return;
    }
    if (!accounts[s].active || !accounts[r].active)
    {
        printf(YELLOW "One account inactive.\n" RESET);
        return;
    }
    if (strcmp(senderAcc,receiverAcc)==0)
    {
        printf(RED "Cannot transfer to same account.\n" RESET);
        return;
    }
    printf(CYAN "Enter transfer amount: " RESET);
    if (scanf("%lf",&amount)!=1)
    {
        while(getchar()!='\n');
        printf(RED "Invalid input.\n" RESET);
        return;
    }
    if (amount<=0)
    {
        printf(RED "Invalid amount.\n" RESET);
        return;
    }
    if (amount>accounts[s].balance)
    {
        printf(RED "Insufficient balance.\n" RESET);
        return;
    }
    double oldS=accounts[s].balance, oldR=accounts[r].balance;
    accounts[s].balance-=amount;
    accounts[r].balance+=amount;
    if (confirmSave())
    {
        logTransaction(senderAcc,"Transfer Out",amount);
        logTransaction(receiverAcc,"Transfer In",amount);
        saveAccounts(accounts,count);
        printf(GREEN "Transfer successful.\nSender balance: %.2f\nReceiver balance: %.2f\n" RESET,accounts[s].balance,accounts[r].balance);
    }
    else
    {
        accounts[s].balance=oldS;
        accounts[r].balance=oldR;
        printf(RED "Transfer cancelled. Balances unchanged.\n" RESET);
    }
}

void showReport(const char *accNum)
{
    char filename[30], line[LINE_LEN];
    sprintf(filename,"%s.txt",accNum);
    FILE *fp=fopen(filename,"r");
    if (!fp)
    {
        printf(YELLOW "No transactions found.\n" RESET);
        return;
    }
    printf("\n========================================\n");
    printf("   TRANSACTION REPORT\n");
    printf("========================================\n");
    printf("Account Number: %s\n\n",accNum);
    printf("Type            Amount      Date\n");
    printf("----------------------------------------\n");
    while(fgets(line,sizeof(line),fp)) printf("%s",line);
    printf("========================================\n");
    fclose(fp);
}

