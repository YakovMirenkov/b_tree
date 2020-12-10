#ifndef STUDENT_H
#define STUDENT_H

#include <cstdio>

class student
{
private:
	char *name = nullptr;
	int value = 0;

	int init(const char *new_name, int new_value);
	void rm();
public:
	enum RETURN_CODES
	{
		SUCCESS,
		EOF_ERROR,
		READ_ERROR,
		ALLOC_ERROR
	};

	student(const char *new_name = nullptr, int new_value = 0) { init(new_name, new_value); }
	student(const student &x) { init(x.name, x.value); }
	student(student &&x) { name = x.name; x.name = nullptr; value = x.value; }
	~student() { rm(); }
	int read(FILE *in);
	int get_value() const { return value; }
	const char *get_name() const { return const_cast<const char *>(name); }
	void print() const;

	student &operator=(const student &x);
	student &operator=(student &&x);
	int operator>(const student &x) const;
	long long int cmp(const student &x) const;
	void swap(student &x);
};

#endif// STUDENT_H
