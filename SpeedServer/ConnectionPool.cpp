#include "ConnectionPool.h"
#include <spdlog/spdlog.h>



ConnectionPool::ConnectionPool() {
}


// 初始化函数的实现
void ConnectionPool::initialize(size_t poolSize, const std::string& host, const std::string& user, const std::string& password, const std::string& database) {
	try {
		// 将参数赋值给成员变量
		this->poolSize = poolSize;
		this->host = host;
		this->user = user;
		this->password = password;
		this->database = database;
		printf("正在初始化Mysql连接池 poolSize：%zu, host：%s, user：%s, password：%s, database：%s\n", poolSize, host.c_str(), user.c_str(), password.c_str(), database.c_str());
		// 进行连接池的初始化
		initializePool();

		// 启动检查连接活跃性的线程
		checkThread = std::thread(&ConnectionPool::checkConnectionActivity, this);
	}
	catch (std::exception& e) {
		// 错误处理
		std::cerr << e.what() << "ConnectionPool Error" << std::endl;
		//throw;关闭
	}
}


ConnectionPool::~ConnectionPool()
{
	stopFlag = true;
	cv.notify_all();
	if (checkThread.joinable()) {
		checkThread.join();
	}
}

void ConnectionPool::initializePool() {
	auto driver = sql::mysql::get_mysql_driver_instance();
	for (size_t i = 0; i < poolSize; ++i) {
		try {
			auto conn = std::shared_ptr<sql::Connection>(driver->connect("tcp://" + host + ":3306/" + database, user, password));
			if (conn) {
				connections.push_back(conn);          // 保存到连接池
				availableConnections.push(conn);     // 保存到可用队列
			}
		}
		catch (std::exception& e) {
			std::cerr << e.what() << "initializePool Error" << std::endl;
			//throw;关闭
		}
	}
}



int ConnectionPool::GetAvailableConnections() {
	std::lock_guard<std::mutex> lock(mutex);
	return availableConnections.size();
}

int ConnectionPool::GetConnectionsTimes() {
	return connectionsTime.size();
}

void ConnectionPool::ReleaseConnections() {
	try
	{
		auto currentTime = std::chrono::steady_clock::now();
		std::lock_guard<std::mutex> lock(mutex);
		for (auto it = connectionsTime.begin(); it != connectionsTime.end();) {
			if (std::chrono::duration_cast<std::chrono::seconds>(currentTime - it->second).count() >= 120) {
				printf("Connection timeout, removing...\n");
				it->first->setAutoCommit(true);
				availableConnections.push(it->first); // 归还连接
				it = connectionsTime.erase(it); // 删除
			}
			else {
				++it;
			}
		}

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "ReleaseConnections Error" << std::endl;
		//throw;关闭
	}

}



std::shared_ptr<sql::Connection> ConnectionPool::getConnection() {
	try
	{
		std::unique_lock<std::mutex> lock(mutex);
		while (availableConnections.empty()) {
			//printf(sql.c_str());
			//添加日志输出
			auto ServerLogger = spdlog::get("SpeedServer_logger");//获取日志库实例
			ServerLogger->info("[数据库缺失连接:{:s}]", database);
			ServerLogger->flush();

			auto driver = sql::mysql::get_mysql_driver_instance();
			auto conn =  std::shared_ptr<sql::Connection>(driver->connect("tcp://" + host + ":3306/" + database, user, password));
			if (conn) {
				connections.push_back(conn);
				availableConnections.push(conn);
			}
		}
		auto conn = availableConnections.front();

		auto currentTime = std::chrono::steady_clock::now();
		connectionsTime[conn] = currentTime; // 记录时间

		acquiredConnections.insert(conn);
		availableConnections.pop();
		return conn;
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "getConnection Error" << std::endl;
		throw;
	}
}



void ConnectionPool::returnConnection(std::shared_ptr<sql::Connection> conn) {

	try
	{
		std::lock_guard<std::mutex> lock(mutex);
		if (connectionsTime.find(conn) != connectionsTime.end()) {
			connectionsTime.erase(conn); // 移除连接的取出时间
		}

		if (acquiredConnections.find(conn) == acquiredConnections.end()) {
			std::cerr  << "连接已经存在" << std::endl;
			return;
		}
		acquiredConnections.erase(conn);
		conn->setAutoCommit(true);
		availableConnections.push(conn); // 归还连接

		condition.notify_one(); // 通知其他等待的线程
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "returnConnection Error" << std::endl;
		////throw;关闭
	}
}

void ConnectionPool::checkConnectionActivity() {
	while (true) {
		//std::this_thread::sleep_for(std::chrono::hours(6)); // 每隔5小时检查一次连接活跃性
		//std::this_thread::sleep_for(std::chrono::seconds(3)); // 每隔5小时检查一次连接活跃性

		{
			std::unique_lock<std::mutex> lock(mutex);
			if (cv.wait_for(lock, std::chrono::hours(6), [this] { return stopFlag.load(); })) {
				break;
			}
			// 创建一个临时队列来存储活跃的连接
			std::queue<std::shared_ptr<sql::Connection>> activeConnections;
			// 遍历所有连接，进行查询
			while (!availableConnections.empty()) {
				auto conn = availableConnections.front();
				availableConnections.pop();

				// 执行一个简单的查询来验证连接是否有效
				if (conn->isValid()) {
					//printf("Connection is active.\n");
					// 将连接重新放入可用连接队列中
					activeConnections.push(conn);
				}
				else {
					// 连接无效，关闭连接并释放资源
					printf("Connection is inactive, removing...\n");
					conn->close();
				}
			}

			// 将活跃的连接重新放回可用连接队列中
			while (!activeConnections.empty()) {
				availableConnections.push(activeConnections.front());
				activeConnections.pop();
			}
		}
	}
}


void ConnectionPool::closePool() {
	for (auto& conn : connections) {
		try {
			if (conn) {
				conn->commit();
				conn->close(); // close connection
			}
		}
		catch (std::exception& e) {
			std::cerr << "Error closing connection: " << e.what() << std::endl;
		}
	}
	connections.clear();
	std::cout << "Connection pool closed." << std::endl;
}


