#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>
using namespace std;
//thread function tpo generate a sum of 0 to N
struct sum_runner_struct{
   long long limit;
   long long answer;
};
// sum computed by the background

void* sum_runner(void* arg){
    struct sum_runner_struct *arg_struct = (struct sum_runner_struct*) arg;
    
    long long sum = 0;
    for (long long i =0; i<=arg_struct->limit; i++){
        sum+=i;
    }
    arg_struct->answer = sum; 
    pthread_exit(0);
}

int main(int argc, char **argv){

    if (argc < 2){
        printf("usage: %s <num1> <num2> <num3>\n", argv[0]);
        exit(-1);
    }
    int num_args = argc -1;
    //thread id:
    pthread_t tids[num_args];

    struct sum_runner_struct args[num_args];
    // create attributes
    for(int i =0;i<num_args;i++){
        args[i].limit = atoll(argv[i+1]);
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&tids[i],&attr,sum_runner, &args[i]);
    } 
    //wait yuntil thread is done its work

    for(int i =0;i<num_args;i++){
        pthread_join(tids[i],NULL);
        printf("sum is %lld\n",args[i].answer);
    }

}
