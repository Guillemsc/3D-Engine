#include "Quadtree.h"
#include "Globals.h"
#include "GameObject.h"

EngineQuadTree::EngineQuadTree()
{
}

EngineQuadTree::~EngineQuadTree()
{
	Clear();
}

void EngineQuadTree::Create(const AABB & limits)
{
}

void EngineQuadTree::Clear()
{
	RELEASE(root);
}

void EngineQuadTree::Insert(GameObject * go)
{
	if (root != nullptr)
	{
		if (go->local_bbox.Intersects(root->limits))
			root->Insert(go);
	}
}

void EngineQuadTree::Remove(GameObject * go)
{
}

EngineQuadTreeNode::EngineQuadTreeNode(const AABB & limits)
{
	this->limits = limits;
	for (int i = 0; i < 4; ++i)
	{
		childs[i] = nullptr;
	}
}

EngineQuadTreeNode::~EngineQuadTreeNode()
{
	for (int i = 0; i < 4; ++i)
	{
		if (childs[i] != nullptr)
			RELEASE(childs[i]);
	}	
}

bool EngineQuadTreeNode::IsLeaf()
{
	return childs[0] == nullptr;
}

void EngineQuadTreeNode::Insert(GameObject * go)
{
	if (IsLeaf() == true && (objects.size() < 8 || (limits.HalfSize().LengthSq() <= 64)))
		objects.push_back(go);
	else
	{
		if (IsLeaf() == true)
			// Create Separations / Childs

		objects.push_back(go);
	}
}
