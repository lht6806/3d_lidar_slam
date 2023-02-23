#include <iostream>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/filters/filter.h>
#include <pcl/filters/voxel_grid.h>

#include "point_cloud.h"

// ------------------------------------------------- //
std::ofstream outfile;
// ------------------------------------------------- //


int main (int argc, char** argv)
{
 	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud (new pcl::PointCloud<pcl::PointXYZ>);
 
  	if (pcl::io::loadPCDFile<pcl::PointXYZ> ("/home/riki/Test/pcl_read/rabbit.pcd", *cloud) == -1) {
    		PCL_ERROR ("Couldn't read file test_pcd.pcd \n");
    		return (-1);
  	}
  	
	std::cout << "Loaded "
            	<< cloud->width * cloud->height
            	<< " data points from test_pcd.pcd with the following fields: "
            	<< std::endl;
  
	pcl::PointCloud<pcl::PointXYZ>::Ptr filtered_pointcloud(new pcl::PointCloud<pcl::PointXYZ>);
 	pcl::VoxelGrid<pcl::PointXYZ> sor;

    	sor.setInputCloud(cloud);

    	sor.setLeafSize(0.5, 0.5, 0.5);

    	sor.filter(*filtered_pointcloud);
	std::cout<<"filtered point cloud size = "<<filtered_pointcloud->width * filtered_pointcloud->height<<std::endl;


	// storage
	outfile.open( "./rabbit", std::ios::app );
        if( !outfile.is_open() ){
                std::cerr<<"open file failed ..."<<std::endl;
                exit(-1);
        }

	std::cout<<"filtered_pointcloud->header.stamp = "<<filtered_pointcloud->header.stamp<<std::endl;

        slam::PointCloud<slam::Point3F> point_cloud( filtered_pointcloud->header.stamp, filtered_pointcloud->width, filtered_pointcloud->height );

        outfile.write( reinterpret_cast<char *>( &point_cloud.time_stamp ), sizeof( point_cloud.time_stamp ) );
        outfile.write( reinterpret_cast<char *>( &point_cloud.width ), sizeof( point_cloud.width ) );
        outfile.write( reinterpret_cast<char *>( &point_cloud.height ), sizeof( point_cloud.height ) );

        for ( size_t i = 0; i < filtered_pointcloud->points.size(); i ++ ) {
                point_cloud.points[i].x = filtered_pointcloud->points[i].x;
                point_cloud.points[i].y = filtered_pointcloud->points[i].y;
                point_cloud.points[i].z = filtered_pointcloud->points[i].z;
        }

	outfile.write( reinterpret_cast<char *>( point_cloud.points.data() ), point_cloud.points.size() * sizeof( slam::Point3F ) );

	// end storage

	pcl::visualization::CloudViewer viewer("Simple Cloud Viewer");//����һ����ʾ����
  
	viewer.showCloud(filtered_pointcloud);					//��ʾ����
  
	while(!viewer.wasStopped()) {  
  	
	}
 
  	return (0);
}
