#include "athletematch.h"
#include "ui_athletematch.h"
#include <QtSql>
#include <QtGui>
#include <QDebug>
#include <QMessageBox>
#include <QListWidget>

//using namespace std;

AthleteMatch::AthleteMatch(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AthleteMatch)
{
    ui->setupUi(this);

    QListWidgetItem *message=new QListWidgetItem;
    QString currentdate=getDateTime(DateTime);
    //窗口显示插入信息
    message->setText(QString("=========!!!欢迎使用男女运动员匹配系统!!!========="));
    message->setTextAlignment(Qt::AlignCenter);
    ui->listView->addItem(message);
    ui->listView->addItem(QString("现在时间为%1！").arg(currentdate));
    ui->listView->addItem(QString(">>请在左边输入框内输入男女运动员的各自的人数（男女人数相等，所以您只需输入一个数）"));

}

AthleteMatch::~AthleteMatch()
{
    delete ui;
}

QString AthleteMatch::getDateTime(AthleteMatch::DateTimeType type)
{
    QDateTime datetime=QDateTime::currentDateTime();
    QString date=datetime.toString("yyyy-MM-dd");
    QString time=datetime.toString("hh:mm");
    QString dateAndTime=datetime.toString("yyyy-MM-dd dddd hh:mm");
    if(type==Date) return date;
    else if(type==Time) return time;
    else return dateAndTime;
}

void AthleteMatch::on_pushButton_1_clicked()
{
    QString number=ui->lineEdit->text();
    QString numView;
    if(ckeckTime==0)
    {
        NumberNow=number.toInt();//记录要求的运动员数
        ui->listView->addItem(QString("已记录要求的男女运动员数分别为%1！").arg(NumberNow));
        ui->listView->addItem(QString(">>接下来,请依次输入男运动员对女运动员的满意度:"));
        ckeckTime++;
        mf.resize(NumberNow);
        for (int i = 0; i < NumberNow; ++i)
                mf[i].resize(NumberNow);
        fm.resize(NumberNow);
        for (int i = 0; i < NumberNow; ++i)
                fm[i].resize(NumberNow);
        sumall.resize(NumberNow);
        for (int i = 0; i < NumberNow; ++i)
                sumall[i].resize(NumberNow);
        //cout<<"Row:"<<mf.size()<<"  Column:"<<mf[0].size()<<endl;
        ui->listView->addItem(QString("请输入第%1个男运动员对每个女运动员的满意度:(一行%2个，以空格隔开)\n注意要在最后一个数字后面加一个空格!").arg(ckeckTime).arg(NumberNow));
    }else if(ckeckTime>0&&ckeckTime<=2*NumberNow)
    {
        //ui->listView->addItem(QString("%1:"+number).arg(number.length()));
        int i=0,j=0;
        QString num;
        while(j<NumberNow)
        {
            if(number[i]!=' ') num=num+number[i];
            else {
                if(ckeckTime<=NumberNow) mf[ckeckTime-1][j]=num.toInt();
                else if(ckeckTime<=2*NumberNow) fm[ckeckTime-1-NumberNow][j]=num.toInt();
                j++;
                num=' ';
            }
            i++;
        }
        ckeckTime++;
        if (ckeckTime<=NumberNow) ui->listView->addItem(QString(">>请输入第%1个男运动员对每个女运动员的满意度:(一行%2个，以空格隔开)\n注意要在最后一个数字后面加一个空格!").arg(ckeckTime).arg(NumberNow));
        else if (ckeckTime<=2*NumberNow) ui->listView->addItem(QString(">>请输入第%1个女运动员对每个男运动员的满意度:(一行%2个，以空格隔开)\n注意要在最后一个数字后面加一个空格!").arg(ckeckTime-NumberNow).arg(NumberNow));
        else {
            ui->listView->addItem(QString(">>共记录%1组数据,可以开始匹配!").arg(ckeckTime));
            matchFlag=true;
        }
//        for(int i = 0; i < sumall.size(); i++)
//        {
//            for(int j = 0; j < mf.size(); j++)
//                numView=numView+QString::number(mf[i][j],10)+' ';
//            ui->listView->addItem(numView);
//            numView=' ';
//        }
    }else {
        ui->listView->addItem(QString(">>可以开始匹配!"));
        matchFlag=true;
    }

}

void AthleteMatch::on_pushButton_2_clicked()
{
    if (matchFlag)
    {
        QString numView;
        ui->listView->addItem(QString(">>开始进行匹配......"));
        SumMatrix(sumall, mf, fm);
        Convert_Matrix(sumall);
        for(int i = 0; i < sumall.size(); i++)
            SearchRow_min(sumall, i);
        for(int j = 0; j < sumall.size(); j++)
            SearchColumn_min(sumall, j);
        ui->listView->addItem(QString("最终的任务详细分配情况如下:"));
        Try_Distribute(sumall, NumberNow);
//        for(int i = 0; i < sumall.size(); i++)
//        {
//            for(int j = 0; j < sumall.size(); j++)
//                numView=numView+QString::number(sumall[i][j],10)+' ';
//            ui->listView->addItem(numView);
//            numView=' ';
//        }
    }else ui->listView->addItem(QString("还未录入数据,不能匹配.请录入相关数据后再试!"));
}

void AthleteMatch::SumMatrix(vector<vector<ElemType> > &matrix1, vector<vector<ElemType> > matrix2, vector<vector<ElemType> > matrix3)
{
    int i, j;
    for(i = 0; i < matrix2.size(); i++){
       for(j = 0; j < matrix2[i].size(); j++){
           matrix1[i][j] = matrix2[i][j]+matrix3[j][i];  // 将男i与女j的满意度总和放入矩阵1中
       }
    }
}

void AthleteMatch::Convert_Matrix(vector<vector<ElemType> > &matrix)
{
    int i, j;
    ElemType max = 0;
    for(i = 0; i < matrix.size(); i++){      //找出矩阵中的最大值元素
        for(j = 0; j < matrix[i].size(); j++){
            if(max < matrix[i][j])
                max = matrix[i][j];
        }
    }
    for(i = 0; i < matrix.size(); i++){      //将矩阵中每个元素变为 最大值-原先的元素
        for(j = 0; j < matrix[i].size(); j++){
            matrix[i][j] = max - matrix[i][j];  //矩阵中所有元素皆非负
        }
    }
}

void AthleteMatch::SearchRow_min(vector<vector<ElemType> > &matrix, int n)
{
    ElemType min = matrix[n][0];
    int j;
    vector<ElemType>::iterator i;
    for(i = matrix[n].begin(); i < matrix[n].end(); i++)
        if(*i < min)
            min = *i;
    for(j = 0; j < matrix[n].size(); j++){
        matrix[n][j] -= min;
    }
}

void AthleteMatch::SearchColumn_min(vector<vector<ElemType> > &matrix, int n)
{
    ElemType min = matrix[0][n];
    vector<vector<ElemType> >::iterator i;
    int j;
    for(i = matrix.begin(); i < matrix.end(); i++)
        if((*i)[n] < min)
            min = (*i)[n];
    for(j = 0; j < matrix.size(); j++){
        matrix[j][n] -= min;
    }
}

bool AthleteMatch::Is_NoZero(vector<vector<ElemType> > matrix)
{
    int i, j;
   for(i = 0; i<matrix.size(); i++){
       for(j = 0; j<matrix[0].size(); j++)
           if(matrix[i][j] == 0)
               return false;
   }
   return true;
}

int AthleteMatch::Count_Zero(vector<vector<ElemType> > matrix, int n)
{
    int count = 0, i;
   for(i = 0; i < matrix.size(); i++)
       if(matrix[n][i] == 0)
           count++;
   return count;
}

int AthleteMatch::Count_RowIndepZero(vector<vector<ElemType> > matrix, int n)
{
    int count = 0, i;
    for(i = 0; i < matrix[n].size(); i++)
        if(matrix[n][i] == Indepent_Zero)
            count++;
    return count;
}

int AthleteMatch::Count_IndepZero(vector<vector<ElemType> > matrix)
{
    int sum = 0, i;
    for(i = 0; i < matrix.size(); i++)
        sum += Count_RowIndepZero(matrix, i);
    return sum;
}

void AthleteMatch::Sign_Zero(vector<vector<ElemType> > &matrix, int N)
{
    int a[N], i, j, temp, q, count;  //a[n]用于存放矩阵每行零元素个数
   for(i = 0; i < matrix.size(); i++)
       for(j = 0; j < matrix[i].size(); j++)
           if(matrix[i][j] == Indepent_Zero || matrix[i][j] == Other_Zero)
               matrix[i][j] = 0;
   while(!Is_NoZero(matrix)){      //标记独立零元素和其他零元素
       for(i = 0; i < matrix.size(); i++){
           a[i] = Count_Zero(matrix, i);
       }
       for(i = 0; i < matrix.size(); i++){
           temp = i;
           count = 0;
           for(j = 0; j < matrix.size(); j++){
               if(a[temp] > a[j]){
                   temp = j;
               }
           }
           for(q = 0; q < matrix.size(); q++){
               if(matrix[temp][q] == 0 && count == 0){
                   matrix[temp][q] = Indepent_Zero;
                   // cout<<matrix[temp][q]<<endl;
                   count++;
                   for(j = 0; j < matrix[q].size(); j++){
                       if(matrix[j][q] == 0){
                           matrix[j][q] = Other_Zero;
                           a[j]--;
                         //  cout<<matrix[j][q]<<endl;
                       }
                   }
               }
               else if(matrix[temp][q] == 0 && count > 0){
                   matrix[temp][q] = Other_Zero;
               }
           }
           a[temp] = N+5;     //不重复运算该行
       }
   }
}

void AthleteMatch::Try_Distribute(vector<vector<ElemType> > &matrix, int N)
{
    int i, j, sum = 0;//signrow, signcolumn用于标记矩阵的行列
    if(Count_IndepZero(matrix) == N){
        cout<<"这群男女运动员间的一个最佳搭配为："<<endl;
        for(i = 0; i < matrix.size(); i++){
            for(j = 0; j < matrix[i].size(); j++){
                if(matrix[i][j] == Indepent_Zero)
                {
                    ui->listView->addItem(QString("第%1个男运动员搭配第%2个女运动员").arg(i+1).arg(j+1));
                    cout<<"第"<<i+1<<"个男运动员"<<"配第"<<j+1<<"个女运动员"<<endl;
                }
            }
        }
    }
    else {
        int signrow[1000] = {0}, signcolumn[1000] = {0};
        for(i = 0; i < matrix.size(); i++){        //无独立零行标记
            if(Count_RowIndepZero(matrix, i) == 0){
                signrow[i] = 1;
                for(j = 0; j < matrix[i].size(); j++){     //无独立零行有零列标记
                    if(matrix[i][j] == Other_Zero)
                        signcolumn[j] = 1;
                }
            }
        }
        for(j = 0; j < matrix.size(); j++){     //标记列有独立零行标记
            if(signcolumn[j] == 1){
                for(i = 0; i < matrix.size(); i++){
                    if(matrix[i][j] == Indepent_Zero)
                        signrow[i] = 1;
                }
            }
        }
        int min = MAXN;     //再次进行规约
        for(i = 0; i < matrix.size(), signrow[i]==1; i++){  //找出未被画线部分的最小值
            for(j = 0; j < matrix[i].size(), signcolumn[j]!=1; j++){
                if(matrix[i][j]< min)
                    min = matrix[i][j];
            }
        }
        for(i = 0; i < matrix.size(); i++){   //调整行减最小值， 调整列加最小值
            for(j = 0; j < matrix[i].size(); j++){
                if(signcolumn[j] == 1 && signrow[i] != 1)
                    matrix[i][j] += min;
                else if(signcolumn[j] != 1 && signrow[i] == 1)
                    matrix[i][j] -= min;
            }
        }
        Sign_Zero(matrix, N);  //再次标记独立零与其他零元素
        Try_Distribute(matrix, N); //递归求解
    }
}
