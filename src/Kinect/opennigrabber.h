 #ifndef OPNIGRABBER
 #define OPNIGRABBER

 #include <pcl/io/openni_grabber.h>
 #include <pcl/visualization/cloud_viewer.h>
 #include <QObject>
// #include <QThread>

typedef pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr RGBAPointCloudConstPtr;
typedef pcl::PointCloud<pcl::PointXYZRGBA>::Ptr RGBAPointCloudPtr;
 class myOpenNIViewer:public QObject
 {
   Q_OBJECT

 public:
   myOpenNIViewer():isWorking(false),cloud_to_process(new pcl::PointCloud<pcl::PointXYZRGBA>){}
   void cloud_cb_ (const RGBAPointCloudConstPtr &cloud)
   {
       if(cloud_mutex.try_lock()){
       *(this->cloud_to_process) = *cloud;
       cloud_mutex.unlock();
       std::cout<<"raw point cloud emmited    "<<cloud_to_process->size()<<"    point"<<endl;
       emit(newPointCloud(this->cloud_to_process));
       }
   }
  ~myOpenNIViewer(){}
 signals:
    void newPointCloud(const RGBAPointCloudPtr &cloud);

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
     boost::mutex cloud_mutex;
     RGBAPointCloudPtr cloud_to_process;
//     pcl::visualization::CloudViewer viewer;
 };
#endif
