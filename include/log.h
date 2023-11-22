#ifndef __LOG_H
#define __LOG_H

enum LOG_PRIORITY {
    LOG_PRIORITY_DEBUG,
    LOG_PRIORITY_INFO,
    LOG_PRIORITY_WARN,
    LOG_PRIORITY_ERROR
};

#define DEFAULT_LOG_PRIORITY LOG_PRIORITY_INFO

#define MAX_LOG_MESSAGE_SIZE 256

void Log(const char *fmt, ...);
void LogDebug(const char *fmt, ...);
void LogInfo(const char *fmt, ...);
void LogWarn(const char *fmt, ...);
void LogError(const char *fmt, ...);
void LogMessage(int priority, const char *fmt, ...);

#endif // __LOG_H