#ifndef COPY_H
#define COPY_H

template <class T>
void copy(T *dest, T *source, int size)
{
	for( int i = 0; i<size; i++ )
	{
		dest[i] = static_cast<T &&>(source[i]);
	}
}

#endif
