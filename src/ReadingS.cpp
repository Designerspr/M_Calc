//ReadingS.h�ṩ��ReadingS�࣬�ṩ��������һϵ�з������ܹ�������ı��ʽ��ֵ��
//Author: SpringCityStudio
//Date:2017.12.5(Since)
//second update: finished the rawToRPN function,RPNcalc function(2017.12.6 20:21)
//Third Update:add the defination about var(2017.12.19,17:21)��"ans" function(12.20,18:33)
//Fourth Update:add the function,and bugs fixed(mainly).(12.21 12:37+12.22 00:02)
//Fifth Update:add equation solveing��bug fixed(12.27,10:44)
#include "ReadingS.h"

//Class ReadingS
bool solveMode=false;
string errorList[]={"error0���Ƿ��ı��ʽ��",
                              "error1���������;",
                              "error2:����ȷ������Ƕ��;",
                              "error3:���ʽ�߼�����",
                              "error4:��������󣨵�����ָ����;",
                              "error5:�׳��������(���󣬹�С�����߷������ĵ�����",
                              "error6:δ����ĺ�����",
                              "error7:�Ƿ��Ķ�������",
                              "error8:�Ը�������",
                              "Warning0�������ظ������Ǵ�ǰ�����",
                              "error10:�Դ˷����޷���ֵ:",
                              "error11:�����б����ظ����Ա�����",
                              "error12:�޷����������̣�",
                              "error13:��������������(inf)�������(nan)��",
                              "Warning1:��ɾ�����������"};
string keyword[]={"","calc","solve","help","exit","logout","print","del"};
//mathFuncLib�����˽���math��ĺ�������
//constLib��funcLib�ṩ��Ԥ�õĳ��ú�����������
//varList�ṩ�����ڴ����飬funcList�ṩ�����ڴ�����
string mathFuncLib[]={"sin","cos","tan","arcsin","arccos","arctan","sinh","cosh","tanh","abs","sqrt","ln","log","inf","nan"};
varL varList;
varL constLib;
funcL funcList;
funcL funcLib;
//������
ReadingS::ReadingS(string str)
        {
            //����0������ͷ���ŵĴ���
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

//����get�ַ����ķ���
string ReadingS::getRawStr()
        {
            return rawStr;
        }
string ReadingS::getRPNStr()
        {
            return stringRPN;
        }

// �ַ���Ԥ����������Ҫ��Բ��淶�����������ӿո����Ϊ�������沨��ʽ�Ĵ���
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

// ת���ַ���Ϊ�沨��ʽ�����ں����Ĵ�������׺���ʽ�ĺ��Ĵ���ģ�顣
void ReadingS::rawToRPN( )
        {
            string RPNout="",rawCopy=rawStr;
            string symStack[100];//���Ŷ�ջ
            int top=0;//ջ��ָ��
            string nex;
            int staCode;

            symStack[0]="#";//ջ��ռλ�������������㣬�����������ڼ򻯴��롣
            readNext(rawCopy,nex,staCode);
            while (staCode!=-1)
            {
                //�����׳��쳣
                if ((isInvaild)||(staCode==0))
                {
                    errorExit(nex,0);
                    break;
                }
                //�������ţ���Ԫ��������������ȼ�����ߵģ�ֱ���������ַ���β�����ɡ�
                if ((staCode==3)||(staCode==4)||(staCode==5))
                    RPNout=RPNout+nex;
                else if (staCode==2)
                {
                    //�ȵ����������ȵķ���
                    while (!latterBetter(symStack[top],nex))
                    {
                        RPNout=RPNout+symStack[top];
                        symStack[top]="";
                        top--;
                    }
                    // �����ַ���ջ��
                    top++;
                    symStack[top]=nex;
                }
                else
                {
                    //����ֱ�ӽ���RPNʽ���С�
                    RPNout=RPNout+nex;
                }
                //��ȡ��һ��
                readNext(rawCopy,nex,staCode);
            }
            //����ջ��ķ���
            while (top!=0)
            {
                RPNout=RPNout+symStack[top];
                top--;
            }
            stringRPN=RPNout;
        }

//�����沨��ʽ���Ǻ�׺���ʽ�ĺ��Ĵ���ģ�顣
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
                nex.assign(nex,0,nex.length()-1);//����ĩβ�ո�...(����readNext���ַ���Ҫ��ͬ)
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

//���㲽��
double ReadingS::calculate()
        {
            this->strTretment();
            this->rawToRPN();
            double numout=this->calcRPN();
            return numout;
        }

// �жϸ����ַ��Ƿ������������
bool ReadingS::isSymbol(char sym)
        {
            char symbolList[6]={'+','-','*','/','^','!'};
            bool flag=false;
            for(int i=0;i<6;i++)
                if (sym==symbolList[i])
                    flag=true;
            return flag;
        }

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
void ReadingS::readNext(string& rawStr,string& numStr,int& type)
        {
            //VAR
            int s1=0,s2=0;
            numStr="";
            type=0;
            for(s1=0;((rawStr[s1]==' ')&&(s1<(rawStr.length())));s1++);
            //�����Ѿ������򷵻�-1.
            if (s1==rawStr.length())
                type=-1;
            else
            {
                numStr[0]=rawStr[s1];
                //���ݵ�һλ�ַ��ж���������
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
                //��������ִ�в�ͬ���и��
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
                            //��Ϊ���ʽ
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
                            //��Ϊ����������RPN����ʱ���Բ�����������⣩
                            else
                            {
                                for(s2=s1;((rawStr[s2]!=' ')&&(s2<(rawStr.length())));s2++);
                                numStr.assign(rawStr,s1,s2-s1);
                                //12.20����
                                //�����ȣ��������ж�
                                if (varList.isInList(numStr)>=0)
                                {
                                    numStr=varList.getValue(varList.isInList(numStr));
                                    type=1;
                                }
                                //��Σ��Դ�������
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

        //������һ���������ŵı��ʽβ��λ�á�
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


        //���ʽ��ִֵ�м���ĵײ㲽�衣
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
                    //�����Լ��
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
                    //FOR TEST��cout<<"nex="<<nex<<";"<<endl<<"Value="<<numStack[top-1]<<endl;
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

        //�Ƚ���������������ȼ���
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
//������
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

//����Var������ࡣ
//������
varL::varL(){
    varNum=0;}

//���ӱ�����
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

//ɾ��ָ��������
void varL::delVar(int delPos)
    {
        for(int i=delPos;i<varNum;i++)
            varlist[i]=varlist[i+1];
        varNum--;
    }

//�жϸ��������Ƿ����б��С�
/* -1������
    0�����ڱ�ͷ��һ���Ӧ��δ֪������Ans��
    n>0�������б�һ�������*/
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

//�趨ֵ
void varL::setValue(int pos,var inVar)
{
    varlist[pos]=inVar;
}

//����ֵ
string varL::getValue(int pos)
{
        stringstream str;
        string outputStr;
        //���������������ʽ��
        //��Ȼ��������������ڸ����Bug��������ʱ�������߾ȹ�= =
        //Q:��ת��ʱ�����Ȳ���
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
//���ȫ��
void varL::display()
{
    for(int i=0;i<varNum+1;i++)
        cout<<i<<":NAME="<<varlist[i].name<<";VALUE="<<varlist[i].value<<";"<<endl;
}

//class func

//���ع�����*3
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
            //�����ͻ���������⣺�����ʽ���Ա��������滻Ϊ_var_func.name.
            string new_ux="__VAR__"+name;
            replaceVarName(expr,ux,new_ux);
            ux=new_ux;
            //�Ϸ��Բ��ԣ���Ҫ�����Ƿ�ʹ����δ����ı�����������
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
//������
funcL::funcL(){
    funcNum=0;}
//����Func��
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
//ɾ��ָ��������
void funcL::delFunc(int delPos)
{
    for(int i=delPos;i<funcNum;i++)
        funclist[i]=funclist[i+1];
    funcNum--;
}
//�жϸ��������Ƿ����б��С�
/* -1������
0�����ڱ�ͷ��һ���Ӧ��δ֪������Ans��
n>0�������б�һ�������*/
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
//����������set,get,display.
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
//������
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
    solveMode=true;//���ز���Ҫ���쳣���
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
//ţ�ٵ��������ģ���iniValueΪ��ֵ�������𰸡�
double solve_x::newton_rep(double iniValue,int repTime)
{
    equFunc.expr2value(iniValue);
    double b=equFunc.value;
    //���������ڶ�����ж�
    equFunc.expr2value(iniValue-0.000005);
    double bl=equFunc.value;
    equFunc.expr2value(iniValue+0.000005);
    double br=equFunc.value;
    if (bl*br<=0)
        return iniValue;
    double k=getK(iniValue,b);
    //���ĵ�����ϵʽ
    double nexti=iniValue-b/k;
    //���޽�ķ���ѭ������
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


//���ú���

//���ڴ�������ĺ����������û�ָ�����ʽ���������⡣
void errorExit(string tipStr,int errorCode)
{
    if (!solveMode)
        cout<<("    "+errorList[errorCode]+tipStr)<<endl;
}



//���ַ���ת��Ϊʵ���Ĺ��̡�strtod��atof���Ȳ�����= =
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

//ʹ��varL,func�ṩ���ó���,����
//���ֳ���ʹ����xkcd.com/1047�ṩ�����㷨��
void commonLibInit()
{
    //i��������created������
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

//������ĵ�ʽ���з���
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
        //lP,rP������Ŵ���
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
//�����ͻ���������⣺�����ʽ���Ա��������滻Ϊ_VAR_.
void replaceVarName(string& replaced,string& kword,string nword)
{
    string doneStr="";
    //�ַ����Ķ����Լ��Ԥ����
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
//��������Round
double round(double r)
{
    return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
}
//��������cmp
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
//��̬�ֲ������������ʹ�ø��෵��0�����Ľ⡣
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
