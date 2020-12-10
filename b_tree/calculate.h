#ifndef CALCULATE_H
#define CALCULATE_H

template <class T>
int b_tree<T>::total_size_num_childs(const int num_childs) const
{
	if( root==nullptr || num_childs<0 )
		return 0;
	return b_tree<T>::total_size_num_childs_node(root, num_childs);
}

template <class T>
int b_tree<T>::total_size_num_childs_node(const b_tree_node<T> *node, const int num_childs)
{
	int total = 0;

	if( node->childs!=nullptr )
	{
		if( num_childs==(node->size + 1) )
			total = node->size;

		for( int i = 0; i<=node->size; i++ )
		{
			total += b_tree<T>::total_size_num_childs_node(node->childs[i], num_childs);
		}
	} else if( num_childs==0 )
	{
		total = node->size;
	}

	return total;
}


template <class T>
int b_tree<T>::total_size_sbtrs_nodes_ng(const int max) const
{
	if( root==nullptr || max<=0 )
		return 0;
	int num_nodes, num_values;
	return b_tree<T>::total_size_sbtrs_nodes_ng_node(root, max, num_nodes, num_values);
}

template <class T>
int b_tree<T>::total_size_sbtrs_nodes_ng_node(const b_tree_node<T> *node, const int max, int &num_nodes, int &num_values)
{
	int total = 0;
	num_nodes = 1;
	num_values = 0;

	if( node->childs!=nullptr )
	{
		int i = 0, child_num_nodes, child_num_values;
		while( i<=node->size )
		{
			total += b_tree<T>::total_size_sbtrs_nodes_ng_node(node->childs[i], max, child_num_nodes, child_num_values);
			i++;
			num_nodes += child_num_nodes;
			if( num_nodes>max )
			{
				while( i<=node->size )
				{
					total += b_tree<T>::total_size_sbtrs_nodes_ng_node(node->childs[i], max, child_num_nodes, child_num_values);
					i++;
				}
				return total;
			}
			num_values += child_num_values;
		}
	}

	num_values += node->size;
	total += num_values;
	return total;
}


template <class T>
int b_tree<T>::total_size_sbtrs_levels_ng(const int max) const
{
	if( root==nullptr || max<=0 )
		return 0;
	int num_levels, num_values;
	return b_tree<T>::total_size_sbtrs_levels_ng_node(root, max, num_levels, num_values);
}

template <class T>
int b_tree<T>::total_size_sbtrs_levels_ng_node(const b_tree_node<T> *node, const int max, int &num_levels, int &num_values)
{
	int total = 0;
	num_levels = 1;
	num_values = 0;

	if( node->childs!=nullptr )
	{
		int child_num_values;
		total += b_tree<T>::total_size_sbtrs_levels_ng_node(node->childs[0], max, num_levels, child_num_values);
		num_values += child_num_values;
		for( int i = 1; i<=node->size; i++ )
		{
			total += b_tree<T>::total_size_sbtrs_levels_ng_node(node->childs[i], max, num_levels, child_num_values);
			num_values += child_num_values;
		}
		num_levels++;
	}

	num_values += node->size;
	if( num_levels<=max )
		total += num_values;
	return total;
}


template <class T>
int b_tree<T>::total_size_sbtrs_width_ng(const int max) const
{
	if( root==nullptr || max<=0 )
		return 0;
	int width, num_values;
	return b_tree<T>::total_size_sbtrs_width_ng_node(root, max, width, num_values);
}

template <class T>
int b_tree<T>::total_size_sbtrs_width_ng_node(const b_tree_node<T> *node, const int max, int &width, int &num_values)
{
	int total = 0;
	num_values = 0;

	if( node->childs!=nullptr )
	{
		int i = 0, child_width, child_num_values;
		width = 0;

		while( i<=node->size )
		{
			total += b_tree<T>::total_size_sbtrs_width_ng_node(node->childs[i], max, child_width, child_num_values);
			i++;
			width += child_width;
			if( width>max )
			{
				while( i<=node->size )
				{
					total += b_tree<T>::total_size_sbtrs_width_ng_node(node->childs[i], max, child_width, child_num_values);
					i++;
				}
				return total;
			}
			num_values += child_num_values;
		}
	} else
	{
		width = 1;
	}

	num_values += node->size;
	total += num_values;
	return total;
}


template <class T>
int b_tree<T>::num_values_level(const int level) const
{
	if( root==nullptr || level<1 )
		return 0;
	return b_tree<T>::num_values_level_node(root, level);
}

template <class T>
int b_tree<T>::num_values_level_node(const b_tree_node<T> *node, const int level)
{
	if( level==1 )
		return node->size;

	int num = 0;
	if( node->childs!=nullptr )
	{
		for( int i = 0; i<=node->size; i++ )
		{
			num += b_tree<T>::num_values_level_node(node->childs[i], level - 1);
		}
	}

	return num;
}


template <class T>
int b_tree<T>::total_size_paths_len(const int len) const
{
	if( root==nullptr || len<=0 )
		return 0;

	b_tree_node<T> *tmp = root;

	for( int i = 0; i<(len - 1); i++ )
	{
		if( tmp->childs==nullptr )
			return 0;
		tmp = tmp->childs[0];
	}
	if( tmp->childs!=nullptr )
		return 0;

	int num_ends;
	return b_tree<T>::total_size_paths_node(root, num_ends);
}

template <class T>
int b_tree<T>::total_size_paths_node(const b_tree_node<T> *node, int &num_ends)
{
	num_ends = 1;

	int total = 0;
	if( node->childs!=nullptr )
	{
		num_ends = 0;
		int child_num_ends;
		for( int i = 0; i<=node->size; i++ )
		{
			total += b_tree<T>::total_size_paths_node(node->childs[i], child_num_ends);
			num_ends += child_num_ends;
		}
	}

	return total + num_ends*node->size;
}

#endif// CALCULATE_H
