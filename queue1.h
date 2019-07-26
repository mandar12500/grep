typedef struct data {
	char name[16];
}data;
typedef struct node1 {
	data d;
	struct node1 *next;
}node1;
typedef struct queue {
	node1 *head, *tail;
}queue;
void qinit(queue *q);
int qempty(queue *q);
void enq(queue *q, data d);
data deq(queue *q);
int qfull(queue *q);
void qprint(queue *q);
