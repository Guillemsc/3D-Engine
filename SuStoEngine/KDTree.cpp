#include "KDTree.h"
#include "GameObject.h"
#include "Functions.h"
#include <algorithm>
#include "ComponentMesh.h"

KDTree::Node::Node(uint partition_num)
{
	this->partition_num = partition_num;
}

KDTree::Node::~Node()
{
}

void KDTree::Node::AddElement(GameObject * go)
{
	elements.push_back(go);
}

void KDTree::Node::CreatePartition()
{
	if (left == nullptr && right == nullptr) 
	{
		left = new Node(partition_num);
		left->SetParent(this);
		right = new Node(partition_num);
		right->SetParent(this);

		if (parent == nullptr)
			axis = Axis::a_x;
		else
		{
			// Decide Axis
			switch (parent->axis)
			{
			case Node::a_x:
				axis = Axis::a_z;
				break;
			case Node::a_y:
				axis = Axis::a_x;
				break;
			case Node::a_z:
				axis = Axis::a_y;
				break;
			}
		}
		// Get max and min point of the AABB depending on the axis
		std::vector<float> points;
		for (int i = 0; i < elements.size(); ++i)
		{
			switch (axis)
			{
			case Node::a_x:
				points.push_back(elements[i]->GetBbox().MaxX());
				points.push_back(elements[i]->GetBbox().MinX());
				break;
			case Node::a_y:
				points.push_back(elements[i]->GetBbox().MaxY());
				points.push_back(elements[i]->GetBbox().MinY());
				break;
			case Node::a_z:
				points.push_back(elements[i]->GetBbox().MaxZ());
				points.push_back(elements[i]->GetBbox().MinZ());
				break;
			}
		}

		// Calculate mid-point
		std::sort(points.begin(), points.end());
		float mid_point = points[points.size() / 2];

		// Create a point in the space with the mid-point and create the plane
		float3 plane_point = float3::zero;
		switch (axis)
		{
		case Node::a_x:
			plane_point.x = mid_point;
			cut_plane.Set(plane_point, math::vec::unitX);
			break;
		case Node::a_y:
			plane_point.y = mid_point;
			cut_plane.Set(plane_point, math::vec::unitY);
			break;
		case Node::a_z:
			plane_point.z = mid_point;
			cut_plane.Set(plane_point, math::vec::unitZ);
			break;
		}

		// Set the elements of the node to one or both of the created node
		for (vector<GameObject*>::iterator it = elements.begin(); it != elements.end(); ++it)
		{
			uint node = 0; // new node determine
			AABB box = (*it)->GetBbox();
			switch (axis)
			{
			case Node::a_x:
				if (box.MaxX() >= mid_point)
				{
					if (box.MinX() >= mid_point)
						node = 1;
					else
						node = 0;
				}
				else
				{
					if (box.MinX() < mid_point)
						node = -1;
					else
						node = 0;
				}
				break;
			case Node::a_y:
				if (box.MaxY() >= mid_point)
				{
					if (box.MinY() >= mid_point)
						node = 1;
					else
						node = 0;
				}
				else
				{
					if (box.MinY() < mid_point)
						node = -1;
					else
						node = 0;
				}
				break;
			case Node::a_z:
				if (box.MaxZ() >= mid_point)
				{
					if (box.MinZ() >= mid_point)
						node = 1;
					else
						node = 0;
				}
				else
				{
					if (box.MinZ() < mid_point)
						node = -1;
					else
						node = 0;
				}
				break;
			}

			switch (node)
			{
			case -1:
				right->AddElement(*it);
				break;
			case 0:
				left->AddElement(*it);
				right->AddElement(*it);
				break;
			case 1:
				left->AddElement(*it);
				break;
			}
		}

		left->CheckPartition();
		right->CheckPartition();
	}
	else
	{
		LOG_OUTPUT("K-D Tree: This node has partitions already");
	}

}

void KDTree::Node::GetElements(std::vector<GameObject*>& elements) const
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

void KDTree::Node::CheckPartition()
{
	if (elements.size() > partition_num)
	{
		CreatePartition();
	}
}

void KDTree::Node::SetParent(Node * parent)
{
	this->parent = parent;
}

const KDTree::Node * KDTree::Node::GetLeft() const
{
	return left;
}

const KDTree::Node * KDTree::Node::GetRight() const
{
	return right;
}

KDTree::KDTree()
{
}

KDTree::~KDTree()
{
}

void KDTree::CreateTree(const std::vector<GameObject*>& elements, uint num_partition)
{
	if (root == nullptr)
	{
		root = new Node(num_partition);

		for (int i = 0; i < elements.size(); ++i)
		{
			if (!elements[i]->ContainsComponent(ComponentType::MESH))
				continue;
			root->AddElement(elements[i]);
		}
		root->CheckPartition();
	}
}

void KDTree::EraseTree()
{
	std::vector<const Node*> nodes_to_visit;
	nodes_to_visit.push_back(root);

	while (!nodes_to_visit.empty())
	{
		std::vector<const Node*>::iterator node = nodes_to_visit.begin();

		// Add childs to visit later
		if ((*node)->GetLeft() != nullptr && (*node)->GetRight() != nullptr)
		{
			nodes_to_visit.push_back((*node)->GetLeft());
			nodes_to_visit.push_back((*node)->GetRight());
		}

		RELEASE(*node);

		nodes_to_visit.erase(node);
	}
}

void KDTree::GetElementsToTest(const Frustum & frustum) const
{
}

void KDTree::GetElementsToTest(const Ray & ray) const
{
}

void KDTree::GetElementsToTest(const AABB & aabb) const
{
}

bool KDTree::HasTree() const
{
	return root != nullptr;
}
