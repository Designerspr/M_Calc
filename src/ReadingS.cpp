//ReadingS.h提供了ReadingS类，提供，包含了一系列方法，能够对输入的表达式求值。
//Author: SpringCityStudio
//Date:2017.12.5(Since)
//second update: finished the rawToRPN function,RPNcalc function(2017.12.6 20:21)
//Third Update:add the defination about var(2017.12.19,17:21)，"ans" function(12.20,18:33)
//Fourth Update:add the function,and bugs fixed(mainly).(12.21 12:37+12.22 00:02)
//Fifth Update:add equation solveing，bug fixed(12.27,10:44)
#include "ReadingS.h"

//Class ReadingS
bool solveMode=false;
string errorList[]={"error0：非法的表达式：",
                              "error1：除零错误;",
                              "error2:不正确的括号嵌套;",
                              "error3:表达式逻辑错误：",
                              "error4:幂运算错误（底数或指数）;",
                              "error5:阶乘运算错误(过大，过小，或者非整数的底数：",
                              "error6:未定义的函数：",
                              "error7:非法的对数计算",
                              "error8:对负数开根",
                              "Warning0：名称重复，覆盖此前定义的",
                              "error10:对此符号无法求值:",
                              "error11:与已有变量重复的自变量：",
                              "error12:无法求解给定方程：",
                              "error13:计算产生了无穷大(inf)或非零数(nan)。",
                              "Warning1:已删除这个变量："};
string keyword[]={"","calc","solve","help","exit","logout","print","del"};
//mathFuncLib储存了借助math库的函数名称
//constLib，funcLib提供了预置的常用函数，常量。
//varList提供变量内存数组，funcList提供函数内存数组
string mathFuncLib[]={"sin","cos","tan","arcsin","arccos","arctan","sinh","cosh","tanh","abs","sqrt","ln","log","inf","nan"};
varL varList;
varL constLib;
funcL funcList;
funcL funcLib;
//构造器
ReadingS::ReadingS(string str)
        {
            //补充0消除开头负号的处理
            int s1=0;
            for(s1=0;((str[s1]==' ')&&(s1<(str.length())));s1++);
            if (str[s1]=='-')
                rawStr="0"+str;
            else
                rawStr=str;
            rawBackUp=rawStr;
            stringRPN="";
            isInvaild=false;
        }

//两个get字符串的方法
string ReadingS::getRawStr()
        {
            return rawStr;
        }
string ReadingS::getRPNStr()
        {
            return stringRPN;
        }

// 字符串预处理函数。主要针对不规范的输入进行添加空格等行为，便于逆波兰式的处理。
void ReadingS::strTretment()
        {
            string outStr="";
            for(int i=0;i<rawStr.length();i++)
                if (isSymbol(rawStr[i]))
                    if (((rawStr[i]=='+')||(rawStr[i]=='-'))&&(((rawStr[i-1]=='E')||(rawStr[i-1]=='e'))&&i>1&&rawStr[i-2]>='0'&&rawStr[i-2]<='9'))
                        outStr=outStr+rawStr[i];
                    else
                        outStr=outStr+" "+rawStr[i]+" ";
                else outStr=outStr+rawStr[i];
            rawStr=outStr;
            //cout<<"rawStr="<<rawStr<<endl;
        }

// 转换字符串为逆波兰式，便于后续的处理。是中缀表达式的核心处理模块。
void ReadingS::rawToRPN( )
        {
            string RPNout="",rawCopy=rawStr;
            string symStack[100];//符号堆栈
            int top=0;//栈顶指针
            string nex;
            int staCode;

            symStack[0]="#";//栈顶占位符。不参与运算，不弹出，用于简化代码。
            readNext(rawCopy,nex,staCode);
            while (staCode!=-1)
            {
                //处理抛出异常
                if ((isInvaild)||(staCode==0))
                {
                    errorExit(nex,0);
                    break;
                }
                //对于括号，单元素运算符，其优先级是最高的，直接连接在字符串尾部即可。
                if ((staCode==3)||(staCode==4)||(staCode==5))
                    RPNout=RPNout+nex;
                else if (staCode==2)
                {
                    //先弹出更加优先的符号
                    while (!latterBetter(symStack[top],nex))
                    {
                        RPNout=RPNout+symStack[top];
                        symStack[top]="";
                        top--;
                    }
                    // 顶部字符进栈。
                    top++;
                    symStack[top]=nex;
                }
                else
                {
                    //数字直接进入RPN式当中。
                    RPNout=RPNout+nex;
                }
                //读取下一个
                readNext(rawCopy,nex,staCode);
            }
            //弹出栈余的符号
            while (top!=0)
            {
                RPNout=RPNout+symStack[top];
                top--;
            }
            stringRPN=RPNout;
        }

//计算逆波兰式。是后缀表达式的核心处理模块。
double ReadingS::calcRPN()
        {
            double numStack[100];
            int top=0;
            string nex,RPNcopy=stringRPN;
            int staCode;
            readNext(RPNcopy,nex,staCode);
            while ((staCode!=-1)&&(!isInvaild))
            {
                if ((isInvaild)||(staCode==0))
                {
                    errorExit(nex,0);
                    break;
                }
                nex.assign(nex,0,nex.length()-1);//消除末尾空格...(重载readNext对字符串要求不同)
                if (staCode==1)
                {
                    numStack[top]=convertToFloat(nex);
                    top++;
                }
                else
                    numCalc(numStack,top,nex);
                if (top<0)
                {
                    isInvaild=true;
                    errorExit("",3);
                }
                readNext(RPNcopy,nex,staCode);
            }
            if (top>1)
            {
                isInvaild=true;
                errorExit("",3);
            }
            if (!isInvaild)
                return numStack[0];
            return 0;
        }

//顶层步骤
double ReadingS::calculate()
        {
            this->strTretment();
            this->rawToRPN();
            double numout=this->calcRPN();
            return numout;
        }

// 判断给定字符是否属于运算符。
bool ReadingS::isSymbol(char sym)
        {
            char symbolList[6]={'+','-','*','/','^','!'};
            bool flag=false;
            for(int i=0;i<6;i++)
                if (sym==symbolList[i])
                    flag=true;
            return flag;
        }

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
void ReadingS::readNext(string& rawStr,string& numStr,int& type)
        {
            //VAR
            int s1=0,s2=0;
            numStr="";
            type=0;
            for(s1=0;((rawStr[s1]==' ')&&(s1<(rawStr.length())));s1++);
            //若串已经结束则返回-1.
            if (s1==rawStr.length())
                type=-1;
            else
            {
                numStr[0]=rawStr[s1];
                //依据第一位字符判定数据类型
                if (isSymbol(numStr[0]))
                    if (numStr[0]=='!')
                        type=3;
                    else
                        type=2;
                else if (((numStr[0]>='A')&&(numStr[0]<='Z'))||((numStr[0]>='a')&&(numStr[0]<='z'))||(numStr[0]=='_'))
                        type=4;
                    else if (numStr[0]=='(')
                        type=5;
                        else if ((numStr[0]>='0')&&(numStr[0]<='9'))
                                 type=1;
                //依据类型执行不同的切割方法
                switch (type)
                    {
                    case 0:
                        {
                            errorExit(numStr,1);
                            isInvaild=true;
                            break;
                        }
                    case 1:
                        {
                            for(s2=s1;((rawStr[s2]!=' ')&&(s2<rawStr.length()));s2++);
                            numStr.assign(rawStr,s1,s2-s1);
                            numStr=numStr+" ";
                            string outStr;outStr.assign(rawStr,s2,rawStr.length()-s2);rawStr=outStr;
                            break;
                        }
                    case 2:
                        {
                            numStr.assign(rawStr,s1,1);
                            numStr=numStr+" ";
                            string outStr;outStr.assign(rawStr,s1+1,rawStr.length()-s1-1);rawStr=outStr;
                            break;
                        }
                    case 3:
                        {
                            numStr.assign(rawStr,s1,1);
                            numStr=numStr+" ";
                            string outStr;outStr.assign(rawStr,s1+1,rawStr.length()-s1-1);rawStr=outStr;
                            break;
                        }
                    case 4:
                        {

                            for(s2=s1;((rawStr[s2]!='(')&&(rawStr[s2]!=' ')&&(s2<(rawStr.length())));s2++);
                            //若为表达式
                            if (rawStr[s2]=='(')
                            {
                                int endPos=findEnd(rawStr,s2);
                                if (endPos==0)
                                    errorExit("",2);
                                else
                                {
                                    string partStr;
                                    partStr.assign(rawStr,s2+1,endPos-s2-1);
                                    ReadingS partRS(partStr);
                                    partRS.rawToRPN();
                                    numStr.assign(rawStr,s1,s2-s1);
                                    numStr=partRS.getRPNStr()+" "+numStr+" ";
                                    string outStr;outStr.assign(rawStr,endPos+1,rawStr.length()-endPos-1);rawStr=outStr;
                                }
                            }
                            //若为操作符（即RPN计算时，以操作符进行理解）
                            else
                            {
                                for(s2=s1;((rawStr[s2]!=' ')&&(s2<(rawStr.length())));s2++);
                                numStr.assign(rawStr,s1,s2-s1);
                                //12.20增添
                                //最优先：变量的判定
                                if (varList.isInList(numStr)>=0)
                                {
                                    numStr=varList.getValue(varList.isInList(numStr));
                                    type=1;
                                }
                                //其次：自带常量库
                                else if (constLib.isInList(numStr)>=0)
                                {
                                    numStr=constLib.getValue(constLib.isInList(numStr));
                                    type=1;
                                }
                                //End Of Add Of 12.20
                                numStr=numStr+" ";
                                string outStr;outStr.assign(rawStr,s2,rawStr.length()-s2);rawStr=outStr;
                            }
                            break;
                        }
                    case 5:
                        {
                            int endPos=findEnd(rawStr,s1);
                            if (endPos==0)
                                errorExit("",2);
                            else
                            {
                                string partStr;
                                partStr.assign(rawStr,s1+1,endPos-s1-1);
                                ReadingS partRS(partStr);
                                partRS.rawToRPN();
                                numStr=partRS.getRPNStr()+" ";
                                string outStr;outStr.assign(rawStr,endPos+1,rawStr.length()-endPos-1);rawStr=outStr;
                            }
                            break;
                        }
                    }
            }
            //cout<<"NUMSTR="<<numStr<<"|with type of"<<type<<endl;
        }

        //返回下一个包含括号的表达式尾部位置。
int ReadingS::findEnd(string rawStr,int pin)
        {
            int temp=1;
            while (temp!=0)
            {
                pin++;
                if (rawStr[pin]=='(')
                        temp++;
                else if (rawStr[pin]==')')
                        temp--;
                if (pin>=rawStr.length())
                {
                    isInvaild=true;
                    break;
                }
            }
            if (isInvaild)
                return 0;
            return pin;
        }


        //表达式求值执行计算的底层步骤。
double ReadingS::numCalc(double numStack[],int& top,string nex)
        {
            if  ((isSymbol(nex[0]))&&(nex[0]!='!'))
            {
                if (top<2)
                {
                    isInvaild=true;
                    errorExit(nex,3);
                    return (-1);
                }
                top--;
                if (nex== "+")
                    numStack[top-1]=numStack[top-1]+numStack[top];
                else if (nex== "-")
                    numStack[top-1]=numStack[top-1]-numStack[top];
                else if (nex== "*")
                    numStack[top-1]=numStack[top-1]*numStack[top];
                else if (nex== "/")
                {
                    if (isAlmost(numStack[top],false,0))
                    {
                        isInvaild=true;
                        errorExit("",1);
                    }
                    else
                        numStack[top-1]=numStack[top-1]/numStack[top];
                }
                else if (nex=="^")
                {
                    numStack[top-1]=pow(numStack[top-1],numStack[top]);
                    //合理性检测
                    if (!(numStack[top-1]==numStack[top-1]))
                    {
                        isInvaild=true;
                        errorExit("",4);
                    }
                }
            }
            else
            {
                //cout<<"nex after comparsion="<<nex<<"|"<<endl;
                if  (top<1)
                {
                    isInvaild=true;
                    errorExit(nex,3);
                    return (-1);
                }

                if (nex== "!")
                {
                    if ((!isAlmost(numStack[top-1],true))||(numStack[top-1]<0))
                    {
                        isInvaild=true;
                        errorExit("",1);
                    }
                    else
                    {
                        double sum=1;
                        for(long long i=(long long)round(numStack[top-1]);i>0;i--)
                        {
                            sum *= i;
                        }
                        numStack[top-1]=sum;
                    }
                }
                else if (funcList.isInList(nex)>=0)
                {
                    //FOR TEST：cout<<"nex="<<nex<<";"<<endl<<"Value="<<numStack[top-1]<<endl;
                    numStack[top-1]=funcList.getValue(funcList.isInList(nex),numStack[top-1]);
                    if (funcList.isInvaild(funcList.isInList(nex)))
                        this->isInvaild=true;
                }
                else if(funcLib.isInList(nex)>=0)
                {
                    numStack[top-1]=funcLib.getValue(funcLib.isInList(nex),numStack[top-1]);
                    if (funcLib.isInvaild(funcLib.isInList(nex)))
                        this->isInvaild=true;
                }
                else
                {
                    //FIND WHERE NEX IS
                    int status=-1;
                    for (int i=0;i<Math_MAX;i++)
                        if (nex==mathFuncLib[i])
                            status=i;
                    //SWITCH-CASE
                    switch (status)
                    {
                    case 0://sin
                        {numStack[top-1]=sin(numStack[top-1]);break;}
                    case 1://cos
                        {numStack[top-1]=cos(numStack[top-1]);break;}
                    case 2://tan
                        {numStack[top-1]=tan(numStack[top-1]);break;}
                    case 3://arcsin
                        {numStack[top-1]=asin(numStack[top-1]);break;}
                    case 4://arccos
                        {numStack[top-1]=acos(numStack[top-1]);break;}
                    case 5://arctan
                        {numStack[top-1]=atan(numStack[top-1]);break;}
                    case 6://sinh
                        {numStack[top-1]=sinh(numStack[top-1]);break;}
                    case 7://cosh
                        {numStack[top-1]=cosh(numStack[top-1]);break;}
                    case 8://tanh
                        {numStack[top-1]=tanh(numStack[top-1]);break;}
                    case 9://abs
                        {if  (numStack[top-1]<0)
                            numStack[top-1]=-numStack[top-1];break;}
                    case 10://sqrt
                        {if  (numStack[top-1]>=0)
                            numStack[top-1]=sqrt(numStack[top-1]);
                            else
                            {
                                isInvaild=true;
                                errorExit("",8);
                            }
                         break;}
                    case 11:
                        {if (numStack[top-1]>0)
                            numStack[top-1]=log(numStack[top-1]);
                            else
                            {
                                isInvaild=true;
                                errorExit("",7);
                            }
                        break;}
                    case 12:
                        {   if (numStack[top-1]>0)
                            numStack[top-1]=log10(numStack[top-1]);
                            else
                            {
                                isInvaild=true;
                                errorExit("",7);
                            }
                        break;}
                    case 13:
                        {   if (numStack[top-1]>0)
                            numStack[top-1]=log10(numStack[top-1]);
                            else
                            {
                                isInvaild=true;
                                errorExit("",7);
                            }
                        break;}
                    case 14:
                        {//inf
                            isInvaild=true;
                            errorExit("",13);
                            break;}
                    case  15:
                        { //nan
                            isInvaild=true;
                            errorExit("",13);
                            break;}
                    default:
                        {
                            isInvaild=true;
                            errorExit(nex,6);}
                    }
                }
            }
        }

        //比较两个运算符的优先级。
bool ReadingS::latterBetter(string t1,string t2)
        {
            //cout<<"t1=|"<<t1<<"|;t2=|"<<t2<<"|"<<endl;
            if (t1=="#")
                return true;
            if (t2=="^ ")
                return true;
            if ((t1=="^ ")||(t1=="* ")||(t1=="/ "))
                return false;
            if (((t1=="+ ")||(t1=="- "))&&((t2=="+ ")||(t2=="- ")))
                return false;
            return true;
        }

//Class var
//构造器
var::var(){
    isInvaild=false;}
var::var(string fullstr)
    {
        isInvaild=false;
        int ePos,num=0;
        for(int i=0;i<fullstr.length();i++)
            if (fullstr[i]=='=')
            {
                ePos=i;
                num++;
            }
        //Vaild Dect
        if (num!=1)
        {
            isInvaild=true;
            errorExit(fullstr,0);
        }
        else
        {
            name.assign(fullstr,0,ePos);
            expr.assign(fullstr,ePos+1,fullstr.length()-ePos-1);
            expr2value();
        }
    }
var::var(string name,double value)
{
    this->name=name;
    this->value=value;
    isInvaild=false;
}
void var::expr2value()
    {
        ReadingS expression(expr);
        double ans=expression.calculate();
        if (!expression.isInvaild)
            value=ans;
        else
        {
            isInvaild=true;
            errorExit(name,10);
        }
        stringstream s;
        s<<setprecision(15)<<ans;
        string str;
        s>>str;
    }

//关于Var管理的类。
//构造器
varL::varL(){
    varNum=0;}

//增加变量。
void varL::newVar(string fullstr,int& lognum)
{
    int rPos=-1;
            if (varNum>=MEMO_MAX)
            {
                cout<<"Too Many Vars.";
            }
            else
            {
                varNum++;
                varlist[varNum]=var(fullstr);
                for(int i=0;i<varNum;i++)
                    if (varlist[i].name==varlist[varNum].name)
                        rPos=i;
                if (varlist[varNum].isInvaild)
                    varNum--;
                else
                {
                    if (rPos!=-1)
                        {
                        errorExit(varlist[varNum].name,9);
                        delVar(rPos);
                        }
                    lognum++;
                    if (lognum>0)
                        cout<<"Created:"<<varlist[varNum].name<<"="<<varlist[varNum].value<<endl;
                }
            }
        }

//删除指定变量。
void varL::delVar(int delPos)
    {
        for(int i=delPos;i<varNum;i++)
            varlist[i]=varlist[i+1];
        varNum--;
    }

//判断给定名称是否在列表中。
/* -1不存在
    0存在于表头（一般对应于未知数或者Ans）
    n>0存在于列表（一般变量）*/
int varL::isInList(string givenName)
   {
       int exitcode=-1;
       for(int i=0;i<varNum+1;i++)
            if (varlist[i].name==givenName)
            {
                exitcode=i;
                break;
            }
        return exitcode;
   }

//设定值
void varL::setValue(int pos,var inVar)
{
    varlist[pos]=inVar;
}

//返回值
string varL::getValue(int pos)
{
        stringstream str;
        string outputStr;
        //负数的修正输出格式。
        //虽然这个问题可能相关于更多的Bug，不过暂时这样曲线救国= =
        //Q:做转换时，精度不够
        if (varlist[pos].value<0)
        {
            str<<setprecision(15)<<-varlist[pos].value;
            str>>outputStr;
            outputStr="0 "+outputStr+" -";
        }
        else
        {
            str<<setprecision(15)<<varlist[pos].value;
            str>>outputStr;
        }
       return outputStr;
}
//输出全表
void varL::display()
{
    for(int i=0;i<varNum+1;i++)
        cout<<i<<":NAME="<<varlist[i].name<<";VALUE="<<varlist[i].value<<";"<<endl;
}

//class func

//重载构造器*3
func::func()
{
    ux="";
    isInvaild=false;
}
func::func(string fullstr)
{
    isInvaild=false;
    int ePos,num=0;
    for(int i=0;i<fullstr.length();i++)
        if (fullstr[i]=='=')
        {
            ePos=i;
            num++;
        }
    //Vaild Dect
    if (num!=1)
    {
        isInvaild=true;
        errorExit(fullstr,0);
    }
    else
    {
        name.assign(fullstr,0,ePos);
        expr.assign(fullstr,ePos+1,fullstr.length()-ePos-1);
        int lP,rP;
        for(lP=0;(lP<fullstr.length())&&(fullstr[lP]!='(');lP++);
        for(rP=lP;(rP<fullstr.length())&&(fullstr[rP]!=')');rP++);
        if ((lP==fullstr.length())||(lP==fullstr.length()))
            isInvaild=true;
        else
        {
            ux.assign(name,lP+1,rP-lP-1);
            name.assign(name,0,lP);
            //处理冲突变量的问题：将表达式中自变量名字替换为_var_func.name.
            string new_ux="__VAR__"+name;
            replaceVarName(expr,ux,new_ux);
            ux=new_ux;
            //合法性测试：主要关于是否使用了未定义的变量，函数。
            ReadingS vaildTest=ReadingS(expr);
            vaildTest.strTretment();
            vaildTest.rawToRPN();
            string rpnTest=vaildTest.getRPNStr();
            int pl=0,dl=0;string cutted;
            while(pl<rpnTest.length())
            {
                for(dl=0;((pl+dl<rpnTest.length())&&(rpnTest[pl+dl]!=' '));dl++);
                cutted.assign(rpnTest,pl,dl);
                bool isInMathList=false;
                for(int i=0;i<Math_MAX;i++)
                    if (cutted==mathFuncLib[i])
                        isInMathList=true;
                if ((cutted[0]>='A')&&(cutted[0]<='Z')||(cutted[0]>='a')&&(cutted[0]<='z')||(cutted[0]=='_'))
                    if ((funcList.isInList(cutted)<0)&&(funcLib.isInList(cutted)<0)&&(cutted!=new_ux)&&(!isInMathList))
                    {
                        errorExit(cutted,6);
                        isInvaild=true;
                    }
                pl+=dl+1;
            }
            //END OF DETECT
        }
    }
}
func::func(string name,string ux,string expr)
{
        isInvaild=false;
        this->name=name;
        this->ux=ux;
        this->expr=expr;
}
void func::expr2value(double xvalue)
{
    stringstream ss;
    ss<<ux<<"="<<setprecision(15)<<xvalue;
    string str;
    ss>>str;
    int i=-1;
    varList.newVar(str,i);
    var::expr2value();
    varList.delVar(varList.varNum);
}

//class funcL
//构造器
funcL::funcL(){
    funcNum=0;}
//增加Func。
void funcL::newFunc(string fullstr,int& lognum)
{
    int rPos=-1;
    if (funcNum>=MEMO_MAX)
    {
        cout<<"Too Many Functions.";
    }
    else
    {
        funcNum++;
        funclist[funcNum]=func(fullstr);
        for(int i=0;i<funcNum;i++)
            if (funclist[i].name==funclist[funcNum].name)
                rPos=i;
        //VERSION beta 0.4.0 used to check: getValue(funcNum,0);
        if (funclist[funcNum].isInvaild)
            funcNum--;
        else if(lognum>0)
        {
            if (rPos!=-1)
            {
                errorExit(funclist[funcNum].name,9);
                delFunc(rPos);
            }
            lognum++;
            cout<<"Created:"<<funclist[funcNum].name<<"("<<funclist[funcNum].ux<<")="<<funclist[funcNum].expr<<";"<<endl;
        }
    }
}
//删除指定变量。
void funcL::delFunc(int delPos)
{
    for(int i=delPos;i<funcNum;i++)
        funclist[i]=funclist[i+1];
    funcNum--;
}
//判断给定名称是否在列表中。
/* -1不存在
0存在于表头（一般对应于未知数或者Ans）
n>0存在于列表（一般变量）*/
int funcL::isInList(string givenName)
{
       int exitcode=-1;
       for(int i=0;i<funcNum+1;i++)
            if (funclist[i].name==givenName)
            {
                exitcode=i;
                break;
            }
        return exitcode;
}
bool funcL::isInvaild(int pos){
    return funclist[pos].isInvaild;}
//基础函数：set,get,display.
double funcL::getValue(int pos,double xvalue)
{
    funclist[pos].isInvaild=false;
    funclist[pos].expr2value(xvalue);
    return funclist[pos].value;
}

void funcL::setFunc(int pos,func inFunc){
    funclist[pos]=inFunc;}

void funcL::display()
{
    for(int i=1;i<funcNum+1;i++)
        cout<<i<<" : "<<funclist[i].name<<"("<<funclist[i].ux<<")="<<funclist[i].expr<<";"<<endl;
}

//Class Solve_x
//构造器
solve_x::solve_x(string& fullstr,string ux)
{
    isInvaild=false;
    int ePos,num=0;
    for(int i=0;i<fullstr.length();i++)
        if (fullstr[i]=='=')
        {
            ePos=i;
            num++;
        }
    //Vaild Dect
    if (num!=1)
    {
        isInvaild=true;
        errorExit(fullstr,0);
    }
    else
    {
        string left;left.assign(fullstr,0,ePos);
        string right;right.assign(fullstr,ePos+1,fullstr.length()-ePos-1);
        string expr=left+"-("+right+")";
        cout<<"Created Equation:"<<expr<<"=0;"<<endl;
        this->ux=ux;
        equFunc=func("__SOLVE__",ux,expr);
        if (equFunc.isInvaild)
        {
            isInvaild=true;
            errorExit(expr,3);
        }
    }
}
//MAIN
void solve_x::solveMain()
{
    solveMode=true;//隐藏不必要的异常输出
    int repTot=1000,repTime=0,ansNum=0;
    double ansList[20],x,ansThis;
    int noNewAns=0;
    srand((unsigned)time(NULL));
    while (repTime<repTot&&noNewAns<150&&ansNum<20)
    {
        newtonVaild=true;
        repTime++;
        noNewAns++;
        //RAND X
        x=nrand(0,(ansNum+1));
        ansThis=newton_rep(x,1);
        if (newtonVaild)
            if (!inList(ansList,ansNum,ansThis))
            {
                ansList[ansNum]=ansThis;
                ansNum++;
                noNewAns=0;
            }
        if (isInvaild)
            break;
    }
    if (isInvaild)
        errorExit("",12);
    else
    {
        qsort(ansList,ansNum,sizeof(ansList[0]),cmp);
        if (ansNum==0)
            cout<<"SOLVE FINISHED, BUT NO RESULT."<<endl;
        else
            cout<<"SOLVE FINISHED.RESULT:"<<endl;
        for(int i=0;i<ansNum;i++)
            cout<<" "<<ux<<i+1<<" = "<<setprecision(6)<<ansList[i]<<";"<<endl;
    }
}
//牛顿迭代法核心：以iniValue为初值迭代出答案。
double solve_x::newton_rep(double iniValue,int repTime)
{
    equFunc.expr2value(iniValue);
    double b=equFunc.value;
    //基于零点存在定理的判定
    equFunc.expr2value(iniValue-0.000005);
    double bl=equFunc.value;
    equFunc.expr2value(iniValue+0.000005);
    double br=equFunc.value;
    if (bl*br<=0)
        return iniValue;
    double k=getK(iniValue,b);
    //核心迭代关系式
    double nexti=iniValue-b/k;
    //对无解的防死循环处理
    if (repTime>50){
        newtonVaild=false;
        return iniValue;}
    if (newtonVaild)
        return newton_rep(nexti,repTime+1);
    return 0;
}
double solve_x::getK(double x_value,double b1)
{
    double dx=0.00001;
    equFunc.expr2value(x_value+dx);
    double b2=equFunc.value;
    if (b2==b1)
        newtonVaild=false;
    //cout<<"k="<<(b2-b1)/dx<<";b0="<<b1<<endl;
    return ((b2-b1)/dx);
}


//公用函数

//用于处理问题的函数，将向用户指明表达式产生的问题。
void errorExit(string tipStr,int errorCode)
{
    if (!solveMode)
        cout<<("    "+errorList[errorCode]+tipStr)<<endl;
}



//将字符串转换为实数的过程。strtod和atof精度不大行= =
double convertToFloat(string floatStr)
{
    double floatNum=0;
    int dotPos,ePos;
    for(dotPos=0;(dotPos<floatStr.length()&&floatStr[dotPos]!='.');dotPos++);
    for(ePos=dotPos;(ePos<floatStr.length()&&floatStr[ePos]!='e'&&floatStr[ePos]!='E');ePos++);
    string intNum,smallNum,expNum;
    intNum.assign(floatStr,0,dotPos);
    if (dotPos==floatStr.length())
        smallNum="";
    else
        smallNum.assign(floatStr,dotPos+1,ePos-dotPos-1);
    if (ePos==floatStr.length())
        expNum="";
    else
        expNum.assign(floatStr,ePos+1,floatStr.length()-ePos-1);
    const char* int_c=intNum.c_str(),*small_c=smallNum.c_str(),*exp_c=expNum.c_str();
    int exper=atof(exp_c)-smallNum.length();
    double exp10=pow(10,exper);
    floatNum=((double)(atof(int_c)*pow(10,smallNum.length())+(double)(atof(small_c))))*exp10;
    return floatNum;
}

//使用varL,func提供常用常数,函数
//部分常量使用了xkcd.com/1047提供的速算法。
void commonLibInit()
{
    //i用于隐藏created操作。
    int i=-MEMO_MAX;
    //constLib INIT
    constLib.newVar("pi=3.1415926535",i);
    constLib.newVar("exp=2.718281828",i);
    constLib.newVar("g_=6+ln(45)",i);
    constLib.newVar("R=(exp+1)*sqrt(5)",i);
    constLib.newVar("NA=69^pi^sqrt(5)",i);
    constLib.newVar("G_=1/exp^(pi-1)^(pi+1)",i);
    constLib.newVar("h_=1/30^pi^exp",i);
    i=-MEMO_MAX;
    //funcLib INIT
    funcLib.newFunc("d2r(x)=x/180*pi",i);
    funcLib.newFunc("r2d(x)=x/pi*180",i);
    funcLib.newFunc("invsqrt(x)=1/sqrt(x)",i);
}

//对输入的等式进行分流
void varOrFunc(string str,int& lognum)
{
    int ePos=-1;
    for(int i=0;i<str.length();i++)
        if (str[i]=='=')
        {
            ePos=i;
            break;
        }
    //Vaild Dect
    if (ePos==-1)
        errorExit(str,0);
    else
    {
        string name;
        name.assign(str,0,ePos);
        //lP,rP检测括号存在
        int lP,rP;
        for(lP=0;(lP<name.length())&&(name[lP]!='(');lP++);
        for(rP=lP;(rP<name.length())&&(name[rP]!=')');rP++);
        if ((lP==name.length())||(lP==name.length()))
            varList.newVar(str,lognum);
        else
            funcList.newFunc(str,lognum);
    }
}

bool theSame(string& s1,int sp1,string s2)
{
    bool same=true;
    int i;
    for(i=0;i<s2.length();i++)
        if (s1[sp1+i]!=s2[i])
            same=false;
    if (((s1[sp1-1]>='A')&&(s1[sp1-1]<='Z'))||((s1[sp1-1]>='a')&&(s1[sp1-1]<='z'))||(s1[sp1-1]=='_'))
        same=false;
    if (((s1[sp1+i]>='A')&&(s1[sp1+i]<='Z'))||((s1[sp1+i]>='a')&&(s1[sp1+i]<='z'))||(s1[sp1+i]=='_'))
        same=false;
    return same;
}
//处理冲突变量的问题：将表达式中自变量名字替换为_VAR_.
void replaceVarName(string& replaced,string& kword,string nword)
{
    string doneStr="";
    //字符串的独立性检测预处理
    replaced=" "+replaced+" ";
    for(int i=1;i<replaced.length()-kword.length();i++)
        if (theSame(replaced,i,kword))
        {
            doneStr=doneStr+nword;
            i+=kword.length()-1;
        }
        else doneStr=doneStr+replaced[i];
    replaced=doneStr;
}
//辅助函数Round
double round(double r)
{
    return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}
//辅助函数cmp
int cmp( const void *a , const void *b )
{
return *(double *)a > *(double *)b ? 1 : -1;
}
bool isAlmost(double thefloat,bool defaultInt,long integer,double deltaX)
{
    if (defaultInt)
        integer=(long)round(thefloat);
    if ((thefloat<integer+deltaX)&&(thefloat>integer-deltaX))
        return true;
    return false;
}
bool inList(double List[],int Len,double Value)
{
    double dx=0.00001;
    bool inlist=false;
    for(int i=0;i<Len;i++)
        if ((List[i]-Value>-dx)&&(List[i]-Value<dx))
            inlist=true;
    return inlist;
}
//正态分布产生随机数，使得更多返回0附近的解。
int nrand(double miu,double sigma)
{
    double np_A=1/sqrt(2*3.1415926)/sigma;
    double np_B=-0.5/sigma/sigma;
    int n;
    double p,np;
    do
    {
        n=(rand()-RAND_MAX/2);
        p=(double)rand()/RAND_MAX;
        np=np_A*exp(np_B*(n-miu)*(n-miu));
    }
    while (p>np);
    return n;
}
