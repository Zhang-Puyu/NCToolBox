
#include "AbstractReader.h"

#include "qdebug.h"
#include "EigenExtention.hpp"
#include "ReadAPT.h"

#pragma execution_character_set("utf-8")

using namespace NC::Toolpath;
using namespace Eigen;

AbstractrReader::AbstractrReader(QObject* parent)
    : QObject(parent)
{
}

void AbstractrReader::read(Eigen::MatrixXf& points, const QString& fileName, const QString& codec)
{
    // ���ļ�
    QFile file(fileName);
    // ���㵶λ�����鳤��
    int cols = head().split(',').size();

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // һ���Զ�ȡ�ļ�
        // �ı���
        QTextStream stream(&file);
        // ָ���ļ������ʽ
        stream.setCodec(codec.toLocal8Bit());
        // ��ȡ�ļ�
        QStringList lines = stream.readAll().trimmed().split("\n");

        points.resize(1, cols); points.setZero();

		emit readStarted(lines.size());

        VectorXf lastPoint(cols); lastPoint.setZero();
        VectorXf point(cols);

        for (int row = 0; row < lines.size(); ++row)
        {
            point = lastPoint;
            parseLine(lines.at(row), point);
            if (!areSame(lastPoint, point))
            {
                points.conservativeResize(points.rows() + 1, points.cols());
                points.row(points.rows() - 1) = point;
                lastPoint = point;
            }
            emit readProgress(row);
        }

		removeMatrixRow(points, 0);

		file.close();

		emit readFinished(points.rows());
    }
    else
    {
        qDebug() << "Failed to open file:" << fileName;
    }
}

bool AbstractrReader::areSame(const VectorXf& p1, const VectorXf& p2) const
{
    return p1[X] == p2[X] && p1[Y] == p2[Y] && p1[Z] == p2[Z];
}

