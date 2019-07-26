#define QSIZE 17
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include"queue1.h"
void qinit(queue *q) {
	q->head = q->tail = NULL;
}
int qempty(queue *q) {
	return q->head == NULL;
}
void enq(queue *q, data d) {
	node1 *tmp;
	tmp = (node1 *)malloc(sizeof(node1));
	data d1;
	strcpy(tmp->d.name, d.name);
	if(q->head == NULL) {
		q->head = q->tail = tmp->next = tmp;
		return;
	}
	tmp->next = q->head;
	q->tail->next = tmp;
	q->head = tmp;
}
void qprint(queue *q) {
	node1 *p;
	if(q->head == NULL) {
		printf("QUEUE EMPTY\n");
		return;
	}
	p = q->head;
	do {
		printf("%s\n", p->d.name);
		p = p->next;
	}while(p != q->head);
}
data deq(queue *q) {
	data d3, err;
	strcpy(err.name, "QUEUE EMPTY");
	node1 *j, *p;
	if(q->head == NULL) {
		return err;
	}
	if(q->head == q->tail) {
		strcpy(d3.name, q->tail->d.name);
		q->head = q->tail = NULL;
		return d3;
	}
	strcpy(d3.name, q->tail->d.name);
	j = q->tail;
	while(j->next != q->tail) {
		j = j->next;
	}
	q->tail = j;
	j->next = q->head;
	return d3;
}
