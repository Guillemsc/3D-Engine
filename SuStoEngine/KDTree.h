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
			a_null,
			a_x,
			a_y,
			a_z
		};

	public:
		Node(uint partition_num);
		~Node();

		void AddElement(GameObject* go);

		void CreatePartition();

		void GetElements(std::vector<GameObject*>& elements) const;

		void CheckPartition();

		void SetParent(Node* parent);
		const Node* GetLeft() const;
		const Node* GetRight() const;
	public:
		Plane cut_plane;
		Axis axis = Axis::a_null;
		std::vector<GameObject*> elements;

		uint partition_num = 0;
	private:
		Node* left = nullptr;
		Node* right = nullptr;
		Node* parent = nullptr;
	};

public:
	KDTree();
	~KDTree();

	void CreateTree(const std::vector<GameObject*>& elements, uint num_partition);
	void EraseTree();

	void GetElementsToTest(const Frustum& frustum) const;
	void GetElementsToTest(const Ray& ray) const;
	void GetElementsToTest(const AABB& aabb) const;

	bool HasTree() const;

private:
	Node* root = nullptr;
};


#endif // !_QUADTREE_H_