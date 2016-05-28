#include <stdlib.h>

#include "db_manager.h"
#include "investor.h"

#include <sqlite3.h>

static struct DB{
    sqlite3 * db;
};

DB_t DB_new(const char * dbFile) {
    DB_t self = malloc(sizeof(DB_s));
    int rc = sqlite3_open(dbFile, &self->db);
    if (SQLITE_ERROR == rc) {
        printf("can't open database\n");
        exit(1);
    }
    return self;
}

void DB_delete(DB_t self) {
    sqlite3_close(self->db);
    free(self);
}

static int _getMaxId(DB_t self){
    sqlite3_stmt * stmt = NULL;
    sqlite3_prepare_v2(self->db, "SELECT MAX(Id) FROM Investor;", -1, &stmt, 0);

    int rc = sqlite3_step(stmt);
    if (SQLITE_ERROR == rc) {
        printf("can't get max id\n");
        sqlite3_finalize(stmt);
        exit(1);
    }

    int maxId = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    return maxId;
}

int DB_insertInvestor(DB_t self, Investor_t investor) {
    int newInvestorId = -1;

    sqlite3_stmt * stmt = NULL;
    sqlite3_prepare_v2(self->db, "INSERT INTO Investor (Name, Surname, Birthdate, Investment, ProjectsNumber)"
                       "VALUES(?, ?, ?, ?, ?);", -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, investor->name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, investor->surname, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, investor->birthdate, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, investor->investment);
    sqlite3_bind_int(stmt, 5, investor->projectsNumber);

    int rc = sqlite3_step(stmt);
    if (SQLITE_ERROR == rc) {
        printf("can't insert investor\n");
        sqlite3_finalize(stmt);
        exit(1);
    }

    newInvestorId = _getMaxId(self);

    sqlite3_finalize(stmt);
    return newInvestorId;
}

Investor_t DB_getInvestorById(DB_t self, int id) {
    sqlite3_stmt * stmt = NULL;
    sqlite3_prepare_v2(self->db, "SELECT * FROM Investor WHERE Id = ?;", -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, id);

    int rc = sqlite3_step(stmt);
    if (SQLITE_ERROR == rc) {
        printf("can't select count\n");
        sqlite3_finalize(stmt);
        exit(1);
    }

    Investor_t investor = Investor_new();
    strcpy(investor->name, sqlite3_column_text(stmt, 1));
    strcpy(investor->surname, sqlite3_column_text(stmt, 2));
    strcpy(investor->birthdate, sqlite3_column_text(stmt, 3));
    investor->investment = sqlite3_column_double(stmt, 4);
    investor->projectsNumber = sqlite3_column_int(stmt, 5);

    sqlite3_finalize(stmt);
    return investor;
}

void DB_updateInvestor(DB_t self, Investor_t investor) {
    sqlite3_stmt * stmt = NULL;
    sqlite3_prepare_v2(self->db, "UPDATE Investor SET Name = ?, Surname = ?, Birthdate = ?, Investment = ?, ProjectsNumber = ?"
                       "WHERE Id = ?;", -1, &stmt, 0);

    sqlite3_bind_text(stmt, 1, investor->name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, investor->surname, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, investor->birthdate, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, investor->investment);
    sqlite3_bind_int(stmt, 5, investor->projectsNumber);
    sqlite3_bind_int(stmt, 6, investor->id);

    int rc = sqlite3_step(stmt);
    if (SQLITE_ERROR == rc){
        printf("can't update investor\n");
        exit(1);
    }

    sqlite3_finalize(stmt);
}

void DB_deleteInvestor(DB_t self, int id) {
    sqlite3_stmt * stmt = NULL;
    sqlite3_prepare_v2(self->db, "DELETE FROM Investor WHERE Id = ?;", -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, id);

    int rc = sqlite3_step(stmt);
    if (SQLITE_ERROR == rc) {
        printf("can't delete investor\n");
        sqlite3_finalize(stmt);
        exit(1);
    }

    sqlite3_finalize(stmt);
}

int  DB_countInvestors(DB_t self) {
    sqlite3_stmt * stmt = NULL;
    sqlite3_prepare_v2(self->db, "SELECT COUNT(*) FROM Investor;", -1, &stmt, 0);

    int rc = sqlite3_step(stmt);
    if (SQLITE_ERROR == rc) {
        printf("can't select count\n");
        sqlite3_finalize(stmt);
        exit(1);
    }

    int count = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);
    return count;
}

static void _fillInvestor(sqlite3_stmt * stmt, Investor_t investor) {
    int id = sqlite3_column_int(stmt, 0);
    const unsigned char * name = sqlite3_column_text(stmt, 1);
    const unsigned char * surname = sqlite3_column_text(stmt, 2);
    const unsigned char * birthdate = sqlite3_column_text(stmt, 3);
    double investment = sqlite3_column_double(stmt, 4);
    int projectsNumber = sqlite3_column_int(stmt, 5);

    investor->id = id;
    strcpy(investor->name, name);
    strcpy(investor->surname, surname);
    strcpy(investor->birthdate, birthdate);
    investor->investment = investment;
    investor->projectsNumber = projectsNumber;
}

int DB_getAllInvestors(DB_t self, List_t list) {
    sqlite3_stmt * stmt = NULL;
    const char * sqlQuery = "SELECT * FROM Investor;";
    sqlite3_prepare_v2(self->db, sqlQuery, -1, &stmt, 0);
    int count = 0;
    while (1) {
        int rc = sqlite3_step(stmt);
        if (SQLITE_ERROR == rc) {
            printf("can't select investors\n");
            sqlite3_finalize(stmt);
            exit(1);
        } else if (SQLITE_DONE == rc) {
            break;
        } else if(SQLITE_ROW == rc){
            Investor_t investor = Investor_new();
            _fillInvestor(stmt, investor);
            List_append(list, investor);
            count++;
        }
    }
    sqlite3_finalize(stmt);
    return count;
}


int DB_getInvestorsTask(DB_t self, double maxInvestment, int maxProjectsNumber, List_t list) {
    sqlite3_stmt * stmt = NULL;
    const char * sqlQuery = "SELECT * FROM Investor WHERE Investment < ? OR ProjectsNumber < ?;";
    sqlite3_prepare_v2(self->db, sqlQuery, -1, &stmt, 0);
    sqlite3_bind_double(stmt, 1, maxInvestment);
    sqlite3_bind_int(stmt, 2, maxProjectsNumber);
    int count = 0;
    while (1) {
        int rc = sqlite3_step(stmt);
        if (SQLITE_ERROR == rc) {
            printf("can't select investors\n");
            sqlite3_finalize(stmt);
            exit(1);
        } else if (SQLITE_DONE == rc) {
            break;
        } else if(SQLITE_ROW == rc){
            Investor_t investor = Investor_new();
            _fillInvestor(stmt, investor);
            List_append(list, investor);
            count++;
        }
    }
    sqlite3_finalize(stmt);
    return count;
}
