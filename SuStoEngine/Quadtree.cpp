#include "Quadtree.h"
#include "Globals.h"
#include "GameObject.h"

Node::Node()
{
}

Node::~Node()
{
}

void Node::AddElement(GameObject * go)
{
	elements.push_back(go);
}

void Node::CreatePartition()
{
}

void Node::GetElements(std::vector<GameObject*>& elements) const
{
	std::vector<const Node*> nodes_to_visit;
	nodes_to_visit.push_back(this);

	while (!nodes_to_visit.empty())
	{
		std::vector<const Node*>::iterator node = nodes_to_visit.begin();

		// Add childs to visit later
		if ((*node)->left != nullptr && (*node)->right != nullptr)
		{
			nodes_to_visit.push_back((*node)->left);
			nodes_to_visit.push_back((*node)->right);
		}

		// Iterate through the list
		for (int i = 0; i < (*node)->elements.size(); ++i)
		{
			elements.push_back((*node)->elements[i]);
		}

		nodes_to_visit.erase(node);
	}
}
