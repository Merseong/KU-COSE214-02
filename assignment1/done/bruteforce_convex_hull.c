#include <stdlib.h> // atoi, rand
#include <stdio.h>
#include <assert.h> // assert
#include <time.h> //time

#define RANGE 100

typedef struct
{
	int x;
	int y;
} point;

typedef struct
{
	point from;
	point to;
} line_segment;

////////////////////////////////////////////////////////////////////////////////
void print_header(char *filename)
{
	printf( "#! /usr/bin/env Rscript\n");
	printf( "png(\"%s\", width=700, height=700)\n", filename);
	
	printf( "plot(1:%d, 1:%d, type=\"n\")\n", RANGE, RANGE);
}
////////////////////////////////////////////////////////////////////////////////
void print_footer(void)
{
	printf( "dev.off()\n");
}

////////////////////////////////////////////////////////////////////////////////
void print_points( point *p, int n);

// [input] p : set of points
// [input] n : number of points
// [output] l : set of line segments that form the convex hull
// return value : number of line segments
int convex_hull( point *p, int n, line_segment *l);

void print_line_segments( line_segment *l, int num_line);

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	int x, y;
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

	point p[n];
	line_segment l[2 * n];

	fprintf( stderr, "Create %d points\n", n);

	// making n points
	srand( time(NULL));
	for (int i = 0; i < n; i++)
	{
		x = rand() % RANGE + 1; // 1 ~ RANGE random number
		y = rand() % RANGE + 1;
		
		p[i].x = x;
		p[i].y = y;
 	}

	print_header( "convex.png");
	
	print_points( p, n);
	
	int num_line = convex_hull( p, n, l);

	print_line_segments( l, num_line);
	
	print_footer();
	
	return 0;
}

void print_points( point *p, int n)
{
	printf("\n# %d points\n", n);
	for (int i = 0; i < n; i++)
	{
		printf("points(%d, %d)\n", p[i].x, p[i].y); 
	}
}

// [input] p : set of points
// [input] n : number of points
// [output] l : set of line segments that form the convex hull
// return value : number of line segments
int convex_hull( point *p, int n, line_segment *l)
{
	// remove duplicated point
	int newCount = 0;
	point* newP = (point*) malloc (sizeof (point) * n);
	for (int i = 0; i < n; i++)
	{
		int isDuplicated = 0;
		for (int j = 0; j < newCount; j++)
		{
			if (newP[j].x == p[i].x && newP[j].y == p[i].y) isDuplicated = 1;
		}
		if (!isDuplicated) newP[newCount++] = p[i];
	}

	// convex hull main algorithm
	int lineCount = 0;
	for (int i = 0; i < newCount; i++) // first point
	{
		for (int j = i + 1; j < newCount; j++) // second point
		{
			// ax + by = c
			int a = newP[j].y - newP[i].y;
			int b = newP[i].x - newP[j].x;
			int c = newP[i].x * newP[j].y - newP[i].y * newP[j].x;

			int checker = 0;
			int isLine = 1;

			for (int k = 0; k < newCount; k++) // check for every point
			{
				if (k == i || k == j) continue;
				int where = a * newP[k].x + b * newP[k].y - c;
				if (checker == 0)
				{
					checker = where;
				}
				else if (checker * where < 0)
				{
					isLine = 0;
					break;
				}
				if (where == 0 && ((newP[i].x - newP[k].x) * (newP[j].x - newP[k].x) < 0 || (newP[i].y - newP[k].y) * (newP[j].y - newP[k].y) < 0))
				{ // remove overlaped line
					isLine = 0;
					break;
				}
			}

			if (isLine)
			{
				l[lineCount].from = newP[i];
				l[lineCount++].to = newP[j];
			}
		}
	}

	return lineCount;
}

void print_line_segments( line_segment *l, int num_line)
{
	printf("\n# %d line segments\n", num_line);
	for (int i = 0; i < num_line; i++)
	{
		printf("segments(%d, %d, %d, %d)\n", l[i].from.x, l[i].from.y, l[i].to.x, l[i].to.y); 
	}
}