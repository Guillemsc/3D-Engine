#ifndef _QUADTREE_H_
#define _QUADTREE_H_

#include <vector>
#include <list>
#include "GeometryMath.h"

class GameObject;
class EngineQuadTreeNode;

class EngineQuadTree
{
public:
	EngineQuadTree();
	virtual ~EngineQuadTree();

	void Create(const AABB& limits);
	void Clear();
	void Insert(GameObject* go);
	void Remove(GameObject* go);
	template<typename T>
	void Intersect(std::vector<GameObject*>&, const T& PRIMITIVE);

private:

public:
	EngineQuadTreeNode* root = nullptr;
	
};

class EngineQuadTreeNode {
public:
	EngineQuadTreeNode(const AABB& limits);
	virtual ~EngineQuadTreeNode();

	bool IsLeaf();
	void Insert(GameObject* go);

private:

public:
	AABB limits;
	EngineQuadTreeNode* parent = nullptr;
	EngineQuadTreeNode* childs[4];
	std::list<GameObject*> objects;
};


template<typename T>
inline void EngineQuadTree::Intersect(std::vector<GameObject*>&, const T & PRIMITIVE)
{
}


#endif // !_QUADTREE_H_