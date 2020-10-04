#ifndef LOGGER_H
#define LOGGER_H

#include "DateTime.h"
#include "Paths.h"

class Log
{
    void logTransactions(const std::string& log_account_num, const std::string& transaction_type, const long long int& amount);
    void log(const std::string& account_num, const std::string& account_name, const std::string& log_sg, bool note );
    void logInsideAccount(const std::string& in_account);
    void logError(const std::string& account_num, const std::string& msg );

    DateTime dtime;
    Log(){}

    friend class Account;
};

#endif // LOGGER_H
