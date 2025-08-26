#pragma once

#include <QObject>

#include "AbstractReader.h"

namespace NC {
namespace Toolpath {

	class ReadAPT : public AbstractrReader
	{
		Q_OBJECT

	public:
		ReadAPT(QObject* parent = nullptr);
		~ReadAPT();

		// 宏定义
		static const int I = 5;
		static const int J = 6;
		static const int K = 7;

		/// @brief 表头信息
		virtual QString head() const override { return "x,y,z,f,s,i,j,k"; };

	protected:
		/// @brief 解析一行代码
		virtual void parseLine(const QString& line, Eigen::VectorXf& point) override;

		/// @brief 解析坐标信息
		virtual bool parsePostion(const QString& line, Eigen::VectorXf& point);
		/// @brief 解析进给速度
		virtual bool parseFeedRate(const QString& line, Eigen::VectorXf& point);
		/// @brief 解析主轴转速
		virtual bool parseSpindleSpeed(const QString& line, Eigen::VectorXf& point);

		/// @brief 判断两个刀位点是否相同
		virtual bool areSame(const Eigen::VectorXf& p1, const Eigen::VectorXf& p2) const override;

		/// @brief 进给
		double m_feedRate = 0;
		/// @brief 转速
		double m_spindleSpeed = 0;

		bool m_rapidMark = false;
	};


} // namespace Toolpath
} // namespace NC

