#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INSERT_OP 0x01
#define DELETE_OP 0x02
#define SUBSTITUTE_OP 0x04
#define MATCH_OP 0x08
#define TRANSPOSE_OP 0x10

#define INSERT_COST 1
#define DELETE_COST 1
#define SUBSTITUTE_COST 1
#define TRANSPOSE_COST 1

void backtrace_main(int **bt, int *op_matrix, int col_size, char *str1, char *str2, int n, int m, int level, char **align_str, int *align_case);
void print_matrix(int **op_matrix, int n, int m);
int min_editdistance(char *str1, char *str2);
int __GetMin3(int a, int b, int c);
int __GetMin4(int a, int b, int c, int d);
void print_alignment(char **align_str, int level);
void backtrace(int **bt, int *op_matrix, int col_size, char *str1, char *str2, int n, int m);

// 재귀적으로 연산자 행렬을 순회하며, 두 문자열이 최소편집거리를 갖는 모든 가능한 정렬(alignment) 결과를 출력한다.
// op_matrix : 이전 상태에 대한 연산자 정보(backtrace)가 저장된 행렬 (1차원 배열임에 주의!)
// col_size : op_matrix의 열의 크기
// str1 : 문자열 1
// str2 : 문자열 2
// n : 문자열 1의 길이
// m : 문자열 2의 길이
// level : 재귀호출의 레벨 (0, 1, 2, ...)
// align_str : 정렬된 문자쌍들의 정보가 저장된 문자열 배열 예) "a - a", "a - b", "* - b", "ab - ba"
void backtrace_main(int **bt, int *op_matrix, int col_size, char *str1, char *str2, int n, int m, int level, char **align_str, int *align_case)
{
	//printf("%d, %d start, op_matrix[col_size - 1] = %d\n", n, m, op_matrix[col_size - 1]);
	if (n < 1 && m < 1)
	{
		printf("\n[%d] ==============================\n", *align_case);
		print_alignment(align_str, level - 1);
		(*align_case)++;
		return;
	}

	if (op_matrix[col_size - 1] & INSERT_OP)
	{
		op_matrix[col_size] = bt[n][m - 1];
		char strl[8] = "* - ";
		strncat(strl, str2 + m - 1, 1);
		strcpy(align_str[level], strl);
		//printf("I: %s", align_str[level]);
		backtrace_main(bt, op_matrix, col_size + 1, str1, str2, n, m - 1, level + 1, align_str, align_case);
	}
	if (op_matrix[col_size - 1] & DELETE_OP)
	{
		op_matrix[col_size] = bt[n - 1][m];
		char strl[8] = "";
		strncat(strl, str1 + n - 1, 1);
		strcat(strl, " - *");
		strcpy(align_str[level], strl);
		//printf("D: %s", align_str[level]);
		backtrace_main(bt, op_matrix, col_size + 1, str1, str2, n - 1, m, level + 1, align_str, align_case);
	}
	if (op_matrix[col_size - 1] & TRANSPOSE_OP)
	{
		op_matrix[col_size] = bt[n - 2][m - 2];
		char strl[8] = "";
		strncat(strl, str1 + n - 2, 1);
		strncat(strl, str1 + n - 1, 1);
		strcat(strl, " - ");
		strncat(strl, str2 + m - 2, 1);
		strncat(strl, str2 + m - 1, 1);
		strcpy(align_str[level], strl);
		//printf("T: %s", align_str[level]);
		backtrace_main(bt, op_matrix, col_size + 1, str1, str2, n - 2, m - 2, level + 1, align_str, align_case);
	}
	if (op_matrix[col_size - 1] & SUBSTITUTE_OP || op_matrix[col_size - 1] & MATCH_OP)
	{
		op_matrix[col_size] = bt[n - 1][m - 1];
		char strl[8] = "";
		strncat(strl, str1 + n - 1, 1);
		strcat(strl, " - ");
		strncat(strl, str2 + m - 1, 1);
		strcpy(align_str[level], strl);
		//printf("S|M: %s", align_str[level]);
		backtrace_main(bt, op_matrix, col_size + 1, str1, str2, n - 1, m - 1, level + 1, align_str, align_case);
	}
	//printf("%d, %d end\n", n, m);
	return;
}

// 강의 자료의 형식대로 op_matrix를 출력 (좌하단(1,1) -> 우상단(n, m))
// 각 연산자를  괄호 안의 기호로 표시한다. 삽입(I), 삭제(D), 교체(S), 일치(M), 전위(T)
void print_matrix(int **op_matrix, int n, int m)
{
	for (int i = n; i > 0; --i)
	{
		for (int j = 1; j <= m; ++j)
		{
			char op[5] = "";
			if (op_matrix[i][j] & INSERT_OP)
			{
				strcat(op, "I");
			}
			if (op_matrix[i][j] & DELETE_OP)
			{
				strcat(op, "D");
			}
			if (op_matrix[i][j] & SUBSTITUTE_OP)
			{
				strcat(op, "S");
			}
			if (op_matrix[i][j] & MATCH_OP)
			{
				strcat(op, "M");
			}
			if (op_matrix[i][j] & TRANSPOSE_OP)
			{
				strcat(op, "T");
			}
			printf(" %-6s ", op);
		}
		printf("\n");
	}
}

// 두 문자열 str1과 str2의 최소편집거리를 계산한다.
// return value : 최소편집거리
// 이 함수 내부에서 print_matrix 함수와 backtrace 함수를 호출함
int min_editdistance(char *str1, char *str2)
{
	// init dp array
	int str1_len = strlen(str1);
	int str2_len = strlen(str2);
	// edit distance matrix
	int **ed = (int **)malloc(sizeof(int *) * (str1_len + 1));
	// backtrace matrix
	int **bt = (int **)malloc(sizeof(int *) * (str1_len + 1));
	for (int i = 0; i <= str1_len; ++i)
	{
		ed[i] = (int *)malloc(sizeof(int) * (str2_len + 1));
		bt[i] = (int *)malloc(sizeof(int) * (str2_len + 1));
		ed[i][0] = i * DELETE_COST;
		bt[i][0] = DELETE_OP;
		if (i == 0)
		{
			bt[0][0] = 0;
			for (int j = 1; j <= str2_len; ++j)
			{
				ed[0][j] = j * INSERT_COST;
				bt[0][j] = INSERT_OP;
			}
		}
	}

	// dp main
	for (int i = 1; i <= str1_len; ++i) // Deletion
	{
		for (int j = 1; j <= str2_len; ++j) // Insertion
		{
			int delete = ed[i - 1][j] + DELETE_COST;
			int insert = ed[i][j - 1] + INSERT_COST;
			int match = str1[i - 1] == str2[j - 1];
			int substitute = ed[i - 1][j - 1] + (match ? 0 : SUBSTITUTE_COST);
			int transpose = -1;
			if (i > 1 && j > 1 && str1[i - 1] == str2[j - 2] && str1[i - 2] == str2[j - 1])
			{
				transpose = ed[i - 2][j - 2] + TRANSPOSE_COST;
			}

			ed[i][j] = __GetMin3(delete, insert, substitute);
			if (transpose > -1)
			{
				ed[i][j] = transpose < ed[i][j] ? transpose : ed[i][j];
			}

			bt[i][j] = 0;
			if (ed[i][j] == delete)
			{
				bt[i][j] += DELETE_OP;
			}
			if (ed[i][j] == insert)
			{
				bt[i][j] += INSERT_OP;
			}
			if (match)
			{
				bt[i][j] += MATCH_OP;
			}
			else if (ed[i][j] == transpose)
			{
				bt[i][j] += TRANSPOSE_OP;
			}
			else if (ed[i][j] == substitute)
			{
				bt[i][j] += SUBSTITUTE_OP;
			}
		}
	}

	print_matrix(bt, str1_len, str2_len);

	// find backtrace routes
	int *op_matrix = (int *)malloc(sizeof(int) * (str1_len + str2_len + 2));
	op_matrix[0] = bt[str1_len][str2_len];
	backtrace(bt, op_matrix, 1, str1, str2, str1_len, str2_len);

	int output = ed[str1_len][str2_len];
	// free dp array
	free(op_matrix);
	for (int i = 0; i <= str1_len; ++i)
	{
		free(ed[i]);
		free(bt[i]);
	}
	free(ed);
	free(bt);

	return output;
}

////////////////////////////////////////////////////////////////////////////////
// 세 정수 중에서 가장 작은 값을 리턴한다.
int __GetMin3(int a, int b, int c)
{
	int min = a;
	if (b < min)
		min = b;
	if (c < min)
		min = c;
	return min;
}

////////////////////////////////////////////////////////////////////////////////
// 네 정수 중에서 가장 작은 값을 리턴한다.
int __GetMin4(int a, int b, int c, int d)
{
	int min = __GetMin3(a, b, c);
	return (min > d) ? d : min;
}

////////////////////////////////////////////////////////////////////////////////
// 정렬된 문자쌍들을 출력
void print_alignment(char **align_str, int level)
{
	int i;

	for (i = level; i >= 0; i--)
	{
		printf("%s\n", align_str[i]);
	}
}

////////////////////////////////////////////////////////////////////////////////
// backtrace_main을 호출하는 wrapper 함수
void backtrace(int **bt, int *op_matrix, int col_size, char *str1, char *str2, int n, int m)
{
	char **align_str = (char **)malloc(sizeof(char *) * (n + m)); // n+m strings
	for (int i = 0; i < n + m; ++i)
	{
		align_str[i] = (char *)malloc(sizeof(char) * 8);
	}
	int *align_case = (int *)malloc(sizeof(int));
	*align_case = 1;

	backtrace_main(bt, op_matrix, col_size, str1, str2, n, m, 0, align_str, align_case);

	for (int i = 0; i < n + m; ++i)
	{
		free(align_str[i]);
	}
	free(align_str);
	free(align_case);
}

////////////////////////////////////////////////////////////////////////////////
int main()
{
	char str1[30];
	char str2[30];

	int distance;

	printf("INSERT_COST = %d\n", INSERT_COST);
	printf("DELETE_COST = %d\n", DELETE_COST);
	printf("SUBSTITUTE_COST = %d\n", SUBSTITUTE_COST);
	printf("TRANSPOSE_COST = %d\n", TRANSPOSE_COST);

	while (fscanf(stdin, "%s\t%s", str1, str2) != EOF)
	{
		printf("\n==============================\n");
		printf("%s vs. %s\n", str1, str2);
		printf("==============================\n");

		distance = min_editdistance(str1, str2);

		printf("\nMinEdit(%s, %s) = %d\n", str1, str2, distance);
	}
	return 0;
}
