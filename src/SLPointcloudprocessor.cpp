#include "SLPointcloudprocessor.h"

SLPointCloudProcessor::SLPointCloudProcessor():cloud(new pcl::PointCloud<pcl::PointXYZ>),
                                         RGBAcloud(new pcl::PointCloud<pcl::PointXYZRGBA>)
{
    seg.setOptimizeCoefficients (true);
    // Mandatory
    seg.setModelType (pcl::SACMODEL_PLANE);
    seg.setMethodType (pcl::SAC_RANSAC);
    seg.setDistanceThreshold (0.01);
}
void SLPointCloudProcessor::receiveNewRGBAPointCloud(RGBAPointCloudConstPtr cloud_){
//    std::cout<<"to be filtered , received"<<std::endl;
   if( pointcloudMutex.tryLock()){
//    time.start();
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
    emit NewpointcloudProcessed(RGBAcloud);
    pointcloudMutex.unlock();
    std::cout<<"processed"<<std::endl;
   }
   else{
       std::cout<<"lock failed"<<std::endl;
   }
}
