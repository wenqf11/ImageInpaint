/*******************************************************
Copyright: School of Software, Tsinghua University
Author: Qingfu Wen
Email: thssvince@163.com
Created: 2015-01-09
Description: image inpainting using PatchMatch
*******************************************************/

#include "inpaint.h"

inpaint::inpaint(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.saveButton->setEnabled(false);
	ui.inpaintButton->setEnabled(false);
}

inpaint::~inpaint()
{

}

void inpaint::on_loadButton_clicked()
{
	QString fileName;
	fileName = QFileDialog::getOpenFileName(this,
		tr("Select a picture"),
		"",
		tr("images (*.png *.bmp *.jpg *.tif *.GIF )"));

	if(fileName.isEmpty()){
		return;
	}

	QImage img;
	if(!(img.load(fileName)))
	{
		QMessageBox::information(this,
			tr("error"),
			tr("error openning picture!"));
		return;
	}
	srcImage = QImage2Mat(img);
	dstImage = srcImage.clone();

	display(img);

	ui.saveButton->setEnabled(true);
	ui.inpaintButton->setEnabled(true);
	ui.saveButton->setEnabled(true);
}

QImage inpaint::Mat2QImage(const cv::Mat & mat)
{
	QImage img;

	if(mat.channels()==3)
	{
		cv::cvtColor(mat, rgb, CV_BGR2RGB);
		img =QImage((const unsigned char*)(rgb.data),
			rgb.cols,rgb.rows,
			rgb.cols*rgb.channels(),
			QImage::Format_RGB888);
	}
	else
	{
		img = QImage((const unsigned char*)(mat.data),
			mat.cols,mat.rows,
			mat.cols*mat.channels(),
			QImage::Format_RGB888);
	}

	return img;
}

cv::Mat inpaint::QImage2Mat(const QImage & src) {
	cv::Mat mat = cv::Mat(src.height(), src.width(), CV_8UC4, (uchar*)src.bits(), src.bytesPerLine());
	cv::Mat mat2 = cv::Mat(mat.rows, mat.cols, CV_8UC3 );
	int from_to[] = { 0,0,  1,1,  2,2};
	cv::mixChannels( &mat, 1, &mat2, 1, from_to, 3 );
	return mat2;
};

void inpaint::display(QImage img)
{
	imgScaled = img.scaled(ui.label->size(),Qt::KeepAspectRatio);
	ui.label->setPixmap(QPixmap::fromImage(imgScaled));
}

void inpaint::on_saveButton_clicked()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Open image"),
		"output.jpg",
		tr("images (*.png *.bmp *.jpg *.tif *.GIF )"));

	if (!fileName.isNull()){
		imwrite(fileName.toStdString(),dstImage);
	}
}
double inpaint::uniformRandom(double dMinValue, double dMaxValue)
{
	double pRandomValue = (double)(rand()/(double)RAND_MAX);
	pRandomValue = pRandomValue*(dMaxValue-dMinValue)+dMinValue;
	return pRandomValue;
}
bool inpaint::isInside(int x, int y)
{
	if(x >= patch_size/2 && x < srcImage.rows - patch_size/2
		&& y >= patch_size/2 && y < srcImage.cols - patch_size/2)
	{
		return true;
	}
	return false;
}
float inpaint::calculatePatchDist(const cv::Mat& A, const cv::Mat& B, CvPoint P_a, CvPoint P_b)
{
	float dist = 0;
	float pixel = 0;
	for(int i = -patch_size/2; i <= patch_size/2; ++i)
		for(int j = -patch_size/2; j <= patch_size/2; ++j){
			pixel = 0;
			for(int k = 0; k < 3; ++k)
				if(P_a.x+i>=0 && P_a.x+i<srcImage.rows && P_a.y+j >= 0 &&  P_a.y+j <= srcImage.cols
					&& P_b.x+i>=0 && P_b.x+i<srcImage.rows && P_b.y+j >= 0 &&  P_b.y+j <= srcImage.cols){
					pixel += pow((A.at<cv::Vec3b>(P_a.x+i, P_a.y+j)[k]-B.at<cv::Vec3b>(P_b.x+i, P_b.y+j)[k]),3);
				}
			dist += pow(pixel, 2);
		}
	return dist;
}
void inpaint::image_inpaint(cv::Mat& image, cv::Mat& result)
{    
	cv::Mat copy_image = image.clone();
	int width = image.cols;
	int height = image.rows;  
	float* f = new float[height*width];
	bool* mask = new bool[height*width];
	CvPoint* g = new CvPoint[height*width];
	int x = 0, y = 0;


	for (int i = 0; i < height; ++i)
		for(int j = 0; j < width; ++j){
			if(image.at<cv::Vec3b>(i,j)[0]==0 && image.at<cv::Vec3b>(i,j)[1]==0
				&&image.at<cv::Vec3b>(i,j)[2]==255)
			{
				mask[i*width+j] = true;
			}
			else{
				mask[i*width+j] = false;
			}
		}

	//init
	srand((unsigned)time(NULL));
	for(int i = 0; i < height; ++i)
		for(int j = 0; j < width; ++j)
		if(mask[i*width+j]){
			do 
			{
				x = rand() % height;
				y = rand() % width;
			} while (!isInside(x, y));

			g[i*width+j] = cvPoint(x, y);
			f[i*width+j] = calculatePatchDist(image, copy_image, cvPoint(i,j), cvPoint(x,y));
			image.at<cv::Vec3b>(i,j)[0] = copy_image.at<cv::Vec3b>(x,y)[0];
			image.at<cv::Vec3b>(i,j)[1] = copy_image.at<cv::Vec3b>(x,y)[1];
			image.at<cv::Vec3b>(i,j)[2] = copy_image.at<cv::Vec3b>(x,y)[2];
		}
		else{
			g[i*width+j] = cvPoint(i, j);
			f[i*width+j] = 0;
		}

	double threshold = 0;
	int iteration_times = 0;

	while(1){
		float minPatchDist = 0;
		float patchDist = 0;
		float alpha = 0.5, w = MAX(width, height), R, T;
		int u, v;
		++iteration_times;

		//propagation
		if(iteration_times % 2 == 1){
			for (int i = 0; i < height; ++i)
				for(int j = 0; j < width; ++j)
				if(mask[i*width+j]){
					minPatchDist = f[i*width+j];

					if(i-1 >= 0 && g[(i-1)*width+j].x-1>=0){
						u = g[(i-1)*width+j].x-1;
						v = g[(i-1)*width+j].y;
						if(isInside(u, v)){
							patchDist = calculatePatchDist(image, copy_image, cvPoint(i,j), cvPoint(u, v));
							if(patchDist < minPatchDist){
								minPatchDist = patchDist;
								g[i*width+j] = cvPoint(g[(i-1)*width+j].x-1, g[(i-1)*width+j].y);
								f[i*width+j] = patchDist;
							}
						}
					}

					if(j-1 >= 0){
						u = g[i*width+j-1].x;
						v = g[i*width+j-1].y-1;
						if(isInside(u, v)){
							patchDist = calculatePatchDist(image, copy_image, cvPoint(i,j), cvPoint(u, v));
							if(patchDist < minPatchDist){
								minPatchDist = patchDist;
								g[i*width+j] = cvPoint(g[i*width+j-1].x, g[i*width+j-1].y-1);
								f[i*width+j] = patchDist;
							}
						}
					}
				}
		}
		else{
			for (int i = height-1; i >= 0; --i)
				for(int j = width-1; j >=0; --j)
					if(mask[i*width+j]){
						minPatchDist = f[i*width+j];

						if(i+1<height && g[(i+1)*width+j].x+1<height){
							u = g[(i+1)*width+j].x+1;
							v = g[(i+1)*width+j].y;
							if(isInside(u, v)){
								patchDist = calculatePatchDist(image, copy_image, cvPoint(i,j), cvPoint(u, v));
								if(patchDist < minPatchDist){
									minPatchDist = patchDist;
									g[i*width+j] = cvPoint(g[(i+1)*width+j].x+1, g[(i+1)*width+j].y);
									f[i*width+j] = patchDist;
								}
							}
						}

						if(j+1 < width){
							u = g[i*width+j+1].x;
							v = g[i*width+j+1].y+1;
							if(isInside(u, v)){
								patchDist = calculatePatchDist(image, copy_image, cvPoint(i,j), cvPoint(u, v));
								if(patchDist < minPatchDist){
									minPatchDist = patchDist;
									g[i*width+j] = cvPoint(g[i*width+j+1].x, g[i*width+j+1].y+1);
									f[i*width+j] = patchDist;
								}
							}
						}
					}
		}

		//random search
		float alpha_i = alpha;
		srand((unsigned)time(NULL));
		for (int i = 0; i < height; ++i)
			for(int j = 0; j < width; ++j)
			if(mask[i*width+j]){
				while(alpha_i*w>=1){
					R = uniformRandom(-1, 1);
					T = uniformRandom(-1, 1);
					u = i + alpha_i*w*R;
					v = j + alpha_i*w*T;
					if(isInside(u, v)){
						patchDist = calculatePatchDist(image, copy_image, cvPoint(i,j), cvPoint(u,v));
						if(patchDist < f[i*width+j]){
							f[i*width+j] = patchDist;
							g[i*width+j] = cvPoint(u,v);
						}
					}
					alpha_i *= alpha;
				}
			}
		
		threshold = 0;

		for (int i = 0; i < height; ++i)
			for(int j = 0; j < width; ++j){
				if(mask[i*width+j]){
					u = g[i*width+j].x;
					v = g[i*width+j].y;
					threshold += abs(image.at<cv::Vec3b>(i,j)[0] - copy_image.at<cv::Vec3b>(u,v)[0]);
					threshold += abs(image.at<cv::Vec3b>(i,j)[1] - copy_image.at<cv::Vec3b>(u,v)[1]);
					threshold += abs(image.at<cv::Vec3b>(i,j)[2] - copy_image.at<cv::Vec3b>(u,v)[2]);
					image.at<cv::Vec3b>(i,j)[0] = copy_image.at<cv::Vec3b>(u,v)[0];
					image.at<cv::Vec3b>(i,j)[1] = copy_image.at<cv::Vec3b>(u,v)[1];
					image.at<cv::Vec3b>(i,j)[2] = copy_image.at<cv::Vec3b>(u,v)[2];
				}
			}
		if(threshold == 0) break; 
		if(iteration_times>MAX_ITERATION) break;
		copy_image = image.clone();
		
	}
	delete mask;
	delete f;
	delete g;
	result = image.clone();
}

void inpaint::on_inpaintButton_clicked()
{
	image_inpaint(srcImage,dstImage);
	QImage img = Mat2QImage(dstImage);
	display(img);
}

void inpaint::keyPressEvent(QKeyEvent *k) 
{ 
	if(k->key() == Qt::Key_U) 
	{ 
		QImage img = Mat2QImage(srcImage);
		display(img);
	}
}
