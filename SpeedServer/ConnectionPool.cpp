#include "ConnectionPool.h"
#include <spdlog/spdlog.h>



ConnectionPool::ConnectionPool() {
}


// ��ʼ��������ʵ��
void ConnectionPool::initialize(size_t poolSize, const std::string& host, const std::string& user, const std::string& password, const std::string& database) {
	try {
		// ��������ֵ����Ա����
		this->poolSize = poolSize;
		this->host = host;
		this->user = user;
		this->password = password;
		this->database = database;
		printf("���ڳ�ʼ��Mysql���ӳ� poolSize��%zu, host��%s, user��%s, password��%s, database��%s\n", poolSize, host.c_str(), user.c_str(), password.c_str(), database.c_str());
		// �������ӳصĳ�ʼ��
		initializePool();

		// ����������ӻ�Ծ�Ե��߳�
		checkThread = std::thread(&ConnectionPool::checkConnectionActivity, this);
	}
	catch (std::exception& e) {
		// ������
		std::cerr << e.what() << "ConnectionPool Error" << std::endl;
		//throw;�ر�
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
				connections.push_back(conn);          // ���浽���ӳ�
				availableConnections.push(conn);     // ���浽���ö���
			}
		}
		catch (std::exception& e) {
			std::cerr << e.what() << "initializePool Error" << std::endl;
			//throw;�ر�
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
				availableConnections.push(it->first); // �黹����
				it = connectionsTime.erase(it); // ɾ��
			}
			else {
				++it;
			}
		}

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "ReleaseConnections Error" << std::endl;
		//throw;�ر�
	}

}



std::shared_ptr<sql::Connection> ConnectionPool::getConnection() {
	try
	{
		std::unique_lock<std::mutex> lock(mutex);
		while (availableConnections.empty()) {
			//printf(sql.c_str());
			//�����־���
			auto ServerLogger = spdlog::get("SpeedServer_logger");//��ȡ��־��ʵ��
			ServerLogger->info("[���ݿ�ȱʧ����:{:s}]", database);
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
		connectionsTime[conn] = currentTime; // ��¼ʱ��

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
			connectionsTime.erase(conn); // �Ƴ����ӵ�ȡ��ʱ��
		}

		if (acquiredConnections.find(conn) == acquiredConnections.end()) {
			std::cerr  << "�����Ѿ�����" << std::endl;
			return;
		}
		acquiredConnections.erase(conn);
		conn->setAutoCommit(true);
		availableConnections.push(conn); // �黹����

		condition.notify_one(); // ֪ͨ�����ȴ����߳�
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << "returnConnection Error" << std::endl;
		////throw;�ر�
	}
}

void ConnectionPool::checkConnectionActivity() {
	while (true) {
		//std::this_thread::sleep_for(std::chrono::hours(6)); // ÿ��5Сʱ���һ�����ӻ�Ծ��
		//std::this_thread::sleep_for(std::chrono::seconds(3)); // ÿ��5Сʱ���һ�����ӻ�Ծ��

		{
			std::unique_lock<std::mutex> lock(mutex);
			if (cv.wait_for(lock, std::chrono::hours(6), [this] { return stopFlag.load(); })) {
				break;
			}
			// ����һ����ʱ�������洢��Ծ������
			std::queue<std::shared_ptr<sql::Connection>> activeConnections;
			// �����������ӣ����в�ѯ
			while (!availableConnections.empty()) {
				auto conn = availableConnections.front();
				availableConnections.pop();

				// ִ��һ���򵥵Ĳ�ѯ����֤�����Ƿ���Ч
				if (conn->isValid()) {
					//printf("Connection is active.\n");
					// ���������·���������Ӷ�����
					activeConnections.push(conn);
				}
				else {
					// ������Ч���ر����Ӳ��ͷ���Դ
					printf("Connection is inactive, removing...\n");
					conn->close();
				}
			}

			// ����Ծ���������·Żؿ������Ӷ�����
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


