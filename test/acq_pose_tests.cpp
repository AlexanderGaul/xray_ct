#define CATCH_CONFIG_MAIN

#include <iostream>
#include <math.h>
#include <Eigen/Dense>

#include "catch.hpp"
#include "AcquisitionPose.h"


bool approx(Eigen::Vector3f v1, Eigen::Vector3f v2)
{
	return
		(v1(0) == Approx(v2(0))) &&
		(v1(1) == Approx(v2(1))) &&
		(v1(2) == Approx(v2(2)));
}

TEST_CASE("")
{}

TEST_CASE("")
{

	AcquisitionPose pose(2, 2, 2, 4, 4);

	Eigen::Hyperplane<float, 3> plane(Eigen::Vector3f(1.f, 0.f, 0.f), Eigen::Vector3f(1.f, 0.f, 0.f));
	pose.setRotation(0.f);
	pose.setCenter(Eigen::Vector3f(0.f, 0.f, 0.f));

	pose.getRay(0, 0).distance(pose.getDetectorUpperLeft()) == Approx(sqrt(2) * 0.5f);
	REQUIRE(approx(pose.getRay(0, 0).intersectionPoint(plane), Eigen::Vector3f(1.f, 0.75f, 0.75f) ));
	REQUIRE(approx(pose.getDetectorUpperLeft(), Eigen::Vector3f(1.f, 1.f, 1.f)));

	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			REQUIRE(pose.getRay(i, j).distance(pose.getDetectorCenter()) == 
				Approx(pose.getRay(3 - i, 3 - j).distance(pose.getDetectorCenter())));
		}
	}


	pose.setRotation(M_PI / 2.f);
	pose.setCenter(Eigen::Vector3f(2.f, 2.f, -2.f));

	plane = Eigen::Hyperplane<float, 3>(Eigen::Vector3f(0.f, 1.f, 0.f), Eigen::Vector3f(2.f, 3.f, -2.f));

	
	REQUIRE(approx(pose.getRay(0, 0).intersectionPoint(plane), Eigen::Vector3f(-0.75f + 2.f, 3.f, 0.75f - 2.f) ));
	REQUIRE(approx(pose.getDetectorUpperLeft(), Eigen::Vector3f(1.f, 3.f, -1.f)));
	REQUIRE(approx(pose.getDetectorCenter(), Eigen::Vector3f(2.f, 3.f, -2.f)));
	REQUIRE(approx(pose.getDetectorLowerRight(), Eigen::Vector3f(3.f, 3.f, -3.f)));
	REQUIRE(approx(pose.getSourcePosition(), Eigen::Vector3f(2.f, 1.f, -2.f)));

	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			REQUIRE(pose.getRay(i, j).distance(pose.getDetectorCenter()) == 
				Approx(pose.getRay(3 - i, 3 - j).distance(pose.getDetectorCenter())));
		}
	}

	//REQUIRE(approx(pose.getDetectorCenter(), Eigen::Vector3f(2.f, 2.f + 1.f, -2.f)));
	//REQUIRE(approx(pose.getDetectorUpperLeft(), Eigen::Vector3f(2.f - 1.f, 2.f, -2.f)));

	//REQUIRE(pose.getRay(0, 0).distance(pose.getDetectorUpperLeft()) == Approx(sqrt(2) * 0.25f));
	//REQUIRE(pose.getRay(0, 0).distance(pose.getDetectorCenter()) == Approx(sqrt(2) * 0.75f));




	pose.setRotation(1.f);
	pose.setCenter(Eigen::Vector3f(2.5f, 10.f, -8.f));

	Eigen::Vector3f normal = pose.getDetectorCenter() - pose.getSourcePosition();
	normal.normalize();
	plane = Eigen::Hyperplane<float, 3>(normal, Eigen::Vector3f(2.5f, 10.f, -8.f) + normal);



	Eigen::Vector3f distance = pose.getRay(0, 0).intersectionPoint(plane) - pose.getDetectorUpperLeft();

	REQUIRE(distance.norm() == Approx(sqrt(2) * 0.25f));

	//distance = pose.getRay(0, 0).intersectionPoint(plane) - 

	distance = pose.getDetectorUpperLeft() - pose.getDetectorCenter();
	REQUIRE(distance.norm() == Approx(sqrt(2)));
	

	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 2; j++)
		{
			REQUIRE(pose.getRay(i, j).distance(pose.getDetectorCenter()) == 
				Approx(pose.getRay(3 - i, 3 - j).distance(pose.getDetectorCenter())));
		}
	}

}