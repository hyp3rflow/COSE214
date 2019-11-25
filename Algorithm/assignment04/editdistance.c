#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define INSERT_OP      0x01
#define DELETE_OP      0x02
#define SUBSTITUTE_OP  0x04
#define MATCH_OP       0x08
#define TRANSPOSE_OP   0x10

#define INSERT_COST	1
#define DELETE_COST	1
#define SUBSTITUTE_COST	1
#define TRANSPOSE_COST	1

// 재귀적으로 연산자 행렬을 순회하며, 두 문자열이 최소편집거리를 갖는 모든 가능한 정렬(alignment) 결과를 출력한다.
// op_matrix : 이전 상태에 대한 연산자 정보(backtrace)가 저장된 행렬 (1차원 배열임에 주의!)
// col_size : op_matrix의 열의 크기
// str1 : 문자열 1
// str2 : 문자열 2
// n : 문자열 1의 길이
// m : 문자열 2의 길이
// level : 재귀호출의 레벨 (0, 1, 2, ...)
// align_str : 정렬된 문자쌍들의 정보가 저장된 문자열 배열 예) "a - a", "a - b", "* - b", "ab - ba"
void backtrace_main( int *op_matrix, int col_size, char *str1, char *str2, int n, int m, int level, char align_str[][8]);

// 강의 자료의 형식대로 op_matrix를 출력 (좌하단(1,1) -> 우상단(n, m))
// 각 연산자를  괄호 안의 기호로 표시한다. 삽입(I), 삭제(D), 교체(S), 일치(M), 전위(T)
void print_matrix( int op_matrix[][31], int col_size, int n, int m){
	char arr_print[5] = {'I', 'D', 'S', 'M', 'T'};
	for(int i=n; i>0; i--){
		for(int j=1; j<m+1; j++){
			for(int k=0; k<5; k++){
				if(op_matrix[i][j] & (1 << k)) printf("%c", arr_print[k]);
			}
			printf("\t");
		}
		printf("\n");
	}
}


// 세 정수 중에서 가장 작은 값을 리턴한다.
int __GetMin3( int a, int b, int c)
{
	int min = a;
	if (b < min)
		min = b;
	if(c < min)
		min = c;
	return min;
}

// 네 정수 중에서 가장 작은 값을 리턴한다.
int __GetMin4( int a, int b, int c, int d)
{
	int min = __GetMin3( a, b, c);
	return (min > d) ? d : min;
}

// 두 문자열 str1과 str2의 최소편집거리를 계산한다.
// return value : 최소편집거리
// 이 함수 내부에서 print_matrix 함수와 backtrace 함수를 호출함
int min_editdistance( char *str1, char *str2){
	int min_edit[31][31];
	int op_matrix[31][31];

	// op_matrix initialize
	for(int i=0; i<31; i++)
		for(int j=0; j<31; j++) op_matrix[i][j] = 0;

	// initialize
	int n = strlen(str1);
	int m = strlen(str2);
	
	for(int i=0; i<n+1; i++) min_edit[i][0] = i;
	for(int i=0; i<m+1; i++) min_edit[0][i] = i;

	int arr_COST[4] = {INSERT_COST, DELETE_COST, SUBSTITUTE_COST, TRANSPOSE_COST};
	int arr_OP[4] = {INSERT_OP, DELETE_OP, SUBSTITUTE_OP, TRANSPOSE_OP};
	int diff_x[4] = {0, -1, -1, -2};
	int diff_y[4] = {-1, 0, -1, -2};
 
	for(int i=1; i<n+1; i++)
		for(int j=1; j<m+1; j++){
			if(str1[i-1] == str2[j-1]){
				min_edit[i][j] = min_edit[i-1][j-1];
				op_matrix[i][j] = MATCH_OP;
			}
			else if(str1[i-1] == str2[j-2] && str1[i-2] == str2[j-1]){
				min_edit[i][j] = min_edit[i-2][j-2] + TRANSPOSE_COST;
				op_matrix[i][j] = TRANSPOSE_OP;
			}
			else{
				min_edit[i][j] = __GetMin3(min_edit[i-1][j] + DELETE_COST,
											min_edit[i][j-1] + INSERT_COST, 
											min_edit[i-1][j-1] + SUBSTITUTE_COST);
				for(int k=0; k<3; k++){
					if(min_edit[i][j] == min_edit[i+diff_x[k]][j+diff_y[k]] + arr_COST[k])
						op_matrix[i][j] += arr_OP[k];
				}
				/*if(i > 1 && j > 1){
					min_edit[i][j] = __GetMin4(min_edit[i-1][j] + DELETE_COST,
											min_edit[i][j-1] + INSERT_COST, 
											min_edit[i-1][j-1] + SUBSTITUTE_COST,
											min_edit[i-2][j-2] + TRANSPOSE_COST);

					for(int k=0; k<4; k++){
						if(min_edit[i][j] == min_edit[i+diff_x[k]][j+diff_y[k]] + arr_COST[k])
							op_matrix[i][j] += arr_OP[k];
					}
				}
				else{
					min_edit[i][j] = __GetMin3(min_edit[i-1][j] + DELETE_COST,
											min_edit[i][j-1] + INSERT_COST, 
											min_edit[i-1][j-1] + SUBSTITUTE_COST);
					for(int k=0; k<3; k++){
						if(min_edit[i][j] == min_edit[i+diff_x[k]][j+diff_y[k]] + arr_COST[k])
							op_matrix[i][j] += arr_OP[k];
					}
				}*/
			}
		}

	print_matrix(op_matrix, 0, n, m);

	backtrace()

	return min_edit[n][m];
}

// 정렬된 문자쌍들을 출력 
void print_alignment( char align_str[][8], int level)
{
	int i;
	
	for (i = level; i >= 0; i--)
	{
		printf( "%s\n", align_str[i]);
	}
}

// backtrace_main을 호출하는 wrapper 함수
void backtrace( int *op_matrix, int col_size, char *str1, char *str2, int n, int m)
{
	char align_str[n+m][8]; // n+m strings
	
	//backtrace_main( op_matrix, col_size, str1, str2, n, m, 0, align_str);
}

int main()
{
	char str1[30];
	char str2[30];
	
	int distance;
	
	printf( "INSERT_COST = %d\n", INSERT_COST);
	printf( "DELETE_COST = %d\n", DELETE_COST);
	printf( "SUBSTITUTE_COST = %d\n", SUBSTITUTE_COST);
	printf( "TRANSPOSE_COST = %d\n", TRANSPOSE_COST);
	
	while( fscanf( stdin, "%s\t%s", str1, str2) != EOF)
	{
		printf( "\n==============================\n");
		printf( "%s vs. %s\n", str1, str2);
		printf( "==============================\n");
		
		distance = min_editdistance( str1, str2);
		
		printf( "\nMinEdit(%s, %s) = %d\n", str1, str2, distance);
	}
	return 0;
}
