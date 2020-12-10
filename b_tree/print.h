#ifndef PRINT_H
#define PRINT_H

#include <cstdio>

inline void print_tabs(const int num);

template <class T>
void b_tree<T>::print(const int level, const int num_values, const int num_childs) const
{
	if( root==nullptr || level<=0 )
		return;

	print_node(root, level, num_values, num_childs);
}

template <class T>
void b_tree<T>::print_node(const b_tree_node<T> *node, const int max_level,
	const int max_num_values, const int max_num_childs, int level) const
{
	print_tabs(level - 1);
	node->print(max_num_values);

	if( node->childs==nullptr )
		return;
	if( level==max_level )
	{
		print_tabs(level);
		printf("...\n");
		return;
	}

	bool print_all;
	int num_childs;
	if( (node->size + 1)<=max_num_childs )
	{
		num_childs = node->size + 1;
		print_all = true;
	} else
	{
		num_childs = max_num_childs;
		print_all = false;
	}

	for( int i = 0; i<num_childs; i++ )
	{
		print_node(node->childs[i], max_level, max_num_values, max_num_childs, level + 1);
	}

	if( !print_all )
	{
		print_tabs(level);
		printf("...\n");
	}
}

template <class T>
void b_tree_node<T>::print(int max_num_values) const
{
	if( size<=0 )
	{
		printf("{}\n");
		return;
	}
	if( max_num_values<=0 )
	{
		printf("{...}\n");
		return;
	}

	bool print_all = false;
	if( size<=max_num_values )
	{
		max_num_values = size;
		print_all = true;
	}

	printf("{");
	values[0].print();
	for( int i = 1; i<max_num_values; i++ )
	{
		printf(", ");
		values[i].print();
	}
	if( !print_all )
		printf(", ...");
	printf("}\n");
}

inline void print_tabs(const int num)
{
	for( int i = 0; i<num; i++ )
	{
		printf("\t");
	}
}

#endif// PRINT_H
