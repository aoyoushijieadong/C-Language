#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

#define CLEAR "\033[0m"                 //清除设置的格式
#define YELLOW "\033[1;33;40m"            //黄色
#define RED "\033[1;31;40m"            //红色

#define N 10

int add=0;       //这个变量是用于计算增加多少个名字

int random_name(char **name,int t);     //随机抽取名字所在的指针数组的下标值

int myMENU(int num);                    //主页面

int roll_call(char **name);             //显示出随机的名字

int nameList(char **name);              //显示名单

int alterName(char **name,char p[100][50]); //修改名字

int  addName(char **name,char p[100][50]);    //增加名字
