#include <stdlib.h> // atoi, rand, qsort, malloc
#include <stdio.h>
#include <assert.h> // assert
#include <time.h> //time

#define RANGE 100.0

typedef struct
{
    float x;
    float y;
} point;

typedef struct
{
    point from;
    point to;
} line_segment;

////////////////////////////////////////////////////////////////////////////////
// function declaration
// convex hull 함수 (재귀호출)
int convex_hull_main( point *p, int n, point p1, point pn, line_segment *l, int *num_l);

// 직선(ax+by-c=0)과 주어진 점 p 간의 거리
// distance = | ( ax+by-c ) / sqrt(a^2 + b^2) |
float distance( float a, float b, float c, point p){
    return abs((a * p.x + b * p.y - c) / sqrt(a*a + b*b));
}

// 두 점(from, to)을 연결하는 직선(ax + by - c = 0)으로 n개의 점들의 집합(p)를 s1(점의 수 n1)과 s2(점의 수 n2)로 분리하는 함수
// p1 : 직선의 upper(left)에 속한 점들은 ax+by-c가 minus임
// p2 : lower(right)에 속한 점들은 ax+by-c가 plus임
void separate_points( point *p, int n, point from, point to, point *s1, point *s2, int *n1, int *n2){
    float a = to.y - from.y;
    float b = from.x - to.x;
    float c = to.y * from.x - from.y * to.x;

    for(int i=0; i<n; i++){
        float cur_x = p[i].x;
        float cur_y = p[i].y;

        float chksign = cur_x * a + b * cur_y - c;

        if((cur_x == from.x && cur_y == from.y) || (cur_x == to.x && cur_y == to.y)) continue;

        if(chksign < 0){
            s1[(*n1)++] = p[i];
        }

        else if (chksign > 0) {
            s2[(*n2)++] = p[i];
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
void print_header(char *filename)
{
    printf( "#! /usr/bin/env Rscript\n");
    printf( "png(\"%s\", width=700, height=700)\n", filename);

    printf( "plot(1:%.3f, 1:%.3f, type=\"n\")\n", RANGE, RANGE);
}

////////////////////////////////////////////////////////////////////////////////
void print_footer(void)
{
    printf( "dev.off()\n");
}

////////////////////////////////////////////////////////////////////////////////
// qsort를 위한 비교 함수
int cmp_x( const void *p1, const void *p2)
{
    point *p = (point *)p1;
    point *q = (point *)p2;

    float diff = p->x - q->x;

    return ((diff >= 0.0) ? ((diff > 0.0) ? +1 : 0) : -1);
}

////////////////////////////////////////////////////////////////////////////////
void print_points( point *p, int n)
{
    int i;
    printf( "\n#points\n");

    for (i = 0; i < n; i++)
        printf( "points(%.3f,%.3f)\n", p[i].x, p[i].y);
}

////////////////////////////////////////////////////////////////////////////////
void print_line_segments( line_segment *l, int num_line)
{
    int i;

    printf( "\n#line segments\n");

    for (i = 0; i < num_line; i++)
        printf( "segments(%.3f,%.3f,%.3f,%.3f)\n", l[i].from.x, l[i].from.y, l[i].to.x, l[i].to.y);
}

////////////////////////////////////////////////////////////////////////////////
// [input] p : set of points
// [input] n : number of points
// [output] l : set of line segments that forms the convex hull
// return value : number of line segments
int convex_hull( point *p, int n, int min_index, int max_index, line_segment *l)
{
    int num_l = 0;
    int n1 = 0, n2 = 0;

    point *s1 = (point *)malloc(sizeof (point *) * n);
    assert( s1 != NULL);

    point *s2 = (point *)malloc(sizeof (point *) * n);
    assert( s2 != NULL);

    // p[0] = min x point
    // p[n-1] = max x point
    //*** uncomment the below 2 lines

    separate_points( p, n, p[0], p[n-1], s1, s2, &n1, &n2);
    //separate_points( p, n, p[min_index], p[max_index], s1, s2, &n1, &n2);

    //**/printf( "segments(%.3f,%.3f,%.3f,%.3f)\n", p[0].x, p[0].y, p[n-1].x, p[n-1].y);

    convex_hull_main( s1, n1, p[0], p[n-1], l, &num_l);
    convex_hull_main( s2, n2, p[n-1], p[0], l, &num_l);

    //*** uncomment the below 2 lines
    //convex_hull_main( s1, n1, p[min_index], p[max_index], l, &num_l);
    //convex_hull_main( s2, n2, p[max_index], p[min_index], l, &num_l);

    return num_l;
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
    float x, y;
    int n; // number of points

    if (argc != 2)
    {
        printf( "%s number_of_points\n", argv[0]);
        return 0;
    }

    n = atoi( argv[1]);
    if (n <= 0)
    {
        printf( "The number of points should be a positive integer!\n");
        return 0;
    }

    point *p;
    p = (point *)malloc(sizeof (point *) * n);
    assert( p != NULL);

    line_segment *l;
    l = (line_segment *)malloc(sizeof (line_segment *) * n);
    assert( l != NULL);

    fprintf( stderr, "Create %d points\n", n);

    // making n points
    srand( time(NULL));
    for (int i = 0; i < n; i++)
    {
        x = (float)rand() / (float)(RAND_MAX) * RANGE; // 0 ~ RANGE random float number
        y = (float)rand() / (float)(RAND_MAX) * RANGE; // 0 ~ RANGE random float number

        p[i].x = x;
        p[i].y = y;
    }

    qsort( p, n, sizeof(point), cmp_x);

    print_header( "convex.png");

    print_points( p, n);

    // convex hull algorithm
    int num_line = convex_hull( p, n, 0, n-1, l);

    print_line_segments( l, num_line);

    print_footer();

    free( p);
    free( l);

    return 0;
}

int convex_hull_main( point *p, int n, point p1, point pn, line_segment *l, int *num_l){
    float a = pn.y - p1.y;
    float b = p1.x - pn.x;
    float c = pn.y * p1.x - pn.x * p1.y;

    int max_index = 0;
    float max = 0;

    if(n == 0){
        int i = (*num_l);
        l[*num_l].from.x = p1.x;
        l[*num_l].from.y = p1.y;
        l[*num_l].to.x = pn.x;
        l[(*num_l)++].to.y = pn.y;
        return 0;
    }

    for(int i=0; i<n; i++){
        float d = distance(a, b, c, p[i]);
        if(d > max){
            max = d;
            max_index = i;
        }
    }

    int n0 = 0, n1 = 0, n2 = 0;
    point *s0 = (point *)malloc(sizeof(point *) * n);
    point *s1 = (point *)malloc(sizeof(point *) * n);
    point *s2 = (point *)malloc(sizeof(point *) * n);

    separate_points(p, n, p1, p[max_index], s0, s1, &n0, &n1);
    free(s1);
    s1 = (point *)malloc(sizeof(point *) * n); n1 = 0;
    separate_points(p, n, p[max_index], pn, s2, s1, &n2, &n1);
    free(s1);

    point highest = p[max_index]; free(p); // -> abort

    convex_hull_main(s0, n0, p1, highest, l, num_l);
    convex_hull_main(s2, n2, highest, pn, l, num_l);
}
