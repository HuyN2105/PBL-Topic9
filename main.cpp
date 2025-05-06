#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <SDL.h>

#define MAX 20

typedef struct {
    int city;
    int matrix[MAX][MAX];
    int visited[MAX];
    int X[MAX], Toiuu[MAX];
    int CP;
    int ans;
    int dp[1 << MAX][MAX];
    int path[MAX];

} TSP;

TSP tsp;
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
    scanf("%d", &tsp.city);
    printf("Nhap ma tran chi phi:\n");
    for (int i = 0; i < tsp.city; i++)
        for (int j = 0; j < tsp.city; j++)
            scanf("%d", &tsp.matrix[i][j]);
    memset(tsp.visited, 0, sizeof(tsp.visited));
}

// Chọn cách nhập dữ liệu
void laydulieu() {
    int a;
    printf("|--------------------------------|\n");
    printf("| 1. Lay du lieu tu file.        |\n");
    printf("| 2. Nhap du lieu tu ban phim.   |\n");
    printf("|--------------------------------|");
    printf("\nHay chon cach thuc:");
    scanf("%d", &a);

    if (a == 1) {
        printf("Da lay du lieu.\n");
        return ;
    }
    if (a == 2) nhap();
    else if (a != 1 && a!=2 )
        printf("Nhap sai du lieu.\n");
}

// In kết quả
void inKetQua(int cost, int route[]) {
    printf("Chi phi nho nhat la: %d\n", cost);
    printf("Duong di: ");
    for (int i = 0; i < tsp.city; i++) {
        printf("Thanh Pho %d -> ", route[i] + 1);
    }
    printf("Thanh Pho %d\n", route[0] + 1);
}

// Tìm chi phí tối thiểu giữa các thành phố
int min_out_cost() {
    int min = INT_MAX;
    for (int i = 0; i < tsp.city; i++)
        for (int j = 0; j < tsp.city; j++)
            if (i != j && tsp.matrix[i][j] < min)
                min = tsp.matrix[i][j];
    return min;
}

// Nhánh cận
void Try(int row) {
    for (int col = 1; col < tsp.city; col++) {
        if (!tsp.visited[col]) {
            tsp.visited[col] = 1;
            tsp.X[row] = col;
            tsp.CP += tsp.matrix[tsp.X[row - 1]][tsp.X[row]];

            int estimate = tsp.CP + (tsp.city - row) * min_out_cost();
            if (row == tsp.city - 1) {
                int totalCost = tsp.CP + tsp.matrix[tsp.X[row]][0];
                if (totalCost < tsp.ans) {
                    tsp.ans = totalCost;
                    for (int i = 0; i < tsp.city; i++)
                        tsp.Toiuu[i] = tsp.X[i];
                }
            } else if (estimate < tsp.ans) {
                Try(row + 1);
            }

            tsp.visited[col] = 0;
            tsp.CP -= tsp.matrix[tsp.X[row - 1]][tsp.X[row]];
        }
    }
}

void branch_and_bound() {
    tsp.X[0] = 0;
    tsp.visited[0] = 1;
    tsp.CP = 0;
    tsp.ans = INT_MAX;
    Try(1);
    inKetQua(tsp.ans, tsp.Toiuu);
}

// Quy hoạch động
int tsp_dp(int mask, int pos) {
    if (mask == (1 << tsp.city) - 1)
        return tsp.matrix[pos][0];

    if (tsp.dp[mask][pos] != -1)
        return tsp.dp[mask][pos];

    int res = INT_MAX;
    for (int next = 0; next < tsp.city; next++) {
        if (!(mask & (1 << next))) {
            int newCost = tsp.matrix[pos][next] + tsp_dp(mask | (1 << next), next);
            if (newCost < res) res = newCost;
        }
    }
    return tsp.dp[mask][pos] = res;
}

void build_path_dp(int mask, int pos, int idx) {
    if (mask == (1 << tsp.city) - 1)
        return;

    for (int next = 0; next < tsp.city; next++) {
        if (!(mask & (1 << next))) {
            int expected = tsp.matrix[pos][next] + tsp_dp(mask | (1 << next), next);
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

// MAIN
int main() {
    info();
    laydulieu();
    if (tsp.city < 10) {
        branch_and_bound();
    } else {
        dynamic_programming();
    }
    return 0;
}
