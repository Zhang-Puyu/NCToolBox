#pragma once

#include <Eigen/Dense>

#include "qstring.h"
#include "qstringlist.h"
#include "qdebug.h"
#include "qfile.h"
#include "qtextstream.h"

#include "StringExtension.hpp"

namespace Eigen
{
	/// @brief qDebug�������
	static void qDebugEigenMatrix(const Eigen::MatrixXf& mat)
	{
		for (int row = 0; row < mat.rows(); ++row)
		{
			QString line;
			for (int col = 0; col < mat.cols(); ++col)
				line += QString::number(mat(row, col)) + ", ";
			qDebug() << line;
		}
	}

	/// @brief ����������ϵ����Χ
	/// @param vector ԭ����
	/// @param minfoeff ������Сֵ
	/// @param maxcoeff �������ֵ
	/// @param coeff ����
	static void coeffRange(const Eigen::VectorXf& vector, double& minCoeff, double& maxCoeff, float coeff)
	{
		double maxTrue = vector.maxCoeff();
		double minTrue = vector.minCoeff();
		minCoeff = minTrue - (maxTrue - minTrue) * (coeff - 1);
		maxCoeff = maxTrue + (maxTrue - minTrue) * (coeff - 1);
	}

	/// @brief ������λ���Ƿ��غ�
	/// @param p1 (x, y, z, a, b, c)
	/// @param p2 (x, y, z, a, b, c)
	static bool isCoincident(const Eigen::VectorXf& p1, const Eigen::VectorXf& p2)
	{
		return (p1 - p2).segment(0, 3).norm() < 1e-6;
	}

	/// @brief ������λ�㵶���Ƿ�ƽ��
	/// @param p1 (x, y, z, a, b, c)
	/// @param p2 (x, y, z, a, b, c)
	static bool isParalleled(const Eigen::VectorXf& p1, const Eigen::VectorXf& p2)
	{
		return (p1.segment(3, 3) - p2.segment(3, 3)).norm() < 1e-6;
	}

	/// @brief ����ɾ��һ��
	/// @param matrix ����
	/// @param rowToRemove Ҫɾ�����к�
	static void removeMatrixRow(Eigen::MatrixXf& matrix, unsigned int rowToRemove) {

		unsigned int numRows = matrix.rows() - 1;
		unsigned int numCols = matrix.cols();

		if (rowToRemove < numRows) {
			matrix.block(rowToRemove, 0, numRows - rowToRemove, numCols) =
				matrix.block(rowToRemove + 1, 0, numRows - rowToRemove, numCols);
		}
		matrix.conservativeResize(numRows, numCols);
	}

	/// @brief ����ɾ��һ��
	/// @param matrix ����
	/// @param colToRemove Ҫɾ�����к�
	static void removeMatrixColumn(Eigen::MatrixXf& matrix, unsigned int colToRemove) {

		unsigned int numRows = matrix.rows();
		unsigned int numCols = matrix.cols() - 1;

		if (colToRemove < numCols) {
			matrix.block(0, colToRemove, numRows, numCols - colToRemove) =
				matrix.block(0, colToRemove + 1, numRows, numCols - colToRemove);
		}

		matrix.conservativeResize(numRows, numCols);
	}

	/// @brief Eigen�������һ�У���Ԫ�ص�ֵΪ�к�����
	static void addMatrixColumn(Eigen::MatrixXf& mat)
	{
		// ��ȡ��������
		int rows = mat.rows();

		// ������в���ֵ�к�
		mat.conservativeResize(rows, mat.cols() + 1);
		// �����и�ֵ�кţ�0,1,2,...��
		for (int i = 0; i < rows; ++i) 
			mat(i, mat.cols() - 1) = i;
	}

	
	/// @brief Eigen�������һ�У���Ԫ�ص�ֵΪ����ֵ
	static void addMatrixColumn(Eigen::MatrixXf& mat, double value)
	{
		// ���������С������һ��
		mat.conservativeResize(mat.rows(), mat.cols() + 1);
		// Ϊ�������ó���ֵ
		mat.col(mat.cols() - 1).setConstant(value);
	}

	/// @brief ��ȡcsv�ļ�������
	/// @param matrix ����
	/// @param fileName �ļ�·��
	/// @param skipFirstRow �Ƿ�������һ�У���ͷ��
	/// @param separator �ָ���
	/// @param codec �����ʽ
	static void readCSV(Eigen::MatrixXf& matrix, const QString& fileName, 
		      QStringList& head,
		const QString& separator = ",",
		const QString& codec = "UTF-8") 
	{ 
		QFile file(fileName);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			// һ���Զ�ȡ�ļ�
			// �ı���
			QTextStream stream(&file);
			stream.setCodec(codec.toLocal8Bit());
			QStringList lines = stream.readAll().trimmed().split("\n");

			// ������ͷ
			head.clear();
			QString firstLine = lines.first();
			if (String::isHead(firstLine, separator))
			{
				for (const QString& cell : firstLine.trimmed().split(separator))
					head.append(cell.trimmed());
				lines.removeFirst();
			}
			else
			{
				for (int i = 0; i < firstLine.split(separator).size(); i++)
					head.append(QString("col ") + QString::number(i));
			}
				
			int rows = lines.size();
			int cols = lines.first().split(separator).size();
			matrix.resize(rows, cols);

			for (int row = 0; row < rows; ++row)
			{
				QStringList values = lines.at(row).split(separator);
				for (int col = 0; col < cols; ++col)
					matrix(row, col) = values.at(col).toDouble();
			}
			file.close();
		}
		else
		{
			qDebug() << "Failed to open file:" << fileName;
		}
	}

	/// @brief ������д��csv�ļ�
	/// @param matrix ����
	/// @param fileName �ļ�·��
	/// @param head ��ͷ
	/// @param separator �ָ���
	/// @param codec �����ʽ
	static void writeCSV(const Eigen::MatrixXf& matrix, const QString& fileName,
		const QString& head = "",
		const QString& separator = ",",
		const QString& codec = "UTF-8") 
	{ 
		QFile file(fileName);
		if (file.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QTextStream stream(&file);
			stream.setCodec(codec.toLocal8Bit());

			if (!head.isEmpty())
				stream << head << "\n";

			for (int row = 0; row < matrix.rows(); ++row)
			{
				for (int col = 0; col < matrix.cols(); ++col)
				{
					stream << matrix(row, col);
					if (col < matrix.cols() - 1)
						stream << separator;
				}
				stream << "\n";
			}
			file.close();
		}
	}
}