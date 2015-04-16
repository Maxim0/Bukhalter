#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <map>
#include <QVector>
#include <QDebug>
#include "number.h"
using namespace std;

namespace Ui {class Dialog;}

class Dialog : public QDialog
{
	Q_OBJECT

public:
	explicit Dialog(QWidget *parent = 0);
	~Dialog();

private:
	Ui::Dialog *ui;
	Number num;
	QString res;
	QString digit;
	map<int, QString> numWord;

	void numByWords();
	QString numberByWords(QString tmp);

public slots:
	void buttonPressed();
};

#endif // DIALOG_H
