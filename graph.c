#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

vertex* createVertexNode();

vertex* CreateVertex(void* data){
    vertex* newNode = createVertexNode();
    newNode->data = data;
    return newNode;
}

void AddEdge(vertex* from, vertex* to){
    if(!Contains(from->edges, to)){
        AddNode(from->edges, to);
    }
}

int HasEdge(vertex* from, vertex* to){
    if(Contains(from->edges, to)){
        return 1;
    } else {
        return 0;
    }
}

void* GetData(vertex* vertex){
    if(vertex == NULL){
        return NULL;
    }
    return vertex->data;
}

void DeleteVertex(vertex* vertex){
    DeleteLinkedList(vertex->edges);
    free(vertex);
}

vertex* createVertexNode(){
    vertex* newNode = malloc(sizeof(vertex));
    newNode->data = NULL;
    newNode->edges = CreateLinkedList();
    return newNode;
}

int DetectCycleInGraph(list_node* start){
    //To-do: Detect cycle and exit with error if one is found. You can use CycleInGraphError in error_handle module.
    return 0;
}