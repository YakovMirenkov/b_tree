#ifndef ADD_VALUE_H
#define ADD_VALUE_H

#include "right_bin_search.h"
//#include "left_bin_search.h"
#include "move.h"
#include "copy.h"

template <class T>
int b_tree<T>::add_value(T *new_value)
{
	b_tree_node<T> *new_node = nullptr;
	int res_add = add_value_node(root, new_value, new_node);

	if( res_add!=NEW_NODE )
		return res_add;

	// root->size==2*base, creating new_root and adding new_node

	b_tree_node<T> *new_root;

	if( alloc_non_leaf(new_root)!=SUCCESS )
	{
		rm_node(new_node);
		rm();
		return ALLOC_ERROR;
	}

	new_root->values[0] = static_cast<T &&>(*new_value);
	new_root->childs[0] = root;
	new_root->childs[1] = new_node;
	new_root->size = 1;

	root = new_root;
	return SUCCESS;
}

template <class T>
int b_tree<T>::add_value_node(b_tree_node<T> *node, T *new_value, b_tree_node<T> *&new_node)
{
	int position = bin_search<T>(node->values, node->size, new_value);
	T *delimiter;

	if( node->childs==nullptr )
	{
		if( node->size<2*base )
		{
			move<T>(node->values + position, node->size - position);
			node->values[position] = static_cast<T &&>(*new_value);
			node->size++;
			return SUCCESS;
		}

		// node->size==2*base, creating new_node

		if( alloc_leaf(new_node)!=SUCCESS )
		{
			rm();
			return ALLOC_ERROR;
		}

		if( position==base )
		{
			copy<T>(new_node->values, node->values + base, base);
		} else
		{
			if( (delimiter = new T)==nullptr )
			{
				rm_node(new_node);
				rm();
				return ALLOC_ERROR;
			}

			if( position<base )
			{
				*delimiter = static_cast<T &&>(node->values[base - 1]);

				copy<T>(new_node->values, node->values + base, base);

				move<T>(node->values + position, node->size - position - 1);
				node->values[position] = static_cast<T &&>(*new_value);
			} else
			{// position>base
				*delimiter = static_cast<T &&>(node->values[base]);

				copy<T>(new_node->values, node->values + base + 1, position - base - 1);
				copy<T>(new_node->values + position - base, node->values + position, 2*base - position);
				new_node->values[position - base - 1] = static_cast<T &&>(*new_value);
			}

			*new_value = static_cast<T &&>(*delimiter);
			delete delimiter;
		}

		new_node->size = base;
		node->size = base;
		return NEW_NODE;
	}

	// node->childs!=nullptr

	b_tree_node<T> *new_child = nullptr;
	int res_add = add_value_node(node->childs[position], new_value, new_child);

	if( res_add!=NEW_NODE )
		return res_add;

	// adding new_child

	if( node->size<2*base )
	{
		move<T>(node->values + position, node->size - position);
		move<b_tree_node<T> *>(node->childs + position + 1, node->size - position);
		node->values[position] = static_cast<T &&>(*new_value);
		node->childs[position + 1] = new_child;
		node->size++;
		return SUCCESS;
	}

	// node->size==2*base, creating new_node

	if( alloc_non_leaf(new_node)!=SUCCESS )
	{
		rm_node(new_child);
		rm();
		return ALLOC_ERROR;
	}

	if( position==base )
	{
		copy<T>(new_node->values, node->values + base, base);
		copy<b_tree_node<T> *>(new_node->childs + 1, node->childs + base + 1, base);
		new_node->childs[0] = new_child;
	} else
	{
		if( (delimiter = new T)==nullptr )
		{
			rm_node(new_child);
			rm_node(new_node);
			rm();
			return ALLOC_ERROR;
		}

		if( position<base )
		{
			*delimiter = static_cast<T &&>(node->values[base - 1]);

			copy<T>(new_node->values, node->values + base, base);
			copy<b_tree_node<T> *>(new_node->childs, node->childs + base, base + 1);

			move<T>(node->values + position, node->size - position - 1);
			move<b_tree_node<T> *>(node->childs + position + 1, node->size - position - 1);
			node->values[position] = static_cast<T &&>(*new_value);
			node->childs[position + 1] = new_child;
		} else
		{// position>base
			*delimiter = static_cast<T &&>(node->values[base]);

			copy<T>(new_node->values, node->values + base + 1, position - base - 1);
			copy<b_tree_node<T> *>(new_node->childs, node->childs + base + 1, position - base);
			copy<T>(new_node->values + position - base, node->values + position, 2*base - position);
			copy<b_tree_node<T> *>(new_node->childs + position - base, node->childs + position, 2*base - position + 1);
			new_node->values[position - base - 1] = static_cast<T &&>(*new_value);
			new_node->childs[position - base] = new_child;
		}

		*new_value = static_cast<T &&>(*delimiter);
		delete delimiter;
	}

	new_node->size = base;
	node->size = base;
	return NEW_NODE;
}

#endif// ADD_VALUE_H
