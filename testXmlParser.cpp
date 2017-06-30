// 解析模块的测试主程序
#include "./WangYingXmlParser/WangYingXmlParser/WangYingXmlParser.h"
#include <iostream>
#include <cstdlib>

const std::string xml_0 = "<name sex=\"male\">wang ying</name><age>12</age><love action=\"coding\"/>";
const std::string xml_1 = "<person isStudent=\"false\">\
<name>wang ying</name>\
<age>12</age>\
<country isAsia=\"true\">china</country>\
</person>";
const std::string xml_2 = "<person isStudent=\"false\">\
<name>wang ying</name>\
<age>12</age>\
<country isAsia=\"true\">china</country>\
<hobby>\
<coding language=\"c++\"/>\
<sexygirl/>\
</hobby>\
</person>";

int main()
{
	CWangYingXmlParser wangYingXmlParser;
	// 解析
	std::string strXml = xml_2;
	std::cout << "得到的 xml 文本:" << std::endl << strXml << std::endl << std::endl;
	wangYingXmlParser.ParseXml(strXml);
	// 获取
	WangYingXmlParser::CDocument xmlDocument;
	wangYingXmlParser.GetXmlDocument(&xmlDocument);
	// 打印
	wangYingXmlParser.PrintXmlData(xmlDocument);
	system("pause");
	return 0;
}
