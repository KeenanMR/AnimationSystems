#include "ccd.h"
#include <algorithm>
#include <system/debug_log.h>
#include <maths/math_utils.h>


bool CalculateCCD(
	gef::SkeletonPose& pose,
	const gef::SkinnedMeshInstance& animatedModel,
	const gef::Vector4& destPoint,
	const std::vector<int>& boneIndices)
{
	std::vector<gef::Matrix44> global_pose;
	std::vector<gef::Matrix44> local_pose;
	local_pose.resize(pose.skeleton()->joint_count());
	global_pose = pose.global_pose();


	for (int i = 0; i < pose.skeleton()->joint_count(); ++i)
		local_pose[i] = pose.local_pose()[i].GetMatrix();

	//obtain the inverse of the animated model's transform
	gef::Matrix44 worldToModelTransform;
	worldToModelTransform.Inverse(animatedModel.transform());

	//...and use it to move the destination point from world space to model space
	gef::Vector4 destPointModelSpace = destPoint.Transform(worldToModelTransform);

	//Get the transform for the endEffector
	gef::Matrix44 endEffector = global_pose[boneIndices.back()];


	// Get the end effectors position
	gef::Vector4 endEffectorPos = endEffector.GetTranslation();

	//calculate the distance between the end effector's position and the destination position
	float distance = (endEffectorPos - destPointModelSpace).Length();

	float epsilon = 0.001f;
	//max number of iterations is used to stop CCD if there is no solution
	int maxIterations = 200;

	
	//perform the CCD algorithm if all the following conditions are valid
	/*
	- if the distance between the end effector point is greater than epsilon
	- we have not reached the maximum number of iterations
	*/
	while ((distance > epsilon) && (maxIterations > 0))
	{
		
		//if there are a valid number of bones
		if (boneIndices.size() > 1)
		{
			for (auto it = boneIndices.begin(); it != boneIndices.end(); ++it)
			{

				//First calculate the vector EB and DB
				gef::Vector4 EB, DB;

				//Vectors to store point data
				gef::Vector4 bonePos = global_pose[*it].GetTranslation();

				//Calculate vectors and normalise
				EB = endEffectorPos - bonePos;
				DB = destPointModelSpace - bonePos;
				if(EB.Length() > epsilon)
					EB.Normalise();
				if (DB.Length() > epsilon)
					DB.Normalise();

				//Calculate rotation angle and check validity
				float dotProduct = EB.DotProduct(DB);
				if (fabsf(dotProduct))
				{
					if (dotProduct < -1.f)
							dotProduct = -1.f;
					if (dotProduct > 1.f)
							dotProduct = 1.f;
					dotProduct = acosf(dotProduct);
				}
				dotProduct = std::max(dotProduct, 0.0f);
				dotProduct = std::min(dotProduct, gef::DegToRad(180.0f));
				

				//Calculate rotation axis
				gef::Vector4 crossProduct = EB.CrossProduct(DB);
				crossProduct.Normalise();

				if (dotProduct == 0)
					continue;

				//Calculate new bone rotation
				gef::Quaternion boneRotation = gef::Quaternion(crossProduct.x() *sinf(dotProduct / 2) , crossProduct.y()*sinf(dotProduct / 2), crossProduct.z()*sinf(dotProduct / 2), cosf(dotProduct / 2));

				//Calculate the new rotation and normalise
				gef::Quaternion currentRotation,newRotation;
				gef::Matrix44 currentBoneTransform = global_pose[*it];
				currentRotation.SetFromMatrix(currentBoneTransform);
				newRotation = currentRotation * boneRotation;
				newRotation.Normalise();

				//Set global pose to the new rotation and previous translation
				global_pose[*it].Rotation(newRotation);
				global_pose[*it].SetTranslation(currentBoneTransform.GetTranslation());

				//Recalculate all child bone transforms - currentParent = currentBoneTransfrom, newParent = newTransform

	

				for (auto childIt = it + 1; childIt != boneIndices.end(); ++childIt)
				{
					gef::Matrix44 currentChildTransform = global_pose[*childIt];
					gef::Matrix44 inverseParentTransform;
					inverseParentTransform.Inverse(currentBoneTransform);

					gef::Matrix44 localChildTransform = currentChildTransform * inverseParentTransform;

					global_pose[*childIt] = localChildTransform * global_pose[*(childIt - 1)];

					currentBoneTransform = currentChildTransform;

					endEffector = global_pose[boneIndices.back()];

					endEffectorPos = endEffector.GetTranslation();

					distance = (endEffectorPos - destPointModelSpace).Length();

					if (distance < epsilon)
						break;
				}

				
			}
		}
		//if a solution has not been reached, decrement the iterations counter
		--maxIterations;
	}


	//
	// This remain part of the function updates the gef::SkeletonPose with the newly calculate bone
	// transforms.
	// The gef::SkeletonPose interface wasn't originally designed to be updated in this way
	// You may wish to consider altering the interface to this class to remove redundant calculations

	// calculate new local pose of bones in IK chain
	for (size_t i = 0; i < boneIndices.size(); ++i)
	{
		int boneNum = boneIndices[i];

		const gef::Joint& joint = pose.skeleton()->joint(boneNum);
		if (joint.parent == -1)
		{
			local_pose[boneNum] = global_pose[boneNum];
		}
		else
		{
			gef::Matrix44 parentInv;
			parentInv.Inverse(global_pose[joint.parent]);
			local_pose[boneNum] = global_pose[boneNum] * parentInv;
		}
	}

	// recalculate global pose based on new local pose
	for (int i = 0; i < pose.skeleton()->joint_count(); ++i)
	{
		const gef::Joint& joint = pose.skeleton()->joint(i);
		if (joint.parent == -1)
			global_pose[i] = local_pose[i];
		else
			global_pose[i] = local_pose[i] * global_pose[joint.parent];
	}

	// update skeleton pose data structure
	pose.CalculateLocalPose(global_pose);
	pose.CalculateGlobalPose();


	if (maxIterations <= 0)
	{
		//if the iterations counter reaches 0 (or less), then a CCD solution was not found
		return false;
	}
	else
	{

		//if a solution was found, return true
		return true;
	}

}
