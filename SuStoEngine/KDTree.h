#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include <vector>
#include "GeometryMath.h"
#include "Globals.h"

class GameObject;

class KDTree
{
public:
	class Node
	{
	public:
		enum Axis
		{
			A_NULL,
			A_X,
			A_Y,
			A_Z
		};

	public:
		Node(uint partition_num);
		~Node();

		//Add an element to this node.
		void AddElement(GameObject* element);

		void CheckPartition();

		//fills elements vector with all elements of this node and child nodes
		void GetElements(std::vector<GameObject*>& elements) const;

		Node* GetLeft()const;
		Node* GetRight()const;

		void DrawPlane(int width, int height, float3 initial_translation);

	private:
		//Divides this node into 2 nodes that contais the elements (half on each node)
		void CreatePartition();
		void SetParent(Node* parent);

	public:
		Plane cut_plane;
		Axis axis = A_NULL;

	private:
		Node* right = nullptr;
		Node* left = nullptr;
		Node* parent = nullptr;

		std::vector<GameObject*> elements;

		uint partition_num = 1;
	};

public:
	KDTree();
	~KDTree();

	void CreateTree(std::vector<GameObject*>& elements, uint ele_on_partition = 5);

	void EraseTree();

	void GetElementsToTest(const Frustum& frustum) const;
	void GetElementsToTest(const Ray& ray) const;
	void GetElementsToTest(const AABB& box) const;

	bool HasTree()const;
	void DebugDraw()const;

private:
	Node* root_node = nullptr;

};

#endif
