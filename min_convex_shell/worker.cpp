//
//  worker.cpp
//  min_convex_shell
//
//  Created by Constantine on 28.03.2023.
//

#include "worker.hpp"

void get_worker_bins(MPI_Datatype PointType, Point *&bin, int &bin_size) {
    MPI_Recv(&bin_size, 1, MPI_INTEGER, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    bin = new Point[bin_size];
    MPI_Recv(bin, bin_size, PointType, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

void send_hull(const MPI_Datatype &PointType, const ConvexHull &local) {
    MPI_Send(&local.num_points, 1, MPI_INTEGER, 0, 0, MPI_COMM_WORLD);
    MPI_Send(local.points, local.num_points, PointType, 0, 0, MPI_COMM_WORLD);
}
