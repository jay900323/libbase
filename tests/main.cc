#include "stdafx.h"
#include <windows.h>
#include "zx_json.h"
#include "gtest/gtest.h"
#include "zx_base.h"
#include "zx_dir.h"
#include "zx_logger.h"
#include "zx_zip.h"

// using namespace testing;


TEST(TestDIR,mkdir)
{
	EXPECT_EQ(z_mkdir_recursive("D:\\MyProject\\base\\libs\\12\\22"), 0);
};

TEST(TestDIR, mkdir2)
{
	EXPECT_EQ(z_mkdir_recursive("D:\\MyProject\\base\\libs\\12\\22"), 0);
};

TEST(TestDIR, removedir)
{
	EXPECT_EQ(z_remove_dir("D:\\MyProject\\base\\libs\\12"), 0);
};

TEST(TestDIR, diraccess)
{
	EXPECT_EQ(z_dir_access("D:\\MyProject\\base\\libs\\12"), 0);
};

TEST(TestDIR, deldir)
{
	EXPECT_EQ(z_delete_dir_recursive("E:\\help"), 0);
};

TEST(TestJSON, parse_json)
{
	int iValue;
	double fTimeout;
	std::string strValue;
	ZxJson oJson("{\"refresh_interval\":60,"
		"\"test_float\":[18.0, 10.0, 5.0],"
		"\"timeout\":12.5,"
		"\"dynamic_loading\":["
		"{"
		"\"so_path\":\"plugins/User.so\", \"load\":false, \"version\":1,"
		"\"cmd\":["
		"{\"cmd\":2001, \"class\":\"neb::CmdUserLogin\"},"
		"{\"cmd\":2003, \"class\":\"neb::CmdUserLogout\"}"
		"],"
		"\"module\":["
		"{\"path\":\"im/user/login\", \"class\":\"neb::ModuleLogin\"},"
		"{\"path\":\"im/user/logout\", \"class\":\"neb::ModuleLogout\"}"
		"]"
		"},"
		"{"
		"\"so_path\":\"plugins/ChatMsg.so\", \"load\":false, \"version\":1,"
		"\"cmd\":["
		"{\"cmd\":2001, \"class\":\"neb::CmdChat\"}"
		"],"
		"\"module\":[]"
		"}"
		"]"
		"}");
	std::cout << oJson.ToString() << std::endl;
	std::cout << "-------------------------------------------------------------------" << std::endl;
	std::cout << oJson["dynamic_loading"].GetArraySize() << std::endl;
	std::cout << oJson["dynamic_loading"][0]["cmd"][1]("class") << std::endl;
	oJson["dynamic_loading"][0]["cmd"][0].Get("cmd", iValue);
	std::cout << "iValue = " << iValue << std::endl;
	oJson["dynamic_loading"][0]["cmd"][0].Replace("cmd", -2001);
	oJson["dynamic_loading"][0]["cmd"][0].Get("cmd", iValue);
	std::cout << "iValue = " << iValue << std::endl;
	oJson.Get("timeout", fTimeout);
	std::cout << "fTimeout = " << fTimeout << std::endl;
	oJson["dynamic_loading"][0]["module"][0].Get("path", strValue);
	std::cout << "strValue = " << strValue << std::endl;
	std::cout << "-------------------------------------------------------------------" << std::endl;
	oJson.AddEmptySubObject("depend");
	oJson["depend"].Add("nebula", "https://github.com/Bwar/Nebula");
	oJson["depend"].AddEmptySubArray("bootstrap");
	oJson["depend"]["bootstrap"].Add("BEACON");
	oJson["depend"]["bootstrap"].Add("LOGIC");
	oJson["depend"]["bootstrap"].Add("LOGGER");
	oJson["depend"]["bootstrap"].Add("INTERFACE");
	oJson["depend"]["bootstrap"].Add("ACCESS");
	std::cout << oJson.ToString() << std::endl;
	std::cout << "-------------------------------------------------------------------" << std::endl;
	std::cout << oJson.ToFormattedString() << std::endl;

	std::cout << "-------------------------------------------------------------------" << std::endl;
	ZxJson oCopyJson = oJson;
	if (oCopyJson == oJson)
	{
		std::cout << "json equal" << std::endl;
	}
	oCopyJson["depend"]["bootstrap"].Delete(1);
	oCopyJson["depend"].Replace("nebula", "https://github.com/Bwar/CJsonObject");
	std::cout << oCopyJson.ToString() << std::endl;
	std::cout << "-------------------------key traverse------------------------------" << std::endl;
	std::string strTraversing;
	while (oJson["dynamic_loading"][0].GetKey(strTraversing))
	{
		std::cout << "traversing:  " << strTraversing << std::endl;
	}
	std::cout << "---------------add a new key, then key traverse---------------------" << std::endl;
	oJson["dynamic_loading"][0].Add("new_key", "new_value");
	while (oJson["dynamic_loading"][0].GetKey(strTraversing))
	{
		std::cout << "traversing:  " << strTraversing << std::endl;
	}

	std::cout << oJson["test_float"].GetArraySize() << std::endl;
	float fTestValue = 0.0;
	for (int i = 0; i < oJson["test_float"].GetArraySize(); ++i)
	{
		oJson["test_float"].Get(i, fTestValue);
		std::cout << fTestValue << std::endl;
	}
	oJson.AddNull("null_value");
	std::cout << oJson.IsNull("test_float") << "\t" << oJson.IsNull("null_value") << std::endl;
	oJson["test_float"].AddNull();
	std::cout << oJson.ToString() << std::endl;
	EXPECT_EQ(z_delete_dir_recursive("E:\\help"), 0);
};

TEST(logging, log)
{
	logging::setPath("1.txt");
	logging::set_log_appender(FILE_APPENDER | CONSOLE_APPENDER);
	logging::setLogLevel(severity_level::LEVEL_DEBUG);
	logging::setLogLineNum(true);
	LOG_DEBUG("dasdsadasczx");
	LOG_WARN("ghhgfhfg");
	LOG_INFO("asdfghj");
	LOG_ERROR("231231");
	LOG_FATAL("231231");
	std::shared_ptr<Logger> test1 = logging::getLogger("test1");
	test1->setPath("2.txt");
	VLOG_INFO(test1, "ghhgfhfg");
	VLOG_DEBUG(test1, "debugghhgfhfg");
}

// TEST(zip, compress)
// {
// 	ZxZip zxip;
// 	zxip.compress_zip_package("D:\\MyProject\\base\\libs", "D:\\MyProject\\base\\ttt.zip");
// 	zxip.uncompress_zip_package("D:\\MyProject\\base\\ttt.zip","D:\\MyProject\\base\\1" );
// }

#define _CRT_SECURE_NO_WARNINGS 1
int main(int argc, char **argv)
{

	 testing::InitGoogleTest(&argc, argv);
	 return RUN_ALL_TESTS();

}


