# 出错处理函数：

```c
void __attribute__((noreturn))
__fatal_error(const char *expr, const char *msg, const char *file, int line,
              const char *func);
void __attribute__((noreturn))
error_shutdown(const char *line1, const char *line2, const char *line3,
               const char *line4);

#define ensure(expr, msg) \
  (((expr) == sectrue)    \
       ? (void)0          \
       : __fatal_error(#expr, msg, __FILE__, __LINE__, __func__))
```

# log

### my_log.c

```c

/**
日志打印示例。
使用：
mylog(DEBUG, "This is debug info\n");
结果：
[2018-07-22 23:37:27:172] [DEBUG] [main.cpp:5] This is debug info
默认打印当前时间（精确到毫秒）、文件名称、行号。
*/
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>
#include "my_log.h"

#define LOG_PATH "./bha_key_log.txt"

FILE *log_file=NULL;

#ifndef LOGLEVEL
#define LOGLEVEL DEBUG
#endif

static const char* s_loginfo[] = {
    [ERROR] = "ERROR",
    [WARN]  = "WARN",
    [INFO]  = "INFO",
    [DEBUG] = "DEBUG",
};

static void get_timestamp(char *buffer)
{
    time_t t;
    struct tm *p;
    struct timeval tv;
    int len;
    int millsec;

    t = time(NULL);
    p = localtime(&t);

    gettimeofday(&tv, NULL);
    millsec = (int)(tv.tv_usec / 1000);

    /* 时间格式：[2011-11-15 12:47:34:888] */
    len = snprintf(buffer, 32, "[%04d-%02d-%02d %02d:%02d:%02d:%03d] ",
        p->tm_year+1900, p->tm_mon+1,
        p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec, millsec);

    buffer[len] = '\0';
}

static void mylog1(const char* filename, int line, enum LogLevel level, const char* fmt, ...)
{
    if(level > LOGLEVEL)
        return;
	if(log_file==NULL)
		return;

    va_list arg_list;
    char buf[1024];
    memset(buf, 0, 1024);
    va_start(arg_list, fmt);
    vsnprintf(buf, 1024, fmt, arg_list);
    char time[32] = {0};

    const char* tmp = strrchr(filename, '/');
    if (!tmp) tmp = filename;
    else tmp++;
    get_timestamp(time);

    fprintf(log_file,"%s[%s] [%s:%d] %s\n", time, s_loginfo[level], tmp, line, buf);

    va_end(arg_list);
}

void myLogInit(void)
{
	log_file=fopen(LOG_PATH,"a+");
	if(log_file==NULL)
	{
		perror("open log file");
	}
}


int main(int argc,char *argv[])
{
	myLogInit();
	mylogDebug("mylogDebug");
	mylogInfo("mylogInfo");
	mylogWarn("mylogWarn");
	mylogError("mylogError");
}

```

### my_log.h

```c
#ifndef __MY_LOG_H_
#define __MY_LOG_H_

#ifdef __cplusplus 
extern "C" {
#endif

enum LogLevel
{
    ERROR = 1,
    WARN  = 2,
    INFO  = 3,
    DEBUG = 4,
};

static void mylog1(const char* filename, int line, enum LogLevel level, const char* fmt, ...) __attribute__((format(printf,4,5)));

#define mylogDebug(format, ...) mylog1(__FILE__, __LINE__, DEBUG, format, ## __VA_ARGS__)
#define mylogInfo(format, ...) mylog1(__FILE__, __LINE__, INFO, format, ## __VA_ARGS__)
#define mylogWarn(format, ...) mylog1(__FILE__, __LINE__, WARN, format, ## __VA_ARGS__)
#define mylogError(format, ...) mylog1(__FILE__, __LINE__, ERROR, format, ## __VA_ARGS__)

#ifdef __cplusplus 
};
#endif

#endif

```

