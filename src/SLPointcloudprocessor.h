#ifndef SLPOINTCLOUDPROCESSOR
#define SLPOINTCLOUDPROCESSOR

#include <pcl/point_types.h>
#include <pcl/filters/extract_indices.h>
#include <pcl/io/pcd_io.h>
#include <pcl/ModelCoefficients.h>
#include <pcl/sample_consensus/method_types.h>
#include <pcl/sample_consensus/model_types.h>
#include <pcl/segmentation/sac_segmentation.h>
 #include <pcl/visualization/cloud_viewer.h>
#include <QtCore>
#include <QMutex>
typedef pcl::PointCloud<pcl::PointXYZRGB>::Ptr PointCloudPtr;
typedef pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr PointCloudConstPtr;
typedef pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr RGBAPointCloudConstPtr;
typedef pcl::PointCloud<pcl::PointXYZRGBA>::Ptr RGBAPointCloudPtr;

class SLPointCloudProcessor:public QObject
{
    Q_OBJECT
public:
    explicit SLPointCloudProcessor();
signals:
    void NewpointcloudProcessed(RGBAPointCloudConstPtr RGBAcloud);
private slots:
    void receiveNewRGBAPointCloud(RGBAPointCloudConstPtr cloud_);
private:
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud ;
    pcl::PointCloud<pcl::PointXYZRGBA>::Ptr RGBAcloud ;
    // Create the segmentation object
    pcl::SACSegmentation<pcl::PointXYZ> seg;
    QTime time;
    QMutex pointcloudMutex;
    pcl::visualization::CloudViewer viewer;
};


#endif // SLPOINTCLOUDPROCESSOR

