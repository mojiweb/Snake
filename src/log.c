#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "log.h"

char *GetLogPriorityString(int priority);

void Log(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    LogMessage(DEFAULT_LOG_PRIORITY, fmt, ap);
    va_end(ap);
}

void LogDebug(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    LogMessage(LOG_PRIORITY_DEBUG, fmt, ap);
    va_end(ap);
}

void LogInfo(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    LogMessage(LOG_PRIORITY_INFO, fmt, ap);
    va_end(ap);
}

void LogWarn(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    LogMessage(LOG_PRIORITY_WARN, fmt, ap);
    va_end(ap);
}

void LogError(const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    LogMessage(LOG_PRIORITY_ERROR, fmt, ap);
    va_end(ap);
}

char *GetLogPriorityString(int priority)
{
    switch (priority)
    {
    case LOG_PRIORITY_ERROR:
        return "ERROR";
    case LOG_PRIORITY_DEBUG:
        return "DEBUG";
    case LOG_PRIORITY_WARN:
        return "WARN";
    case LOG_PRIORITY_INFO:
    default:
        return "INFO";
    }
}

void LogMessage(int priority, const char *fmt, ...)
{
    va_list ap;
    int len = 0;
    size_t size = 0;
    char *message = NULL;

    va_start(ap, fmt);
    len = vsnprintf(message, 0, fmt, ap);
    va_end(ap);

    if (len < 0)
        return;

    size = (size_t)len + 1;

    message = malloc(size);
    if (message == NULL)
        return;

    va_start(ap, fmt);
    len = vsnprintf(message, size, fmt, ap);
    va_end(ap);

    if (len >= 0)
    {
        if (priority >= LOG_PRIORITY_ERROR)
        {
            fprintf(stderr, "[%s] %s\n", GetLogPriorityString(priority), message);
        }
        else
        {
            fprintf(stdout, "[%s] %s\n", GetLogPriorityString(priority), message);
        }
    }

    free(message);
}