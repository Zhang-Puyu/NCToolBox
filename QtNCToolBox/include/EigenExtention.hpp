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
	/// @brief qDebug输出矩阵
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

	/// @brief 按比例计算系数范围
	/// @param vector 原向量
	/// @param minfoeff 比例最小值
	/// @param maxcoeff 比例最大值
	/// @param coeff 比例
	static void coeffRange(const Eigen::VectorXf& vector, double& minCoeff, double& maxCoeff, float coeff)
	{
		double maxTrue = vector.maxCoeff();
		double minTrue = vector.minCoeff();
		minCoeff = minTrue - (maxTrue - minTrue) * (coeff - 1);
		maxCoeff = maxTrue + (maxTrue - minTrue) * (coeff - 1);
	}

	/// @brief 两个刀位点是否重合
	/// @param p1 (x, y, z, a, b, c)
	/// @param p2 (x, y, z, a, b, c)
	static bool isCoincident(const Eigen::VectorXf& p1, const Eigen::VectorXf& p2)
	{
		return (p1 - p2).segment(0, 3).norm() < 1e-6;
	}

	/// @brief 两个刀位点刀轴是否平行
	/// @param p1 (x, y, z, a, b, c)
	/// @param p2 (x, y, z, a, b, c)
	static bool isParalleled(const Eigen::VectorXf& p1, const Eigen::VectorXf& p2)
	{
		return (p1.segment(3, 3) - p2.segment(3, 3)).norm() < 1e-6;
	}

	/// @brief 矩阵删除一行
	/// @param matrix 矩阵
	/// @param rowToRemove 要删除的行号
	static void removeMatrixRow(Eigen::MatrixXf& matrix, unsigned int rowToRemove) {

		unsigned int numRows = matrix.rows() - 1;
		unsigned int numCols = matrix.cols();

		if (rowToRemove < numRows) {
			matrix.block(rowToRemove, 0, numRows - rowToRemove, numCols) =
				matrix.block(rowToRemove + 1, 0, numRows - rowToRemove, numCols);
		}
		matrix.conservativeResize(numRows, numCols);
	}

	/// @brief 矩阵删除一列
	/// @param matrix 矩阵
	/// @param colToRemove 要删除的列号
	static void removeMatrixColumn(Eigen::MatrixXf& matrix, unsigned int colToRemove) {

		unsigned int numRows = matrix.rows();
		unsigned int numCols = matrix.cols() - 1;

		if (colToRemove < numCols) {
			matrix.block(0, colToRemove, numRows, numCols - colToRemove) =
				matrix.block(0, colToRemove + 1, numRows, numCols - colToRemove);
		}

		matrix.conservativeResize(numRows, numCols);
	}

	/// @brief Eigen矩阵添加一列，各元素的值为行号索引
	static void addMatrixColumn(Eigen::MatrixXf& mat)
	{
		// 获取矩阵行数
		int rows = mat.rows();

		// 添加新列并赋值行号
		mat.conservativeResize(rows, mat.cols() + 1);
		// 给新列赋值行号（0,1,2,...）
		for (int i = 0; i < rows; ++i) 
			mat(i, mat.cols() - 1) = i;
	}

	
	/// @brief Eigen矩阵添加一列，各元素的值为给定值
	static void addMatrixColumn(Eigen::MatrixXf& mat, double value)
	{
		// 调整矩阵大小，增加一列
		mat.conservativeResize(mat.rows(), mat.cols() + 1);
		// 为新列设置常量值
		mat.col(mat.cols() - 1).setConstant(value);
	}

	/// @brief 读取csv文件到矩阵
	/// @param matrix 矩阵
	/// @param fileName 文件路径
	/// @param skipFirstRow 是否跳过第一行（表头）
	/// @param separator 分隔符
	/// @param codec 编码格式
	static void readCSV(Eigen::MatrixXf& matrix, const QString& fileName, 
		      QStringList& head,
		const QString& separator = ",",
		const QString& codec = "UTF-8") 
	{ 
		QFile file(fileName);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			// 一次性读取文件
			// 文本流
			QTextStream stream(&file);
			stream.setCodec(codec.toLocal8Bit());
			QStringList lines = stream.readAll().trimmed().split("\n");

			// 解析表头
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

	/// @brief 将矩阵写入csv文件
	/// @param matrix 矩阵
	/// @param fileName 文件路径
	/// @param head 表头
	/// @param separator 分隔符
	/// @param codec 编码格式
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