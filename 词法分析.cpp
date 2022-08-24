#include<iostream>
#include<cstring>
#include <fstream>
#include <sstream>
#include <cassert>
using namespace std;
ifstream infile;
/*
最初的产生式：
A：界符
B：运算符
C：标识符
D:数值常量
H：数值超出范围 

最后的类别表示：
0：界符
1：运算符
2：标识符
3：常量
4：关键字 
5：出错 
6：标识符出错 
7：数值超出范围 
*/

string keyboard[37]={"auto","bool","break","case","char","class",
"const","continue","default","define","delete","do","double",
"else","false","float","for","if","int","include","namespace","new","private","protected","public",
"return","signed","sizeof","static","struct","switch",
"true","union","void","while","using","string"};
string def[9]={"int","char","bool","class","double","float","struct","string","void"};
string type[8]={"界符","运算符","标识符","常量","关键字","出错","标识符出错","数值超出范围"};
struct VNimf{//非终结符信息 
	char vn; //非终结符 
	string next;//可能的输入符号 
	int n;//可能的输入符号数目 
	string end[27];//对应于输入符号的下一个状态 
};
VNimf VN[100];
string str;
struct subset{
	string t;
	string next;
	//int n;
	string start[27];
	int end[27];
};
subset Ti[1000];

struct Token{
	string t;
	string type;
	int Type;
	int line;
	int isDef; 
};
Token token[1000];
int tokenN;
bool lastIsDef=false;
int ProdN;//产生式数量 
int VnN;//非终结符数量 
string VT;//终结符 
int TiN=0;//DFA子集个数 
int ifExistVN(char a){
	for(int j=0;j<VnN;j++){
		if(a==VN[j].vn){
			return j;
		}
	}return -1;
}

void NFA(){

	infile.open("文法.txt");
	assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
	
	
	string s;
	infile>>ProdN; getline(infile,s);
	
	VN[0].vn='X';
	VN[0].next="@";
	VN[0].n=1;
	VN[0].end[0]="ABCD";
	
	int i=1,tem,tem1;VnN=1;
	while(getline(infile,s)){
		tem=ifExistVN(s[0]);
		if((VT.find(s[3])==-1)&&(s[3]!='@')){
			
			VT+=s[3];
			
		}
		if(tem==-1){
			VN[i].vn=s[0];
			VN[i].next+=s[3];
			if(s.length()==5){
				VN[i].end[VN[i].n]+=s[4];
			}else{
				VN[i].end[VN[i].n]+='Y';
			}
			VN[i].n++;
			i++;VnN++;
		}else{tem1=VN[tem].next.find(s[3]);
	
			if(tem1==-1){
				
				VN[tem].next+=s[3]; 
				if(s.length()==5){
					VN[tem].end[VN[tem].n]+=s[4];
				}else{
					VN[tem].end[VN[tem].n]+='Y';
				}VN[tem].n++;
			}else{
				
				if(s.length()==5){
					VN[tem].end[tem1]+=s[4];
				}else{
					VN[tem].end[tem1]+='Y';
				}
			}
			
		}
	}
	infile.close();
	
} 
void order(int a){//给子集内字符排序 

	int n=Ti[a].t.length();
	int i,j=1,tip=1;char tem;
	while(tip!=0){
		tip=0;
		for(i=0;i<n-j;i++){
			
			if(Ti[a].t[i]-Ti[a].t[i+1]>0){
				tem=Ti[a].t[i];
				Ti[a].t[i]=Ti[a].t[i+1];
				Ti[a].t[i+1]=tem;
				tip++;
			}
		}
		j++;
	}
	return;
}
void closure(int k){
	string tem;int tip;
	do{tip=1;
		for(int i=0;i<VnN;i++){
		
			if((Ti[k].t.find(VN[i].vn)!=-1)&&(VN[i].next.find('@')!=-1)){
				tem=VN[i].end[VN[i].next.find('@')];
				for(int j=0;j<tem.length();j++){
					if(Ti[k].t.find(tem[j])==-1){
						Ti[k].t+=tem[j];tip=0;
						
					}
				}
			}
		}
	}while(tip==0);
	order(k);
	return;
}
void closureMove(int i,char a,int now){
	string s;int len,tem;str="";
	for(int j=0;j<VnN;j++){
		tem=VN[j].next.find(a);
		if((tem!=-1)&&(Ti[i].t.find(VN[j].vn)!=-1)){
			
			len=VN[j].end[tem].length();
			for(int k=0;k<len;k++){
				if(s.find(VN[j].end[tem][k])==-1){
					s+=VN[j].end[tem][k];
				}
			}
			if(str.find(VN[j].vn)==-1){
				str+=VN[j].vn;
			}
			
		}
	}
	Ti[now].t=s;
	closure(now);
	return;
}
void DFA(){
	Ti[0].t="X";
	closure(0);
	int i=0,j,now=1;
	TiN++;
	int nontip=1;
	while(nontip!=0){
		for(j=0;j<VT.length();j++){
			closureMove(i,VT[j],now);
			if(Ti[now].t==""){
				continue;
			}
			TiN++;
			Ti[i].next+=VT[j];
			Ti[i].end[Ti[i].next.length()-1]=now;
			Ti[i].start[Ti[i].next.length()-1]=str;
			for(int j=0;j<now;j++){
				if(Ti[j].t==Ti[now].t){
					
					Ti[i].end[Ti[i].next.length()-1]=j;
					Ti[now].t="";
					now--;nontip--;TiN--;
					break;
				}
			}
			now++;nontip++;
		}i++;nontip--;
	}
	
	return;
	
	
}
int isLetter(char c){
	if((c-'a'>=0)&&('z'-c>=0)){
		return 1;
	}
	if((c-'A'>=0)&&('Z'-c>=0)){
		return 1;
	}
	return 0;
}
int isDecimalN(char c){
	if((c-'0'>=0)&&('9'-c>=0)){
		return 1;
	}return 0;
}
int jud(string tem,int now,int line){
	int j=0,l=0,i,last=-1;char s,s1;
	for(i=0;i<=tem.length();i++){
		if(isLetter(tem[i])==1){
			s='l';s1='\0';
		}
		else if(isDecimalN(tem[i])==1){
			s='d';s1='\0';
		}else {
			s=tem[i];s1='\0';
		}
		if(tem[i]=='e'||tem[i]=='i'){
			s=tem[i];s1='l';
		}
		if(tem[i]=='E'){
			s='e';s1='l';
		} 
		for(int k=0;k<Ti[j].t.length();k++){
			if(Ti[j].t[k]=='Y'){
				
				int pre=0;
				for(int k=0;k<Ti[j].next.length();k++){
					if(s==Ti[j].next[k]||s1==Ti[j].next[k]){
						pre=1;
					}
				}
				if(pre==1){
					break;
				}
				for(l=last+1;l<i;l++){
					token[now].t+=tem[l];
					
				}
				token[now].type=str;
				for(int k=0;k<37;k++){
					if(token[now].t==keyboard[k]){
						token[now].type="4";
					
					}
				}
				
				for(int k=0;k<9;k++){
					if(token[now].t==def[k]){
						token[now].isDef=true;
					}
				}
				token[now].line=line;
				now++;j=0;last=i-1;
				if(i==tem.length()){
					return now;
				}
				if(token[now-1].t=="'"){
					if(tem[l]=='\\'&&tem[l+2]=='\''){
						token[now].t+=tem[l];
						token[now].t+=tem[l+1];
						token[now].type="3";
						token[now].line=line;
						now++;
						token[now].t+=tem[l+2];
						token[now].type="0";
						token[now].line=line;
						now++;
						last=l+2;i=last+1;j=0;
						
						break;
					}
					if(tem[l+1]=='\''){
						token[now].t+=tem[l];
						token[now].type="3";
						token[now].line=line;
						now++;
						token[now].t+=tem[l+1];
						token[now].type="0";
						token[now].line=line;
						now++;
						last=l+1;i=last+1;j=0;
						break;
					}
				}
				
				if(token[now-1].t=="\""){
					if(token[now-2].type=="3"){
						break;
					}
					
					while(tem[l]!='"'||tem[l-1]=='\\'){
						token[now].t+=tem[l];
						l++;
						
					}
					token[now].type="3";
					token[now].line=line;
					now++;
					token[now].t+=tem[l];
					token[now].type="0";
					token[now].line=line;
					i=l+1;j=0;now++;last=i-1;l++;
					break;
				}
			}
		}
		if(isLetter(tem[i])==1){
			s='l';s1='\0';
		}
		else if(isDecimalN(tem[i])==1){
			s='d';s1='\0';
		}else {
			s=tem[i];s1='\0';
		}
		if(tem[i]=='e'||tem[i]=='i'){
			s=tem[i];s1='l';
		}
		if(tem[i]=='E'){
			s='e';s1='l';
		} 
		for(int k=0;k<Ti[j].next.length();k++){
			if(s==Ti[j].next[k]||s1==Ti[j].next[k]){
				if(j==0){
					str=Ti[j].start[k];
				}
				j=Ti[j].end[k];
			}
		}
	}
	if(i==(tem.length()+1)){
		for(l=last;l<tem.length();l++){
			token[now].t+=tem[l];
		
		}
		token[now].type="5";
		token[now].line=line;
		now++;
		return now;
	}	
			
}
void scan(){
	infile.open("源程序.txt");
	assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 
	string s;string tem;int line=1,tip=0,now=0;
	while(getline(infile,s)){
		stringstream word(s);
		while(word>>tem){
			tip=0;
			for(int i=0;i<37;i++){
				if(tem==keyboard[i]){
					token[now].t=tem;
					for(int k=0;k<9;k++){
						if(token[now].t==def[k]){
							token[now].isDef=true;
						}
					}
					
					token[now].type="4";
					token[now].line=line;
					now++;tip=1;
				}
			}
			
			if(tip==1){
				continue;
			}
			
			now=jud(tem,now,line);
			
		}
		line++;
	}
	
	tokenN=now;
	infile.close();

	tip=1;
	
	while(tip==1){
		tip=0;
		for(int i=0;i<tokenN;i++){
			//cout<<token[i].t<<' '<<token[i].type<<' '<<token[i].Type<<endl;
			if(token[i].type.length()>1){
				tem=token[i].type;
				if(tem.find('A')!=-1&&tem.find('B')!=-1){
					if((token[i-1].type=="C"||token[i-1].type=="D")&&(token[i+1].type=="C"||token[i+1].type=="D")){
						token[i].Type=1;token[i].type="B";
					}else{
						token[i].Type=0;token[i].type="A";
					}
					
					
				}
				if(tem.find('B')!=-1&&tem.find('D')!=-1){
					if(isDecimalN(token[i].t[1])==1){
						token[i].Type=3;token[i].type="D";
					}else{
						token[i].Type=1;token[i].type="B";
					}
					
					
				}
				tip=1;
			}else if(token[i].type=="D"){
				int sign,j,n=0;string ds,ck="2147483647";
				if(token[i].t[0]=='-'){
					sign=-1;
				}else{
					sign=1;
				}
				for(j=0;j<token[i].t.length();j++){
					if(j==0&&isDecimalN(token[i].t[j])==0){
						continue;
					}
					if(isDecimalN(token[i].t[j])==0){
						
						break;
					}
					n+=1;
					ds+=token[i].t[j];
				}
				if(j==token[i].t.length()){
					if(n>=10){
						token[i].Type=7;token[i].type="H"; 
					}else if(n==10){
						for(int k=0;i<10;k++){
							if(k==9&&sign==-1&&ds[k]>'8'){
								token[i].Type=7;token[i].type="H"; break;
							}
							if(ds[k]>ck[k]){
								token[i].Type=7;token[i].type="H"; 
							}
						}
					}else{
						token[i].Type=3;token[i].type="D"; 
					}
				}
			}
			if(token[i].type.length()==1){
				if(isLetter(token[i].type[0])==1){
					token[i].Type=token[i].type[0]-'A';
				}else{
					token[i].Type=token[i].type[0]-'0';
				}
			}
			
			
			
		}
	}
	for(int i=0;i<tokenN;i++){
			if(token[i-1].isDef==true){
				int j=0;
				while(token[i+j].t!=","&&token[i+j].t!=";"&&token[i+j].t!="="&&token[i+j].t!="("){
					if(token[i+j+1].t==","){
						token[i+j+1].isDef=true;
					}
					if(j>0){
						token[i].t+=token[i+j].t;
						token[i+j].t="";
						token[i].Type=6;
						
					}
					j++;
				}
			}	
	}

	
	return;
}
int main(){
	NFA();
	cout<<"终结符：		"<<VT<<endl<<endl;
	int i,j,k;
	int a; 
	for(i=0;i<VnN;i++){
		cout<<"非终结符："<<VN[i].vn<<endl;
		for(j=0;j<VN[i].n;j++){
			cout<<"可能的输入符号："<<VN[i].next[j]<<"     下一个状态："<<VN[i].end[j]<<endl;
		}cout<<endl;
	}
	cout<<endl; 
	DFA();
	for(i=0;i<TiN;i++){
		cout<<"第"<<i<<"个子集"<<Ti[i].t<<endl;
		for(j=0;j<Ti[i].next.length();j++){
			cout<<"终结符子集："<<Ti[i].start[j]<<"        可能的输入符号："<<Ti[i].next[j]<<"     下一个状态："<<Ti[i].end[j]<<endl;
		}
		cout<<endl;
	}
	scan();
	for(i=0;i<tokenN;i++){
		if(token[i].t==""){
			continue;
		}
		cout<<token[i].line<<' '<<token[i].t<<' '<<type[token[i].Type]<<endl;
		//cout<<token[i].Type<<' '<<token[i].t<<endl;
	}
	cout<<"输入任意数字并回车以结束程序"<<endl;
	cin>>a;
	 
	return 0;
}
