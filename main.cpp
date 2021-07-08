
#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <tchar.h>
#include <windows.h>
#include <math.h>
#include <cmath>
#include<bits/stdc++.h>
#include <iostream>

using namespace std;

struct types
{
    string Name;
    string ShapeAlgo;
    string ShapeColor;
    int xc=-1;
    int xr=-1;
    int yc=-1;
    int yr=-1;
    int first=-1;
    int second=-1;
    int third=-1;
    int forth=-1;

};

vector<types>Shapes;

void Save(const vector<types>&Shape){
    ofstream file("shapes.txt");
    for (auto const &j : Shape){
        file<< j.Name<<" "<<j.ShapeAlgo<<" "<<j.ShapeColor<<" "<<j.xc<<" "<<j.yc<<" "<<j.xr<<" "<<j.yr<<" "<<j.first<<" "<<j.second<<" "<<j.third<<" "<<j.forth<<" "<<endl;
    }
}

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");

#define LINE_PARAMETRIC     1
#define LINE_DDA            2
#define LINE_MID            3
#define CIRCLE_DIRECT       4
#define CIRCLE_POLAR        5
#define CIRCLE_ITERATIVE    6
#define CIRCLE_MIDPOINT     7
#define CIRCLE_MODIFIED     8
#define ELLIPSE_POLAR       9
#define ELLIPSE_DIRECT      10
#define CLIPPING_POINT      11
#define CLIPPING_lINE       12
#define SAVE                13
#define LOAD                14
#define CLEAR               15
#define CIRCLE_FILLING      16
#define Color_id_red        17
#define Color_id_green      18
#define Color_id_blue       19
#define Color_id_black      20
#define Fill_first          21
#define Fill_second         22
#define Fill_third          23
#define Fill_fourth         24
void AddMenus(HWND hwnd);

string Color_String="Black";

string quad_type;

int Round(double x)
{
    return (int)(x+0.5);
}

COLORREF getColor(string color){
    if(color=="RED")
        return RGB(255,0,0);
    if(color=="BLUE")
        return RGB(0,0,255);
    if(color=="GREEN")
        return RGB(0,255,0);
    if(color=="BLACK")
        return RGB(0,0,0);
}

class point{
public:
    int x,y;
    point(){}
    point(int x,int y)
    {
        this->x=x;
        this->y=y;
    }

};
int length(point a,point b)
{
    return sqrt(pow((a.y-b.y),2)+pow((a.x-b.x),2));

}
class ellipse{
public:
    ellipse(){}
    void draw(HDC hdc, int xc, int yc, int rx, int ry,string type, string color_String){
        int length1;
        int length2;
        point a(xc,yc);
        point b(rx,ry);
        COLORREF color = getColor(color_String);
        if(abs(xc-rx)>abs(yc-ry)){
            length1=length(a,b);
            length2=((double)((double)1/(double)3)*(double)length(a,b));
        }
        else{
            length2=length(a,b);
            length1=((double)((double)1/(double)3)*(double)length(a,b));
        }
        if(type=="direct"){
            direct_ellipse(hdc,xc,yc,length1,length2,color);
        }
        else if(type =="polar"){
            polar_ellipse(hdc,xc,yc,length1,length2,color);
        }
    }

    void draw_ellipse(HDC hdc, int xc, int yc,float x,float y,COLORREF color){
        SetPixel(hdc,(int)(xc+x+0.5),(int)(yc+y+0.5),color);
        SetPixel(hdc,(int)(xc+x+0.5),(int)(yc-y+0.5),color);
        SetPixel(hdc,(int)(xc-x+0.5),(int)(yc-y+0.5),color);
        SetPixel(hdc,(int)(xc-x+0.5),(int)(yc+y+0.5),color);
    }
    void direct_ellipse(HDC hdc, int xc, int yc, int rx, int ry,COLORREF color){
        float x = 0;
        float y = rx;
        draw_ellipse(hdc, xc, yc, y, x,color);
        while (x <= y)
        {
            float tempx=pow(x,2);
            float temprx=pow(rx,2);
            float tempry=pow(ry,2);
            y =round(sqrt((float)((float)1-tempx/temprx)*tempry));
            draw_ellipse(hdc, xc, yc, x, y,color);
            x+=0.1;
        }
        y=0;
        x=ry;
        while (y <= x)
        {
            float tempy=pow(y,2);
            float temprx=pow(rx,2);
            float tempry=pow(ry,2);
            x =round(sqrt((float)((float)1-tempy/tempry)*temprx));
            draw_ellipse(hdc, xc, yc, x, y,color);
            y+=0.1;
        }
    }


    void polar_ellipse(HDC hdc, int xc, int yc, int rx, int ry,COLORREF color){
        float x=0;
        float y=ry;
        float angle=0;
        float range=rx;
        while(angle<=range){
            draw_ellipse(hdc,xc,yc,x,y,color);
            angle+=0.05;
            x=(rx*cos(angle));
            y=(ry*sin(angle));
        }
    }

};
class line{
public:
    line(){}
    void draw(HDC hdc,int x1,int y1,int x2,int y2,string type,string color_String){
        COLORREF color = getColor(color_String);
        if (type=="parametric")
            Parametric_line(hdc,x1,y1,x2,y2, color);
        if (type=="DDA")
            SimpleDDA(hdc,x1,y1,x2,y2, color);
        if(type=="midPoint")
            midPointLine(hdc,x1,y1,x2,y2, color);

    }
    void Parametric_line(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color)
    {
        double dx = x2-x1;
        double dy = y2-y1;
        for(double t=0 ; t<1 ; t+=0.001)
        {
            int point_x = x1+(t*dx);
            int point_y = y1+(t*dy);
            SetPixel(hdc,point_x,point_y,color);
        }
    }

    void SimpleDDA(HDC hdc,int xs,int ys,int xe,int ye,COLORREF color)
    {
        int dx=xe-xs;
        int dy=ye-ys;
        SetPixel(hdc,xs,ys,color);
        if(abs(dx)>=abs(dy))
        {
            int x=xs,xinc= dx>0?1:-1;
            double y=ys,yinc=(double)dy/dx*xinc;
            while(x!=xe)
            {
                x+=xinc;
                y+=yinc;
                SetPixel(hdc,x,round(y),color);
            }
        }
        else
        {
            int y=ys,yinc= dy>0?1:-1;
            double x=xs,xinc=(double)dx/dy*yinc;
            while(y!=ye)
            {
                x+=xinc;
                y+=yinc;
                SetPixel(hdc,round(x),y,color);
            }
        }
    }

    void midPointLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
    {
        int dx = (x2 - x1);
        int dy = (y2 - y1);
        if(((double)dy / dx) >= 0 &&((double)dy / dx) <= 1)
        {
            if(x1 > x2)
                swap(x1, x2), swap(y1, y2), dx = (x2 - x1), dy = (y2 - y1);
            int x = x1, y = y1;
            int d = dx - 2 * dy;
            int d1 = -2 * dy;
            int d2 = 2 * dx - 2 * dy;
            SetPixel(hdc, x, y, c);
            while(x < x2)
            {
                if(d < 0)
                    y++, x++, d += d2;
                else
                    x++, d += d1;
                SetPixel(hdc, x, y, c);
            }
        }
        else if (((double)dy / dx) <= -1)
        {
            if(y1 > y2)
                swap(x1, x2), swap(y1, y2);
            else
                dx = -1 * dx, dy = -1 * dy;

            int x = x1, y = y1;
            int d = 2 * dx - dy;
            int d1 = 2 * dx;
            int d2 = 2 * dx + 2 * dy;
            SetPixel(hdc, x, y, c);
            while(y < y2)
            {
                if(d < 0)
                    y++, d += d1;
                else
                    y++, x--, d += d2;
                SetPixel(hdc, x, y, c);
            }
        }
        else if (((double)dy / dx) > 1)
        {
            if(y1 > y2)
                swap(x1, x2), swap(y1, y2), dx = (x2 - x1), dy = (y2 - y1);
            int x = x1, y = y1;
            int d = dx - 2 * dy;
            int d1 = 2 * dx;
            int d2 = 2 * dx - 2 * dy;
            SetPixel(hdc, x, y, c);
            while(y < y2)
            {
                if(d < 0)
                    y++, d += d1;
                else
                    x++, y++, d += d2;
                SetPixel(hdc, x, y, c);
            }
        }
            ///   -1 to 0
        else
        {
            if(x1 > x2)
                swap(x1, x2), swap(y1, y2), dx = -1 * dx, dy = -1 * dy;
            int x = x1, y = y1;
            int d = dx + 2 * dy;
            int d1 = 2 * dy;
            int d2 = 2 * dx + 2 * dy;
            SetPixel(hdc, x, y, c);
            while(x < x2)
            {
                if(d > 0)
                    x++, d += d1;
                else
                    x++, y--, d += d2;
                SetPixel(hdc, x, y, c);
            }
        }
    }
};

void Line(HDC hdc,int x1,int y1,int x2,int y2,COLORREF color){
    double dx=x2-x1;
    double dy=y2-y1;
    for(double t=0;t<1;t+=0.001){
        int x=x1+(dx*t);
        int y=y1+(dy*t);
        SetPixel(hdc,x,y,color);
    }
}

void MyFloodFill(HDC hdc,int x1,int y1,int x2,int y2,string Color,COLORREF fillColor,string quad){
    COLORREF borderColor =getColor(Color);
    int x=0;
    point a(x1,y1);
    point b(x2,y2);
    int r=length(a,b);
    int y=r;
    double d=1-r;
    while(x<y){
        if(d<=0){
            d=d+2*x+3;
            x++;
        }
        else{
            d=d+2*(x-y)+5;
            x++;
            y--;
        }
        if(quad=="first"){
            Line(hdc,x1,y1,x1+x,y1-y,fillColor);//First quad
            Line(hdc,x1,y1,x1+y,y1-x,fillColor);//Second quad
        }
        else if(quad =="second"){
            Line(hdc,x1,y1,x1+y,y1+x,fillColor);//third quad
            Line(hdc,x1,y1,x1+x,y1+y,fillColor);//FOURTH quad
        }
        else if(quad == "third")
        {
            Line(hdc,x1,y1,x1-x,y1+y,fillColor);//FIFTH quad
            Line(hdc,x1,y1,x1-y,y1+x,fillColor);//six quad
        }
        else if(quad =="fourth"){
            Line(hdc,x1,y1,x1-y,y1-x,fillColor);//seventh quad
            Line(hdc,x1,y1,x1-x,y1-y,fillColor);//eight quad
        }

    }


}
class circle{
public:
    circle(){}
    void draw(HDC hdc,int xc,int yc, int xr,int yr,string type,string color_String){
        point a(xc,yc),b(xr,yr);
        int r=length(a,b);
        COLORREF color =getColor(color_String);
        if(type=="direct")
            DirectCircle(hdc, xc, yc, r, color);
        if(type=="polar")
            PolarCircle( hdc, xc, yc, r, color);
        if(type=="iterative")
            IterativePolarCircle(hdc, xc, yc, r, color);
        if (type=="midPoint")
            MidpointCircle(hdc, xc, yc, r, color);
        if(type=="modified")
            modifiedMidpointCircle(hdc, xc, yc, r, color);
    }
    void draw8Points(HDC hdc,int xc,int yc,int x,int y,COLORREF color)
    {
        SetPixel(hdc, xc+x , yc+y , color);
        SetPixel(hdc, xc-x , yc+y , color);
        SetPixel(hdc, xc-x , yc-y , color);
        SetPixel(hdc, xc+x , yc-y , color);
        SetPixel(hdc, xc+y , yc+x , color);
        SetPixel(hdc, xc-y , yc+x , color);
        SetPixel(hdc, xc-y , yc-x , color);
        SetPixel(hdc, xc+y , yc-x , color);
    }

    void DirectCircle(HDC hdc,int xc,int yc, int R,COLORREF color)
    {
        int x=0,y=R;
        int R2=R*R;
        draw8Points(hdc,xc,yc,x,y,color);
        while(x<y)
        {
            x++;
            y=round(sqrt((double)(R2-x*x)));
            draw8Points(hdc,xc,yc,x,y,color);
        }
    }

    void PolarCircle(HDC hdc,int xc,int yc,int R,COLORREF color)
    {
        double x=R,y=0;
        double theta=0,dtheta=1.0/R;
        draw8Points(hdc,xc,yc,R,0,color);
        while(x>y)
        {
            theta+=dtheta;
            x=R*cos(theta);
            y=R*sin(theta);
            draw8Points(hdc,xc,yc,Round(x),Round(y),color);
        }
    }

    void IterativePolarCircle(HDC hdc,int xc,int yc, int R,COLORREF color)
    {
        double x=R,y=0;
        double dtheta=1.0/R;
        double cdtheta=cos(dtheta),sdtheta=sin(dtheta);
        draw8Points(hdc,xc,yc,R,0,color);
        while(x>y)
        {
            double x1=x*cdtheta-y*sdtheta;
            y=x*sdtheta+y*cdtheta;
            x=x1;
            cout<<round(x)<<endl;
            draw8Points(hdc,xc,yc,round(x),round(y),color);
        }
    }

    void MidpointCircle(HDC hdc,int xc,int yc, int R,COLORREF color)
    {
        int x=0,y=R;
        int d=1-R;
        draw8Points(hdc,xc,yc,x,y,color);
        while(x<y)
        {
            if(d<0)
            {
                d+=2*x+2;
            }
            else
            {
                d+=2*(x-y)+5;
                y--;
            }
            x++;
            draw8Points(hdc,xc,yc,x,y,color);
        }
    }

    void modifiedMidpointCircle(HDC hdc,int xc,int yc, int R,COLORREF color)
    {
        int x=0,y=R;
        int d=1-R;
        int c1=3, c2=5-2*R;
        draw8Points(hdc,xc,yc,x,y,color);
        while(x<y)
        {
            if(d<0)
            {
                d+=c1;
                c2+=2;
            }
            else
            {
                d+=c2;
                c2+=4;
                y--;
            }
            c1+=2;
            x++;
            draw8Points(hdc,xc,yc,x,y,color);
        }
    }

};

void PointClipping(HDC hdc, int x,int y,int xleft,int ytop,int xright,int ybottom,string color_string)
{
    COLORREF color = getColor(color_string);
    if(x>=xleft && x<= xright && y>=ytop && y<=ybottom)
    {
        SetPixel(hdc,x,y,color);
    }
}


union OutCode
{
    unsigned All:4;
    struct
    {
        unsigned left:1,top:1,right:1,bottom:1;
    };
};

OutCode GetOutCode(double x,double y,int xleft,int ytop,int xright,int ybottom)
{
    OutCode out;
    out.All=0;
    if(x<xleft)
        out.left=1;
    else if(x>xright)
        out.right=1;
    if(y<ytop)
        out.top=1;
    else if(y>ybottom)
        out.bottom=1;
    return out;
}

void VIntersect(double xs,double ys,double xe,double ye,int x,double *xi,double *yi)
{
    *xi=x;
    *yi=ys+(x-xs)*(ye-ys)/(xe-xs);
}

void HIntersect(double xs,double ys,double xe,double ye,int y,double *xi,double *yi)
{
    *yi=y;
    *xi=xs+(y-ys)*(xe-xs)/(ye-ys);
}

void CohenSuth(HDC hdc,int xs,int ys,int xe,int ye,int xleft,int ytop,int xright,int ybottom)
{
    double x1=xs,y1=ys,x2=xe,y2=ye;
    OutCode out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
    OutCode out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
    while( (out1.All || out2.All) && !(out1.All & out2.All))
    {
        double xi,yi;
        if(out1.All)
        {
            if(out1.left)
                VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out1.top)
                HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out1.right)
                VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else
                HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x1=xi;
            y1=yi;
            out1=GetOutCode(x1,y1,xleft,ytop,xright,ybottom);
        }
        else
        {
            if(out2.left)
                VIntersect(x1,y1,x2,y2,xleft,&xi,&yi);
            else if(out2.top)
                HIntersect(x1,y1,x2,y2,ytop,&xi,&yi);
            else if(out2.right)
                VIntersect(x1,y1,x2,y2,xright,&xi,&yi);
            else
                HIntersect(x1,y1,x2,y2,ybottom,&xi,&yi);
            x2=xi;
            y2=yi;
            out2=GetOutCode(x2,y2,xleft,ytop,xright,ybottom);
        }
    }
    if(!out1.All && !out2.All)
    {

        MoveToEx(hdc,round(x1),round(y1),NULL);
        LineTo(hdc,round(x2),round(y2));
    }
}

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_CROSS);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND + 5;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
            0,                   /* Extended possibilites for variation */
            szClassName,         /* Classname */
            _T("Graphics Project"),       /* Title Text */
            WS_OVERLAPPEDWINDOW, /* default window */
            CW_USEDEFAULT,       /* Windows decides the position */
            CW_USEDEFAULT,       /* where the window ends up on the screen */
            544,                 /* The programs width */
            375,                 /* and height in pixels */
            HWND_DESKTOP,        /* The window is a child-window to desktop */
            NULL,                /* No menu */
            hThisInstance,       /* Program Instance handler */
            NULL                 /* No Window Creation data */
    );
    /* Make the window visible on the screen */
    AddMenus(hwnd);
    ShowWindow (hwnd, nCmdShow);
    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }
    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}
void load(HWND hwnd,vector<types>&Shape){
    HDC hdc=GetDC(hwnd);
    fstream file("shapes.txt");
    PAINTSTRUCT p ;
    bool check =false;
    string type, shape, color;
    types shap;
    int c_x,c_y,r_x,r_y,first,second,forth,third,ch1,ch2,ch3,ch4;
    circle cir;
    line lin;
    ellipse ell;

    while (file>>shape>>type>>color>>c_x>>c_y>>r_x>>r_y>>first>>second>>third>>forth) {
        shap.Name=shape ,shap.ShapeAlgo=type,shap.ShapeColor=color;
        shap.xc=c_x,shap.yc=c_y,shap.xr=r_x,shap.yr=r_y,shap.first=first,shap.second=second,shap.third=third,shap.forth=forth;
        if(shape=="Ellipse"){
            ell.draw(hdc,c_x,c_y,r_x,r_y,type,color);
        }
        if(shape=="Circle"){
            cir.draw(hdc,c_x,c_y,r_x,r_y,type,color);
        }
        if(shape=="Fill"){
            MyFloodFill(hdc,c_x,c_y,r_x,r_y,color,RGB(0,0,0),quad_type);
        }
        if(shape=="Line"){
            lin.draw(hdc,c_x,c_y,r_x,r_y,type,color);
//            cout<<"Line"<<endl;
        }
        if(shape=="Clipping"){
            if(!check||(ch1!=c_x&&ch2!=c_y&&ch3!=r_x&&ch4!=r_y)) {
                Rectangle(hdc, c_x, c_y, r_x, r_y);
                check=true;
                ch1=c_x,ch2=c_y,ch3=r_x,ch4=r_y;
            }
            if(type=="point"){
                PointClipping(hdc,first,second,c_x,c_y,r_x,r_y,color);
            }
            if(type=="line"){
                CohenSuth(hdc,first,second,third,forth,c_x,c_y,r_x,r_y);
            }
        }
        Shape.push_back(shap);
    }

}
void clearScreen(HWND hwnd){
    CloseWindow(hwnd);
    ShowWindow(hwnd,1);
    Shapes.clear();
}
int x,y;
int X1,Y1,X2,Y2;
bool flag = false;
point c, r;
vector<point> points;
string type="parametric";
ellipse ell;
string shape ="Line";
circle cir;
line lin;

types shap;

/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT p;


    switch (message)                  /* handle the messages */
    {
        case WM_PAINT:
            BeginPaint(hwnd, &p);
            if (flag)
            {

                shap.Name=shape ,shap.ShapeAlgo=type,shap.ShapeColor=Color_String;
                shap.xc=c.x,shap.yc=c.y,shap.xr=r.x,shap.yr=r.y;

                cout<<shap.Name<<" Nice"<<endl;
                if(shape=="Ellipse"){
                    ell.draw(p.hdc,c.x,c.y,r.x,r.y,type,Color_String);
                }
                if(shape=="Circle"){
                    cir.draw(p.hdc,c.x,c.y,r.x,r.y,type,Color_String);
                }
                if(shape=="Fill"){
                    MyFloodFill(p.hdc,c.x,c.y,r.x,r.y,Color_String,RGB(0,0,0),quad_type);
                }
                if(shape=="Line"){
                    lin.draw(p.hdc,c.x,c.y,r.x,r.y,type,Color_String);
                }
                if(shape=="Clipping"){
                    if(type=="point"){
                        shap.first=points[2].x,shap.second=points[2].y;
                        PointClipping(p.hdc,points[2].x,points[2].y,points[0].x,points[0].y,points[1].x,points[1].y,Color_String);
                        points.pop_back();
                    }
                    if(type=="line"){
                        shap.first=points[2].x,shap.second=points[2].y,shap.third=points[3].x,shap.forth=points[3].y;
                        CohenSuth(p.hdc,points[2].x,points[2].y,points[3].x,points[3].y,points[0].x,points[0].y,points[1].x,points[1].y);
                        points.pop_back();
                        points.pop_back();
                    }
                }
                Shapes.push_back(shap);
                cout<<"SHAPE: "<<shape<<endl;
                if(shape=="Fill")
                    cout<<"Quad: "<<quad_type<<endl;
                else
                    cout<<"TYPE: "<<type<<endl;
                cout<<"Color: "<<Color_String<<endl;
                flag=false;
            }
            EndPaint(hwnd, &p);
            break;

        case WM_LBUTTONDOWN:
            if(!flag&&shape!="Fill"&&shape!="Clipping")
            {
                if (points.size()==2)
                {
                    points.clear();
                }
                x = LOWORD(lParam);
                y = HIWORD(lParam);
                points.push_back(point(x,y));
                if (points.size()==2)
                {
                    flag=true;

                    c=points[0];
                    r=points[1];
                }
            }
            else if(!flag&&shape=="Clipping"){
                x = LOWORD(lParam);
                y = HIWORD(lParam);
                points.push_back(point(x,y));
                if(points.size()==1){
                    c.x=x;
                    c.y=y;
                }
                if(points.size()==2){
                    r.x=x;
                    r.y=y;
                }
                if(points.size()==2){
                    HDC hdc=GetDC(hwnd);

                    Rectangle(hdc, c.x,c.y,r.x,r.y);
                    cout<<"RECTANGLE"<<endl;
                }

                if (type=="point")
                {

                    if(points.size()>2){
                        flag=true;
                    }
                }
                if (points.size()==4&&type=="line")
                {
                    if(points.size()==4){
                        flag=true;
                    }
                }
            }
            InvalidateRect(hwnd, NULL, false);
            break;
        case WM_CREATE:
            AddMenus(hwnd);
            break;
        case WM_COMMAND:

            if (LOWORD(wParam) == Color_id_red) {
                Color_String="RED";
            }
            else if (LOWORD(wParam) == Color_id_green) {
                Color_String="GREEN";
            }
            else if (LOWORD(wParam) == Color_id_blue) {
                Color_String="BLUE";
            }
            else if (LOWORD(wParam) == Color_id_black) {
                Color_String="BLACK";
            }
            else if(LOWORD(wParam) == ELLIPSE_DIRECT){
                shape ="Ellipse";
                type="direct";
            }
            else if(LOWORD(wParam) == ELLIPSE_POLAR){
                shape ="Ellipse";
                type="polar";
            }
            else if(LOWORD(wParam) == CIRCLE_DIRECT){
                shape ="Circle";
                type="direct";
            }
            else if(LOWORD(wParam) == CIRCLE_POLAR){
                shape ="Circle";
                type="polar";
            }
            else if(LOWORD(wParam) == CIRCLE_ITERATIVE){
                shape ="Circle";
                type="iterative";
            }
            else if(LOWORD(wParam) == CIRCLE_MIDPOINT){
                shape ="Circle";
                type="midPoint";
            }
            else if(LOWORD(wParam) == CIRCLE_MODIFIED){
                shape ="Circle";
                type="modified";
            }
            else if(LOWORD(wParam) == Fill_first){
                shape ="Fill";
                quad_type="first";
                flag=true;
                InvalidateRect(hwnd, NULL, false);
            }
            else if(LOWORD(wParam) == Fill_second){
                shape ="Fill";
                quad_type="second";
                flag=true;
                InvalidateRect(hwnd, NULL, false);
            }
            else if(LOWORD(wParam) == Fill_third){
                shape ="Fill";
                quad_type="third";
                flag=true;
                InvalidateRect(hwnd, NULL, false);
            }
            else if(LOWORD(wParam) == Fill_fourth){
                shape ="Fill";
                quad_type="fourth";
                flag=true;
                InvalidateRect(hwnd, NULL, false);
            }
            else if(LOWORD(wParam) == LINE_PARAMETRIC){
                shape ="Line";
                type="parametric";
            }
            else if(LOWORD(wParam) == LINE_MID){
                shape ="Line";
                type="midPoint";
            }
            else if(LOWORD(wParam) == LINE_DDA){
                shape ="Line";
                type="DDA";
            }
            else if(LOWORD(wParam) == CLIPPING_POINT){
                shape ="Clipping";
                type="point";
                points.clear();
            }
            else if(LOWORD(wParam) == CLIPPING_lINE){
                shape ="Clipping";
                type="line";
                points.clear();
            }
            else if(LOWORD(wParam) == SAVE){//Aktb el 7aga hena
                Save(Shapes);
            }
            else if(LOWORD(wParam) == LOAD){
                load(hwnd,Shapes);
            }
            else if(LOWORD(wParam) == CLEAR){
                clearScreen(hwnd);
            }
//            cout<<Color_String<<endl;
            break;
        case WM_DESTROY:
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break;
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);

    }

    return 0;
}


void AddMenus(HWND hwnd)
{
    HMENU hMenubar = CreateMenu();
    HMENU hMenu = CreateMenu();
    HMENU data = CreateMenu();
    HMENU hMenuColor=CreateMenu();
    HMENU lineSubMenu = CreatePopupMenu();
    HMENU circleSubMenu = CreatePopupMenu();
    HMENU ellipseSubMenu = CreatePopupMenu();
    HMENU clippingSubMenu = CreatePopupMenu();
    HMENU hMenuFill=CreateMenu();
    AppendMenuW(hMenu, MF_STRING | MF_POPUP, (UINT_PTR) lineSubMenu, L"&Line");
    AppendMenuW(lineSubMenu, MF_STRING, LINE_PARAMETRIC, L"&Parametric");
    AppendMenuW(lineSubMenu, MF_STRING, LINE_DDA, L"&DDA");
    AppendMenuW(lineSubMenu, MF_STRING, LINE_MID, L"&Midpoint");

    AppendMenuW(hMenu, MF_STRING | MF_POPUP, (UINT_PTR) circleSubMenu, L"&Circle");
    AppendMenuW(circleSubMenu, MF_STRING, CIRCLE_DIRECT, L"&Direct");
    AppendMenuW(circleSubMenu, MF_STRING, CIRCLE_POLAR, L"&Polar");
    AppendMenuW(circleSubMenu, MF_STRING, CIRCLE_ITERATIVE, L"&Iterative Polar");
    AppendMenuW(circleSubMenu, MF_STRING, CIRCLE_MIDPOINT, L"&Midpoint");
    AppendMenuW(circleSubMenu, MF_STRING, CIRCLE_MODIFIED, L"&Modified Midpoint");
    AppendMenuW(hMenuFill, MF_STRING, Fill_first, L"&First");
    AppendMenuW(hMenuFill, MF_STRING, Fill_second,L"&Second");
    AppendMenuW(hMenuFill, MF_STRING, Fill_third, L"&Third");
    AppendMenuW(hMenuFill, MF_STRING, Fill_fourth, L"&Fourth");
    AppendMenuW(circleSubMenu, MF_STRING|MF_POPUP, (UINT_PTR) hMenuFill, L"&Quarter Filling");

    AppendMenuW(hMenu, MF_STRING | MF_POPUP, (UINT_PTR) ellipseSubMenu, L"&Ellipse");
    AppendMenuW(ellipseSubMenu, MF_STRING, ELLIPSE_DIRECT, L"&Direct");
    AppendMenuW(ellipseSubMenu, MF_STRING, ELLIPSE_POLAR, L"&Polar");

    AppendMenuW(hMenu, MF_STRING | MF_POPUP, (UINT_PTR) clippingSubMenu, L"&Clipping");
    AppendMenuW(clippingSubMenu, MF_STRING, CLIPPING_POINT, L"&Clipping Point");
    AppendMenuW(clippingSubMenu, MF_STRING, CLIPPING_lINE, L"&Clipping Line");


    AppendMenuW(hMenuColor, MF_STRING, Color_id_red, L"&RED");
    AppendMenuW(hMenuColor, MF_STRING, Color_id_green,L"&GREEN");
    AppendMenuW(hMenuColor, MF_STRING, Color_id_blue, L"&BLUE");
    AppendMenuW(hMenuColor, MF_STRING, Color_id_black, L"&Black");

    AppendMenuW(data, MF_STRING, LOAD, L"&Load");
    AppendMenuW(data, MF_STRING, SAVE, L"&Save");
    AppendMenuW(data, MF_STRING, CLEAR, L"&Clear");

    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR) hMenu, L"&draw");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR) hMenuColor, L"&colors");
    AppendMenuW(hMenubar, MF_POPUP, (UINT_PTR) data, L"&data");

    SetMenu(hwnd, hMenubar);

}

