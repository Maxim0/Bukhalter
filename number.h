#ifndef NUMBER_H
#define NUMBER_H

#include <QVector>
#include <QString>

class Number
{
	QVector<int> num;
	QVector<int> nds;
	QVector<int> end;
	int commaNum;
	int commaNDS;

	void putNulls();
	void plusVectors(QVector<int> &tmp, QVector<int> &res, int level);
	void plus(QVector<int> t, QVector<int> r, QVector<int> &result);

public:
	void parse(QString s1, QString s2);
	void mul();
	void clearNulls();
	void swapComma();
	void clear();
	void kopToRub();
	QString resultToQString();
	QString numToQString();

	Number():commaNum(0), commaNDS(0){}
};

#endif // NUMBER_H
