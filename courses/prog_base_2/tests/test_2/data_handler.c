#include <libxml/parser.h>
#include <libxml/tree.h>
#include <sqlite3.h>

#include <stdlib.h>

#include "data_handler.h"

#include "list.h"

struct DataHandler{
    sqlite3 * db;
};

static int _getMaxIdContacts(DataHandler_t self){
    sqlite3_stmt * stmt = NULL;
    sqlite3_prepare_v2(self->db, "SELECT MAX(Id) FROM contacts;", -1, &stmt, 0);

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

static int _getMaxIdInvestors(DataHandler_t self){
    sqlite3_stmt * stmt = NULL;
    sqlite3_prepare_v2(self->db, "SELECT MAX(Id) FROM Investors;", -1, &stmt, 0);

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

DataHandler_t DataHandler_new(const char * invesorsDBPath){
    DataHandler_t self = (DataHandler_t) malloc(sizeof(DataHandler_s));
    int rc = sqlite3_open(invesorsDBPath, &self->db);
    if (SQLITE_ERROR == rc) {
        printf("can't open database\n");
        exit(1);
    }
    return self;
}

void DataHandler_delete(DataHandler_t self){
    sqlite3_close(self->db);
    free(self);
}

char * DataHandler_investorsToXmlStr(DataHandler_t self, double minInvestment, double maxInvestment, int minProjectsNumber, int maxProjectsNumber, char * mem_p){
    xmlDoc * doc = NULL;
	xmlNode * root_element = NULL;
	xmlChar * xmlStr = NULL;

	doc = xmlNewDoc("1.0");
	root_element = xmlNewNode(NULL, "investors");
	xmlDocSetRootElement(doc, root_element);

    void Inv_toXmlNode(Investor_t inv){
        Investor_toXmlNode(inv, doc, root_element);
    }

    DataHandler_foreachInv(self, Inv_toXmlNode, minInvestment, maxInvestment, minProjectsNumber, maxProjectsNumber);

	xmlDocDumpMemory(doc, &xmlStr, NULL);

	strcpy(mem_p, xmlStr);

    free(xmlStr);
    xmlFreeDoc(doc);
    return mem_p;
}

int DataHandler_getInvestorsNum(DataHandler_t self){
    sqlite3_stmt * stmt = NULL;
    sqlite3_prepare_v2(self->db, "SELECT COUNT(*) FROM Investors;", -1, &stmt, 0);

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

Investor_t DataHandler_getInvestor(DataHandler_t self, unsigned int id){
    sqlite3_stmt * stmt = NULL;
    sqlite3_prepare_v2(self->db, "SELECT * FROM Investors WHERE Id = ?;", -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, id);

    int rc = sqlite3_step(stmt);
    if (SQLITE_ERROR == rc) {
        printf("error\n");
        sqlite3_finalize(stmt);
        exit(1);
    }else if(SQLITE_ROW != rc){
        return NULL;
    }

    Investor_t investor = Investor_new();
    investor->id = sqlite3_column_int(stmt, 0);
    strcpy(investor->name, sqlite3_column_text(stmt, 1));
    strcpy(investor->surname, sqlite3_column_text(stmt, 2));
    strcpy(investor->birthdate, sqlite3_column_text(stmt, 3));
    investor->investment = sqlite3_column_double(stmt, 4);
    investor->projectsNumber = sqlite3_column_int(stmt, 5);

    int contactsId = sqlite3_column_int(stmt, 6);

    sqlite3_finalize(stmt);
    stmt = NULL;
    sqlite3_prepare_v2(self->db, "SELECT * FROM contacts WHERE Id = ?;", -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, contactsId);

    rc = sqlite3_step(stmt);
    if (SQLITE_ERROR == rc) {
        printf("error\n");
        sqlite3_finalize(stmt);
        exit(1);
    }else if(SQLITE_ROW != rc){
        return NULL;
    }

    strcpy(investor->contacts.telephoneNumber, sqlite3_column_text(stmt, 1));
    strcpy(investor->contacts.address, sqlite3_column_text(stmt, 2));
    strcpy(investor->contacts.email, sqlite3_column_text(stmt, 3));

    sqlite3_finalize(stmt);
    return investor;
}

int DataHandler_addInvestor(DataHandler_t self, Investor_t investor){
    sqlite3_stmt * stmt = NULL;
    int newInvestorId = -1, newContactsId = -1;

    sqlite3_prepare_v2(self->db, "INSERT INTO Contacts (TelephoneNumber, Address, Email)"
                       "VALUES(?, ?, ?);", -1, &stmt, 0);

    sqlite3_bind_text(stmt, 1, investor->contacts.telephoneNumber, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, investor->contacts.address, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, investor->contacts.email, -1, SQLITE_STATIC);

    int rc = sqlite3_step(stmt);
    if (SQLITE_ERROR == rc) {
        printf("can't insert investor\n");
        sqlite3_finalize(stmt);
        exit(1);
    }

    newContactsId = _getMaxIdContacts(self);

    sqlite3_prepare_v2(self->db, "INSERT INTO Investors (Name, Surname, Birthdate, Investment, ProjectsNumber, ContactsId)"
                       "VALUES(?, ?, ?, ?, ?, ?);", -1, &stmt, 0);
    sqlite3_bind_text(stmt, 1, investor->name, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, investor->surname, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, investor->birthdate, -1, SQLITE_STATIC);
    sqlite3_bind_double(stmt, 4, investor->investment);
    sqlite3_bind_int(stmt, 5, investor->projectsNumber);
    sqlite3_bind_int(stmt, 6, newContactsId);

    rc = sqlite3_step(stmt);
    if (SQLITE_ERROR == rc) {
        printf("can't insert investor\n");
        sqlite3_finalize(stmt);
        exit(1);
    }

    newInvestorId = _getMaxIdInvestors(self);

    sqlite3_finalize(stmt);
    return newInvestorId;
}

void DataHandler_deleteInvestor(DataHandler_t self, unsigned int id){
    sqlite3_stmt * stmt = NULL;

    sqlite3_prepare_v2(self->db, "SELECT ContactsId FROM Investors WHERE Id = ?;", -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, id);

    int rc = sqlite3_step(stmt);
    if (SQLITE_ERROR == rc) {
        printf("error\n");
        sqlite3_finalize(stmt);
        exit(1);
    }else if(SQLITE_ROW != rc){
        return NULL;
    }
    int contactsId = sqlite3_column_int(stmt, 0);

    sqlite3_finalize(stmt);

    sqlite3_prepare_v2(self->db, "DELETE FROM Investors WHERE Id = ?;", -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, id);

    rc = sqlite3_step(stmt);
    if (SQLITE_ERROR == rc) {
        printf("can't delete investor\n");
        sqlite3_finalize(stmt);
        exit(1);
    }
    sqlite3_finalize(stmt);

    sqlite3_prepare_v2(self->db, "DELETE FROM contacts WHERE Id = ?;", -1, &stmt, 0);
    sqlite3_bind_int(stmt, 1, contactsId);

    rc = sqlite3_step(stmt);
    if (SQLITE_ERROR == rc) {
        printf("can't delete investor\n");
        sqlite3_finalize(stmt);
        exit(1);
    }
    sqlite3_finalize(stmt);
}

void DataHandler_foreachInv(DataHandler_t self, void (fn)(Investor_t inv), double minInvestment, double maxInvestment, int minProjectsNumber, int maxProjectsNumber)
{
    sqlite3_stmt * stmt = NULL;
    sqlite3_stmt * stmt1 = NULL;

    sqlite3_prepare_v2(self->db, "SELECT * FROM Investors"
                       " WHERE (Investment > ? AND Investment < ?) OR (ProjectsNumber > ? AND ProjectsNumber < ?);", -1, &stmt, 0);
    sqlite3_bind_double(stmt, 1, minInvestment);
    sqlite3_bind_double(stmt, 2, maxInvestment);
    sqlite3_bind_int(stmt, 3, minProjectsNumber);
    sqlite3_bind_int(stmt, 4, maxProjectsNumber);

    int rc = sqlite3_step(stmt);
    while (SQLITE_ROW == rc) {
        Investor_t investor = Investor_new();
        investor->id = sqlite3_column_int(stmt, 0);
        strcpy(investor->name, sqlite3_column_text(stmt, 1));
        strcpy(investor->surname, sqlite3_column_text(stmt, 2));
        strcpy(investor->birthdate, sqlite3_column_text(stmt, 3));
        investor->investment = sqlite3_column_double(stmt, 4);
        investor->projectsNumber = sqlite3_column_int(stmt, 5);

        sqlite3_prepare_v2(self->db, "SELECT * FROM contacts;", -1, &stmt1, 0);

        rc = sqlite3_step(stmt1);

        strcpy(investor->contacts.telephoneNumber, sqlite3_column_text(stmt1, 1));
        strcpy(investor->contacts.address, sqlite3_column_text(stmt1, 2));
        strcpy(investor->contacts.email, sqlite3_column_text(stmt1, 3));

        fn(investor);

        DataHandler_updateInvestor(self, investor);

        Investor_delete(investor);

        sqlite3_finalize(stmt1);
        rc = sqlite3_step(stmt);
    }

    sqlite3_finalize(stmt);
}

void DataHandler_updateInvestor(DataHandler_t self, Investor_t investor) {
    sqlite3_stmt * stmt = NULL;
    sqlite3_prepare_v2(self->db, "UPDATE Investors SET Name = ?, Surname = ?, Birthdate = ?, Investment = ?, ProjectsNumber = ?"
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
