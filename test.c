#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minesweeper.c"

int main(){
    struct cell test[12];
    int dim_sizes[]={3,4};
    int *mined[] = {
        (int[]){2,1},
        (int[]){1,2}};
    init_game(test,2,dim_sizes,2,mined);
    for(int i=0; i<12; i++){
        printf("coords: %d %d mined: %d, selected: %d, num_adjacent: %d, hint: %d\n",test[i].coords[0], test[i].coords[1], test[i].mined, test[i].selected, test[i].num_adjacent, test[i].hint);
        for(int j=0; j<test[i].num_adjacent;j++){
            printf("adjacent coords: %d %d\n", test[i].adjacent[j]->coords[0], test[i].adjacent[j]->coords[1]);
        }
        puts("");
    }
    return 0;
}