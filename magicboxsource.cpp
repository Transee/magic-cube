#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include <gl/glut.h>
#include <stdio.h>
FILE *fp;
//小方块的结构，包括小方块在x,y,z上的旋转角度，颜色数组下标，小方块的三维坐标。
struct rcube{
    int xr;
    int yr;
    int zr;
    int cl[6];
    GLfloat x;
    GLfloat y;
    GLfloat z;
};
struct rcube rc[3][3][3];
struct rcube *temp[3][3];
//颜色数组
GLfloat color[7][3]={{1.0,0.2,0.2},{0.2,1.0,0.2},{0.0,0.2,1.0},{1.0,1.0,0.2},{1.0,0.3,1.0},{1.0,1.0,1.0},{0.0,0.0,0.0}};
int tempc[3][3][6];
//有关旋转的一些变量
GLfloat xRot = 10.0f;
GLfloat yRot = 10.0f;
int rotateType=0;
int rotateOK=0;
int rotateRate=25;
int rotate=0;
int *T[100];
int RES[8][4][100];
int resOK=0;
int ri,rj,rk;
int step;
//判断面，楞和角颜色的一些变量
int *U,*L,*F,*R,*B,*D;
int *UF[2],*UR[2],*UB[2],*UL[2],*LF[2],*FR[2],*RB[2],*BL[2],*FD[2],*RD[2],*BD[2],*LD[2];
int *UFL[3],*URF[3],*UBR[3],*ULB[3],*LFD[3],*FRD[3],*RBD[3],*BLD[3];
///////////////////////////////////////////////////////////////////////////////
//画小方块
void drawcube(int cl[6])
{    
    glBegin(GL_QUADS);
//右面
    glColor3fv(color[cl[0]]);
    glVertex3f(0.14f,-0.14f,-0.14f);
    glVertex3f(0.14f,0.14f,-0.14f);
    glVertex3f(0.14f,0.14f,0.14f);
    glVertex3f(0.14f,-0.14f,0.14f);
//左面
    glColor3fv(color[cl[1]]);
    glVertex3f(-0.14f,-0.14f,0.14f);
    glVertex3f(-0.14f,0.14f,0.14f);
    glVertex3f(-0.14f,0.14f,-0.14f);
    glVertex3f(-0.14f,-0.14f,-0.14f);
//前面
    glColor3fv(color[cl[2]]);
    glVertex3f(-0.14f,0.14f,0.14f);
    glVertex3f(-0.14f,-0.14f,0.14f);
    glVertex3f(0.14f,-0.14f,0.14f);
    glVertex3f(0.14f,0.14f,0.14f);
//后面
    glColor3fv(color[cl[3]]);
    glVertex3f(-0.14f,0.14f,-0.14f);
    glVertex3f(0.14f,0.14f,-0.14f);
    glVertex3f(0.14f,-0.14f,-0.14f);
    glVertex3f(-0.14f,-0.14f,-0.14f);
//上面
    glColor3fv(color[cl[4]]);
    glVertex3f(-0.14f,0.14f,-0.14f);
    glVertex3f(-0.14f,0.14f,0.14f);    
    glVertex3f(0.14f,0.14f,0.14f);
    glVertex3f(0.14f,0.14f,-0.14f);
//下面
    glColor3fv(color[cl[5]]);
    glVertex3f(-0.14f,-0.14f,-0.14f);
    glVertex3f(0.14f,-0.14f,-0.14f);
    glVertex3f(0.14f,-0.14f,0.14f);
    glVertex3f(-0.14f,-0.14f,0.14f);
    glEnd();
    glFlush();
}    
//窗口刷新时被调用。
void RenderScene(void)
{
    int i,j,k;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix(); 
	glRotatef(xRot, 1.0f, 0.0f, 0.0f);
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			for(k=0;k<3;k++)
			{
             //魔方在这显示
				glPushMatrix();       
                glRotatef(rc[i][j][k].xr, 1.0f, 0.0f, 0.0f);
                glRotatef(rc[i][j][k].zr, 0.0f, 0.0f, 1.0f); 
                glRotatef(rc[i][j][k].yr, 0.0f, 1.0f, 0.0f);     
                glTranslatef(rc[i][j][k].x,rc[i][j][k].y,rc[i][j][k].z);                
				drawcube(rc[i][j][k].cl);            
				glPopMatrix();
			}
        }
    }            
	glPopMatrix();
	glutSwapBuffers();
}

void MI(void)
{
	U=&rc[1][2][1].cl[4];
	L=&rc[0][1][1].cl[1];
	F=&rc[1][1][2].cl[2];
	R=&rc[2][1][1].cl[0];
	B=&rc[1][1][0].cl[3];
	D=&rc[1][0][1].cl[5];
}
void LI(void)
{
	UF[0]=&rc[1][2][2].cl[4];UF[1]=&rc[1][2][2].cl[2];
	UR[0]=&rc[2][2][1].cl[4];UR[1]=&rc[2][2][1].cl[0];
	UB[0]=&rc[1][2][0].cl[4];UB[1]=&rc[1][2][0].cl[3];
	UL[0]=&rc[0][2][1].cl[4];UL[1]=&rc[0][2][1].cl[1];
	LF[0]=&rc[0][1][2].cl[1];LF[1]=&rc[0][1][2].cl[2];
	FR[0]=&rc[2][1][2].cl[2];FR[1]=&rc[2][1][2].cl[0];
	RB[0]=&rc[2][1][0].cl[0];RB[1]=&rc[2][1][0].cl[3];
	BL[0]=&rc[0][1][0].cl[3];BL[1]=&rc[0][1][0].cl[1];
	FD[0]=&rc[1][0][2].cl[2];FD[1]=&rc[1][0][2].cl[5];
	RD[0]=&rc[2][0][1].cl[0];RD[1]=&rc[2][0][1].cl[5];
	BD[0]=&rc[1][0][0].cl[3];BD[1]=&rc[1][0][0].cl[5];
	LD[0]=&rc[0][0][1].cl[1];LD[1]=&rc[0][0][1].cl[5];
}
void GI(void)
{
	UFL[0]=&rc[0][2][2].cl[4];UFL[1]=&rc[0][2][2].cl[2];UFL[2]=&rc[0][2][2].cl[1];
	URF[0]=&rc[2][2][2].cl[4];URF[1]=&rc[2][2][2].cl[0];URF[2]=&rc[2][2][2].cl[2];
	UBR[0]=&rc[2][2][0].cl[4];UBR[1]=&rc[2][2][0].cl[3];UBR[2]=&rc[2][2][0].cl[0];
	ULB[0]=&rc[0][2][0].cl[4];ULB[1]=&rc[0][2][0].cl[1];ULB[2]=&rc[0][2][0].cl[3];
	LFD[0]=&rc[0][0][2].cl[1];LFD[1]=&rc[0][0][2].cl[2];LFD[2]=&rc[0][0][2].cl[5];
	FRD[0]=&rc[2][0][2].cl[2];FRD[1]=&rc[2][0][2].cl[0];FRD[2]=&rc[2][0][2].cl[5];
	RBD[0]=&rc[2][0][0].cl[0];RBD[1]=&rc[2][0][0].cl[3];RBD[2]=&rc[2][0][0].cl[5];
	BLD[0]=&rc[0][0][0].cl[3];BLD[1]=&rc[0][0][0].cl[1];BLD[2]=&rc[0][0][0].cl[5];
}
//初始化。
void SetupRC()
{
    GLfloat x,y,z;
	int i,j,k,l;
	for(i=0,x=-0.3f;i<3;i++,x+=0.3f)
	{
		for(j=0,y=-0.3f;j<3;j++,y+=0.3f)
		{
			for(k=0,z=-0.3f;k<3;k++,z+=0.3f)
			{
				rc[i][j][k].x=x;rc[i][j][k].xr=0;
                rc[i][j][k].y=y;rc[i][j][k].yr=0;
                rc[i][j][k].z=z;rc[i][j][k].zr=0;                                      
                for(l=0;l<6;l++)
                {
					if((i==1&&j==2&&k==1&&l==4)||(i==0&&j==1&&k==1&&l==1)||(i==1&&j==1&&k==2&&l==2)||(i==2&&j==1&&k==1&&l==0)||
					   (i==1&&j==1&&k==0&&l==3)||(i==1&&j==0&&k==1&&l==5)||(i==1&&j==2&&k==2&&l==4)||(i==1&&j==2&&k==2&&l==2)||
					   (i==2&&j==2&&k==1&&l==4)||(i==2&&j==2&&k==1&&l==0)||(i==1&&j==2&&k==0&&l==4)||(i==1&&j==2&&k==0&&l==3)||
					   (i==0&&j==2&&k==1&&l==4)||(i==0&&j==2&&k==1&&l==1)||(i==0&&j==1&&k==2&&l==1)||(i==0&&j==1&&k==2&&l==2)||
					   (i==2&&j==1&&k==2&&l==2)||(i==2&&j==1&&k==2&&l==0)||(i==2&&j==1&&k==0&&l==0)||(i==2&&j==1&&k==0&&l==3)||
					   (i==0&&j==1&&k==0&&l==3)||(i==0&&j==1&&k==0&&l==1)||(i==1&&j==0&&k==2&&l==2)||(i==1&&j==0&&k==2&&l==5)||
					   (i==1&&j==0&&k==2&&l==2)||(i==1&&j==0&&k==2&&l==5)||(i==2&&j==0&&k==1&&l==0)||(i==2&&j==0&&k==1&&l==5)||
					   (i==1&&j==0&&k==0&&l==3)||(i==1&&j==0&&k==0&&l==5)||(i==0&&j==0&&k==1&&l==1)||(i==0&&j==0&&k==1&&l==5)||
					   (i==0&&j==2&&k==2&&l==4)||(i==0&&j==2&&k==2&&l==2)||(i==0&&j==2&&k==2&&l==1)||(i==2&&j==2&&k==2&&l==4)||
					   (i==2&&j==2&&k==2&&l==0)||(i==2&&j==2&&k==2&&l==2)||(i==2&&j==2&&k==0&&l==4)||(i==2&&j==2&&k==0&&l==3)||
					   (i==2&&j==2&&k==0&&l==0)||(i==0&&j==2&&k==0&&l==4)||(i==0&&j==2&&k==0&&l==1)||(i==0&&j==2&&k==0&&l==3)||
					   (i==0&&j==0&&k==2&&l==1)||(i==0&&j==0&&k==2&&l==2)||(i==0&&j==0&&k==2&&l==5)||(i==2&&j==0&&k==2&&l==2)||
					   (i==2&&j==0&&k==2&&l==0)||(i==2&&j==0&&k==2&&l==5)||(i==2&&j==0&&k==0&&l==0)||(i==2&&j==0&&k==0&&l==3)||
					   (i==2&&j==0&&k==0&&l==5)||(i==0&&j==0&&k==0&&l==3)||(i==0&&j==0&&k==0&&l==1)||(i==0&&j==0&&k==0&&l==5)
						)
						rc[i][j][k].cl[l]=l;   
					else
						rc[i][j][k].cl[l]=6;   
                }    
             }
         }
     }
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}
//选择转动的那一组小方块。
void elect(int type)
{ 
    int i,j; 
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            switch (type)
            {
                case 1:
                    temp[i][j]=&rc[0][i][j]; break;
                case 2:
                    temp[i][j]=&rc[1][i][j]; break;
                case 3:
                    temp[i][j]=&rc[2][i][j]; break;
                case 4:
                    temp[i][j]=&rc[j][0][i]; break;
                case 5:
                    temp[i][j]=&rc[j][1][i]; break;
                case 6:
                    temp[i][j]=&rc[j][2][i]; break;
                case 7:
                    temp[i][j]=&rc[i][j][0]; break;
                case 8:
                    temp[i][j]=&rc[i][j][1]; break;
                case 9:
                    temp[i][j]=&rc[i][j][2]; break;
				case 111:
                    temp[i][j]=&rc[0][i][j]; break;
                case 222:
                    temp[i][j]=&rc[1][i][j]; break;
                case 333:
                    temp[i][j]=&rc[2][i][j]; break;
                case 444:
                    temp[i][j]=&rc[j][0][i]; break;
                case 555:
                    temp[i][j]=&rc[j][1][i]; break;
                case 666:
                    temp[i][j]=&rc[j][2][i]; break;
                case 777:
                    temp[i][j]=&rc[i][j][0]; break;
                case 888:
                    temp[i][j]=&rc[i][j][1]; break;
                case 999:
                    temp[i][j]=&rc[i][j][2]; break;
				case 60:
                    ; break;
            }            
        }
    }     
} 
//判断是否胜利。
int win(void)
{
    int i,j,k,c,cl[6];
    for(k=0;k<6;k++)
    {
        cl[k]=rc[0][0][0].cl[k];
    }    
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            for(k=0;k<3;k++)
            {
                for(c=0;c<6;c++)
                {
                    if(rc[i][j][k].cl[c]!=cl[c])
                    return 0;
                }    
            }    
        }    
    }
    return 1;
}   
//转动小方块，实际上是交换颜色。
void shift(void)
{    
    int i,j,k,c;
    elect(rotateType);
	if(rotateType<10)
	{
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				(*temp[i][j]).xr=0;
				(*temp[i][j]).yr=0;
				(*temp[i][j]).zr=0;    
				if(rotateType>0&&rotateType<=3)
				{
					c=(*temp[i][j]).cl[2];
					(*temp[i][j]).cl[2]=(*temp[i][j]).cl[4];
					(*temp[i][j]).cl[4]=(*temp[i][j]).cl[3];
					(*temp[i][j]).cl[3]=(*temp[i][j]).cl[5];
					(*temp[i][j]).cl[5]=c;
				}
				if(rotateType>3&&rotateType<=6)
				{
					c=(*temp[i][j]).cl[0];
					(*temp[i][j]).cl[0]=(*temp[i][j]).cl[2];
					(*temp[i][j]).cl[2]=(*temp[i][j]).cl[1];
					(*temp[i][j]).cl[1]=(*temp[i][j]).cl[3];
					(*temp[i][j]).cl[3]=c;
				} 
				if(rotateType>6&&rotateType<=9)
				{
					c=(*temp[i][j]).cl[4];
					(*temp[i][j]).cl[4]=(*temp[i][j]).cl[0];
					(*temp[i][j]).cl[0]=(*temp[i][j]).cl[5];
					(*temp[i][j]).cl[5]=(*temp[i][j]).cl[1];
					(*temp[i][j]).cl[1]=c;
				}
			}
		}    
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				for(k=0;k<6;k++)
				{
					tempc[2-j][i][k]=(*temp[i][j]).cl[k];
				}    
			}
		}
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				for(k=0;k<6;k++)
				{
					(*temp[i][j]).cl[k]=tempc[i][j][k];
				}    
			}
		}
	}
	else if(rotateType!=60)
	{
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				(*temp[i][j]).xr=0;
				(*temp[i][j]).yr=0;
				(*temp[i][j]).zr=0;    
				for(k=0;k<6;k++)
				{
					tempc[i][j][k]=(*temp[2-j][i]).cl[k];
				} 
			}
		}    
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				for(k=0;k<6;k++)
				{
					(*temp[i][j]).cl[k]=tempc[i][j][k];
				}    
			}
		}
		for(i=0;i<3;i++)
		{
			for(j=0;j<3;j++)
			{
				if(rotateType==111||rotateType==333)
				{
					c=(*temp[i][j]).cl[2];
					(*temp[i][j]).cl[2]=(*temp[i][j]).cl[5];
					(*temp[i][j]).cl[5]=(*temp[i][j]).cl[3];
					(*temp[i][j]).cl[3]=(*temp[i][j]).cl[4];
					(*temp[i][j]).cl[4]=c;
				}
				if(rotateType==444||rotateType==666)
				{
					c=(*temp[i][j]).cl[0];
					(*temp[i][j]).cl[0]=(*temp[i][j]).cl[3];
					(*temp[i][j]).cl[3]=(*temp[i][j]).cl[1];
					(*temp[i][j]).cl[1]=(*temp[i][j]).cl[2];
					(*temp[i][j]).cl[2]=c;
				} 
				if(rotateType==777||rotateType==999)
				{
					c=(*temp[i][j]).cl[4];
					(*temp[i][j]).cl[4]=(*temp[i][j]).cl[1];
					(*temp[i][j]).cl[1]=(*temp[i][j]).cl[5];
					(*temp[i][j]).cl[5]=(*temp[i][j]).cl[0];
					(*temp[i][j]).cl[0]=c;
				}
			}
		}
	}
	else if(rotateType==60)
	{
		int n;
		for(n=4;n<=6;n++)
		{
			elect(n);
			for(i=0;i<3;i++)
			{
				for(j=0;j<3;j++)
				{
					(*temp[i][j]).xr=0;
					(*temp[i][j]).yr=0;
					(*temp[i][j]).zr=0;    
					for(k=0;k<6;k++)
					{
						tempc[2-j][i][k]=(*temp[i][j]).cl[k];
					} 
				}
			}    
			for(i=0;i<3;i++)
			{
				for(j=0;j<3;j++)
				{
					for(k=0;k<6;k++)
					{
						(*temp[i][j]).cl[k]=tempc[i][j][k];
					}    
				}
			}
			for(i=0;i<3;i++)
			{
				for(j=0;j<3;j++)
				{
					c=(*temp[i][j]).cl[0];
					(*temp[i][j]).cl[0]=(*temp[i][j]).cl[2];
					(*temp[i][j]).cl[2]=(*temp[i][j]).cl[1];
					(*temp[i][j]).cl[1]=(*temp[i][j]).cl[3];
					(*temp[i][j]).cl[3]=c;
				}
			}
		}
	}
//如果完成将背景色设成黑色，否则设成灰色。
    if(win()==1)
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    else
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f );
}    
//转动魔方，也就是改变小方块在x,y,z上的旋转角度。
void turn (void)
{
    int j,k;
    elect(rotateType);//选择旋转面
    for(j=0;j<3;j++)//相应面旋转10°
    {
        for(k=0;k<3;k++)
        {            
            if(rotateType>0&&rotateType<=3)
                (*temp[j][k]).xr+=10;
            if(rotateType>3&&rotateType<=6)
                (*temp[j][k]).yr+=10; 
            if(rotateType>6&&rotateType<=9)
                (*temp[j][k]).zr+=10;   
			if(rotateType==111||rotateType==333)
                (*temp[j][k]).xr-=10;
            if(rotateType==444||rotateType==666)
                (*temp[j][k]).yr-=10; 
            if(rotateType==777||rotateType==999)
                (*temp[j][k]).zr-=10;  
			if(rotateType==60)
			{
				int n;
				for(n=4;n<=6;n++)
				{
					elect(n);
					(*temp[j][k]).yr+=10;
				}
			}
		}
    }
}   
//定时器回调函数，当魔方转动时产生动画。 

int step1(void)
{
	if(*F==*UF[0]&&*D==*UF[1])
	{
	    *T[0]=666;*T[1]=1;*T[2]=9;*T[3]=111;*T[4]=60;fprintf(fp,"1.1-");
		//666 1 9 111 654
	}
	else if(*F==*UR[0]&&*D==*UR[1])
	{
		*T[0]=3;*T[1]=999;*T[2]=333;*T[3]=60;fprintf(fp,"1.2-");
		//3 999 333 654
	}
	else if(*F==*UB[0]&&*D==*UB[1])
	{
		*T[0]=666;*T[1]=3;*T[2]=999;*T[3]=333;*T[4]=60;fprintf(fp,"1.3-");
		//666 3 999 333 654
	}
	else if(*F==*UL[0]&&*D==*UL[1])
	{
		*T[0]=1;*T[1]=9;*T[2]=111;*T[3]=60;fprintf(fp,"1.4-");
		//1 9 111 654
	}
	else if(*F==*LF[0]&&*D==*LF[1])
	{
		*T[0]=111;*T[1]=6;*T[2]=1;*T[3]=9;*T[4]=9;*T[5]=60;fprintf(fp,"1.5-");
		//111 6 1 9 9 654
	}
	else if(*F==*FR[0]&&*D==*FR[1])
	{
		*T[0]=999;*T[1]=60;fprintf(fp,"1.6-");
		//999 654
	}
	else if(*F==*RB[0]&&*D==*RB[1])
	{
		*T[0]=3;*T[1]=666;*T[2]=333;*T[3]=9;*T[4]=9;*T[5]=60;fprintf(fp,"1.7-");
		//3 666 333 9 9 654
	}
	else if(*F==*BL[0]&&*D==*BL[1])
	{
		*T[0]=777;*T[1]=6;*T[2]=6;*T[3]=7;*T[4]=9;*T[5]=9;*T[6]=60;fprintf(fp,"1.8-");
		//777 6 6 7 9 9 654
	}
	else if(*F==*FD[0]&&*D==*FD[1])
	{
		*T[0]=60;fprintf(fp,"1.9-");
	}
	else if(*F==*RD[0]&&*D==*RD[1])
	{
		*T[0]=3;*T[1]=3;*T[2]=666;*T[3]=9;*T[4]=9;*T[5]=60;fprintf(fp,"1.10-");
		//3 3 666 9 9 654
	}
	else if(*F==*BD[0]&&*D==*BD[1])
	{
		*T[0]=7;*T[1]=7;*T[2]=6;*T[3]=6;*T[4]=9;*T[5]=9;*T[6]=60;fprintf(fp,"1.11-");
		//7 7 6 6 9 9 654
	}
	else if(*F==*LD[0]&&*D==*LD[1])
	{
		*T[0]=1;*T[1]=1;*T[2]=6;*T[3]=9;*T[4]=9;*T[5]=60;fprintf(fp,"1.12-");
		//1 1 6 9 9 654
	}
	else if(*F==*UF[1]&&*D==*UF[0])
	{
		*T[0]=9;*T[1]=9;*T[2]=60;fprintf(fp,"1.13-");
		// 9 9 654
	}
	else if(*F==*UR[1]&&*D==*UR[0])
	{
		*T[0]=666;*T[1]=9;*T[2]=9;*T[3]=60;fprintf(fp,"1.14-");
		//666 9 9 654
	}
	else if(*F==*UB[1]&&*D==*UB[0])
	{
		*T[0]=6;*T[1]=6;*T[2]=9;*T[3]=9;*T[4]=60;fprintf(fp,"1.15-");
		//6 6 9 9 654
	}
	else if(*F==*UL[1]&&*D==*UL[0])
	{
		*T[0]=6;*T[1]=9;*T[2]=9;*T[3]=60;fprintf(fp,"1.16-");
		//6 9 9 654
	}
	else if(*F==*LF[1]&&*D==*LF[0])
	{
		*T[0]=9;*T[1]=60;fprintf(fp,"1.17-");
		//9 654
	}
	else if(*F==*FR[1]&&*D==*FR[0])
	{
		*T[0]=333;*T[1]=666;*T[2]=3;*T[3]=9;*T[4]=9;*T[5]=60;fprintf(fp,"1.18-");
		//333 666 3 9 9 654
	}
	else if(*F==*RB[1]&&*D==*RB[0])
	{
		*T[0]=7;*T[1]=6;*T[2]=6;*T[3]=777;*T[4]=9;*T[5]=9;*T[6]=60;fprintf(fp,"1.19-");
		//7 6 6 777 9 9 654
	}
	else if(*F==*BL[1]&&*D==*BL[0])
	{
		*T[0]=1;*T[1]=6;*T[2]=111;*T[3]=9;*T[4]=9;*T[5]=60;fprintf(fp,"1.20-");
		//1 6 111 9 9 654
	}
	else if(*F==*FD[1]&&*D==*FD[0])
	{
		*T[0]=999;*T[1]=111;*T[2]=6;*T[3]=1;*T[4]=9;*T[5]=9;*T[6]=60;fprintf(fp,"1.21-");
		//999 111 6 1 9 9 654
	}
	else if(*F==*RD[1]&&*D==*RD[0])
	{
		*T[0]=333;*T[1]=999;*T[2]=60;fprintf(fp,"1.22-");
		//333 999 654
	}
	else if(*F==*BD[1]&&*D==*BD[0])
	{
		*T[0]=777;*T[1]=1;*T[2]=6;*T[3]=111;*T[4]=9;*T[5]=9;*T[6]=60;fprintf(fp,"1.23-");
		//777 1 6 111 9 9 654
	}
	else if(*F==*LD[1]&&*D==*LD[0])
	{
		*T[0]=111;*T[1]=9;*T[2]=60;fprintf(fp,"1.24-");
		//111 9 654
	}
	return 0;
}
int step2(void)
{
	if(*L==*UFL[0]&&*F==*UFL[1]&&*D==*UFL[2])
	{
		*T[0]=111;*T[1]=6;*T[2]=1;*T[3]=60;fprintf(fp,"2.1-");
		//L-U-L+ 111 6 1
	}
	else if(*L==*URF[0]&&*F==*URF[1]&&*D==*URF[2])
	{
		*T[0]=666;*T[1]=111;*T[2]=6;*T[3]=1;*T[4]=60;fprintf(fp,"2.2-");
		//U+L-U-L+ 666 111 6 1
	}
	else if(*L==*UBR[0]&&*F==*UBR[1]&&*D==*UBR[2])
	{
		*T[0]=6;*T[1]=6;*T[2]=111;*T[3]=6;*T[4]=1;*T[5]=60;fprintf(fp,"2.3-");
		//U+U+L-U-L+ 6 6 111 6 1
	}
	else if(*L==*ULB[0]&&*F==*ULB[1]&&*D==*ULB[2])
	{
		*T[0]=6;*T[1]=111;*T[2]=6;*T[3]=1;*T[4]=60;fprintf(fp,"2.4-");
		//U-L-U-L+ 6 111 6 1
	}
	else if(*L==*LFD[0]&&*F==*LFD[1]&&*D==*LFD[2])
	{
		*T[0]=60;fprintf(fp,"2.5-");
		//
	}
	else if(*L==*FRD[0]&&*F==*FRD[1]&&*D==*FRD[2])
	{
		*T[0]=333;*T[1]=666;*T[2]=3;*T[3]=111;*T[4]=6;*T[5]=1;*T[6]=60;fprintf(fp,"2.6-");
		//R+U+R-L-U-L+ 333 666 3 111 6 1
	}
	else if(*L==*RBD[0]&&*F==*RBD[1]&&*D==*RBD[2])
	{
		*T[0]=7;*T[1]=6;*T[2]=6;*T[3]=777;*T[4]=111;*T[5]=6;*T[6]=1;*T[7]=60;fprintf(fp,"2.7-");
		//B+U+U+B-L-U-L+ 7 6 6 777 111 6 1
	}
	else if(*L==*BLD[0]&&*F==*BLD[1]&&*D==*BLD[2])
	{
		*T[0]=777;*T[1]=6;*T[2]=7;*T[3]=999;*T[4]=666;*T[5]=9;*T[6]=60;fprintf(fp,"2.8-");
		//B-U-B+F+U+F- 777 6 7 999 666 9
	}
	else if(*L==*UFL[1]&&*F==*UFL[2]&&*D==*UFL[0])
	{
		*T[0]=999;*T[1]=6;*T[2]=9;*T[3]=6;*T[4]=6;*T[5]=999;*T[6]=666;*T[7]=9;*T[8]=60;fprintf(fp,"2.9-");
		//F+U-F-U+U+F+U+F- 999 6 9 6 6 999 666 9
	}
	else if(*L==*URF[1]&&*F==*URF[2]&&*D==*URF[0])
	{
		*T[0]=666;*T[1]=999;*T[2]=6;*T[3]=9;*T[4]=6;*T[5]=6;*T[6]=999;*T[7]=666;*T[8]=9;*T[9]=60;fprintf(fp,"2.10-");
		//U+F+U-F-U+U+F+U+F- 666 999 6 9 6 6 999 666 9
	}
	else if(*L==*UBR[1]&&*F==*UBR[2]&&*D==*UBR[0])
	{
		*T[0]=6;*T[1]=6;*T[2]=999;*T[3]=6;*T[4]=9;*T[5]=6;*T[6]=6;*T[7]=999;*T[8]=666;*T[9]=9;fprintf(fp,"2.11-");
		*T[10]=60;
		//U+U+F+U-F-U+U+F+U+F- 6 6 999 6 9 6 6 999 666 9
	}
	else if(*L==*ULB[1]&&*F==*ULB[2]&&*D==*ULB[0])
	{
		*T[0]=6;*T[1]=999;*T[2]=6;*T[3]=9;*T[4]=6;*T[5]=6;*T[6]=999;*T[7]=666;*T[8]=9;*T[9]=60;fprintf(fp,"2.12-");
		//U-F+U-F-U+U+F+U+F- 6 999 6 9 6 6 999 666 9
	}
	else if(*L==*LFD[1]&&*F==*LFD[2]&&*D==*LFD[0])
	{
		*T[0]=111;*T[1]=6;*T[2]=1;*T[3]=666;*T[4]=111;*T[5]=6;*T[6]=1;*T[7]=60;fprintf(fp,"1.13-");
		//L-U-L+U+L-U-L+ 111 6 1 666 111 6 1
	}
	else if(*L==*FRD[1]&&*F==*FRD[2]&&*D==*FRD[0])
	{
		*T[0]=9;*T[1]=6;*T[2]=999;*T[3]=6;*T[4]=6;*T[5]=111;*T[6]=6;*T[7]=1;*T[8]=60;fprintf(fp,"2.14-");
		//F-U-F+U+U+L-U-L+ 9 6 999 6 6 111 6 1
	}
	else if(*L==*RBD[1]&&*F==*RBD[2]&&*D==*RBD[0])
	{
		*T[0]=3;*T[1]=6;*T[2]=6;*T[3]=333;*T[4]=111;*T[5]=6;*T[6]=1;*T[7]=60;fprintf(fp,"2.15-");
		//R-U+U+R+L-U-L+ 3 6 6 333 111 6 1
	}
	else if(*L==*BLD[1]&&*F==*BLD[2]&&*D==*BLD[0])
	{
		*T[0]=777;*T[1]=6;*T[2]=7;*T[3]=111;*T[4]=6;*T[5]=1;*T[6]=60;fprintf(fp,"2.16-");
		//B-U-B++L-U-L+ 777 6 7 111 6 1
	}
	else if(*L==*UFL[2]&&*F==*UFL[0]&&*D==*UFL[1])
	{
		*T[0]=999;*T[1]=666;*T[2]=9;*T[3]=60;fprintf(fp,"2.17-");
		//F+U+F- 999 666 9
	}
	else if(*L==*URF[2]&&*F==*URF[0]&&*D==*URF[1])
	{
		*T[0]=666;*T[1]=999;*T[2]=666;*T[3]=9;*T[4]=60;fprintf(fp,"2.18-");
		//U+F+U+F- 666 999 666 9
	}
	else if(*L==*UBR[2]&&*F==*UBR[0]&&*D==*UBR[1])
	{
		*T[0]=6;*T[1]=6;*T[2]=999;*T[3]=666;*T[4]=9;*T[5]=60;fprintf(fp,"2.19-");
		//U+U+F+U+F- 6 6 999 666 9
	}
	else if(*L==*ULB[2]&&*F==*ULB[0]&&*D==*ULB[1])
	{
		*T[0]=6;*T[1]=999;*T[2]=666;*T[3]=9;*T[4]=60;fprintf(fp,"2.20-");
		//U-F+U+F- 6 999 666 9
	}
	else if(*L==*LFD[2]&&*F==*LFD[0]&&*D==*LFD[1])
	{
		*T[0]=999;*T[1]=666;*T[2]=9;*T[3]=6;*T[4]=999;*T[5]=666;*T[6]=9;*T[7]=60;fprintf(fp,"2.21-");
		//F+U+F-U-F+U+F- 999 666 9 6 999 666 9
	}
	else if(*L==*FRD[2]&&*F==*FRD[0]&&*D==*FRD[1])
	{
		*T[0]=333;*T[1]=666;*T[2]=3;*T[3]=999;*T[4]=666;*T[5]=9;*T[6]=60;fprintf(fp,"2.22-");
		//R+U+R-F+U+F- 333 666 3 999 666 9
	}
	else if(*L==*RBD[2]&&*F==*RBD[0]&&*D==*RBD[1])
	{
		*T[0]=7;*T[1]=6;*T[2]=6;*T[3]=777;*T[4]=999;*T[5]=666;*T[6]=9;*T[7]=60;fprintf(fp,"2.23-");
		//B+U+U+B-F+U+F- 7 6 6 777 999 666 9
	}
	else if(*L==*BLD[2]&&*F==*BLD[0]&&*D==*BLD[1])
	{
		*T[0]=1;*T[1]=666;*T[2]=111;*T[3]=6;*T[4]=6;*T[5]=999;*T[6]=666;*T[7]=9;*T[8]=60;fprintf(fp,"2.24-");
		//L+U+L-U-U-F+U+F- 1 666 111 6 6 999 666 9
	}
	return 0;
}
int step3(void)
{
	if(*L==*UF[0]&&*F==*UF[1])
	{
	    *T[0]=6;*T[1]=111;*T[2]=666;*T[3]=1;*T[4]=666;*T[5]=999;*T[6]=6;*T[7]=9;*T[8]=60;fprintf(fp,"3.1-");
		//U-L-U+L+U+F+U-F- 6 111 666 1 666 999 6 9
	}
	else if(*L==*UR[0]&&*F==*UR[1])
	{
	    *T[0]=111;*T[1]=666;*T[2]=1;*T[3]=666;*T[4]=999;*T[5]=6;*T[6]=9;*T[7]=60;fprintf(fp,"3.2-");
		//L-U+L+U+F+U-F- 111 666 1 666 999 6 9
	}
	else if(*L==*UB[0]&&*F==*UB[1])
	{
	    *T[0]=666;*T[1]=111;*T[2]=666;*T[3]=1;*T[4]=666;*T[5]=999;*T[6]=6;*T[7]=9;*T[8]=60;fprintf(fp,"3.3-");
		//U+L-U+L+U+F+U-F- 666 111 666 1 666 999 6 9
	}
	else if(*L==*UL[0]&&*F==*UL[1])
	{
	    *T[0]=6;*T[1]=6;*T[2]=111;*T[3]=666;*T[4]=1;*T[5]=666;*T[6]=999;*T[7]=6;*T[8]=9;*T[9]=60;fprintf(fp,"3.4-");
		//U+U+L-U+L+U+F+U-F- 6 6 111 666 1 666 999 6 9
	}
	else if(*L==*LF[0]&&*F==*LF[1])
	{
		*T[0]=60;fprintf(fp,"3.5-");
		//
	}
	else if(*L==*FR[0]&&*F==*FR[1])
	{
	    *T[0]=666;*T[1]=333;*T[2]=6;*T[3]=3;*T[4]=6;*T[5]=9;*T[6]=666;*T[7]=999;*T[8]=666;*T[9]=111;
		*T[10]=666;*T[11]=1;*T[12]=666;*T[13]=999;*T[14]=6;*T[15]=9;*T[16]=60;fprintf(fp,"3.6-");//16
		//U+R+U-R-U-F-U+F+U+L-U+L+U+F+U-F- 666 333 6 3 6 9 666 999 666 111 666 1 666 999 6 9
	}
	else if(*L==*RB[0]&&*F==*RB[1])
	{
	    *T[0]=666;*T[1]=7;*T[2]=6;*T[3]=777;*T[4]=6;*T[5]=3;*T[6]=666;*T[7]=333;*T[8]=6;*T[9]=6;
		*T[10]=111;*T[11]=666;*T[12]=1;*T[13]=666;*T[14]=999;*T[15]=6;*T[16]=9;*T[17]=60;fprintf(fp,"3.7-");
		//U+B+U-B-U-R-U+R+U+U+L-U+L+U+F+U-F- 666 7 6 777 6 3 666 333 6 6 111 666 1 666 999 6 9
	}
	else if(*L==*BL[0]&&*F==*BL[1])//
	{
	    *T[0]=666;*T[1]=1;*T[2]=6;*T[3]=111;*T[4]=6;*T[5]=777;*T[6]=666;*T[7]=7;*T[8]=6;*T[9]=111;
		*T[10]=666;*T[11]=1;*T[12]=666;*T[13]=999;*T[14]=6;*T[15]=9;*T[16]=60;fprintf(fp,"3.8-");
		//U+L+U-L-U-B-U+B+U-L-U+L+U+F+U-F- 666 1 6 111 6 777 666 7 6 111 666 1 666 999 6 9
	} 
	else if(*L==*UF[1]&&*F==*UF[0])
	{
		*T[0]=6;*T[1]=6;*T[2]=999;*T[3]=6;*T[4]=9;*T[5]=6;*T[6]=111;*T[7]=666;*T[8]=1;*T[9]=60;fprintf(fp,"3.9-");
		//U+U+F+U-F-U-L-U+L+ 6 6 999 6 9 6 111 666 1
	}
	else if(*L==*UR[1]&&*F==*UR[0])
	{
	    *T[0]=6;*T[1]=999;*T[2]=6;*T[3]=9;*T[4]=6;*T[5]=111;*T[6]=666;*T[7]=1;*T[8]=60;fprintf(fp,"3.10-");
		//U-F+U-F-U-L-U+L+ 6 999 6 9 6 111 666 1
	}
	else if(*L==*UB[1]&&*F==*UB[0])
	{
	    *T[0]=999;*T[1]=6;*T[2]=9;*T[3]=6;*T[4]=111;*T[5]=666;*T[6]=1;*T[7]=60;fprintf(fp,"3.11-");
		//F+U-F-U-L-U+L+ 999 6 9 6 111 666 1
	}
	else if(*L==*UL[1]&&*F==*UL[0])
	{
	    *T[0]=666;*T[1]=999;*T[2]=6;*T[3]=9;*T[4]=6;*T[5]=111;*T[6]=666;*T[7]=1;*T[8]=60;fprintf(fp,"3.12-");
		//U+F+U-F-U-L-U+L+ 666 999 6 9 6 111 666 1
	}
	else if(*L==*LF[1]&&*F==*LF[0])
	{
	    *T[0]=666;*T[1]=999;*T[2]=6;*T[3]=9;*T[4]=6;*T[5]=111;*T[6]=666;*T[7]=1;*T[8]=6;*T[9]=999;
		*T[10]=6;*T[11]=9;*T[12]=6;*T[13]=111;*T[14]=666;*T[15]=1;*T[16]=60;fprintf(fp,"3.13-");
		//U+F+U-F-U-L-U+L+U-F+U-F-U-L-U+L+ 666 999 6 9 6 111 666 1 6 999 6 9 6 111 666 1
	}
	else if(*L==*FR[1]&&*F==*FR[0])
	{
	    *T[0]=666;*T[1]=333;*T[2]=6;*T[3]=3;*T[4]=6;*T[5]=9;*T[6]=666;*T[7]=9;*T[8]=9;*T[9]=6;
		*T[10]=9;*T[11]=6;*T[12]=111;*T[13]=666;*T[14]=1;*T[15]=60;fprintf(fp,"3.14-");
		//U+R+U-R-U-F-U+F+F+U-F-U-L-U+L+ 666 333 6 3 6 9 666 9 9 6 9 6 111 666 1
	}
	else if(*L==*RB[1]&&*F==*RB[0])
	{
	    *T[0]=666;*T[1]=7;*T[2]=6;*T[3]=777;*T[4]=6;*T[5]=3;*T[6]=666;*T[7]=333;*T[8]=666;*T[9]=999;
		*T[10]=6;*T[11]=9;*T[12]=6;*T[13]=111;*T[14]=666;*T[15]=1;*T[16]=60;fprintf(fp,"3.15-");
		//U+B+U-B-U-R-U+R+U+F+U-F-U-L-U+L+ 666 7 6 777 6 3 666 333 666 999 6 9 6 111 666 1
	}
	else if(*L==*BL[1]&&*F==*BL[0])
	{
	    *T[0]=666;*T[1]=1;*T[2]=6;*T[3]=111;*T[4]=6;*T[5]=777;*T[6]=666;*T[7]=7;*T[8]=6;*T[9]=6;
		*T[10]=999;*T[11]=6;*T[12]=9;*T[13]=6;*T[14]=111;*T[15]=666;*T[16]=1;*T[17]=60;fprintf(fp,"3.16-");
		//U+L+U-L-U-B-U+B+U+U+F+U-F-U-L-U+L+ 666 1 6 111 6 777 666 7 6 6 999 6 9 6 111 666 1
	};
	return 0;
}

int step4(void)
{
	if(*U==*UF[0]&&*U==*UR[0]&&*U==*UB[0]&&*U==*UL[0])
	{
		;fprintf(fp,"4.0-");//
	}
	else if((*U!=*UF[0]&&*U!=*UR[0]&&*U==*UB[0]&&*U==*UL[0])||
			(*U==*UF[0]&&*U==*UR[0]&&*U!=*UB[0]&&*U!=*UL[0])||
			(*U!=*UF[0]&&*U==*UR[0]&&*U!=*UB[0]&&*U==*UL[0]))
	{
	    *T[0]=999;*T[1]=333;*T[2]=666;*T[3]=3;*T[4]=6;*T[5]=9;fprintf(fp,"4.1-");
		//F+R+U+R-U-F- 999 333 666 3 6 9
	}
	else if((*U==*UF[0]&&*U!=*UR[0]&&*U!=*UB[0]&&*U==*UL[0])||
			(*U!=*UF[0]&&*U==*UR[0]&&*U==*UB[0]&&*U!=*UL[0])||
			(*U==*UF[0]&&*U!=*UR[0]&&*U==*UB[0]&&*U!=*UL[0]))
	{
	    *T[0]=666;*T[1]=999;*T[2]=333;*T[3]=666;*T[4]=3;*T[5]=6;*T[6]=9;fprintf(fp,"4.2-");
		//U+F+R+U+R-U-F- 666 999 333 666 3 6 9
	}
	else
	{
		*T[0]=999;*T[1]=333;*T[2]=666;*T[3]=3;*T[4]=6;*T[5]=9;fprintf(fp,"4.3-");
		//F+R+U+R-U-F- 999 333 666 3 6 9
	}
	return 0;
}

int step5(void)
{
	if((*U==*ULB[2]&&*U==*UBR[2]&&*U==*URF[0]&&*U==*UFL[2])||
	   (*U==*ULB[1]&&*U==*UBR[2]&&*U==*URF[2]&&*U==*UFL[1])||
	   (*U==*ULB[2]&&*U==*UBR[1]&&*U==*URF[2]&&*U==*UFL[1])||
	   (*U==*ULB[2]&&*U==*UBR[0]&&*U==*URF[1]&&*U==*UFL[0]))
	{
		*T[0]=3;*T[1]=6;*T[2]=6;*T[3]=333;*T[4]=666;*T[5]=3;*T[6]=666;*T[7]=333;fprintf(fp,"5.1-");
		//5.1 R-U+U+R+U+R-U+R+ 3 6 6 333 666 3 666 333
	}
	if((*U==*ULB[1]&&*U==*UBR[1]&&*U==*URF[0]&&*U==*UFL[1])||
	   (*U==*ULB[0]&&*U==*UBR[1]&&*U==*URF[2]&&*U==*UFL[0])||
	   (*U==*ULB[0]&&*U==*UBR[0]&&*U==*URF[2]&&*U==*UFL[1]))
	{
		*T[0]=6;*T[1]=333;*T[2]=6;*T[3]=6;*T[4]=3;*T[5]=6;*T[6]=333;*T[7]=6;*T[8]=3;fprintf(fp,"5.2-");
		//5.2 U-R+U-U-R-U-R+U-R- 6 333 6 6 3 6 333 6 3
	}
	//
	if((*U==*ULB[2]&&*U==*UBR[2]&&*U==*URF[2]&&*U==*UFL[0])||
	   (*U==*ULB[1]&&*U==*UBR[1]&&*U==*URF[2]&&*U==*UFL[2])||
	   (*U==*ULB[1]&&*U==*UBR[2]&&*U==*URF[1]&&*U==*UFL[2])||
	   (*U==*ULB[0]&&*U==*UBR[2]&&*U==*URF[0]&&*U==*UFL[1]))
	{
		*T[0]=6;*T[1]=3;*T[2]=6;*T[3]=6;*T[4]=333;*T[5]=666;*T[6]=3;*T[7]=666;*T[8]=333;fprintf(fp,"5.3-");
		//5.1 U-R-U+U+R+U+R-U+R+ 6 3 6 6 333 666 3 666 333
	}
	if((*U==*ULB[1]&&*U==*UBR[1]&&*U==*URF[1]&&*U==*UFL[0])||
	   (*U==*ULB[0]&&*U==*UBR[0]&&*U==*URF[1]&&*U==*UFL[2])||
	   (*U==*ULB[1]&&*U==*UBR[0]&&*U==*URF[0]&&*U==*UFL[2]))
	{
		*T[0]=6;*T[1]=6;*T[2]=333;*T[3]=6;*T[4]=6;*T[5]=3;*T[6]=6;*T[7]=333;*T[8]=6;*T[9]=3;fprintf(fp,"5.4-");
		//5.2 U-U-R+U-U-R-U-R+U-R- 6 6 333 6 6 3 6 333 6 3
	}
	//u+
	if((*U==*ULB[0]&&*U==*UBR[2]&&*U==*URF[2]&&*U==*UFL[2])||
	   (*U==*ULB[2]&&*U==*UBR[1]&&*U==*URF[1]&&*U==*UFL[2])||
	   (*U==*ULB[2]&&*U==*UBR[1]&&*U==*URF[2]&&*U==*UFL[1])||
	   (*U==*ULB[1]&&*U==*UBR[0]&&*U==*URF[2]&&*U==*UFL[0]))
	{
		*T[0]=6;*T[1]=6;*T[2]=3;*T[3]=6;*T[4]=6;*T[5]=333;*T[6]=666;*T[7]=3;*T[8]=666;*T[9]=333;fprintf(fp,"5.5-");
		//5.1 U-U-R-U+U+R+U+R-U+R+ 6 6 3 6 6 333 666 3 666 333
	}
	if((*U==*ULB[0]&&*U==*UBR[1]&&*U==*URF[1]&&*U==*UFL[1])||
	   (*U==*ULB[2]&&*U==*UBR[0]&&*U==*URF[0]&&*U==*UFL[1])||
	   (*U==*ULB[2]&&*U==*UBR[1]&&*U==*URF[0]&&*U==*UFL[0]))
	{
		*T[0]=666;*T[1]=333;*T[2]=6;*T[3]=6;*T[4]=3;*T[5]=6;*T[6]=333;*T[7]=6;*T[8]=3;fprintf(fp,"5.6-");
		//5.2 U-U-U-R+U-U-R-U-R+U-R- 666 333 6 6 3 6 333 6 3
	}
	//u++
	if((*U==*ULB[2]&&*U==*UBR[0]&&*U==*URF[2]&&*U==*UFL[2])||
	   (*U==*ULB[2]&&*U==*UBR[2]&&*U==*URF[1]&&*U==*UFL[1])||
	   (*U==*ULB[1]&&*U==*UBR[2]&&*U==*URF[1]&&*U==*UFL[2])||
	   (*U==*ULB[0]&&*U==*UBR[1]&&*U==*URF[0]&&*U==*UFL[2]))
	{
		*T[0]=666;*T[1]=3;*T[2]=6;*T[3]=6;*T[4]=333;*T[5]=666;*T[6]=3;*T[7]=666;*T[8]=333;fprintf(fp,"5.7-");
		//5.1 U-U-U-R-U+U+R+U+R-U+R+ 666 3 6 6 333 666 3 666 333
	}
	if((*U==*ULB[1]&&*U==*UBR[0]&&*U==*URF[1]&&*U==*UFL[1])||
	   (*U==*ULB[1]&&*U==*UBR[2]&&*U==*URF[0]&&*U==*UFL[0])||
	   (*U==*ULB[0]&&*U==*UBR[2]&&*U==*URF[1]&&*U==*UFL[0]))
	{
		*T[0]=333;*T[1]=6;*T[2]=6;*T[3]=3;*T[4]=6;*T[5]=333;*T[6]=6;*T[7]=3;fprintf(fp,"5.8-");
		//5.2 R+U-U-R-U-R+U-R- 333 6 6 3 6 333 6 3
	}
	//u+++
	return 0;
}

int step6(void)
{
	int i=0;
	if(*UFL[1]==*URF[2])
		i++;
	if(*URF[1]==*UBR[2])
		i++;
	if(*UBR[1]==*ULB[2])
		i++;
	if(*ULB[1]==*UFL[2])
		i++;
	if(i==0)
	{
		*T[0]=333;*T[1]=777;*T[2]=333;*T[3]=9;*T[4]=9;*T[5]=3;*T[6]=7;*T[7]=333;*T[8]=9;*T[9]=9;
		*T[10]=3;*T[11]=3;fprintf(fp,"6.1-");
		//6 R+B-R+F+F+R-B+R+F+F+R+R+ 333 777 333 9 9 3 7 333 9 9 3 3
	}
	if(i==1)
	{
		if(*UFL[1]==*URF[2])
		{
			*T[0]=333;*T[1]=777;*T[2]=333;*T[3]=9;*T[4]=9;*T[5]=3;*T[6]=7;*T[7]=333;*T[8]=9;*T[9]=9;
			*T[10]=3;*T[11]=3;fprintf(fp,"6.2-");
			//6 R+B-R+F+F+R-B+R+F+F+R+R+ 333 777 333 9 9 3 7 333 9 9 3 3
		}
		else if(*URF[1]==*UBR[2])
		{
			*T[0]=666;*T[1]=333;*T[2]=777;*T[3]=333;*T[4]=9;*T[5]=9;*T[6]=3;*T[7]=7;*T[8]=333;*T[9]=9;
			*T[10]=9;*T[11]=3;*T[12]=3;fprintf(fp,"6.3-");
			//U+6 U+R+B-R+F+F+R-B+R+F+F+R+R+ 666 333 777 333 9 9 3 7 333 9 9 3 3
		}
		else if(*UBR[1]==*ULB[2])
		{
			*T[0]=6;*T[1]=6;*T[2]=333;*T[3]=777;*T[4]=333;*T[5]=9;*T[6]=9;*T[7]=3;*T[8]=7;*T[9]=333;
			*T[10]=9;*T[11]=9;*T[12]=3;*T[13]=3;fprintf(fp,"6.4-");
			//U++6 U+U+R+B-R+F+F+R-B+R+F+F+R+R+ 6 6 333 777 333 9 9 3 7 333 9 9 3 3
		}
		else if(*ULB[1]==*UFL[2])
		{
			*T[0]=6;*T[1]=333;*T[2]=777;*T[3]=333;*T[4]=9;*T[5]=9;*T[6]=3;*T[7]=7;*T[8]=333;*T[9]=9;
			*T[10]=9;*T[11]=3;*T[12]=3;fprintf(fp,"6.5-");
			//U+++6 U+U+U+R+B-R+F+F+R-B+R+F+F+R+R+ 6 333 777 333 9 9 3 7 333 9 9 3 3
		}
	}
	return 0;
}

int step7(void)
{
	int i=0,j=0;
	if(*UFL[1]==*URF[2]&&*UFL[1]==*UF[1])
	{		
		j++;
	}
	if(*URF[1]==*UBR[2]&&*URF[1]==*UR[1])
	{
		j++;	
	}
	if(*UBR[1]==*ULB[2]&&*UBR[1]==*UB[1])
	{
		j++;
	}
	if(*ULB[1]==*UFL[2]&&*ULB[1]==*UL[1])
	{
		j++;
	}
	if(j!=4)
	{
		if(*UFL[1]==*URF[2]&&*UFL[1]!=*UF[1])
		{		
			i++;
		}
		if(*URF[1]==*UBR[2]&&*URF[1]!=*UR[1])
		{
			i++;	
		}
		if(*UBR[1]==*ULB[2]&&*UBR[1]!=*UB[1])
		{
			i++;
		}
		if(*ULB[1]==*UFL[2]&&*ULB[1]!=*UL[1])
		{
			i++;
		}
		if(i==3)
		{
			if(*UFL[1]==*URF[2]&&*UFL[1]==*UF[1])
			{
				*T[0]=6;*T[1]=6;*T[2]=333;*T[3]=6;*T[4]=333;*T[5]=666;*T[6]=333;*T[7]=666;*T[8]=333;*T[9]=6;
				*T[10]=3;*T[11]=6;*T[12]=3;*T[13]=3;fprintf(fp,"7.1-");
				//  R+U-R+U+R+U+R+U-R-U-R-R- 6 6 333 6 333 666 333 666 333 6 3 6 3 3
			}
			else if(*URF[1]==*UBR[2]&&*URF[1]==*UR[1])
			{
				*T[0]=6;*T[1]=333;*T[2]=6;*T[3]=333;*T[4]=666;*T[5]=333;*T[6]=666;*T[7]=333;*T[8]=6;*T[9]=3;
				*T[10]=6;*T[11]=3;*T[12]=3;fprintf(fp,"7.2-");
				//  R+U-R+U+R+U+R+U-R-U-R-R- 6 333 6 333 666 333 666 333 6 3 6 3 3
			}
			else if(*UBR[1]==*ULB[2]&&*UBR[1]==*UB[1])
			{
				*T[0]=333;*T[1]=6;*T[2]=333;*T[3]=666;*T[4]=333;*T[5]=666;*T[6]=333;*T[7]=6;*T[8]=3;*T[9]=6;
				*T[10]=3;*T[11]=3;fprintf(fp,"7.3-");
				// R+U-R+U+R+U+R+U-R-U-R-R- 333 6 333 666 333 666 333 6 3 6 3 3
			}
			else if(*ULB[1]==*UFL[2]&&*ULB[1]==*UL[1])
			{
				*T[0]=666;*T[1]=333;*T[2]=6;*T[3]=333;*T[4]=666;*T[5]=333;*T[6]=666;*T[7]=333;*T[8]=6;*T[9]=3;
				*T[10]=6;*T[11]=3;*T[12]=3;fprintf(fp,"7.4-");
				// R+U-R+U+R+U+R+U-R-U-R-R- 666 333 6 333 666 333 666 333 6 3 6 3 3
			}
		}
		else if(i==4)
		{
			*T[0]=333;*T[1]=6;*T[2]=333;*T[3]=666;*T[4]=333;*T[5]=666;*T[6]=333;*T[7]=6;*T[8]=3;*T[9]=6;
			*T[10]=3;*T[11]=3;fprintf(fp,"7.5-");
			//7 R+U-R+U+R+U+R+U-R-U-R-R- 333 6 333 666 333 666 333 6 3 6 3 3
		}
	}
	return 0;
}
int step8(void)
{
	fclose(fp);
	if(*UF[1]!=*F)
		*T[0]=6;
	else
		*T[0]=0;
	return 0;
}

//接收键盘输入。
void guizero(void)
{
	int i,j,k;
	for(i=0;i<7;i++)
	{
		for(j=0;j<4;j++)
		{
			for(k=0;k<100;k++)
			{
				RES[i][j][k]=0;
			}
		}
	}
}
void TimerFunc(int value) //相当于定时中断函数
{
	int h;
	if(resOK==0)
	{
		rotate+=10;        
		turn();//旋转魔方
		glutPostRedisplay();
		if(rotate==90)
		{
			shift();//
			rotate=0;
			rotateOK=0;        
			rotateType=0;       
			return;
		}       
		glutTimerFunc(rotateRate,TimerFunc,1);
	}
	else
	{
		if(rotate==0&&rk==0)
		{
			for(h=0;h<100;h++)
			{
				T[h]=&RES[ri][rj][h];
			}		
			switch(ri)
			{
				case 0:
					step1();break;
				case 1:
					step2();break;
				case 2:
					step3();break;
				case 3:
					step4();break;
				case 4:
					step5();break;
				case 5:
					step6();break;
				case 6:
					step7();break;
				case 7:
					step8();break;
			}
		}
		rotate+=10;   
		rotateType=RES[ri][rj][rk];
		if(rotateType!=0)
		turn();//旋转魔方
		if(rotate==90||rotateType==0)
		{
			if(rotateType!=0)
			shift();//
			rotate=0;
//			rotateOK=0;   
			if(RES[ri][rj][rk+1]!=0)
				rk++;
			else if(rj<3)
			{
				rk=0;
				rj++;
			}
			else
			{
				rk=0;
				rj=0;
				ri++;
			}
			rotateType=0;   
		}       
		glutPostRedisplay();
		if(ri>7)
		{
			ri=0;
			rotate=0;
			guizero();
			rotateOK=0;   
			resOK=0;
			rotateType=0;   
			return;
		}
		glutTimerFunc(rotateRate,TimerFunc,1);
	}
}
void SpecialKeys(int key, int x, int y)
{
    int temp=0;
    switch (key)
    {     
        case GLUT_KEY_F1:
            temp=1; break;
        case GLUT_KEY_F2:
            temp=2; break;
        case GLUT_KEY_F3:
            temp=3; break;                       
        case GLUT_KEY_F4:
            temp=4; break;
        case GLUT_KEY_F5:
            temp=5; break;
        case GLUT_KEY_F6:
            temp=6; break;
        case GLUT_KEY_F7:
            temp=7; break;
        case GLUT_KEY_F8:
            temp=8; break;
        case GLUT_KEY_F9:
            temp=9; break;//选择F1-F9建，给temp赋值  
        case GLUT_KEY_F10:
            temp=10;break;
		case GLUT_KEY_F11: 
			SetupRC();break;
        case GLUT_KEY_UP:
            xRot -= 5.0f; break;
		case GLUT_KEY_DOWN:
            xRot += 5.0f; break;
		case GLUT_KEY_LEFT:
            yRot -= 5.0f; break;
		case GLUT_KEY_RIGHT://选择方向键，改变坐标
            yRot += 5.0f; break;
    }
    if ( rotateOK==0 && temp!=0&&temp!=10)
    {
        rotateType=temp;//旋转方式
        rotateOK=1;//开启旋转
        glutTimerFunc(rotateRate/*毫秒数*/,TimerFunc/*延时函数*/,1/*区别值*/);//定时器
    }
	if ( rotateOK==0&&temp==10/*&&resOK!=0*/)
	{
		step=0;
		ri=0,rj=0,rk=0;
		guizero();
		resOK=1;
		MI();
		LI();
		GI();
        rotateOK=1;//开启旋转
		fp=fopen("step.txt","w+");
		fp=fopen("step.txt","a");
        glutTimerFunc(rotateRate/*毫秒数*/,TimerFunc/*延时函数*/,1/*区别值*/);//定时器
	}
	else temp=0;
    if(xRot > 356.0f) xRot = 0.0f;
	if(xRot < -1.0f) xRot = 355.0f;
	if(yRot > 356.0f) yRot = 0.0f;
	if(yRot < -1.0f) yRot = 355.0f;
	glutPostRedisplay();        
}
//当窗口改变尺寸时被调用。
void ChangeSize(int w, int h)
{
	GLfloat nRange = 1.9f;
	if(h == 0)//防止高度为0
	h = 1;
    glViewport(0, 0, w, h);// 设置视口大小为增个窗口大小
	glMatrixMode(GL_PROJECTION);//设置当前矩阵为投影矩阵
	glLoadIdentity();// 单位化投影矩阵
    if (w <= h) 
	glOrtho (-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
    else 
	glOrtho (-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);
	glMatrixMode(GL_MODELVIEW);//设置模型视图矩阵
	glLoadIdentity();// 单位化模型视图矩阵
}
//主函数。
int main(int argc, char* argv[])
{
	//ShellExecute(NULL,"open","help.txt",NULL,NULL,SW_SHOWNORMAL);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("GLUT Shapes"); 
	SetupRC();
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);
	glutMainLoop();
	return 0;
}
