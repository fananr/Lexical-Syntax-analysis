#include <iostream>
#include<cstring>
#include <fstream>
#include <sstream>
#include <cassert>
#include <string.h>
#include <iomanip>
using namespace std;
ifstream infile;
string VT[24]={"using","namespace","std","cout","cin","include","if","else","while","do","for","break","continue","return","++","--"};
string def[9]={"int","char","bool","class","double","float","struct","string","void"};
string ExpOp[12]={"+","-","*","/","<","<=",">",">=","==","!=","&&","||"};
struct Prod{
	char Vn;
	string right;
};
Prod P[100];
string s[100];
struct Item{
	char Vn;
	string left;
	string right;
	string ahead;
};
struct Itemset{
	Item item[200];
	string pre;
	int preI[200];
	string next;
	int nextI[2000];
	int n;
};
Itemset I[2000];
int ItemsetN=0;
int ProdN;
struct ACTION{
	//char Vt;
	char a[2000];
	int aI[2000];
};
ACTION Action[100];
string Vt;
struct GOTO{
	//char Vn;
	int n[2000];
};

GOTO Goto[100];
string Vn;

int isUpLetter(char c){
	if((c-'A'>=0)&&('Z'-c>=0)){
		return 1;
	}
	return 0;
}
string orderQc(string a){//字符串排序并去重 
	
	int n=a.length();
	int i,j=1,tip=1;char tem;
	while(tip!=0){
		tip=0;
		for(i=0;i<n-j;i++){
			if(a[i]-a[i+1]>0){
				tem=a[i];
				a[i]=a[i+1];
				a[i+1]=tem;
				tip++;
			}
		}
		j++;
	}
	for(i=0;i<a.length()-1;i++){
		if(a[i]==a[i+1]){
			a.erase(i,1);
			i--;
		}
	}
	return a;
}
string first(string tem){
	
	string s="";int i,j,k;
	for(i=0;i<tem.length();i++){
		if((s.length()==0)&&(isUpLetter(tem[i])==0)&&(tem[i]!='&')){
			s=tem[i];//if(tem=="P}")cout<<1;
			return s;
		}
		if((s.length()==0)&&(isUpLetter(tem[i])==1)){
			for(j=0;j<ProdN;j++){
				if(tem[i]==P[j].Vn){
					s+=first(P[j].right);
				}
				
			}
			return s;
		}
	}
	
}
void closure(int i){
	//cout<<"i:"<<i<<endl;
	int j,k,tip;string tem;
	for(j=0;j<I[i].n;j++){
		
		if(I[i].next.find(I[i].item[j].right[0])==-1&&I[i].item[j].right.length()>0){
			I[i].next+=I[i].item[j].right[0];
		}
		if(isUpLetter(I[i].item[j].right[0])==1){//cout<<j<<endl;
			//tip=1;
			//for(k=0;k<I[i].n;k++){
			//	if(I[i].item[j].right[0]==I[i].item[k].Vn&&I[i].item[j]){
			//		tip=0;
			//	}
			//}
			//if(tip==0){
			//	continue;
			//}else{
				for(k=1;k<I[i].item[j].right.length();k++){
					tem+=I[i].item[j].right[k];
				}
			
				//cout<<I[i].item[j].Vn<<"->"<<I[i].item[j].left<<"·"<<I[i].item[j].right<<","<<I[i].item[j].ahead<<endl;
				//cout<<tem<<"的first集："; 
				
				if(I[i].item[j].ahead.length()==1){
					tem+=I[i].item[j].ahead;
					tem=first(tem);
					tem=orderQc(tem);
				}else{
					tem=first(tem);
					if(tem.length()==0||tem=="@"){
						tem=I[i].item[j].ahead;
						tem=orderQc(tem);
					}
				}
				/*
				tem=first(tem);
				cout<<tem<<endl;
				tem+=I[i].item[j].ahead;cout<<tem<<endl;
				tem=orderQc(tem);*/
				for(k=0;k<ProdN;k++){
					tip=1;
					if(I[i].item[j].right[0]==P[k].Vn){//cout<<P[k].Vn<<' '<<P[k].right<<endl;
						//cout<<P[k].Vn<<endl;
						for(int m=0;m<I[i].n;m++){
							if(P[k].Vn==I[i].item[m].Vn&&I[i].item[m].left==""&&I[i].item[m].right==P[k].right&&tem==I[i].item[m].ahead){
								tip=0;//cout<<j<<endl;
								//cout<<P[k].Vn<<"->"<<P[k].right<<endl;
							}
						}
						if(tip==0){
							continue;
						}
						I[i].item[I[i].n].Vn=P[k].Vn;
						
						I[i].item[I[i].n].right=P[k].right;
						
						I[i].item[I[i].n].left="";
						
						I[i].item[I[i].n].ahead=tem;
						I[i].n++;
						
					}
				}
				tem="";
				
			//}
		}
	}
	return;
	
}
void orderI(int a){//cout<<a<<endl;
	int i,j=1,tip=1;char tem;string tem1;
	while(tip!=0){
		tip=0;
		for(i=0;i<I[a].n-1;i++){
			if(I[a].item[i].Vn-I[a].item[i+1].Vn==0){
				for(int k=0;(k<I[a].item[i].left.length())&&(k<I[a].item[i+1].left.length());k++){
					if(I[a].item[i].left[k]-I[a].item[i+1].left[k]<=0){
						continue;
					}
					if(I[a].item[i].left[k]-I[a].item[i+1].left[k]>0){
						//cout<<a<<' '<<I[a].item[i].Vn<<' '<<I[a].item[i].left<<' '<<I[a].item[i].right<<endl;
						tem=I[a].item[i].Vn;
						I[a].item[i].Vn=I[a].item[i+1].Vn;
						I[a].item[i+1].Vn=tem;
						tem1=I[a].item[i].left;
						I[a].item[i].left=I[a].item[i+1].left;
						I[a].item[i+1].left=tem1;
						tem1=I[a].item[i].right;
						I[a].item[i].right=I[a].item[i+1].right;
						I[a].item[i+1].right=tem1;
						tem1=I[a].item[i].ahead;
						I[a].item[i].ahead=I[a].item[i+1].ahead;
						I[a].item[i+1].ahead=tem1;
						tip++;
						break;
					}
				}
			}
				if(I[a].item[i].Vn-I[a].item[i+1].Vn>0){
					tem=I[a].item[i].Vn;
					I[a].item[i].Vn=I[a].item[i+1].Vn;
					I[a].item[i+1].Vn=tem;
					tem1=I[a].item[i].left;
					I[a].item[i].left=I[a].item[i+1].left;
					I[a].item[i+1].left=tem1;
					tem1=I[a].item[i].right;
					I[a].item[i].right=I[a].item[i+1].right;
					I[a].item[i+1].right=tem1;
					tem1=I[a].item[i].ahead;
					I[a].item[i].ahead=I[a].item[i+1].ahead;
					I[a].item[i+1].ahead=tem1;
					tip++;
					break;
				}
		}
		j++;
	}
	
	return;
}
int go(int i,char next,int now){
	int j,k;
	if(isUpLetter(next)==1&&Vn.find(next)==-1){
		Vn+=next;
	}
	if(isUpLetter(next)==0&&Vt.find(next)==-1){
		Vt+=next;
	}
	for(j=0;j<I[i].n;j++){
		if(I[i].item[j].right[0]==next&&I[i].item[j].right.length()>0){
			
				I[now].item[I[now].n].Vn=I[i].item[j].Vn;
				I[now].item[I[now].n].left=I[i].item[j].left;
				I[now].item[I[now].n].left+=I[i].item[j].right[0];
				for(k=1;k<I[i].item[j].right.length();k++){
					I[now].item[I[now].n].right+=I[i].item[j].right[k];
				}
				I[now].item[I[now].n].ahead=I[i].item[j].ahead;
				if(I[now].pre[I[now].pre.length()-1]!=next){
					I[now].pre+=next;
				
					I[now].preI[I[now].pre.length()-1]=i;
				}
				
				I[i].nextI[I[i].next.find(next)]=now;
				I[now].n++;
		}
	}
	closure(now);
	orderI(now);
	for(int l=0;l<now;l++){
				
				if(I[l].n!=I[now].n)continue;
				for(k=0;k<I[now].n;k++){
					
					if(I[l].item[k].ahead!=I[now].item[k].ahead)break;
					if(I[l].item[k].Vn!=I[now].item[k].Vn)break;
					if(I[l].item[k].left!=I[now].item[k].left)break;
					if(I[l].item[k].right!=I[now].item[k].right)break;
				
				}
			
				if(k==I[now].n){
					I[i].nextI[I[i].next.find(next)]=l;
					I[now].pre="";
					memset(I[now].preI,0,sizeof(I[now].preI));
					for(k=0;k<I[now].n;k++){
						I[now].item[k].Vn='\0';
						I[now].item[k].left="";
						I[now].item[k].right="";
						I[now].item[k].ahead="";
					}
					I[now].next="";
					I[now].n=0;
				
					now--;
					break;
				}
	}		
	return now;
}
void order(string s[]){//给产生式排序 

	int i,j=1,tip=1;string tem;
	while(tip!=0){
		tip=0;
		for(i=1;i<ProdN-j;i++){
			for(int k=0;k<s[i].length();k++){
				if(s[i][k]-s[i+1][k]<0){
					break;
				}
				if(s[i][k]-s[i+1][k]>0){
					tem=s[i];
					s[i]=s[i+1];
					s[i+1]=tem;
					tip++;
					break;
				}
			}
			
		}
		j++;
	}
	return;
}
void getProd(){
	infile.open("语法分析_2型文法.txt");
	assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
	
	int now,i=0,j;
	P[0].Vn='Z';P[0].right="S";i++;
	infile>>ProdN; getline(infile,s[0]);
	while(getline(infile,s[i])){
		i++;
	}
	ProdN++;
	order(s);
	for(i=1;i<ProdN;i++){
		
		P[i].Vn=s[i][0];
		for(j=3;j<s[i].length();j++){
			P[i].right+=s[i][j];
		}
	
	}
	infile.close();
	return;
} 
void CreatItemSet(){
	int i,j,now;
	
	I[0].item[0].Vn='Z';
	I[0].item[0].left="";
	I[0].item[0].right="S";
	I[0].item[0].ahead="@";
	I[0].next+='S';
	I[0].n=1;
	i=0;ItemsetN++;now=i+1;
	closure(0);
	for(i=0;i<now;i++){
		for(j=0;j<I[i].next.length();j++){
			
			now=go(i,I[i].next[j],now);
			now++;
		}

	}
	ItemsetN=now;
	return;
}
void creatSheet(){
	int i,j,tem;
	for(i=0;i<Vn.length();i++){
		memset(Goto[i].n,0,sizeof(Goto[i].n));
	}
	for(i=0;i<ItemsetN;i++){
		
		for(j=0;j<I[i].next.length();j++){
			tem=Vt.find(I[i].next[j]);
			if(tem!=-1){
				Action[tem].aI[i]=I[i].nextI[j];
				Action[tem].a[i]='S';
			}else{
				tem=Vn.find(I[i].next[j]);
				Goto[tem].n[i]=I[i].nextI[j];
			}
		}
		for(j=0;j<I[i].n;j++){
			if(I[i].item[j].Vn=='Z'&&I[i].item[j].left=="S"&&I[i].item[j].ahead=="@"){
				tem=Vt.find('@');
				Action[tem].a[i]='A';
				continue;
			}
			if(I[i].item[j].right.length()==0){
				
				for(int k=0;k<I[i].item[j].ahead.length();k++){
					tem=Vt.find(I[i].item[j].ahead[k]);
					Action[tem].a[i]='r';
					for(int l=0;l<ProdN;l++){
						if(P[l].Vn==I[i].item[j].Vn&&P[l].right==I[i].item[j].left){
							Action[tem].aI[i]=l;
							break;
						}
					}
					
				}
			}
		}
	}
	
	return;
}
void analyse(string input,int last){
	input+='@';
	string symbol;int i=0,j,k,state[1000]={0},stateN=0;char now;int tem;
	cout<<"转换后的终结符串： "<<input<<endl;
	stateN++;
	while(input[0]!='\0'){
		now=input[0];
		for(j=0;j<Vt.length();j++){
			if(now==Vt[j]){
				if(Action[j].a[state[i]]=='\0'){
					cout<<"no"<<endl;
				
					cout<<"当前符号所在token表行号："<<last<<endl;
					
					cout<<"当前符号栈："<<symbol<<" ACTION表"<<now<<"（终结符）"<<state[i]<<"（状态）出错"<<endl<<endl;
					return; 
				}
		//		cout<<"状态栈：";
		//		for(k=0;k<=i;k++){
		//			cout<<"("<<state[k]<<")";
		//		}
		//		cout<<"\t符号栈："<<symbol<<"\t输入串："<<input;
				if(Action[j].a[state[i]]=='A'){
					
					cout<<"yes"<<endl<<endl;
					if(input!="@"){
						analyse(input,last+symbol.length()+1);
						
					}
					return;
					
				}
				if(Action[j].a[state[i]]=='S'){
			//		cout<<i<<endl;
					state[i+1]=Action[j].aI[state[i]];
					symbol+=now;
					last++;
			
			//		cout<<"\tACTION：S"<<state[i+1]<<endl;
					input.erase(0,1);
					i++;
					break;
				}
				while(Action[j].a[state[i]]=='r'){
			//		cout<<i<<endl;
					tem=Action[j].aI[state[i]];
					for(k=0;k<P[tem].right.length();k++){
						symbol.erase(symbol.length()-1,1);
						
						
						state[i]=0;
						i--;
					}
		//			cout<<tem;
					symbol+=P[tem].Vn;
					for(k=0;k<Vn.length();k++){
						if(Vn[k]==P[tem].Vn){
							i++;
							state[i]=Goto[k].n[state[i-1]];
		//					cout<<"\tACTION：r"<<Action[j].aI[state[i]];
		//					cout<<"\tGOTO："<<state[i]<<endl;
							
							break;
						}
					}
	//				cout<<"状态栈：";
	//				for(k=0;k<=i;k++){
	//					cout<<"("<<state[k]<<")";
	//				}
	//				cout<<"\t符号栈："<<symbol<<"\t输入串："<<input;
				}
			}
		}
	}
	cout<<"no"<<endl<<endl;
	return;
}
void readToken(){
	infile.open("token表.txt");
	assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
	int i=0,l=0,tip=0,n=0;string tem,input;
	int f=0,last=0;
	while(getline(infile,tem)){
		tip=0;
		for(int j=2;j<tem.length();j++){
			cout<<tem[j];
		}cout<<' ';
		char c=tem[0];
		tem.erase(0,2);
		
		if(tip==0){
			for(i=0;i<9;i++){
				if(tem==def[i]){
					input+='9';
					tip=1;
					break;
				}
			}
		}
		
		if(tip==0){
			for(i=0;i<24;i++){
				if(tem==VT[i]){
					input+='a'+i;
					tip=1;
					break;
				}
			}
		}
		
		if(c=='6'&&tip==0){
			input+='2';
			tip=1;
		}else if(c=='7'){
			input+='3';
			tip=1;
		}
		if(tip==0&&Vt.find(c)!=-1){
			input+=c;tip=1;
		}
		
		if(tip==0&&c=='1'){
			for(i=0;i<12;i++){
				if(tem==ExpOp[i]){
					input+='8';
					tip=1;
					break;
				}
			}
		}
		if(tip==0&&tem.length()==1){
			input+=tem;
			tip=1;
		}
		if(input=="#f<2>"){
			cout<<"yes"<<endl;
			l++;
			input="";
			continue;
		}
		
		l++;
		if(input[input.length()-1]=='{'){
			n++;
		}
		if(tem=="for"){
			f=1;
		}
		if(f==3&&input[input.length()-1]==')'){
			input.erase(input.length()-1,1);
			input+=";)";
			f=0;
			continue;
		}
		if(input[input.length()-1]==';'){
			if(f==1||f==2){
				f++;
				continue;
			}
			
			if(n==0){
				analyse(input,last);
				last=l;
				input=""; 
			}
			
		}
		if(input[input.length()-1]=='}'){
			n--;
			if(n==0){
				analyse(input,last);
				last=l;
				input="";
			}
		}
	}
	
	infile.close();
	return;
}
int main(){
	int i,j;
	
	getProd();
	cout<<"产生式（除拓广式Z->S外按升序排列）: "<<endl;
	for(i=0;i<ProdN;i++){
		cout<<P[i].Vn<<' '<<P[i].right<<endl;
	}
	cout<<"************************************************************"<<endl;
	
	CreatItemSet();
	/*
	for(i=0;i<ItemsetN;i++){
		cout<<"第"<<i<<"个项目集:"<<endl;
		cout<<"前移进符号： "<<I[i].pre<<endl<<"对应的前一个项目集： ";
		for(j=0;j<I[i].pre.length();j++){
			cout<<I[i].preI[j]<<' ';
		}cout<<endl;
		cout<<"后移进符号： "<<I[i].next<<endl<<"对应的下一个项目集：";
		for(j=0;j<I[i].next.length();j++){
			cout<<I[i].nextI[j]<<' ';
		}cout<<endl<<"项目： "<<endl; 
		for(j=0;j<I[i].n;j++){
			cout<<I[i].item[j].Vn<<"->"<<I[i].item[j].left<<"  · "<<I[i].item[j].right<<"\t向前搜索符： "<<I[i].item[j].ahead<<endl;
		}cout<<endl; 
	}
	cout<<"************************************************************"<<endl;*/
	Vn=orderQc(Vn);
	Vt=orderQc(Vt);
	if(Vt.find('@')==-1){
		Vt+='@';
	} 
	cout<<"VN: "<<Vn<<endl<<"VT: "<<Vt<<endl;
	cout<<"************************************************************"<<endl;
	
	creatSheet();
	/*
	cout<<"分析表（分两段打印）："<<endl<<"状态\t";
	for(i=0;i<30;i++){
		cout<<"|"<<setw(6)<<Vt[i];
	}cout<<endl;
	for(i=0;i<ItemsetN;i++){
		cout<<i<<"\t";
		for(j=0;j<30;j++){
			if(Action[j].a[i]=='\0'){
				cout<<"|"<<setw(6)<<' ';
			}else{
				cout<<"|"<<Action[j].a[i]<<setw(5)<<Action[j].aI[i];
			}
		}
		cout<<endl;
	}
	cout<<"续表："<<endl<<"状态\t";;
	for(i=30;i<Vt.length();i++){
		cout<<"|"<<setw(6)<<Vt[i];
	}
	for(i=0;i<Vn.length();i++){
		cout<<"|"<<setw(6)<<Vn[i];
	}cout<<endl;
	for(i=0;i<ItemsetN;i++){
		cout<<i<<"\t";
		for(j=30;j<Vt.length();j++){
			if(Action[j].a[i]=='\0'){
				cout<<"|"<<setw(6)<<' ';
			}else{
				cout<<"|"<<Action[j].a[i]<<setw(5)<<Action[j].aI[i];
			}
		}
		for(j=0;j<Vn.length();j++){
			if(Goto[j].n[i]==0){
				cout<<"|"<<setw(6)<<' ';
			}else{
				cout<<"|"<<setw(6)<<Goto[j].n[i];
			}
		}cout<<endl;
	}
	*/
	
	cout<<"语法分析："<<endl;
	readToken();
	int a;
	cout<<"输入任意数字并回车以结束程序"<<endl;
	cin>>a;
	return 0;
}
