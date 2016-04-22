#ifndef DB_MANAGER_H_INCLUDED
#define DB_MANAGER_H_INCLUDED

#include "investor.h"
#include "list.h"

typedef struct DB DB_s;
typedef DB_s * DB_t;

DB_t DB_new(const char * dbFile);
void DB_delete(DB_t self);

int DB_insertInvestor();
Investor_t DB_getInvestorById(DB_t self, int id);
void DB_updateInvestor(DB_t self, Investor_t investor);
void DB_deleteInvestor(DB_t self, int id);

int DB_countInvestors(DB_t self);

int DB_getAllInvestors(DB_t self, List_t list);
int DB_getInvestorsTask(DB_t self, double maxInvestment, int maxProjectsNumber, List_t list);

#endif // DB_MANAGER_H_INCLUDED
