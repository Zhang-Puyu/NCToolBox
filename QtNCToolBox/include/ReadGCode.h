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

		// �궨��
		static const int A = 5;
		static const int B = 6;
		static const int C = 7;

		/// @brief ��ͷ��Ϣ
		virtual QString head() const override { return "x,y,z,f,s,a,b,c"; };

protected:
	/// @brief ����һ�д���
	virtual void parseLine(const QString& line, Eigen::VectorXf& point) override;

	/// @brief �ж�������λ���Ƿ���ͬ
	virtual bool areSame(const Eigen::VectorXf& p1, const Eigen::VectorXf& p2) const override;

	double m_feedRate = -1;
	double m_spindleSpeed = -1;

	bool m_rapidMark = false;
};


} // namespace Toolpath
} // namespace NC

