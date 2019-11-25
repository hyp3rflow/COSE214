#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#define PEASANT 0x08
#define WOLF   0x04
#define GOAT   0x02
#define CABBAGE   0x01

// 인접행렬로 표현된 graph를 화면에 출력
void print_graph( int graph[][16], int num){
   
   for (int i = 0; i < num; i++){
      for (int j = 0; j < num; j++){
         printf("%d ", graph[i][j]);
      }
      printf("\n");
   }
};

// 주어진 상태 state의 마지막 4비트를 파일에 출력 (화면 출력은 stdout)
void bit_print( FILE *fp, int state){
   
   int a, b, c, d;
   a = state % 2;
   state /= 2;
   
   b = state % 2;
   state /=2;
   
   c = state % 2;
   state /=2;
   
   d = state;
   
   fprintf(fp, "\"%d%d%d%d\"\n", d, c, b, a);
   
};

// 주어진 상태 state에서 농부, 늑대, 염소, 양배추의 상태를 각각 추출하여 p, w, g, c에 저장
void get_pwgc( int state, int *p, int *w, int *g, int *c){
   
   if ( (state & PEASANT) == 0 )
      *p = 0;
   else *p = 1;
   
   if ((state & WOLF) == 0)
      *w = 0;
   else *w = 1;
   
   if ((state & GOAT) == 0)
      *g = 0;
   else *g = 1;
   
   if ((state & CABBAGE) == 0)
      *c = 0;
   else *c = 1;
                                    
};

// 주어진 그래프(graph)를 .net 파일로 저장
// pgwc.net 참조
void save_graph( char *filename, int graph[][16], int num){
   
   FILE *fp = fopen(filename, "w");
   
   fprintf(fp, "*Vertices %d\n", num);
   for(int i = 0; i < num; i++){
      fprintf(fp, "%3d ", i+1);
      bit_print(fp, i);
   }
   
   fprintf(fp, "*Edges\n");
   for (int i = 0; i<8; i++){      //edges 앞에는 state, 옆에는 연결 된 state
      for(int j = 0; j < num; j++){
         if(graph[i][j] == 1){
            fprintf(fp, "%3d  %d\n", i+1, j+1);
         }
      }
   }
   
   
};

// 허용되지 않는 상태인지 검사
// 예) 농부없이 늑대와 염소가 같이 있는 경우 / 농부없이 염소와 양배추가 같이 있는 경우 `// 6가지 경우?
// return value: 1 허용되지 않는 상태인 경우, 0 허용되는 상태인 경우
int is_dead_end( int state){      
   
   int p, w, g, c;
   
   get_pwgc (state, &p, &w, &g, &c);
   
   if ( (w == g) && (w != p))
      return 1;
   else if ((g == c) && (p != g))
      return 1;
   else if (state > 15)
      return 1;
   else
      return 0;
};

// state1 상태에서 state2 상태로의 전이 가능성 점검
// 농부 또는 농부와 다른 하나의 아이템이 강 반대편으로 이동할 수 있는 상태만 허용
// return value: 1 전이 가능한 경우, 0 전이 불이가능한 경우 
int is_possible_transition( int state1,   int state2){
   
   int res = state1 ^ state2;
   if(res == 8 || (res == 9) || (res == 10) || (res == 12))
      return 1;
   else if (state1 == state2)
      return 0;
   else
      return 0;
};

// 상태들의 인접 행렬을 구하여 graph에 저장
// 상태간 전이 가능성 점검
// 허용되지 않는 상태인지 점검 
void make_adjacency_matrix( int graph[][16]){
   
   for (int i = 0; i<16; i++){
      if(is_dead_end(i) == 0){
         for(int j = 0; j<16; j++){
            if(is_dead_end(j) == 0){
               if(is_possible_transition(i, j) == 1)
                  graph[i][j] = 1;
               else
                  graph[i][j] = 0;
            }
         }
      }
      else{
         for(int j = 0; j<16; j++)
            graph[i][j] = 0;
      }
   }
   
};

// 상태 변경: 농부 이동
// return value : 새로운 상태
int changeP( int state){
   
   if (state >= 8) {      //1000 농부가 있던 상태에서 변해~
      state = state - 8;
   }
   
   else
      state = state + 8;
   
   return state;
   
};

// 상태 변경: 농부, 늑대 이동
// return value : 새로운 상태
int changePW( int state){
   
   if (state >= 12)
      state = state - 12;
   
   else
      state = state + 12;
   
   return state;
   
};

// 상태 변경: 농부, 염소 이동
// return value : 새로운 상태
int changePG( int state){
   
   if (state >= 10)
      state = state - 10;
   else
      state = state + 10;
   
   return state;
   
};

// 상태 변경: 농부, 양배추 이동
// return value : 새로운 상태
int changePC( int state){
   
   if (state >= 9)
      state = state - 9;
   else
      state = state + 9;
   
   return state;
   
};

// 방문한 상태인지 검사
// return value : 1 visited, 0 not visited
int is_visited( int *visited, int level, int state){
   for(int i = 0; i<=level; i++){
      if(visited[i] == state){
         return 1;
      }
   }
   return 0;
};

// 방문한 상태들을 차례로 화면에 출력
void print_states( int *visited, int count){
   
   int a, b, c, d;
   
   printf("---find!!---\n");
   
   for(int i = 0; i<=count; i++){
      int tmp = visited[i];    
      a = tmp % 2;
      tmp /= 2;
      
      b = tmp % 2;
      tmp /= 2;
      
      c = tmp % 2;
      tmp /=2;
      
      d = tmp;
      
      printf("%d%d%d%d\n", d, c, b, a);
   }
   
   printf("\n\n\n");
   
};

// recursive function
void dfs_main(int state, int goal_state, int level, int *visited){
   
   visited[level] = state;
   
   if(state == goal_state){     
      print_states(visited, level);
      return;   
   }
   else{
       /*
      if(is_dead_end(changeP(state)) == 0 && is_possible_transition(state, changeP(state)) == 1 && is_visited(visited, level, changeP(state)) == 0){
         newstate = changeP(state);
         visited[level] = newstate;
         dfs_main(newstate, goal_state, level+1, visited);
      }
      if(is_dead_end(changePW(state)) == 0 && is_possible_transition(state, changePW(state)) == 1 &&is_visited(visited, level, changePW(state)) == 0){
         newstate = changePW(state);
         visited[level] = newstate;
         //level++;
         dfs_main(newstate, goal_state, level+1, visited);
      }
      if(is_dead_end(changePG(state)) == 0 && is_possible_transition(state, changePG(state)) == 1 && is_visited(visited, level, changePG(state)) == 0){
         newstate = changePG(state);
         visited[level] = newstate;
         //level++;
         dfs_main(newstate, goal_state, level+1, visited);
      }
      if(is_dead_end(changePC(state)) == 0 && is_possible_transition(state, changePC(state)) == 1 && is_visited(visited, level, changePC(state)) == 0){
         newstate = changePC(state);
         visited[level] = newstate;
         //level++;
         dfs_main(newstate, goal_state, level+1, visited);
      }
      */

        int (*func[4])(int) = {changeP, changePC, changePG, changePW};
        for(int i=0; i<4; i++){
            int newstate = func[i](state);
            if(!is_dead_end(newstate) && is_possible_transition(state, newstate) && !is_visited(visited, level, newstate)){
                // visited[level+1] = newstate;
                dfs_main(newstate, goal_state, level+1, visited);
            }
        }
      
   }
   
};

// 깊이 우선 탐색 (초기 상태 -> 목적 상태)
void depth_first_search( int init_state, int goal_state){
   int level = 0;
   int visited[16] = {0,};
   
   dfs_main (init_state, goal_state, level, visited);
   /*
   printf("%d\n", visited[0]);
   printf("%d\n", visited[1]);
   printf("%d\n", visited[2]);
   printf("%d\n", visited[3]);
   printf("%d\n", visited[4]);
*/
};

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
   int graph[16][16] = {0,};
   
   // 인접 행렬 만들기
   
   make_adjacency_matrix( graph);

   // 인접 행렬 출력
   //print_graph( graph, 16);
   
   // .net 파일 만들기 pajek
   save_graph( "pwgc.net", graph, 16);

   //////////////////////////////////////////
   
   depth_first_search( 0, 15); // initial state, goal state
   
   return 0;
}