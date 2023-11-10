#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct
{
    float x;
    float y;

    int hull;
    int next;
} point;

void output_to_file(point* s, int n);

void gen_points(point* s, int n)
{
    srand(time(NULL));

    for (int i = 0; i < n; i++)
    {
        s[i].x = (float) rand()/RAND_MAX;
        s[i].y = (float) rand()/RAND_MAX;
        s[i].hull = 0;
        s[i].next = -1;
    }
}

// Find whether p3 is to the left or right of vector p1p2
int left_turn(point p1, point p2, point p3)
{
    // k-component of cross product (i and j will be zero)
    float k = (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);
    
    return (k > 0);
}
  
// Prints convex hull of a set of n points.
void hull_points(point* s, int n)
{
    // There must be at least 3 points
    if (n < 3)
        return;
    
    // Find the leftmost point
    int leftmost = 0;
    for (int i = 1; i < n; i++)
    {
        if (s[i].x < s[leftmost].x)
            leftmost = i;
    }
    s[leftmost].hull = 1;
    
    // Start from leftmost point, keep moving anticlockwise
    // until reach the start point again
    int h = leftmost, p; // 'h' is most recent point on hull
    do
    {
        // Search for a point 'p' such that p is to left of vector hi
        // for all points 'i'
        p = (h + 1) % n;
        for (int i = 0; i < n; i++)
            if (left_turn(s[h], s[i], s[p]))
                p = i;
   
        s[h].next = p; // Add p to result as a next point of h
        h = p; // Set h as p for next iteration
        s[h].hull = 1;
    }
    while (h != leftmost);
    
    output_to_file(s, n);
    
    // Print Result
    printf("The points in the convex hull are: \n");
    do {
        printf("Point %9d - (%.3f, %.3f) - next: %9d\n", h, s[h].x, s[h].y, s[h].next);
        h = s[h].next;        
    } while(h != leftmost);
}

void output_to_file(point* s, int n)
{
    FILE *fptr;

    fptr = fopen("points.txt", "w");

    for (int i = 0; i < n; i++)
        fprintf(fptr, "%f %f %d %d\n", s[i].x, s[i].y, s[i].hull, s[i].next);

    fclose(fptr); 
}

int main()
{
    char input[10];
    int n;
    point* s;

    printf("Enter number of points (default=100): ");
    fgets(input, 10, stdin);
    if(atoi(input))
        n = atoi(input);
    else 
        n = 100;

    s = (point*) malloc(n * sizeof(point));
    
    printf("Generating %d points\n", n);
    gen_points(s, n);

    hull_points(s, n);

    free(s);

    return 0;
}
