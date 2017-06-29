// 解析模块的测试主程序
#include "./WangYingXmlParser/WangYingXmlParser/WangYingXmlParser.h"
#include <iostream>
#include <cstdlib>

int main()
{
	CWangYingXmlParser wangYingXmlParser;
	// 解析
	std::string strXml = "<name sex=\"male\">wang ying</name><age>12</age><love action=\"coding\"/>";
	std::cout << "得到的 xml 文本:" << std::endl << strXml << std::endl << std::endl;
	wangYingXmlParser.ParseXml(strXml);
	// 获取
	WangYingXmlParser::CDocument xmlDocument;
	wangYingXmlParser.GetXmlDocument(&xmlDocument);
	int nItemIndex = 0;
	int nAttriIndex = 0;
	std::cout << "解析后得到的信息：" << std::endl;
	for (auto item : xmlDocument.items) {
		std::cout << "item" << nItemIndex++ << "			" << item.name << std::endl;
		nAttriIndex = 0;
		for (auto atrribute : item.attributes) {
			std::cout << "attribute" << nAttriIndex++ << "				" \
				<< atrribute.name << "		" << atrribute.value <<std::endl;
		}
	}
	std::cout << std::endl;
	system("pause");
	return 0;
}
