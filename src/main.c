#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <windows.h>
#include <conio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h> // position randomizer

#include "HuyN_PBL_Graphic.h"
#include "HuyN_SDL_Button.h"
#include "SDL.h"
#include "SDL_ttf.h"


// ********************************************** VARIABLES & STRUCTS ********************************************** //



#define MAX 20 // số lượng thành phố tối đa


bool isRunning = true;
bool isSaveToFile = false;

typedef struct {
    int id;
    bool visited;
    int Cost_To_City[MAX];

    SDL_Pos position;

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


int currentPageId = 1;


HuyN_SDL_Button buttons[6] = {{
        .x = 20,
        .y = 20,
        .w = 220,
        .h = 40,
        .text = "MO FILE DU LIEU",
        .bgColor = {0xFF, 0xFF, 0xFF, 0xFF},
        .fgColor = {0x00, 0x00, 0x00, 0xFF}
    },  // OPEN FILE
    {
        .x = 20,
        .y = 80,
        .w = 220,
        .h = 40,
        .text = "NHANH CAN",
        .bgColor = {0xFF, 0xFF, 0xFF, 0xFF},
        .fgColor = {0x00, 0x00, 0x00, 0xFF}
    },   // NHANH CAN
    {
        .x = 20,
        .y = 140,
        .w = 220,
        .h = 40,
        .text = "QUY HOACH DONG",
        .bgColor = {0xFF, 0xFF, 0xFF, 0xFF},
        .fgColor = {0x00, 0x00, 0x00, 0xFF}
    },   // QUY HOACH DONG
    {
        .x = 20,
        .y = 660,
        .w = 160,
        .h = 40,
        .text = "LUU FILE",
        .bgColor = {0xFF, 0xFF, 0xFF, 0xFF},
        .fgColor = {0x00, 0x00, 0x00, 0xFF}
    },   // QUY HOACH DONG
    {
        .x = 190,
        .y = 680,
        .w = 55,
        .h = 20,
        .text = "CREDIT",
        .bgColor = {0xFF, 0xFF, 0xFF, 0xFF},
        .fgColor = {0x00, 0x00, 0x00, 0xFF}
    },
    {
        .x = 20,
        .y = 20,
        .w = 100,
        .h = 40,
        .text = "RETURN",
        .bgColor = {0xFF, 0xFF, 0xFF, 0xFF},
        .fgColor = {0x00, 0x00, 0x00, 0xFF}
    },
};

int buttonsMenuId[6] = {1, 1, 1, 1, 1, 2};


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



// ********************************************** RENDER PRESET ********************************************** //



void SDLGraphic_ReRenderPreset(SDL_Renderer *renderer, const bool isAwaitingInput)
{
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderClear(renderer);


    for (int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++)
    {
        if (buttonsMenuId[i] != currentPageId) continue;
        SDLButton_draw(renderer, buttons[i]);
    }

    if (currentPageId == 1){

        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderDrawLine(renderer, 260, 0, 260, 720);

        for (int i = 0; i < tsp.cityAmount; i++)
        {
            const city city_i = tsp.cities[i];
            for (int j = 0; j < tsp.cityAmount; j++)
            {
                if (i == j) continue;
                SDLGraphic_ConnectNode(renderer, city_i.position, tsp.cities[j].position, city_i.Cost_To_City[j], false, i, j);
            }
        }

        for (int i = 0; i < tsp.cityAmount; i++) SDLGraphic_DrawNode(renderer, tsp.cities[i].position, i + 1, false);
    } else if (currentPageId == 2)
    {
        SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_Rect containerRect = { 150, 50, 980, 620};
        SDL_RenderDrawRect(renderer, &containerRect);

        SDL_RenderDrawLine(renderer, containerRect.x + containerRect.w / 2, 210, containerRect.x + containerRect.w / 2, containerRect.y + containerRect.h - 50);

        SDL_Rect PBL_TITLE_rect = { 307, containerRect.y + 50, 665, 50};
        SDLGraphic_RenderText(renderer, "PBL1: DO AN LAP TRINH TINH TOAN", 40, (SDL_Color){0xFF, 0xFF, 0xFF, 0xFF}, PBL_TITLE_rect);

        SDL_Rect PBL_PROJECT_NAME_rect = { 370, PBL_TITLE_rect.y + PBL_TITLE_rect.h, 540, 40};
        SDLGraphic_RenderText(renderer, "DE TAI: BAI TOAN NGUOI DU LICH", 35, (SDL_Color){0xFF, 0xFF, 0xFF, 0xFF}, PBL_PROJECT_NAME_rect);

        SDL_Rect SV_THUC_HIEN_rect = { containerRect.x + 95, PBL_PROJECT_NAME_rect.y + PBL_PROJECT_NAME_rect.h + 50, 300, 35};
        SDLGraphic_RenderText(renderer, "SINH VIEN THUC HIEN", 30, (SDL_Color){0xFF, 0xFF, 0xFF, 0xFF}, SV_THUC_HIEN_rect);


        SDL_Rect SV_1_rect = { containerRect.x + 42, SV_THUC_HIEN_rect.y + SV_THUC_HIEN_rect.h + 83, 355, 30};
        SDLGraphic_RenderText(renderer, "Nguyen Thanh Huy", 25, (SDL_Color){0xFF, 0xFF, 0xFF, 0xFF}, SV_1_rect);
        SDL_Rect SV_1_id_rect = { 483, SV_THUC_HIEN_rect.y + SV_THUC_HIEN_rect.h + 83, 125, 30};
        SDLGraphic_RenderText(renderer, "102240311", 25, (SDL_Color){0xFF, 0xFF, 0xFF, 0xFF}, SV_1_id_rect);

        SDL_Rect SV_2_rect = { containerRect.x + 42, SV_1_rect.y + SV_1_rect.h + 83, 260, 30};
        SDLGraphic_RenderText(renderer, "Nguyen Dang Le Hoang", 25, (SDL_Color){0xFF, 0xFF, 0xFF, 0xFF}, SV_2_rect);
        SDL_Rect SV_2_id_rect = { SV_2_rect.x + SV_2_rect.w + 31, SV_1_rect.y + SV_1_rect.h + 83, 125, 30};
        SDLGraphic_RenderText(renderer, "102240308", 25, (SDL_Color){0xFF, 0xFF, 0xFF, 0xFF}, SV_2_id_rect);


        SDL_Rect GV_HUONG_DAN_rect = { containerRect.x + containerRect.w - 75 - 340, PBL_PROJECT_NAME_rect.y + PBL_PROJECT_NAME_rect.h + 50, 340, 35};
        SDLGraphic_RenderText(renderer, "GIANG VIEN HUONG DAN", 30, (SDL_Color){0xFF, 0xFF, 0xFF, 0xFF}, GV_HUONG_DAN_rect);

        SDL_Rect GV_rect = { containerRect.x + containerRect.w - 127 - 235, GV_HUONG_DAN_rect.y + GV_HUONG_DAN_rect.h + 83, 235, 30};
        SDLGraphic_RenderText(renderer, "Ts. Nguyen Van Hieu", 25, (SDL_Color){0xFF, 0xFF, 0xFF, 0xFF}, GV_rect);


        SDL_Rect LopSH_rect = {containerRect.x + (containerRect.w - 120) / 2, containerRect.y + containerRect.h - 40, 120, 30};
        SDLGraphic_RenderText(renderer, "24T_Nhat1", 25, (SDL_Color){0xFF, 0xFF, 0xFF, 0xFF}, LopSH_rect);


    }


    // SAVE TO FILE CHECKBOX

    if (currentPageId == 1){
        const SDL_Rect rect = {140, 663, 30, 30};
        SDL_SetRenderDrawColor(renderer, 0x50, 0x50, 0x50, 0xFF);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xFF);
        SDL_RenderFillRect(renderer, &rect);
        if (isSaveToFile) SDLGraphic_RenderDrawTick(renderer, rect.x, rect.y);
    }

    if (!isAwaitingInput) SDL_RenderPresent(renderer);
}



// ********************************************** DATA INPUT ********************************************** //


void nhap(int t) {
    if (t == 2) printf("Nhap so thanh pho: ");
    scanf("%d", &tsp.cityAmount);
    if (t == 2) printf("Nhap ma tran chi phi:\n");
    for (int i = 0; i < tsp.cityAmount; i++){
        for (int j = 0; j < tsp.cityAmount; j++)
            scanf("%d", &tsp.cities[i].Cost_To_City[j]);
        tsp.cities[i].visited = false;
    }
    printf("DU LIEU:\n");
    if (t == 1) {
        printf("%d\n", tsp.cityAmount);
        for (int i = 0; i < tsp.cityAmount; i++) {
            for (int j = 0; j < tsp.cityAmount; j++) {
                printf("%d ", tsp.cities[i].Cost_To_City[j]);
            }
            printf("\n");
        }
    }
}


int numInputProcess(const int keysym)
{
    // 0 -> 9 : 48 -> 57
    if (keysym >= 48 && keysym <= 57)
    {
        return keysym - 48;
    }
    // numpad: 1->9 : 1073741913 -> 1073741921 | 0 : 1073741922
    if (keysym >= 1073741913 && keysym <= 1073741922) return (keysym == 1073741922 ? 0 : keysym - 1073741912);
    // not number input
    return -1;
}

void keyInputWait(SDL_Renderer *renderer, SDL_Event _event, const SDL_Pos _p_node1, const SDL_Pos _p_node2, int *cost, const int id1, const int id2)
{

    SDLGraphic_ConnectNode(renderer, _p_node1, _p_node2, -1, true, id1, id2);
    SDL_RenderPresent(renderer);

    bool awaitInput = true;
    *cost = 0;
    while (awaitInput)
    {
        while (SDL_PollEvent(&_event))
        {
            switch (_event.type)
            {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                switch (_event.key.keysym.sym)
                {
                    case SDLK_KP_ENTER:
                        awaitInput = false;
                        break;
                    case SDLK_RETURN:
                        awaitInput = false;
                        break;
                    case SDLK_BACKSPACE:
                        *cost = trunc(*cost / 10);
                        break;
                    default:
                        if (numInputProcess(_event.key.keysym.sym) != -1) *cost = *cost * 10 + numInputProcess(_event.key.keysym.sym);
                        break;
                }
                break;
            default:
                SDLGraphic_ReRenderPreset(renderer, true);
                SDLGraphic_ConnectNode(renderer, _p_node1, _p_node2, *cost, true, id1, id2);
                SDL_RenderPresent(renderer);
                break;
            }
        }
    }
}


void addCity(SDL_Renderer *renderer, SDL_Pos _p, SDL_Event _event)
{
    tsp.cityAmount++;
    int currentCity_temp = tsp.cityAmount - 1;
    tsp.cities[currentCity_temp].position = _p;
    SDLGraphic_DrawNode(renderer, _p, tsp.cityAmount, true);
    for (int i = 0; i < currentCity_temp; i++)
    {
        int costAB, costBA;

        printf("CHI PHI KHI DI TU THANH PHO %d SANG THANH PHO %d: \n", tsp.cityAmount, i+1);
        keyInputWait(renderer, _event, _p, tsp.cities[i].position, &costAB, tsp.cityAmount - 1, i);

        tsp.cities[currentCity_temp].Cost_To_City[i] = costAB;

        printf("CHI PHI KHI DI TU THANH PHO %d SANG THANH PHO %d: \n", i + 1, tsp.cityAmount);
        keyInputWait(renderer, _event, tsp.cities[i].position, _p, &costBA, i, tsp.cityAmount - 1);

        tsp.cities[i].Cost_To_City[currentCity_temp] = costBA;
        printf("\n");
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
        freopen("data.txt", "r", stdin);
    }
    else if (a != 2) {
        printf("Ban da nhap sai, hay nhap lai.\n");
        return laydulieu();
    }
    nhap(a);
    freopen("CON", "r", stdin);
}

char* openFileDialog() {
    OPENFILENAME ofn;
    char szFile[260] = {0};
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

    if (GetOpenFileName(&ofn) == TRUE) {
        return _strdup(ofn.lpstrFile); // Return a duplicate of the file path
    }
    return NULL; // Return NULL if canceled
}


void get_data_from_file()
{
    const char *filePath = openFileDialog();

    if (filePath == NULL) return;

    freopen(filePath, "r", stdin);

    int cityAmount;
    int costMatrix[MAX][MAX];

    if (scanf("%d", &cityAmount) != 1) {
        printf("Error reading city amount.\n");
        return;
    }
    tsp.cityAmount = cityAmount;
    printf("Number of cities: %d\n", cityAmount);

    // Read the cost matrix
    for (int i = 0; i < cityAmount; i++) {
        for (int j = 0; j < cityAmount; j++) {
            if (scanf("%d", &costMatrix[i][j]) != 1) {
                printf("Error reading cost matrix.\n");
                return;
            }
            tsp.cities[i].Cost_To_City[j] = costMatrix[i][j];
        }
    }

    // Seed the random number generator
    srand((unsigned int)time(NULL));

    // Generate non-overlapping random positions
    const int minDistance = 40; // Minimum distance between nodes
    const int windowWidth = 1280;
    const int windowHeight = 720;
    const int margin = 50; // Margin from window edges
    for (int i = 0; i < cityAmount; i++) {
        bool overlap;
        do {
            overlap = false;
            tsp.cities[i].position.x = margin + (rand() % (windowWidth - 2 * margin));
            tsp.cities[i].position.y = margin + (rand() % (windowHeight - 2 * margin));

            // Check overlap with all previous cities
            for (int j = 0; j < i; j++) {
                int dx = tsp.cities[i].position.x - tsp.cities[j].position.x;
                int dy = tsp.cities[i].position.y - tsp.cities[j].position.y;
                int distance = (int)sqrt(dx * dx + dy * dy);
                if (distance < minDistance) {
                    overlap = true;
                    break;
                }
            }
        } while (overlap);
    }

    free((void*)filePath); // Free the duplicated string
    freopen("CON", "r", stdin); // SWITCH BACK TO CONSOLE
}



// ********************************************** ALGORITHMs ********************************************** //



void inKetQua(int cost, int route[]) {
    printf("\n");
    printf("Chi phi nho nhat la: %d\n", cost);
    printf("Duong di: ");
    for (int i = 0; i < tsp.cityAmount; i++) {
        printf("Thanh Pho %d -> ", route[i] + 1);
    }
    printf("Thanh Pho %d\n", route[0] + 1);

    if (isSaveToFile)
    {
        freopen("result.txt", "w", stdout);
        for (int i = 0; i < tsp.cityAmount; i++)
        {
            printf("%d ", route[i] + 1);
        }
        freopen("CON", "w", stdout);
        printf("\n###### DA LUU KET QUA VAO FILE RESULT.TXT ######\n");
    }

}

// Tìm chi phí tối thiểu giữa các thành phố
int min_out_cost() {
    int min = INT_MAX;
    for (int i = 0; i < tsp.cityAmount; i++) {
        for (int j = 0; j < tsp.cityAmount; j++) {
            if (i != j && tsp.cities[i].Cost_To_City[j] < min) {
                min = tsp.cities[i].Cost_To_City[j];
            }
        }
    }
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
        const double beforeAlgoTick = SDL_GetTicks();
        branch_and_bound();
        const double afterAlgoTick = SDL_GetTicks();
        printf("NHANH CAN HOAN THANH TINH TOAN TRONG: %.3lfs\n", (afterAlgoTick - beforeAlgoTick) / 1000.0);
    }
    else if (b == 2) {
        printf("Phuong phap quy hoach dong:\n");
        const double beforeAlgoTick = SDL_GetTicks();
        dynamic_programming();
        const double afterAlgoTick = SDL_GetTicks();
        printf("QUY HOACH DONG HOAN THANH TINH TOAN TRONG: %.3lfs\n", (afterAlgoTick - beforeAlgoTick) / 1000.0);
    }
    else {printf("Lua chon khong hop le!\n");
        return chayThuatToan();
    }
}



// ********************************************** MAIN ********************************************** //

int main(int argc, char *argv[]) {

    info();

    tsp.cityAmount = 0;

    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        SDLGraphic_ErrorHandler("SDL_Init", SDL_GetError());
    }

    if (TTF_Init() == -1) {
        SDLGraphic_ErrorHandler("TTF_Init", TTF_GetError());
    }

    SDL_Window *window = SDL_CreateWindow("Huy_Hoang_PBL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_HIDDEN);
    if(window == nullptr){
        SDLGraphic_ErrorHandler("SDL_CreateWindow", SDL_GetError());
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == nullptr) {
        SDLGraphic_ErrorHandler("SDL_CreateRenderer", SDL_GetError());
    }

    SDL_ShowWindow(window);

    // info();
    // laydulieu();
    // chayThuatToan();

    isRunning = true;
    SDL_Event event;
    SDL_Pos MousePos = {0, 0};

    while (isRunning)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    isRunning = false;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT){
                        SDL_GetMouseState(&MousePos.x, &MousePos.y);

                        if (SDLButton_isContain(MousePos, buttons[0]) && buttonsMenuId[0] == currentPageId)
                        {
                            get_data_from_file();
                            break;
                        }
                        if (SDLButton_isContain(MousePos, buttons[1]) && buttonsMenuId[1] == currentPageId)
                        {
                            const double currentTick = SDL_GetTicks();
                            branch_and_bound();
                            const double afterAlgoTick = SDL_GetTicks();
                            printf("NHANH CAN HOAN THANH TINH TOAN TRONG: %.3lfs\n", abs(currentTick - afterAlgoTick) / 1000.0);
                            break;
                        }
                        if (SDLButton_isContain(MousePos, buttons[2]) && buttonsMenuId[2] == currentPageId)
                        {
                            const double currentTick = SDL_GetTicks();
                            dynamic_programming();
                            const double afterAlgoTick = SDL_GetTicks();
                            printf("NHANH CAN HOAN THANH TINH TOAN TRONG: %.3lfs\n", abs(currentTick - afterAlgoTick) / 1000.0);
                            break;
                        }
                        if (SDLButton_isContain(MousePos, buttons[3]) && buttonsMenuId[3] == currentPageId)
                        {
                            isSaveToFile = !isSaveToFile;
                            break;
                        }

                        if (SDLButton_isContain(MousePos, buttons[4]) && buttonsMenuId[4] == currentPageId)
                        {
                            currentPageId = 2;
                            break;
                        }
                        if (SDLButton_isContain(MousePos, buttons[5]))
                        {
                            currentPageId = 1;
                            break;
                        }

                        if (currentPageId == 1) addCity(renderer, MousePos, event);

                        break;
                    } if (event.button.button == SDL_BUTTON_RIGHT)
                    {
                        // TODO: right click delete node
                    }
                case SDL_MOUSEMOTION:
                    SDL_GetMouseState(&MousePos.x, &MousePos.y);
                    for (int i = 0; i < sizeof(buttons) / sizeof(buttons[0]); i++)
                    {
                        if (SDLButton_isContain(MousePos, buttons[i]) && currentPageId == buttonsMenuId[i])
                        {
                            SDL_Cursor *cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
                            SDL_SetCursor(cursor);
                            buttons[i].bgColor = (SDL_Color){ 0x00, 0xFF, 0xFF, 0xFF};
                            break;
                        }
                        buttons[i].bgColor = (SDL_Color){ 0xFF, 0xFF, 0xFF, 0xFF};
                        SDL_Cursor *cursor = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);
                        SDL_SetCursor(cursor);
                    }
                    break;
                default:
                    break;
            }
        }

        SDLGraphic_ReRenderPreset(renderer, false);

    }

    SDL_Quit();
    TTF_Quit();

    return 0;
}
