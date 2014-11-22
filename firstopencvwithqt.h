#ifndef FIRSTOPENCVWITHQT_H
#define FIRSTOPENCVWITHQT_H

#include <QtWidgets/QMainWindow>
#include "ui_firstopencvwithqt.h"
#include <highgui.h>
#include "qlabel.h"
#include <QTimer>
#include <video/background_segm.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>

using namespace cv;

struct point{
	short X;
	short Y;
};

class FirstOpenCVwithQt : public QMainWindow
{
	Q_OBJECT
		
public:
	FirstOpenCVwithQt(QWidget *parent = 0);
	~FirstOpenCVwithQt();

private:
	Ui::FirstOpenCVwithQtClass ui;
	cv::Mat cv_img;//ԭʼͼƬ
	cv::Mat cv_img_processed;//�������ͼƬ����
	cv::Mat cv_img_pre;
	cv::Mat cv_frame;
	cv::BackgroundSubtractorMOG2 mog;
	QImage img; //��ʾ��ͼƬ
	QLabel *label;
	VideoCapture *cv_cam;
	QTimer timer;
	//��ɫֱ��ͼ����ͶӰ��
	float hranges[2];
	const float* ranges[3];
	int channels[3];
	float threshold ;
	int histSize[3];
	cv::MatND histogram;
	cv::Mat ROI_img;

	Vector<cv::Point> points;

	void ProcessImg(double newValue);

public slots:
	void ShowImage(void);
private:
	void ScanCaptureToAddPoint();
	// ��������ͷ������ĺ���
	void ProcesssCapture();
	void DrawPoints()
	{
		for (Vector<cv::Point>::const_iterator p = points.begin();p != points.end(); ++p)
		{
			cv::circle(cv_img, *p, 5, cv::Scalar(0, 0, 255), -1);
		}
	}
};

#endif // FIRSTOPENCVWITHQT_H