#include <ros.h>
#include <mike_msgs/Encoders.h>
#include <std_msgs/Empty.h>

#include <Encoder.h>

Encoder encoderLeft(6, 5);
Encoder encoderRight(8, 7);

ros::NodeHandle  nh;

void resetCallback( const std_msgs::Empty& reset)
{
  encoderLeft.write(0);
  encoderRight.write(0);
}

ros::Subscriber<std_msgs::Empty> sub_reset("reset", resetCallback);

mike_msgs::Encoders encoder_ticks;
ros::Publisher pub_encoders("encoder_ticks", &encoder_ticks);

void setup() 
{
  nh.initNode();
  nh.advertise(pub_encoders);
  nh.subscribe(sub_reset);

  while (!nh.connected())
  {
    nh.spinOnce();
  }
  std_msgs::Empty reset;
  resetCallback(reset);
  delay(1);
}

long positionLeft  = -100;
long positionRight = -100;

void loop() {  
  long newLeft, newRight;
  newLeft = encoderLeft.read();
  newRight = encoderRight.read();

  encoder_ticks.ticks[0] = newLeft;
  encoder_ticks.ticks[1] = newRight;
  pub_encoders.publish(&encoder_ticks);
  nh.spinOnce();
  
  delay(5);
  
  if (newLeft != positionLeft || newRight != positionRight) {
    positionLeft = newLeft;
    positionRight = newRight;
  }
}