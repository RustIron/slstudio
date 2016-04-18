#include "SLKinectDialog.h"
#include "ui_SLKinectDialog.h"
#include <QtTest/QTest>
#include <QSettings>

#include <opencv2/opencv.hpp>
#include "Kinect/opencvgrabber.hpp"


SLKinectDialog::SLKinectDialog(SLStudio *parent) :QDialog(parent), ui(new Ui::SLKinectDialog) {
    ui->setupUi(this);
    setSizeGripEnabled(false);
    reviewMode = false;
    isdevice   = false;
    delay = 33.333; //ms
}

//    Freenect::Freenect* SLKinectDialog::myfreenect_ = new(Freenect::Freenect);
//    MyFreenectDevice*   SLKinectDialog::mydevice_   = &myfreenect_->createDevice<MyFreenectDevice>(0);
//    Freenect::Freenect* myfreenect_;
//    MyFreenectDevice* mydevice_=&myfreenect_->createDevice<MyFreenectDevice>(0);

SLKinectDialog::~SLKinectDialog()
{
    delete ui;
}
void SLKinectDialog::timerEvent(QTimerEvent *event){

    if(event->timerId() != liveViewTimer){
        std::cerr << "Something fishy..." << std::endl << std::flush;
        return;
    }

    QApplication::processEvents();
    Mat depthMat(cv::Size(640,480),CV_16UC1);
    Mat depthf(cv::Size(640,480),CV_8UC1);
    Mat rgbMat(cv::Size(640,480),CV_8UC3,Scalar(0));
 //   Mat ownMat(cv::Size(640,480),CV_8UC3,Scalar(0));
    mydevice_->getVideo(rgbMat);
    mydevice_->getDepth(depthMat);

    depthMat.convertTo(depthf, CV_8UC1, 255.0/2048.0);

    ui->depthMap->showFrameCV(depthf);
    ui->rgbMap->showFrameCV(rgbMat);

    QApplication::processEvents();
}

void SLKinectDialog::closeEvent(QCloseEvent *)
{
//    killTimer(liveViewTimer);
    this->deleteLater();
}
void SLKinectDialog::onActionOk()
{
    if(isdevice)
    {
        if(reviewMode){
        liveViewTimer = startTimer(delay);
        reviewMode = false;
        return;
        }
      // Stop live view
        killTimer(liveViewTimer);
        reviewMode = true;
    }
    else
    {
        myfreenect_ = new(Freenect::Freenect);
        mydevice_   = &myfreenect_->createDevice<MyFreenectDevice>(0);
        mydevice_->startVideo();
        mydevice_->startDepth();
        liveViewTimer = startTimer(delay);
        isdevice = true;
    }
}
void SLKinectDialog::onActionCancel()
{
    mydevice_->stopVideo();
    mydevice_->stopDepth();
    isdevice = false;
}
