#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PEASANT 0x08
#define WOLF 0x04
#define GOAT 0x02
#define CABBAGE 0x01

// 인접행렬로 표현된 graph를 화면에 출력
void print_graph(int graph[][16], int num) {
    for(int i=0; i<num; i++){
        for(int j=0; j<num; j++){
            printf("%d ", graph[i][j]);
        }
        printf("\n");
    }
}

// 주어진 상태 state의 마지막 4비트를 파일에 출력 (화면 출력은 stdout)
void bit_print(FILE *fp, int state) {
    for (int i = 0; i < 4; i++) {
        fprintf(fp, "%d", state & (1 << (3 - i)) ? 1 : 0);
    }
}

// 주어진 상태 state에서 농부, 늑대, 염소, 양배추의 상태를 각각 추출하여 p, w,
// g, c에 저장
void get_pwgc(int state, int *p, int *w, int *g, int *c) {
    int *arr[4] = {p, w, g, c};
    for (int i = 0; i < 4; i++) {
        *arr[i] = state & (1 << (3 - i)) ? 1 : 0;
    }
}

// 주어진 그래프(graph)를 .net 파일로 저장
// pgwc.net 참조
void save_graph(char *filename, int graph[][16], int num) {
    FILE *fp = fopen(filename, "w");

    fprintf(fp, "*Vertices %d\n", num);
    for (int i = 0; i < num; i++) {
        fprintf(fp, "%3d \"", i + 1);
        bit_print(fp, i);
        fprintf(fp, "\"\n");
    }

    fprintf(fp, "*Edges\n");
    for (int i = 0; i < num; i++) {
        for (int j = i + 1; j < num; j++) {
            if (graph[i][j])
                fprintf(fp, "%3d %3d\n", i + 1, j + 1);
        }
    }

    fclose(fp);
}

// 허용되지 않는 상태인지 검사
// 예) 농부없이 늑대와 염소가 같이 있는 경우 / 농부없이 염소와 양배추가 같이
// 있는 경우 return value: 1 허용되지 않는 상태인 경우, 0 허용되는 상태인 경우
int is_dead_end(int n) {
    // 농부 없이 늑대-염소 or 농부 없이 염소-양배추 -> 허용 안되면 1
    int p, w, g, c;
    p = n & PEASANT ? 1 : 0;
    w = n & WOLF ? 1 : 0;
    g = n & GOAT ? 1 : 0;
    c = n & CABBAGE ? 1 : 0;

    // 늑대-염소인 경우 -> 농부 없는 경우
    if (w == g && w != p) {
        return 1;
    } else if (g == c && g != p) {
        return 1;
    }

    return 0;
}

// state1 상태에서 state2 상태로의 전이 가능성 점검
// 농부 또는 농부와 다른 하나의 아이템이 강 반대편으로 이동할 수 있는 상태만
// 허용 return value: 1 전이 가능한 경우, 0 전이 불이가능한 경우
int is_possible_transition(int s1, int s2) {
    // 강 반대편으로 넘어갈 수 있는 경우 생각
    // 일단 두 상태의 농부의 위치는 달라야 한다.
    // 다음으로는 아이템의 위치가 최대 1개만 바뀌어야 한다.
    int p, w, g, c;
    p = (s1 ^ s2) & PEASANT ? 1 : 0;
    w = (s1 ^ s2) & WOLF ? 1 : 0;
    g = (s1 ^ s2) & GOAT ? 1 : 0;
    c = (s1 ^ s2) & CABBAGE ? 1 : 0;

    if (p) {
        int chk = w ^ g ^ c;
        if (w == 0 && g == 0 && c == 0) {
            return 1;
        } else if (w == 1 && g == 1 && c == 1) {
            return 0;
        } else if (chk) {
            return 1;
        }
    }

    return 0;
}

// 상태들의 인접 행렬을 구하여 graph에 저장
// 상태간 전이 가능성 점검
// 허용되지 않는 상태인지 점검
void make_adjacency_matrix(int graph[][16]) {
    for (int i = 0; i < 16; i++) {
        if (is_dead_end(i))
            continue;
        for (int j = 0; j < 16; j++) {
            if (is_dead_end(j))
                continue;
            else if (is_possible_transition(i, j)) {
                graph[i][j] = 1;
            }
        }
    }
}

// 상태 변경: 농부 이동
// return value : 새로운 상태
int changeP(int state) {
    int p = state & PEASANT;

    if (p)
        return state - PEASANT;
    else
        return state + PEASANT;
}

// 상태 변경: 농부, 늑대 이동
// return value : 새로운 상태
int changePW(int state) {
    int newState = changeP(state);
    int w = newState & WOLF;

    if (w)
        return newState - WOLF;
    else
        return newState + WOLF;
}

// 상태 변경: 농부, 염소 이동
// return value : 새로운 상태
int changePG(int state) {
    int newState = changeP(state);
    int g = newState & GOAT;

    if (g)
        return newState - GOAT;
    else
        return newState + GOAT;
}

// 상태 변경: 농부, 양배추 이동
// return value : 새로운 상태
int changePC(int state) {
    int newState = changeP(state);
    int c = newState & CABBAGE;

    if (c)
        return newState - CABBAGE;
    else
        return newState + CABBAGE;
}

// 방문한 상태인지 검사
// return value : 1 visited, 0 not visited
int is_visited(int *visited, int level, int state) {
	if(visited[state]) return 1;
	return 0;
}

// 방문한 상태들을 차례로 화면에 출력
void print_states(int *visited, int count){
	int currentNode = count;
	bit_print(stdout, count);
	printf("\n");

	while(count != 15){
		bit_print(stdout, visited[count]);
		printf("\n");
		count = visited[count];
	}
}

// recursive function
void dfs_main(int state, int goal_state, int level, int *visited) {
	// bit_print(stdout, state);
	// printf("\n");

	if(state == goal_state){
		printf("Goal-state found!\n");
		print_states(visited, 0);
		printf("\n");
	}
	else{
		int (*fp[4])(int) = {changeP, changePC, changePG, changePW};
		for(int i=0; i<4; i++){
			int newState = fp[i](state);
			if(!is_dead_end(newState) && !visited[newState] && is_possible_transition(state, newState)){
				visited[state] = newState;
				dfs_main(newState, goal_state, level, visited);
				visited[newState] = 0;
			}
		}
	}
}

// 깊이 우선 탐색 (초기 상태 -> 목적 상태)
void depth_first_search(int init_state, int goal_state) {
	int visited[16] = {0,};
	dfs_main(init_state, goal_state, 0, visited);
}

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv) {
    int graph[16][16] = {0,};

    // 인접 행렬 만들기
    make_adjacency_matrix(graph);

    // 인접 행렬 출력
    // print_graph(graph, 16);

    // .net 파일 만들기 pajek
    save_graph("pwgc.net", graph, 16);

    //printf("%d", is_dead_end(0b1101));
	//printf("%d\n", is_possible_transition(0b0100, 0b1101));

    //////////////////////////////////////////
    depth_first_search(0, 15); // initial state, goal state

    return 0;
}
