#ifndef DATA_HANDLER_H_INCLUDED
#define DATA_HANDLER_H_INCLUDED

#include "investor.h"

typedef struct DataHandler DataHandler_s;
typedef DataHandler_s * DataHandler_t;

DataHandler_t DataHandler_new(const char * invesorsDBPath);
void DataHandler_delete(DataHandler_t self);

char * DataHandler_investorsToXmlStr(DataHandler_t self, double minInvestment, double maxInvestment, int minProjectsNumber, int maxProjectsNumber, char * mem_p);

int DataHandler_getInvestorsNum(DataHandler_t self);
Investor_t DataHandler_getInvestor(DataHandler_t self, unsigned int id);
int DataHandler_addInvestor(DataHandler_t self, Investor_t inv);
void DataHandler_deleteInvestor(DataHandler_t self, unsigned int id);
void DataHandler_updateInvestor(DataHandler_t self, Investor_t investor);

void DataHandler_foreachInv(DataHandler_t self, void (fn)(Investor_t inv), double minInvestment, double maxInvestment, int minProjectsNumber, int maxProjectsNumber);

#endif // DATA_HANDLER_H_INCLUDED
