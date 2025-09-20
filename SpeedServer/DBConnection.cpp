#include "DBConnection.h"

DBConnection::DBConnection(ConnectionPool& externalPool) : connection(nullptr), pool(externalPool) {
	// �ڹ��캯���л�ȡ���ݿ�����
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
