#include <iostream>
#include <set>
#include <utility>
#include <algorithm>
#include <cstring>

#define L 6
#define R 10
#define NIMIOUS (L*R-56) //кількість клітинок, які потрібно видалити
#define MAX_DOMINOES 28 //Максимальна кількість доміно
#define TOTAL_ELEMENTS (L*R) //Розмір матриці

using namespace std;
//Початкова матриця
const int DOMINO_MATRIX[L][R] = {
    {0, 0, 1, 1, 2, 3, 3, 5, 4, 0},
    {3, 3, 2, 5, 6, 6, 3, 4, 2, 0},
    {6, 3, 0, 6, 1, 1, 6, 4, 2, 0},
    {2, 2, 4, 3, 6, 6, 1, 3, 2, 2},
    {2, 4, 4, 0, 0, 5, 4, 5, 5, 5},
    {1, 1, 4, 6, 0, 5, 1, 3, 5, 3}
};
//Клас для роботи з ігровим полем
class Game_board{
    private:
    bool used[L][R];
    int board[L][R];
    public:
    //Конструктор створює чорнове поле та скидає стан використаних клітинок
    Game_board(const int domino_matrix_draft[L][R]){
        for (int i = 0; i < L; ++i){
            for (int j = 0; j < R; ++j) {
                board[i][j]=domino_matrix_draft[i][j];
                used[i][j]=false;
            }
        }
    }
    //Перевірка чи клітинка в доступних межах
    bool is_valid(int c, int u ){
        return c>=0 && u>=0 && c<L && u<R;
    }
    //Перевірка чи клітинка зайнята
    bool is_used(int c, int u ){
        return used[c][u];
    }
    //Функція для отримання значення координати
    int get_value(int c, int u ){
        return board[c][u];
    }
    //Функція для встановлення значення координати
    void set_value(int c, int u, int value){
        board[c][u] =value;
    }
    //Функція для встановлення зайнятості координати
    void set_used(int c, int u, bool value=true){
        used[c][u]=value;
    }
    //Функція скидання зайвих клітинок
    void reset_used(){
        for(int i=0; i<L;++i){
            for(int j=0; j<R; ++j){
                used[i][j]=false;
            }
        }
    }
    //Функція виводу стартового поля
    void print_board(){
        cout<<"Робоче поле:"<<endl<<endl;
        for(int i = 0; i < L; i++){
            for(int j = 0; j < R; j++){
                cout<< board[i][j]<<" ";
            }
            cout<<endl;
        }
    }
};
//Клас для роботи з фішками доміно, збереження, вивід
class Domino{
    private:
    //Значення координат пар доміно
    int x1,y1,x2,y2; 
    public:
    //Конструктор за замовчуванням
    Domino(){x1=0;y1=0;x2=0;y2=0;}
    //Конструктор для ініціалізації доміно координатами клітинок
    Domino(int c1, int u1, int c2, int u2){
        x1=c1;
        y1=u1;
        x2=c2;
        y2=u2;
    }
    //Вивід інформації про пари доміно
    void print(Game_board& board){
        int val_1 = board.get_value(x1,y1);
        int val_2 = board.get_value(x2,y2);
        cout << "(" << x1 << "," << y1 << ") — (" << x2 << "," << y2 << ")"<<" Domino: "<<"["<< val_1<<"|"<<val_2<<"]"<<endl;
    }
    //Гетери для доступу до полів
    int get_x1(){ return x1;}
    int get_y1(){ return y1;}
    int get_x2(){ return x2;}
    int get_y2(){ return y2;}
};
//Клас для візуалізації знайденого рішення
class Board_show_solve {
    public:
    // Метод для виведення розв'язаного поля
    static void show_solve(Game_board& board, const Domino solution_arr[], int solution_size) {
        char display[L * 2 + 1][R * 4 + 1];
        // Побудова пустого поля з рамками
        for(int i =0; i<= L *2;++i) {
            for(int j =0; j<= R * 4;++j) {
                if(i % 2 == 0){ //Парні рядки для горизонтальних ліній та перетинів
                    display[i][j] = (j % 4 == 0) ? '+' : '-';
                }else{ //Непарні рядки  для вертикальних ліній та значень
                    display[i][j] = (j % 4 == 0) ? '|' : ' ';
                }
            }
        }
        // Заповнення матриці значеннями з умови
        for(int r = 0; r < L; ++r){
            for(int c = 0; c < R; ++c){
                int val = board.get_value(r, c);
                if(val != -1){
                    display[2 * r + 1][4 * c + 2] =(char)('0'+val); //якщо клітинка не зайва виводимо її значення
                }else{//Якщо зайва виводимо (- -)
                    display[2 * r + 1][4 * c + 1] = '-';
                    display[2 * r + 1][4 * c + 2] = ' ';
                    display[2 * r + 1][4 * c + 3] = '-';
                }
            }
        }
        // Додавання горизонтальних або вертикальних зв'язків між частинами доміно в залежності від орієнтації
        for(int k = 0;k<solution_size;++k) {
            Domino d = solution_arr[k];
            int x1 =d.get_x1();
            int y1 =d.get_y1();
            int x2 =d.get_x2();
            int y2 =d.get_y2();
            if(x1 == x2&&y2 == y1+1) {
                // Горизонтальне доміно між клітинками '='
                display[2 * x1 + 1][4 * y1 + 4] = '=';
            }else if(y1 == y2&&x2 == x1 +1){
                // Вертикальне доміно доміно між клітинками '==='
                display[2 * x1 + 2][4 * y1 + 1] = '=';
                display[2 * x1 + 2][4 * y1 + 2] = '=';
                display[2 * x1 + 2][4 * y1 + 3] = '=';
            }
        }
        // Вивід поля
        for(int i=0;i <=L*2;++i){
            for(int j = 0; j <= R * 4; ++j){
                cout << display[i][j];
            }
            cout<<endl;
        }
    }
};
//Клас для пошуку розміщення пар доміно та пошуку зайвих клітинок
class Domino_solver{
    private:
    //Рух для пошуку сусідніх клітинок 0,1 (вправо) 1,0 (вниз)
    const int dx[2]={0,1};
    const int dy[2]={1,0};
    set<pair<int,int>> used_dominoes; //Контейнер для зберігань пар одним об'єктом
    Game_board work_board; //Робоче поле, яке можна змінювати
    Domino solution_arr[MAX_DOMINOES]; //масив для зберігання правильно розміщених пар
    int solution_size = 0;
    public:
    //Конструктор, ініціалізує робоче поле початковою матрицею
    Domino_solver(const int domino_mutrix[L][R]) : work_board(domino_mutrix) {}	
    //Скидання розв'язку
    void reset_solver(){
        solution_size=0;
        used_dominoes.clear();
        work_board.reset_used();
    }
    //Метод пошуку пар доміно
    bool solve() {
        for (int i = 0; i < L; i++) {
            for (int j = 0; j < R; j++) {
                // Перевірка на валідність клітинки
                if (!work_board.is_used(i, j) && work_board.get_value(i, j) != -1) {
                    for (int d = 0; d < 2; ++d) {
                        int ni = i + dx[d]; //Перехід на сусідню клітинку(горизонт)
                        int nj = j + dy[d]; //Перехід на сусідню клітинку(горизонт)
                        // Перевірка стану сусідньої клітинки
                        if (work_board.is_valid(ni, nj) && !work_board.is_used(ni, nj) &&work_board.get_value(ni, nj) != -1) {
                            //Значення пари
                            int a = work_board.get_value(i, j);
                            int b = work_board.get_value(ni, nj);
                            pair<int, int> domino = {min(a, b), max(a, b)};//Об'єднуємо, щоб пари до прикладу (2, 3) і (3, 2) вважались однаковими
                            if (used_dominoes.count(domino)) continue; //Якщо така пара вже використана пропускаємо
                            // Позначаємо клітинки як використані
                            work_board.set_used(i, j);
                            work_board.set_used(ni, nj);
                            used_dominoes.insert(domino);
                            solution_arr[solution_size++] = Domino(i, j, ni, nj); // Додаємо доміно до розв’язку
                            if (solve()) return true; //Якщо при такому розміщенні вийшло досягти успіху повертаємо розв'язок
                            --solution_size; //Ні, відкат
                            used_dominoes.erase(domino); //Видаляємо з використаних
                            //Позначаємо клітинки як не використані
                            work_board.set_used(i, j, false);
                            work_board.set_used(ni, nj, false);
                        }
                    }
                    return false; // Якщо не вдалося знайти пару
                }
            }
        }
        //Перевірка чи всі кілтинки з НЕ негативним значенням були покриті доміно
        int non_neg = 0;
        for (int i = 0; i < L; ++i) {
            for (int j = 0; j < R; ++j) {
                if (work_board.get_value(i, j) != -1) {
                    non_neg++;
                }
            }
        }
        return solution_size * 2 == non_neg;
    }

    //Метод пошуку значень зайвих клітинок    
    void find_repeat(const int domino_matrix1[L][R], int more_8[NIMIOUS], int &size){
        int counts[7] = {0};
        size = 0;
        for (int i = 0; i < L; i++){
            for (int j = 0; j < R; j++){
                int val = domino_matrix1[i][j];
                if (val >= 0 && val <= 6){
                    counts[val]++; //Кількість входження кожного числа
                }
            }
        }
        for (int i = 0; i < 7; i++){
            int over = counts[i] - 8; //Кількість надлишкових чисел
            for (int j = 0; j < over; j++) {
                more_8[size++] = i; //Додаємо надлишкове число
            }
        }
    }

    //Метод для знаходження комбінації розміщення зайвих цфир
    bool solve_selection(const int domino_matrix2[L][R], const int target_digits[NIMIOUS], int target_digits_size){
        //Рахуємо скільки разів кожне надлишкове значення зустрічається у видалених клітинках
        int reminder_counts[7]={0};
        for(int i=0; i<NIMIOUS; ++i){
            int digit = target_digits[i];
            reminder_counts[digit]++;
        }
        int indices[NIMIOUS];//Індекси, які будуть видалені

        for(int index_0 = 0; index_0 <= TOTAL_ELEMENTS - NIMIOUS; ++index_0){
            int val_0 = domino_matrix2[index_0/R][index_0%R];// Потенційний індекс на виключення
            if(reminder_counts[val_0]>0){// Перевірка чи входить до шуканих
                indices[0]=index_0; //Так, запам'ятовуємо
                int reminder_counts_draft1[7];
                copy(reminder_counts, reminder_counts+7, reminder_counts_draft1);//Копіювання для забезпечення незмінності початкових значень при наступних спробах пошуку комбінацій
                reminder_counts_draft1[val_0]--;// Зменшуємо лічильник для цього значенння

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
                                            //Копіюємо початкову матрицю в робоче поле
                                            for(int col=0; col<R; ++col) {
                                                this->work_board.set_value(row, col, domino_matrix2[row][col]);
                                            }
                                        }
                                        //Позначаємо вибрані клітинки як -1
                                        for(int idx=0;idx<NIMIOUS; ++idx){
                                            int r = indices[idx]/R;
                                            int c = indices[idx]%R;
                                            this->work_board.set_value(r,c,-1);
                                        }
                                        this->reset_solver();//Скидання стану розв'язувача після кожної спроби розміщення
                                        //Намагаємося розв'язати для такого розміщення блокованих цифр
                                        if(this->solve()){ //Якщо так, отримуємо вивід, якщо ні, пробуємо до поки не вийде
                                            cout<<endl<<"Розвязок головоломки знайдено:"<<endl;
                                            for(int solve_idx=0; solve_idx < this->solution_size; ++solve_idx){
                                                this->solution_arr[solve_idx].print(this->work_board);
                                            }
                                            cout<<endl<<endl;
											Board_show_solve::show_solve(this->work_board, this->solution_arr, this->solution_size);
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
    view.print_board(); //Відображення початкового поля
    Domino_solver solver(DOMINO_MATRIX);
    int target_digits_arr[NIMIOUS];
    int target_digits_size = 0;
    solver.find_repeat(DOMINO_MATRIX, target_digits_arr, target_digits_size); //Пошук зайвих чисел
    bool solution_found =  solver.solve_selection(DOMINO_MATRIX,target_digits_arr,target_digits_size); //Пошук рішення
    if (!solution_found) {
        cout << "\nРозвязок знайти не можливо для поля 6 на 10\n"; //Вивід при безуспішному розв'язку
    }
    return 0;
}
