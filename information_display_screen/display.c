#include "bmp.h"

void display_xy(char const *jpgname,int x,int y)
{
    FILE *fp = fopen(jpgname,"r");
    if (fp==NULL)
    {
        perror("文件打开失败");
        exit(0);
    }
    
    struct bitmap_header head;
    struct bitmap_info info;

    bzero(&head,sizeof(head));
    bzero(&info,sizeof(info));

    fread(&head,sizeof(head),1,fp);
    fread(&info,sizeof(info),1,fp);

    
    int num = (4-((info.width*(info.bit_count/8))%4)) %4 ;
    
    int lineBytes = info.width*(info.bit_count/8) + num;

    int rgbSize = info.height*lineBytes;


    if (info.compression)
    {
        struct rgb_quad q;
        fread(&q, sizeof(q), 1, fp);
    }
    
    char *rgb = calloc(1,rgbSize);
    fread(rgb,rgbSize,1,fp);

    
    int lcd = open("/dev/fb0",O_RDWR);
    char *p = mmap(NULL,800*480*4,PROT_READ|PROT_WRITE,MAP_SHARED,lcd,0);
    
    // bzero(p,800*480*4);

    char *tmp = rgb+lineBytes*(info.height-1);

    int lcd_line = 800*4;

    int lcd_width = 800 - x;
    int lcd_height = 480 - y;

    int n = info.width/lcd_width;
    int m = info.height/lcd_height;
   

    int max = n > m ? n : m;

    if ((info.width) % lcd_width !=0 || (info.height) % lcd_height !=0 )
        max +=1;
  
    int X = (lcd_width-info.width/max)/2;
    int Y = (lcd_height-info.height/max)/2;

    char *s = p+(y*800+x)*4;
    
    for (int i = 0,k=0; i<info.height && k < lcd_height-Y;k++,i+=max)
    {
        for (int j = 0,t=0; j<info.width && t < lcd_width-X; t++, j+=max)
        {
            memcpy(s+4*t+lcd_line*k,tmp-lineBytes*i+3*j,3);
        }
        
    }

    munmap(p,800*480*4);
    close(lcd);
    free(rgb);
    return;
    
    
}

void display(char const *jpgname)
{

    FILE *fp = fopen(jpgname, "r");
    if (fp == NULL)
    {
        perror("文件打开失败");
        exit(0);
    }

    struct bitmap_header head;
    struct bitmap_info   info;

    bzero(&head, sizeof(head));
    bzero(&info, sizeof(info));

    fread(&head, sizeof(head), 1, fp);
    fread(&info, sizeof(info), 1, fp);

    int n = (4-((info.width*(info.bit_count/8))%4)) % 4;

    int lineBytes = info.width * (info.bit_count/8) + n;

    int rgbSize = info.height*lineBytes;

    if(info.compression)
    {
        struct rgb_quad q;
        fread(&q, sizeof(q), 1, fp);
    }

    char *rgb = calloc(1,  rgbSize);
    fread(rgb, rgbSize, 1, fp);

    int lcd = open("/dev/fb0", O_RDWR);
    char *p = mmap(NULL, 800*480*4, PROT_WRITE, MAP_SHARED, lcd, 0);
    bzero(p,800*480*4);

    char *tmp = rgb+lineBytes*(info.height-1);
     
    
    int lcd_line = 800*4;
    
    if (info.width == 800 && info.height == 480)
    {
        for (int i = 0; i < 480 ; i++)
        {
            
            for (int j = 0; j < 800;j++)
            {
                memcpy(p+4*j+lcd_line*i,tmp+3*j,3);
            }
            tmp -= lineBytes;
            
        }
        
    }

    if (info.width != 800 || info.height != 480)
    {
        int n = info.width/800;
        int m =info.height/480;

        int  max = n > m ? n : m;

        if (info.width%800 !=0 || info.height%480 !=0)
        {
            max +=1;
        }

        int x = (800-info.width/max)/2;
        int y = (480-info.height/max)/2; 
        char *s = p+(y*800+x)*4;
        for (int i = 0,k=0; i < info.height && k < 480-y ;k++, i+=max)
        {
            for (int j = 0,t=0; j < info.width && t < 800-x;t++, j+=max)
            {
                memcpy(s+4*t+lcd_line*k,tmp-lineBytes*i+3*j,3);
            }
            
        }
        
    }
    
    munmap(p,800*480*4);
    close(lcd);
    free(rgb);

    return;
}

