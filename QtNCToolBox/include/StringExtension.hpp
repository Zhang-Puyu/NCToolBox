#pragma once

#include "qstring.h"
#include "qregexp.h"
#include "qregularexpression.h"

namespace String
{
	/// @brief �ж��ַ����Ƿ���������ַ�
	static bool hasChinese(const QString& str) 
	{
		return str.contains(QRegExp("[\\x4e00-\\x9fa5]+"));
	}

	/// @brief �ж��ַ����Ƿ�Ϊ��ͷ��ֻ�������֡�С���㡢�Ӻš����š��ָ�����
	static bool isHead(QString str, const QString& sep = ",")
	{
		// ʹ��������ʽ����Ƿ�ֻ�������֡�С���㡢�Ӻš�����
		QRegularExpression regex("^[0-9\\+\\-\\.]*$");
		QRegularExpressionMatch match = regex.match(str.remove(sep));

		return !match.hasMatch();
	}
}