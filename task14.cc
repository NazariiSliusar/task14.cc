#include <iostream>
#define L 6
#define R 10
#define NIMIOUS (L*R-56)

using namespace std;

int domino_matrix[L][R] = {
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
    Domino(int c1, int c2, int u1, int u2){
        x1=c1;
        x2=c2;
        y1=u1;
        y2=u2;
    }
    void print(){
        cout << "(" << x1 << "," << y1 << ") — (" << x2 << "," << y2 << ")";
    }

};

int main(){ 
}

void find_repeat(int domino_matrix[L][R], int more_8[NIMIOUS], int &size){
    int counts[7] = {0};
    size = 0;
    for (int i = 0; i < L; i++){
        for (int j = 0; j < R; j++){
            int val = domino_matrix[i][j];
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


    



