#ifndef DATA_HANDLER_H_INCLUDED
#define DATA_HANDLER_H_INCLUDED

#include "investor.h"

typedef struct DataHandler DataHandler_s;
typedef DataHandler_s * DataHandler_t;

DataHandler_t DataHandler_new();
void DataHandler_delete(DataHandler_t self);

void DataHandler_deserializeInvestors(DataHandler_t self); // adds to serialized to encapsulated ones
void DataHandler_serializeInvestors(DataHandler_t self);
char * DataHandler_investorsToXmlStr(DataHandler_t self, char * mem_p);

int DataHandler_getInvestorsNum(DataHandler_t self);
Investor_t DataHandler_getInvestor(DataHandler_t self, unsigned int index);
void DataHandler_addInvestor(DataHandler_t self, Investor_t inv);
void DataHandler_deleteInvestor(DataHandler_t self, unsigned int index);
void DataHandler_updateInvestor(DataHandler_t self, unsigned int index, const char * key, const char * value);

#endif // DATA_HANDLER_H_INCLUDED
