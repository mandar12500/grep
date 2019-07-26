typedef struct node {
	char c[110];
	int byte;
	struct node *next;
}node;
typedef struct list {
	node *head;
}list;
void init(list *l);
void insert(list *l, char *str, int byte1, int pos);
char *remov(list *l, int pos);
void sort(list *l);
void reverse(list *l);
void printlist(list *l);
int listlength(list *l);
