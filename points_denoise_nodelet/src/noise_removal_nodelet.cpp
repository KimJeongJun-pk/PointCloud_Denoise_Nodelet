#include <nodelet/nodelet.h>
#include <pluginlib/class_list_macros.h>
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl_conversions/pcl_conversions.h>
#include <pcl/point_types.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/filters/voxel_grid.h> 

namespace pointcloud_denoise_nodelet
{
class NoiseRemovalNodelet : public nodelet::Nodelet
{
public:
    virtual void onInit()
    {
        ros::NodeHandle& nh = getNodeHandle();
        ros::NodeHandle& private_nh = getPrivateNodeHandle();

        sub_ = nh.subscribe("/ilidar/points", 1, &NoiseRemovalNodelet::cloudCallback, this);
        pub_ = nh.advertise<sensor_msgs::PointCloud2>("/denoise_points", 1);

        private_nh.param("mean_k", mean_k_, 50);
        private_nh.param("std_dev_mul_thresh", std_dev_mul_thresh_, 1.0);
    }

private:
    void cloudCallback(const sensor_msgs::PointCloud2ConstPtr& msg)
{
    // 1. ROSMsg → PCL PointCloud
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>());
    pcl::fromROSMsg(*msg, *cloud);

    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_voxel(new pcl::PointCloud<pcl::PointXYZ>());
    pcl::VoxelGrid<pcl::PointXYZ> voxel;
    voxel.setInputCloud(cloud);
    voxel.setLeafSize(0.015f, 0.015, 0.015f); 
    voxel.filter(*cloud_voxel);

    pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor;
    sor.setInputCloud(cloud_voxel);           
    sor.setMeanK(mean_k_);
    sor.setStddevMulThresh(std_dev_mul_thresh_);

    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZ>());
    sor.filter(*cloud_filtered);

    sensor_msgs::PointCloud2 output;
    pcl::toROSMsg(*cloud_filtered, output);
    output.header = msg->header;
    pub_.publish(output);
}

    ros::Subscriber sub_;
    ros::Publisher pub_;
    int mean_k_;
    double std_dev_mul_thresh_;
};
}

PLUGINLIB_EXPORT_CLASS(pointcloud_denoise_nodelet::NoiseRemovalNodelet, nodelet::Nodelet)

