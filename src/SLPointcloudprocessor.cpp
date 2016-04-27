#include "SLPointcloudprocessor.h"

SLPointCloudProcessor::SLPointCloudProcessor():cloud(new pcl::PointCloud<pcl::PointXYZ>),
                                         RGBAcloud(new pcl::PointCloud<pcl::PointXYZRGBA>),processing(false)
{

}



void SLPointCloudProcessor::setup(){
    seg.setOptimizeCoefficients (true);
    // Mandatory
    seg.setModelType (pcl::SACMODEL_PLANE);
    seg.setMethodType (pcl::SAC_RANSAC);
    seg.setDistanceThreshold (0.01);
}
void SLPointCloudProcessor::receiveNewRGBAPointCloud(RGBAPointCloudPtr cloud_){
    // Recursively call self until latest event is hit
    busy = true;
    QCoreApplication::sendPostedEvents(this, QEvent::MetaCall);
    bool result = busy;
    busy = false;
    if(!result){
        std::cerr << "SLPointCloudProcessor: dropped Point Cloud!" << std::endl;
        return;
    }
    std::cout<<"to be filtered , received"<<std::endl;
    if(cloud_->points.empty()){std::cout<<"no Point"<<std::endl; return;}
////    time.start();
    pcl::copyPointCloud(*cloud_,*RGBAcloud);
    pcl::copyPointCloud(*RGBAcloud,*cloud);
//    int t = time.elapsed();

//    time.start();
    pcl::ModelCoefficients::Ptr coefficients (new pcl::ModelCoefficients);
    pcl::PointIndices::Ptr inliers (new pcl::PointIndices);
    seg.setInputCloud (cloud);
    seg.segment(*inliers, *coefficients);
    pcl::ExtractIndices<pcl::PointXYZRGBA> extract;
//    t = time.elapsed();
//    std::cout<<"segmented"<<t<<std::endl;
//    time.start();
    extract.setInputCloud (RGBAcloud);
    extract.setIndices (inliers);
    extract.setNegative (false);
    extract.filter(*RGBAcloud);
//    t = time.elapsed();
//    std::cout<<"filter ok"<<t<<std::endl;
//    std::cout<<RGBAcloud->size()<<std::endl;
    std::cout<<"processed"<<std::endl;
    emit NewpointcloudProcessed(RGBAcloud);
}
