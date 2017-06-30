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

public:// 外部接口
	
	// 外部接口：解析，有待多种类型参数适配
	bool ParseXml(std::string strXml);

	// 外部接口：获得出错信息
	bool GetErrorMessage(std::string *pstrErrorMessage);

	// 外部接口：获取当前 xml 的数据
	bool GetXmlDocument(WangYingXmlParser::CDocument *pXmlDocument);

	// 外部接口：将当前 xml 的数据打印到标准输出
	void PrintXmlData(WangYingXmlParser::CDocument xmlDocument);

protected:// 检查相关

	// 检查：输入文本是否合法
	bool _CheckXmlValid(std::string strXml);

	// 检查：标签对对称性检查
	bool _CheckTabSymmetry(std::string strXml, bool *pbIsSymmetry);

protected:// 解析相关

	// 解析：解析文本，将 xml 文本解析成一个一个项目
	bool _ParseSource(std::string strXml);

	// 解析：解析一个项目
	bool _ParseOneItem(std::string strOneItemXml, WangYingXmlParser::CItem *pItem);

	// 解析：解析一个项目的子项目或者 text 属性（在项目的标签对之间的认为是子项目或者 text 属性）
	bool _ParseSubItem(std::string strOneItemXml, WangYingXmlParser::CItem *pItem);

	// 解析：解析一个属性
	bool _ParseOneAttribute(std::string strOneAttribute, WangYingXmlParser::CAttribute *pAttribute);

protected:// 打印相关

	// 打印：打印一个 Item
	void _PrintOneItem(WangYingXmlParser::CItem item, int nSubLevel);

private:
	WangYingXmlParser::CDocument m_cDocument;	// 缓存：解析总数据结构
	std::string m_strErroMessage;				// 缓存：解析失败的错误消息
};
