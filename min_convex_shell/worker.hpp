//
//  worker.hpp
//  min_convex_shell
//
//  Created by Constantine on 28.03.2023.
//

#ifndef worker_hpp
#define worker_hpp

#include <stdio.h>
#include <mpi.h>

#include "data.hpp"

void get_worker_bins(MPI_Datatype PointType, Point *&bin, int &bin_size);
void send_hull(const MPI_Datatype &PointType, const ConvexHull &local);

#endif /* worker_hpp */
