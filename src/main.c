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

typedef struct {
    int id;
    bool visited;
    int Cost_To_City[MAX];

    // SDL_Pos position;

} city;

typedef struct {
    int cityAmount;
    int dp[1 << MAX][MAX];
    int path[MAX];
    city cities[MAX];
} TSP;

typedef struct {
    int currentPath[MAX];
    int bestPath[MAX];
    int CP;
    int ans;
} Answer;

TSP tsp;

Answer answer;

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
void nhap() {
    printf("Nhap so thanh pho: ");
    scanf("%d", &tsp.cityAmount);
    printf("Nhap ma tran chi phi:\n");
    for (int i = 0; i < tsp.cityAmount; i++){
        for (int j = 0; j < tsp.cityAmount; j++)
            scanf("%d", &tsp.cities[i].Cost_To_City[j]);
        tsp.cities[i].visited = false;
    }
}

// Chọn cách nhập dữ liệu
void laydulieu() {
    int a;
    printf("HAY CHON CACH NHAP DU LIEU:\n");
    printf("  1. Lay du lieu tu file.        \n");
    printf("  2. Nhap du lieu tu ban phim.   \n");
    printf("Lua chon cua ban:");
    scanf("%d", &a);

    if (a == 1) {
        FILE *f = fopen("PBL1.txt", "r");
        if (f == NULL) {
            printf("Loi lay file!");
            exit(1);
        }
        fscanf(f, "%d", &tsp.cityAmount);  // Đọc số thành phố từ file
        for (int i = 0; i < tsp.cityAmount; i++) {
            for (int j = 0; j < tsp.cityAmount; j++) {
                fscanf(f, "%d", &tsp.cities[i].Cost_To_City[j]);  // Đọc ma trận chi phí
            }
            tsp.cities[i].visited = false;
        }
        fclose(f);
    }
    else if (a == 2) nhap();
    else {
        printf("Ban da nhap sai, hay nhap lai.\n");
        return laydulieu();
    }
}


// In kết quả
void inKetQua(int cost, int route[]) {
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
            answer.currentPath[row] = col;
            answer.CP += tsp.cities[answer.currentPath[row - 1]].Cost_To_City[col];

            int estimate = answer.CP + (tsp.cityAmount - row) * min_out_cost();
            if (row == tsp.cityAmount - 1) {
                int totalCost = answer.CP + tsp.cities[col].Cost_To_City[0];
                if (totalCost < answer.ans) {
                    answer.ans = totalCost;
                    memcpy(answer.bestPath, answer.currentPath, sizeof(answer.currentPath));
                }
            } else if (estimate < answer.ans) {
                Try(row + 1);
            }

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
    if (mask == (1 << tsp.cityAmount) - 1)
        return tsp.cities[pos].Cost_To_City[0];

    if (tsp.dp[mask][pos] != -1)
        return tsp.dp[mask][pos];

    int res = INT_MAX;
    for (int next = 0; next < tsp.cityAmount; next++) {
        if (!(mask & (1 << next))) {
            int newCost = tsp.cities[pos].Cost_To_City[next] + tsp_dp(mask | (1 << next), next);
            if (newCost < res) res = newCost;
        }
    }
    return tsp.dp[mask][pos] = res;
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
    printf("CHON PHUONG PHAP:\n");
    printf("1. Nhanh can\n");
    printf("2. Quy hoach dong\n");
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
