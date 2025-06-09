
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h>

#include "HS.h"
#include "Sspace.h"

extern SSPACE* GetNewSpace(int key, int Ssize);
extern SSPACE* GetSpaceAddr(int key);
extern SSPACE* GenModelFromFileS(int key, char* fname);

int makeK_max(int* M, int* K1_M, int* K2_M, int num);
int checkbase(int* M, int* K1_M, int* base_M, int num);
int checkloop(int* M, int* K1_M, int* base_M, int num);

int main(int argc, char* argv[])
{
    int i, j;
    int num;

    // 隣接行列
    int* M;
    //隣接行列＋単位行列
    int* K1_M;
    //可達行列
    int* K2_M;
    int* base_M;
    int* loop_M;
    int k;
    int count;
    int check;
    k = i = j = count = 0;

    scanf("%d", &num);

    M = (int*)malloc(num * num * sizeof(int)); //メモリ確保
    K1_M = (int*)malloc(num * num * sizeof(int));
    K2_M = (int*)malloc(num * num * sizeof(int));
    base_M = (int*)malloc(num * sizeof(int));
    loop_M = (int*)malloc(num * sizeof(int));
    //	printf("%d\n",num);

    for (i = 0; i <= num * num; i++) {
        M[i] = 0;
    }

    //	printf("隣接行列を入力してください[行:lower  列:upper]\n");
    for (i = 0; i < num * num; i++) {

        scanf("%d", &M[i]);
    }
    for (i = 0; i < num * num; i++) {
        K1_M[i] = M[i];
    }
    for (i = 0; i < num; i++) {
        base_M[i] = 0;
    }
    for (i = 0; i < num; i++) {
        loop_M[i] = 0;
    }

    check = makeK_max(M, K1_M, K2_M, num);
    if (check == -1) {
        printf("可達行列が作られませんでした\n");
    } else if (check == 1) {
        printf("%d\n",num);
        for(i=0;i<num;i++){
            for(j=0;j<num;j++){
                printf("%d ",K1_M[i*num+j]);
            }
            printf("\n");
        }


        checkbase(M, K1_M, base_M, num);
        checkloop(M, K1_M, base_M, num);
    }

    free(M);
    free(K1_M);
    free(K2_M);
    free(base_M);
    free(loop_M);

    return 0;
}

int makeK_max(int* M, int* K1_M, int* K2_M, int num)
{
    int i, j, k;
    int count;
    int flag;
    count = 0;
    flag = 0;

    for (i = 0; i < num; i++) {
        M[i * num + i] = 1;
    }

    for (i = 0; i < num * num; i++) {
        K1_M[i] = M[i];
    }

    for (;;) {
        for (i = 0; i < num; i++) {
            for (j = 0; j < num; j++) {
                for (k = 0; k < num; k++) {
                    K2_M[i * num + j] += K1_M[i * num + k] * M[k * num + j];
                }
            }
        }
        for (i = 0; i < num * num; i++) {
            if (K2_M[i] == 0) {
                K2_M[i] = 0;
            } else {
                K2_M[i] = 1;
            }
        }
        for (i = 0; i < num * num; i++) {
            if (K1_M[i] != K2_M[i]) {
                for (j = 0; j < num * num; j++) {
                    K1_M[j] = K2_M[j];
                }
                flag = 1;
                break;
            } else {
                flag = 0;
            }
        }

        if (flag == 0) {
            return (1);
        }
        if (count == 100) {
            return (-1);
        }
        count++;
    }

    return 0;
}

int checkbase(int* M, int* K1_M, int* base_M, int num)
{
    int i, j, k;
    int flag1, flag2;
    int countbase;
    countbase = 0;
    flag1 = flag2 = 0;

    for (i = 0; i < num; i++) {
        for (j = 0; j < num; j++) {
            if (K1_M[i * num + j] != 1) {
                flag1 = 0;
                break;
            } else {
                flag1 = 1;
            }
        }
        if (flag1 == 1) {
            base_M[i] = 1;

        } else {
            base_M[i] = 0;
        }
    }

    for (i = 0; i < num; i++) {
        if (base_M[i] == 1) {
            countbase++;
        }
    }

    if (countbase > 1) {
        printf("baseJointが%d個あります\n", countbase);
        printf("basejoint:");
        for (i = 0; i < num; i++) {
            if (base_M[i] == 1) {
                printf("%d\t", i);
            }
        }
        printf("\n");
    } else if (countbase == 0) {
        printf("baseJointがありません");
    } else if (countbase == 1) {
        for (i = 0; i < num; i++) {
            if (base_M[i] == 1) {
                printf("baseJoint:%d", i);
            }
        }
    }
    printf("\n");

    return 0;
}

int checkloop(int* M, int* K1_M, int* loop_M, int num)
{
    int i, j, k;
    k = 0;
    for (i = 0; i < num; i++) {
        for (j = 0; j < num; j++) {
            if (i != j) {
                if (K1_M[i * num + j] == K1_M[j * num + i]) {
                    if(K1_M[i * num + j]==1){
                    loop_M[i] = 1;
                    k++;  
                    }
                    
                }
            }
        }
    }
    if (k > 0) {
        printf("jointがループしています\n");

        printf("\n");
    }
    return 0;
}