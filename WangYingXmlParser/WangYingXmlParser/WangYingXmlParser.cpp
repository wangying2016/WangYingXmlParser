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
// 匹配标签头
// <\w+[^\n<]*>
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
	}
	// 获得：属性
	s = strOneItemXml;
	std::smatch tabHeadMatch;
	std::regex regexTabHead("<\\w+[^\\n<]*>");
	if (std::regex_search(s, tabHeadMatch, regexTabHead)) {
		std::string strTabHead = tabHeadMatch.str();
		std::smatch attributeMatch;
		std::regex regexAtrribute("\\b\\w+=\"\\w+\"");
		while (std::regex_search(strTabHead, attributeMatch, regexAtrribute)) {
			WangYingXmlParser::CAttribute newAtrribute;
			_ParseOneAttribute(attributeMatch.str(), &newAtrribute);
			newItem.attributes.push_back(newAtrribute);
			strTabHead = attributeMatch.suffix().str();
		}
	}
	
	// 特殊属性：双标签对中认作 text 属性值
	// 子项目往往也出现在 text 属性值中，此时需要根据 text 是否
	// 匹配得出新的 Item 将其设置为子 Item 还是 text 属性值
	s = strOneItemXml;
	std::regex regexText(">.*<");
	if (std::regex_search(s, m, regexText)) {
		// --获取：中间的文本
		std::string strTempText = m.str();
		strTempText.erase(strTempText.end() - 1, strTempText.end());
		strTempText.erase(0, 1);
		// --解析：解析中间文本，视匹配结果将其视作属性或者子项目
		_ParseSubItem(strTempText, &newItem);
	}
	*pItem = newItem;
	return true;
}

// 解析：解析一个项目的子项目或者 text 属性（在项目的标签对之间的认为是子项目或者 text 属性）
bool CWangYingXmlParser::_ParseSubItem(std::string strOneItemXml, WangYingXmlParser::CItem *pItem)
{
	if (pItem == nullptr) return false;
	std::string s = strOneItemXml;
	std::smatch m;
	std::regex regexItem("<(\\w+).*>.*</\\1>|<\\w+[^/>]*/>");
	// 匹配：识别成项目，递归调用 _ParseOneItem 匹配子项目
	if (std::regex_search(s, m, regexItem)) {
		WangYingXmlParser::CItem newSubItem;
		_ParseOneItem(m.str(), &newSubItem);
		pItem->subitems.push_back(newSubItem);
		s = m.suffix().str();
		while (std::regex_search(s, m, regexItem)) {
			WangYingXmlParser::CItem newSubItem;
			std::string strss = m.str();
			_ParseOneItem(m.str(), &newSubItem);
			pItem->subitems.push_back(newSubItem);
			s = m.suffix().str();
		}
	} 
	// 匹配：识别成 text 属性
	else {
		WangYingXmlParser::CAttribute textAtrribute;
		textAtrribute.name = "text";
		textAtrribute.value = s;
		pItem->attributes.push_back(textAtrribute);
	}
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

// 打印：将当前 xml 的数据打印到标准输出
void CWangYingXmlParser::PrintXmlData(WangYingXmlParser::CDocument xmlDocument)
{
	std::cout << "解析后得到的信息：" << std::endl;
	for (auto item : xmlDocument.items) {
		_PrintOneItem(item, 0);
	}
}

// 打印：打印一个 Item
void CWangYingXmlParser::_PrintOneItem(WangYingXmlParser::CItem item, int subLevel)
{
	std::string strTab;
	for (int i = 0; i < subLevel; ++i) strTab += "\t";
	std::cout << "Level_" << subLevel << std::endl;
	std::cout << strTab << item.name << ":" << std::endl;
	for (auto attribute : item.attributes) {
		std::cout << "\tattributes: " << attribute.name << "：" << attribute.value << std::endl;
	}
	subLevel++;
	for (auto subItem : item.subitems) {
		_PrintOneItem(subItem, subLevel);
	}
}