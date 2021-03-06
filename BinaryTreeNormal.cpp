#pragma once

#include<stack>
#include<iostream>

struct TreeNode
{
	TreeNode* leftNode;
	TreeNode* rightNode;
	int data;
	int occurrences;
	TreeNode(int val) :data(val), occurrences(1), leftNode(nullptr), rightNode(nullptr) {}
	TreeNode(const TreeNode* node) : leftNode(nullptr), rightNode(nullptr) { data = node->data; occurrences = node->occurrences; }
};

class BinaryTree
{
private:
	TreeNode* m_root;
	void DeleteTree(TreeNode* root);
	void CopyTree(TreeNode* root);
	TreeNode* FindSmallestBiggerThanRoot(TreeNode* root = nullptr)const;
	TreeNode* FindFatherOfNode(TreeNode* node, TreeNode* root = nullptr)const;
public:
	BinaryTree() : m_root(nullptr) {}
	BinaryTree(int val) :m_root(new TreeNode(val)) {}
	~BinaryTree() { DeleteTree(m_root); }
	void add(int& val);
	void print(TreeNode* root)const;
	bool empty()const { return m_root == nullptr; }
	TreeNode* find(int val, TreeNode* root = nullptr)const;
	void DeleteNode(TreeNode* node);
};

void BinaryTree::DeleteTree(TreeNode* root)
{
	if (root)
	{
		std::stack<TreeNode*> nodes;
		nodes.push(root);
		TreeNode* temp;
		while (!nodes.empty())
		{
			temp = nodes.top();
			nodes.pop();
			if (temp->leftNode) nodes.push(temp->leftNode);
			if (temp->rightNode)nodes.push(temp->rightNode);
			delete temp;
		}
	}
	root = nullptr;
}

void BinaryTree::CopyTree(TreeNode * root)
{
	m_root = nullptr;
	if (root)
	{
		m_root = new TreeNode(root);
		m_root->occurrences = root->occurrences;
		std::stack<TreeNode*> originalNodes;
		std::stack<TreeNode*> copiedNodes;
		originalNodes.push(root);
		copiedNodes.push(m_root);
		while (!originalNodes.empty())
		{
			TreeNode* temp=originalNodes.top();
			while (temp->leftNode)
			{
				temp = temp->leftNode;
				originalNodes.push(temp);
				TreeNode* newNode = new TreeNode(temp);
				copiedNodes.top()->leftNode = newNode;
				copiedNodes.push(newNode);
			}
			while (!originalNodes.empty() && (!temp->rightNode || !temp->leftNode))
			{
				temp = originalNodes.top();
				originalNodes.pop();
				copiedNodes.pop();
			}
			if (temp->rightNode)
			{
				temp = temp->rightNode;
				originalNodes.push(temp);
				TreeNode* newNode = new TreeNode(temp);
				copiedNodes.top()->rightNode = newNode;
				copiedNodes.push(newNode);
			}
		}
	}
}

TreeNode * BinaryTree::FindSmallestBiggerThanRoot(TreeNode * root)const
{
	if (!root) root = m_root;
	if (root->rightNode)
	{
		root = root->rightNode;
		while (root->leftNode)
		{
			root = root->leftNode;
		}
	}
	return root;
}

TreeNode* BinaryTree::FindFatherOfNode(TreeNode* node, TreeNode* root) const
{
	if (!node) return nullptr;
	if (!m_root) return nullptr;
	if (node == m_root) return m_root;
	if (!root) root = m_root;
	if (m_root)
	{
		TreeNode* temp = root;
		while (temp->leftNode != node && temp->rightNode != node)
		{
			if (node->data > temp->data) temp = temp->rightNode;
			else temp = temp->leftNode;
		}
		return temp;
	}
}

void BinaryTree::add(int& val)
{
	if (!m_root) m_root = new TreeNode(val);
	else
	{
		TreeNode* temp = m_root;
		while (true)
		{
			if (temp->data > val)
			{
				if (temp->leftNode) temp = temp->leftNode;
				else
				{
					temp->leftNode = new TreeNode(val);
					break;
				}
			}
			if (temp->data < val)
			{
				if (temp->rightNode) temp = temp->rightNode;
				else
				{
					temp->rightNode = new TreeNode(val);
					break;
				}
			}
			if (temp->data == val)
			{
				++temp->occurrences;
				break;
			}
		}
	}

}

void BinaryTree::print(TreeNode * root = nullptr) const
{
	if (!root) root = m_root;
	if (root)
	{
		std::stack<TreeNode*> nodes;
		nodes.push(root);
		while (!nodes.empty())
		{
			while (root->leftNode)
			{
				root = root->leftNode;
				nodes.push(root);
			}
			while ((!root->leftNode || !root->rightNode) && !nodes.empty())
			{
				root = nodes.top();
				nodes.pop();
				std::cout << root->data << " ";
			}
			if (root->rightNode)
			{
				root = root->rightNode;
				nodes.push(root);
			}
		}
	}
}

TreeNode * BinaryTree::find(int val, TreeNode * root) const
{
	if (!root) root = m_root;
	while (root)
	{
		if (root->data == val) return root;
		else if (root->data > val) root = root->leftNode;
		else root = root->rightNode;
	}
	return nullptr;
}


void BinaryTree::DeleteNode(TreeNode* node)
{
	bool IsRoot = node == m_root;
	if (IsRoot)
	{
		if (!m_root->leftNode && !m_root->rightNode)
		{
			delete m_root;
			m_root = nullptr;
		}
	}
	while (node->rightNode || node->leftNode)
	{
		TreeNode* temp = FindSmallestBiggerThanRoot(node);
		node->data = temp->data;
		node = temp;
		if (temp->rightNode)
		{
			node = temp;
		}
		else if (temp->leftNode)
		{
			TreeNode* secondTemp = temp->leftNode;
			while (secondTemp)
			{
				temp->data = secondTemp->data;
				temp = secondTemp;
				secondTemp = secondTemp->leftNode;
			}
			node = temp;
		}
	}
	TreeNode* temp;
	if (IsRoot)
	{
		if (node->data > m_root->leftNode->data)
			temp = FindFatherOfNode(node, m_root->rightNode);
		else temp = FindFatherOfNode(node, m_root->leftNode);
	}
	else temp = FindFatherOfNode(node);
	print(m_root); std::cout << std::endl;
	if (temp->rightNode && node->data == temp->rightNode->data)
		temp->rightNode = nullptr;
	else temp->leftNode = nullptr;
	delete node;
}

void test()
{
	BinaryTree tree;
	int arr[] = { 5,3,7,6,4,2,1,8,10,9 };
	for (int i = 0; i < sizeof(arr) / sizeof(int); ++i)
	{
		tree.add(arr[i]);
	}
	BinaryTree tree2(tree);
	tree2.print();
}
int main()
{
	test();
}