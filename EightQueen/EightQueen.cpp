#include<stdio.h>
#include<iostream>
#include<algorithm>
using namespace std;

int path[8];//record the position of queen of each line
int sum = 0;//count the number of possibility

//check the position safe
int check(int i,int j,int num){

    //check column
    for(int n = 0;n < num-1;++n)
    {
        if((j + 1) == path[n])
            return 0;
    }

    //check slash
    for(int n = 0;n < num-1;++n)
    {
        if((i - n) == abs(path[n] -1 - j))
            return 0;
    }

    return 1;
}

void print()
{
    for(int i = 0;i < 8;++i)
    {
        for(int j = 0;j < 8;++j)
        {
            if((j + 1) == path[i])
                cout<<"¡ö";
            else
                cout<<"¡õ";
        }
        cout<<endl;
    }
}

void place(int p)
{
    if(p == 8){
        for(int i = 0;i < 8; ++i){
            if(check(7,i,8)){
                path[7] = i + 1;
                sum++;
                print();
                cout<<endl;
            }
        }
    }
    else{
        for(int i = 0; i < 8; ++i)
        {
            if(check(p-1,i,p))
            {
                path[p-1] = i + 1;
                place(p+1);
            }
        }
    }
}

int main()
{
    cout<<"°Ë»ÊºóÎÊÌâ\n"<<endl;
    place(1);
    cout<<"sum: "<<sum<<endl;
    return 0;
}

