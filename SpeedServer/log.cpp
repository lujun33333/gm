#include "log.h"
#include <filesystem>
#include <fstream>


void spdlog_init()
{
	try {
		auto now = std::chrono::system_clock::now();
		auto in_time_t = std::chrono::system_clock::to_time_t(now);
		std::tm bt{};

		localtime_s(&bt, &in_time_t);

		std::ostringstream oss;
		oss << std::put_time(&bt, "Logs/%Y%m%d%H%M%S_log.txt");
		std::string sFileName = oss.str();

		// 创建日志目录
		try {
			std::filesystem::create_directory("Logs");
		}
		catch (const std::exception& e) {
			spdlog::error("无法创建日志目录: {} (函数: {}, 文件: {}, 行: {})",
				e.what(), __func__, __FILE__, __LINE__);
			throw;
		}

		// 添加日志文件轮转功能
		auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
			sFileName, 1024 * 1024 * 50, 10);

		spdlog::drop_all();
		spdlog::init_thread_pool(8192, 4);

		auto myLogger = std::make_shared<spdlog::logger>("SpeedServer_logger", rotating_sink);

		myLogger->set_pattern("[%Y-%m-%d %H:%M:%S.%e][%l][%s:%#][%!] %v");
		myLogger->set_level(spdlog::level::info);

		spdlog::set_default_logger(myLogger);

		myLogger->info("[初始化日志库]");
		myLogger->flush();

	}
	catch (const std::exception& e) {
		spdlog::error("spdlog 初始化失败: {} (函数: {}, 文件: {}, 行: {})",
			e.what(), __func__, __FILE__, __LINE__);
		std::ofstream errorLogFile("Logs/spdlog_error.log", std::ios::app);
		if (errorLogFile.is_open()) {
			errorLogFile << "spdlog 初始化失败: " << e.what()
				<< " (函数: " << __func__
				<< ", 文件: " << __FILE__
				<< ", 行: " << __LINE__ << ")" << std::endl;
		}
		throw;  // 重新抛出异常
	}
	catch (...) {
		spdlog::error("spdlog 初始化发生未知异常 (函数: {}, 文件: {}, 行: {})",
			__func__, __FILE__, __LINE__);
		std::ofstream errorLogFile("Logs/spdlog_error.log", std::ios::app);
		if (errorLogFile.is_open()) {
			errorLogFile << "spdlog 初始化发生未知异常 (函数: " << __func__
				<< ", 文件: " << __FILE__
				<< ", 行: " << __LINE__ << ")" << std::endl;
		}
		throw;  // 重新抛出异常
	}
}
