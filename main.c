#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX 2147483647

typedef struct {
    // struct ChartGraph *prev;
    // struct ChartGraph *next;
    long length;
    int index;
}ChartGraph;

typedef struct {
    bool checked;
    long length_to_node;
}GraphsToCheck;

// variable to calc the index of a new graph inserted in the program
int counter_graph_inserted = -1;

int nodes=0;
int k=0;
int heap_size=0;

ChartGraph *chartGraph;


void getParameters();
void startCollect();
void addgraph();
void topK();
long calcNumber(char number[12],int number_counter);
void swap(ChartGraph *x, ChartGraph *y);
void maxHeapifyRoot(int index);
void insertInChart(long length,int index);
void insertNewGraph(int index);

int main() {

    getParameters();
    chartGraph = malloc(sizeof (ChartGraph)*(k));
    startCollect();
    free(chartGraph);
    return 0;
}

void startCollect() {
    char command[20];
    while (1){
        char *ptr = fgets(command,20,stdin);
        if (ptr == NULL){
            break;
        }
        else{
            if(command[0]=='A')
                addgraph();
            else
                topK();
        }
    }
}

void addgraph() {

    long matrix[nodes][nodes];
    char parameters_input[nodes*11];

    //populate the graph matrix
    for (int i = 0; i < nodes; i++) {
        char *useless = fgets(parameters_input, nodes*11, stdin);
        if(useless == NULL)
            return;
        int l = 0;
        int counter = 0;
        int number_counter = 0;
        char number[12];
        number[1]='\0';
        while (parameters_input[counter] != '\n') {
            if(parameters_input[counter]==','||parameters_input[counter+1]=='\n'){
                if(parameters_input[counter+1]=='\n'){
                    number[number_counter]=parameters_input[counter];
                    number_counter++;
                    number[number_counter]='\0';
                }
                long value = calcNumber(number,number_counter);
                matrix[i][l] = value;
                number[1]='\0';
                number_counter=0;
                l++;
            }
            else{
                number[number_counter]=parameters_input[counter];
                number_counter++;
                number[number_counter]='\0';
            }
            counter++;
        }
    }

//--------------------------------------------------------------------------------------------------------------------
    int next_node = 0;
    GraphsToCheck *graphsToCheck = malloc(sizeof(GraphsToCheck)*nodes);
    graphsToCheck[0].length_to_node = 0;
    graphsToCheck[0].checked = true;

    for (int i = 1; i < nodes ; i++) {
        graphsToCheck[i].length_to_node = MAX;
        graphsToCheck[i].checked = false;
    }

    while (1){
        for(int j=0;j<nodes;j++){
            if( matrix[next_node][j] + graphsToCheck[next_node].length_to_node < graphsToCheck[j].length_to_node && matrix[next_node][j] != 0 && next_node!= j){
                graphsToCheck[j].length_to_node = matrix[next_node][j] + graphsToCheck[next_node].length_to_node;
            }
        }
        long min = MAX;
        for(int i = 0; i < nodes ; i++){
            if(graphsToCheck[i].length_to_node < min && graphsToCheck[i].checked==false){
                next_node = i;
                min = graphsToCheck[i].length_to_node;
            }
        }
        if(min != MAX)
            graphsToCheck[next_node].checked = true;
        else
            break;

    }

    long sum = 0;
    for (int i = 0; i < nodes; i++) {
        if(graphsToCheck[i].length_to_node != MAX)
            sum += graphsToCheck[i].length_to_node;
    }
    // printf(" index: %d -> %ld\n",counter_graph_inserted+1,sum);
    free(graphsToCheck);
    counter_graph_inserted++;
    insertInChart(sum,counter_graph_inserted);
}

void insertInChart(long length, int index) {

    if (heap_size<k){
        //case first graph
        if(heap_size==0){
            heap_size++;
            chartGraph[0].length=length;
            chartGraph[0].index=index;
        }
        else {
            heap_size++;
            chartGraph[heap_size - 1].length = length;
            chartGraph[heap_size - 1].index = index;
            if(heap_size==10){
                for (int i=heap_size-1;i>=0;i--){
                    maxHeapifyRoot(i);
                }
            }
        }
    }
    else {
        if (chartGraph[0].length > length) {

            chartGraph[0].length = length;
            chartGraph[0].index = index;
            insertNewGraph(0);
        }
    }

    /* for (int i=0;i<heap_size;i++){
         printf("%ld ",chartGraph[i].length);
     }
     printf("\n");*/
}

void topK() {
    int i =0;
    while (i < heap_size){
        printf("%d",chartGraph[i].index);
        i++;
        if(i != heap_size)
            printf(" ");
    }
    printf("\n");
}
//get the value of nodes and k
void getParameters() {
    char parameters_input[50];
    char *useless = fgets(parameters_input,50,stdin);
    if(useless == NULL)
        return;
    char *pt;
    pt = strtok (parameters_input," ");
    while (pt != NULL) {
        if(nodes==0)
            nodes = atoi(pt);
        else
            k= atoi(pt);
        pt = strtok (NULL, ",");
    }
}

long calcNumber(char number[12],int number_counter) {
    long result = 0;
    int check_power = number_counter;
    int i = 0;
    while (number[i] != '\0') {
        long increment = number[i] - '0';
        switch (check_power) {
            case 11:
                increment = increment * 10000000000;
                break;
            case 10:
                increment = increment * 1000000000;
                break;
            case 9:
                increment = increment * 100000000;
                break;
            case 8:
                increment = increment * 10000000;
                break;
            case 7:
                increment = increment * 1000000;
                break;
            case 6:
                increment = increment * 100000;
                break;
            case 5:
                increment = increment * 10000;
                break;
            case 4:
                increment = increment * 1000;
                break;
            case 3:
                increment = increment * 100;
                break;
            case 2:
                increment = increment * 10;
                break;
            case 1:
                increment = increment * 1;
                break;
        }
        check_power--;
        result += increment;
        i++;
    }
    return result;
}

void maxHeapifyRoot(int index)
{
    int left_child = (2*index)+1;
    int right_child = (2*index)+2;
    int next_node;

    if (left_child < heap_size && chartGraph[left_child].length > chartGraph[index].length)
        next_node = left_child;
    else
        next_node = index;

    if (next_node != index)
    {
        swap(&chartGraph[index],&chartGraph[next_node]);
        maxHeapifyRoot(next_node);
    }

    if (right_child < heap_size && chartGraph[right_child].length > chartGraph[index].length)
        next_node = right_child;
    else
        next_node = index;

    if (next_node != index)
    {
        swap(&chartGraph[index],&chartGraph[next_node]);
        maxHeapifyRoot(next_node);
    }
}

void swap(ChartGraph *x, ChartGraph *y)
{
    ChartGraph temp = *x;
    *x = *y;
    *y = temp;

}

void insertNewGraph(int index){
    int left_child = (2*index)+1;
    int right_child = (2*index)+2;
    int next_node;
    if(left_child < heap_size && chartGraph[left_child].length > chartGraph[index].length && right_child < heap_size && chartGraph[right_child].length > chartGraph[index].length){
        if(chartGraph[left_child].length > chartGraph[right_child].length)
            next_node = left_child;
        else
            next_node = right_child;
    }
    else if (left_child < heap_size && chartGraph[left_child].length > chartGraph[index].length)
        next_node = left_child;
    else if (right_child < heap_size && chartGraph[right_child].length > chartGraph[index].length)
        next_node = right_child;
    else
        next_node=index;


    if (next_node != index)
    {
        swap(&chartGraph[index],&chartGraph[next_node]);
        insertNewGraph(next_node);
    }
}