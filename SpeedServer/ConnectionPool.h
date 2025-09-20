#ifndef CONNECTION_POOL_H
#define CONNECTION_POOL_H

#include <iostream>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include "jdbc/mysql_connection.h"
#include "jdbc/mysql_driver.h"
#include "jdbc/cppconn/statement.h"
#include "jdbc/cppconn/prepared_statement.h"
#include <unordered_set>
#include <unordered_map>

class ConnectionPool {
public:
    ConnectionPool();
    ~ConnectionPool();
    std::shared_ptr<sql::Connection> getConnection(); // 获取连接
    void returnConnection(std::shared_ptr<sql::Connection> conn); // 归还连接
    void closePool();
    int GetAvailableConnections();
    int GetConnectionsTimes();
    void ReleaseConnections();
    void checkConnectionActivity();
    void initialize(size_t poolSize, const std::string& host, const std::string& user, const std::string& password, const std::string& database);
private:
    size_t poolSize; // 连接池的大小
    std::string host;
    std::string user;
    std::string password;
    std::string database;
    std::vector<std::shared_ptr<sql::Connection>> connections; // 所有连接
    std::queue<std::shared_ptr<sql::Connection>> availableConnections; // 可用连接队列
    std::unordered_set<std::shared_ptr<sql::Connection>> acquiredConnections;
    std::mutex mutex; // 保护连接池的锁
    std::condition_variable condition; // 用于等待可用连接的条件变量
    std::unordered_map<std::shared_ptr<sql::Connection>, std::chrono::steady_clock::time_point> connectionsTime;
    void initializePool(); // 初始化连接池

	std::thread checkThread;
	std::atomic<bool> stopFlag = false; // 标志位，用于通知线程退出
	std::condition_variable cv; // 条件变量，用于唤醒线程

};
#endif // CONNECTION_POOL_H
