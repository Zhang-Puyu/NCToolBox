#pragma once

#include "qstring.h"
#include "qregexp.h"
#include "qregularexpression.h"

namespace String
{
	/// @brief 判断字符串是否包含中文字符
	static bool hasChinese(const QString& str) 
	{
		return str.contains(QRegExp("[\\x4e00-\\x9fa5]+"));
	}

	/// @brief 判断字符串是否为表头（只包含数字、小数点、加号、减号、分隔符）
	static bool isHead(QString str, const QString& sep = ",")
	{
		// 使用正则表达式检查是否只包含数字、小数点、加号、减号
		QRegularExpression regex("^[0-9\\+\\-\\.]*$");
		QRegularExpressionMatch match = regex.match(str.remove(sep));

		return !match.hasMatch();
	}
}