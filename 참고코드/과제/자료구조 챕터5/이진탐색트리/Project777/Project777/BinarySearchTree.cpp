#include "BinarySearchTree.h"


BinarySearchTree::BinarySearchTree()
{
}

void BinarySearchTree::BSTMakeAndInit(BTreeNode **pRoot)
{
	*pRoot = NULL;
}

BSTData BinarySearchTree::BSTGetNodeData(BTreeNode *bst)
{
	return GetData(bst);
}

int BinarySearchTree::BSTInsert(BTreeNode **pRoot, BSTData data)
{
	BTreeNode * pNode = NULL;    // parent node
	BTreeNode * cNode = *pRoot;    // current node
	BTreeNode * nNode = NULL;    // new node

	// 새로운 노드가 추가될 위치를 찾는다. - 저장위치 찾기
	while (cNode != NULL)
	{
		if (data == GetData(cNode))
			return 1;    // 키의 중복을 허용하지 않음

		pNode = cNode;

		if (GetData(cNode) > data)
			cNode = GetLeftSubTree(cNode);
		else
			cNode = GetRightSubTree(cNode);
	}

	// pNode의 서브 노드에 추가할 새 노드의 생성
	nNode = MakeBTreeNode();    // 새 노드의 생성
	SetData(nNode, data);    // 새 노드에 데이터 저장

	// pNode의 서브 노드에 새 노드를 추가
	if (pNode != NULL)    // 새 노드가 루트 노드가 아니라면,
	{
		if (data < GetData(pNode))
			MakeLeftSubTree(pNode, nNode);
		else
			MakeRightSubTree(pNode, nNode);
	}
	else    // 새 노드가 루트 노드라면,
	{
		*pRoot = nNode;
	}
}

BTreeNode *BinarySearchTree::BSTSearch(BTreeNode *bst, BSTData target)
{
	BTreeNode * cNode = bst;    // current node
	BSTData cd;    // current data

	while (cNode != NULL)
	{
		cd = GetData(cNode);

		if (target == cd)
			return cNode;
		else if (target < cd)
			cNode = GetLeftSubTree(cNode);
		else
			cNode = GetRightSubTree(cNode);
	}

	return NULL;
}

void ShowIntData(int data)
{
	printf("%d ", data);
}

void BinarySearchTree::Show_All(BTreeNode *bst)
{
	InorderTraverse(bst, ShowIntData);
}

BTreeNode *BinarySearchTree::BSTRemove(BTreeNode ** pRoot, BSTData target)
{
	BTreeNode *pVRoot = MakeBTreeNode(); // 가상 루트 노드
	BTreeNode *pNode = pVRoot; // 부모노드
	BTreeNode *cNode = *pRoot; // 현재노드
	BTreeNode *dNode; // 삭제할 노드
}

BinarySearchTree::~BinarySearchTree()
{
}
