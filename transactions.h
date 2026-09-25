#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include "structures.h"

void logTransaction(const char *accNum, const char *type, double amount);
void deposit(Account accounts[], int count, const char *accNum);
void withdraw(Account accounts[], int count);
void transfer(Account accounts[], int count,const char *senderAcc,const char *receiverAcc);
double getDailyTotal(const char *accNum);
double getTodayWithdrawTotal(const char *accNum);
void logDailyWithdraw(const char *accNum, double amount);
void showReport(const char *accNum);

#endif


