//
//  temp.cpp
//  min_convex_shell
//
//  Created by Constantine on 27.03.2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

#include "manager.hpp"
#include "worker.hpp"
#include "data.hpp"


int main(int argc, char** argv) {
    int rank, world_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Barrier(MPI_COMM_WORLD);
    
//    if (argc != 2) {
//        if (rank == 0) {
//            printf("Usage: mpiexec -n <num_procs> %s <input_file>\n", argv[0]);
//        }
//        MPI_Finalize();
//        return 1;
//    }

    //MPI struct definition
    MPI_Datatype PointType = define_point_type();
    double min_x, max_x;
    int bin_size = 0;
    Point* bin;
    if (rank == 0) {
        calculate_and_send_bins(PointType, argv, bin, bin_size, max_x, min_x, world_size);
    }
    else {
        get_worker_bins(PointType, bin, bin_size);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    ConvexHull local;
    if (bin_size > 0)
    {
        local = compute_convex_hull(bin, bin_size);
    }
    else {
        local.num_points = 0;
        local.points = nullptr;
    }
    
    ConvexHull* hulls;
    if (rank == 0) {
        hulls = new ConvexHull[world_size];
        hulls[0] = local;
        recieve_hulls(PointType, hulls, world_size);
    }
    else {
        send_hull(PointType, local);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == 0) {
        ConvexHull merged = merge_hulls(hulls, world_size);
        output_convex_hull(merged);
    }
    
    MPI_Type_free(&PointType);
    MPI_Finalize();
    return 0;
}
