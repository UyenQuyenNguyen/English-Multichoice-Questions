#pragma once
#include<stdio.h>
#include<conio.h>
#include<bits/stdc++.h>
using namespace std;
// Num of answer

#define num 4

typedef struct Question{
	int id;
	char content[2000];
	char answerData[4][100];
	char answer;
}Ques;

struct AVLNODE{
	int balFactor;
	Ques key;
	AVLNODE *left, *right;
};
typedef AVLNODE* tree;

void Init(tree &t){
	t = NULL;
}

void LeftRotate(tree &p){
	tree q;
	q = p->right;
	p->right = q->left;
	q->left = p;
	p = q;
}

void RightRotate(tree &p){
	tree q;
	q = p->left;
	p->left = q->right;
	q->right = p;
	p = q;
}

int LeftBalance(tree &p){
	switch(p->left->balFactor){
		case 1:
			RightRotate(p);
			p->balFactor = 0;
			p->right->balFactor = 0;
			break;
		case 2:
			LeftRotate(p->left);
			RightRotate(p);
			switch(p->balFactor){
				case 0:
					p->left->balFactor = 0;
					p->right->balFactor = 0;
					break;
				case 1:
					p->left->balFactor = 0;
					p->right->balFactor = 2;
					break;
				case 2:
					p->left->balFactor = 1;
					p->right->balFactor = 0;
					break;
			}
			p->balFactor = 0;
			break;
	}
}

int RightBalance(tree &p){
	switch(p->right->balFactor){
		case 1:
			RightRotate(p->right);
			LeftRotate(p);
			switch(p->balFactor){
				case 0:
					p->left->balFactor = 0;
					p->right->balFactor = 0;
					break;
				case 1:
					p->left->balFactor = 1;
					p->right->balFactor = 0;
					break;
				case 2:
					p->left->balFactor = 0;
					p->right->balFactor = 2;
					break;
			}
			p->balFactor = 0;
			break;
		case 2:
			LeftRotate(p);
			p->balFactor = 0;
			p->left->balFactor = 0;
			break;
	}
}

//Insert Node
int InsertNode(tree &t, Ques x){
	int res;
	if(t!=NULL){
		if(t->key.id == x.id)
			return 0;
		else if(t->key.id > x.id){
			res = InsertNode(t->left, x);
			if(res < 2)
				return res;
			switch(t->balFactor){
				case 0:
					t->balFactor = 1;
					return 2;
				case 1:
					LeftBalance(t);
					return 1;
				case 2:
					t->balFactor = 0;
					return 1;
			}
		}
		else{
			res = InsertNode(t->right, x);
			if( res < 2)
				return res;
			switch(t->balFactor){
				case 0:
					t->balFactor = 2;
					return 2;
				case 1:
					t->balFactor = 0;
					return 1;
				case 2:
					RightBalance(t);
					return 1;
			}
		}
	}
	t = new AVLNODE;
	if(t == NULL)
		return -1;
	t->key = x;
	t->left = t->right = NULL;
	t->balFactor = 0;
	return 2;	
}
void InputQues(Ques &x, int id){
	x.id = id;
	printf("Input question: ");fflush(stdin);
	gets(x.content);
	char choice;
	int k;
	for(int i = 0; i < num; i++){
		printf("Input answer thu %d: ", i+1);
		fflush(stdin);
		gets(x.answerData[i]);
	}
	bool flag = false;
	do {
		printf("Correct answer is: ");
		fflush(stdin);
		scanf("%c", &x.answer);
		switch (x.answer) {
			case 'A':
			case 'a':
			case 'B':
			case 'b':
			case 'C': 
			case 'c':
			case 'D':
			case 'd':
				flag = true;
			break;
			default:
				printf ("Please input A,B,C,D (uppercase)!"); 
		}	
	} while (!flag);
}
//Input List
void CreateTree(tree &t, int &index){
	int n;
	Ques x;
	Init(t);
	printf("Enter the number of question: "); 
	scanf("%d", &n);

	for(int i = 1; i <= n; i ++){
		InputQues(x , index+i);
		InsertNode(t,x);
	}
	cout << n;
	index+=n;
}

//Ouput List
void OutputQues(Ques x){
	printf("%d\n", x.id);
	printf("%s\n", x.content);
	for(int i = 0; i < num; i++){
		printf ("%s\n", x.answerData[i]);
	}
	cout <<"XXX" << endl;
	cout << endl;
}
void NLR(tree t){
	if(t!=NULL){
		OutputQues(t->key);
		NLR(t->left);
		NLR(t->right);
	}
}

//Read file
void read_1_question(FILE *fp, Ques &x){
	fscanf(fp," %d,", &x.id);
	fscanf(fp, " %[^,],", &x.content);
	for (int i = 0; i < num; i++) {
		fscanf(fp, "\n%[^,],", &x.answerData[i]);
	}
	fscanf(fp, "\n%[^,],\n\n", &x.answer);
}
void ReadFile(tree &t,char *array_answers,int &index){
	FILE* fp = fopen("EnglishQuestions.txt","r");// read file
	Ques x;
	if(fp == NULL){
		exit(0);
	}
	while(!feof(fp))
	{
		read_1_question(fp,x); 
		//cout << x.id << endl;
		InsertNode(t,x); 
		array_answers[index] = x.answer;
		index++;
	}
	cout <<"successfull" << endl;
	fclose(fp);	
}

// Write file
bool writeSupport (tree &t, FILE *fp) {
	if (t!=NULL) {
		writeSupport(t->left, fp);
		fprintf(fp,"%d,\n",t->key.id);
		fprintf(fp,"%s,\n",t->key.content);
		for (int i = 0; i< num; i++) {
			fprintf(fp, "%s,\n", t->key.answerData[i]);
		}
		fprintf (fp, "%c,\n\n", t->key.answer);
		writeSupport(t->right, fp);
	}else
		return false;
}

void WriteFile(tree &t) {
	FILE * fp = fopen ("EnglishQuestions.txt", "w");
	Ques x;
	
	if (fp == NULL) {
		exit(1);
	}
	
	bool flag = true;
	while (flag) {
		flag = writeSupport(t, fp);
	}
	
	fclose(fp);
}
// Search
tree searchNodeByID (tree t, int IDSearch) {
	if (t) {
		if(t->key.id == IDSearch) return t;
		else if(t->key.id > IDSearch)
			return searchNodeByID(t->left, IDSearch);
		else
			return searchNodeByID(t->right, IDSearch);
	}
	return NULL;
}

// Update
void updateNodeByID (tree &t, int IDUpdate) {
	tree index = new AVLNODE;
	index = searchNodeByID(t, IDUpdate);
	if (t) {
		if (index) {
			InputQues(t->key, IDUpdate);
			InsertNode(t,index->key); 
		}
	}
}

void play(tree t,char *array_answers,int index){
	for( int i=1;i<=index;i++){
		cout <<"Question " << i << endl;
	}
	int choice;
	cout <<"CHOICE question: ";
	cin >> choice;
	tree p = new AVLNODE;
	p = searchNodeByID(t,choice); 
	cout <<"XXX";
	cout << p->key.id;
	if(p == NULL){
		printf("Not found");
	}else
	{
		OutputQues(p->key);
		char user;
		cout <<"answer correct: "; 
		fflush(stdin);
		scanf("%c",&user);
		for( int i=1;i<=index;i++){
			if(i==p->key.id){
				cout << array_answers[i-1] <<endl;
				char letter = array_answers[i-1];
				if(letter==user){
					cout <<"Correct" <<endl;
				}else
					cout <<"Incorrect" <<endl;
				break;
			}
		}
	}
		
}

int Node_the_mang(tree &p,tree &q){
	// p node xoa
	// q: the mang
	int	res;
	if(q->left)	
	{  
		res	= Node_the_mang(p, q->left );
	   if(res < 2)	return res;
	   switch(q->balFactor)	
	   {  case 1: q->balFactor = 0; return 2;
	      case 0: q->balFactor = 2; return 1;
	      case 2: return RightBalance(q); 
	   }
	}  else	
	{  
		p->key = q->key ; 
		p = q; 
		q = q->right ; 
		return 2;
	}
}
//delete
int deleteNode(tree &t, int ID){
	int res;
	if(t == NULL){
		return 0;
	}
	// find value
	if(ID < t->key.id){
		res = deleteNode(t->left, ID);
		if(res < 2)	{
			return res;
		}
	   	switch(t->balFactor)	
	   	{  
		   	case 1: t->balFactor = 0; return 2;
	    	case 0: t->balFactor = 2; return 1;
	      	case 2: return RightBalance(t); 
	   	} 
	}
	else if(ID > t->key.id){
		res = deleteNode(t->right, ID); 
		if(res < 2)	{
			return res;
		}
	   	switch(t->balFactor)	
	   	{  
		   	case 1: t->balFactor = 0; return 2;
	    	case 0: t->balFactor = 1; return 1;
	      	case 2: return LeftBalance(t); 
	   	} 
	}else
	// found value
	{
		tree temp = t;
		// have 2 children 
		if(t->left != NULL || t->right != NULL){
			Node_the_mang(temp,t->right); 
			if(res < 2) return res;
           	switch(t->balFactor)	
           	{  
			   	case 2: t->balFactor = 0; return 2;
              	case 0: t->balFactor = 1; return 1;
              	case 1: return LeftBalance(t); 
          	}
		}else
		{   // has 1 child 
			if(t->left == NULL){
				t = t->right;
				res =2 ;
			}else
				t = t->left;
				res =2;		
		}
		OutputQues(temp->key); 
		// no child
		delete temp;	
		return res;	
	}
}

void xoa(tree &t,char *array_answers, int &index){
	tree p = new AVLNODE ;
	int temp;
	int flag =0;
	char YN[5];
	while(1){
		printf("Enter ID you want to delete: ");
		cin >> temp;
		p = searchNodeByID(t, temp);
		if(p==NULL) printf("not Found!\n");
		else {
			OutputQues(p->key);
			printf("\nBan that su muon xoa? (Y/N) :");
			fflush(stdin);
			gets(YN);
//			if(strcmp(YN,"y")){
//				for( int i=0;i<index;i++){
//					if(p->key.id == i){
//						array_answers[i] = 0;
//					}
//				}
			deleteNode(t,temp);
			flag = 1;
			//}
		}
		if(flag == 1){
			cout <<"delete successfull" <<endl;
			break;
		}else
			break;
			
	} 
}

void Menu(){
	printf("\n>>>----------	MULTI-CHOICE QUESTION MANAGEMENT PROGRAM ------------------<<<");
	printf("\n>>>1. play.                                                              <<<");
	printf("\n>>>2. Add question list to file.                                         <<<");
	printf("\n>>>3. Write question list by File.                                       <<<");
	printf("\n>>>4. Export question list.                                              <<<");
	printf("\n>>>5. Search questions by ID.                                            <<<");
	printf("\n>>>6. Update question information by ID.                                 <<<");
	printf("\n>>>7. Delete question information by ID.                                 <<<");
	printf("\n>>>8. Exit.                                                              <<<");
	printf("\n>>>----------------------------------------------------------------------<<<");
	printf("\nYour choice: ");
}

//int main(){
//	tree t;
//	Init(t);
//	int key;
//	int index=0;
//	char array_answers[50];
//	bool entered = false;
//	while(true){
//		Menu();
//		scanf("%d", &key);
//		switch(key){
//			case 1:
//				{
//					int index =0;
//					ReadFile(t,array_answers, index);
//					NLR(t);
//					char again[3];
//					do{
//						play(t,array_answers,index);
//						cout <<"\nyou play continue ?(Y/N)";
//						fflush(stdin);
//						gets(again);
//					}while(strcmp(again,"n")==0 || strcmp(again,"No")==0);
//				}break;	
//			case 2:
//				{
//					CreateTree(t,index);
//					WriteFile(t);
//					entered = true;
//				}
//				break;
//			case 3:
//				ReadFile(t,array_answers,index);
//				NLR(t);
//				entered = true;
//				break;
//			case 4:
//				{
//					if(entered){
//						NLR(t);
//					}else
//						printf("\nPlease enter question list!");
//				}break;
//			case 5:
//				{
//					if(entered){
//						int IDSearch;
//						printf("\nEnter ID to search: "); scanf("%d", &IDSearch);
//						tree result = new AVLNODE;
//						result = searchNodeByID(t, IDSearch);
//						if(result){
//							OutputQues(result->key);
//						}else{
//							printf("\nQuestion not found!");
//						}
//					}
//				}break;
//			case 6:
//				if(entered){
//					int IDUpdate;
//					printf("\nEnter ID to update: "); scanf("%d", &IDUpdate);
//					updateNodeByID(t, IDUpdate);
//					WriteFile(t);
//				} else
//					printf("\nPlease enter question list !");
//				getch();
//				break;
//			case 7:
//				{
//					xoa(t,array_answers, index);
//					WriteFile(t);
//				}break;
//			case 8:
//				{
//					exit(0);
//				}break;
//		}
//	} 
//}


