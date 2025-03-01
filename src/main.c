#include <stdio.h>
#include <stdlib.h>

#define PBL_Huy_Hoang_ int main()

struct NodeLinkProperties {
    _Bool linked;
    int weight;
};

// ERROR HANDLER

void handleError(const char* message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(1);
}


// Global variables

struct NodeLinkProperties **vNodeLink = nullptr;

#define ROWS 10
#define COLS 10

// Functions

struct NodeLinkProperties single_node_link_init() {
    struct NodeLinkProperties nodeLinkProperties;
    nodeLinkProperties.linked = false;
    nodeLinkProperties.weight = 0;
    return nodeLinkProperties;
}

void node_link_properties_init() {
    vNodeLink = malloc(ROWS * sizeof(struct NodeLinkProperties *));
    for (int i = 0; i < ROWS; i++) {
        vNodeLink[i] = malloc(COLS * sizeof(struct NodeLinkProperties));
        if (vNodeLink[i] == NULL) {
            handleError("Error: Memory allocation failed\n");
        }
        for (int j = 0; j < COLS; j++) {
            vNodeLink[i][j] = single_node_link_init();
        }
    }
}

void addEdge(const int i,const int j,const int weight) {
    if (vNodeLink == NULL) {
        handleError("Error: vNodeLink is null\n");
    }
    vNodeLink[i][j].linked = true;
    vNodeLink[i][j].weight = weight;
}

void removeEdge(const int i, const int j) {
    if (vNodeLink == NULL) {
        handleError("Error: vNodeLink is NULL\n");
    }
    vNodeLink[i][j].linked = false;
    vNodeLink[i][j].weight = 0;
}

void printGraph() {
    printf("Link Graph:\n");
    if (vNodeLink == NULL) {
        handleError("Error: vNodeLink is null\n");
        return;
    }
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (vNodeLink[i][j].linked) {
                printf("Node %d is linked to Node %d with weight %d\n", i, j, vNodeLink[i][j].weight);
            }
        }
    }
}


// MAIN

PBL_Huy_Hoang_ {
    node_link_properties_init();

    // Example usage
    addEdge(0, 1, 5);
    addEdge(1, 0, 10);
    addEdge(2, 1, 99);

    printGraph();

    // Free allocated memory
    for (int i = 0; i < 10; i++) {
        free(vNodeLink[i]);
    }
    free(vNodeLink);
    return 0;
}