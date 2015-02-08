/*******************************************************
Copyright: School of Software, Tsinghua University
Author: Qingfu Wen
Email: thssvince@163.com
Created: 2015-01-09
Description: image inpainting using PatchMatch
*******************************************************/

#ifndef INPAINT_H
#define INPAINT_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets>
#include "ui_inpaint.h"
#include <opencv\highgui.h>
#include <opencv\cv.h>

#define MAX_ITERATION 8
const int patch_size = 9;

class inpaint : public QMainWindow
{
	Q_OBJECT

public:
	inpaint(QWidget *parent = 0);
	~inpaint();

private:
	Ui::inpaintClass ui;
	cv::Mat dstImage, srcImage;
	cv::Mat rgb;
	QImage imgScaled;

	bool isInside(int x, int y);
	float inpaint::calculatePatchDist(const cv::Mat& A, const cv::Mat& B,  CvPoint P_a, CvPoint P_b);
	double inpaint::uniformRandom(double dMinValue, double dMaxValue);

	private slots:
		QImage Mat2QImage(const cv::Mat & mat);
		cv::Mat QImage2Mat(const QImage & src);
		void keyPressEvent( QKeyEvent *k );
		void display(QImage img);
		void on_loadButton_clicked();
		void on_saveButton_clicked();
		void on_inpaintButton_clicked();
		void image_inpaint(cv::Mat& img, cv::Mat& result);
};

#endif // INPAINT_H
