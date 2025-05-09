#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <stdbool.h>

// #include "HuyN_PBL_Graphic.h"
// #include "SDL.h"
// #include "SDL_ttf.h"

#define MAX 20 // số lượng thành phố tối đa
// Cấu trúc thành phố
typedef struct {
    int id;
    bool visited;
    int Cost_To_City[MAX]; // mảng lưu trữ chi phí đi từ TP này đến các TP khác

    // SDL_Pos position;

} city;
// Cấu trúc bài toán TSP
typedef struct {
    int cityAmount;// Số lượng thành phố
    int dp[1 << MAX][MAX];//Bảng luưu trữ kết quả cho quy hoạch động
    int path[MAX];// Đường đi cuối cùng của quy hoạch động
    city cities[MAX];// Danh sách các thành phố
} TSP;
// Cấu trúc lưu kết quả của nhánh cận
typedef struct {
    int currentPath[MAX];// đường đi hiện tại đang xét
    int bestPath[MAX];//đường đi tốt nhất đã tìm được
    int CP;// chi phí hiê tại
    int ans;// chi phí  nhỏ nhất đã tìm đc
} Answer;

TSP tsp;

Answer answer;

bool isLuuFile = false;

// dai dai
void setcolor(int background_color, int text_color) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    int color_code = background_color * 16 + text_color;
    SetConsoleTextAttribute(hStdout, color_code);
}
void info() {
    setcolor(0, 15);
    printf("\n      %c", 218);
    for (int i = 0; i < 86; i++) printf("%c", 196);
    printf("%c\n", 191);

    printf("      %c%87c\n", 179, 179);
    printf("      %c                         PBL1: DO AN LAP TRINH TINH TOAN                              %c\n", 179, 179);
    printf("      %c                          DE TAI: BAI TOAN NGUOI DU LICH                              %c\n", 179, 179);
    printf("      %c%87c\n", 179, 179);
    printf("      %c     SINH VIEN THUC HIEN                   |      GIANG VIEN HUONG DAN                %c\n", 179, 179);
    printf("      %c    - Nguyen Thanh Huy                     |     - Nguyen Van Hieu                    %c\n", 179, 179);
    printf("      %c    - Nguyen Dang Le Hoang                 |                                          %c\n", 179, 179);
    printf("      %c%87c\n", 179, 179);
    printf("      %c", 192);
    for (int i = 0; i < 86; i++) printf("%c", 196);
    printf("%c\n", 217);
    setcolor(0, 15);
}

// Nhập
void nhap(int t) {
    if (t == 2) printf("Nhap so thanh pho: ");
    scanf("%d", &tsp.cityAmount);
    if (t == 2) printf("Nhap ma tran chi phi:\n");
    for (int i = 0; i < tsp.cityAmount; i++){
        for (int j = 0; j < tsp.cityAmount; j++)
            scanf("%d", &tsp.cities[i].Cost_To_City[j]);
        tsp.cities[i].visited = false;
    }
    if (t == 1) {
        printf("DU LIEU SO THANH PHO VA MA TRAN CHI PHI:\n");
        printf("%d\n", tsp.cityAmount);
        for (int i = 0; i < tsp.cityAmount; i++) {
            for (auto j = 0; j < tsp.cityAmount; j++) {
                printf("%d ", tsp.cities[i].Cost_To_City[j]);
            }
            printf("\n");
        }
    }
}

// Chọn cách nhập dữ liệu
void laydulieu() {
    int a;
    printf("HAY CHON CACH NHAP DU LIEU:\n");
    printf("  1. Lay du lieu tu file.        \n");
    printf("  2. Nhap du lieu tu ban phim.   \n");
    printf("Lua chon cua ban: ");
    scanf("%d", &a);

    if (a == 1) {
        freopen("data.txt", "r", stdin);
    }
    else if (a != 2) {
        printf("Ban da nhap sai, hay nhap lai.\n");
        return laydulieu();
    }
    nhap(a);

    freopen("CON", "r", stdin);

    char c = 'n';
    printf("LUU KET QUA RA FILE (Y/N): ");
    scanf("%c", &c);
    if (tolower(c) == 'y') isLuuFile = true;
    else isLuuFile = false;
}


// In kết quả
void inKetQua(int cost, int route[]) {
    printf("Chi phi nho nhat la: %d\n", cost);
    printf("Duong di: ");
    for (int i = 0; i < tsp.cityAmount; i++) {
        printf("Thanh Pho %d -> ", route[i] + 1);
    }
    printf("Thanh Pho %d\n", route[0] + 1);

    freopen("result.txt", "w", stdout);

    printf("SO THANH PHO: %d \n", tsp.cityAmount);
    printf("CHI PHI:\n");
    for (int i = 0; i < tsp.cityAmount; i++) {
        for (int j = 0; j < tsp.cityAmount; j++) {
            printf("%d ", tsp.cities[i].Cost_To_City[j]);
        }
        printf("\n");
    }

    printf("Chi phi nho nhat la: %d\n", cost);
    printf("Duong di: ");
    for (int i = 0; i < tsp.cityAmount; i++) {
        printf("Thanh Pho %d -> ", route[i] + 1);
    }
    printf("Thanh Pho %d\n", route[0] + 1);
}

// Tìm chi phí tối thiểu giữa các thành phố
int min_out_cost() {
    int min = INT_MAX;
    for (int i = 0; i < tsp.cityAmount; i++)
        for (int j = 0; j < tsp.cityAmount; j++)
            if (i != j && tsp.cities[i].Cost_To_City[j] < min)
                min = tsp.cities[i].Cost_To_City[j];
    return min;
}

// Nhánh cận
void Try(int row) {
    for (int col = 1; col < tsp.cityAmount; col++) {
        if (!tsp.cities[col].visited) {
            tsp.cities[col].visited = true;
            answer.currentPath[row] = col;// ghi nhận thành phố vào đường đi hiện tại
            answer.CP += tsp.cities[answer.currentPath[row - 1]].Cost_To_City[col];// chi phí mới
// ước lượng chi ph nếu đi tiếp các thành phố còn lại
            int estimate = answer.CP + (tsp.cityAmount - row) * min_out_cost();
            if (row == tsp.cityAmount - 1) {// đã đi qua n -1 thành phố
                int totalCost = answer.CP + tsp.cities[col].Cost_To_City[0];// cộng thêm chi phí quay về TP đầu
                if (totalCost < answer.ans) { // nếu tốt hơn thì cập nhật
                    answer.ans = totalCost;
                    memcpy(answer.bestPath, answer.currentPath, sizeof(answer.currentPath));
                }
            } else if (estimate < answer.ans) {
                Try(row + 1);
            }
  // quay lui
            tsp.cities[col].visited = false;
            answer.CP -= tsp.cities[answer.currentPath[row - 1]].Cost_To_City[col];
        }
    }
}

void branch_and_bound() {
    answer.currentPath[0] = 0;
    tsp.cities[0].visited = true;
    answer.CP = 0;
    answer.ans = INT_MAX;
    Try(1);
    inKetQua(answer.ans, answer.bestPath);
}


// Quy hoạch động
int tsp_dp(int mask, int pos) {
    if (mask == (1 << tsp.cityAmount) - 1)// nếu đã đi tất cả các thành phố
        return tsp.cities[pos].Cost_To_City[0];// trở  về thành phố đầu tiên

    if (tsp.dp[mask][pos] != -1)
        return tsp.dp[mask][pos];

    int res = INT_MAX;
    for (int next = 0; next < tsp.cityAmount; next++) {
        if (!(mask & (1 << next))) {
            // Nếu TP next chưa được ghé
            // Tính chi phí đi từ pos → next rồi đi tiếp (đệ quy)

            int newCost = tsp.cities[pos].Cost_To_City[next] + tsp_dp(mask | (1 << next), next);
            if (newCost < res)
                res = newCost;// cập nhật chi phí tốt nhất
        }
    }
    return tsp.dp[mask][pos] = res;// lưu kết quả vào
}

void build_path_dp(int mask, int pos, int idx) {
    if (mask == (1 << tsp.cityAmount) - 1)
        return;

    for (int next = 0; next < tsp.cityAmount; next++) {
        if (!(mask & (1 << next))) {
            int expected = tsp.cities[pos].Cost_To_City[next] + tsp_dp(mask | (1 << next), next);
            if (expected == tsp.dp[mask][pos]) {
                tsp.path[idx] = next;
                build_path_dp(mask | (1 << next), next, idx + 1);
                return;
            }
        }
    }
}

void dynamic_programming() {
    memset(tsp.dp, -1, sizeof(tsp.dp));
    int total_cost = tsp_dp(1, 0);
    tsp.path[0] = 0;
    build_path_dp(1, 0, 1);
    inKetQua(total_cost, tsp.path);
}

void chayThuatToan() {
    int b;
    printf("HAY CHON PHUONG PHAP:\n");
    printf("1. Phuong phap nhanh can\n");
    printf("2. Phuong phap quy hoach dong\n");
    printf("Lua chon cua ban: ");
    scanf("%d", &b);
    if (b == 1) {
        printf("Phuong phap nhanh can:\n");
        branch_and_bound();
    }
    else if (b == 2) {
        printf("Phuong phap quy hoach dong:\n");
        dynamic_programming();
    }
    else {printf("Lua chon khong hop le!\n");
        return chayThuatToan();
    }
}

// MAIN
int main() {

    // if(SDL_Init(SDL_INIT_VIDEO) != 0){
    //     SDLGraphic_ErrorHandler("SDL_Init", SDL_GetError());
    // }
    //
    // if (TTF_Init() == -1) {
    //     SDLGraphic_ErrorHandler("TTF_Init", TTF_GetError());
    // }
    //
    // SDL_Window *window = SDL_CreateWindow("Huy_Hoang_PBL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE);
    // if(window == nullptr){
    //     SDLGraphic_ErrorHandler("SDL_CreateWindow", SDL_GetError());
    // }
    //
    // SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    // if (renderer == nullptr) {
    //     SDLGraphic_ErrorHandler("SDL_CreateRenderer", SDL_GetError());
    // }
    //
    // SDL_ShowWindow(window);


    info();
    laydulieu();
    chayThuatToan();
    return 0;
}