#include "DBConnection.h"

DBConnection::DBConnection(ConnectionPool& externalPool) : connection(nullptr), pool(externalPool) {
	// 在构造函数中获取数据库连接
	try {
		connection = pool.getConnection();
	}
	catch (const std::exception& e) {
		printf(e.what());
		printf("DBConnection Error\n");
	}
}

DBConnection::~DBConnection()
{
	if (connection) {
		try {
			pool.returnConnection(connection);
		}
		catch (const std::exception& e) {
			printf(e.what());
			printf("~DBConnection Error\n");
		}

	}
}

sql::Connection* DBConnection::getConnection() const
{
	return connection.get();
}
