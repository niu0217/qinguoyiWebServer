/* ************************************************************************
> File Name:     log_test.cpp
> Author:        niu0217
> Created Time:  Wed 26 Jun 2024 07:06:57 PM CST
> Description:   
 ************************************************************************/

#include "../log/log.h"
#include <unistd.h>

int main()
{
    int m_close_log = 0;
    Log::get_instance()->init("./ServerLog", m_close_log, 2000, 800000, 800);
    char* text = "hello niu0217";
    LOG_INFO("%s", text);
    // sleep(10);
    return 0;
}