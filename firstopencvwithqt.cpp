#include "firstopencvwithqt.h"
#include <QPixmap>
#include <cv.h>
#include <windows.h>

FirstOpenCVwithQt::FirstOpenCVwithQt(QWidget *parent)
	: QMainWindow(parent),cv_img(cv::imread("D:/62a7d933c895d143cfbebbd773f082025baf077d.jpg"))
{
	ui.setupUi(this);

	const int FPS = 24;
	const int FREQ = 1000/FPS;
	cv_cam = new VideoCapture(0);

	cv_cam->open(0);

	//彩色直方图反向投影法
	hranges[0] = 0.0f;
	hranges[1] = 255.0f;
	channels[0] = 0;
	channels[1] = 1;
	channels[2] = 2;
		
	float threshold = 0.05f;
	histSize[0] = 256;
	histSize[1] = 256;
	histSize[2] = 256;

	ranges[0] = hranges;
	ranges[1] = hranges;
	ranges[2] = hranges;

	ROI_img = (cv::imread("./hand.jpg"))(cv::Rect(40, 40, 100, 100));

	Sleep(2000);
	*cv_cam >> cv_img;

	cv_frame = cv_img.clone();
	cv_img_pre = cv_img.clone();
	//开启定时器
	timer.start(FREQ);

	connect(&timer,SIGNAL(timeout()),this,SLOT(ShowImage()));

	label  = new QLabel(this);
	label->move(100, 50);//图像在窗口中所处的位置
	img = QImage((const unsigned char*)(cv_img_processed.data), 1280, 720, QImage::Format_RGB32); 
	label->setPixmap(QPixmap::fromImage(img));  
	label->resize(QSize(640, 480));
	label->show();
}

void FirstOpenCVwithQt::ShowImage(void)
{
	(*cv_cam) >> cv_img;

	ProcesssCapture();

	cvtColor(cv_img, cv_img, CV_RGB2RGBA);

	label->setPixmap(
		QPixmap::fromImage(QImage((const unsigned char*)(cv_img.data), 
		cv_img.cols, cv_img.rows, QImage::Format_ARGB32))
		);

}

// 处理摄像头捕获画面的函数
void FirstOpenCVwithQt::ProcesssCapture()
{
#if 1//混合高斯前景检测法
	mog(cv_img, cv_img_processed, 0.01);

	cv::erode(cv_img_processed, cv_img_processed, cv::Mat(16, 16, CV_8U, cv::Scalar(1)));

	cv::threshold(cv_img_processed, cv_img_processed, 128, 255 ,cv::THRESH_BINARY_INV);

	ScanCaptureToAddPoint();
	DrawPoints(); 
	cv::flip(cv_img, cv_img, 1);
#endif

#if 0//彩色直方图反向投影法

	cv::calcHist(&cv_img, 1, channels, cv::Mat(), histogram, 3, histSize, ranges);
	cv::normalize(histogram, histogram, 1.0);
	cv::calcBackProject(&cv_img, 1, channels, histogram, cv_img_processed, ranges, 255.0);
	cv::threshold(cv_img_processed, cv_img_processed, 0.05*255, 255, cv::THRESH_BINARY);
	cv::flip(cv_img_processed, cv_img_processed, 1);
#endif
}

void FirstOpenCVwithQt::ScanCaptureToAddPoint()
{
	const uchar *data_processed = cv_img_processed.data;
	CvPoint *newpoint = NULL;
	const int cols = cv_img_processed.cols;

	const int total = cv_img_processed.cols * cv_img_processed.rows * cv_img_processed.channels();
	for (int i = 0;i < total;++i)
	{
		if ((*data_processed) == (uchar)(0))
		{
			points.push_back(cv::Point(i%cols,i/cols));
			
			break;
		}
		data_processed++;
	}
}
FirstOpenCVwithQt::~FirstOpenCVwithQt()
{
	delete cv_cam;
}