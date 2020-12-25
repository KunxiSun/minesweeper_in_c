#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minesweeper.h"

/* USYD CODE CITATION ACKNOWLEDGEMENT
	 * I declare that the following lines of code have been copied from the
     * website titled: "How to convert an index into N coordinates?" and it is not my own work. 
     * 
     * Original URL
     * https://math.stackexchange.com/questions/2008367/how-to-convert-an-index-into-n-coordinates
     * Last access March, 2020
     */
int* convert_index_to_coor(int * coords, int dim, int * dim_sizes, int num_cells, int index){
    int k = index;
    int c = num_cells;
    for(int i=0; i<dim; i++){
        c/=dim_sizes[i];
        int j = k/c;
        k-=j*c;
        coords[i]=j;
    }
    return coords;
}

int convert_coor_to_index(int dim, int * dim_sizes, int num_cells, int* coords){
    int k=0;
    int c = num_cells;
    for(int l=0; l<dim; l++){
        c/=dim_sizes[l];
        k+=coords[l]*c;
    }
    return k;
}
/* end of copied code */

int calculate_num_cells(int dim, int * dim_sizes){
    int num_cells=1;
    for(int i=0; i<dim; i++){
        num_cells*= dim_sizes[i];
    }
    return num_cells;
}


void init_game(struct cell * game, int dim, int * dim_sizes, int num_mines, int ** mined_cells) {
    //dim too large
    if(dim>MAX_DIM){
        return; 
    }

    //calculate the number of cells
    int num_cells=calculate_num_cells(dim,dim_sizes);

    //initial selected, coordinate, and mined in this for loop
    for(int i=0; i<num_cells; i++){
        //initial selected as 0: unselected
        game[i].selected=0;

        //initial coordinate
        int coords[MAX_DIM];
        int * p = convert_index_to_coor(coords,dim,dim_sizes,num_cells,i);
        for(int j=0; j<dim; j++){
            game[i].coords[j]=*p;
            p++;
        }

        //initial mined
        int mined =0;
        for(int j=0; j<num_mines; j++){
            int match=1;
            for(int k=0; k<dim; k++){
                if(coords[k]!=mined_cells[j][k]){
                    match=0;
                }
            }
            if(match==1){
                mined=1;
            }
        }
        game[i].mined=mined;
    }

    //initial adjacent, hint, and num_adjacent
    for(int i=0; i<num_cells; i++){
        int num_adjacent=0;
        int hint=0;
        for(int j=0; j<num_cells; j++){
            //skip when two cells are same
            if(game[i].coords==game[j].coords){
                continue;
            }
            int match=1;
            for(int k=0; k<dim; k++){
                int substract = abs(game[i].coords[k]-game[j].coords[k]);
                if(substract>1){
                    match=0;
                    break;
                }
            }
            if(match==1){
                if(game[j].mined==1){
                    hint+=1;
                }
                //initial adjacents
                game[i].adjacent[num_adjacent] = &game[j];
                num_adjacent+=1;
            }
        }
        //initial num_adjacent
        game[i].num_adjacent=num_adjacent;
        //initial hint
        game[i].hint=hint;
    }
    return;
}

void select_recursion(struct cell cell){
    if(cell.hint==0){
        for(int i=0; i<cell.num_adjacent; i++){
            if(cell.adjacent[i]->selected==0){
                cell.adjacent[i]->selected=1;
                select_recursion(*cell.adjacent[i]);
            }
        }
    }
}

int select_cell(struct cell * game, int dim, int * dim_sizes, int * coords) {
     //calculate the number of cells
    int num_cells=calculate_num_cells(dim,dim_sizes);
    int index = convert_coor_to_index(dim,dim_sizes,num_cells,coords);

    //return 0 when input coords out of bound
    for(int i=0; i<dim; i++){
        if(coords[i]+1>dim_sizes[i]){
            return 0;
        }
    }

    //return 0 if this has been selected
    if(game[index].selected==1){
        return 0;
    }

    game[index].selected=1;

    //return 1 if select a mined cell
    if(game[index].mined==1){
        return 1;
    }else{
        select_recursion(game[index]);
    }

    int win =1;
    for(int i=0; i<num_cells; i++){
        if(game[i].mined==0 && game[i].selected==0){
            win=0;
        }
    }
    if(win==1){
        return 2;
    }

    //else return 0
    return 0;
}