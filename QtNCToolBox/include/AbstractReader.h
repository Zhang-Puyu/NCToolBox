#pragma once

#include <QList>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QStringList>

#include <Eigen/Dense>

namespace NC {
namespace Toolpath  {

class AbstractrReader : public QObject
{
    Q_OBJECT

public:
    AbstractrReader(QObject* parent = nullptr);

    // �궨��
    static const int X = 0;
    static const int Y = 1;
    static const int Z = 2;
    static const int F = 3;
    static const int S = 4;

	/// @brief ��ͷ��Ϣ
    virtual QString head() const = 0;

    void setRapidFeedRate(double f) { m_rapidFeedRate = f; }

    /// @brief ��ȡ��λ����Ϣ
    /// @param fileName �ļ�·��
    /// @param codec �����ʽ
    void read(Eigen::MatrixXf& m_points,
        const QString& fileName, 
        const QString& codec = "UTF-8");

protected:
    /// @brief ����һ�д���
    virtual void parseLine(const QString& line, Eigen::VectorXf& point) = 0;

    /// @brief �ж�������λ���Ƿ���ͬ
    virtual bool areSame(const Eigen::VectorXf& p1, const Eigen::VectorXf& p2) const;

    /// @brief ���ٽ����ٶ�
    double m_rapidFeedRate = -1.0;

signals:
    /// @brief ��ȡ�ļ���ʼ
    /// @param rowCount �ļ�������
    void readStarted(unsigned int rowCount);
    /// @brief ��ȡ�ļ����
    /// @param rowCount ��ȡ���ĵ����
    void readFinished(unsigned int pointCount);

    /// @brief �ļ���ȡ����
    /// @param row ��ǰ�к�
    void readProgress(unsigned int row);
};

} // namespace Toolpath
} // namespace NC