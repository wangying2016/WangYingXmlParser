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
const std::string xml_3 = "<person>\
<name>\
<ag/e>\
<country </country>\
</monkey>";

int main()
{
	CWangYingXmlParser wangYingXmlParser;
	// 解析
	// --修改这句代码可以适配各种 xml 语句
	std::string strXml = xml_2; 
	std::cout << "得到的 xml 文本:" << std::endl << strXml << std::endl << std::endl;
	if (wangYingXmlParser.ParseXml(strXml)) {
		// --获取
		WangYingXmlParser::CDocument xmlDocument;
		wangYingXmlParser.GetXmlDocument(&xmlDocument);
		// --打印
		wangYingXmlParser.PrintXmlData(xmlDocument);
	}
	// 解析失败
	else {
		std::string strErrorMessage;
		wangYingXmlParser.GetErrorMessage(&strErrorMessage);
		std::cout << "解析失败：" << std::endl;
		std::cout << strErrorMessage << std::endl;
	}
	
	system("pause");
	return 0;
}
