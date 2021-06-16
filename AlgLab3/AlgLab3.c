#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <locale.h>

struct tnode                                                                 //дерево
{
	int data;
	int balance;
	struct tnode* left;
	struct tnode* right;
};

typedef struct tnode Tree;

int cmp(const void* a, const void* b);                                       //Для упорядочивания массива
int tsize(Tree* t);                                                          //Размер дерева
int theight(Tree* t);                                                        //Высота дерева
int tmiddleh(Tree* t, int l);                                                //Средняя высота дерева
int csumm(Tree* t);                                                          //Контрольная сумма
void ltr(Tree* t);                                                           //Обход слева направо
void Add2b(Tree** t, int d);                                                 //Двоичное Б-дерево поиска 
void AddAVL(Tree** t, int d);                                                //Добавление новой вершины в АВЛ дерево
void LL(Tree** t);                                                            //Повороты
void LR(Tree** t);
void RR(Tree** t);
void RL(Tree** t);

Tree* root1 = NULL; *root2 = NULL;

int* A;
int n;
bool grown , HR, VR;

int main() {
	setlocale(LC_ALL, "rus");
	srand(time(NULL));
	printf("Введите количество вершин в дереве:\n");
	scanf_s("%d", &n);
	system("cls");

	A = (int*)malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++) {
		A[i] = rand() % 1000;
		AddAVL(&root2, A[i]);
		Add2b(&root1, A[i]);
	}
	
	printf("\n");
	printf("АВЛ дерево поиска:\n");
	ltr(root1);
	printf("\n");
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	printf("+++++++++++|Размер|Высота|Средняя высота|контр. сумма|++++++++++++++++++++++++++++++++\n");
	printf("++ДБД %10d %7d %10.2f %15d\n", tsize(root1), theight(root1), (double)tmiddleh(root1, 1) / tsize(root1), csumm(root1));
	printf("++АВЛ %10d %7d %10.2f %15d\n", tsize(root2), theight(root2), (double)tmiddleh(root2, 1) / tsize(root2), csumm(root2));
	printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
	getch();
	return EXIT_SUCCESS;
}

int tsize(Tree* t) {
	if (t == NULL) {
		return 0;
	}
	else {
		return 1 + tsize(t->left) + tsize(t->right);
	}
}

int theight(Tree* t) {
	if (t == NULL) {
		return 0;
	}
	else {
		return 1 + max(theight(t->left), theight(t->right));
	}
}

int tmiddleh(Tree* t, int l) {
	if (t == NULL) {
		return 0;
	}
	else {
		return l + tmiddleh(t->left, l + 1) + tmiddleh(t->right, l + 1);
	}
}

int csumm(Tree* t) {
	if (t == NULL) {
		return 0;
	}
	else {
		return t->data + csumm(t->left) + csumm(t->right);
	}
}

void ltr(Tree* t) {
	if (t != NULL) {
		ltr(t->left);
		printf("%d%s", t->data, " ");
		ltr(t->right);
	}
}

Tree* isdp(int l, int r, int* A) {
	qsort(A, n, sizeof(int), cmp);
	int k;
	Tree* t;
	if (l > r) return NULL;
	else {
		k = (l + r) / 2;
		t = (Tree*)malloc(sizeof(Tree));
		if (t == NULL) { printf("Error!"); exit(1); }
		t->data = A[k];
		t->left = isdp(l, k - 1, A);
		t->right = isdp(k + 1, r, A);
		return t;
	}
}

int cmp(const void* a, const void* b) {
	return *(int*)a - *(int*)b;
}

void LL(Tree** t) {
	Tree* q;
	q = (*t)->left;
	q->balance = 0;
	(*t)->balance = 0;
	(*t)->left = q->right;
	q->right = (*t);
	(*t) = q;
}

void LR(Tree** t) {
	Tree* q;
	Tree* r;
	q = (*t)->left;
	r = q->right;
	if (r->balance < 0)(*t)->balance = 1;
	else (*t)->balance = 0;
	if (r->balance > 0)q->balance = -1;
	else q->balance = 0;
	r->balance = 0;
	(*t)->left = r->right;
	q->right = r->left;
	r->left = q;
	r->right = (*t);
	(*t) = r;

}

void RR(Tree** t) {
	Tree* q;
	q = (*t)->right;
	q->balance = 0;
	(*t)->balance = 0;
	(*t)->right = q->left;
	q->left = (*t);
	(*t) = q;
}

void RL(Tree** t) {
	Tree* q;
	Tree* r;
	q = (*t)->right;
	r = q->left;
	if (r->balance > 0) (*t)->balance = -1;
	else (*t)->balance = 0;
	if (r->balance < 0) q->balance = 1;
	else q->balance = 0;
	r->balance = 0;
	(*t)->right = r->left;
	q->left = r->right;
	r->left = (*t);
	r->right = q;
	(*t) = r;
}

void AddAVL(Tree** t, int d) {
	if ((*t) == NULL) {
		(*t) = (Tree*)malloc(sizeof(Tree));
		(*t)->data = d;
		(*t)->left = (*t)->right = NULL;
		(*t)->balance = 0;
		grown = true;
	}
	else
		if ((*t)->data >= d) {
			AddAVL(&((*t)->left), d);
			if (grown == true) {
				if ((*t)->balance > 0) { (*t)->balance = 0; grown = false; }
				else if ((*t)->balance == 0) { (*t)->balance = -1; }
				else if ((*t)->left->balance < 0) { LL(&(*t)); grown = false; }
				else { LR(&(*t)); grown = false; }
			}
		}
		else
			if ((*t)->data < d) {
				AddAVL(&((*t)->right), d);
				if (grown == true) {
					if ((*t)->balance < 0) { (*t)->balance = 0; grown = false; }
					else if ((*t)->balance == 0) { (*t)->balance = 1; }
					else if ((*t)->right->balance > 0) { RR(&(*t)); grown = false; }
					else { RL(&(*t)); grown = false; }
				}
			}
}

void Add2b(Tree** t, int d) {
	Tree* q;
	if ((*t) == NULL) {
		(*t) = (Tree*)malloc(sizeof(Tree));
		(*t)->data = d;
		(*t)->left = (*t)->right = NULL;
		(*t)->balance = 0;
		VR = true;
	}
	else
		if ((*t)->data >= d) {
			Add2b(&((*t)->left), d);
			if (VR == true) {
				if ((*t)->balance == 0) {
					q = (*t)->left;
					(*t)->left = q->right;
					q->right = (*t);
					(*t) = q;
					q->balance = 1;
					VR = false;
					HR = true;
				}
				else {
					(*t)->balance = 0;
					VR = true;
					HR = false;
				}
			}
			else HR = false;
		}
		else
			if ((*t)->data < d) {
				Add2b(&((*t)->right), d);
				if (VR == true) {
					(*t)->balance = 1;
					VR = false;
					HR = true;
				}
				else if (HR == true) {
					if ((*t)->balance == 1) {
						q = (*t)->right;
						(*t)->balance = 0;
						q->balance = 0;
						(*t)->right = q->left;
						q->left = (*t);
						(*t) = q;
						VR = true;
						HR = false;
					}
					else HR = false;
				} 
			}
}