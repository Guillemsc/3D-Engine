#include "Quadtree.h"
#include "Globals.h"
#include "GameObject.h"
#include "Functions.h"
#include "ComponentMesh.h"

KDTree::Node::Node()
{
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
	if (left != nullptr && right != nullptr) 
	{
		left = new Node();
		right = new Node();

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

		// Get max and min point of the AABB depending on the axis
		std::vector<float> points;
		for (int i = 0; i < elements.size(); ++i)
		{
			if (!elements[i]->ContainsComponent(ComponentType::MESH))
				continue;

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
		float mid_point = 0.0f;
		for (int i = 0; i < points.size(); ++i)
		{
			mid_point += points[i];
		}
		mid_point /= points.size();

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
		while (!elements.empty())
		{
			uint node = 0; // new node determine
			AABB box = elements[elements.size() - 1]->GetBbox();
			switch (axis)
			{
			case Node::a_x:
				if (box.MaxX() > mid_point)
				{
					if (box.MinX() > mid_point)
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
				if (box.MaxY() > mid_point)
				{
					if (box.MinY() > mid_point)
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
				if (box.MaxZ() > mid_point)
				{
					if (box.MinZ() > mid_point)
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
				right->AddElement(*elements.end());
				break;
			case 0:
				left->AddElement(*elements.end());
				right->AddElement(*elements.end());
				break;
			case 1:
				left->AddElement(*elements.end());
				break;
			}

			elements.pop_back();
		}
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
