#include "ReadAPT.h"

#include "qregularexpression.h"

#pragma execution_character_set("utf-8")

using namespace NC::Toolpath;

ReadAPT::ReadAPT(QObject *parent)
	: AbstractrReader(parent)
{}

ReadAPT::~ReadAPT()
{}

void ReadAPT::parseLine(const QString& line, Eigen::VectorXf& point)
{
	if (parsePostion(line, point)) return;
	if (parseFeedRate(line, point)) return;
	if (parseSpindleSpeed(line, point)) return;
}

bool ReadAPT::parsePostion(const QString& line, Eigen::VectorXf& point)
{
	// NREC: GOTO/108.4843,-280.0770,-19.5434,0.380709,-0.921682,0.074586 $$PT2
	// UG:   GOTO/97.8011,83.8859,-11.4651
	// UG:   GOTO/ -0.7457, -1.3959, 38.7258 $$ 0.0000, 0.0000, 37.3528
	if (line.startsWith("GOTO/"))
	{
		point[S] = m_spindleSpeed;
		point[F] = m_rapidMark ? m_rapidFeedRate : m_feedRate;

		m_rapidMark = false;

		QStringList cells = QString(line).remove("GOTO/")
			.split(QString("$$"), Qt::SkipEmptyParts)
			.first().trimmed()
			.split(',');

		// 读取坐标
		point[X] = cells[0].toDouble();
		point[Y] = cells[1].toDouble();
		point[Z] = cells[2].toDouble();
		if (cells.length() > 3)
		{
			point[I] = cells[3].toDouble();
			point[J] = cells[4].toDouble();
			point[K] = cells[5].toDouble();
		}
		return true;
	}
	return false;
}

bool ReadAPT::parseFeedRate(const QString& line, Eigen::VectorXf& point)
{
	// FEDRAT/MMPM,250.0000
	if (line.startsWith("FEDRAT/MMPM"))
	{
		m_feedRate = line.split(",").last().toDouble();
		return true;
	}
	// FEDRAT/6000.0000
	if (line.startsWith("FEDRAT/"))
	{
		m_feedRate = line.split("/").last().toDouble();
		return true;
	}
	if (line.startsWith("RAPID"))
	{
		m_rapidMark = true;
		return true;
	}
	return false;
}

QRegularExpression regex(",.*");
bool ReadAPT::parseSpindleSpeed(const QString& line, Eigen::VectorXf& point)
{
	// SPINDL/ 2300, CLW
	if (line.startsWith("SPINDL/"))
	{
		QString content = QString(line).remove("SPINDL/").trimmed();
		// 使用正则表达式移除逗号及后面的内容
		m_spindleSpeed = content.replace(regex, "").toDouble();
		return true;
	}
	return false;
}

bool ReadAPT::areSame(const Eigen::VectorXf& p1, const Eigen::VectorXf& p2) const
{
	return p1[X] == p2[X] && p1[Y] == p2[Y] && p1[Z] == p2[Z] && 
		   p1[I] == p2[I] && p1[J] == p2[J] && p1[K] == p2[K];
}
