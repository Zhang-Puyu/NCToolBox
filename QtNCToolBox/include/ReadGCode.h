#pragma once

#include <QObject>

#include "AbstractReader.h"

namespace NC {
namespace Toolpath {

	class ReadGCode : public AbstractrReader
	{
		Q_OBJECT

	public:
		ReadGCode(QObject* parent = nullptr);
		~ReadGCode();

		// 宏定义
		static const int A = 5;
		static const int B = 6;
		static const int C = 7;

		/// @brief 表头信息
		virtual QString head() const override { return "x,y,z,f,s,a,b,c"; };

protected:
	/// @brief 解析一行代码
	virtual void parseLine(const QString& line, Eigen::VectorXf& point) override;

	/// @brief 判断两个刀位点是否相同
	virtual bool areSame(const Eigen::VectorXf& p1, const Eigen::VectorXf& p2) const override;

	double m_feedRate = -1;
	double m_spindleSpeed = -1;

	bool m_rapidMark = false;
};


} // namespace Toolpath
} // namespace NC

