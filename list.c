#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"list.h"
void init(list *l) {
	l->head = NULL;
}
void insert(list *l, char *str, int byte1, int pos) {
	int len = 0;
	len = listlength(l);
	if(pos < 0 || pos > len) {
		return;
	}
	node *p, *tmp;
	tmp = (node *)malloc(sizeof(node));
	int j = 0;
	p = l->head;
	strcpy(tmp->c, str);
	tmp->byte = byte1;
	if(l->head == NULL && pos == 0) {
		l->head = tmp;;
		tmp->next = NULL;
		return;
	}
	else if(l->head != NULL && pos == 0) {
		tmp->next = p;
		l->head = tmp;
		return;	
	}
	while(j < pos - 1) {
		p = p->next;
		j++;
	}
	tmp->next = p->next;
	p->next = tmp;
}
char *remov(list *l, int pos) {
	char ch[111], *tmp;
	int j = 0, len = 0;
	len = listlength(l);
	if(pos < 0 || pos > len - 1) {
		return "Position Invalid";
	}
	node *p, *q;
	p = l->head;
	if(l->head == NULL) {
		return "List Empty";
	}
	if(pos == 0) {
		if(len == 1) {
			l-> head = NULL;
			return p->c;
		}
		else {
			l->head = p->next;
			tmp = p->c;
			//free(p);
			return tmp;
		}
	}
	while(j < pos - 1) {
		p = p->next;
		j++;
	}
	q = p->next;
	p->next = q->next;
	p = q->next;
	tmp = q->c;
	//printf("%s", tmp);
	//free(q);
	return q->c;
}
void reverse(list *l) {
	node *p, *q, *r;
	p = l->head;
	q = p->next;
	if(q == NULL) {
		return;
	}
	r = q->next;
	if(l->head == NULL) {
		return;
	}
	p->next = NULL;
	while(r != NULL) {
		q->next = p;
		p = q;
		q = r;
		r = r->next;
	}
	l->head = q;
	q->next = p;
}
void sort(list *l) {
	int len = 0, j = 0, h = 0;
	len = listlength(l);
	if(len == 0 || len == 1) {
		return;
	}
	char ch[111];
	node *p, *q;
	p = l->head;
	q = l->head;
	while(q->next != NULL) {
		p = l->head;
		while(p->next != NULL) {
			if(strcmp(p->c, p->next->c) > 0) {
				strcpy(ch, p->c);
				strcpy(p->c, p->next->c);
				strcpy(p->next->c, ch);
			}
			p = p->next;
		}
		q = q->next;
	}
}
void printlist(list *l) {
	node *p;
	p = l->head;
	if(!p) {
		printf("]\n");
		return;
	}
	do {
		printf("%s\n", p->c);
		p = p->next;
	}while(p != NULL);
}
int listlength(list *l) {
	int length = 0;
	node *p;
	p = l->head;
	while(p != NULL) {
		p = p->next;
		length++;
	}
	return length;
}
