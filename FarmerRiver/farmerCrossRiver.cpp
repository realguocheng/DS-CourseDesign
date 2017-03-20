#include<stdio.h>
#include<iostream>
#include"LinkedQueue.h"
#include<iomanip>
using namespace std;

/*分别使用深度和广度优先搜索解决农夫过河问题*/

struct Vertex{//顶点

	//在北岸时是1，在南岸时是0
	int farmer;
	int wolf;
	int sheep;
	int cabbage;

	int idx;//编号，与四位二进制对应十进制一致
};

Vertex vet[16];//16个顶点
int edge[16][16];//顶点之间的邻接数组

int binary[4];//存放二进制数
int count = 0;//对binary数组进行计数

int change = 0;//对对状态中发生的变化进行计数
int flag[16];//标记数组，若走过，则flag=1


Vertex* path_pb[16];//路径，供BFS使用
Vertex* path_pd[16];//路径，供DFS使用

int path[16];//记录前驱状态值

//十进制转换为二进制
void outputBinary(int n){

	if(n>1)
	outputBinary(n/2);

	binary[count++] = n % 2;
}

void createGraph(){//无向图
	//如果顶点i，j之间有边，则edge[i][j]=1
	int i,j;
	for(i = 0;i < 16;i++){
		for(j = 0;j < 16;j++){
			if(vet[i].farmer != vet[j].farmer){
				if(vet[i].sheep != vet[j].sheep) change++;
				if(vet[i].cabbage != vet[j].cabbage) change++;
				if(vet[i].wolf != vet[j].wolf) change++;

				if(change >= 2) edge[i][j] = 0;
				else edge[i][j] = 1;
			}
			else
				edge[i][j] = 0;

			change = 0;
		}
	}
}
//判断当前是否是安全状态
bool judgeCondition(Vertex *v){

	if((v->wolf == v->sheep)&&(v->farmer != v->sheep))
		return false;
	if((v->sheep == v->cabbage)&&(v->farmer != v->sheep))
		return false;

	return true;

}
//获取相连的下一个顶点
Vertex* getNext(int i){

	int n;
	for(n = 0;n < 16;n++){
		if(flag[n] != 1 && edge[i][n] == 1 && judgeCondition(&vet[n]))
		return &vet[n];
	}
	return 0;
}

//广度优先搜索遍历，并记录可行状态
void pathSearch_BFS(Vertex* start){

	LinkedQueue<Vertex*> Q;
	Q.Add(start);
	flag[start->idx] = 1;

	Vertex *tmp,*next;
	while(!Q.IsEmpty()){

		Vertex* v = Q.First();
		Q.Delete(tmp);

		while((next = getNext(v->idx))!= 0){
			if(flag[next->idx] == 0){

				Q.Add(next);
				flag[next->idx] = 1;
			}
			path[next->idx] = v->idx;
		}

	}
}
//深度优先搜索，递归查找
bool pathSearch_DFS(Vertex* start,Vertex* end,int& length){

	flag[start->idx] = 1;
	Vertex* u = getNext(start->idx);

	while(u){

		if(!flag[u->idx]){

			length++;
			path_pd[length] = u;
			if(u == end) return true;
			if(pathSearch_DFS(u,end,length)) return true;
			length--;
		}
		u = getNext(start->idx);
	}
	return false;
}

void printPath_BFS(Vertex* end){

	//定义一个正序存放路径的数组
	int n = 0;

	int e = end->idx;
	while(path[e] != -1){
		path_pb[n++] = &vet[e];
		e = path[e];
	}
	path_pb[n] = &vet[e];
	//打印路径
	int i;
	for(i = n;i >=0;i--)
    {
        if(path_pb[i]->farmer == 0)
            cout<<"南岸_";
        else
            cout<<"北岸_";
        if(path_pb[i]->wolf == 0)
            cout<<"南岸_";
        else
            cout<<"北岸_";
        if(path_pb[i]->sheep == 0)
            cout<<"南岸_";
        else
            cout<<"北岸_";
        if(path_pb[i]->cabbage == 0)
            cout<<"南岸"<<endl;
        else
            cout<<"北岸"<<endl;
        cout<<endl;
    }
		//cout<<path_pb[i]->farmer<<"  "<<path_pb[i]->wolf<<"  "<<path_pb[i]->sheep<<"  "<<path_pb[i]->cabbage<<endl;
}

void printPath_DFS(int length){
	//打印路径

	int i;
	for(i = 0;i <= length;i++)
    {
        if(path_pd[i]->farmer == 0)
            cout<<"南岸_";
        else
            cout<<"北岸_";
        if(path_pd[i]->wolf == 0)
            cout<<"南岸_";
        else
            cout<<"北岸_";
        if(path_pd[i]->sheep == 0)
            cout<<"南岸_";
        else
            cout<<"北岸_";
        if(path_pd[i]->cabbage == 0)
            cout<<"南岸"<<endl;
        else
            cout<<"北岸"<<endl;
        cout<<endl;
    }
	//cout<<path_pd[i]->farmer<<"  "<<path_pd[i]->wolf<<"  "<<path_pd[i]->sheep<<"  "<<path_pd[i]->cabbage<<endl;
}
void init(){
	//binary数组初始化
	int i,j;
	for(i = 0;i < 4;i++)
		binary[i] = 0;

	//顶点初始化
	for(i = 0;i < 16;i++){
		if(i < 2) count = 3;
		else if(i > 1 && i < 4) count = 2;
		else if(i > 3 && i < 8) count = 1;
		else count = 0;
		outputBinary(i);
		vet[i].farmer = binary[0];
		vet[i].wolf = binary[1];
		vet[i].sheep = binary[2];
		vet[i].cabbage = binary[3];
		vet[i].idx = i;
		//为循环使用binary需要重新置空
		for(j = 0;j < 4;j++)
		binary[j] = 0;
	}

	for(i = 0;i < 16;i++){
		path_pd[i] = 0;
		path_pb[i] = 0;
		path[i] = -1;
	}

}

int main(){

	init();
	createGraph();

	cout<<"农夫过河问题\n"<<endl;
	cout<<"农夫-狼-羊-白菜"<<endl;
	cout<<"\n1.使用深度优先搜索查找\n"<<endl;
	int length = 0;
	pathSearch_DFS(&vet[0],&vet[15],length);
	path_pd[0] = &vet[0];
	printPath_DFS(length);

	//置空flag
	int i;
	for(i = 0;i < 16;i++)
		flag[i] = 0;

	cout<<"\n2.使用广度优先搜索查找\n"<<endl;
	pathSearch_BFS(&vet[0]);
	printPath_BFS(&vet[15]);

	return 0;
}
