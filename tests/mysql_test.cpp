/* ************************************************************************
> File Name:     mysql_test.cpp
> Author:        niu0217
> Created Time:  Thu 27 Jun 2024 07:58:38 PM CST
> Description:   
 ************************************************************************/

// 学习MySQL地址：https://www.runoob.com/mysql/mysql-tutorial.html

#include <mysql/mysql.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <list>
#include <pthread.h>
#include <iostream>
#include <map>
using namespace std;

void test1()
{
    string url = "localhost";
    string user = "root";
    string password = "001125";
    string dbname = "NIUdb";
    int port = 3306;

    map<string, string> users;  // 保存user表中的信息

    MYSQL *con = NULL;
	con = mysql_init(con);
    if (con == NULL)
    {
        printf("MySQL Error");
        exit(1);
    }

    con = mysql_real_connect(con, url.c_str(), user.c_str(), password.c_str(), dbname.c_str(), port, NULL, 0);
    if (con == NULL)
    {
        printf("MySQL Error");
        exit(1);
    }

     //在user表中检索username，passwd数据，浏览器端输入
    if (mysql_query(con, "SELECT username,passwd FROM user"))
    {
        printf("SELECT error:%s\n", mysql_error(con));
    }

    //从表中检索完整的结果集
    MYSQL_RES *result = mysql_store_result(con);

    //返回结果集中的列数
    int num_fields = mysql_num_fields(result);

    //返回所有字段结构的数组
    MYSQL_FIELD *fields = mysql_fetch_fields(result);

    //从结果集中获取下一行，将对应的用户名和密码，存入map中
    while (MYSQL_ROW row = mysql_fetch_row(result))
    {
        string temp1(row[0]);
        string temp2(row[1]);
        users[temp1] = temp2;
    }

    for(auto &context : users)
    {
        cout << context.first << "  " << context.second << endl;
    }
    cout << endl;
}

int main()
{
    test1();
}