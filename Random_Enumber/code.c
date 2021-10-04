#include "code.h"

int main(int argc, char const *argv[])
{
    FILE *fb;
    fb = fopen(argv[1], "r");

    if (fb == NULL)
        printf("文件打开失败\n");
    else
        printf("文件打开成功\n");

    char p[100][50] = {0};    //用来存储每行的字符串
    char *name[100] = {NULL}; //用来存储指向字符串的指针

    //获取一个随机种子
    srand((unsigned)time(NULL));

    for (int i = 0; i < N; i++)
        name[i] = fgets(p[i], 100, fb); //获取各行字符串首地址

    fclose(fb);

    for (int i = 0; i < N; i++)
    {
        //将字符串中的\n改为\0
        for (int j = 0; name[i][j] != '\0'; j++)
        {

            if (name[i][j] == '\n')
                name[i][j] = '\0';
        }
    }

    int num = 0;

    while (num != 5)
    {
        switch (num)
        {
        case 0:
            num = myMENU(num);     //主页面
            break;
        case 1:
            num = nameList(name);   //显示名单
            break;
        case 2:
            num = alterName(name,p);  //修改名字
            break;
        case 3:
            num = addName(name,p);     //增加名字
            break;
        case 4:
            num = roll_call(name);      //随机点名
            break;
        }
    }
    return 0;
}
/*****************  分割线  *****************/

int myMENU(int num)
{
    printf(YELLOW" ———————————————————————————————— \n");
    printf("|\t欢迎使用随机点名器!\t|\n");
    printf("|   1. 查看名单   2. 修改名单   |\n");
    printf("|   3. 增加名单   4. 进行点名   |\n");
    printf("|         5. 退出程序           |\n");
    printf(" ———————————————————————————————— \n"CLEAR);

    while (1)
    {
        printf("请输入序号以执行相应功能：");
        int sc = scanf("%d", &num);
    
        if (sc == 0 || num < 0 || num > 5)
            printf(RED"读取错误!\n"CLEAR);
        else
            break;
    }
    return num;
}
/*****************  分割线  *****************/

//功能：将所有名字打印出来
int nameList(char **name)
{
    
    printf("名单如下：\n");
    for (int i = 1; i <= (N+add); i++)
    {
        printf("%d:%s\n",i,name[i-1]);
    }
    return 0;

}
/*****************  分割线  *****************/

//功能：对名单特定名字进行字修改
int alterName(char **name,char p[100][50])
{
    int n;
    while (1)
    {
        printf("你要修改第几个名字(注：只能选择1-%d)：",N+add);
        scanf("%d",&n);
        if (n>=1&&n<=N+add)
            break;
        else
            printf(RED"输入错误，请重新输入1-%d其中任意一个数！！\n"CLEAR,N+add);
    }

    printf("原先第%d个名字:%s\n",n,name[n-1]);
    printf("现在的名字：");
    
    //清空原先名字所在内存的内容
    bzero(p[n-1],sizeof(*(p[n-1])));

    //获取新的名字
    scanf("%s",&p[n-1]);

    //对存放新名字的数组中的‘\n’换成‘\0’
    for (int i = 0; name[n-1][i] != '\0'; i++)
    {
        if (name[n-1][i] == '\n')
            name[n-1][i] = '\0';
    }
    
   
    return 0;

}
/*****************  分割线  *****************/

//功能：增加名单
int addName(char **name,char p[100][50])
{
    printf("请输入要增加的名字：");

    scanf("%s",&p[N+add]);

    name[N+add]=p[N+add];
    for (int j = 0; name[N+add][j] != '\0'; j++)
    {

        if (name[N+add][j] == '\n')
            name[N+add][j] = '\0';
    }
 
    add += 1;
    return 0;

}
/*****************  分割线  *****************/

//功能：随机显示名字
int roll_call(char **name)
{
    int num;
    while (1)
    {
        printf("请输入要点名的人数:");
        scanf("%d", &num);
        if (num > (N+add))
            printf(RED"温馨提示：超过所点人数，请重新输入！！\n"CLEAR);
        else
            break;
    }

    char *s[100] = {NULL};
    for (int i = 0; i < (N+add); i++)
        s[i]=name[i];

    int n = 0;
    //变量t用来记录点过名字的变量，如果随机到的名字先前没有被点过，则t就会减1
    int t = N+add;

    for (int i = 1; i <= num; i++)
    {
        n = random_name(s, t);

        printf("\r第%d位：%s\n",i,s[n]);

        //除掉已经被点过的名字
        for (int i = n; i < t; i++)
        {
            if (n == t - 1)
                break;
            s[i] = s[i + 1];
        }
        t--;
    }
    printf("\n");
    return 0;
}
/*****************  分割线  *****************/

//随机抽取名字所在的指针数组的下标值
int random_name(char **name, int t)
{
    int count = 0;
    int z = 0;
    while (count < 54)
    {
        //随机获取0-(t-1)的数
        z = rand() % t;

        //将光标移到行首，并覆盖掉原来的字符串
        printf("               \r");
        printf("%s", name[z]);

        count += 1;

        //通过count控制延时的大小，以达到点名速度由快到慢的效果
        if (count < 30)
            usleep(100000);
        else if (count >= 30 || count < 45)
            usleep(200000);
        else if (count >= 45 || count < 48)
            usleep(300000);
        else if (count >= 48)
            usleep(20000000);

        //及时地将打印的数据显示出来
        fflush(stdout);
    }
    return z;
}




