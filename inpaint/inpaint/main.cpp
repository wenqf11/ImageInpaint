/*******************************************************
Copyright: School of Software, Tsinghua University
Author: Qingfu Wen
Email: thssvince@163.com
Created: 2015-01-09
Description: image inpainting using PatchMatch
*******************************************************/

#include "inpaint.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	inpaint w;
	w.show();
	return a.exec();
}
