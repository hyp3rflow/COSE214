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
void print_matrix( int *op_matrix, int col_size, int n, int m);

// 두 문자열 str1과 str2의 최소편집거리를 계산한다.
// return value : 최소편집거리
// 이 함수 내부에서 print_matrix 함수와 backtrace 함수를 호출함
int min_editdistance( char *str1, char *str2);

////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////
// 네 정수 중에서 가장 작은 값을 리턴한다.
int __GetMin4( int a, int b, int c, int d)
{
	int min = __GetMin3( a, b, c);
	return (min > d) ? d : min;
}

////////////////////////////////////////////////////////////////////////////////
// 정렬된 문자쌍들을 출력 
void print_alignment( char align_str[][8], int level)
{
	int i;
	for (i = level; i >= 0; i--)
	{
		printf( "%s\n", align_str[i]);
	}
}

////////////////////////////////////////////////////////////////////////////////
// backtrace_main을 호출하는 wrapper 함수
void backtrace( int *op_matrix, int col_size, char *str1, char *str2, int n, int m)
{
	char align_str[n+m][8]; // n+m strings
	
	backtrace_main( op_matrix, col_size, str1, str2, n, m, 0, align_str);
}

////////////////////////////////////////////////////////////////////////////////
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

int min_editdistance(char *str1, char *str2){
	// VARIABLE DEFINITION
	int n = strlen(str1);
	int m = strlen(str2);
	int row_size = m+1;

	int COST[5] = {INSERT_COST, DELETE_COST, SUBSTITUTE_COST, TRANSPOSE_COST, 0};
	int OP[5] = {INSERT_OP, DELETE_OP, SUBSTITUTE_OP, TRANSPOSE_OP, MATCH_OP};
	int dx[3] = {0, -1, -1};
	int dy[3] = {-1, 0, -1};

	int op_matrix[(n+1)*(m+1)];
	int min_edit[(n+1)*(m+1)];
	
	for(int i=0; i<(n+1)*(m+1); i++){
		op_matrix[i] = 0;
		min_edit[i] = 0;
	}

	// OP_MATRIX INITIALIZE for clear alignment
	for(int i=0; i<=n; i++){
		min_edit[row_size * i] = i;
		if(i) op_matrix[row_size * i] = DELETE_OP;
	}
	for(int i=0; i<=m; i++){
		min_edit[i] = i;
		if(i) op_matrix[i] = INSERT_OP;
	}

	for(int j=1; j<=m; j++){
		for(int i=1; i<=n; i++){
			int chk[5] = {1000, 1000, 1000, 1000, 1000};
			
			if(str1[i-1] == str2[j-1]) chk[4] = min_edit[row_size * (i-1) + (j-1)];
			if(i > 1 && j > 1 && str1[i-1] == str2[j-2] && str1[i-2] == str2[j-1])
				chk[3] = min_edit[row_size * (i-2) + (j-2)] + COST[3];
			for(int k=0; k<3; k++){
				chk[k] = min_edit[row_size * (i+dx[k]) + (j+dy[k])] + COST[k];
			}

			int min = 0;
			for(int k=1; k<5; k++)
				if(chk[k] < chk[min]) min = k;
			
			for(int k=0; k<5; k++)
				if(chk[k] == chk[min]) op_matrix[row_size * i + j] += OP[k];
			
			min_edit[row_size * i + j] = chk[min];
		}
	}

	print_matrix(op_matrix, row_size, n, m);

	backtrace(op_matrix, row_size, str1, str2, n, m);

	return min_edit[row_size * n + m];
}

void print_matrix(int* op_matrix, int row_size, int n, int m){
	char PRINT[5] = {'I', 'D', 'S', 'M', 'T'};

	for(int i=n; i>0; i--){
		for(int j=1; j<=m; j++){
			for(int k=0; k<5; k++){
				if(op_matrix[row_size * i + j] & (1 << k)) printf("%c", PRINT[k]);
			}
			printf("\t");
		}
		printf("\n");
	}
}

void backtrace_main(int *op_matrix, int row_size, char *str1, char *str2, int n, int m, int level, char align_str[][8]){
	// STATIC VARIABLE DEFINITION
	static int cnt;

	// INITIALIZE when cycle starts
	if(!level) cnt = 1;

	if(n == 0 && m == 0){
		printf("\n[%d] ==============================\n", cnt++);
		// print_alignment function starts printing from level
		print_alignment(align_str, level-1);
		return;
	}
	else{
		// DFS Call order DEFINITION
		int arr[5] = {2, 3, 0, 1, 4};
		for(int i=0; i<5; i++){
			int k = arr[i];
			if(op_matrix[row_size * n + m] & (1 << k)){
				// MATCH & SUBSTITUTE_OP
				if(k == 2 || k == 3){
					sprintf(align_str[level], "%c - %c", str1[n-1], str2[m-1]);
					backtrace_main(op_matrix, row_size, str1, str2, n-1, m-1, level+1, align_str);
				}

				// INSERT_OP
				else if(k == 0){
					sprintf(align_str[level], "%c - %c", '*', str2[m-1]);
					backtrace_main(op_matrix, row_size, str1, str2, n, m-1, level+1, align_str);
				}
				
				// DELETE_OP
				else if(k == 1){
					sprintf(align_str[level], "%c - %c", str1[n-1], '*');
					backtrace_main(op_matrix, row_size, str1, str2, n-1, m, level+1, align_str);
				}
				
				// TRANSPOSE_OP
				else if(k == 4){
					sprintf(align_str[level], "%c%c - %c%c", str1[n-2], str1[n-1], str2[m-2], str2[m-1]);
					backtrace_main(op_matrix, row_size, str1, str2, n-2, m-2, level+1, align_str);
				}
			}
		}
	}
}