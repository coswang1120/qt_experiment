#ifndef ATHLETEMATCH_H
#define ATHLETEMATCH_H

#include <QWidget>
#include<iostream>
#include<iomanip>
#include<vector>
#include<stack>
#include<queue>
#include<algorithm>
#include<list>
#include<string>
#define Indepent_Zero -1
#define Other_Zero -2
#define MAXN 1000005

using namespace std;
typedef int ElemType;

/*
 4、男女运动员最佳组合匹配系统
【问题描述】
设有N个男羽毛球运动员和N个女羽毛球运动员，现组成N对男女混合最佳组合。
 每个男运动员对每个女运动员都有一个满意度排序，用矩阵mf[0:n-1][0:n-1]表示。
 mf[i][j]表示第i个男运动员对第j个女运动员的满意度，满意度值越高，满意程度越高。
 同理，每个女运动员对每个男运动员也有一个满意度排序，用矩阵fm[0:n-1][0:n-1]表示。
 男女运动员之间的一个完全匹配称为一个组合。
【设计要求】
设计对于给定的满意度，求最佳组合的程序，使得满意度总和达到最大。
（1）采用STL的一维向量类构造构造二维向量矩阵。
（2）应用基本运算，设计算法求解。
 */
namespace Ui {
class AthleteMatch;
}

class AthleteMatch : public QWidget
{
    Q_OBJECT

public:
    explicit AthleteMatch(QWidget *parent = 0);
    ~AthleteMatch();

    enum DateTimeType{Time,Date,DateTime};
    QString getDateTime(DateTimeType type);
    int ckeckTime=0;//确认键确认次数
    int NumberNow=0;//满意度矩阵设定容量
    bool matchFlag=false;

    int NumberMax=100;//满意度矩阵最大容量

    vector<vector<ElemType> > fm; //女运动员对男运动员的满意度矩阵
    vector<vector<ElemType> > mf;//男运动员对女运动员的满意度矩阵
    vector<vector<ElemType> > sumall;

private slots:
    void on_pushButton_1_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::AthleteMatch *ui;
    void SumMatrix(vector<vector<ElemType> > &matrix1, vector<vector<ElemType> > matrix2, vector<vector<ElemType> > matrix3);
    //将男女各自满意度矩阵相加
    void Convert_Matrix(vector<vector<ElemType> > &matrix);
    //找出最大值，并将问题转变为求最小花费的最优解问题
    void SearchRow_min(vector<vector<ElemType> > &matrix, int n);
    //寻找二维向量矩阵行最小值并进行行规约
    void SearchColumn_min(vector<vector<ElemType> > &matrix, int n);
    //寻找二维向量矩阵列最小值并进行列规约
    bool Is_NoZero(vector<vector<ElemType> > matrix);
    //判断矩阵中是否还有零
    int Count_Zero(vector<vector<ElemType> > matrix, int n);
    //判断矩阵某行的零元素的个数
    int Count_RowIndepZero(vector<vector<ElemType> > matrix, int n);
    //判断矩阵每行独立零元素的个数
    int Count_IndepZero(vector<vector<ElemType> > matrix);
    //判断矩阵独立零元素的个数
    void Sign_Zero(vector<vector<ElemType> > &matrix, int N);
    //标记独立零元素和其他零元素
    void Try_Distribute(vector<vector<ElemType> > &matrix, int N);
    //试分配任务，N为效率矩阵的维数


};

#endif // ATHLETEMATCH_H
