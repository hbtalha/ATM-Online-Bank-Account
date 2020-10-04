#include <iostream>

#include "Paths.h"

using namespace std;

const string Paths::BASE_PATH = "C:/ProgramData/BankAcount";

const string Paths::USER_PATH = BASE_PATH + "/User";
const string Paths::ADMIN_PATH = BASE_PATH + "/Administrator";

const string Paths::USER_INFO = USER_PATH + "/Info/";
const string Paths::USER_TRANSACTION = USER_PATH + "/Transactions/";


const string Paths::ADMIN_BACKUP = ADMIN_PATH + "/Backup/";
const string Paths::ADMIN_LOCKED_ACCOUNTS = ADMIN_PATH + "/LockedAccounts/";
const string Paths::ADMIN_LOG = ADMIN_PATH + "/Log/";
const string Paths::ADMIN_ERROR_LOG = ADMIN_PATH + "/Error_Log/";

const string Paths::ADMIN_TRANSFER_DATE = ADMIN_PATH + "/Limits/Transfer/Dates/";
const string Paths::ADMIN_TRANSFER_VAL = ADMIN_PATH + "/Limits/Transfer/Values/";
const string Paths::ADMIN_WITHDRAWAL_DATE = ADMIN_PATH + "/Limits/Withdrawal/Dates/";
const string Paths::ADMIN_WITHDRAWAL_VAL = ADMIN_PATH + "/Limits/Withdrawal/Values/";
