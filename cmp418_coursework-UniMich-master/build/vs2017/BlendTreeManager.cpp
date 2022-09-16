#include "BlendTreeManager.h"
#include "BlendTree.h"


//Initialises the blend tree and the nodes used
void BlendTreeManager::Init(gef::Platform& platform, std::vector<const char*> animNames, gef::SkinnedMeshInstance* skinnedMesh)
{
	
	manager = gef::InputManager::Create(platform);

	blendingNode = new LinearBlendNode;

	for (int i = 0; i < animNames.size(); i++)
	{
		ClipNode* newClip = new ClipNode;
		newClip->Init(platform, animNames.at(i));
		newClip->SetPoseMesh(skinnedMesh);
		clipNodes.push_back(newClip);
		if(i == 0 || i == 1)
		blendingNode->SetInput(clipNodes.at(i));
	}

	tree.InitInput(platform);
	tree.InitTreeVariables(clipNodes);
	tree.Init(skinnedMesh, blendingNode);
}


void BlendTreeManager::Input()
{
	if (manager)
	{
		manager->Update();

		// keyboard input
		gef::Keyboard* keyboard = manager->keyboard();
		if (keyboard)
		{
			if (keyboard->IsKeyDown(gef::Keyboard::KC_ESCAPE))
			{
				exit(0);
			}

			if (keyboard->IsKeyPressed(gef::Keyboard::KC_SPACE))
			{
				ResetClipNodes();
			}
		}
	}

	tree.Input(clipNodes.at(animIterator)->GetName());
}

void BlendTreeManager::Update(float deltaTime)
{
	tree.Update(deltaTime);
}

//Iterates through the anims to be blended, they are linked to 2 to showcase the variables table easily
//but could be extended easily to shoe them all blending together as seen in the AnimManger class
void BlendTreeManager::ResetClipNodes()
{
	ClipNode* newInputNodes[2];

	newInputNodes[0] = clipNodes.at(animIterator);
	if (animIterator == 3)
	{
		animIterator = 0;
		newInputNodes[1] = clipNodes.at(animIterator);
	}
	else
	{
		newInputNodes[1] = clipNodes.at(animIterator + 1);
		animIterator++;
	}
	
	blendingNode->UpdateInputNodes(newInputNodes);
	tree.UpdateInputNode(blendingNode);
}

//Returns the output pose from the blend tree
gef::SkeletonPose BlendTreeManager::GetOutputPose()
{
	return tree.Output();
}
