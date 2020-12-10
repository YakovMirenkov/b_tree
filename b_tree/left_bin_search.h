#ifndef BIN_SEARCH_H
#define BIN_SEARCH_H

// Find position: array[0..position]<*value<=array[(position + 1)..(size - 1)]
template <class T>
int bin_search(T *array, int size, T *value)
{
	int left = 0, right = size, middle;

	while( left<right )
	{
		middle = (left + right)/2;

		if( *value>array[middle] )
			left = middle + 1;
		else
			right = middle;
	}

	return right;
}

#endif
