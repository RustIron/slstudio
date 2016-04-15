#ifndef SLKINECTDIALOG_H
#define SLKINECTDIALOG_H

#include <QDialog>
#include <QModelIndex>
#include <SLStudio.h>
#include <opencv2/opencv.hpp>
#include "Kinect/opencvgrabber.hpp"
namespace Ui {
class SLKinectDialog;
}
class SLKinectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SLKinectDialog(SLStudio *parent = 0);
    void timerEvent(QTimerEvent *event);
    void closeEvent(QCloseEvent *);
    ~SLKinectDialog();
private slots:
    void onActionOk();
    void onActionCancel();
//signals:
//    void newDepthMapCaptured();

private:
    Ui::SLKinectDialog *ui;
    bool reviewMode;
//    Mat depthMat;
//    Mat depthf;
//    Mat rgbMat;
//    Mat ownMat;
    unsigned int delay; //ms
    int liveViewTimer;
    Freenect::Freenect* freenect = new(Freenect::Freenect);
    MyFreenectDevice& device = freenect->createDevice<MyFreenectDevice>(0);
};
#endif // SLKINECTDIALOG_H
