#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <math.h>
#include <dlfcn.h>

#include "font.h"
#include "bmp.h"

#define CCT (+8)

int  flag = 0;


void showbitmap(bitmap *bm, int x, int y)
{
    
    int lcd = open("/dev/fb0",O_RDWR);

    char *p = mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcd,0);
    
    //第一次调用此函数时，清屏
    if (flag==0)
    {
        bzero(p,800*480*4);
        flag=1;
    }
    int lcd_line = 800*4;

    int bm_line = bm->width*4;


    char *s=p+(y*800+x)*4;

    for (int i = 0; i < bm->height ; i++)
    {
        for (int j = 0; j < bm->width; j++)
        {
            memcpy(s+4*j+lcd_line*i,bm->map+4*j+bm_line*i,4);
        }
        
    }
    munmap(p,800*480*4);

    close(lcd);
    
}

/*****************  分割线  *****************/
void *show_time(void *arg)
{
    //初始化字库 
    font *f1 = fontLoad("simfang.ttf"); 
    //设置字体大小
    fontSetSize(f1, 30);

    //3.设置字体输出框的大小
    bitmap *bm1;
    bm1 = createBitmapWithInit(800, 40, 4, 0x00000000);

    char *wday[]={"星期日","星期一","星期二","星期三","星期四","星期五","星期六"};
    char buf[50];

    //显示时间
    while (1)
    {
        time_t t;

        struct tm *info;

        time(&t);

        info = gmtime(&t);

        bzero(buf,50);

        int k = info->tm_wday;

        snprintf(buf,50,"%d年%d月%d日  %s  %d时%d分%d秒",+1900+info->tm_year,info->tm_mon+1,info->tm_mday,wday[k],info->tm_hour,info->tm_min,info->tm_sec);
        
        fontPrint(f1, bm1, 0, 0, buf, 0XFAFF7400, 800);
        
        showbitmap(bm1, 0, 0);


        //清除输出框bm1中的内容
        bzero(bm1->map,bm1->height*bm1->width*bm1->byteperpixel);
        
    }
    
    
}

/*****************  分割线  *****************/

void roll_left(char **words)
{
    
    //初始化字库
    font *f = fontLoad("simfang.ttf");

    //设置字体输出框大小
    bitmap *bm;
    bm = createBitmapWithInit(800, 480, 4, 0x00000000);
    
    //字体在输出框起始位置的x轴偏移量
    int x_offset=800;

    int word_size;
    printf("设置字体大小:");
    scanf("%d",&word_size);
    int word_color;
    printf("设置字体颜色：");
    scanf("%x",&word_color);

    int y_loc=0;
    printf("滚动字体在显示屏的相对位置:");
    scanf("%d",&y_loc);


    int n = strlen(words[0]);    //计算字符串的字节数
    int num = word_size*(n/3);   //根据内容和字体大小计算出输出框的最大宽度
 
    while (1)
    {
        //设置输出框中字体大小
        fontSetSize(f, word_size);
        //把字体输出到输出框
        fontPrint(f, bm, x_offset, 0,words[0] , word_color, num+100);
        //将bm妥善地放置到LCD上显示出来
        showbitmap(bm,0, y_loc);
        //通过不断减小字体到输出框起始位置的x轴偏移量来实现字体左移的效果
        x_offset -=10;
        //当x_offset到达一定值的时候，将其置为800，实现字体重新开始左滚动效果
        if (x_offset==-num)
        {
            x_offset=800;
        }
        //清空输出框bm中的内容
        bzero(bm->map,bm->height*bm->width*bm->byteperpixel);
    }
    
   
}

/*****************  分割线  *****************/

void roll_down(char **words)
{
    //初始化字库
    font *f = fontLoad("simfang.ttf");
    //设置字体输出框大小
    bitmap *bm;
    bm = createBitmapWithInit(800, 480, 4, 0x00000000);

    //字体在输出框起始位置的y轴偏移量
    int y_offset=0;

    int word_size;
    printf("设置字体大小:");
    scanf("%d",&word_size);
    int word_color;
    printf("设置字体颜色：");
    scanf("%x",&word_color);

    int x_loc=0;
    printf("字体在输出框的相对位置：");
    scanf("%d",&x_loc);

    int n = strlen(words[0]);            //计算字符串的字节数
    int num = (word_size*(n/3))/800;    //根据内容和字体大小计算出输出框的最大宽度

    while (1)
    {
        
        fontSetSize(f, word_size);
        fontPrint(f, bm, x_loc, y_offset, words[0], word_color, 800);
        showbitmap(bm, 0, 0);

        y_offset +=2;
        if (y_offset==480)
        {
            y_offset=-num*word_size-90;
        }
        bzero(bm->map,bm->height*bm->width*bm->byteperpixel);
       
    
    }
}
/*****************  分割线  *****************/
void roll(char **words)
{
    font *f1 = fontLoad("simfang.ttf");
    bitmap *bm1;
    bm1 = createBitmapWithInit(800, 50, 4, 0x00000000);
    //字体在输出框起始位置的x轴偏移量
    int x_offset=800;

    int m = strlen(words[0]);   //计算字符串的字节数
    int num = 30*(m/3);         //根据内容和字体大小计算出输出框的最大宽度

    while (1)
    {
        fontSetSize(f1, 30);
        fontPrint(f1, bm1, x_offset, 0,words[0] , 0XFFF00000, num+100);
        showbitmap(bm1,0, 420);
        x_offset -=5;
        if (x_offset==-num)
        {
            x_offset=800;
        }
        
        bzero(bm1->map,bm1->height*bm1->width*bm1->byteperpixel);
    }
}
/*****************  分割线  *****************/
void show_word(char **words,int count)
{
    //初始化字库
    font *f = fontLoad("simfang.ttf");
  
    //设置字体输出框大小
    bitmap *bm;
   
    bm = createBitmapWithInit(800, 350, 4, 0x00000000);
   

    int n = 1; //用来区分不同字体串
    count -=1;
    
    while (1)
    {
        
        if (count > 0)
        {
            //设置字体大小
            int word_size;
            printf("设置字体%d大小：",n);
            scanf("%d",&word_size);
            fontSetSize(f, word_size);

            //设置字体在输出框bm的相对位置
            int x_loc=0,y_loc=0;
            printf("字体%d在输出框的相对位置x,y：",n);
            scanf("%d%d",&x_loc,&y_loc);

            //设置字体颜色
            int color;
            printf("请输入字体%d颜色16进制码：",n);
            scanf("%x",&color);
            fontPrint(f, bm, x_loc, y_loc, words[count], color, 800);
            showbitmap(bm, 0, 80);
            
            count--;
            n++;
        }
        else
            break;
        
    }

    roll(words);
    
    
}

/*****************  分割线  *****************/
//图文模式
int show_word_pic(char **words,char const *pic_name)
{
    font *f1 = fontLoad("simfang.ttf"); 
    fontSetSize(f1, 50);
    bitmap *bm1;
    bm1 = createBitmapWithInit(800, 80, 4, 0x00000000);
    fontPrint(f1, bm1, 0, 0, words[1], 0XFAFF7400, 800);
    showbitmap(bm1, 300, 10);

    int x,y;
    printf("请输入图片显示的起始位置x,y:");
    scanf("%d%d",&x,&y);
    display_xy(pic_name, x, y);
    roll(words);
    return 0;
}


/*****************  分割线  *****************/
int show_file_content(char **words,int count)
{
    for (int i = 0; i < count; i++)
        printf("%s",words[i]);

    printf("*****************  分割线  *****************\n");
    return 0;
}
/*****************  分割线  *****************/
int myUI()
{
    printf(" ———————————————————————————————— \n");
    printf("|\t欢迎使用小区信息展示屏!\t   |\n");
    printf("|   1.查看文件内容  2.图文模式     |\n");
    printf("|   3. 自定义模式   4. 左滚模式    |\n");
    printf("|   5. 下滚模式     6. 退出程序    |\n");
    printf(" ———————————————————————————————— \n");

    int mode;
     while (1)
    {
        printf("请输入相应模式的序号：");
        scanf("%d",&mode);
        if(mode <= 0 || mode > 6)
            printf("模式输入有误，请重新输入！\n");
        else
            break;  
    }
    return mode;
}
/*****************  分割线  *****************/
int main(int argc, char const *argv[])
{
    
    FILE *fp = fopen(argv[1],"r");

    if (fp==NULL)
        perror("打开文件失败");
    
    //用于存储文本每行的字体
    char p[50][300]={0};
    char *words[300]={NULL};

    int count = 0; //判断从文本中获取多少行字体输出到输出框中
    //获取文本中的字体
    for (int i = 0; ; i++)
    {
        words[i]=fgets(p[i],300,fp);
        if (feof(fp))
            break;
        count++;
        
    }
    fclose(fp);
    int mode = 0;

    pthread_t t;

    while (mode !=6 )
    {
        switch (mode)
        {
        case 0: mode = myUI();break;
        case 1: mode = show_file_content(words,count);break;
        case 2: mode = show_word_pic(words,argv[2]);break;
        case 3: pthread_create(&t, NULL, show_time, NULL);
                show_word(words, count);
                break;
        case 4:roll_left(words);break;
        
        case 5:roll_down(words);break;
        }
        
    }
    
    return 0;
}
