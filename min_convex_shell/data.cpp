//
//  data.cpp
//  min_convex_shell
//
//  Created by Constantine on 28.03.2023.
//

#include "data.hpp"

MPI_Datatype define_point_type() {
    int block_lengths[2] = {1, 1};
    MPI_Datatype types[2] = {MPI_DOUBLE, MPI_DOUBLE};
    MPI_Aint displacements[2] = {offsetof(Point, x), offsetof(Point, y)};
    MPI_Datatype PointType;
    MPI_Type_create_struct(2, block_lengths, displacements, types, &PointType);
    MPI_Type_commit(&PointType);
    return PointType;
}

int get_bin(double x, double min_x, double max_x, int num_bins) {
    int bin = floor((x - min_x) / (max_x - min_x) * num_bins);
    return bin < num_bins ? bin : (num_bins - 1);
}

ConvexHull compute_convex_hull(Point* points, int num_points) {
    ConvexHull hull;
    hull.num_points = 0;
    hull.points = new Point[num_points];
    // Find the leftmost point
    int leftmost = 0;
    for (int i = 1; i < num_points; i++) {
        if (points[i].x < points[leftmost].x) {
            leftmost = i;
        }
    }

    // Start from the leftmost point and iterate over all points in CCW order
    int current = leftmost;
    do {
        hull.points[hull.num_points++] = points[current];
        int next = (current + 1) % num_points;
        for (int i = 0; i < num_points; i++) {
            if (i == current || i == next) {
                continue;
            }
            double cross_product = (points[i].x - points[current].x) * (points[next].y - points[current].y) - (points[i].y - points[current].y) * (points[next].x - points[current].x);
            if (cross_product < 0) {
                next = i;
            }
        }
        current = next;
    } while (current != leftmost);

    if (hull.num_points < num_points) {
        Point* temp = new Point[num_points];
        
        for (int i = 0; i < hull.num_points; i++) {
            temp[i] = hull.points[i];
        }
        
        delete[] hull.points;
        
        hull.points = temp;
    }
    
    return hull;
}

void output_convex_hull(const ConvexHull& hull) {
    printf("%d\n", hull.num_points);
    for (int i = 0; i < hull.num_points; i++) {
        printf("%lf %lf\n", hull.points[i].x, hull.points[i].y);
    }
}


ConvexHull merge_hulls(const ConvexHull* hulls, const int num_hulls){
    int num_points = 0;
    for (int i = 0; i < num_hulls; i++) {
        num_points += hulls[i].num_points;
    }
    
    Point* points = new Point[num_points];
    
    int index = 0;
    for (int i = 0; i < num_hulls; i++) {
        for (int j = 0; j < hulls[i].num_points; j++) {
            points[index] = hulls[i].points[j];
            index++;
        }
    }
    
    return compute_convex_hull(points, num_points);
}
