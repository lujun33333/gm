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
    std::shared_ptr<sql::Connection> getConnection(); // ��ȡ����
    void returnConnection(std::shared_ptr<sql::Connection> conn); // �黹����
    void closePool();
    int GetAvailableConnections();
    int GetConnectionsTimes();
    void ReleaseConnections();
    void checkConnectionActivity();
    void initialize(size_t poolSize, const std::string& host, const std::string& user, const std::string& password, const std::string& database);
private:
    size_t poolSize; // ���ӳصĴ�С
    std::string host;
    std::string user;
    std::string password;
    std::string database;
    std::vector<std::shared_ptr<sql::Connection>> connections; // ��������
    std::queue<std::shared_ptr<sql::Connection>> availableConnections; // �������Ӷ���
    std::unordered_set<std::shared_ptr<sql::Connection>> acquiredConnections;
    std::mutex mutex; // �������ӳص���
    std::condition_variable condition; // ���ڵȴ��������ӵ���������
    std::unordered_map<std::shared_ptr<sql::Connection>, std::chrono::steady_clock::time_point> connectionsTime;
    void initializePool(); // ��ʼ�����ӳ�

	std::thread checkThread;
	std::atomic<bool> stopFlag = false; // ��־λ������֪ͨ�߳��˳�
	std::condition_variable cv; // �������������ڻ����߳�

};
#endif // CONNECTION_POOL_H
