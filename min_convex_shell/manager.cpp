//
//  manager.cpp
//  min_convex_shell
//
//  Created by Constantine on 28.03.2023.
//

#include "manager.hpp"

void distribute_points(Point** bins, int* counts, double max_x, double min_x, int num_bins, int num_points, Point *points) {
    
    for (int i = 0; i < num_bins; i++) {
        counts[i] = 0;
    }
    
    for (int i = 0; i < num_points; i++) {
        counts[get_bin(points[i].x, min_x, max_x, num_bins)]++;
    }
    
    int indexes[num_bins];
    for (int i = 0; i < num_bins; i++) {
        indexes[i] = 0;
        bins[i] = new Point[counts[i]];
    }
    
    for (int i = 0; i < num_points; i++) {
        int bin = get_bin(points[i].x, min_x, max_x, num_bins);
        bins[bin][indexes[bin]] = points[i];
        indexes[bin]++;
    }
}

void read_points(const char* filename, int* num_points, double* min_x, double* max_x, Point* &points) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        fp = fopen("/Users/kostyansa/input", "r");
    }
    if (fp == NULL) {
        printf("Error: could not open file %s\n", filename);
        exit(1);
    }
    fscanf(fp, "%d", num_points);
    points = new Point[*num_points];
    *max_x = -INFINITY;
    *min_x = INFINITY;
    
    double x, y;
    int index = 0;
    while (fscanf(fp, "%lf %lf", &x, &y) != EOF) {
        points[index].x = x;
        points[index].y = y;
        if (x < *min_x) {
            *min_x = x;
        }
        if (x > *max_x) {
            *max_x = x;
        }
        index++;
    }
    fclose(fp);
}

void calculate_and_send_bins(MPI_Datatype PointType, char **argv, Point *&bin, int &bin_size, double &max_x, double &min_x, int world_size) {
    const char *input_file = argv[1];
    const int num_bins = world_size;
    int num_points = 0;
    Point* points = nullptr;
    read_points(input_file, &num_points, &min_x, &max_x, points);
    
    Point** bins = new Point*[num_bins];
    int* counts = new int[num_bins];
    distribute_points(bins, counts, max_x, min_x, num_bins, num_points, points);
    
    for (int i = 1; i < world_size; i++) {
        MPI_Send(&counts[i], 1, MPI_INTEGER, i, 0, MPI_COMM_WORLD);
        MPI_Send(bins[i], counts[i], PointType, i, 0, MPI_COMM_WORLD);
    }
    bin_size = counts[0];
    bin = bins[0];
}

void recieve_hulls(const MPI_Datatype &PointType, ConvexHull *&hulls, const int &world_size) {
    for (int i = 1; i < world_size; i++) {
        int hull_size = 0;
        MPI_Recv(&hull_size, 1, MPI_INTEGER, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        Point* points = nullptr;
        if (hull_size > 0) {
            points = new Point[hull_size];
            MPI_Recv(points, hull_size, PointType, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        ConvexHull hull;
        hull.num_points = hull_size;
        hull.points = points;
        hulls[i] = hull;
    }
}
