#include "number.h"
#include <QDebug>

void Number::parse(QString s1, QString s2)
{
	for(int i = 0; i < s1.size(); i++)
	{
		num.push_back(s1[i].digitValue());
		if(s1[i] == ',')
			commaNum = s1.size() - i - 1;
	}

	bool b = 0;
	for(int i = 0; i < s2.size(); i++)
	{
		nds.push_back(s2[i].digitValue());

		if(s2[i] == ',')
		{
			b = 1;
			commaNDS = s2.size() - i - 1;
		}
	}

	if(b == 0)
		commaNDS = -1;

	qDebug() << "parse: " << num << nds;
}

void Number::putNulls()
{
	for(int i = 0; i < num.size(); ) // убираем запятые
	{
		if(num[i] == -1) num.remove(i);
		else i++;
	}
	for(int i = 0; i < nds.size(); )
	{
		if(nds[i] == -1) nds.remove(i);
		else i++;
	}
	qDebug() << "putNulls: " << num << nds;

	if(num.size() > nds.size())
	{
		if(commaNDS != -1)
		{
			while(commaNDS /*!=*/< commaNum)
			{
				nds.push_back(0);
				++commaNDS;
				qDebug() << num << nds << commaNum << commaNDS;
			}
		}
		while(num.size() /*!=*/ > nds.size())
			nds.push_front(0);
	}
	else if(num.size() < nds.size())
	{
		qDebug() << "Нужное условие";
		while(num.size() != nds.size())
			num.push_front(0);
		//		num.push_front(0);
	}

	qDebug() << num << nds;
}

void Number::plusVectors(QVector<int>& tmp, QVector<int>& res, int level)
{
	if(level == 0) res = tmp;
	else
	{
		//		qDebug() << "Сложение, нули:" << res << tmp << "lvl=" << level;

		for(int i = 0; i < level; i++) tmp.push_back(0);
		while(tmp.size() != res.size()) res.push_front(0);

		int buf = 0;
		//		qDebug() << "Сложение, нули:" << res << tmp << "lvl=" << level;

		for(int i = res.size() - 1; i >= 0; i--)
		{
			int t = res[i] + tmp[i] + buf;
			res[i] = t % 10;
			buf = t / 10;

			if(i == 0 && buf != 0)
			{
				res.push_front(buf);
				break;
			}
		}// qDebug() << "Сложили, res = " << res;
	}
}

void Number::mul()
{
	putNulls();
	QVector<int> tmp(num.size());
	QVector<int> res(num.size());
	int buf = 0, c, i;

	for(/*int*/ i = nds.size() - 1, c = 0; i >= 0; i--, c++)
	{
		buf = 0;

		for(int j = num.size() - 1; j >= 0; j--)
		{
			tmp[j] += ((buf + (nds[i] * num[j])) % 10);
			buf = (buf + (nds[i] * num[j])) / 10;

			if(j == 0 && buf != 0)
			{
				tmp.push_front(0);
				tmp[0] = buf;
				qDebug() << "Нужен еще разряд tmp:" << tmp;
				break;
			}
		}

		plusVectors(tmp, res, c);

		//		qDebug() << "Сложили: " << res;
		tmp.clear(); tmp.resize(num.size());
	}

	end = res;
	clearNulls();
	swapComma();
}

void Number::clearNulls()
{
	while(1)
	{
		if(end[0] == 0 && end.size() != 0)
			end.pop_front();
		else break;
	}
}

void Number::plus(QVector<int> t, QVector<int> r, QVector<int>& result) // num, end, end
{
	t.insert(t.end() - commaNum++, -1);

	qDebug() << "plus() t=" << t << "r=" << r << "result=" << result;
	int intPartNum = 0, intPartEnd = 0, fracPartNum = 0, fracPartEnd = 0;

	for(intPartNum = 0; intPartNum < t.size(); intPartNum++) if(t[intPartNum] == -1) break;
	for(intPartEnd = 0; intPartEnd < r.size(); intPartEnd++) if(r[intPartEnd] == -1) break;
	if(intPartNum != t.size())
		for(int i = intPartNum+1; i < t.size(); i++, fracPartNum++){}
	if(intPartEnd != r.size())
		for(int i = intPartEnd+1; i < r.size(); i++, fracPartEnd++){}

	while(intPartNum < intPartEnd) {t.push_front(0); ++intPartNum;}
	while(intPartNum > intPartEnd) {r.push_front(0); ++intPartEnd;}
	while(fracPartNum < fracPartEnd) {t.push_back(0); ++fracPartNum;}
	while(fracPartNum > fracPartEnd) {r.push_back(0); ++fracPartEnd;}

	QVector<int> ip1(intPartNum), ip2(intPartEnd), fp1(fracPartNum), fp2(fracPartEnd);

	for(int i = 0; i < intPartNum; i++) {ip1[i] = t[i]; ip2[i] = r[i];}
	for(int i = intPartNum+1, c = 0; i < t.size(); i++, c++) {fp1[c] = t[i]; fp2[c] = r[i];/*qDebug() << i << c;*/}
	qDebug() << ip1 << ip2 << fp1 << fp2;

	int buf = 0;

	for(int i = fp1.size() - 1; i >= 0; i--)
	{
		int tmp = (fp1[i] + fp2[i] + buf);
		fp1[i] = tmp % 10;
		buf = tmp / 10;

		if(i == 0 && buf != 0)
		{
//			qDebug() << "Новый разряд";
//			qDebug() << fp1 << buf;
			break;
		}
	}

	for(int i = ip1.size() - 1; i >= 0; i--)
	{
		int tmp = (ip1[i] + ip2[i] + buf);
		ip1[i] = tmp % 10;
		buf = tmp / 10;

		if(i == 0 && buf != 0)
		{
//			qDebug() << "Новый разряд";
			ip1.push_front(buf);
//			qDebug() << ip1;
			break;
		}
	}
	qDebug() << ip1 << "," << fp1;

	QVector<int> ret(ip1.size() + fp1.size() + 1);

	for(int i = 0; i < ip1.size(); i++) ret[i] = ip1[i];
	ret[ip1.size()] = -1;
	for(int i = 0, j = ip1.size() + 1; i < fp1.size(); i++, j++) ret[j] = fp1[i];
	qDebug() << "ret" << ret;

	end = ret;
	--commaNum;

	qDebug() << "plus() t=" << t << "r=" << r << "result=" << result << "fracPartNum = " << fracPartNum;
}

void Number::swapComma()
{
	if(commaNDS == -1) ++commaNDS;
	if(commaNum == -1) ++commaNum;

	int c = end.size() - (commaNDS + commaNum);
	qDebug() << "end" << end << commaNum << commaNDS << c;

	for(int i = 0; c < 0; i++)
	{
		end.push_front(0);
		c++;
	}

	end.insert(end.begin() + c, -1);
	if(c == 0) {end.push_front(0); ++c;}
	qDebug() << end << c;

	qDebug() << "Деление c=" << c;
	for(int j = 0, i = c; j < 2; j++)
	{
		int t = end[i]; end[i] = end[i-1]; end[i-1] = t;

		if(i - 1 != 0)
			--i;
		else
			end.push_front(0);
		qDebug() << end;
	}

	plus(num, end, end); // ВОТ ЗДЕСЬ НДС ПЛЮС ЧИСЛО
}

QString Number::resultToQString()
{
	QString str;
	int c = 0;
	for(int i = 0; i < end.size(); i++)
	{
		if(end[i] == -1)
			{str.append(QChar(',')); c = i;}
		else
			str.append(QChar(end[i]+48));
	}

	if(c != 0)
	{
		while(c > 1 && str[0] == '0') {str.remove(0, 1); --c;}
	}

	return str;
}

QString Number::numToQString()
{
	qDebug() << "numToQString() num=" << num << commaNum;
	num.insert(num.end() - commaNum, -1);
	QString str;
	for(int i = 0; i < num.size(); i++)
	{
		if(num[i] == -1)
			str.append(QChar(','));
		else
			str.append(QChar(num[i]+48));
	}
	return str;
}

void Number::clear()
{
	commaNDS = 0; commaNum = 0;
	end.clear();
	num.clear();
	nds.clear();
}

void Number::kopToRub()
{
	qDebug() << "kopToRub(): end =" << end;
	int i;
	for(i = 0; end[i] != -1 && i != end.size(); i++){}
	int dif = end.size() - i - 1;
	int buf = 0;

	while(dif > 2) {buf = end.back(); end.pop_back(); --dif;}
	if(buf >= 5) ++end.back();
	qDebug() << end;
}

