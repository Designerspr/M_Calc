//ReadingS.h�ṩ��ReadingS�࣬�ṩ��������һϵ�з������ܹ�������ı��ʽ��ֵ��
//Author: SpringCityStudio
//Date:2017.12.5(Since)
//12.20��Ϊ�����߼����ӻ����������˶�Ӧͷ�ļ���
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
        //����get�ַ����ķ���
        string getRawStr();
        string getRPNStr();
        // �ַ���Ԥ����������Ҫ��Բ��淶�����������ӿո����Ϊ�������沨��ʽ�Ĵ���
        void strTretment();
        // ת���ַ���Ϊ�沨��ʽ�����ں����Ĵ�������׺���ʽ�ĺ��Ĵ���ģ�顣
        void rawToRPN();
        //�����沨��ʽ���Ǻ�׺���ʽ�ĺ��Ĵ���ģ�顣
        double calcRPN();
        //���㲽��
        double calculate();
    protected:
        string rawStr;
        string rawBackUp;
        string stringRPN;
        // �жϸ����ַ��Ƿ������������
        bool isSymbol(char sym);
        //�ײ㺯���������ַ�������һ�����ֻ���š�
        /*
        ����˵����
        -1 ����ֹ��numStr���ؿ�
        0 �Ƿ����ַ���numStr���ؿջ�������
        1 ���������֣�float����numStr���ض�Ӧ������
        2 ˫Ԫ�������(+,-,*,/,^)��numStr���ض�Ӧ�����
        3 ��Ԫ�������(!)��numStr���ض�Ӧ�����
        4 �����������㡣numStr���ش�����ɵ��沨��ʽ������������ڴ���������
        5 �������ģ����š�numStr���ش�����ɵ��沨��ʽ��

        WARNING:readNext�Ķ����Ƕ�ԭ�ַ����������˵ģ�ʹ��ǰ���鱸�ݡ�
        */
        void readNext(string& rawStr,string& numStr,int& type);
        //������һ���������ŵı��ʽβ��λ�á�
        int findEnd(string rawStr,int pin);
        //���ʽ��ִֵ�м���ĵײ㲽�衣
        double numCalc(double numStack[],int& top,string nex);
        //�Ƚ���������������ȼ���
        bool latterBetter(string t1,string t2);
    private:
};

//Class var
class var
{
public:
    //������
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

//����Var������ࡣ
class varL
{
public:
    //������
    varL();
    //���ӱ�����
    void newVar(string fullstr,int& lognum);
    //ɾ��ָ��������
    void delVar(int delPos);
    //�жϸ��������Ƿ����б��С�
    /* -1������
        0�����ڱ�ͷ��һ���Ӧ��δ֪������Ans��
        n>0�������б�һ�������*/
   int isInList(string givenName);
   //����������set,get,display.
   string getValue(int pos);
   void setValue(int pos,var inVar);
   void display();
   int varNum;
private:
    var varlist[MEMO_MAX];
};

//���ں�������func
class func:public var
{
public:
    func();
    func(string fullstr);
    func(string name,string ux,string expr);
    string ux;
    void expr2value(double value);
};

//�ṩ����������funcL���������֮�¶�varL���ع��̶�̫�����ֱ���½���
class funcL
{
public:
    //������
    funcL();
    //���ӱ�����
    void newFunc(string fullstr,int& lognum);
    //ɾ��ָ��������
    void delFunc(int delPos);
    //�жϸ��������Ƿ����б��С�
    /* -1������
        0�����ڱ�ͷ��һ���Ӧ��δ֪������Ans��
        n>0�������б�һ�������*/
   int isInList(string givenName);
   bool isInvaild(int pos);
   //����������set,get,display.
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
//�ж�������double�Ƿ���ĳ����deltaX�������ڡ�
/*�ж���0������˷����׳�ʱ�������������Ӧ����������
������double�޷�ֱ�Ӻ�int���������ȱȽϣ��������ù��̡�
����˵����
thefloat�����Ƚϵĸ�����
integer��ָ������������ָ����Ĭ��Ϊround(thefloat)
deltaX:�����������Χ��Ĭ��10^-6.
defaultInt:�Ƿ�ʹ��Ĭ�ϵ���ӽ�����x��*/
bool isAlmost(double thefloat,bool defaultInt = false,long integer = 0,double deltaX = 0.000001);
//��������Round��cmp
double round(double r);
int cmp( const void *a , const void *b );
void commonLibInit();
//���ַ���ת��Ϊʵ���Ĺ��̡�
double convertToFloat(string floatStr);
//������ĵ�ʽ���з���
void varOrFunc(string str,int& lognum);
//�����ͻ���������⣺�����ʽ���Ա��������滻Ϊ_VAR_.
void replaceVarName(string& replaced,string& kword,string nword);
bool inList(double List[],int Len,double Value);
int nrand(double miu,double sigma);
#endif
