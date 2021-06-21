#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

// Define a global client that can request services
ros::ServiceClient client;

// This function calls the command_robot service to drive the robot in the specified direction
void drive_bot(float lin_x, float ang_z)
{

ROS_INFO_STREAM("DriveToPositionRequest");
    // TODO: Request a service and pass the velocities to it to drive the robot
	ball_chaser::DriveToTarget srv;
	srv.request.linear_x = lin_x;
	srv.request.angular_z = ang_z;
	
	if (!client.call(srv))
	{
        ROS_ERROR("Failed to call service ball_chaser");
	}
}

// This callback function continuously executes and reads the image data
void process_image_callback(const sensor_msgs::Image img)
{


bool white_color=false;
int col_value=0;
int white_pixel=255;
for(int i=0;i<img.height;i++)
{


int offset=img.step*i;

	for(int j=0;j<img.step;j+=3)
	{

		if(img.data[offset+j] == white_pixel && img.data[offset+j+1] == white_pixel  && img.data[offset+j+2] == white_pixel )
		{
			white_color=true;
			col_value=j/3;
	                break;
 		}
		

	}
	if(white_color==true)break;

}


if(!white_color){
		drive_bot(0.0,0.0);
		return;
	     }

if(col_value<(img.width/3))
		{	drive_bot(0.0,0.5);
			return;
		}

if(col_value>(img.width/3)*2)
		{
			drive_bot(0.0,-0.5);
			return;
		}
drive_bot(0.5,0.0);

}


    // TODO: Loop through each pixel in the image and check if there's a bright white one
    // Then, identify if this pixel falls in the left, mid, or right side of the image
    // Depending on the white ball position, call the drive_bot function and pass velocities to it
    // Request a stop when there's no white ball seen by the camera


int main(int argc, char** argv)
{
    // Initialize the process_image node and create a handle to it
    ros::init(argc, argv, "process_image");
    ros::NodeHandle n;

    // Define a client service capable of requesting services from command_robot
    client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

    // Subscribe to /camera/rgb/image_raw topic to read the image data inside the process_image_callback function
    ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

    // Handle ROS communication events
    ros::spin();

    return 0;
}
