#include "Account.h"
#include "Paths.h" // a class where constant strings, that contains entire paths to where file containing the necessary info, will be managed
#include "Utility.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <stdlib.h>
#include <conio.h>
#include <direct.h>
#include <windows.h>
#include <algorithm>

using namespace std;

// display the simple menu at the beginning
void Account::mainMenu()
{
    system("cls");

    util::createFolder();

    cout << "\n\n\n\n\n\n \t\t\t\t\t" << "1 -> Sign In" << endl;
    cout << " \t\t\t\t\t" << "2 -> Sign Up" << endl << endl;
    cout << " \t\t\t\t\t" << " -> ";
    char option = _getch();

    while (option != '1' && option != '2' && option != 27)
    {
        option = _getch();
    }

    switch (option)
    {
    case '1':
        system("cls");
        signIn();
        break;

    case '2':
        system("cls");
        signUp();
        break;

    case '0':
        cout << "\nProcess returned 0" << endl << "Press enter key to continue";
        cin.get();
        exit(0);
        break;

    case 27:
        cout << "\nProcess returned 0" << endl << "Press enter key to continue";
        cin.get();
        exit(0);
        break;

    default:
        mainMenu();
        break;

    }
}


// a function to lock the account that had 3 failed attempts on a password input
void Account::lockAccount()
{
    string file_path = Paths::ADMIN_LOCKED_ACCOUNTS + account_num;

    ofstream lock (file_path); // create the file that contains the time of the lock

    lock << dtime.mday() << " " << dtime.month() << " " << dtime.year()  << " " << dtime.hour() << " " <<  dtime.minute();

    lock.close();
}

// a function that everytime a user goes to sign in it checks if the account is locked or not
void Account::lockAccountCheck()
{
    string sfile_path = Paths::ADMIN_LOCKED_ACCOUNTS + account_num;

    ifstream file_in (sfile_path);

    // checks for the existence of a file that contains the time of the lock, if the file doesn't exist it returns meaning that
    // the account is not locked
    if ( ! file_in.good() )
    {
        return;
    }

    string bufferLine;

    // integers that hold the dates values
    int lock_day, lock_month, lock_year, lock_hour, lock_minute;

    // it gets the information of the time from the lock account file and hold it in char
    getline(file_in, bufferLine);

    istringstream lockFile (bufferLine); // it parses the contents of the file

    // it assigns each date value to the respective variable
    lockFile >> lock_day >> lock_month >> lock_year >> lock_hour >> lock_minute;

    file_in.close();

    CustomDateTime lock_time{lock_year, lock_month, lock_day, lock_hour, lock_minute};

    long long int hours_passed = util::countHoursPassed(lock_time);

    if(hours_passed < 24)
    {
        cout << "\n\nYour Account Has Been Locked Since " << lock_day
             << " - " << lock_month << " - " << lock_year << " | "
             << lock_hour << ":" << lock_minute << "\n\n"
             << "Locking Period: 24 Hours\n\n";

        util::pressEnter();

        mainMenu();
    }

    // the file that contains the  locking info will be deleted after it is known that it has passed 24hours after the lock
    util::deleteFile(sfile_path);
}

void Account::parseAccountInfo(string buffer, string& assigned_name, string& assigned_account_num,
                               long long int& assigned_balance, string& assigned_pin) const
{
    // associate an istrstream object with the accountut
    istringstream account(buffer);

    // character string
    account >> assigned_name;

    // now the account number
    account >> assigned_account_num;

    // and the balance
    account >> assigned_balance;

    account >> assigned_pin;
}

// function that checks wether the file exists or not when the user is signing in
string Account::accountExistenceCheckSignIn(string& account_num) // the string takes the account number input by the user
{
    string sfile_path = Paths::USER_INFO + account_num;

    ifstream file_stream(sfile_path);

    // a condition that checks wether the file exists or not, if it is good it means that the file exists thus the account too
    if (file_stream.good())
    {
        file_stream.close();
        return sfile_path;
    }

    while (true)
    {
        cout << endl << endl << "Account Doesn't Exist!!" << endl;
        cout << endl << "Enter Your Bank Account Number: ";
        account_num = util::inputAccountNumber(this, mainMenu);

        // when the user learns that the account doesnt exist he or she has the optionion of entering '0' and go back to the main menu
        if (account_num == "0")
        {
            mainMenu();
        }

        sfile_path = Paths::USER_INFO + account_num;

        ifstream file_stream(sfile_path);

        // the user will keep being noticed that the account doesnt exist until he or she either inputs a valid account number or '0'
        if (file_stream.good())
        {
            file_stream.close();
            break;
        }
    }

    return sfile_path; // the function will return the path of the acoount file
}

// function that checks wether the file exists or not when the user is signing up
string Account::accountExistenceCheckSignUp(string& account_num)
{
    string sfile_path = Paths::USER_INFO + account_num;

    ifstream file_stream(sfile_path);

    // a condition that checks wether the file exists or not, if it doesnt exit it breaks out the loop meaning that the account
    // doesnt exist so it is safe to create an account with that number
    if ( ! file_stream.good())
    {
        file_stream.close();
        return sfile_path;
    }

    while (true)
    {
        cout << endl << endl << "Account Already Exists!!" << endl;
        cout << "Account Number: ";
        account_num = util::inputAccountNumber(this, mainMenu);

        // when the user learns that the account doesnt exist he or she has the optionion of entering '0' and go back to the main menu
        if (account_num == "0")
        {
            mainMenu();
        }

        sfile_path = Paths::USER_INFO + account_num;

        ifstream file_stream (sfile_path);

        // the user will keep being noticed that the account doesnt exist until he or she either inputs a valid account number or '0'
        if ( ! file_stream.good())
        {
            file_stream.close();
            break;
        }
    }

    cout << endl;

    return sfile_path;
}

// a function that checks for the existence of a file that is aimed to be transfered an amount
string Account::accountExistenceCheckTransfer(string& transfer_account_number)
{
    string sfile_path = Paths::USER_INFO + transfer_account_number;

    ifstream file_stream(sfile_path);

    if (file_stream.good())
    {
        file_stream.close();
        return sfile_path;
    }

    while (true)
    {
        cout << endl << endl << "Account Doesn't Exist!!" << endl;
        cout << setw(10) << "To Account Number: ";
        transfer_account_number = util::inputAccountNumber(this, myAccount);

        while (transfer_account_number == account_num)
        {
            cout << endl << endl << "Can't Transfer Money To Your Own Account!" << endl;
            cout << setw(10) << "To Account Number: ";
            transfer_account_number = util::inputAccountNumber(this, myAccount);
        }

        sfile_path = Paths::USER_INFO + transfer_account_number;

        file_stream.open(sfile_path);

        if (file_stream.good())
        {
            file_stream.close();
            break;
        }
    }

    return sfile_path;
}

// function that gets the user through the 'sign up' steps
void Account::signUp()
{
    balance = 0; // balance is set to zero because when the user creates a new account his or her balance will be zero

    string sname;

    system("cls");

    cout << "\t\t\t Sign Up" << endl << endl;

    cout << "Name: ";
    getline(cin, sname);

    if (sname == "0")
    {
        mainMenu();
    }

    while (sname.empty())
    {
        system("cls");

        cout << "\t\t\t Sign Up" << endl << endl;

        cout << "Name: ";
        getline(cin, sname);

        if (sname == "0")
        {
            mainMenu();
        }
    }

    for (unsigned int i = 0; i < sname.size(); ++i)
    {
        if  (( ! isalpha(sname[i]) && ! isspace(sname[i]) ) || isspace(sname[0]))
        {
            cout << endl << "Invalid Name \n Re-enter: ";
            getline(cin, sname);

            if (sname == "0")
            {
                mainMenu();
            }

            while (sname.empty())
            {
                cout << endl << "Can't Be Empty \n Re-enter: ";
                getline(cin, sname);

                if (sname == "0")
                {
                    mainMenu();
                }
            }

            i = -1;
        }
    }

    system("cls");

    cout << "\t\t\t Sign Up" << endl << endl;

    cout << "Name: " << sname;

    cout << endl << endl << "Account Number: ";
    account_num = util::inputAccountNumber(this, mainMenu);

    // after the account number has been input this will call the function that checks wether the ccount number already exists or not
    file_path = accountExistenceCheckSignUp(account_num);

    system("cls");

    cout << "\t\t\t Sign Up" << endl << endl;

    cout << "Name: " << sname << endl << endl;

    cout << "Account Number: " << account_num << endl << endl;

    cout << "Pin: ";
    pin = util::inputPin(this, mainMenu);

    // it creates a file that will contain all the account information
    ofstream file(file_path);

    util::setNamesStraight(sname);

    // it will write the account needed information to the recently created file
    file << sname << " " << account_num << " " << " " << balance << " " << pin;

    file.close();

    try
    {
        util::backupAccountFile(account_num);
    }
    catch(...)
    {
        logger.logError(account_num, "backing up account file failed");
    }

    system("cls");

    cout << "\a \nAccount Successfully Created!!!";

    logger.log(account_num, sname, "Sign Up", false);

    Sleep(1000);

    system("cls");

    util::createLimitFiles(account_num);

    ifstream file_stream(file_path); // it enters the just created account file so the user can directly sign in after sign up

    getline(file_stream, buffer);

    parseAccountInfo(buffer, name, account_num, balance, pin);

    myAccount();
}

// function that gets the user through the 'sign in' steps
void Account::signIn()
{
    cout << "\t\t\t Sign In" << endl << endl;

    cout << "Enter Your Bank Account Number: ";
    account_num = util::inputAccountNumber(this, mainMenu);

    // it checks for the existence of the account
    file_path = accountExistenceCheckSignIn(account_num);

    // it checks if the account is locked
    lockAccountCheck();

    ifstream file_stream(file_path);

    getline(file_stream, buffer);

    parseAccountInfo(buffer, name, account_num, balance, pin);

    file_stream.close();

    system("cls");

    cout << "\t\t\t My Account" << endl << endl;

    cout << "Welcome " << name << endl << endl;

    string pin_check;
    bool note = false;

    cout << "Pin: ";
    pin_check = util::inputPin(this, mainMenu);

    int pin_checkCount = 1;

    while (pin_check != pin)
    {
        system("cls");

        cout << "\t\t\t My Account" << endl << endl;

        cout << "Welcome " << name << endl << endl;

        // it counts how many passwords input attempts the user has tried, when it equal 3 the account is locked for 24 hours
        if (pin_checkCount == 3)
        {
            cout << endl << "3 Attempts Failed!!" << endl;
            cout << "Your Account Has Been Locked For the Next 24 Hours" << endl;

            cout << endl << "Press enter key to continue";

            note = true; //the note is set to true to log the fact the account got locked

            logger.log(account_num, name, "Attempted Sign In", note);
            lockAccount();

            char ch = _getch();

            while(ch != 13)
            {
                ch = _getch();
            }

            mainMenu();
        }

        cout << "Wrong Pin!!" << endl;
        cout << "Pin: ";
        pin_check = util::inputPin(this, mainMenu);

        ++pin_checkCount;
    }

    logger.log(account_num, name, "Sign In", note); // it will log the time of the 'sign in'

    try
    {
        util::backupAccountFile(account_num);
    }
    catch(...)
    {
        logger.logError(account_num, "backing up account file failed");
    }

    util::createLimitFiles(account_num);

    myAccount(); // once the user is signed into his or her account the function 'myAccount' will be called
}

// function that lets the user navigate into his or her account
void Account::myAccount()
{
    system("cls");

    while (true)
    {
        cout << endl << "Account: " << name << endl << endl;

        cout << setw(10) << "1 -> Balance";
        cout << setw(22) << "2 -> Withdrawal" << endl << endl;
        cout << setw(10) << "3 -> Deposit";
        cout << setw(20) << "4 -> Transfer" << endl << endl;
        cout << setw(10) << "5 -> Transactions";
        cout << setw(16) << "6 -> Settings " << endl << endl;
        cout << setw(10) << "0 -> Sign Out" << endl << endl;
        cout << setw(17) << "-> ";
        char option = _getch();

        while (option != '0' and option != '1' && option != '2' && option != '3' && option != '4' && option != '5' && option != '6')
        {
            option = _getch();
        }

        switch(option)
        {
        case '1':
            logger.logInsideAccount("View Balance");
            viewBalance();
            break;

        case '2':
            logger.logInsideAccount("Withdrawal");
            withdrawal();
            break;

        case '3':
            logger.logInsideAccount("Deposit");
            deposit();
            break;

        case '4':
            logger.logInsideAccount("Transfer");
            transfer();
            break;

        case '5':
            // it logs the activity (View Transaction) made in the account
            logger.logInsideAccount("View Transactions");

            transactions(account_num);
            break;

        case '6':
            logger.logInsideAccount("Enter Settings");
            settings();
            break;

        case '0':
            signOut();

        default:
            myAccount();
        }
    }
}

void Account::viewBalance() const
{
    system("cls");

    cout << endl << "Account: " << name << endl << endl;

    cout << "Balance: " << balance << endl << endl;

    util::pressEnter();

    system("cls");
}


void Account::withdrawal()
{
    system("cls");

    util::createLimitFiles(account_num);

    string sAmount;

    long double amount;

    cout << "\t\t\t\t Withdrawal" << endl;

    cout << endl << "Account: " << name << endl << endl;

    cout << endl << " Amount Of Withdrawal: ";
    getline(cin, sAmount);

    for (unsigned int i = 0; i < sAmount.size(); ++i)
    {
        if (! isdigit(sAmount[i]))
        {
            cout << endl << "Invalid Amount!! \nAmount: ";
            getline(cin, sAmount);

            i = -1;
        }
    }

    amount = atof(sAmount.c_str()); // it coverts the string to a float number

    while (amount > balance)
    {
        cout << endl << "Insufficient Balance!!";
        cout << endl << "   Amount Of Withdrawal: ";
        getline(cin, sAmount);

        for (unsigned int i = 0; i < sAmount.size(); ++i)
        {
            if (! isdigit(sAmount[i]))
            {
                cout << endl << "Invalid Amount!! \n  Amount Of Withdrawal: ";
                getline(cin, sAmount);

                i = -1;
            }
        }

        /*
        amount = strtod(sAmount.c_str(), nullptr);
        amount = atof(sAmount.c_str());
        */

        amount = stof(sAmount); // converting string to int
    }

    system("cls");

    if (amount != 0)
    {
        cout << endl << "Account: " << name <<  endl << endl;

        cout << "Withdrawal Amount: " << amount << endl << endl;

        cout << setw(18) << "Confirm" << endl << endl;

        cout << setw(10) << "1 -> Yes" << setw(20) << "2 -> No" << endl << endl;

        cout << setw(15) << "-> ";
        char option = _getch();

        while (option != '1' && option != '2')
        {
            option = _getch();
        }

        switch (option)
        {
        case '1':
            withdrawalLimit(amount);
            balance -= amount;
            logger.logTransactions(account_num, "Withdrawal", amount);
            break;

        case '2':
            withdrawal();
            break;
        }

        ofstream drawFile (file_path);

        drawFile << name << " " << account_num << " " << " " << balance << " " << pin; // updates the information in the account file

        drawFile.close();
    }

    system("cls");

    myAccount();
}

void Account::withdrawalLimit(int withdrawalAmount)
{
    dayShiftCheck();

    string sfile_path = Paths::ADMIN_WITHDRAWAL_VAL + account_num;

    int amountDayDrawal;

    ifstream file (sfile_path);

    getline(file, buffer);

    istringstream amountFile (buffer);

    amountFile >> amountDayDrawal;

    file.close();

    int amount_limit = amountDayDrawal + withdrawalAmount;

    ofstream day_amount_file (sfile_path);

    if ( amount_limit > 20000)
    {
        system("cls");
        cout << "\a \n\n\n\n\n\n \t" << "Exceeded The Withdrawal Amount Limit For The Day"
             << endl << "\t" << "Withdrawal Amount Limit -> 20000" << endl;

        Sleep(2000);

        day_amount_file << amountDayDrawal;

        day_amount_file.close();

        myAccount();
    }
    else
    {
        day_amount_file << amount_limit;

        day_amount_file.close();
    }
}

// a function that checks wether the day has changed so it can update files that will control the limit of withdrawal and transfer done in a day
void Account::dayShiftCheck()
{
    string dayfile_path = Paths::ADMIN_WITHDRAWAL_DATE + account_num;

    string valuefile_path = Paths::ADMIN_WITHDRAWAL_VAL + account_num;

    string dayfile_pathTransfer = Paths::ADMIN_TRANSFER_DATE + account_num;

    string valuefile_pathTransfer = Paths::ADMIN_TRANSFER_VAL + account_num;

    int day = dtime.mday(); // will hold the current day
    int month = dtime.month(); // will hold the current day
    int year = dtime.year(); // will hold the current day

    int file_day, file_month, file_year;

    ifstream dayFile (dayfile_path);

    getline(dayFile, buffer);

    istringstream file (buffer);

    file >> file_day;
    file >> file_month;
    file >> file_year;

    if ( day != file_day || month != file_month || year != file_year )
    {
        ofstream valueFile (valuefile_path);

        valueFile << 0;

        valueFile.close();

        ofstream dateFile (dayfile_path);

        dateFile << day;

        dateFile.close();
    }

    ifstream dayFileTransfer (dayfile_pathTransfer);

    getline( dayFileTransfer, buffer);

    istringstream file_transfer (buffer);

    file_transfer >> file_day;
    file_transfer >> file_month;
    file_transfer >> file_year;

    if ( day != file_day or month != file_month || year != file_year )
    {
        ofstream value_file_transfer (valuefile_pathTransfer);

        value_file_transfer << 0;

        value_file_transfer.close();

        ofstream date_file_transfer (dayfile_pathTransfer);

        date_file_transfer << day;

        date_file_transfer.close();
    }
}

void Account::deposit()
{
    system("cls");

    string sAmount;

    int amount;

    cout << "\t\t\t\t Deposit" << endl;

    cout << endl << "Account: " << name << endl << endl;

    cout << endl << " Amount Of Deposit: ";
    getline(cin, sAmount);

    if (sAmount == "0")
    {
        system("cls");
        ifstream file (file_path);
        myAccount();
    }

    for (unsigned int i = 0; i < sAmount.size(); ++i)
    {
        if (! isdigit(sAmount[i]))
        {
            cout << endl << "Invalid Amount!! \nAmount Of Deposit: ";
            getline(cin, sAmount);

            if (sAmount == "0")
            {
                system("cls");
                myAccount();
            }

            i = -1;
        }
    }

    amount = atof(sAmount.c_str());

    while (amount < 0)
    {
        cout << endl << "Invalid Amount!";
        cout << endl << " Amount Of Deposit: ";
        getline(cin, sAmount);

        if (sAmount == "0")
        {
            system("cls");
            myAccount();
        }

        for (unsigned int i = 0; i < sAmount.size(); ++i)
        {
            if (! isdigit(sAmount[i]))
            {
                cout << endl << "Invalid Amount!! \nAmount Of Deposit: ";
                getline(cin, sAmount);

                if (sAmount == "0")
                {
                    system("cls");
                    myAccount();
                }

                i = -1;
            }
        }

        amount = atof(sAmount.c_str());
    }

    system("cls");

    if ( amount > 0)
    {
        cout << endl << "Account: " << name <<  endl << endl;

        cout << "Deposit Amount: " << amount << endl << endl;

        cout << setw(18) << "Confirm" << endl << endl;

        cout << setw(10) << "1 -> Yes" << setw(20) << "2 -> No" << endl << endl;

        cout << setw(15) << "-> ";
        char option = _getch();

        while (option != '1' && option != '2')
        {
            option = _getch();
        }

        switch (option)
        {
        case '1':
            balance += amount;
            logger.logTransactions(account_num, "Despoit", amount);
            break;

        case '2':
            deposit();
            break;
        }

        system("cls");

        ofstream depoFile (file_path);

        depoFile << name << " " << account_num << " " << " " << balance << " " << pin; // updates the information in the account file

        depoFile.close();

        cout << endl << "Succesful Deposit" << endl << endl;

        util::pressEnter();

    }

    system("cls");

    myAccount();
}

void Account::transfer()
{
    system("cls");

    util::createLimitFiles(account_num);

    string transfer_account_number; // account number to where the money will be transfered
    string transfer_name; // name of the  destined account owner
    string transfer_file_path; // file path of the account to where the money will be transfered
    string sTransfer_amount; // amount that is going to be transfrered in string
    long int transfer_amount; // amount that is going to be transfrered in int
    long long int transfer_balance; // updated balance of the account to where the money will be transfered
    string transfer_pin; // pin of the account to where the money will be transfered

    cout << "\t\t\t\t Transfer" << endl;

    cout << endl << "Account: " << name << endl << endl;

    cout << setw(10) << "To Account Number: ";
    transfer_account_number = util::inputAccountNumber(this, myAccount);

    while (transfer_account_number == account_num)
    {

        cout << endl << endl << "Can't Transfer Money To Your Own Account!" << endl;
        cout << setw(10) << "To Account Number: ";
        transfer_account_number = util::inputAccountNumber(this, myAccount);
    }

    // checks for the exixtence of the account  to where the money will be transfered
    transfer_file_path = accountExistenceCheckTransfer(transfer_account_number);

    ifstream file (transfer_file_path);

    getline(file, buffer);

    // parses the account file and assign each information to new variables that are going to be used later
    parseAccountInfo(buffer, transfer_name, transfer_account_number, transfer_balance, transfer_pin);

    system("cls");

    cout << "\t\t\t\t Transfer" << endl;

    cout << endl << "Account: " << name << endl << endl;

    cout << "Balance: " << balance << endl << endl;

    cout << "Transfer To " << transfer_name << endl;
    cout << setw(10) << "Amount: ";
    getline(cin, sTransfer_amount); // input the amount wished to be transfered

    if (sTransfer_amount == "0")
    {
        myAccount();
    }

    // makes sure that only numbers are input
    for (unsigned int i = 0; i < sTransfer_amount.size(); ++i)
    {
        if (! isdigit(sTransfer_amount[i]))
        {
            cout << endl << "Invalid Amount!! \nAmount Of Transfer: ";
            getline(cin, sTransfer_amount);

            // a condition to cancel the operation and head back to the main menu
            if (sTransfer_amount == "0")
            {
                myAccount();
            }

            i = -1;
        }
    }

    // since the amount that is going to be transfered is in a string variable it needs to be coverted to int variable
    transfer_amount = atof(sTransfer_amount.c_str());

    // the transfer amount can not be larger than the balance
    while (transfer_amount > balance)
    {
        cout << endl << "Insufficient Balance!!";
        cout << endl << " Amount: ";
        cin >> sTransfer_amount;

        transfer_amount = atof(sTransfer_amount.c_str());

        for (unsigned int i = 0; i < sTransfer_amount.size(); ++i)
        {
            // makes sure that only numbers are input
            if (! isdigit(sTransfer_amount[i]))
            {
                cout << endl << "Invalid Amount!! \nAmount Of Transfer: ";
                getline(cin, sTransfer_amount);

                // a condition to cancel the operation and head back to the main menu
                if (transfer_account_number == "0")
                {
                    myAccount();
                }

                transfer_amount = atof(sTransfer_amount.c_str());

                i = -1;
            }
        }
    }

    system("cls");

    cout << "\t\t\t\t Transfer" << endl;

    cout << endl << "Account: " << name << endl << endl;

    cout << "Transfer To " << transfer_name << endl << endl;

    cout << "Transfer Amount: " << sTransfer_amount << endl << endl;

    cout << setw(18) << "Confirm" << endl << endl;  // confirm that the amount input is right

    cout << setw(10) << "1 -> Yes" << setw(20) << "2 -> No" << endl << endl;

    cout << setw(15) << "-> ";
    char option = _getch();

    while (option != '1' && option != '2')
    {
        option = _getch();
    }

    // if the user chooses the first optionion there will be a whole process to update the information contained both in the account file of user that
    // transfered  the money and the one that received
    if (option == '1' && transfer_amount  > 0)
    {
        transferLImit(transfer_amount);

        balance -= transfer_amount; // the balance will be withdrawn the amount transfered

        ofstream account_file (file_path);

        account_file << name << " " << account_num << " " << " " << balance << " " << pin;

        try
        {
            util::backupAccountFile(account_num); // backup the file
        }
        catch(...)
        {
            logger.logError(account_num, "backing up account file failed");
        }

        account_file.close();

        // the amount transfered will be added to the updated balance variable of the account that receives the transfer
        transfer_balance += transfer_amount;

        // it creates a file that overwrite the file of the receiver account updating the information necessary
        ofstream transfer_file (transfer_file_path);
        transfer_file << transfer_name << " " << transfer_account_number << " " << " " << transfer_balance << " " << transfer_pin;

        try
        {
            util::backupAccountFile(transfer_account_number);
        }
        catch(...)
        {
            logger.logError(transfer_account_number, "backing up account file failed");
        }
        transfer_file.close();

        // log this activity in the receiver account
        logger.logTransactions(transfer_account_number, "Received Transfer From Account Number: " + account_num, transfer_amount);

    }
    else
        transfer();

    system("cls");

    // log this activity in the sender account
    logger.logTransactions(account_num, "Transfer To Account Number: " + transfer_account_number + " ( " + transfer_name + " )", transfer_amount);

    cout << endl << "Succesful Transfer" << endl << endl;

    util::pressEnter();

    system("cls");
}

void Account::transferLImit (int transferAmount)
{
    dayShiftCheck();

    string sfile_path = Paths::ADMIN_TRANSFER_VAL + account_num;

    int amount_day_transfer;

    ifstream file (sfile_path);

    getline(file, buffer);

    istringstream amount_file (buffer);

    amount_file >> amount_day_transfer;

    file.close();

    int amount_limit = amount_day_transfer + transferAmount;

    ofstream day_amount_file (sfile_path);

    if ( amount_limit > 20000)
    {
        system("cls");
        cout << "\a \n\n\n\n\n\n \t" << "Exceeded The Transfer Amount Limit For The Day" << endl <<
             "\t" << "Transfer Amount Limit -> 20000" << endl;

        Sleep(2000);

        day_amount_file << amount_day_transfer;

        day_amount_file.close();

        myAccount();
    }
    else
    {
        day_amount_file << amount_limit;

        day_amount_file.close();
    }
}

//  view transactions
void Account::transactions(const string& account_num) const
{
    system("cls");

    string sfile_path = Paths::USER_TRANSACTION + account_num;

    cout << "\t\t\t\t Transactions" << endl;

    cout << endl << "Account: " << name  << " \b\b" << endl << endl;

    ifstream file_in(sfile_path);

    // print out the entire content of the file
    if(file_in.peek() != ifstream::traits_type::eof()) // if empty
    cout << file_in.rdbuf();

    cout << endl << endl;

    util::pressEnter();

    system("cls");
}

void Account::settings()
{
    system("cls");

    cout << "\t\t\t\t Settings" << endl;

    cout << endl << "Account: " << name  << " \b\b" << endl << endl << endl;

    cout << setw(10) << "1 -> Change Name";
    cout << setw(28) << "2 -> Change Pin" << endl << endl << endl;
    cout << setw(10) << "3 -> Clear Transactions";
    cout << setw(15) << "0 -> Back" << endl << endl << endl;
    cout << setw(20) << "-> ";
    char option = _getch();

    while (option != '1' && option != '2' && option != '3' && option != '0')
    {
        option = _getch();
    }

    switch(option)
    {
    case '1':
        logger.logInsideAccount("Enter Settings/Change Name");
        changeName();
        break;

    case '2':
        logger.logInsideAccount("Enter Settings/Change Pin");
        changePin();
        break;

    case '3':
        logger.logInsideAccount("Enter Settings/Clear Transactions");
        clearTransactions();
        break;

    case '0':
        myAccount();
        break;

    default:
        settings();
        break;
    }
}

void Account::changeName()
{
    system("cls");

    //char chName;

    cout << "\t\t\t\t Settings" << endl;

    cout << endl << "Account: " << name  << " \b\b" << endl << endl << endl;

    cout << ".../Change Name" << endl << endl;

    cout << "New Name: ";
    getline(cin, new_name);

    // it will return to the menu
    if (new_name == "0")
    {
        settings();
    }

    while (new_name.empty())
    {
        system("cls");

        cout << "\t\t\t Sign Up" << endl << endl;

        cout << "Name: ";
        getline(cin, new_name);

        // it will return to the menu
        if (new_name == "0")
        {
            settings();
        }
    }

    for (unsigned int i = 0; i < new_name.size(); ++i)
    {
        if  (( ! isalpha(new_name[i]) && ! isspace(new_name[i]) ) || isspace(new_name[0]))
        {
            cout << endl << "Invalid Name \n Re-enter: ";
            getline(cin, new_name);

            // it will return to the menu
            if (new_name == "0")
            {
                settings();
            }

            while (new_name.empty())
            {
                cout << endl << "Can't Be Empty \n Re-enter: ";
                getline(cin, new_name);

                // it will return to the menu
                if (new_name == "0")
                {
                    settings();
                }
            }

            i = -1;
        }
    }

    // this block of commented code was meant to make the program only accept chraracters input where the name is required
    // but couldn't do it because a set of keys require two ASCII numbers input to work and if not met they would display a letter
    /*
    chName = _getch();

    if (chName  == 72 || chName == 75 || chName == 77 || chName == 80 || chName == 65 || chName == 66 || chName == 67 \
            || chName == 68 || chName == 82 || chName == 71 || chName == 73 || chName == 79 || chName == 81 || chName == 83 )
    {
        if (new_name.empty())
            cout << " \b\b";

    }

    while (chName == 13 && new_name.empty() && ! isalpha(chName))
    {
        cout << " \b";

        chName = _getch();
    }

    while ( chName != 13 ) // while the user doesnt hit enter, 13 is the ASCII code for enter
    {
        if (new_name.empty())
        {
            cout << "  \b";
        }

        if (chName  == 72 || chName == 75 || chName == 77 || chName == 80 || chName == 65 || chName == 66 || chName == 67 \
                || chName == 68 || chName == 82 || chName == 71 || chName == 73 || chName == 79 || chName == 81 || chName == 83 )
        {
            if (new_name.empty())
                cout << " \b\b";

        }
        else if (chName == 8) // when the user hits backspace, 8 s the ASCII code for backspace
        {
            if ( ! new_name.empty())
            {
                new_name.pop_back();
            }

            cout << "\b \b";
        }
        else if ( isalpha(chName) || isspace(chName)) // the name will only receive alphabet characters and spaces
        {
            if (new_name.empty())
                cout << "\b \b";

            new_name.push_back(chName);

            cout << chName;
        }
        else if ( new_name.empty()) // when the user clear the input this condition won't allow the cursor to improper forward move
        {
            cout << "\b \b";
        }

        if ( isspace(new_name[0]) ) // it guarantess  the user won't ever be able to create an account  name consisted of  spaces only or starting a space for that matter
        {
            cout << "\b";

            new_name.pop_back();
        }

        chName = _getch();

        while (chName == 13 && new_name.empty()) // if the name is clear yet, the user won't be able to go ahead
        {
            cout << " \b";

            chName = _getch();
        }
    }
    */

    system("cls");

    cout << "\t\t\t\t Settings" << endl;

    cout << ".../Change Name" << endl << endl;

    cout << "New Name: " << new_name << endl << endl;

    cout << "1-> Confirm" << setw(12) << "2-> Alter" << setw(15) << "3-> Cancel" << endl << endl;
    cout << "-> ";
    char option = _getch();

    while (option != '1' && option != '2' && option != '3' )
    {
        option = _getch();
    }

    switch(option)
    {
    case '1':
        nameChanged();
        break;

    case '2':
        changeName();
        break;

    case '3':
        settings();
        break;

    default:
        changeName();
        break;
    }
}

void Account::nameChanged()
{
    logger.logInsideAccount("Successfully Changed The Name");

    util::setNamesStraight(new_name);

    name = new_name;

    ofstream file(file_path);

    file << new_name << " " << account_num << " " << " " << balance << " " << pin;

    file.close();

    system("cls");

    new_name.clear();

    cout << endl << "Name Successfully Changed" << endl << endl;

    try
    {
        util::backupAccountFile(account_num);
    }
    catch(...)
    {
        logger.logError(account_num, "backing up account file failed");
    }

    util::pressEnter();

    settings();
}

void Account::changePin()
{
    system("cls");

    logger.logInsideAccount("ChangePin");

    cout << "\t\t\t\t Settings" << endl;

    cout << endl << "Account: " << name  << " \b\b" << endl << endl << endl;

    cout << ".../Change Pin" << endl << endl;

    cout << "New Pin: ";

    new_pin = util::inputPin(this, settings);

    system("cls");

    cout << "\t\t\t\t Settings" << endl;

    cout << ".../Change Pin" << endl << endl;

    cout << "New Pin: " << new_pin << endl << endl;

    string pin_check;

    cout << "1-> Confirm" << setw(12) << "2-> Alter" << setw(15) << "0-> Cancel" << endl << endl;
    cout << "-> ";
    char option = _getch();

    while (option != '1' && option != '2' && option != '0')
    {
        option = _getch();
    }

    if (option == '1')
    {
        cout << option;

        cout << endl << endl << "Enter Your Current Pin: ";
        pin_check = util::inputPin(this, changePin);

        int pinCount = 1;

        while (pin_check != pin)
        {
            if (pinCount == 3)
            {
                cout  << endl<< endl << "3 Attempts Failed!!" << endl;
                cout << "Your Account Has Been Locked For the Next 24 Hours" << endl;

                cout  << endl<< endl << "Press enter key to continue";

                logger.logInsideAccount("Account Locked");
                lockAccount();

                cin.get();

                mainMenu();
            }

            cout << endl << endl << "Wrong Pin // Try Again: ";
            pin_check = util::inputPin(this, settings);

            ++pinCount;
        }
    }

    switch(option)
    {
    case '1':
        pinChanged();
        break;

    case '2':
        changePin();
        break;

    case '0':
        settings();
        break;

    default:
        changePin();
        break;
    }
}

void Account::pinChanged()
{
    logger.logInsideAccount("Successfully Changed The Pin");

    pin = new_pin;

    ofstream file(file_path);

    file << name << " " << account_num << " " << " " << balance << " " << pin;

    system("cls");

    cout << endl << "Pin Successfully Changed" << endl << endl;

    util::pressEnter();

    try
    {
        util::backupAccountFile(account_num);
    }
    catch(...)
    {
        logger.logError(account_num, "backing up account file failed");
    }

    settings();
}

void Account::clearTransactions()
{
    system("cls");

    cout << "\t\t\t\t Settings" << endl;

    cout << endl << "Account: " << name  << " \b\b" << endl << endl << endl;

    cout << ".../Clear Transactions" << endl << endl;

    cout << "1-> Confirm" << setw(12) << "2-> Cancel" << endl << endl;
    cout << "-> ";
    char option = _getch();

    while (option != '1' && option != '2')
    {
        option = _getch();
    }

    switch(option)
    {
    case '1':
        transactionsCleared();
        break;

    case '2':
        settings();
        break;

    default:
        clearTransactions();
        break;
    }
}

void Account::transactionsCleared()
{
    logger.logInsideAccount("Transactions Successfully Cleared");

    string sfile_path = Paths::USER_TRANSACTION + account_num;

    ofstream file(sfile_path);

    system("cls");

    cout << endl << "Transactions Successfully Cleared" << endl << endl;

    util::pressEnter();

    settings();
}

void Account::signOut()
{
    system("cls");

    cout << "\n\n\n\n\n\n \t\t\t\t\t" << "Sign Out" << endl << endl;
    cout << " \t\t\t\t  " << "1 -> Yes" << "     " << "2 -> No" << endl << endl;
    cout << " \t\t\t\t\t " << " -> ";
    char option = _getch();

    while (option != '1' && option != '2')
    {
        option = _getch();
    }

    switch (option)
    {
    case '1':
        logger.logInsideAccount("Sign Out");
        mainMenu();
        break;

    case '2':
        break;

    default:
        signOut();
        break;
    }
}















