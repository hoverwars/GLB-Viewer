#ifndef LOGS_H
#define LOGS_H

#include <iostream>
#include <string>
#include <time.h>

std::string getFormattedTime();

inline void logInfo(std::string message)
{
    std::cout << getFormattedTime() << " [INFO] " << message << std::endl;
}

inline void logInfo(const char* message)
{
    std::cout << getFormattedTime() << " [INFO] " << message << std::endl;
}

inline void logWarning(std::string message)
{
    std::cout << getFormattedTime() << " [WARN] " << message << std::endl;
}

inline void logWarning(const char* message)
{
    std::cout << getFormattedTime() << " [WARN] " << message << std::endl;
}

inline void logError(std::string message)
{
    std::cout << getFormattedTime() << " [ERROR] " << message << std::endl;
}

inline void logError(const char* message)
{
    std::cout << getFormattedTime() << " [ERROR] " << message << std::endl;
}

#endif