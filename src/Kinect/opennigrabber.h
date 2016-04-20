 #ifndef OPNIGRABBER
 #define OPNIGRABBER

 #include <pcl/io/openni_grabber.h>
 #include <pcl/visualization/cloud_viewer.h>
 #include <QObject>
 #include <QThread>
typedef pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr RGBAPointCloudConstPtr;

 class myOpenNIViewer:public QObject
 {
   Q_OBJECT

 public:
   myOpenNIViewer():isWorking(false){}
   void cloud_cb_ (const RGBAPointCloudConstPtr &cloud)
   {
//       if (!viewer.wasStopped())
//         viewer.showCloud(cloud);
        emit newPointCloud(cloud);
   }
  ~myOpenNIViewer(){}
 signals:
    void newPointCloud(const RGBAPointCloudConstPtr &cloud);

 public slots:
    void setup(){
        interface = new pcl::OpenNIGrabber();
        boost::function<void (const RGBAPointCloudConstPtr&)> f =
        boost::bind (&myOpenNIViewer::cloud_cb_, this, _1);
        interface->registerCallback (f);
        interface->start();
        isWorking = true;
    }
    void startWork(){
        while(isWorking){
        boost::this_thread::sleep (boost::posix_time::seconds (1));
        }
    }
    void stopWork() {
        isWorking = false;
        interface->stop();
        delete interface;
    }
 private:
     pcl::Grabber* interface;
     bool isWorking;
//     pcl::visualization::CloudViewer viewer;
 };
#endif
