/* Implementation of the grep command*/
#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<dirent.h>
#include"list.h"
#include"queue1.h"
int l = 0, t = 1, g = 0, k1 = 0, status = 0, pos = 0, count2 = 0, b = 0, nomode = 1, grepc = 0, grepv = 0, greph = 0, grepw = 0, grepb = 0, grepr = 0, grepH = 0, grepi = 0, grepm = 0, grepq = 0, grepf = 0, grepe = 0, indexh = 0, indexH = 0, carat = 0, dollar = 0, ecount = 0;
char *result;
/* checkopt function checks for the modes that are present and sets the variable(or flags) to 1 */
void checkopt(char *argv, int j1) {
	int j2 = 0;
	while(argv[j2] != '\0') {
		if((argv[j2] == 'c') && (argv[0] == '-'))		/* Display the count of lines */
			grepc = 1;
		if((argv[j2] == 'v') && (argv[0] == '-'))		/* Gives the non-matching lines */
			grepv = 1;
		if((argv[j2] == 'h') && (argv[0] == '-')) {		/* Does not allow the names of files to get printed */
			greph = 1;
			indexh = j1;
		}
		if((argv[j2] == 'w') && (argv[0] == '-'))		/* Gives the lines having the pattern as a whole word */
			grepw = 1;
		if((argv[j2] == 'b') && (argv[0] == '-'))		/* Gives the byte offset of the line */
			grepb = 1;
		if((argv[j2] == 'H') && (argv[0] == '-')) {		/* The names of files get printed */
			grepH = 1;
			indexH = j1;
		}
		if((argv[j2] == 'i') && (argv[0] == '-'))		/* Gives matches irrespective of cases */
			grepi = 1;
		if((argv[j2] == 'm') && (argv[0] == '-'))		/* Matches the first specified number of lines */
			grepm = 1;
		if((argv[j2] == 'q') && (argv[0] == '-'))		/* Exits froms the program on the first match */
			grepq = 1;
		if((argv[j2] == 'f') && (argv[0] == '-'))		/* Takes pattern from a file */
			grepf = 1;
		if((argv[j2] == 'r') && (argv[0] == '-'))		/* Searches the pattern in all the files in the directory as well subdirectories */
			grepr = 1;
		if((argv[j2] == 'e') && (argv[0] == '-'))		/* Allows multiple patterns */
			grepe = 1;
		j2++;
	}
}
/* searchstrings searches for the word in the line, it returns the line if the pattern or word is found */
char *searchstrings(char *line, char *pattern) {
	int j = 0, k = 0, l = -1, len1 = 0, len2 = 0;
	len1 = strlen(pattern);
	while(line[j] != '\0') {
		k = 0;
		l = j;
		while(pattern[k] != '\0') {
			if(line[j] == pattern[k]) {
				len2++;
				j++;
			}
			else {
				j = l;
				break;
			}
			k++;
		}
		if(len1 == len2) {
			if(carat == 1 && j - len1 == 0) {		/* This part deals with the carat part of the regex matching */
				return line;
				break;
			}
			else if(dollar == 1 && j - strlen(line) == 0) {		/* This part deals with the dollar part of the regec matching */
				return line;
				break;
			}
			else if(carat == 0 && dollar == 0) {		/* This part deals with the normal matching (without carat and dollar) */
				return line;
				break;
			}
		}
		len2 = 0;
		j++;
	}
	return "NOT FOUND";
}
/* modev deals with the -v mode, it returns the line if the word is not found*/
char *modev(char *line, char *pattern) {
	int j = 0, k = 0, l = -1, len1 = 0, len2 = 0, flag = 0;
	static int count = 0;
	len1 = strlen(pattern);
	while(line[j] != '\0') {
		k = 0;
		l = j;
		while(pattern[k] != '\0') {
			if(line[j] == pattern[k]) {
				len2++;
				j++;
			}
			else {
				j = l;
				break;
			}
			k++;
		}
		if(len1 == len2) {
			flag = 1;
			break;
		}
		len2 = 0;
		j++;
	}
	if(flag == 0) {
		return line;
	}
	else {
		return "NOT FOUND";
	}
}
/* modew deals with the -w mode, it returns the line if the searched pattern is found as a separate word in the line */
char *modew(char *line, char *pattern) {
	int j = 0, k = 0, h = 0, l = -1, len1 = 0, len2 = 0;
	len1 = strlen(pattern);
	while(line[j] != '\0') {
		k = 0;
		l = j;
		h = 0;
		while(pattern[k] != '\0') {
			if(line[j] == pattern[k]) {
				len2++;
				j++;
				h++;
			}
			else {
				j = l;
				break;
			}
			k++;
		}
		if(((len1 == len2) && ((line[j] == ' ' && line[j - len2 - 1] == ' ') || (line[j] == '\0' && (line[j - len2 - 1] == ' ' || j - len2 == 0)) || (j - len2 == 0 && line[j] == ' ')))) {
			if(carat == 1 && j - len1 == 0) {		/* This part deals with the carat part of the regex matching */
				return line;
				break;
			}
			else if(dollar == 1 && j - strlen(line) == 0) {		/* This part deals with the carat part of the regex matching */
				return line;
				break;
			}
			else if(carat == 0 && dollar == 0) {		/* This part deals with the normal part of matching (without carat and dollar) */
				return line;
				break;
			}
		}
		len2 = 0;
		j++;
	}
	return "NOT FOUND";
}
/* printgrep deals with the printing part of the project, it deals with all the modes which influence printing */
void printgrep(list k, char *filename, int argc) {
	node *q;
	q = (node *)malloc(sizeof(node));
	if(grepq == 1) {
		exit(0);
	}
	if(grepc == 1) {
		if(argc - l - 2 == 1 && grepr == 0) {
			if(grepH == 1 && greph == 0)
				printf("%s:%d\n", filename, listlength(&k));
			else
				printf("%d\n",listlength(&k));
		}
		else if(argc - l - 2 > 1 || grepr == 1) {
			if(greph == 1)
				printf("%d\n", listlength(&k));
			else
				printf("%s:%d\n", filename, listlength(&k));
		}
	}
	if(grepb == 1) {
		q = k.head;
		while(q != NULL) {
			if(grepc == 0) {
				if(argc - l - 2 == 1 && grepH == 0)
					printf("%d:%s\n", q->byte, q->c);
				else if(argc - l - 2 > 1 || grepH == 1)		
					printf("%s:%d:%s\n", filename, q->byte, q->c);
			}
			q = q->next;
		}
	}
	if(grepc == 0 && grepb == 0) {
		q = k.head;
		while(q != NULL) {
			if(indexH > indexh)
				printf("%s:%s\n", filename, q->c);
			else if(indexh > indexH)
				printf("%s\n", q->c);
			q = q->next;
		}
	}
	if(grepc == 0 && greph == 0 && grepH == 0 && grepb == 0) {
		if((grepv == 1 || grepw == 1 || grepi == 1) && (l > 1 && l <= 3)) {
			q = k.head;
			while(q != NULL) {
				if(argc - l - 2 == 1 && grepr == 0)
					printf("%s\n", q->c);
				else if(argc - l - 2 > 1 || grepr == 1)
					printf("%s:%s\n", filename, q->c);
				q = q->next;
			}
		}
	}
	if(grepc == 0 && grepH == 0 && greph == 0 && grepb == 0) {
		if(nomode == 1 || ((grepv == 1 || grepw == 1 || grepi == 1 || grepr == 1 || grepf == 1) && l == 1) || (grepm == 1 && l == 2) || ecount > 0) {
			q = k.head;
			while(q != NULL) {
				if(argc - l - 2 == 1 && grepr == 0)
					printf("%s\n", q->c);
				else if(argc - l - 2 > 1 || grepr == 1)
					printf("%s:%s\n", filename, q->c);
				q = q->next;
			}
		}
	}
}
/* checkpattern combines the modes which influence the matching in the project */
void checkpattern(list *q, char *c, char *pattern1) {
	char *c2, *pattern2;
	c2 = (char *)malloc(sizeof(char) * 200);
	pattern2 = (char *)malloc(sizeof(char) * 200);
	if(grepi == 1) {				/* Here, the pattern as well as the line gets converted to lower case and then the searching occurs */
		while(c[k1] != '\0') {
			c2[k1] = tolower(c[k1]);
			k1++;
		}
		c2[k1] = c[k1];
		strcpy(c, c2);
		k1 = 0;
		while(pattern1[k1] != '\0') {
			pattern2[k1] = tolower(pattern1[k1]);
			k1++;
		}
		pattern2[k1] = pattern1[k1];
		strcpy(pattern1, pattern2);
	}
	free(c2);
	free(pattern2);
	if(grepw == 1) {
		result = modew(c, pattern1);
		if(grepv == 0) {
			if(strcmp(result, c) == 0) {
				status = 1;
			}
		}
		else if(grepv == 1) {
			if(strcmp(result, c) != 0) {
				status = 1;
			}
			else {
				status = 0;
				g = 0;
				return;
			}
		}
	}
	else {
		result = searchstrings(c, pattern1);
		if(grepv == 0) {
			if((strcmp(result, c) == 0)) {
				status = 1;
			}
		}
		else if(grepv == 1) {
			if(strcmp(result, c) != 0) {
				status = 1;
			}
			else {
				status = 0;
				g = 0;
				return;
			}
		}
	}
}
/* modes deals with combining of -f with rest of the modes */
void modes(list *k, char *filename, char *pattern1, char *c6, char *c) {
	char ch2;
	int fc;
	if(grepf == 1) {
		g = 0;
		fc = open(filename, O_RDONLY);
		while(read(fc, &ch2, 1)) {	/* This part separates the lines of the file having patterns */
			pattern1[g] = ch2;	/* Every pattern gets stored in the same string, one after another.*/
			if(ch2 == '\n') {	/* Once the searching of one pattern gets completed, another pattern is obtained for searching */
				pattern1[g] = '\0';
				checkpattern(k, c, pattern1);
				if(grepv == 1) {
					if(status == 0) {
						g = 0;
						break;
					}
				}
				g = 0;
				continue;
			}
			g++;
		}
		close(fc);
		if(status == 1) {
			insert(k, c6, b - 1, pos);	/* If even a single pattern is found in the line, then line is inserted in a list */
			count2++;
			pos++;
		}
		status = 0;
	}
	else {						/* This part is executed if -f is not present */
		checkpattern(k, c, pattern1);
		if(status == 1) {
			insert(k, c6, b - 1, pos);		/* If the pattern is present in the line, then the line is inserted in a list */
			count2++;
			pos++;
		}
		status = 0;
	}
}
/* This part deals with the -r mode */
void opendirectories(queue *q1, const char *files) {
	data d;
	struct dirent *dp;
	DIR *dir;
	if(!(dir = opendir(files))) {
		printf("File or folder does not exist\n");
		return;
	}
	while((dp = readdir(dir)) != NULL) {
		char line1[1024], line2[1024];
		if(dp->d_type == DT_REG) {		/* If readdir finds a regular file, this part is executed */
			char copyh[1025];
			strcpy(copyh, files);
			strcat(copyh, "/");
			strcat(copyh, dp->d_name);
			strcpy(d.name, copyh);
			enq(q1, d);			/* The path of the directory gets stored in a queue */
		}
		if(dp->d_type == DT_DIR) {		/* If readdir finds a directory, this part is executed */
			if((strcmp(dp->d_name, ".") == 0) || (strcmp(dp->d_name, "..") == 0)) {
				continue;
			}
			strcpy(line2, files);
			strcat(line2, "/");
			strcat(line2, dp->d_name);
			strcpy(line1, line2);
			opendirectories(q1, line1);		/* Here, recursion occurs, the directory found by readdir is again passed to the same function */
		}
	}
	closedir(dir);
}
/* regex function identifies the regular expression part, the meta characters and sets variables (or flags) to 1 and modifies the pattern */
queue regex(char *create) {
	char *create2;
	create2 = (char *)malloc(sizeof(char) * 50);
		queue tempq;
	qinit(&tempq);
	data d2;
	int i = 0, j = 0, k = 0, length, brackcount = 0;;
	length = strlen(create);
	j = 0;
	while(create[j] != '\0') {
		if(create[0] == '^' && j == 0) {		/* This part deals with the carat */
			carat = 1;
			i = j;
			while(i < length - 1) {
				create2[i] = create[i + 1];
				i++;
			}
			create2[i] = '\0';
		}
		i = 0;
		if(create[length - 1] == '$' && j == length - 1) {	/* This part deals with the dollar */
			dollar = 1;
			while(i < length - 1) {
				create2[i] = create[i];
				i++;
			}
			create2[i] = '\0';
		}
		j++;
	}
	if(carat == 0 && dollar == 0 && brackcount == 0) {
		strcpy(create2, create);
	}
	strcpy(d2.name, create2);
	enq(&tempq, d2);
	return tempq;
}
int main(int argc, char *argv[]) {
	int fd, fp, fc, count = 0, j = 0, j1 = 0, j2 = 0, i = 0, k2 = 0, k3 = 0, m = 0, h = 0, h1 = 0, pos1 = 0;
	char ch, ch2, *count1, *c, *c2, *c3, *c6, *result2, *file, *pattern1, *pattern2, *pattern3, *directory, *filename, *filepattern;
	list k;
	init(&k);
	node *q;
	queue q1;
	data d1;
	qinit(&q1);
	node1 *q2;
	queue patternq;
	qinit(&patternq);
	node *q3;
	data d5;
	count1 = (char *)malloc(sizeof(char) * 20);
	pattern1 = (char *)malloc(sizeof(char) * 200);
	result = (char *)malloc(sizeof(char) * 200);
	c = (char *)malloc(sizeof(char) * 200);
	c6 = (char *)malloc(sizeof(char) * 200);
	pattern3 = (char *)malloc(sizeof(char) * 200);
	directory = (char *)malloc(sizeof(char) * 200);
	file = (char *)malloc(sizeof(char) * 200);
	filename = (char *)malloc(sizeof(char) * 200);
	filepattern = (char *)malloc(sizeof(char) * 200);
	h = argc - 1;
	t = 1;
	while(t < argc) {		/* This part deals with the command line adjustments for modes -m, -r and -f */
		j2 = 0;
		while(argv[t][j2] != '\0') {
			if(argv[t][j2] == 'm' && argv[t][0] == '-') {
				strcpy(count1, argv[t + 1]);
				break;
			}
			else if(argv[t][j2] == 'f' && argv[t][0] == '-') {
				strcpy(filename, argv[t + 1]);
				break;
			}
			else if(argv[t][j2] == 'r' && argv[t][0] == '-') {
				strcpy(directory, argv[t + 1]);
				break;
			}
			else if(argv[t][j2] == 'e' && argv[t][0] == '-') {
				strcpy(d5.name, argv[t + 1]);
				enq(&patternq, d5);
				ecount++;
			}
			j2++;
		}
		t++;
	}
	while(i <= h) {						/*This part calculates the number of modes present */
		if(argv[i][0] == '-' || (strcmp(argv[i], count1) == 0) || (strcmp(argv[i], filename) == 0)) {
			l++;
		}
		i++;
	}
	if(ecount == 2) {
		ecount = ecount / 2;
		l = l + ecount;
	}
	else if(count != 0) {
		l = l + ecount - 1;
	}
	j1 = 0;
	j2 = 0;
	while(j1 < argc) {			/* This part calls the checkopt functions */
		checkopt(argv[j1], j1);
		j1++;
	}
	count = atoi(count1);
	t = 1;
	if(grepf != 1) {
		while(t < argc) {
			if(argv[t][0] != '-' && (strcmp(argv[t], count1) != 0)) {
				strcpy(pattern1,argv[t]);
				break;
			}
			t++;
		}
	}
	t = 1;
	while(t < argc) {
		if(argv[t][0] == '-') {
			nomode = 0;
		}
		t++;
	}
	if(grepf == 1) {
		fc = open(filename, O_RDONLY);
		if(fc == -1) {
			perror("open failed");
			return errno;
		}
		l--;
	}
	if(grepr == 1) {
		opendirectories(&q1, directory);
	}
	t = 0;
	patternq = regex(pattern1);		/* patternq has the pattern which it gets after the regex functions works on it */
	data d4;
	while(!qempty(&patternq)) {
		d4 = deq(&patternq);		/* Each element of patternq gets dequeued one by one for searching */
		pattern1 = d4.name;
		if(grepr == 1) {		/* This part gets called if -r is present */
			while(!qempty(&q1)) {
				d1 = deq(&q1);
				strcpy(file, d1.name);
				fd = open(file, O_RDONLY);
				if(fd == -1) {
					perror("open failed");
					continue;
				}
				k2 = 0;
				free(c);
				c = malloc(sizeof(int) * 200);
				count2 = 0;
				while(read(fd, &ch, 1)) {
					if(((grepm == 1) && (count2 == count)) || ((grepq == 1) && (count2 > 0))) {		/* This deals with the -m and -q modes */
						break;
					}
					c[j] = ch;
					k2++;
					if(ch == '\n') {
						c[j] = '\0';
						k1 = 0;
						strcpy(c6, c);
						b = k2 - strlen(c6);			/* This part deals with the -b mode by calculating byte offset */
						modes(&k, filename, pattern1, c6, c);
						j = 0;
						carat = 0;
						dollar = 0;
						continue;
					}
					j++;
				}
				close(fd);
				printgrep(k, file, argc);
				pos = 0;
				init(&k);
			}
		}
		else {								/*This part gets called if -r is not present */
			while(t < argc - l - 2) {
				fd = open(argv[l + t + 2], O_RDONLY);
				if(fd == -1) {
					perror("open failed");
					return errno;
				}
				k2 = 0;
				c = realloc(c, sizeof(int) * 200);
				count2 = 0;
				while(read(fd, &ch, 1)) {
					if(((grepm == 1) && (count2 == count)) || ((grepq == 1) && (count2 > 0))) {
						break;
					}
					c[j] = ch;
					k2++;
					if(ch == '\n') {
						c[j] = '\0';
						k1 = 0;
						strcpy(c6, c);
						b = k2 - strlen(c6);
						modes(&k, filename, pattern1, c6, c);
						j = 0;
						continue;
					}
					j++;
				}
				close(fd);
				printgrep(k, argv[l + t + 2], argc);
				pos = 0;
				t++;
				init(&k);
			}
		}
	}
	return 0;
}
