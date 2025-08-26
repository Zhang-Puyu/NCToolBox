#include "ReadGCode.h"

#include "qregularexpression.h"

#pragma execution_character_set("utf-8")

using namespace NC::Toolpath;

ReadGCode::ReadGCode(QObject *parent)
	: AbstractrReader(parent)
{}

ReadGCode::~ReadGCode()
{}

// 正则表达式匹配G代码中的字母和数字组合
const QRegularExpression regex(R"(([I-Z])(-?\d+(?:\.\d+)?|\.\d+))");
QRegularExpressionMatchIterator matches;

void ReadGCode::parseLine(const QString& line, Eigen::VectorXf& point)
{
	QHash<QChar, float> map;
    QChar letter;
    QString number;

    matches = regex.globalMatch(line);

    while (matches.hasNext()) 
    {
        auto match = matches.next();
        if (match.capturedTexts().size() > 2) 
        {
            letter = match.captured(2).at(0);
            number = match.captured(3);

            if (letter == 'G' && number.toFloat() == 0) 
                map.insert('F', m_rapidFeedRate);          
            else 
                map.insert(letter, number.toFloat());
        }
    }

    // 更新点的坐标值
    if (map.contains('X')) point[X] = map['X'];
    if (map.contains('Y')) point[Y] = map['Y'];
    if (map.contains('Z')) point[Z] = map['Z'];
    if (map.contains('A')) point[A] = map['A'];
    if (map.contains('B')) point[B] = map['B'];
    if (map.contains('C')) point[C] = map['C'];
    if (map.contains('F')) point[F] = map['F'];
    if (map.contains('S')) point[S] = map['S'];
}

bool ReadGCode::areSame(const Eigen::VectorXf& p1, const Eigen::VectorXf& p2) const
{
	return p1[X] == p2[X] && p1[Y] == p2[Y] && p1[Z] == p2[Z] &&
		p1[A] == p2[A] && p1[B] == p2[B] && p1[C] == p2[C];
}
