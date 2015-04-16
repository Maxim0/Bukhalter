#include "dialog.h"
#include "ui_dialog.h"
#include <iostream>
#include <QMessageBox>
using namespace std;

Dialog::Dialog(QWidget *parent):QDialog(parent), ui(new Ui::Dialog)
{
//	this->setWindowFlags(Qt::Tool);
	this->setWindowFlags(Qt::Window);
	QRect r = this->geometry();
	r.setX(450); r.setY(150);
	this->setGeometry(r);

	numWord[1] = "один"; numWord[2] = "два"; numWord[3] = "три"; numWord[4] = "четыре"; numWord[5] = "пять"; numWord[6] = "шесть";
	numWord[7] = "семь"; numWord[8] = "восемь"; numWord[9] = "девять";

	ui->setupUi(this);
	setWindowTitle("Бухгалтер v1.0");

	ui->lineNumber->setText("987,99");
	ui->lineNDS->setText("18,99");
	ui->lineByWords->setReadOnly(1);
	ui->lineResult->setReadOnly(1);
	ui->lineByWordsResult->setReadOnly(1);

	connect(ui->buttonCalculate, SIGNAL(clicked()), this, SLOT(buttonPressed()));
}

Dialog::~Dialog()
{
	delete ui;
}

QString Dialog::numberByWords(QString tmp)
{
	QString word;
	int intPart, fracPart;
	bool isComma = 1;

	for(intPart = 0; tmp[intPart] != ','; ++intPart){if(intPart == tmp.size()) {isComma = 0; break;}}
	fracPart = tmp.size() - intPart - 1;

	while(tmp[0] == '0' && intPart > 1) {tmp.remove(0, 1); --intPart; qDebug() << "Удаление";}

	qDebug() << "numberByWords() tmp=" << tmp;
	qDebug() << "numberByWords() comma=" << intPart << fracPart;


	if(intPart == 1 && tmp[0] == '0')
		word += "ноль";

	for(int i = 0; i < intPart; i++)
	{
		int n = tmp[i].digitValue();
		switch(intPart - i)
		{
			case 7:
				word = word + numWord[n] + " " + "миллион";
				if(n >= 2 && n <= 4) word += "а";
				if(n > 4) word += "ов";
				word.append(' ');
				break;

			case 6:
				if(n == 1) word += "сто";
				if(n == 2) word += "двести";
				if(n == 3) word += "триста";
				if(n == 4) word += "четыреста";
				else if(n >= 5) word = (word + numWord[n] + "сот");

				word.append(' ');
				break;

			case 5:
				if(n == 1 && tmp[i+1].digitValue() != 0)
				{
					word = (word + numWord[tmp[i+1].digitValue()] + "надцать тысяч ");
					++i;
					break;
				}
				if(n != 1 && n != 4 && n != 9)
				{
					word += numWord[n];
					word += "десят";
				}
				else if(n == 1) word += "десять";
				else if(n == 4) word += "сорок";
				else if(n == 9) word += "девяносто";
				word.append(' ');

				break;
			case 4:
				if(n == 1) word += "одна тысяча";
				if(n == 2) word += "две тысячи";
				if(n == 0) word += "тысяч";
				if(n >= 3 && n <= 4) word = (word + numWord[n] + " тысячи");
				if(n >= 5) word = (word + numWord[n] + " тысяч");
				word.append(' ');
				break;
			case 3:
				if(n == 1) word += "сто";
				if(n == 2) word += "двести";
				if(n == 3) word += "триста";
				if(n == 4) word += "четыреста";
				else if(n >= 5) word = (word + numWord[n] + "сот");

				word.append(' ');
				break;

			case 2:
				if(n == 1 && tmp[i+1].digitValue() != 0)
				{
					word = (word + numWord[tmp[i+1].digitValue()] + "надцать ");
					++i;
					break;
				}
				if(n == 2 || n == 3) word = (word + numWord[n] + "дцать");
				if(n >= 5 && n <= 8) word = (word + numWord[n] + "десят");
				else if(n == 1) word += "десять";
				else if(n == 4) word += "сорок";
				else if(n == 9) word += "девяносто";
				word.append(' ');
				break;

			case 1:
				word += (numWord[n] + " Р.");
				break;
		}
	}

	if(isComma == 1)
	{
		word.append(',');
		word.append(' ');
		for(int i = intPart + 1; i < tmp.size(); i++) word.append(tmp[i]);
		word += " к.";
	}

	return word;
}

void Dialog::buttonPressed()
{
	if(ui->lineNDS->text().isEmpty())
	{
		QMessageBox msg;
		msg.setText("Ошибка: пустое значение числа!");
		msg.exec();
	}

	else if(ui->lineNumber->text().toInt() == 0 && !(ui->lineNumber->text().contains(',')))
	{
		QMessageBox msg;
		msg.setText("Ошибка: число должно быть неотрицательным!");
		msg.exec();
	}

	else if(ui->lineNDS->text().toInt() > 100)
	{
		QMessageBox msg;
		msg.setText("Нельзя назначить НДС большим числа!");
		msg.exec();
	}

	else if(ui->lineNDS->text().toInt() == 0 && !(ui->lineNDS->text().contains(',')))
	{
		QMessageBox msg;
		msg.setText("Невозможно вычислить 0 процентов!");
		msg.exec();

	}

	else if(ui->lineNDS->text().toInt() < 0)
	{
		QMessageBox msg;
		msg.setText("Проценты должны быть неотрициательными!");
		msg.exec();
	}

	else if(ui->lineNumber->text() != "" && ui->lineNDS->text() != "")
	{
		num.parse(ui->lineNumber->text(), ui->lineNDS->text());
		num.mul();
		num.kopToRub();

		res = num.resultToQString();
		digit = num.numToQString();

		ui->lineResult->setText(res);
		ui->lineByWordsResult->setText(numberByWords(res));
		ui->lineByWords->setText(numberByWords(digit));


		num.clear();
	}
}
