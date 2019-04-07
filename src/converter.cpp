#include <ros/ros.h>
#include <sensor_msgs/CompressedImage.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/PointCloud.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/image_encodings.h>
#include <sensor_msgs/NavSatFix.h>
#include <visualization_msgs/MarkerArray.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/Point32.h>
#include <geometry_msgs/Point.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseArray.h>
#include <geometry_msgs/Polygon.h>`
#include <geometry_msgs/Quaternion.h>
#include <image_transport/image_transport.h>
#include <sensor_msgs/point_cloud_conversion.h>

ros::Publisher velodynePublisher;


void chatterCallback(const sensor_msgs::PointCloud2 msg)
{
    sensor_msgs::PointCloud oldPointCloud, newPointCloud;

    sensor_msgs::convertPointCloud2ToPointCloud(msg, oldPointCloud);

    newPointCloud.header = oldPointCloud.header;

    for (geometry_msgs::Point32 point : oldPointCloud.points) {
        if ( ( point.x > -2 && point.x < 0 ) && (point.y > -1 && point.y < 1) ) continue;

        newPointCloud.points.push_back(point);
    }

    sensor_msgs::PointCloud2 newPointCloud2;
    sensor_msgs::convertPointCloudToPointCloud2(newPointCloud, newPointCloud2);

    velodynePublisher.publish(newPointCloud2);
}

int main( int argc, char ** argv )
{
    ros::init(argc, argv, "zcm2ros_publisher");

    ros::NodeHandle rosNode;

    velodynePublisher = rosNode.advertise<sensor_msgs::PointCloud2>("/velodyne_points", 2);

    ros::Subscriber apoloSubscriber = rosNode.subscribe("/apollo/sensor/velodyne16/PointCloud2", 2, chatterCallback);

    ros::spin();

}