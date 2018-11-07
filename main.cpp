#include "ReadingS.h"

int isKeyword(string inStr);
void getline2(int type,int lognum,string& str);
void helpPrint();

int main()
{
    //For FUN:)
    cout<<"M_CALCULATOR beta 0.0.6 (v0.0.2:2c5fed8) [MSC v.1900 64 bit (AMD64)] on win32"<<endl;
    cout<<"Type 'help' for more information."<<endl;
    //VAR INIT
    string str="";
    string logOut[100]={""};
    double ansOut[100]={0};
    int status=1;
    int lognum=1;
    commonLibInit();
    //MAIN LOOP
    while (status!=4)
    {
        solveMode=false;
        int possKey=0;
        switch (status)
            {
            case 1:
                {
                    getline2(1,lognum,str);
                    possKey=isKeyword(str);
                    logOut[lognum]=str;
                    if (possKey)
                        break;
                    bool isEqualInIt=false;
                    for(int i=0;i<str.length();i++)
                        if (str[i]=='=')
                            isEqualInIt=true;
                    if (isEqualInIt)
                        varOrFunc(str,lognum);
                    else
                    {
                        ReadingS myMain(str);
                        double ans=-1;
                        ans=myMain.calculate();
                        if (!myMain.isInvaild)
                        {
                            //SAVE OUT
                            ansOut[lognum]=ans;
                            //PRINT
                            cout<<endl<<"calc Out["<<lognum<<"] >>> ans = "<<setprecision(15)<<ans<<endl<<endl;
                            varList.setValue(0,var("ans",ans));
                            lognum++;
                        }
                        else
                            cout<<endl<<"calc Out["<<lognum<<"]:无法计算"<<endl<<endl;
                    }
                    break;
                }
            case 2:
                {
                    getline2(2,lognum,str);
                    possKey=isKeyword(str);
                    if (possKey)
                        break;
                    cout<<"Undefined Num Is:";
                    string ux;
                    getline2(-1,lognum,ux);
                    solve_x equ=solve_x(str,ux);
                    if (!equ.isInvaild)
                    {
                        equ.solveMain();
                        lognum++;
                    }
                    cout<<endl;
                    break;
                }
            }
        if (possKey)
        {
            if (possKey==3)
                helpPrint();
            else if(possKey==6)
            {
                int pos=str.find('-');
                    if (pos==string::npos)
                        cout<<endl<<"No arguments for "<<keyword[6]<<endl<<"With  -v prints all the calculation and equation;"<<endl
                            <<"With -f prints all the variances and function."<<endl;
                    else
                    {
                        string ch;ch.assign(str,pos+1,str.length()-1);
                        int i=0;
                        for(i=ch.length()-1;ch[i]==' ';i--);
                        ch.assign(ch,0,i+1);
                        if (ch=="v")
                            varList.display();
                        else if (ch=="f")
                            funcList.display();
                        else
                            cout<<endl<<"Invaild arguments:"<<ch<<endl<<"With  -v prints all the calculation and equation;"<<endl
                            <<"With -f prints all the variances and function."<<endl;
                    }
            }
            else if (possKey==5)
            {
               int pos=str.find('-');
                    if (pos==string::npos)
                        cout<<endl<<"No arguments for "<<keyword[5]<<endl
                        <<"Input a filename(with path accepted)so that the log_file can be saved successfully."<<endl<<endl;
                    else
                    {
                        string fn;fn.assign(str,pos+1,str.length()-1);
                        fn=fn+".txt";
                        ofstream fileOut;
                        fileOut.open(fn.c_str());
                        if (!fileOut)
                            cout<<endl<<"Invaild arguments for "<<keyword[5]<<":"<<endl
                            <<"Input a vaild filename"<<endl;
                        else
                            for(int i=1;i<lognum;i++)
                                fileOut<<"IN [ "<<i<<" ] = "<<logOut[i]<<endl<<"OUT [ "<<i<<" ] = "<<setprecision(15)<<ansOut[i]<<endl;
                        fileOut.close();
                    }
            }
            else if (possKey==7)
            {
                int pos=str.find('-');
                    if (pos==string::npos)
                        cout<<endl<<"No arguments for "<<keyword[7]<<endl
                        <<"Input a filename(with path accepted)so that the log_file can be saved successfully."<<endl<<endl;
                    else
                    {
                        string fn;fn.assign(str,pos+1,str.length()-1);
                        int i=0;
                        for(i=fn.length()-1;fn[i]==' ';i--);
                        fn.assign(fn,0,i+1);
                        if (varList.isInList(fn)>=0){
                            varList.delVar(varList.isInList(fn));
                            errorExit(fn,14);}
                        else if (funcList.isInList(fn)>=0){
                            funcList.delFunc(funcList.isInList(fn));
                            errorExit(fn,14);}
                        else
                            cout<<"Invaild argument:"<<fn<<endl<<"It doesn't exist in Variance or Function Dictionary."<<endl;
                            cout<<endl;
                    }
            }
            else status=(possKey);
        }
    }
    return 0;
}

//判断输入字符包含关键字。
int isKeyword(string inStr)
{
    int flag=0;
    for(int i=0;i<8;i++)
        if (inStr.find(keyword[i])!=string::npos)
            flag=i;
    return flag;
}
//确保不读入空串
void getline2(int type,int lognum,string& str)
{
    str="";
    while(str=="")
    {
        if (type!=-1)
            cout<<keyword[type]<<" In["<<lognum<<"] >>>";
        getline(cin,str);
    }
}
void helpPrint()
{
    /*帮助文字内容.建议放入文本文档中读取*/
    ifstream helpfiles;
    helpfiles.open("help.dll");
    while (!helpfiles.eof())
    {
        string thisline;
        getline(helpfiles,thisline);
        cout<<thisline<<endl;
    }
    cout<<endl;
    helpfiles.close();
}
