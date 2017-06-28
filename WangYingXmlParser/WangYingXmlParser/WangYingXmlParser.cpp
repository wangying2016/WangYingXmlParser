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
	// 解析：项目
	_ParseItem(strXml);

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

// 解析：解析项目
bool CWangYingXmlParser::_ParseItem(std::string strXml)
{
	// TODO:解析文本拿到项目和属性
	std::string s = strXml;
	std::smatch m;
	std::regex e("[<>/]");
	std::string result;
	while (std::regex_search(s, m, e)) {
		s = m.suffix().str();
		std::string prefix= m.prefix().str();
	}
	return true;
}