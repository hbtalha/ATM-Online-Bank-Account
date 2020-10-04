#include "Utility.h"

#include <iostream>
#include <fstream>
#include <conio.h>
#include <filesystem>
#include <chrono>
#include <date/date.h>
#include <windows.h>

using namespace std;
namespace fs = filesystem;

namespace util
{
// a function that creates the directories where the account files  will be storaged
// it checks wether the necessary folders are already created before creating the folders
void createFolder()
{
    fs::create_directories(Paths::USER_INFO);

    fs::create_directories(Paths::USER_TRANSACTION);

    fs::create_directories(Paths::ADMIN_BACKUP);

    fs::create_directories(Paths::ADMIN_LOCKED_ACCOUNTS);

    fs::create_directories(Paths::ADMIN_LOG);

    fs::create_directories(Paths::ADMIN_ERROR_LOG);

    //  paths to where the dates and values information will be saved for withdrawal and transfer limits purposes

    fs::create_directories(Paths::ADMIN_WITHDRAWAL_DATE);

    fs::create_directories(Paths::ADMIN_WITHDRAWAL_VAL);

    fs::create_directories(Paths::ADMIN_TRANSFER_DATE);

    fs::create_directories(Paths::ADMIN_TRANSFER_VAL);
}

// a function that creates a backup file of the account file and of the transaction
void backupAccountFile(const string& account_num)
{
    string source = Paths::USER_INFO + account_num; // original file path
    string target = Paths::ADMIN_BACKUP + account_num ; // backup file path

    filesystem::copy_file(source, target);
}

void backupTransactionFile(const string& account_num)
{
    string source = Paths::USER_TRANSACTION + account_num;
    string target = Paths::ADMIN_BACKUP + account_num + "-trans" ;

    filesystem::copy_file(source, target);
}

void deleteFile(std::string file_to_delete)
{
    // convert string to const char* so it can be used in file_path for deletion
    /*
      const char* deleteFile = sfile_path.data();
      const char* deleteFile = sfile_path.c_str();
      const char* delete_file = &file_to_delete[0];
    */
    remove(file_to_delete.c_str());
}

// create a set  of files that will contain days and amounts of withdrawal and transfer that helps ensuring the limit for withdrawal and transfer
void createLimitFiles(string account_num)
{
    // a file path to where the file that will contain the current day will be saved
    fs::path datefile_path = Paths::ADMIN_WITHDRAWAL_DATE + account_num;

    DateTime dtime;

    if( ! fs::exists(datefile_path))
    {
        ofstream date_file (datefile_path); // it is to check the current day to ensure the amount of the withdrawal in one day won't be exceeded

        date_file << dtime.mday() << " " << dtime.month() << ' ' << dtime.year();

        date_file.close();
    }

    // a file path to where the file that will contain theamount of withdrawal in one day will be saved
    fs::path valuefile_path = Paths::ADMIN_WITHDRAWAL_VAL + account_num;

    if( ! fs::exists(valuefile_path))
    {
        ofstream valueFile (valuefile_path);

        valueFile << 0; // the initial amount is 0

        valueFile.close();
    }

    // a file path to where the file that will contain the current day will be saved
    fs::path datefile_pathTransfer = Paths::ADMIN_TRANSFER_DATE + account_num;

    if( ! fs::exists(datefile_pathTransfer))
    {
        ofstream dateFileTransfer (datefile_pathTransfer); // it is to check the current day to ensure the amount of the withdrawal in one day won't be exceeded

        dateFileTransfer << dtime.mday() << " " << dtime.month() << ' ' << dtime.year();

        dateFileTransfer.close();
    }

    // a file path to where the file that will contain theamount of withdrawal in one day will be saved
    fs::path valuefile_pathTransfer = Paths::ADMIN_TRANSFER_VAL + account_num;

    if( ! fs::exists(valuefile_pathTransfer))
    {
        ofstream valueFileTransfer (valuefile_pathTransfer);

        valueFileTransfer << 0; // the initial amount is 0

        valueFileTransfer.close();
    }
}


// ensures  the names will begin with an uppercase letter and there will be no space betweens in the name
void setNamesStraight(std::string& str)
{
    int len = str.size();
    for(int i = 0; i <len; ++i)
    {
        if(i == 0)
            for(int j = i + 1; i < len; ++j)
            {
                str[ j ] = tolower(str[ j ]);

                if(isspace(str[ j ]))
                {
                    i = j;
                    break;
                }
            }

        if(isspace(str[i]))
            for(int j = i + 1; j < len; ++j)
            {
                if(! isspace(str[j]))
                {
                    if( ! isupper(str[j]))
                        str[j] = toupper(str[j]);

                    for(int k = j + 1; k < len; ++k)
                    {
                        if( isupper(str[k]))
                            str[k] = tolower(str[k]);

                        if(isspace(str[k]))
                        {
                            i = k - 1;
                            break;
                        }
                    }

                    break;
                }
            }
    }

    if (! isupper(str[0]))
        str[0] = toupper(str[0]);

    // erases all space in between the names
    str.erase(std::remove(str.begin(), str.end(), ' '), str.end());
}

int countHoursPassed(CustomDateTime& time_since)
{
    using namespace chrono;
    using namespace date;

    DateTime curr_time;

    chrono::system_clock::time_point end_time = sys_days(year(curr_time.year())/curr_time.month()/curr_time.mday())
            + hours(curr_time.hour()) + minutes(curr_time.minute());

    chrono::system_clock::time_point start_point = sys_days(year(time_since.year)/time_since.month/time_since.day)
            + hours(time_since.hour) + minutes(time_since.min);

    return chrono::duration_cast<chrono::hours>(end_time - start_point).count();
}

std::string inputAccountNumber(Account *a, void (Account::*returnFunction) ())
{
    int num_length = 1;
    char ch_num;
    string s_a_num;
    ch_num = getch();


    if (ch_num == 27)
    {
        (a->*returnFunction)();
    }

    // in case the user presses enter right away
    while (ch_num == 13 && num_length <= 16)
    {
        if (s_a_num.empty())
            cout << "\b  \b";

        ch_num = getch();

        // if the user wishes to go back to the menu he/she presses esc
        if (ch_num == 27)
        {
            (a->*returnFunction)();
        }
    }

    // while the user doesnt hit enter, 13 is the ASCII code for enter
    while (ch_num != 13)
    {
        if (s_a_num.empty())
        {
            cout << "  \b";
        }

        // when the user hits backspace, 8 s the ASCII code for backspace
        if (ch_num == 8)
        {
            if ( ! s_a_num.empty())
            {
                -- num_length;
                s_a_num.pop_back();
            }

            cout << "\b \b";
        }
        // if the pin lenght is equal or below 4 asterisk will be displayed to mask the pin and s_a_num will be take the character input by the user
        // only numbers are accepted
        else if (num_length <= 16 && isdigit(ch_num))
        {
            if (s_a_num.empty())
                cout << "\b \b";

            s_a_num.push_back(ch_num);

            cout << ch_num;

            ++num_length;
        }
        else if ( s_a_num.empty()) // when the user clear the input this condition won't allow the cursor to improper forward move
        {
            cout << "\b \b";
        }

        ch_num = _getch();

        // if the user wishes to go back to the menu he/she presses esc
        if (ch_num == 27)
        {
            (a->*returnFunction)();
        }

        while (ch_num == 13 && num_length <= 16)
        {
            if (s_a_num.empty())
            {
                cout << " \b";

            }

            ch_num = _getch();

            // if the user wishes to go back to the menu he/she presses esc
            if (ch_num == 27)
            {
                (a->*returnFunction)();
            }
        }
    }

    return s_a_num;
}

std::string inputPin(Account *a, void (Account::*returnFunction) ())
{
    char ch_pin;
    string s_pin;
    int pin_length = 1;

    ch_pin =  _getch();

    if (ch_pin == 27)
        (a->*returnFunction)();

    while (ch_pin == 13 && pin_length <= 4)
    {
        if (s_pin.empty())
        {
            cout << "\b  \b";
        }

        ch_pin = _getch();

        if (ch_pin == 27)
        {
            (a->*returnFunction)();
        }
    }

    while (ch_pin != 13)
    {
        if (s_pin.empty())
        {
            cout << "  \b";
        }

        // when the user hits backspace, 8 s the ASCII code for backspace
        if (ch_pin == 8)
        {
            if ( ! s_pin.empty())
            {
                -- pin_length;
                s_pin.pop_back();

                //move_space = true;
            }

            cout << "\b \b";
        }
        // if the pin lenght is equal or below 4 asterisk will be displayed to mask the pin and s_pin will be take the character input by the user
        // only numbers are accepted
        else if (pin_length <= 4 && isdigit(ch_pin))
        {
            if (s_pin.empty())
                cout << "\b \b";

            s_pin.push_back(ch_pin);

            cout << "*";

            ++pin_length;
        }
        else if ( s_pin.empty()) // when the user clear the input this condition won't allow the cursor to improper forward move
        {
            cout << "\b \b";
        }

        ch_pin = _getch();

        if (ch_pin == 27)
        {
            (a->*returnFunction)();
        }

        while (ch_pin == 13 && pin_length < 5)
        {
            if (s_pin.empty())
            {
                // if (move_space)
                cout << "\b  \b";
                // else
                //   cout << " \b";

                //move_space = 0;
            }

            ch_pin = _getch();

            if (ch_pin == 27)
            {
                (a->*returnFunction)();
            }
        }
    }

    return s_pin;
}

void pressEnter()
{
    cout << "******** Click Enter To Return ********\n";

    char ch;

    ch = _getch();

    while (ch != 13)
    {
        ch = _getch();
    }
}

}
