#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<string>
#include<windows.h>
#include<iomanip>
#include<time.h>

#define HashSize 10000
#define RSize 8000
using namespace std;

typedef unsigned int (*HASH) (const char *);

typedef struct node{
	char* name;
	char* add;
	char* tel;
	struct node* next;
}Record;

struct HashTable{
	Record *elem[HashSize];
	int count;
	int size;
};

struct Chain{
	Record *head;
	Record *last;
	int count;
};

struct ChainHashTable{
	Chain* c[HashSize];
	int count;
};

Record test[RSize];
HashTable H[6];
ChainHashTable cht[6];


int eq(char* x,char* y){
	if(strcmp(x,y)==0)
	return 1;
	else return 0;
}
 unsigned int strFoldHash(const char * str){

	unsigned char *p = (unsigned char *) str;
	unsigned int sum = 0;

	while(*p)
	sum += *p++;
	return sum % HashSize;
}

 unsigned int bkdr_hash(const char *str)
{

         unsigned int seed = 131;
         unsigned int hash = 0;
         unsigned char *p = (unsigned char *) str;

         while (*p)

             hash = hash*seed + (*p++);

         return hash % HashSize;

     }

unsigned int SDBMHash(const char *str)
{
    unsigned int hash = 0;

    while (*str)
        hash = (*str++) + (hash << 6) + (hash << 16) - hash;

    return (hash & 0x7FFFFFFF) % HashSize;
}


void generateName(char* name){
		int char_size = rand()%40;

		int i;
		for(i = 0;i < char_size;i++){
			*(name+i)=(rand()%2==0)?(rand()%26+'a'):(rand()%26+'A');
		}

		*(name+i)='\0';
}
void generateTel(char* tel){

		int char_size = 11;

		int i;
		for(i = 0;i < char_size;i++){
			tel[i] = rand()%10+'0';
		}
		tel[i] = '\0';
	}
int createHashTableOfName(HashTable* H,Record *a,HASH hash){

	cout<<endl;
	int i,p = -1,colltimes = 0;


	for(i = 0;i < RSize;i++){

	p = hash(a[i].name);

	if(H->elem[p] != 0){
		colltimes++;

		int pp = p;
		int scount = 0;
		while(H->elem[pp]!=0 && scount<HashSize){
			pp = (pp + 1) % HashSize;
			scount++;
		}

	    if(p == pp) cout<<endl;
		else{
			p = pp;
			H->elem[p] = &a[i];
			H->count++;
		}
}
	else{
		H->elem[p] = &a[i];
		H->count++;
	}

}
return colltimes;
}

int createChainHashTableOfName(ChainHashTable *H,Record *a,HASH hash){

	int i,p=-1,colltimes = 0;

	for(i = 0;i < RSize;i++){
		p =  hash(a[i].name);
		if(H->c[p]->head == 0){
			H->c[p]->head = &a[i];
			H->c[p]->last = &a[i];
			a[i].next = 0;
		}
		else{
			colltimes++;
			H->c[p]->last->next = &a[i];
			H->c[p]->last = &a[i];
		}
		H->c[p]->count++;
		H->count++;
	}
	return colltimes;
}

int createHashTableOfPhone(HashTable* H,Record *a,HASH hash){

	cout<<endl;
	int i,p = -1,colltimes = 0;

	for(i = 0;i < RSize;i++){

	p = hash(a[i].tel);

	if(H->elem[p] != 0){
		colltimes++;

		int pp = p;
		int scount = 0;
		while(H->elem[pp]!=0 && scount<HashSize){
			pp = (pp + 1) % HashSize;
			scount++;
		}

	    if(p == pp) cout<<endl;
		else{
			p = pp;
			H->elem[p] = &a[i];
			H->count++;
		}
}
	else{
		H->elem[p] = &a[i];
		H->count++;
	}

}
return colltimes;
}
int createChainHashTableOfPhone(ChainHashTable *H,Record *a,HASH hash){

	int i,p=-1,colltimes = 0;

	for(i = 0;i < RSize;i++){
		p =  hash(a[i].tel);
		if(H->c[p]->head == 0){
			H->c[p]->head = &a[i];
			H->c[p]->last = &a[i];
			a[i].next = 0;
		}
		else{
			colltimes++;
			H->c[p]->last->next = &a[i];
			H->c[p]->last = &a[i];
		}
		H->c[p]->count++;
		H->count++;
	}
	return colltimes;
}
	int searchByNameAtuo1(HashTable *H,HASH hash,char * str){

		int step = 0,p,pp,num = 0;

			p = hash(str);
			pp = p;

			while(H->elem[p]!=0 && num < HashSize){
				if(eq(str,H->elem[p]->name)==1)
				break;
				else{
					p = (p + 1) % HashSize;
					num++;
				}
			}
			if(p >= pp) step = p -pp;
			else step = HashSize - (pp - p);
			return step + 1;
	}
	 int searchByNameAtuo2(ChainHashTable *cht,HASH hash,char * str){

			int step = 0,p = -1;

			p = hash(str);

			Record* current = cht->c[p]->head;
			while(current != 0){
				step++;
				if(eq(str,current->name))
					break;

				current = current->next;
			}
			//cout<<step<<endl;
			return step;
	}

	int searchByPhoneAtuo1(HashTable *H,HASH hash,char * str){

			int step = 0,p,pp,num = 0;

			p = hash(str);
			pp = p;

			while(H->elem[p]!=0 && num < HashSize){
				if(eq(str,H->elem[p]->tel)==1)
				break;
				else{
					p = (p + 1) % HashSize;
					num++;
				}
			}
			if(p >= pp) step = p -pp;
			else step = HashSize - (pp - p);
			return step + 1;
	}
	 int searchByPhoneAtuo2(ChainHashTable *cht,HASH hash,char * str){

			int step = 0,p = -1;

			p = hash(str);

			Record* current = cht->c[p]->head;
			while(current != 0){
				step++;
				if(eq(str,current->tel))
					break;

				current = current->next;
			}
			//cout<<step<<endl;
			return step;
	}
	void init()
	{
		srand((int)time(NULL));

	}
int main(){
		init();
		//哈希表：数组形式和链表形式初始化

		int i,n;
		for(n = 0;n < 6;n++){
			for(i = 0;i < HashSize;i++)
			H[n].elem[i] = 0;
			H[n].size = HashSize;
			H[n].count = 0;
		}


		for(n = 0;n < 6;n++){
			for(i = 0;i < HashSize;i++){
			cht[n].c[i] = (Chain*)malloc(sizeof(Chain));
			cht[n].c[i]->head = 0;
			cht[n].c[i]->last = 0;
			cht[n].c[i]->count = 0;
		}
		cht[n].count = 0;
		}


		//冲突次数和查找长度定义
		double colltimes[6];
		double sumsteps[6];
		for(i = 0;i < 6;i++)
			sumsteps[i] = 0;

		//哈希函数名称声明
		string hash;
		HASH method;
		//record数组初始化
		for(i = 0;i < RSize;i++){
			test[i].name = (char*)malloc(50*sizeof(char));
			generateName(test[i].name);
			test[i].tel = (char*)malloc(50*sizeof(char));
			generateTel(test[i].tel);
		}
		cout<<"1.使用姓名为关键字进行建表和查找,测试结果如下所示："<<endl;

		//for循环简化测试实现
		for(i = 0;i < 3;i++){

		if(i == 0){
			hash = "SDBMHash";
			method = SDBMHash;
		}
		else if(i == 1){
			hash = "strFoldHash";
			method = strFoldHash;
		}
		else{
			hash = "bkdr_hash";
			method = bkdr_hash;
		}
		colltimes[i] = createHashTableOfName(&H[i],test,method);
		cout<<" "<<hash<<"冲突率："<<colltimes[i]/RSize*100<<"%"<<endl;
		for(int j = 0;j < RSize;j++){
			sumsteps[i] += searchByNameAtuo1(&H[i],method,test[j].name);
		}
		cout<<" "<<hash<<"在开放定址法下的平均查找长度:"<<sumsteps[i]/RSize<<endl;

		sumsteps[i] = 0;
		createChainHashTableOfName(&cht[i],test,method);
		for(int j = 0;j < RSize;j++){
			sumsteps[i] += searchByNameAtuo2(&cht[i],method,test[j].name);
		}
		cout<<" "<<hash<<"在拉链法下的平均查找长度:"<<sumsteps[i]/RSize<<endl;

		cout<<endl;

		}

		cout<<"2.使用电话号码为关键字进行建表和查找,测试结果如下所示："<<endl;
		//for循环简化测试实现
		for(i = 3;i < 6;i++){

		if(i == 3){
			hash = "SDBMHash";
			method = SDBMHash;
		}
		else if(i == 4){
			hash = "strFoldHash";
			method = strFoldHash;
		}
		else{
			hash = "bkdr_hash";
			method = bkdr_hash;
		}
		colltimes[i] = createHashTableOfPhone(&H[i],test,method);
		cout<<" "<<hash<<"冲突率："<<colltimes[i]/RSize*100<<"%"<<endl;
		for(int j = 0;j < RSize;j++){
			sumsteps[i] += searchByPhoneAtuo1(&H[i],method,test[j].tel);
		}
		cout<<" "<<hash<<"在开放定址法下的平均查找长度:"<<sumsteps[i]/RSize<<endl;

		sumsteps[i] = 0;
		createChainHashTableOfPhone(&cht[i],test,method);
		for(int j = 0;j < RSize;j++){
			sumsteps[i] += searchByPhoneAtuo2(&cht[i],method,test[j].tel);
		}
		cout<<" "<<hash<<"在拉链法下的平均查找长度:"<<sumsteps[i]/RSize<<endl;

		cout<<endl;

		}
}
