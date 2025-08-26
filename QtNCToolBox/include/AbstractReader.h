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

    // 宏定义
    static const int X = 0;
    static const int Y = 1;
    static const int Z = 2;
    static const int F = 3;
    static const int S = 4;

	/// @brief 表头信息
    virtual QString head() const = 0;

    void setRapidFeedRate(double f) { m_rapidFeedRate = f; }

    /// @brief 读取刀位点信息
    /// @param fileName 文件路径
    /// @param codec 编码格式
    void read(Eigen::MatrixXf& m_points,
        const QString& fileName, 
        const QString& codec = "UTF-8");

protected:
    /// @brief 解析一行代码
    virtual void parseLine(const QString& line, Eigen::VectorXf& point) = 0;

    /// @brief 判断两个刀位点是否相同
    virtual bool areSame(const Eigen::VectorXf& p1, const Eigen::VectorXf& p2) const;

    /// @brief 快速进给速度
    double m_rapidFeedRate = -1.0;

signals:
    /// @brief 读取文件开始
    /// @param rowCount 文件总行数
    void readStarted(unsigned int rowCount);
    /// @brief 读取文件完成
    /// @param rowCount 读取到的点个数
    void readFinished(unsigned int pointCount);

    /// @brief 文件读取进度
    /// @param row 当前行号
    void readProgress(unsigned int row);
};

} // namespace Toolpath
} // namespace NC