#include <iostream>
#include <set>
#include <utility>
#include <algorithm>
#include <cstring>

#define L 6
#define R 10
#define NIMIOUS (L*R-56)
#define MAX_DOMINOES 28
#define TOTAL_ELEMENTS (L*R)

using namespace std;

const int DOMINO_MATRIX[L][R] = {
    {0, 0, 1, 1, 2, 3, 3, 5, 4, 0},
    {3, 3, 2, 5, 6, 6, 3, 4, 2, 0},
    {6, 3, 0, 6, 1, 1, 6, 4, 2, 0},
    {2, 2, 4, 3, 6, 6, 1, 3, 2, 2},
    {2, 4, 4, 0, 0, 5, 4, 5, 5, 5},
    {1, 1, 4, 6, 0, 5, 1, 3, 5, 3}
};

class Domino{
    public:
    int x1,y1,x2,y2;
    Domino(){x1=0;y1=0;x2=0;y2=0;}
    Domino(int c1, int u1, int c2, int u2){
        x1=c1;
        y1=u1;
        x2=c2;
        y2=u2;
    }
    void print(){
        cout << "(" << x1 << "," << y1 << ") — (" << x2 << "," << y2 << ")";
    }

};

class Game_board{
    public:
    bool used[L][R];
    int board[L][R];

    Game_board(const int domino_matrix_draft[L][R]){
        for (int i = 0; i < L; ++i){
            for (int j = 0; j < R; ++j) {
                board[i][j]=domino_matrix_draft[i][j];
                used[i][j]=false;
            }
        }
    }

    bool isValid(int c, int u ){
        return c>=0 && u>=0 && c<L && u<R;
    }
    
    bool isUsed(int c, int u ){
        return used[c][u];
    }
    
    int getValue(int c, int u ){
        return board[c][u];
    }
    
    void setValue(int c, int u, int value){
        board[c][u] =value;
    }
    
    void setUsed(int c, int u, bool value=true){
        used[c][u]=value;
    }
    
    void resetUsed(){
        for(int i=0; i<L;++i){
            for(int j=0; j<R; ++j){
                used[i][j]=false;
            }
        }
    }

    void print_board(){
        cout<<"Робоче поле:\n";
        for(int i = 0; i < L; i++){
            for(int j = 0; j < R; j++){
                cout<< board[i][j]<<" ";
            }
            cout<<endl;
        }
    }
};

class Domino_solver{
    public:
    const int dx[2]={0,1};
    const int dy[2]={1,0};
    set<pair<int,int>> used_Dominoes;
    Game_board work_board;
    Domino solution_arr[MAX_DOMINOES];
    int solution_size = 0;
    Domino_solver(const int domino_mutrix[L][R]) : work_board(domino_mutrix) {}	
    
    void reset_solver(){
        solution_size=0;
        used_Dominoes.clear();
        work_board.resetUsed();
    }

    bool solve(){
        for (int i = 0; i < L; i++){
            for (int j = 0; j < R; j++){
                if(!work_board.isUsed(i,j)&& work_board.getValue(i,j)!=-1){
                    for(int d =0; d<2; ++d){
                        int ni = i +dx[d];
                        int nj = j +dy[d];
                        if(work_board.isValid(ni,nj)&& !work_board.isUsed(ni,nj)&& work_board.getValue(ni,nj)!= -1){
                            int a = work_board.getValue(i,j);
                            int b= work_board.getValue(ni, nj);
                            pair<int, int> domino =  {min(a, b), max(a, b)};
                            if(used_Dominoes.count(domino)) continue;
                            work_board.setUsed(i,j);
                            work_board.setUsed(ni,nj);
                            used_Dominoes.insert(domino);
                            solution_arr[solution_size++] =Domino(i,j,ni,nj);
                            if(solve()) return true;
                            --solution_size;
                            used_Dominoes.erase(domino);
                            work_board.setUsed(i,j,false);
                            work_board.setUsed(ni,nj,false);
                            }
                        }
                        return false;
                    }
                }
            }
            int non_neg=0;
            for(int i =0; i<L;++i){
                for(int j = 0; j < R; ++j){
                    if(work_board.getValue(i,j)!= -1){
                        non_neg++;
                    }
                }
            }
            return solution_size*2 == non_neg;
        }

    void find_repeat(const int domino_matrix1[L][R], int more_8[NIMIOUS], int &size){
        int counts[7] = {0};
        size = 0;
        for (int i = 0; i < L; i++){
            for (int j = 0; j < R; j++){
                int val = domino_matrix1[i][j];
                if (val >= 0 && val <= 6){
                    counts[val]++;
                }
            }
        }
        for (int i = 0; i < 7; i++){
            int over = counts[i] - 8; 
            for (int j = 0; j < over; j++) {
                more_8[size++] = i;
            }
        }
    }

    bool solve_selection(const int domino_matrix2[L][R], const int target_digits[NIMIOUS], int target_digits_size){
        int reminder_counts[7]={0};
        for(int i=0; i<NIMIOUS; ++i){
            int digit = target_digits[i];
            reminder_counts[digit]++;
        }
        int indices[NIMIOUS];

        for(int index_0 = 0; index_0 <= TOTAL_ELEMENTS - NIMIOUS; ++index_0){
            int val_0 = domino_matrix2[index_0/R][index_0%R];
            if(reminder_counts[val_0]>0){
                indices[0]=index_0;
                int reminder_counts_draft1[7];
                copy(reminder_counts, reminder_counts+7, reminder_counts_draft1);
                reminder_counts_draft1[val_0]--;

                for(int index_1 = index_0 + 1; index_1 <= TOTAL_ELEMENTS - (NIMIOUS-1); ++index_1){
                    int val_1 = domino_matrix2[index_1/R][index_1%R];
                    if(reminder_counts_draft1[val_1]>0){
                        indices[1]=index_1;
                        int reminder_counts_draft2[7];
                        copy(reminder_counts_draft1, reminder_counts_draft1+7, reminder_counts_draft2);
                        reminder_counts_draft2[val_1]--;

                        for(int index_2 = index_1 + 1; index_2 <= TOTAL_ELEMENTS - (NIMIOUS-2); ++index_2){
                            int val_2 = domino_matrix2[index_2/R][index_2%R];
                            if(reminder_counts_draft2[val_2]>0){
                                indices[2]=index_2;
                                int reminder_counts_draft3[7];
                                copy(reminder_counts_draft2, reminder_counts_draft2+7, reminder_counts_draft3);
                                reminder_counts_draft3[val_2]--;

                                for(int index_3 = index_2 + 1; index_3 < TOTAL_ELEMENTS; ++index_3){
                                    int val_3 = domino_matrix2[index_3/R][index_3%R];
                                   
                                    if(reminder_counts_draft3[val_3]>0){
                                        indices[3]=index_3;
                                        for(int row=0; row<L; ++row) {
                                            for(int col=0; col<R; ++col) {
                                                this->work_board.setValue(row, col, domino_matrix2[row][col]);
                                            }
                                        }
                                        for(int idx=0;idx<NIMIOUS; ++idx){
                                            int r = indices[idx]/R;
                                            int c = indices[idx]%R;
                                            this->work_board.setValue(r,c,-1);
                                        }
                                        this->reset_solver();
                                        if(this->solve()){
                                            cout<<"Розвязок головоломки знайдено \n";
                                            for(int solve_idx=0; solve_idx < this->solution_size; ++solve_idx){
                                                this->solution_arr[solve_idx].print();
                                                cout<<endl;
                                            }
                                            return true;
                                        }
                                        
                                    }
                                }
                            }
                        }            
                    }
                }        
            }
        }
        return false;
    }
};
   

int main(){
    Game_board view (DOMINO_MATRIX);
    view.print_board();
    Domino_solver solver(DOMINO_MATRIX);
    int target_digits_arr[NIMIOUS];
    int target_digits_size = 0;
    solver.find_repeat(DOMINO_MATRIX, target_digits_arr, target_digits_size);
    bool solution_found =  solver.solve_selection(DOMINO_MATRIX,target_digits_arr,target_digits_size);
    if (!solution_found) {
        cout << "\nРозвязок знайти не можливо для поля 6 на 10\n";
    }
}
