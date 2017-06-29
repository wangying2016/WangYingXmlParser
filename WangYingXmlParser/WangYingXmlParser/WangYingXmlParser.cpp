#include "WangYingXmlParser.h"
#include <string>
#include <iostream>
#include <stack>
#include <regex>

CWangYingXmlParser::CWangYingXmlParser()
{
	m_cDocument.items.clear();
}

CWangYingXmlParser::~CWangYingXmlParser()
{
	m_cDocument.items.clear();
}

bool CWangYingXmlParser::ParseXml(std::string strXml)
{
	// 检查：输入文本是否合法
	bool bIsXmlValid = false;
	std::string strErrorMsg;
	bIsXmlValid = _IsXmlValid(strXml, &strErrorMsg);
	if (!bIsXmlValid) {
		std::cout << strErrorMsg << std::endl;
		return false;
	}
	// 解析：xml 文本
	_ParseSource(strXml);

	return true;
}

// 检查：输入文本是否合法
bool CWangYingXmlParser::_IsXmlValid(std::string strXml, std::string *pstrErrorMsg)
{
	if (nullptr == pstrErrorMsg) return false;
	std::stack<char> stackBracket;	// 栈：尖括号
	std::stack<char> stackBackSlash;// 栈：反斜线
	bool bBlackSlashValid = true;
	std::string strErrorMsg;
	// 检查：括号匹配
	for (int i = 0; i < strXml.size(); ++i) {
		// --记录：当出现 < ，则直接压栈
		if ('<' == strXml[i]) stackBracket.push(strXml[i]);
		// --记录：当出现 > ，则判断栈是否为空、是否栈顶为 '<'，满足条件则出栈
		else if ('>' == strXml[i] && stackBracket.size() != 0 && stackBracket.top() == '<')
			stackBracket.pop();
		// --记录：当出现 / ，则判断栈顶是否为 <、左侧是否有 < 或者右侧是否有 >，
		// 满足条件压栈（计算项目个数），否则报错
		else if ('/' == strXml[i] && stackBracket.top() == '<' &&
			((i != 0 && '<' == strXml[i - 1]) || (i + 1 != strXml.size() && '>' == strXml[i + 1])))
			stackBackSlash.push(strXml[i]);
		else if ('/' == strXml[i]) {
			bBlackSlashValid = false;
			break;
		}
	}
	// 出错：反斜线不匹配
	if (!bBlackSlashValid)
		strErrorMsg = "parse error: black slash write error.";
	// 出错：尖括号不匹配
	if (stackBracket.size() != 0) {
		bBlackSlashValid = false;
		strErrorMsg = "parse error: bracket does not matched.";
	}
	*pstrErrorMsg = strErrorMsg;
	return bBlackSlashValid;
}

// 解析：解析文本，将 xml 文本解析成一个一个项目
// 匹配 <name sex="male">wang ying</name> 文本
// <\w+.*>.*</\1>
// 匹配 <name sex="male" /> 文本
// <\w+[^/>]*/>
// 这里按照以上两种格式匹配 xml 文本中的每一个 Item
// 这个函数将 xml 文本解析成一个一个的项目
bool CWangYingXmlParser::_ParseSource(std::string strXml)
{
	std::string s = strXml;
	std::smatch m;
	std::regex e("<(\\w+).*>.*</\\1>|<\\w+[^/>]*/>");
	while (std::regex_search(s, m, e)) {
		WangYingXmlParser::CItem newItem;
		_ParseOneItem(m.str(), &newItem);
		s = m.suffix().str();
		m_cDocument.items.push_back(newItem);
	}
	return true;
}

// 解析：解析一个项目
// 拿到项目的子项目列表、属性
// 匹配标签名 <name
// <\w+\b  
// 匹配属性 sex="male"
// \b\w+="\w+"
// 匹配属性 >wang ying<
// >.*<
bool CWangYingXmlParser::_ParseOneItem(std::string strOneItemXml, WangYingXmlParser::CItem *pItem)
{
	if (pItem == nullptr) return false;
	WangYingXmlParser::CItem newItem;
	// 获得：项目名称
	std::string s = strOneItemXml;
	std::smatch m;
	std::regex regexItemName("<\\w+\\b");
	if (std::regex_search(s, m, regexItemName)) {
		std::string strItemName = m.str();
		newItem.name = strItemName.erase(0, 1);
		s = m.suffix().str();
		while (std::regex_search(s, m, regexItemName)) {
			WangYingXmlParser::CItem subItem;
			_ParseOneItem(m.str(), &subItem);
			newItem.subitems.push_back(subItem);
			s = m.suffix().str();
		}
	}
	// 获得：属性
	s = strOneItemXml;
	std::regex regexAtrribute("\\b\\w+=\"\\w+\"");
	while (std::regex_search(s, m, regexAtrribute)) {
		WangYingXmlParser::CAttribute newAtrribute;
		_ParseOneAttribute(m.str(), &newAtrribute);
		newItem.attributes.push_back(newAtrribute);
		s = m.suffix().str();
	}
	// 特殊属性：双标签对中认作 text 属性值
	s = strOneItemXml;
	std::regex regexText(">.*<");
	if (std::regex_search(s, m, regexText)) {
		WangYingXmlParser::CAttribute textAtrribute;
		std::string strTempText = m.str();
		strTempText.erase(strTempText.end() - 1, strTempText.end());
		strTempText.erase(0, 1);
		textAtrribute.name = "text";
		textAtrribute.value = strTempText;
		newItem.attributes.push_back(textAtrribute);
	}
	*pItem = newItem;
	return true;
}

// 解析：解析一个属性
// 传递过来的文本类似于 sex="male"
// 匹配 sex=
// \b\w+=
// 匹配 “male”
// "\w*"
bool CWangYingXmlParser::_ParseOneAttribute(std::string strOneAttribute, WangYingXmlParser::CAttribute *pAttribute)
{
	if (pAttribute == nullptr) return false;
	WangYingXmlParser::CAttribute newAtrribute;
	// 获取：属性名称
	std::string s = strOneAttribute;
	std::smatch m;
	std::regex regexAtrributeName("\\b\\w+=");
	if (std::regex_search(s, m, regexAtrributeName)) {
		std::string strTempName = m.str();
		strTempName.erase(strTempName.end() - 1, strTempName.end());
		newAtrribute.name = strTempName;
	}
	// 获取：属性值
	s = strOneAttribute;
	std::regex regexAtrributeValue("\"\\w*\"");
	if (std::regex_search(s, m, regexAtrributeValue)) {
		std::string strTempValue = m.str();
		strTempValue.erase(strTempValue.end() - 1, strTempValue.end());
		strTempValue.erase(0, 1);
		newAtrribute.value = strTempValue;
	}
	*pAttribute = newAtrribute;
	return true;
}

// 获取相关，获取当前 xml 项目及其属性值
bool CWangYingXmlParser::GetXmlDocument(WangYingXmlParser::CDocument *pXmlDocument)
{
	if (pXmlDocument == nullptr) return false;
	*pXmlDocument = m_cDocument;
	return true;
}