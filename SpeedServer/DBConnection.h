#include "jdbc/mysql_connection.h"
#include "jdbc/mysql_driver.h"
#include "jdbc/cppconn/statement.h"
#include "jdbc/cppconn/prepared_statement.h"
#include "ConnectionPool.h"

class DBConnection {

public:
    DBConnection(ConnectionPool& externalPool);
    ~DBConnection();
    sql::Connection* getConnection() const;
private:
	std::shared_ptr<sql::Connection> connection;
	ConnectionPool& pool;
     
};