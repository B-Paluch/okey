#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
int main(int argc, char** argv) {
    MPI_Init(NULL, NULL);
    int world_size;
    int world_rank;
    int data = 0;
    int i = 0;
    int j = 0;
    int round = 0;
    int winners = 0;
    int root = 0;
    int didiwin = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    srand(time(NULL)+world_rank);
    int wintable[world_size - 1];
    int bingotable[5][5];
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            bingotable[i][j] = rand() % 50;
        }
    }
    while (1) {
        if (world_rank == root) {
            data = rand() % (50) + rand() % 5 * 50;
            printf("round %d number: %d\n",round++, data);
        }
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Bcast(&data, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        if (world_rank != root) {
            if (data < 50) {
                for (i = 0; i < 5; i++) {
                    if (bingotable[0][i] != -1 && bingotable[0][i] % 10 == data % 10) {
                        bingotable[0][i] = -1;
                        break;
                    }
                }
            }
            else if (data < 100) {
                for (i = 0; i < 5; i++) {
                    if (bingotable[1][i] != -1 && bingotable[1][i] % 10 == data % 10) {
                        bingotable[1][i] = -1;
                        break;
                    }
                }
            }
            else if (data < 150) {
                for (i = 0; i < 5; i++) {
                    if (bingotable[2][i] != -1 && bingotable[2][i] % 10 == data % 10) {
                        bingotable[2][i] = -1;
                        break;
                    }
                }
            }
            else if (data < 200) {
                for (i = 0; i < 5; i++) {
                    if (bingotable[3][i] != -1 && bingotable[3][i] % 10 == data % 10) {
                        bingotable[3][i] = -1;
                        break;
                    }
                }
            }
            else if (data < 250) {
                for (i = 0; i < 5; i++) {
                    if (bingotable[4][i] != -1 && bingotable[4][i] % 10 == data % 10) {
                        bingotable[4][i] = -1;
                        break;
                    }
                }
            }
            if ((bingotable[0][0] == -1 && bingotable[0][1] == -1 && bingotable[0][2] == -1 && bingotable[0][3] == -1 && bingotable[0][4] == -1)
                || (bingotable[0][0] == -1 && bingotable[1][1] == -1 && bingotable[2][2] == -1 && bingotable[3][3] == -1 && bingotable[4][4] == -1)
                || (bingotable[0][0] == -1 && bingotable[1][0] == -1 && bingotable[2][0] == -1 && bingotable[3][0] == -1 && bingotable[4][0] == -1)) {
                didiwin = 1;
		    printf("I won! %d\n",world_rank);
            }
        }
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Gather(&didiwin, 1, MPI_INT, wintable, 1, MPI_INT, 0, MPI_COMM_WORLD);
        for (i = 0; i < world_size; i++) {
            if (wintable[i] == 1)
                winners += 1;
        }
        MPI_Barrier(MPI_COMM_WORLD);
        if (winners) {
            printf("%d winners\n", winners);
            for (i = 0; i < world_size; i++) {
		    if (wintable[i] == 1)
                  printf("%d is a winner!!!\n", i);
            }
            printf("Thats all!");
            break;
        }
    }
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Finalize();
}