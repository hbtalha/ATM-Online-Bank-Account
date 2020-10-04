#include "Logger.h"


#include "Utility.h"

#include <iostream>
#include <fstream>

using namespace std;

// a function that creates a file that contains log information
void Log::log(const std::string& account_num, const std::string& account_name, const std::string& log_msg, bool note ) // the bool note is a bool that  logs wether the account was locked because of too many failed password attempts
{
    string sfile_path = Paths::ADMIN_LOG + dtime.current_date();

    ofstream log(sfile_path, ios::app);

    log << "\n\n-----------------------------------------------------------------------------------\n\n";

    log << "Account Number: " << account_num << " | " << "Name: " << account_name << endl;
    log << log_msg<< " at " << dtime.current_time();
    //log << "Sign In " << " at " << __DATE__;

    // if the note is true it logs in the file that the account was blocked for 24 hours and informing the time of the occured
    if (note == true)
    {
        log << endl << "Account Locked For 24 Hours Started At " << dtime.current_time();
    }

    log.close();
}

// a function that creates a file  that logs every activity inside an account
void Log::logInsideAccount(const string& inAccount_activity)  // the string will take the type of activity made
{
    string sfile_path = Paths::ADMIN_LOG + dtime.current_date();

    ofstream log(sfile_path, ios::app);
    log << " - " << inAccount_activity << " at " << dtime.current_time() << endl;
    // log << " - " << inAccount_activity << " at " << __TIME__ ;

    log.close();  // closes the file after use
}

// log every transaction
void Log::logTransactions(const string& log_account_num, const string& transaction_type,
                          const long long int& amount)
{
    string sfile_path = Paths::USER_TRANSACTION + log_account_num;

    ofstream file (sfile_path, ios::app);

    file << "-> " << dtime.full_date_time();// << endl ;
    file << "[ " << transaction_type << " ] -> ";
    file << "Amount: " << amount << "\n\n" ;

    file.close(); // closes the file after use

    try
    {
        util::backupTransactionFile(log_account_num);
    }
    catch(...)
    {
        logError(log_account_num, "backing up transaction file failed");
    }
}

void Log::logError(const std::string& account_num, const std::string& msg )
{
    string file_path = Paths::ADMIN_ERROR_LOG + dtime.current_date();

    ofstream file(file_path, ios::app);

    file << "Account Number: " << account_num << endl;
    file << "Error: " << msg <<endl;
    file << "Date & Time: " << dtime.full_date_time() << endl << endl;
}
