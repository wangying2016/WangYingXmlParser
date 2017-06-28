// 解析模块
#pragma once
#include <string>
#include <vector>

// 自定义数据结构
namespace WangYingXmlParser{
	// 属性
	struct CAttribute {
		std::string name;
		std::string value;
	};

	// 项目
	struct CItem {
		std::string name;
		std::vector<CAttribute> attributes;
	};

	// 文档
	struct CDocument {
		std::vector<CItem> items;
	};
}

class CWangYingXmlParser {
public:
	CWangYingXmlParser();
	virtual ~CWangYingXmlParser();

public:// 解析相关，有待完善更多类型的输入
	bool ParseXml(std::string strXml);

protected:
	// 检查：输入文本是否合法
	bool _IsXmlValid(std::string strXml, std::string *pstrErrorMsg);

	// 解析：解析项目
	bool _ParseItem(std::string strXml);

private:
	WangYingXmlParser::CDocument m_cDocument;	// 缓存：解析总数据结构
};
