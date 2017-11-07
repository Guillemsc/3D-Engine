#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include <vector>
#include "GeometryMath.h"

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
		Node();
		~Node();

		void AddElement(GameObject* go);

		void CreatePartition();

		void GetElements(std::vector<GameObject*>& elements) const;

	public:
		Plane cut_plane;
		Axis axis = Axis::a_null;
		std::vector<GameObject*> elements;

	private:
		Node* left = nullptr;
		Node* right = nullptr;
		Node* parent = nullptr;
	};
public:

};


#endif // !_QUADTREE_H_