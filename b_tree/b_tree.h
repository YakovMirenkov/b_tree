#ifndef B_TREE_H
#define B_TREE_H

#include <cstdio>

#define DEFAULT_BASE 1
#define DEFAULT_LEVEL_PRINT 0
#define DEFAULT_VALUES_PRINT 0
#define DEFAULT_CHILDS_PRINT 0

template <class T> class b_tree;
template <class T> class b_tree_node;

template <class T>
class b_tree_node
{
private:
	T *values = nullptr;// 2*base array
	b_tree_node **childs = nullptr;// nullptr or (2*base + 1) array
	int size = 0;// number of values
	// Number of childs is (size + 1) or 0 (for leaf nodes)
	// For all nodes except root base<=size<=2*base
public:
	b_tree_node() = default;
	void print(int max_values) const;

	friend class b_tree<T>;
};

template <class T>
class b_tree
{
private:
	b_tree_node<T> *root = nullptr;
	int base = DEFAULT_BASE;

	int alloc_leaf(b_tree_node<T> *&node);
	int alloc_non_leaf(b_tree_node<T> *&node);
	void rm();
	void rm_node(b_tree_node<T> *node);
	int add_value(T *new_value);
	int add_value_node(b_tree_node<T> *node, T *new_value, b_tree_node<T> *&new_node);
public:
	enum RETURN_CODES
	{
		SUCCESS,
		NEW_NODE,
		ALLOC_ERROR,
		OPEN_ERROR,
		READ_ERROR,
		BASE_ERROR
	};

	b_tree() = default;
	~b_tree() { rm(); }
	int set_base(int new_base);
	int get_base() const { return base; }
	int read(const char *filename);
	void print(const int level = DEFAULT_LEVEL_PRINT,
		const int num_values = DEFAULT_VALUES_PRINT, const int num_childs = DEFAULT_CHILDS_PRINT) const;
	void print_node(const b_tree_node<T> *node, const int max_level,
		const int max_values, const int max_childs, int level = 1) const;

	// Calculate:
	// Total number of values in nodes with num_childs childs
	int total_size_num_childs(const int num_childs) const;
	static int total_size_num_childs_node(const b_tree_node<T> *node, const int num_childs);
	// Total number of values in subtrees with number of nodes non-greater than max
	int total_size_sbtrs_nodes_ng(const int max) const;
	static int total_size_sbtrs_nodes_ng_node(const b_tree_node<T> *node, const int max, int &num_nodes, int &num_values);
	// Total number of values in subtrees with number of levels non-greater than max
	int total_size_sbtrs_levels_ng(const int max) const;
	static int total_size_sbtrs_levels_ng_node(const b_tree_node<T> *node, const int max, int &num_levels, int &num_values);
	// Total number of values in subtrees with width non-greater than max
	int total_size_sbtrs_width_ng(const int max) const;
	static int total_size_sbtrs_width_ng_node(const b_tree_node<T> *node, const int max, int &width, int &num_values);
	// Number of values on level
	int num_values_level(const int level) const;
	static int num_values_level_node(const b_tree_node<T> *node, const int level);
	// Total number of values in paths length len from root to leaf nodes
	int total_size_paths_len(const int len) const;
	static int total_size_paths_node(const b_tree_node<T> *node, int &num_ends);
};


#include "add_value.h"
#include "print.h"
#include "calculate.h"


template <class T>
int b_tree<T>::set_base(int new_base)
{
	if( new_base<1 )
		return BASE_ERROR;

	base = new_base;
	return SUCCESS;
}


template <class T>
int b_tree<T>::alloc_leaf(b_tree_node<T> *&node)
{
	if( (node = new b_tree_node<T>)==nullptr )
		return ALLOC_ERROR;

	if( (node->values = new T[2*base])==nullptr )
	{
		delete node;
		return ALLOC_ERROR;
	}

	return SUCCESS;
}

template <class T>
int b_tree<T>::alloc_non_leaf(b_tree_node<T> *&node)
{
	if( (node = new b_tree_node<T>)==nullptr )
		return ALLOC_ERROR;

	if( (node->values = new T[2*base])==nullptr )
	{
		delete node;
		return ALLOC_ERROR;
	}

	if( (node->childs = new b_tree_node<T> *[2*base + 1])==nullptr )
	{
		delete[] node->values;
		delete node;
		return ALLOC_ERROR;
	}

	return SUCCESS;
}


template <class T>
void b_tree<T>::rm()
{
	if( root==nullptr )
		return;
	rm_node(root);
	root = nullptr;
}

template <class T>
void b_tree<T>::rm_node(b_tree_node<T> *node)
{
	delete[] node->values;

	if( node->childs!=nullptr )
	{
		for( int i = 0; i<=node->size; i++ )
		{
			rm_node(node->childs[i]);
		}
		delete[] node->childs;
	}

	delete node;
}


template <class T>
int b_tree<T>::read(const char *filename)
{
	rm();

	FILE *in = fopen(filename, "r");
	if( !in )
		return OPEN_ERROR;

	T *new_value;
	if( (new_value = new T)==nullptr )
	{
		fclose(in);
		return ALLOC_ERROR;
	}

	int res_read;
	if( (res_read = new_value->read(in))!=T::SUCCESS )
	{
		delete new_value;
		fclose(in);
		switch( res_read )
		{
		case T::EOF_ERROR:
			return SUCCESS;
		case T::ALLOC_ERROR:
			return ALLOC_ERROR;
		default:
			return READ_ERROR;
		}
	}

	if( alloc_leaf(root)!=SUCCESS )
	{
		delete new_value;
		fclose(in);
		return ALLOC_ERROR;
	}

	root->values[0] = static_cast<T &&>(*new_value);
	root->size = 1;

	while( (res_read = new_value->read(in))==T::SUCCESS )
	{
		if( add_value(new_value)!=SUCCESS )
		{
			delete new_value;
			fclose(in);
			return ALLOC_ERROR;
		}
	}

	delete new_value;
	fclose(in);
	if( res_read==T::EOF_ERROR )
		return SUCCESS;
	rm();
	switch( res_read )
	{
	case T::ALLOC_ERROR:
		return ALLOC_ERROR;
	default:
		return READ_ERROR;
	}
}

#endif// B_TREE_H
