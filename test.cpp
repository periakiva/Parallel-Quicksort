#include<assert.h>
#include<sys/time.h>
#include<ctime>
#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
#include<pthread.h>
using namespace std;

struct partlimits{
    int *arr;
    int low;
    int high;
    int num_th;
};

void parallel_quicksort(int *arr, int low, int high, int num_th);
void quickSort(int arr[], int low, int high);
void printArray(int arr[], int arraySize);
void swap(int* a, int* b){
    int t = *a;
    *a = *b;
    *b = t;

}

int partition(int arr[],int low, int high){

    int pivot = arr[high];
    int i = low;
    for (int j=high-1;j>=i;--j){
        if(arr[i]>pivot){
            if(arr[j]<pivot){
                swap(&arr[i],&arr[j]);
            }
        }else{
            i++;
            j++;
        }
    }
    //printArray(arr,5);
//    cout << "arr[i]: " << arr[i] << " " << "arr[high]: " << arr[high] << endl;
    swap(&arr[i],&arr[high]);
   // printArray(arr,5);
    return (i);
}

void* quicksortThread(void* arg){
    struct partlimits *i = (struct partlimits*) arg;
    parallel_quicksort(i->arr, i->low,i->high,i->num_th);
    pthread_exit(0);
    //return NULL;
}

void parallel_quicksort(int *arr, int low, int high, int num_th){
/*    cout << "low: " << low << endl;
    cout << "high: " << high << endl<<endl; */
    if(low<high){
        int pi = partition(arr,low,high);
        //cout << "pivot: " <<  pi << endl;
        if (num_th-- > 0){
            pthread_t t1;
            //pthread_t t2;
            struct partlimits arg = {arr, low,pi-1,num_th};
            int ret = pthread_create(&t1,NULL,quicksortThread,&arg);
            assert((ret==0) && "Thread creation failed"); 
            //struct partlimits right = {arr,pi+1,high,num_th};
            //pthread_create(&t2, NULL, quicksortThread,&arg);
            cout << "Threads left to use: " << num_th << endl;
/*          cout << "before entering:" << endl;
            cout << "low: " << low << endl;
            cout << "high: " << high << endl;*/
            parallel_quicksort(arr,pi+1,high,num_th);
            pthread_join(t1,NULL);
            //pthread_join(t2,NULL);
        }else{
            cout << "Using sequential algorithm" << endl;
            quickSort(arr,low,pi-1);
            quickSort(arr,pi+1,high);
    }

    }else{
        return;
    }
}

void quickSort(int *arr, int low, int high){

    if (low < high){
        int pi = partition(arr,low,high);
        quickSort(arr,low,pi-1);
        quickSort(arr,pi+1,high);
    }

}

void printArray(int arr[], int arraySize){
    for(int i=0;i<arraySize;i++)
        cout << arr[i] << " ";     
        cout << endl;

}

int main(int argc, char **argv){
    
    if(argc<2){
        printf("usage: %s <size of array>\n", argv[0]);
        exit(-1);
    }
    struct timespec start,finish;
    double elapsed;
    struct partlimits args;
    int num_th = 100;

    //Get Array of random numbers
    srand(time(NULL));

    int arraySize = atoll(argv[1]);
    int *ar = (int*)calloc(arraySize, sizeof(int));
    assert(ar && "Allocation failed");
    //int ar[] = {54,95,83,57,21};
    for(int i=0; i<arraySize;i++){
        ar[i] = rand() % 100 +1;
    }
    args.arr = ar;
    cout << "Original Array:\n";
    //printArray(args.arr,arraySize);
    clock_gettime(CLOCK_MONOTONIC, &start);
    parallel_quicksort(args.arr,0,arraySize-1,num_th);
    //quickSort(args.arr,0,arraySize-1);
    clock_gettime(CLOCK_MONOTONIC,&finish);
    elapsed = (finish.tv_sec - start.tv_sec);
    elapsed+=(finish.tv_nsec-start.tv_nsec)/1000000000.0;
    //double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    //printf("elapsed time is %.21f seconds.\n ", epalsed);
    cout << "Amount of time: " << elapsed << endl;

    cout << "Final Array:\n";
    //printArray(args.arr,arraySize);
    return 0;
}

