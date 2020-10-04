#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED

#include "DateTime.h"
#include "Paths.h"
#include "Logger.h"
#include "Account.h"

namespace util
{
    void createFolder();
    void backupAccountFile(const std::string& account_num);
    void backupTransactionFile(const std::string& account_num);
    void deleteFile(std::string file_to_delete);
    void createLimitFiles(std::string account_num);
    void setNamesStraight(std::string& str);
    int countHoursPassed(CustomDateTime& dt);
    std::string inputPin(Account *a, void (Account::*returnFuntion) ());
    std::string inputAccountNumber(Account *a, void (Account::*returnFunction) ());
    void pressEnter();
}


#endif // UTILITY_H_INCLUDED
