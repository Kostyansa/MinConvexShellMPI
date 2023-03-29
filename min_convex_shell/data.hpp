//
//  data.hpp
//  min_convex_shell
//
//  Created by Constantine on 28.03.2023.
//

#ifndef data_hpp
#define data_hpp

#include <stdio.h>
#include <mpi.h>
#include <math.h>

typedef struct {
    double x;
    double y;
} Point;

typedef struct {
    int num_points;
    Point* points;
} ConvexHull;

MPI_Datatype define_point_type();
int get_bin(double x, double min_x, double max_x, int num_bins);
ConvexHull compute_convex_hull(Point* points, int num_points);
void output_convex_hull(const ConvexHull& hull);
ConvexHull merge_hulls(const ConvexHull* hull, const int num_hulls);

#endif /* data_hpp */
