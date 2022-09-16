#include "ModelData3D.h"

//Constuctor
ModelData3D::ModelData3D() :
mesh_(NULL),
player_(NULL),
model_scene_(NULL),
renderer_3d_(NULL),
manager(NULL)
{

}

//Getter for the model scene
gef::Scene* ModelData3D::GetScene()
{
	return model_scene_;
}

//Getter for the model mesh
gef::Mesh* ModelData3D::GetMesh()
{
	return mesh_;
}

//Getter for the final model pose
gef::SkeletonPose ModelData3D::GetPose()
{
	return finalPose;
}

//Getter foir the skinned mesh
gef::SkinnedMeshInstance* ModelData3D::GetSkinnedInstance()
{
	return player_;
}

//Init the model data and setup scene
void ModelData3D::Init(gef::Platform& platform)
{
	renderer_3d_ = gef::Renderer3D::Create(platform);
	manager = gef::InputManager::Create(platform);

	SetupLights();

	// create a new scene object and read in the data from the file
	// no meshes or materials are created yet
	// we're not making any assumptions about what the data may be loaded in for
	model_scene_ = new gef::Scene();
	model_scene_->ReadSceneFromFile(platform, "xbot/xbot.scn");

	// we do want to render the data stored in the scene file so lets create the materials from the material data present in the scene file
	model_scene_->CreateMaterials(platform);

	// if there is mesh data in the scene, create a mesh to draw from the first mesh
	mesh_ = GetFirstMesh(model_scene_,platform);



	gef::Skeleton* skeleton = GetFirstSkeleton(model_scene_);
		
	if (skeleton)
	{
		player_ = new gef::SkinnedMeshInstance(*skeleton);
		//for (auto it : animData)
		//{
			//it->GetPlayer().Init(player_->bind_pose());
			//animPlayers.push_back(it->GetPlayer());
		//}
		finalPose = player_->bind_pose();
		player_->set_mesh(mesh_);

	}
	SetupCamera();

	//currentPlay = animPlayers.at(animIter);
	//nextPlay = animPlayers.at(animIter + 1);
	
}

//Input to control aniamtion iterations
void ModelData3D::Input()
{
	manager->Update();

	gef::Keyboard* keyboard = manager->keyboard();
	if (keyboard)
	{
		if (keyboard->IsKeyDown(gef::Keyboard::KC_ESCAPE))
		{
			exit(0);
		}

		if (keyboard->IsKeyDown(gef::Keyboard::KC_SPACE))
		{
 			blending = false;
 		}
	}


}

//Plays a single animation and when space is pressed, blends with the next anim
void ModelData3D::Update(float frame_time, gef::SkeletonPose treeOutput)
{
	fps_ = 1.0f / frame_time;

	//----- Commented code here handles animation playing and blending before blend tree------

	/*
	if (player_)
	{
		
	// update the pose in the anim player from the animation
		currentPlay.Update(frame_time, player_->bind_pose());
		nextPlay.Update(frame_time, player_->bind_pose());


		if (blending)
		{
			finalPose.Linear2PoseBlend(currentPlay.pose(), nextPlay.pose(), alpha);
			player_->UpdateBoneMatrices(finalPose);
			alpha += frame_time / 2;
			if (alpha >= 1)
			{
				//Resets animation iterator
				animIter++;
				if (animIter == 3)
					animIter = -1;
				currentPlay = nextPlay;
				nextPlay = animPlayers.at(animIter+1);
				blending = false;
				alpha = 0;
			}
		}
		else
		{
			player_->UpdateBoneMatrices(currentPlay.pose());
		}

		// update the bone matrices that are used for rendering the character
		// from the newly updated pose in the anim player
	}
	*/

	//Update the bones from the blend tree
	player_->UpdateBoneMatrices(treeOutput);
	
	// build a transformation matrix that will position the character
	// use this to move the player around, scale it, etc.
	if (player_)
	{
		gef::Matrix44 player_transform;
		player_transform.SetIdentity();
		player_->set_transform(player_transform);
	}
}

//renders the model and the scene
void ModelData3D::Render(gef::Platform& platform, bool render)
{
	// setup view and projection matrices
	gef::Matrix44 projection_matrix;
	gef::Matrix44 view_matrix;
	projection_matrix = platform.PerspectiveProjectionFov(camData.camera_fov_, (float)platform.width() / (float)platform.height(), camData.near_plane_, camData.far_plane_);
	view_matrix.LookAt(camData.camera_eye_, camData.camera_lookat_, camData.camera_up_);
	renderer_3d_->set_projection_matrix(projection_matrix);
	renderer_3d_->set_view_matrix(view_matrix);

	// draw meshes here
	renderer_3d_->Begin();

	// draw the player, the pose is defined by the bone matrices
	if (player_ && render)
		renderer_3d_->DrawSkinnedMesh(*player_, player_->bone_matrices());

	renderer_3d_->End();
	
};

//Clean up for pointers
void ModelData3D::Cleanup()
{
	delete mesh_;
	mesh_ = NULL;

	delete player_;
	player_ = NULL;

	delete model_scene_;
	model_scene_ = NULL;

	delete renderer_3d_;
	renderer_3d_ = NULL;
}

//Ligth setup
void ModelData3D::SetupLights()
{
	gef::PointLight default_point_light;
	default_point_light.set_colour(gef::Colour(0.7f, 0.7f, 1.0f, 1.0f));
	default_point_light.set_position(gef::Vector4(-300.0f, -500.0f, 100.0f));

	gef::Default3DShaderData& default_shader_data = renderer_3d_->default_shader_data();
	default_shader_data.set_ambient_light_colour(gef::Colour(0.5f, 0.5f, 0.5f, 1.0f));
	default_shader_data.AddPointLight(default_point_light);
}

//Getter for the scene skeleton
gef::Skeleton* ModelData3D::GetFirstSkeleton(gef::Scene* scene)
{
	gef::Skeleton* skeleton = NULL;
	if (scene)
	{
		// check to see if there is a skeleton in the the scene file
		// if so, pull out the bind pose and create an array of matrices
		// that wil be used to store the bone transformations
		if (scene->skeletons.size() > 0)
			skeleton = scene->skeletons.front();
	}

	return skeleton;
}

//Getter for the scene mesh
gef::Mesh* ModelData3D::GetFirstMesh(gef::Scene* scene, gef::Platform& platform)
{
	gef::Mesh* mesh = NULL;

	if (scene)
	{
		// now check to see if there is any mesh data in the file, if so lets create a mesh from it
		if (scene->mesh_data.size() > 0)
			mesh = model_scene_->CreateMesh(platform, scene->mesh_data.front());
	}

	return mesh;
}

//Camera setup
void ModelData3D::SetupCamera()
{
	// initialise the camera settings
	camData.camera_eye_ = gef::Vector4(-300.0f, 200.0f, 400.0f);
	camData.camera_lookat_ = gef::Vector4(-1,100,0);
	camData.camera_up_ = gef::Vector4(0.0f, 1.0f, 0.0f);
	camData.camera_fov_ = gef::DegToRad(45.0f);
	camData.near_plane_ = 0.01f;
	camData.far_plane_ = 1000.f;
}

