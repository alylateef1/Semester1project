#include <stdio.h>
#include <stdlib.h>

int puzzle[9][9] = {
    {3,0,0,0,2,0,0,7,0},
    {9,0,0,5,0,0,0,1,4},
    {0,1,6,3,7,0,0,0,8},
    {2,0,0,8,0,0,0,0,1},
    {5,0,0,0,4,1,8,0,0},
    {0,8,9,0,0,0,0,5,0},
    {0,0,5,0,1,0,2,8,0},
    {0,4,0,0,0,6,0,9,3},
    {7,3,1,0,8,2,0,0,0},
};

int original[9][9];

void print_puzzle(int grid[9][9]);
int is_valid(int grid[9][9], int row, int col, int num);
int solve_sudoku(int grid[9][9]);
int find_empty(int grid[9][9], int *row, int *col);
void input_puzzle();
void reset_puzzle();
void check_solution();
void copy_grid(int source[9][9], int dest[9][9]);
void clear_input_buffer();

int main() {
    int choice;
    copy_grid(puzzle, original);
    
    do {
        printf("\n~~~SUDOKU SOLVER ~~~\n");
        print_puzzle(puzzle);
        printf("\n1. Solve Puzzle\n");
        printf("2. Input New Puzzle\n");
        printf("3. Check Solution\n");
        printf("4. Reset Puzzle\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        
        if(scanf("%d", &choice) != 1) {
            clear_input_buffer();
            printf("Invalid input! Please enter a number.\n");
            continue;
        }
        
        switch(choice) {
            case 1:
                if(solve_sudoku(puzzle)) {
                    printf("\nSOLVED SUCCESSFULLY:\n");
                    print_puzzle(puzzle);
                } else {
                    printf("\nNo solution exists!\n");
                }
                break;
            case 2:
                input_puzzle();
                break;
            case 3:
                check_solution();
                break;
            case 4:
                reset_puzzle();
                printf("\nPuzzle reset to original.\n");
                break;
            case 5:
                printf("\nGoodbye!\n");
                break;
            default:
                printf("\nInvalid choice! Please enter 1-5.\n");
        }
    } while(choice != 5);
    
    return 0;
}

void clear_input_buffer() {
    int c;
    while ((c = getchar()) != '\n');
}

void copy_grid(int source[9][9], int dest[9][9]) {
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            dest[i][j] = source[i][j];
        }
    }
}

int solve_sudoku(int grid[9][9]) {
    int row, col;
    
    if(!find_empty(grid, &row, &col)) {
        return 1;
    }
    
    for(int num = 1; num <= 9; num++) {
        if(is_valid(grid, row, col, num)) {
            grid[row][col] = num;
            
            if(solve_sudoku(grid)) {
                return 1;
            }
            
            grid[row][col] = 0;
        }
    }
    
    return 0;
}

int find_empty(int grid[9][9], int *row, int *col) {
    for(*row = 0; *row < 9; (*row)++) {
        for(*col = 0; *col < 9; (*col)++) {
            if(grid[*row][*col] == 0) {
                return 1;
            }
        }
    }
    return 0;
}

int is_valid(int grid[9][9], int row, int col, int num) {
   
    for(int i = 0; i < 9; i++) {
        if(grid[row][i] == num){
         return 0;}
    }
    
    for(int i = 0; i < 9; i++) {
        if(grid[i][col] == num){
         return 0;}
    }
    
    // Check 3x3 box
    int box_row = (row / 3) * 3;
    int box_col = (col / 3) * 3;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(grid[box_row + i][box_col + j] == num) {
                return 0;}
        }
    }
    
    return 1;
}

void print_puzzle(int grid[9][9]) {
    printf("\n");
    for(int i = 0; i < 9; i++) {
        if(i % 3 == 0 && i != 0) {
            printf("------+-------+------\n");
        }
        for(int j = 0; j < 9; j++) {
            if(j % 3 == 0 && j != 0) {
                printf("| ");
            }
            if(grid[i][j] == 0) {
                printf(". ");
            } else {
                printf("%d ", grid[i][j]);
            }
        }
        printf("\n");
    }
}

void input_puzzle() {
    printf("\n=== INPUT NEW PUZZLE ===\n");
    printf("Enter 9 numbers for each row (use 0 for empty cells)\n");
    printf("Separate numbers by space\n\n");
    
    int temp[9][9];
    int valid_input = 0;
    
    while(!valid_input) {
        valid_input = 1;
        
        for(int i = 0; i < 9; i++) {
            printf("Row %d: ", i + 1);
            
            for(int j = 0; j < 9; j++) {
                if(scanf("%d", &temp[i][j]) != 1) {
                    printf("Invalid input! Please enter numbers only.\n");
                    clear_input_buffer();
                    valid_input = 0;
                    break;
                }
                
                if(temp[i][j] < 0 || temp[i][j] > 9) {
                    printf("Error: Numbers must be between 0-9.\n");
                    valid_input = 0;
                    break;
                }
            }
            
            if(!valid_input) {
                clear_input_buffer();
                break;
            }
        }
        
        if(valid_input) {
           
            for(int i = 0; i < 9; i++) {
                for(int j = 0; j < 9; j++) {
                    if(temp[i][j] != 0) {
                        int num = temp[i][j];
                        temp[i][j] = 0; 
                        
                        if(!is_valid(temp, i, j, num)) {
                            printf("Error: Invalid puzzle! Conflict at row %d, column %d\n", i+1, j+1);
                            valid_input = 0;
                        }
                        
                        temp[i][j] = num; 
                    }
                    if(!valid_input) break;
                }
                if(!valid_input) break;
            }
        }
        
        if(!valid_input) {
            printf("\nPlease enter the puzzle again:\n");
        }
    }
    
    copy_grid(temp, puzzle);
    copy_grid(temp, original);
    printf("\nPuzzle updated successfully!\n");
}

void reset_puzzle() {

    copy_grid(original, puzzle);
}

void check_solution() {
    int temp[9][9];
    copy_grid(puzzle, temp);
    
    if(solve_sudoku(temp)) {
        printf("\n✓ This puzzle is solvable!\n");



        int empty = 0;
        for(int i = 0; i < 9; i++) {
            for(int j = 0; j < 9; j++) {
                if(puzzle[i][j] == 0) empty++;
            }
        }
        printf("Empty cells: %d\n", empty);
    } else {
        printf("\nThis puzzle cannot be solved!\n");
    }
  
} 