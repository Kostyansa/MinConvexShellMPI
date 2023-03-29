//
//  manager.hpp
//  min_convex_shell
//
//  Created by Constantine on 28.03.2023.
//

#ifndef manager_hpp
#define manager_hpp

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include "data.hpp"

void calculate_and_send_bins(MPI_Datatype PointType, char **argv, Point *&bin, int &bin_size, double &max_x, double &min_x, int world_size);
void recieve_hulls(const MPI_Datatype &PointType, ConvexHull *&hulls, const int &world_size);

#endif /* manager_hpp */
