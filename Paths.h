#ifndef PATHS_H_INCLUDED
#define PATHS_H_INCLUDED

#include <string>

class Paths
{
   Paths() = delete;
public:
    static const std::string BASE_PATH;

    static const std::string USER_PATH;
    static const std::string ADMIN_PATH;

    static const std::string USER_INFO;
    static const std::string USER_TRANSACTION;

    static const std::string ADMIN_BACKUP;
    static const std::string ADMIN_LOCKED_ACCOUNTS;
    static const std::string ADMIN_LOG;
    static const std::string ADMIN_ERROR_LOG;

    static const std::string ADMIN_WITHDRAWAL_DATE;
    static const std::string ADMIN_WITHDRAWAL_VAL;
    static const std::string ADMIN_TRANSFER_DATE;
    static const std::string ADMIN_TRANSFER_VAL;
};


#endif // PATHS_H_INCLUDED
