#include <stdio.h>
#include <stdlib.h>

#include "db_manager.h"
#include "investor.h"
#include "list.h"

void printAllInvestors(DB_t db){
    List_t investorsList = List_new();

    int investorsNum = DB_getAllInvestors(db, investorsList);
    printf("Investors number: %i\n\n", investorsNum);

     for(int i = 0; i < investorsNum; i++){
        Investor_print(List_get(investorsList, i, NULL));
        puts("\n");
    }

    List_deepDelete(investorsList, Investor_delete);
}

void printFilteredInvestors(DB_t db, double filterInvestment, int filterPrjNum){
    List_t filteredInvestorsList = List_new();

    int filteredInvestorsNum = DB_getInvestorsTask(db, filterInvestment, filterPrjNum, filteredInvestorsList);
    printf("Investors with investment < %f or projects number < %i: %i\n\n", filterInvestment, filterPrjNum, filteredInvestorsNum);

    for(int i = 0; i < filteredInvestorsNum; i++){
        Investor_print(List_get(filteredInvestorsList, i, NULL));
        puts("\n");
    }

    List_deepDelete(filteredInvestorsList, Investor_delete);
}

int main(){
    const char * dbFile = "data/investors.db";
    DB_t db = DB_new(dbFile);

    puts("Initial:");
    printAllInvestors(db);
    puts("_______");

    Investor_s investor;
    strcpy(investor.name, "Mykyta");
    strcpy(investor.surname, "Sylchenko");
    strcpy(investor.birthdate, "1998-04-07");
    investor.investment = 3.1;
    investor.projectsNumber = 4;

    investor.id = DB_insertInvestor(db, &investor);

    puts("After inserting one:");
    printAllInvestors(db);
    puts("_______");

    strcpy(investor.surname, "Skywalker");
    DB_updateInvestor(db, &investor);

    puts("After updating one:");
    printAllInvestors(db);
    puts("_______");

    DB_deleteInvestor(db, investor.id);

    puts("After deleting one:");
    printAllInvestors(db);
    puts("_______");

    puts("Filtered:");
    printFilteredInvestors(db, 2.12, 3);

    DB_delete(db);
    return 0;
}
