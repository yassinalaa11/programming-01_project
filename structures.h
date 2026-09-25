#ifndef STRUCTURES_H
#define STRUCTURES_H

#define MAX 100
#define DAILY_LIMIT 50000
#define MAX_TRANS_AMOUNT 10000
#define MAX_TRANS 5
#define LINE_LEN 100

typedef struct {
    int day;
    int month;
    int year;
} Date;

typedef struct {
    char accountNumber[20];
    char name[100];
    char email[100];
    double balance;
    char mobile[20];
    Date opened;
    int active;
} Account;

#endif
