#ifndef DATETIME_H_INCLUDED
#define DATETIME_H_INCLUDED

#include <sstream>
#include <time.h>

#include <sys/types.h>
#include <sys/stat.h>

class DateTime
{
    time_t now = time(nullptr);
    tm* h = localtime(&now);

public:

    std::string s_wday()
    {
        switch(h->tm_wday)
        {
        case 0:
            return "Sun";
            break;

        case 1:
            return "Mon";
            break;

        case 2:
            return "Tue";
            break;

        case 3:
            return "Wed";
            break;

        case 4:
            return "Thu";
            break;

        case 5:
            return "Fri";
            break;

        case 6:
            return "Sat";
            break;
        }
    }

    std::string s_month()
    {
        switch(h->tm_mon)
        {
        case 0:
            return "Jan";
            break;

        case 1:
            return "Feb";
            break;

        case 2:
            return "Mar";
            break;

        case 3:
            return "Apr";
            break;

        case 4:
            return "May";
            break;

        case 5:
            return "Jun";
            break;

        case 6:
            return "Jul";
            break;

        case 7:
            return "Aug";
            break;

        case 8:
            return "Sep";
            break;

        case 9:
            return "Oct";
            break;

        case 10:
            return "Nov";
            break;

        case 11:
            return "Dec";
            break;
        }
    }

    int second()
    {
        return h->tm_sec;
    }

    int minute()
    {
        return h->tm_min;
    }

    int hour()
    {
        return h->tm_hour;
    }

    int wday()
    {
        return h->tm_wday;
    }

    int mday()
    {
        return h->tm_mday;
    }

    int month()
    {
        return 1+ h->tm_mon;
    }

    int yday()
    {
        return h->tm_yday;
    }

    int year()
    {
        return 1900 + h->tm_year;
    }

    std::string current_time()
    {
        std::stringstream sst;

        if(minute() < 10)
            sst << hour() << ":0" << minute() << ":" << second();
        else
            sst << hour() << ":" << minute() << ":" << second();

        return sst.str();
    }

    std::string current_date()
    {
        std::stringstream sst;

        if(month() < 10)
            sst << mday() << "-0" << month() << "-" << year();
        else
            sst << mday() << "-" << month() << "-" << year();

        return sst.str();
    }

    std::string full_date_time()
    {
        std::stringstream sst;

        sst << ctime(&now);

        return sst.str();
    }
};

struct CustomDateTime
{
    int year;
    int month;
    int day;
    int hour = 0;
    int min  = 0;
    int sec  = 0;
};

#endif // DATETIME_H_INCLUDED
