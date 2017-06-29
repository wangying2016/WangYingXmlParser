// 解析模块
#pragma once
#include <string>
#include <vector>

// 自定义数据结构
namespace WangYingXmlParser{
	// 属性
	struct CAttribute {
		std::string name;		// 属性名称
		std::string value;		// 属性值
	};

	// 项目
	struct CItem {
		std::string name;				// 项目名称
		std::vector<CItem> subitems;	// 子项目
		std::vector<CAttribute> attributes;
	};

	// 文档
	struct CDocument {
		std::vector<CItem> items;	// 项目列表
	};
}

class CWangYingXmlParser {
public:
	CWangYingXmlParser();
	virtual ~CWangYingXmlParser();

public:// 解析相关，有待完善更多类型的输入
	bool ParseXml(std::string strXml);

public:// 获取相关，获取当前 xml 项目及其属性值
	bool GetXmlDocument(WangYingXmlParser::CDocument *pXmlDocument);

protected:
	// 检查：输入文本是否合法
	bool _IsXmlValid(std::string strXml, std::string *pstrErrorMsg);

	// 解析：解析文本，将 xml 文本解析成一个一个项目
	bool _ParseSource(std::string strXml);

	// 解析：解析一个项目
	bool _ParseOneItem(std::string strOneItemXml, WangYingXmlParser::CItem *pItem);

	// 解析：解析一个属性
	bool _ParseOneAttribute(std::string strOneAttribute, WangYingXmlParser::CAttribute *pAttribute);

private:
	WangYingXmlParser::CDocument m_cDocument;	// 缓存：解析总数据结构
};
