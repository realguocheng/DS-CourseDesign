#include<stdio.h>
#include<iostream>
#include"LinkedQueue.h"
#include<iomanip>
using namespace std;

/*�ֱ�ʹ����Ⱥ͹�������������ũ���������*/

struct Vertex{//����

	//�ڱ���ʱ��1�����ϰ�ʱ��0
	int farmer;
	int wolf;
	int sheep;
	int cabbage;

	int idx;//��ţ�����λ�����ƶ�Ӧʮ����һ��
};

Vertex vet[16];//16������
int edge[16][16];//����֮����ڽ�����

int binary[4];//��Ŷ�������
int count = 0;//��binary������м���

int change = 0;//�Զ�״̬�з����ı仯���м���
int flag[16];//������飬���߹�����flag=1


Vertex* path_pb[16];//·������BFSʹ��
Vertex* path_pd[16];//·������DFSʹ��

int path[16];//��¼ǰ��״ֵ̬

//ʮ����ת��Ϊ������
void outputBinary(int n){

	if(n>1)
	outputBinary(n/2);

	binary[count++] = n % 2;
}

void createGraph(){//����ͼ
	//�������i��j֮���бߣ���edge[i][j]=1
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
//�жϵ�ǰ�Ƿ��ǰ�ȫ״̬
bool judgeCondition(Vertex *v){

	if((v->wolf == v->sheep)&&(v->farmer != v->sheep))
		return false;
	if((v->sheep == v->cabbage)&&(v->farmer != v->sheep))
		return false;

	return true;

}
//��ȡ��������һ������
Vertex* getNext(int i){

	int n;
	for(n = 0;n < 16;n++){
		if(flag[n] != 1 && edge[i][n] == 1 && judgeCondition(&vet[n]))
		return &vet[n];
	}
	return 0;
}

//���������������������¼����״̬
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
//��������������ݹ����
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

	//����һ��������·��������
	int n = 0;

	int e = end->idx;
	while(path[e] != -1){
		path_pb[n++] = &vet[e];
		e = path[e];
	}
	path_pb[n] = &vet[e];
	//��ӡ·��
	int i;
	for(i = n;i >=0;i--)
    {
        if(path_pb[i]->farmer == 0)
            cout<<"�ϰ�_";
        else
            cout<<"����_";
        if(path_pb[i]->wolf == 0)
            cout<<"�ϰ�_";
        else
            cout<<"����_";
        if(path_pb[i]->sheep == 0)
            cout<<"�ϰ�_";
        else
            cout<<"����_";
        if(path_pb[i]->cabbage == 0)
            cout<<"�ϰ�"<<endl;
        else
            cout<<"����"<<endl;
        cout<<endl;
    }
		//cout<<path_pb[i]->farmer<<"  "<<path_pb[i]->wolf<<"  "<<path_pb[i]->sheep<<"  "<<path_pb[i]->cabbage<<endl;
}

void printPath_DFS(int length){
	//��ӡ·��

	int i;
	for(i = 0;i <= length;i++)
    {
        if(path_pd[i]->farmer == 0)
            cout<<"�ϰ�_";
        else
            cout<<"����_";
        if(path_pd[i]->wolf == 0)
            cout<<"�ϰ�_";
        else
            cout<<"����_";
        if(path_pd[i]->sheep == 0)
            cout<<"�ϰ�_";
        else
            cout<<"����_";
        if(path_pd[i]->cabbage == 0)
            cout<<"�ϰ�"<<endl;
        else
            cout<<"����"<<endl;
        cout<<endl;
    }
	//cout<<path_pd[i]->farmer<<"  "<<path_pd[i]->wolf<<"  "<<path_pd[i]->sheep<<"  "<<path_pd[i]->cabbage<<endl;
}
void init(){
	//binary�����ʼ��
	int i,j;
	for(i = 0;i < 4;i++)
		binary[i] = 0;

	//�����ʼ��
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
		//Ϊѭ��ʹ��binary��Ҫ�����ÿ�
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

	cout<<"ũ���������\n"<<endl;
	cout<<"ũ��-��-��-�ײ�"<<endl;
	cout<<"\n1.ʹ�����������������\n"<<endl;
	int length = 0;
	pathSearch_DFS(&vet[0],&vet[15],length);
	path_pd[0] = &vet[0];
	printPath_DFS(length);

	//�ÿ�flag
	int i;
	for(i = 0;i < 16;i++)
		flag[i] = 0;

	cout<<"\n2.ʹ�ù��������������\n"<<endl;
	pathSearch_BFS(&vet[0]);
	printPath_BFS(&vet[15]);

	return 0;
}
