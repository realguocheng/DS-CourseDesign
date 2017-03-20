#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<string>
#include<windows.h>
#include<iomanip>
#include<time.h>
#define MaxSize 20
#define HashSize 100000
#define Len sizeof(HashTable)
#define Len1 sizeof(ChainHashTable)
#define Len2 sizeof(Chain)
using namespace std;

typedef unsigned int (*HASH) (const char *);//hash method pointer
typedef char Str[MaxSize];//redefine char [MaxSize]

struct node{//record node
	Str name;
	Str add;
	Str tel;
	struct node* next;
};
typedef struct node Record;

struct HashTable{//hash table using array
	Record *elem[HashSize];
	int count;
	int size;
};

struct Chain{
	Record *head;
	Record *last;
	int count;
};

struct ChainHashTable{//hash table use chain
	Chain* c[HashSize];
	int count;
};

int eq(Str x,Str y){//string compare
	if(strcmp(x,y)==0)
	return 1;
	else return 0;
}

Record a[10000];//array of record
ChainHashTable cht,cht1;
HashTable H,H1;
int icount=0;//current size of a

//read in new records
void getIn(Record* a){
	cout<<"请先输入要添加的记录数目：";
	int number;
	cin>>number;
	cout<<endl;

	int i;
	for(i=0;i < number;i++){

		if(i>0) cout<<"------------------"<<endl;
		cout<<"姓名：";
		cin>>a[icount].name;
		cout<<"电话：";
		cin>>a[icount].tel;
		cout<<"地址：";
		cin>>a[icount].add;
		icount++;
	}
}
//show all stored records
void showAllInformation(Record* a){
	cout.setf(ios::left);
	cout<<setw(10)<<"姓名"<<setw(14)<<"电话"<<setw(14)<<"地址"<<endl;
	int i;
	for(i = 0;i < icount;i++){
		cout<<setw(10)<<a[i].name<<setw(14)<<a[i].tel<<setw(14)<<a[i].add<<endl;
	}
}
//all kinds of hash method
 unsigned int strFoldHash(const char * str){

	unsigned char *p = (unsigned char *) str;
	unsigned int sum = 0;

	while(*p != '\0')
	sum += *p++;

	return sum % HashSize;
}

 unsigned int bkdr_hash(const char *str){

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
//create hash table made by array
int createHashTable(HashTable* H,Record *a,HASH hash,int key){

	cout<<"建表中..."<<endl;
	cout<<endl;
	int i,p = -1,colltimes = 0;//colltimes is the times collision happened

	for(i = 0;i < icount;i++){
        //first confirm the type of keyword
        if(key == 1)
            p = hash(a[i].name);
        else
            p = hash(a[i].tel);

        if(H->elem[p] != 0){//find a closest blank position to place the record
            cout<<"第"<<i+1<<"个记录发生冲突"<<endl;
            colltimes++;

            int pp = p;
            int scount = 0;
            while(H->elem[pp]!=0 && scount<HashSize){
                pp = (pp + 1) % HashSize;
                scount++;
            }

            if(p == pp)//has traversed a circle but find it full
                cout<<"第"<<i+1<<"个记录无法解决冲突"<<endl;
            else{
                p = pp;
                H->elem[p] = &a[i];
                H->count++;
            }
        }
        else{//if find then place it
            H->elem[p] = &a[i];
            H->count++;
        }
	}
	cout<<"建表中共发生"<<colltimes<<"次冲突"<<endl;
	cout<<"\n建表完成!!\n哈希表容量: "<<HashSize<<",当前存储容量: "<<H->count<<".\n";
	return colltimes;
}
//create hash table made by chain
int createChainHashTable(ChainHashTable *H,Record *a,HASH hash,int key){
	cout<<"建表中..."<<endl;
	cout<<endl;
	int i,p=-1,colltimes = 0;

	for(i = 0;i < icount;i++){
        //first confirm the type of keyword
        if(key == 1)
            p =  hash(a[i].name);
        else
            p =  hash(a[i].tel);
        //find then place it
		if(H->c[p]->head == 0){
			H->c[p]->head = &a[i];
			H->c[p]->last = &a[i];
			a[i].next = 0;
		}
		else{//not find then insert into the current chain
			cout<<"第"<<i+1<<"个记录发生冲突"<<endl;
			colltimes++;
			H->c[p]->last->next = &a[i];
			H->c[p]->last = &a[i];
		}
		//record the current number of chain and the hash table
		H->c[p]->count++;
		H->count++;
	}
	cout<<"建表中共发生"<<colltimes<<"次冲突"<<endl;
	cout<<"\n建表完成!!\n哈希表容量: "<<HashSize<<",当前存储容量: "<<H->count<<".\n";
	return colltimes;
}

//search the hash table
void searchByName(HashTable* H,ChainHashTable* cht,HASH hash1,HASH hash2){

		Str str;
		cout<<"请输入要查找记录的姓名：";
		cin >>str;
		int p;
        //no need to search if there is nothing
		if(H->count != 0){
			p = hash1(str);

			if(H->elem[p] == 0){
				cout<<"\n查找失败,此人不存在"<<endl;
				return;
			}//next find the possible position
			do{
				if(eq(str,H->elem[p]->name)==1)
				break;
				else p = (p + 1) % HashSize;
			}while(H->elem[p]!=0 && p != p);
            //print the info of the record
            if(H->elem[p]!=0 && eq(str,H->elem[p]->name)==1)
                cout<<"\n查找成功"<<"\n记录信息：姓名："<<H->elem[p]->name<<"\t电话："
                <<H->elem[p]->tel<<"\t地址："<<H->elem[p]->add<<endl;

            else cout<<"\n查找失败,此人不存在";
            return;
		}
		//chain struct is more easy to search
		else if(cht->count != 0){
			p = hash2(str);
			Record* current = cht->c[p]->head;
			while(current != 0){
				if(eq(str,current->name)){
					cout<<"\n查找成功"<<"\n记录信息：姓名："<<current->name<<"\t电话："
					<<current->tel<<"\t地址："<<current->add<<endl;
					return;
				}
				current = current->next;
			}
			 cout<<"\n查找失败,此人不存在";
		}
}
//search use keyword telephone
void searchByPhone(HashTable* H,ChainHashTable* cht,HASH hash1,HASH hash2){

		Str str;
		cout<<"请输入要查找记录的电话号码：";
		cin >>str;
		int p;
        //some operation is same as the function before
		if(H->count != 0){
			p = hash1(str);
			if(H->elem[p] == 0){
				cout<<"\n查找失败,此人不存在"<<endl;
				return;
			}
			do{
				if(eq(str,H->elem[p]->tel)==1)
				break;
				else p = (p + 1) % HashSize;
			}while(H->elem[p]!=0 && p != p);

            if(H->elem[p]!=0 && eq(str,H->elem[p]->tel)==1)
                cout<<"\n查找成功"<<"\n记录信息：姓名："<<H->elem[p]->name<<"\t电话："
                <<H->elem[p]->tel<<"\t地址："<<H->elem[p]->add<<endl;
            else cout<<"\n查找失败,此人不存在";
            return;
		}
		else if(cht->count != 0){
			p = hash2(str);
			Record* current = cht->c[p]->head;
			while(current != 0){
				if(eq(str,current->tel)){
					cout<<"\n查找成功"<<"\n记录信息：姓名："<<current->name<<"\t电话："
					<<current->tel<<"\t地址："<<current->add<<endl;
					return;
				}
				current = current->next;
			}
			 cout<<"\n查找失败,此人不存在";
		}
	}

//update the current hash table by inserting into new records
void updateAllHashTable(HashTable* H,HashTable *H1,ChainHashTable* cht,ChainHashTable* cht1,Record* a,
		HASH hash1,HASH hash2,HASH hash3,HASH hash4){

		int i,p = -1;
		HashTable* h;
		ChainHashTable *ch;
		for(int j = 0;j < 2;j++)
        {
            if(j == 0){
                h = H;
                ch = cht;
            }else{
                h = H1;
                ch = cht1;
            }
            //part for hash table
             if(ch->count != 0){
                for(i = ch->count;i < icount;i++){
                    if(j == 0)
                        p =  hash2(a[i].name);
                    else
                        p =  hash4(a[i].tel);

                    if(ch->c[p]->head == 0){
                        ch->c[p]->head = &a[i];
                        ch->c[p]->last = &a[i];
                        a[i].next = 0;
                    }
                    else{
                        cout<<"第"<<i+1<<"个记录发生冲突"<<endl;

                        ch->c[p]->last->next = &a[i];
                        ch->c[p]->last = &a[i];
                    }
                    ch->c[p]->count++;
                    ch->count++;
                }
            }
            //part for chain hash table
            if(h->count != 0){
                for(i = h->count;i < icount;i++){

                    if(j == 0)
                        p = hash1(a[i].name);
                    else
                        p = hash3(a[i].tel);

                    if(h->elem[p] != 0){
                        cout<<"第"<<i+1<<"个记录发生冲突"<<endl;

                        int pp = p;
                        int scount = 0;
                        while(h->elem[pp]!=0 && scount<HashSize){
                            pp = (pp + 1) % HashSize;
                            scount++;
                        }

                    if(p == pp)
                        cout<<"第"<<i+1<<"个记录无法解决冲突"<<endl;
                    else{
                        p = pp;
                        h->elem[p] = &a[i];
                        h->count++;
                    }
                    }
                    else{
                        h->elem[p] = &a[i];
                        h->count++;
                    }
                }
            }
        }
}

//part for the main page
void Welcome(){
		cout<<"欢迎使用电话号码查找系统"<<endl;
			cout<<"--------------"<<endl;
			cout<<"1.添加记录\n"<<endl;
			cout<<"2.显示所有记录\n"<<endl;
			cout<<"3.创建记录哈希表\n"<<endl;
			cout<<"4.查找记录信息\n"<<endl;
			cout<<"5.更新已创建哈希表\n"<<endl;
			cout<<"6.退出系统"<<endl;
			cout<<"--------------"<<endl;
			cout<<"(执行操作时请输入相应的序号)\n"<<endl;
			cout<<endl;
		}
//function for values initialization
void init()
{
    int i;
		for(i=0;i<HashSize;i++){
		H.elem[i] = 0;
		H1.elem[i]=0;
	    }
		H.size = HashSize;
		H.count = 0;
		H1.size = HashSize;
		H1.count = 0;

		//chain hash table initialize

		cht.count = 0;
		cht1.count = 0;

		for(i = 0;i < HashSize;i++){
			cht.c[i] = (Chain*)malloc(Len2);
			cht.c[i]->head = 0;
			cht.c[i]->last = 0;
			cht.c[i]->count = 0;
			cht1.c[i] = (Chain*)malloc(Len2);
			cht1.c[i]->head = 0;
			cht1.c[i]->last = 0;
			cht1.c[i]->count = 0;
		}
}

//*********************************************************************
	int main(){

        Welcome();
        init();

		HASH hash[2][2],xhash;
		while(1){
			cout<<"操作:";
			int num;
			cin>>num;
            cout<<endl;
			switch(num){
				case 1:
					getIn(a);
					break;
				case 2:
					showAllInformation(a);
					break;
				case 3:
					cout<<"请选择哈希表关键字：1.用户名 2.电话号码"<<endl;
					int keyword;
					cin>>keyword;
					cout<<"请选择哈希函数：1.strFOLD_HASH 2.BKDR_HASH 3.SDBM_HASH"<<endl;
					int method;
					cin>>method;
					cout<<"请选择哈希冲突解决方法：1.开放定址法 2.拉链法 "<<endl;
					int solution;
					cin>>solution;

					if(method == 1) xhash = strFoldHash;
					else if(method == 2) xhash = bkdr_hash;
					else if(method == 3 )xhash = SDBMHash;
                    else{
                        cout<<"输入有误"<<endl;
                        break;
                    }

					if(solution == 1){
                        if((keyword == 1 && H.count != 0) || (keyword == 2 && H.count != 0))
                        {
                            cout<<"您无需重新创建该类型的表"<<endl;
                            break;
                        }
                        if(keyword == 1)
                        {
                            hash[0][0] = xhash;
                            createHashTable(&H,a,xhash,keyword);
                        }
                        else if(keyword == 2)
                        {
                            hash[1][0] = xhash;
                            createHashTable(&H1,a,xhash,keyword);
                        }
                        else{
                            cout<<"输入有误"<<endl;
                        }
					}
					else if(solution == 2){
                        if((keyword == 1 && cht.count != 0) || (keyword == 2 && cht1.count != 0))
                        {
                            cout<<"您无需重新创建该类型的表"<<endl;
                            break;
                        }
                        if(keyword == 1)
                        {
                            hash[0][1] = xhash;
                            createChainHashTable(&cht,a,xhash,keyword);
                        }
                        else if(keyword == 2)
                        {
                            hash[1][1] = xhash;
                            createChainHashTable(&cht1,a,xhash,keyword);
                        }
                        else{
                            cout<<"输入有误"<<endl;
                        }
					}
					else cout<<"输入有误！！"<< endl;
					break;
				case 4:
					cout<<"请选择查找方式：1.用户名 2.电话号码 "<<endl;
					int search;
					cin>>search;
					if(search == 1){
						if(H.count == 0&&cht.count == 0){
							cout<<"抱歉，以用户名为关键字的哈希表并未被创建"<<endl;
							break;
						}
						searchByName(&H,&cht,hash[0][0],hash[0][1]);
					}
					else if(search == 2){
						if(H1.count == 0&&cht1.count == 0){
							cout<<"抱歉，以电话号码为关键字的哈希表并未被创建"<<endl;
							break;
						}
						searchByPhone(&H1,&cht1,hash[1][0],hash[1][1]);
						}
					else cout<<"输入有误"<<endl;
					break;
				case 5:
					updateAllHashTable(&H,&H1,&cht,&cht1,a,hash[0][0],hash[0][1],hash[1][0],hash[1][1]);
					cout<<"更新完成"<<endl;
					break;
				case 6:
				    cout<<"退出成功"<<endl;
					return 0;
					break;
				default:
					cout<<"输入有误"<<endl;
			}
			cout<<endl;
		}
		system("pause");
		return 0;
	}
















