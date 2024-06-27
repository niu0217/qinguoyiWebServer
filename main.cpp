#include "config.h"

int main(int argc, char *argv[])
{
    //需要修改的数据库信息,登录名,密码,库名
    string user = "root";
    string passwd = "001125";
    string databasename = "NIUdb";

    //命令行解析
    Config config;
    config.parse_arg(argc, argv);

    WebServer server;

    //初始化
    server.init(config.PORT, user, passwd, databasename, config.LOGWrite, 
                config.OPT_LINGER, config.TRIGMode,  config.sql_num,  config.thread_num, 
                config.close_log, config.actor_model);
    

    //日志
    server.log_write();  // 如果是异步的，则会创建一个线程用于后端写日志

    //数据库
    server.sql_pool();  // 默认创建了8个数据库连接供我们使用

    //线程池
    server.thread_pool();  // 创建了8个线程，用于处理http连接任务

    //到这里如果开启了异步，则有10个线程存在
    //1个主线程 + 1个用于后端写日志的线程 + 8个用于处理http连接任务的线程

    //触发模式
    server.trig_mode();

    //监听
    server.eventListen();

    //运行
    server.eventLoop();

    return 0;
}