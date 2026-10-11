#include <time.h>
#include <string>

#include "logs.h"

std::string getFormattedTime()
{
    time_t rawtime;
    struct tm * timeinfo;
    char buffer [30];

    time (&rawtime);
    timeinfo = localtime (&rawtime);
    strftime(buffer, 30, "%T", timeinfo);

    return std::string(buffer);
}