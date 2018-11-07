//ReadingS.h提供了ReadingS类，提供，包含了一系列方法，能够对输入的表达式求值。
//Author: SpringCityStudio
//Date:2017.12.5(Since)
//12.20因为引用逻辑复杂化，独立出了对应头文件。
#ifndef READINGS_INCLUDED
#define READINGS_INCLUDED
//INCLUDE
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <sstream>
#include <fstream>
#include <iomanip>

#define MEMO_MAX 25
#define Math_MAX 14
using namespace  std;

//Class ReadingS
class ReadingS
{
    public:
        bool isInvaild;
        ReadingS(string str);
        //两个get字符串的方法
        string getRawStr();
        string getRPNStr();
        // 字符串预处理函数。主要针对不规范的输入进行添加空格等行为，便于逆波兰式的处理。
        void strTretment();
        // 转换字符串为逆波兰式，便于后续的处理。是中缀表达式的核心处理模块。
        void rawToRPN();
        //计算逆波兰式。是后缀表达式的核心处理模块。
        double calcRPN();
        //顶层步骤
        double calculate();
    protected:
        string rawStr;
        string rawBackUp;
        string stringRPN;
        // 判断给定字符是否属于运算符。
        bool isSymbol(char sym);
        //底层函数。返回字符串的下一个数字或符号。
        /*
        参数说明：
        -1 串终止。numStr返回空
        0 非法的字符。numStr返回空或无意义
        1 代表返回数字（float）。numStr返回对应的数字
        2 双元素运算符(+,-,*,/,^)。numStr返回对应运算符
        3 单元素运算符(!)。numStr返回对应运算符
        4 函数名称运算。numStr返回处理完成的逆波兰式。变量定义等在此中修正。
        5 （单独的）括号。numStr返回处理完成的逆波兰式。

        WARNING:readNext的读入是对原字符串存在损伤的，使用前建议备份。
        */
        void readNext(string& rawStr,string& numStr,int& type);
        //返回下一个包含括号的表达式尾部位置。
        int findEnd(string rawStr,int pin);
        //表达式求值执行计算的底层步骤。
        double numCalc(double numStack[],int& top,string nex);
        //比较两个运算符的优先级。
        bool latterBetter(string t1,string t2);
    private:
};

//Class var
class var
{
public:
    //构造器
    var();
    var(string fullstr);
    var(string name,double value);
    //Var
    string name;
    string expr;
    double value;
    bool isInvaild;
protected:
    void expr2value();
};

//关于Var管理的类。
class varL
{
public:
    //构造器
    varL();
    //增加变量。
    void newVar(string fullstr,int& lognum);
    //删除指定变量。
    void delVar(int delPos);
    //判断给定名称是否在列表中。
    /* -1不存在
        0存在于表头（一般对应于未知数或者Ans）
        n>0存在于列表（一般变量）*/
   int isInList(string givenName);
   //基础函数：set,get,display.
   string getValue(int pos);
   void setValue(int pos,var inVar);
   void display();
   int varNum;
private:
    var varlist[MEMO_MAX];
};

//关于函数的类func
class func:public var
{
public:
    func();
    func(string fullstr);
    func(string name,string ux,string expr);
    string ux;
    void expr2value(double value);
};

//提供管理函数的类funcL。由于相比之下对varL的重构程度太大，因此直接新建。
class funcL
{
public:
    //构造器
    funcL();
    //增加变量。
    void newFunc(string fullstr,int& lognum);
    //删除指定变量。
    void delFunc(int delPos);
    //判断给定名称是否在列表中。
    /* -1不存在
        0存在于表头（一般对应于未知数或者Ans）
        n>0存在于列表（一般变量）*/
   int isInList(string givenName);
   bool isInvaild(int pos);
   //基础函数：set,get,display.
   double getValue(int pos,double xvalue);
   void setFunc(int pos,func inFunc);
   void display();
   int funcNum;
private:
    func funclist[MEMO_MAX];
};

class solve_x
{
public:
    solve_x(string& fullstr,string ux);
    void solveMain();
    bool isInvaild;
private:
    bool newtonVaild;
    func equFunc;
    string ux;
    double newton_rep(double iniValue,int repTime);
    double getK(double x_value,double b1);
};
//VAR DEFINE
extern bool solveMode;
extern string errorList[];
extern string keyword[];
extern string mathFuncLib[];
extern varL constLib;
extern varL varList;
extern funcL funcList;
extern funcL funcLib;
//ALL FUNC DEFINE
void errorExit(string tipStr,int errorCode);
//判定给定的double是否在某整数deltaX的邻域内。
/*判定除0，计算乘方，阶乘时参与运算的数字应当是整数。
但由于double无法直接和int进行香的相等比较，因此引入该过程。
参数说明：
thefloat：待比较的浮点数
integer：指定的整数。不指定则默认为round(thefloat)
deltaX:允许最大邻域范围。默认10^-6.
defaultInt:是否使用默认的最接近整数x。*/
bool isAlmost(double thefloat,bool defaultInt = false,long integer = 0,double deltaX = 0.000001);
//辅助函数Round，cmp
double round(double r);
int cmp( const void *a , const void *b );
void commonLibInit();
//将字符串转换为实数的过程。
double convertToFloat(string floatStr);
//对输入的等式进行分流
void varOrFunc(string str,int& lognum);
//处理冲突变量的问题：将表达式中自变量名字替换为_VAR_.
void replaceVarName(string& replaced,string& kword,string nword);
bool inList(double List[],int Len,double Value);
int nrand(double miu,double sigma);
#endif
