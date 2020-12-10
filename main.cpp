#include <cstdio>
#include <ctime>
#include "b_tree/b_tree.h"
#include "student/student.h"

#define ERROR (-1)

#define SCAN_LEVEL_PRINT_FORMAT "-L%d"
#define SCAN_VALUES_PRINT_FORMAT "-V%d"
#define SCAN_CHILDS_PRINT_FORMAT "-C%d"

int find_level_print(const int size, const char **array, int *level_print)
{
	for( int i = 0; i<size; i++ )
	{
		if( sscanf(array[i], SCAN_LEVEL_PRINT_FORMAT, level_print)==1 )
			return 1;
	}

	return 0;
}
int find_values_print(const int size, const char **array, int *values_print)
{
	for( int i = 0; i<size; i++ )
	{
		if( sscanf(array[i], SCAN_VALUES_PRINT_FORMAT, values_print)==1 )
			return 1;
	}

	return 0;
}
int find_childs_print(const int size, const char **array, int *childs_print)
{
	for( int i = 0; i<size; i++ )
	{
		if( sscanf(array[i], SCAN_CHILDS_PRINT_FORMAT,  childs_print)==1 )
			return 1;
	}

	return 0;
}
int find_param(const int size, const char **array, int *param)
{
	for( int i = 0; i<size; i++ )
	{
		if( sscanf(array[i], "%d", param)==1 )
			return 1;
	}

	return 0;
}

int main(const int argc, const char **argv)
{
	int base, level_print = DEFAULT_LEVEL_PRINT, values_print = DEFAULT_VALUES_PRINT,
		childs_print = DEFAULT_CHILDS_PRINT, param;
	int param_set = find_param(argc - 3, argv + 2, &param);
	int num_options = param_set + find_level_print(argc - 3, argv + 2, &level_print) +
		find_values_print(argc - 3, argv + 2, &values_print) +
		find_childs_print(argc - 3, argv + 2, &childs_print);

	if( argc<3 || sscanf(argv[1], "%d", &base)!=1 || num_options!=(argc - 3) )
	{
		fprintf(stderr, "Usage: %s base [-Llevel_print] [-Vvalues_print] [-Cchilds_print] [parameter] filename\n", argv[0]);
		fprintf(stderr, "\tDefault: %s base -L%d -V%d -C%d [parameter] filename\n", argv[0],
			DEFAULT_LEVEL_PRINT, DEFAULT_VALUES_PRINT, DEFAULT_CHILDS_PRINT);
		return ERROR;
	}
	const char *filename = argv[argc - 1];
	b_tree<student> *tr;

	if( (tr = new b_tree<student>)==nullptr )
	{
		fprintf(stderr, "Not enough memory!\n");
		return ERROR;
	}

	if( tr->set_base(base)!=b_tree<student>::SUCCESS )
	{
		fprintf(stderr, "Wrong base: %d!\n", base);
		return ERROR;
	}

	int res_read;
	clock_t begin, end;

	begin = clock();
	if( (res_read = tr->read(filename))!=b_tree<student>::SUCCESS )
	{
		delete tr;

		switch( res_read )
		{
		case b_tree<student>::ALLOC_ERROR:
			fprintf(stderr, "Not enough memory!\n");
			break;
		case b_tree<student>::OPEN_ERROR:
			fprintf(stderr, "Can't open file %s!\n", filename);
			break;
		case b_tree<student>::READ_ERROR:
			fprintf(stderr, "Can't read student from %s!\n", filename);
			break;
		default:
			fprintf(stderr, "Unknown error with code %d!\n", res_read);
		}

		return ERROR;
	}
	end = clock();
	printf("Reading complete.\n");
	if( level_print>0 )
	{
		printf("Result:\n");
		tr->print(level_print, values_print, childs_print);
	}
	printf("Elapsed: %.2lf sec\n", static_cast<double>(end - begin)/CLOCKS_PER_SEC);

	if( param_set )
	{
		int total_size_num_childs, total_size_sbtrs_nodes_ng, total_size_sbtrs_levels_ng,
			total_size_sbtrs_width_ng, num_values_level, total_size_paths_len;

		begin = clock();
		total_size_num_childs = tr->total_size_num_childs(param);
		end = clock();
		printf("\nTotal number of values in nodes with %d childs calculated.\n", param);
		printf("Result: %d\n", total_size_num_childs);
		printf("Elapsed: %.2lf sec\n", static_cast<double>(end - begin)/CLOCKS_PER_SEC);

		begin = clock();
		total_size_sbtrs_nodes_ng = tr->total_size_sbtrs_nodes_ng(param);
		end = clock();
		printf("\nTotal number of values in subtrees with number of nodes non-greater than %d calculated.\n", param);
		printf("Result: %d\n", total_size_sbtrs_nodes_ng);
		printf("Elapsed: %.2lf sec\n", static_cast<double>(end - begin)/CLOCKS_PER_SEC);

		begin = clock();
		total_size_sbtrs_levels_ng = tr->total_size_sbtrs_levels_ng(param);
		end = clock();
		printf("\nTotal number of values in subtrees with number of levels non-greater than %d calculated.\n", param);
		printf("Result: %d\n", total_size_sbtrs_levels_ng);
		printf("Elapsed: %.2lf sec\n", static_cast<double>(end - begin)/CLOCKS_PER_SEC);

		begin = clock();
		total_size_sbtrs_width_ng = tr->total_size_sbtrs_width_ng(param);
		end = clock();
		printf("\nTotal number of values in subtrees with width non-greater than %d calculated.\n", param);
		printf("Result: %d\n", total_size_sbtrs_width_ng);
		printf("Elapsed: %.2lf sec\n", static_cast<double>(end - begin)/CLOCKS_PER_SEC);

		begin = clock();
		num_values_level = tr->num_values_level(param);
		end = clock();
		printf("\nNumber of values on level %d calculated.\n", param);
		printf("Result: %d\n", num_values_level);
		printf("Elapsed: %.2lf sec\n", static_cast<double>(end - begin)/CLOCKS_PER_SEC);

		begin = clock();
		total_size_paths_len = tr->total_size_paths_len(param);
		end = clock();
		printf("\nTotal number of values in paths length %d from root to leaf nodes calculated.\n", param);
		printf("Result: %d\n", total_size_paths_len);
		printf("Elapsed: %.2lf sec\n", static_cast<double>(end - begin)/CLOCKS_PER_SEC);
	}

	delete tr;

	return 0;
}
