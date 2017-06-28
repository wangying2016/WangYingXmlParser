// 解析模块的测试主程序
#include "./WangYingXmlParser/WangYingXmlParser/WangYingXmlParser.h"
#include <iostream>
#include <cstdlib>

int main()
{
	CWangYingXmlParser wangYingXmlParser;
	std::string strXml = "<name sex=\"male\">wang ying</name>";
	std::cout << "xml:" << strXml << std::endl;
	wangYingXmlParser.ParseXml(strXml);

	system("pause");
	return 0;
}
