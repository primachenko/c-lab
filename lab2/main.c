#include <stdio.h>
#include <stdlib.h>

#define MAX_STR_LEN 256

int main(int argc, char ** argv)
{
	printf("enter line number:\n");
	int line_number = 0;
	scanf("%d\n", &line_number);
	char ** str_arr = NULL;
	str_arr = calloc(line_number, sizeof(char *));
	if (NULL == str_arr)
	{
		printf("<%s> %d: calloc failed\n", __FUNCTION__, __LINE__);
		return -1;
	}

	for (int i = 0; i < line_number; ++i)
	{
		str_arr[i] = calloc(1, MAX_STR_LEN);
		if (NULL == str_arr[i])
		{
			printf("<%s> %d: calloc failed\n", __FUNCTION__, __LINE__);
			return -1;
		}
		gets(str_arr[i]);
	}

	int first_char_cmp(const void * l, const void * r)
	{
		return  **((char **)l) - **((char **)r);
	}

	// printf("%lu\n", sizeof(str_arr[0]));
	qsort(str_arr, 10, sizeof(char *), first_char_cmp);

	printf("sorted arr:\n");
	for (int i = 0; i < line_number; ++i)
		puts(str_arr[i]);

	for (int i = 0; i < line_number; ++i)
	{
		free(str_arr[i]);
		str_arr[i] = NULL;
	}

	free(str_arr);

	return 0;
}
