#ifndef ACCOUNT_H_INCLUDED
#define ACCOUNT_H_INCLUDED

#include "DateTime.h"
#include "Logger.h"

class Account
{
private:
    std::string name;
    std::string new_name;
    long long int balance;
    std::string account_num;
    std::string file_path;

    std::string pin, new_pin;

    std::string buffer;

    void withdrawal();
    void deposit ();
    void viewBalance() const;
    void transfer();
    void transactions(const std::string& account_num) const;

    void withdrawalLimit(int withdrawalAmount);
    void transferLImit (int transferAmount);

    void dayShiftCheck();

    void settings();

    void changePin();
    void pinChanged();
    void changeName();
    void nameChanged();
    void clearTransactions();
    void transactionsCleared();

    void lockAccount();
    void lockAccountCheck();

    void signUp();
    void signIn();
    void myAccount();
    void signOut();

    void parseAccountInfo(std::string buffer, std::string& assigned_name, std::string& assigned_account_num, long long int& assigned_Balance, std::string& assigned_pin) const;

    std::string accountExistenceCheckSignIn(std::string& account_num);
    std::string accountExistenceCheckSignUp(std::string& account_num) ;
    std::string accountExistenceCheckTransfer(std::string& account_num) ;

    DateTime dtime;

    Log logger;

    Account() = default;

public:

    static Account& getOnlyInstance()
    {
        static Account acc;

        return acc;
    }

    void mainMenu();
};

#endif // ACCOUNT_H_INCLUDED
