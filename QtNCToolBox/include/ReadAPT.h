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

		// �궨��
		static const int I = 5;
		static const int J = 6;
		static const int K = 7;

		/// @brief ��ͷ��Ϣ
		virtual QString head() const override { return "x,y,z,f,s,i,j,k"; };

	protected:
		/// @brief ����һ�д���
		virtual void parseLine(const QString& line, Eigen::VectorXf& point) override;

		/// @brief ����������Ϣ
		virtual bool parsePostion(const QString& line, Eigen::VectorXf& point);
		/// @brief ���������ٶ�
		virtual bool parseFeedRate(const QString& line, Eigen::VectorXf& point);
		/// @brief ��������ת��
		virtual bool parseSpindleSpeed(const QString& line, Eigen::VectorXf& point);

		/// @brief �ж�������λ���Ƿ���ͬ
		virtual bool areSame(const Eigen::VectorXf& p1, const Eigen::VectorXf& p2) const override;

		/// @brief ����
		double m_feedRate = 0;
		/// @brief ת��
		double m_spindleSpeed = 0;

		bool m_rapidMark = false;
	};


} // namespace Toolpath
} // namespace NC

