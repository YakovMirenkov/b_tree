#ifndef MOVE_H
#define MOVE_H

// Move array one position to the right
template <class T>
void move(T *array, int size)
{
	for( int i = size; i>0; i-- )
	{
		array[i] = static_cast<T &&>(array[i - 1]);
	}
}

#endif
