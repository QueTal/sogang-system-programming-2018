#include"20151521.h"

/*-----------------------------------*/
/*함수 : main*/
/*목적 : 사용자로부터 입력을 받고 명령에
	맞는 함수를 실행한다.*/
/*리턴값 : 제대로 종료될 시 0*/
/*-----------------------------------*/
int main(void) {
	char ch[256] = { 0, }, copy[256] = { 0, };
	int i, j, flag = 0, key = 0;
	long long start = 0, end = 0, value = 0;
	char *tok, *real, *temp, *add;

	/* make hash table */
	makeHash();
	while (1) {
		printf("sicsim> ");

		/* save the input */
		for (i = 0; i < 256; i++) {
			scanf("%c", &ch[i]);
			/*count the number of ','*/
			if (ch[i] == ',')	flag++;
			//prevent segmentation fault
			if (ch[0] == '\n')	break;
			//change last enter to null
			if (ch[i] == '\n') {
				ch[i] = '\0';
				break;
			}
		}

		/* copying the input value to other array with some transforming */
		for(i = 0, j = 0; i < strlen(ch); i++, j++){
			copy[j] = ch[i];
			if(ch[i] == '\t')	copy[j] = ' ';
			if(ch[i] == ','){
				j++; copy[j] = ' ';
			}
		}	
	
		/* trim the input data by tokenize */
		tok = strtok (copy, " ");
		real = tok;
		/*save the mnemonic in variable real */
		while (1) {
			/* save the arguments in variable tok if the instruction has*/
			tok = strtok (NULL, " ");

			/* instruction without argument */
			if (tok == NULL) { 
				/* find the same menmonic by comparing with variable real*/
				if (strcmp (real, "help") == 0 || strcmp (real, "h") == 0) {
					/* save the instruction and execute the function */
					makeHistory(ch);
					help();
					break;
				}
				else if (strcmp (real, "dir") == 0 || strcmp (real, "d") == 0) {
					/* save the instruction and execute the function */
					makeHistory(ch);
					dir();
					break;
				}
				else if (strcmp (real, "history") == 0 || strcmp (real, "hi") == 0) {
					/* save the instruction and execute the function */
					makeHistory(ch);
					printHistory();
					break;
				}
				else if (strcmp (real, "quit") == 0 || strcmp (real, "q") == 0){
					/* free the allocated memory and exit the program */
					freeHistory(); 
					freeHash();
					freeSymTable();
					return 0;
				}
				else if(strcmp(real, "opcodelist") == 0){
					/* save the instruction and execute the function */
					makeHistory(ch);
					opcodelist();
					break;
				}
				else if(strcmp(real, "dump") == 0 || strcmp(real, "du") == 0){
					/* save the instruction and execute the function */
					makeHistory(ch);
					dump(finish + 1, finish + 160);
					finish += 160;
					break;
				}
				else if(strcmp(real, "reset") == 0){
					/* save the instruction and initialize the memory */
					makeHistory(ch);
					memset(memo, 0, 256); 
					break;
				}
				else if(strcmp(real, "symbol") == 0){
					if(assembleFlag == 0){
						printSymbol();
						makeHistory(ch);
					}
					else printf("The symbol table is empty!\n");
					break;
				}
				else {
					/* print the error message if the input is wrong */
					printf ("Invalid input!\n");
					break;
				}
			}

			/*instruction with argument*/
			else { 
				/* find the same menmonic by comparing with variable real */
				if(strcmp(real, "dump") == 0 || strcmp(real, "du") == 0){
					real = tok;
					tok = strtok(NULL, " ");
					/* save the argument to real and tok */
					/* for one argument*/
					if(flag == 0 && tok == NULL && real != NULL){
					/* not consider the value of real is right yet */
						start = strtol(real, &temp, 16);
						/* change the string to hexadecimal number to execute the function dump */
						if(strcmp(temp, "\0") == 0 && start >= 0 && start <= 0xFFFFF){ 
							makeHistory(ch);
							/* save the last address to variable finish */
							finish = 159 + start;
							dump(start, finish);
							break;
						}
						else{
						/* print the error message if the input is wrong */
							printf("Invalid input!\n");
							break;
						}
					}
					/*for two argument*/
					else if(flag == 1 && tok != NULL && real != NULL){
						start = strtol(real, &temp, 16);
						if(strcmp(temp, ",") != 0){
							/* if there is any other character that does not handle in hexadecimal */
							/* print the error message if the input is wrong */
							printf("Invalid input!\n");
							break;
						}
						end = strtol(tok, &temp, 16);
						if(strcmp(temp, "\0") == 0 && start >= 0 && end <= 0xFFFFF && start < end){
							/* if there no error point when transform string to hexadecimal */
							/* if the range of input is valid*/
							makeHistory(ch);
							finish = end;
							dump(start, end);
							break;
						}
						else{
							/* print the error message if the input is wrong */
							printf("Invalid input!\n");
							break;
						}
					}
					else{
						/* print the error message if the input is wrong */
						printf("Invalid input!\n");
						break;
					}
				}
				/*execute fill instruction*/
				else if(strcmp(real, "fill") == 0 || strcmp(real, "f") == 0){
					/* save the argument to real, add, tok */
					real = tok;
					tok = strtok(NULL, " ");
					add = strtok(NULL, " ");
					if(flag == 2 && real != NULL && tok != NULL && add != NULL){
						start = strtol(real, &temp, 16);
						if(strcmp(temp, ",") != 0){
							/* if there is any other character that does not handle in hexadecimal */
							/* print the error message if the input is wrong */
							printf("Invalid input!\n");
							break;
						}
						end = strtol(tok, &temp, 16);
						if(strcmp(temp, ",") != 0){
							/* if there is any other character that does not handle in hexadecimal */
							/* print the error message if the input is wrong */
							printf("Invalid input!\n");
							break;
						}
						value = strtol(add, &temp, 16);
						if(strcmp(temp, "\0") == 0 && start >= 0 && end <= 0xFFFFF && start < end && 0 <= value && value <= 0xFF){
							/* if the instruction is valid, save the instruction to the history and execute the function */
							makeHistory(ch);
							fill(start, end, value);
							break;	
						}
						else{
							/* print the error message if the input is wrong */
							printf("Invalid input!\n");
							break;
						}
					}
				}
				/*execute edit instruction*/
				else if(strcmp(real, "edit") == 0 || strcmp(real, "e") == 0){
					/* save the arguments to the variables */
					real = tok;
					tok = strtok(NULL, " ");
					if(flag == 1 && real != NULL && tok != NULL){
						start = strtol(real, &temp, 16);
						if(strcmp(temp, ",") != 0){
							/* print the error message if the input is wrong */
							printf("Invalid input!\n");
							break;
						}
						value = strtol(tok, &temp, 16);
						if(strcmp(temp, "\0") == 0 && start >= 0 && start <= 0xFFFFF && value <= 0xFF && 0 <= value){
							/* if the instruction is valid, save the instruction to the history and execute the function */
							makeHistory(ch);
							edit(start, value);
							break;
						}
						else{
						/* print the error message if the input is wrong */
							printf("Invalid input!\n");
							break;
						}
					}
					else{
					/* print the error message if the input is wrong */
						printf("invalid input!\n");
						break;
					}
				}

				/*execute opcode instruction*/
				else if(strcmp(real, "opcode") == 0){
					for(i = 0; i < strlen(tok); i++){
						/* add the ascii code and mod the value and save it to the variable to make key value */
						if(tok[i] >= 65 && tok[i] <= 90)	key += (int)tok[i];
						else{
							/* print the error message if the input is wrong */
							printf("Invalid input!\n");
							flag = 1;
							break;
						}
					}
					if(flag != 1){
						/* if the instruction is valid, save the instruction to the history and execute the function */
						makeHistory(ch);
						if(findOpcode(tok, (int)(key % 20)) == 1)	printf("Invalid input!\n");
						else printf("opcode is %-.2X.\n", findOpcode(tok, (int)(key % 20)));
					}
					break;
				}
				else if(strcmp(real, "type") == 0){
					if(type(tok) == 0)	makeHistory(ch);
					break;
				}
				else if(strcmp(real, "assemble") == 0){
					assembleFlag = 0;
					freeSymTable();
					pass1(tok);
					if(assembleFlag == 1){
						printf("assemble failure!\n");
						freeSymTable();
						fileDelete();
						break;
					}
					pass2(tok);
					if(assembleFlag == 1){
						printf("assemble failure!\n");
						real = strtok(tok, ".");
						strcat(real, ".lst");
						remove(real);
						freeSymTable();
						fileDelete();
						break;
					}
					ObjectFile(tok);
					fileDelete();
					if(assembleFlag != 1) makeHistory(ch);
					real = strtok(tok, ".");
					printf("output file: [%s.lst], [%s.obj]\n", real, real);
					break;
				}
				else {
					/* print the error message if the input is wrong */
					printf("Invalid input!\n");
					break;
				}
			}
		}
		/*initialize variables*/
		memset(ch, 0, 256);	memset(copy, 0, 256);
		flag = 0; key = 0;
	}
	return 0;
}

/*---------------------------------------*/
/*함수 : help */
/*목적 : 지원하는 명령어를 모두 출력한다.*/
/*리턴 값 : 없음.*/
/*---------------------------------------*/
void help(){
	printf("h[elp]\n");
	printf("d[ir]\n");
	printf("q[uit]\n");
	printf("hi[story]\n");
	printf("du[mp] [start, end]\n");
	printf("e[dit] address, value\n");
	printf("f[ill] start, end, value\n");
	printf("reset\n");
	printf("opcode mnemonic\n");
	printf("opcodelist\n");
	printf("assemble filename\n");
	printf("type filename\n");
	printf("symbol\n");
}

/*-----------------------------------------------*/
/*함수 : dir*/
/*목적 : 디렉토리내의 파일과 디렉토리를 표시한다.*/
/*리던값 : 없음.*/
/*-----------------------------------------------*/
void dir(){
	DIR *dr = opendir(".");
	struct dirent *de;
	struct stat sb;

	while((de = readdir(dr)) != NULL){
		/* exception control */
		if(strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)	continue;
		stat(de->d_name, &sb);
		/* check the file whether it is directory or executable file or just file */
		if(S_ISDIR(sb.st_mode))	printf("%s/\n", de->d_name);
		else if(S_IEXEC & sb.st_mode)	printf("%s*\n", de->d_name);
		else if(S_ISREG(sb.st_mode))	printf("%s\n", de->d_name);
	}
	closedir(dr);
}

/*-----------------------------------------------------*/
/*함수 : dump*/
/*목적 : 사용자의 입력에 맞춰 메모리의 값들을 출력한다.*/
/*리턴값 : 없음.*/
/*-----------------------------------------------------*/
void dump(long long  last, long long end){
	int i = (last / 16) * 16, j = i, k = i, temp = i, cnt;

	/* confirm the number of line to print out */
	cnt = (end - last + 1) / 16;
	if(last % 16 != 0)	cnt++;
	if(end % 16 == 0) cnt++;

	while(cnt > 0){
		/* print the left most column */
		printf("%.5X ", i);
		
		/* print the middle column */
		for(; j < i + 16; j++){
			if(j < last || j > end)	printf("   ");
			else	printf("%.2X ", memo[j]);
			if(j == 1048576) 	break;
		}
		i = j;
		printf("; ");

		/* print the right most column */
		j = temp;
		for(; j < k + 16; j++){
			if(j < last || j > end)	printf(".");
			else if(32 <= memo[j] && memo[j] <= 127)	printf("%c", memo[j]);
			else printf(".");
			if(j == 1048576)	break;
		}
		k = j; temp = j;
		printf("\n");	

		cnt--;
		/* exception control */
		if(j == 1048576){
			finish = -1;
			return;
		}
	}
}

/*------------------------------------------*/
/*함수 : edit*/
/*목적 : 원하는 지점의 메모리의 값을 바꾼다.*/
/*리턴값 : 없음.*/
/*------------------------------------------*/
void edit(long long add, long long val){
	memo[add] = val;
}

/*-----------------------------------------------*/
/*함수 : fill*/
/*목적 : 원하는 메모리의 값들을 val 값으로 채운다.*/
/*리턴값 : 없음.*/
/*-----------------------------------------------*/
void fill(long long start, long long end, long long val){
	int i;

	for(i = start; i <= end; i++){
		memo[i] = val;
	}
}

/*-----------------------------------------------*/
/*함수 : makeHash*/
/*목적 : opcode.txt로 부터 정보를 읽어 hashNode로 
  	인자를 보내 hash table을 만든다.*/
/*리턴값 : 없음.*/
/*-----------------------------------------------*/
void makeHash() {
	FILE *fp = fopen ("opcode.txt", "r");
	int i, opcode, val = 0;
	char mnemo[10] = {0,}, format[10] = {0,}, buffer[30] = {0,}, *op, *temp;

	/* read the content of file line by line */
	fgets (buffer, 30, fp);
	while (!feof (fp)) {
		/* remove the '\t' to use function strtok */
		for (i = 0; i < 30; i++) {
			if (buffer[i] == '\t')	buffer[i] = ' ';
		}

		/* find and save the value by using strtok */
		op = strtok (buffer, " ");
		temp = strtok (NULL, " ");
		strcpy(mnemo, temp);
		temp = strtok (NULL, " ");
		strcpy(format, temp);
		opcode = strtol (op, &temp, 16);

		/* add the ascii code value to varibale and divide it by 20 to make the key value */
		for (i = 0; i < strlen (mnemo); i++) {
			val += mnemo[i];
		}
		val %= 20;

		/* call the function with argument to make hash table */
		hashNode (val, opcode, mnemo, format);

		/* initialize the variable to use again */
		memset(buffer, 0, 30); memset(mnemo, 0, 10); memset(format, 0, 10);
		fgets (buffer, 30, fp); val = 0;
	}
	fclose(fp);
}

/*----------------------------------------------*/
/*함수 : hashNode*/
/*목적 : 새로운 노드에 mnemonic의 정보를 저장하고
 	이를 linked list로 hash table로 만든다.*/
/*리턴값 : 없음.*/
/*----------------------------------------------*/
void hashNode (int val, int opcode, char* mnemo, char* format) {
	Table* new = (Table*)malloc(sizeof(Table));	
	Table* temp;

	/* save the argument to the new node */
	new->opcode = opcode;
	strcpy(new->mnemo, mnemo);
	strcpy(new->format, format);
	new->next = NULL;

	/* connect the node to head and make linked list */
	if(head[val] == NULL)	head[val] = new;
	else{
		temp = head[val];
		while(1){
			/* find the end of the linked list */
			if(temp->next == NULL) break;
			temp = temp->next;
		}
		temp->next = new;
	}
}

/*-------------------------------------------------*/
/*함수 : opcodelist*/
/*목적 : hash table로 만든 opcode table을 출력한다.*/
/*리턴값 : 없음.*/
/*-------------------------------------------------*/
void opcodelist(){
	int i;
	Table* temp;

	/* just print the hash table to show the opcode list */
	for(i = 0; i < 20; i++){
		printf("%3d: ", i);
		/* if there is no bucket */
		if(head[i] == NULL){
			printf("\n");
			continue;
		}
		else{
			temp = head[i];
			while(1){
				printf("[%s, %.2X]", temp->mnemo, temp->opcode);
				if(temp->next == NULL)	break;
				temp = temp->next;
				printf(" -> ");
			}
		}
		printf("\n");
	}
}

/*----------------------------------*/
/*함수 : findOpcode*/
/*목적 : 입력받은 mnemonic을 찾는다.*/
/*리턴값 : opcode를 찾으면 0, 없으면 1을 반환한다.*/
/*----------------------------------*/
int findOpcode(char* op, int key){
	Table* temp;
	
	/* if the head of the linked list is NULL*/
	if(head[key] == NULL){
		return 1;
	}
	else{
		temp = head[key];
		while(1){
			/*  print the opcode if find the exact mnemonic */
			if(strcmp(temp->mnemo, op) == 0){
				return temp->opcode;
			}
			/* if there is no same mnemonic*/
			if(temp->next == NULL){
				return 1;
			}
			temp = temp->next;
		}
	}
	
}

/*-------------------------------------------*/
/*함수 : makeHistory*/
/*목적 : linked list로 history 목록을 만든다.*/
/*리턴값 : 없음.*/
/*-------------------------------------------*/
void makeHistory(char input[]){
	History* new = (History*)malloc(sizeof(History));
	History* temp;

	/* save the new instruction to the new node */
	strcpy(new->inst, input);
	new->link = NULL;

	/* if the head is NULL */
	if(his_head == NULL)	his_head = new;
	else{
		temp = his_head;
		while(1){
			/* find the end of the linked list*/
			if(temp->link == NULL)	break;
			temp = temp->link;
		}
		temp->link = new;
	}
}

/*-------------------------------*/
/*함수 : printHistory*/
/*목적 : history 목록을 출력한다.*/
/*리턴값 : 없음.*/
/*-------------------------------*/
void printHistory(){
	History* temp;
	int i = 1;
	
	temp = his_head;
	while(1){
		printf("%-7d %s\n", i, temp->inst);
		if(temp->link == NULL) break;
		i++; temp = temp->link;
	}	
}

/*---------------------------------------*/
/*함수 : freeHistory*/
/*목적 : history를 만들기 위해 동적할당한 
 	노드들을 해제한다.*/
/*리턴값 : 없음.*/
/*---------------------------------------*/
void freeHistory(){
	History* temp;

	/*if there is no history list*/
	if(his_head == NULL) return;
	while(1){
		temp = his_head;
		his_head = his_head->link;
		/* if you free all the nodes */
		if(his_head == NULL)	break;
		free(temp);
	}
}

/*-----------------------------------*/
/*함수 : freeHash*/
/*목적 : hsh table을 만들기 위해 
  	동적할당한 노드들을 해제한다.*/
/*리턴값 : 없음.*/
/*-----------------------------------*/
void freeHash(){
	Table* temp;
	int i = 0;

	for(i = 0; i < 20; i++){
		/* if the head is NULL */
		if(head[i] == NULL)	continue;
		while(1){
			temp = head[i];
			head[i] = head[i]->next;
			/* if you free all the nodes*/
			if(head[i] == NULL) break;
			free(temp);
		}
	}
}


/*함수 : type*/
/*목적 : 입력받은 파일의 내용을 출력해준다.*/
/*리턴값 : 제대로 출력하면 0, 문제가 있으면 1 */
int type(char* file){
	FILE* fp = fopen(file, "r");
	char out[256] = {0 ,};
	DIR *dr = opendir(".");
	struct dirent *de;
	struct stat sb;

	if(fp == NULL){
		printf("Input file error!\n");
		return 1;
	}
	
	/*check if input is directory*/
	stat(file, &sb);
	if(S_ISDIR(sb.st_mode)){
		printf("This is a directory!\n");
		return 1;
	}
	while(1){
		if(fgets(out, 256, fp) == NULL) break;
		printf("%s", out);
	}
	fclose(fp); closedir(dr);
	return 0;
}

/*함수 : pass1*/
/*목적 : 입력받은 파일을 읽고 symbol table과 inmmediate 파일을 만든다*/
/*리턴값 : 없음. */
void pass1(char* input){
	FILE* fp1 = fopen(input, "r");
	FILE* fp2 = fopen("temp.txt", "w");
	char out[100] = {0,}, copy[100] = {0,}, temp[20] = {0,};
	char *tok1, *tok2, *tok3;
	int faddr = 0, laddr, i, key = 0, eflag = 0;
	static int sflag = 0;	
	/* error checking */
	if(fp1 == NULL || fp2 == NULL){
		printf("File open error!\n");
		assembleFlag = 1;
		return;
	}

	while(1){
		/* END가 없어도 멈출 수 있게 break함 */
		if(fgets(out, 100, fp1) == NULL) break;
		strcpy(copy, out);

		/* line with no symbol */
		if(out[0] == ' '){
			tok1 = strtok(out, " ");
				
			if(strcmp(tok1, "END") == 0){
				eflag = 1;	
				break;
			}	
			
			if(tok1[0] == '+'){
				fprintf(fp2, "%.4X\t%s", laddr, copy);	
				laddr += 4;
			}
			else {
				for(i = 0; i < strlen(tok1); i++){
					if(tok1[i] == '\n') tok1[i] = '\0';
					key += tok1[i];	
				}
				/* checking format*/
				if(strcmp(tok1, "BASE") == 0)	fprintf(fp2, "\t%s", copy);
				else fprintf(fp2, "%.4X\t%s", laddr, copy);
				
				laddr += retFormat(tok1, key % 20);
			}	
		}	
		
		/* comment line */
		else if(out[0] == '.'){
			fprintf(fp2, "%s", copy);
			memset(out, 100, '\0'); memset(copy, 100, '\0');
			continue;
		}

		/* line with symbol */
		else{ 	
			tok1 = strtok(out, " ");
			strcpy(temp, tok1);
			tok1 = strtok(NULL, " ");
			/* temp : sumbol, tok1 : opcode */
	
			if(strcmp(tok1, "START") == 0){
				tok1 = strtok(NULL, " ");
				if(tok1 == NULL) faddr = 0;
				faddr = strtol(tok1, &tok2, 16);
				laddr = faddr;
				sflag++;
			}

			fprintf(fp2, "%.4X\t%s", laddr, copy);

			/*make symbol table*/
			hashSymbol(temp, laddr);

			if(tok1[0] == '+'){
				laddr += 4;
			}
			else if(strcmp(tok1, "BYTE") == 0){
				tok2 = strtok(NULL, " ");
				if(tok2[0] == 'C'){
					if(strlen(tok2) > 30){
						assembleFlag = 1;
						printf("Too long Charcter for BYTE.\n");
						return;
					}
					laddr += strlen(tok2) - 4;
				}
				else if(tok2[0] == 'X'){
					if(strlen(tok2) > 60){
						assembleFlag = 1;
						printf("Too long Hexadecimal for BYTE.\n");
						return;
					}
					laddr += (strlen(tok2) - 4) / 2 + (strlen(tok2) - 4) % 2;
				}
			}
			else if(strcmp(tok1, "RESB") == 0){
				tok2 = strtok(NULL, " ");
				laddr += strtol(tok2, &tok3, 10);
			}
			else if(strcmp(tok1, "RESW") == 0){
				tok2 = strtok(NULL, " ");
				laddr += 3 * strtol(tok2, &tok3, 10);
			}
			else{
				for(i = 0; i < strlen(tok1); i++){
					if(tok1[i] == '\n')	tok1[i] = '\0';
					key += tok1[i];
				}
				
				/* checking format*/
				laddr += retFormat(tok1, key % 20);
			}
		}
		memset(out, 0, 100); memset(copy, 0, 100); memset(temp, 0, 20);
		tok1 = NULL; tok2 = NULL; key = 0;
	}
	/* checking START */
	if(sflag == 0 || eflag == 0){
		printf("There is no START or END.\n");
		assembleFlag = 1;
		return;
	}
	if(sflag  > 1){
		printf("There is more than one START.\n");
		assembleFlag = 1;
		return;
	}
	Length = laddr - faddr;
	fprintf(fp2, "\t%s", copy);
	fclose(fp1); fclose(fp2);
}


/*함수 : hashSymbol*/
/*목적 : pass1에서 넘어온 symbol을 가지고 hash table을 만들고 중복되면 
 	assembleFlag를 set하고 에러문구 출력과 종료한다. */
/*리턴값 : 없음 */
void hashSymbol(char* sym, int address){
	Symbol* temp, *prev;
	Symbol* new = (Symbol*)malloc(sizeof(Symbol));
	
	/*new node*/
	strcpy(new->symbol, sym);
	new->addr = address;
	new->link = NULL;
	
	/*make linking with easch other*/
	if(sym_head[sym[0] - 'A'] == NULL)	sym_head[sym[0] - 'A'] = new;
	else{
		temp = sym_head[sym[0] - 'A']; prev = temp;

		if(strcmp(sym, temp->symbol) > 0){
			sym_head[sym[0] - 'A'] = new;
			new->link = temp;
			return;
		}
		else if(strcmp(sym, temp->symbol) == 0){
			printf("'%s' this symbol is already in the hash table.\n", sym);
			assembleFlag = 1;
			return;
		}
		else {
			while(temp->link != NULL){
				temp = temp->link;
				if(strcmp(sym, temp->symbol) > 0){
					new->link = temp;
					prev->link = new;
					return;
				}
				else if(strcmp(sym, temp->symbol) == 0){
					printf("'%s' this symbol is already in the hash table.\n", sym);
					assembleFlag = 1;
					return;
				}
				else prev = temp;
			} 
			temp->link = new;
			return;
		}
	}
}


/*함수 : printSymbol */
/*목적 : 입력으로 symbol이 들어왔을때 symbol table을 출력한다. */
/*리턴값 : 없음 */
void printSymbol(){
	int i;
	Symbol* temp;

	for(i = 25; i >= 0; i--){
		if(sym_head[i] == NULL)	continue;
		else{
			temp = sym_head[i];
			while(1){
				printf("\t%s\t%.4X\n", temp->symbol, temp->addr);
				if(temp->link == NULL)	break;
				temp = temp->link;
			}
		}
	}
}


/*함수 : retFormat */
/*목적 : opcode table에 저장한 opcode의 format을 찾아서 반환한다.*/
/*리턴값 : opcode의 format을 반환한다. */
int retFormat(char* op, int key){
	Table* temp;
	char* error;
	int format;

	/* if the head of the linked list is NULL*/
	if(head[key] == NULL){
		return 0;
	}
	else{
		temp = head[key];
		while(1){
			/*  print the opcode if find the exact mnemonic */
			if(strcmp(temp->mnemo, op) == 0){
				format = strtol(temp->format, &error, 10);
				return format;
			}
			/* if there is no same mnemonic*/
			if(temp->next == NULL){
				return 0;
			}
			temp = temp->next;
		}
	}
}

/*함수 : findSymbol */
/*목적 : symbol table에서 해당 symbol이 있는지 확인한다. */
/*리턴값 : 있으면 0 없으면 1을 반환*/
int findSymbol(char* input){
	Symbol* temp;

	temp = sym_head[input[0] - 'A'];

	if(temp == NULL)	return 1;
	else{
		while(1){
			if(strcmp(input, temp->symbol) == 0)	return 0;
			if(temp->link == NULL) return 1;
			temp = temp->link;
		}
	}
}

/*함수 : findSymAddr */
/*목적 : symbol table에 있는 symbol의 주소를 찾고 반환한다. */
/*리턴값 : 입력받은 symbol의 주소 */
int findSymAddr(char* input){
	Symbol* temp;

	temp = sym_head[input[0] - 'A'];
	
	if(temp == NULL)	return -1;
	else {
		while(1){
			if(strcmp(input, temp->symbol) == 0)	return temp->addr;
			if(temp->link == NULL) return -1;
			temp = temp->link;
		}
	}
}


/*함수 : pass2*/
/*목적 : pass1에서 만든 symbol테이블을 가지고 object code를 만들어
 	궁극적으로 .lst파일을 만들고 .obj 파일을 만들기 위한 임시파일을 생성한다.*/
/*리턴값 : 없음. */

void pass2(char* input){
	FILE* fp1 = fopen("temp.txt", "r");
	FILE *fp2, *fp4 = fopen("tempObject.txt", "w"), *fp5 = fopen("format4.txt","w");
	char line[256] = {0,}, copy[256] = {0,}, lst[50] = {0,}, object[50] = {0,};
	char *tok1 = strtok(input, "."), *tok2, *tok3, *tok4;
	int i, key = 0, reg1, reg2, base = 0, pc = 0, laddr = 0, xaddr = 0, taddr = 0, opcode = 0, lineNumber = 5;
	unsigned int  Ocode = 0;

	/*open list file*/
	strcpy(lst, tok1); strcat(lst, ".lst"); 
	fp2 = fopen(lst, "w"); 

	if(fp2 == NULL){
		printf("File input erorr!\n");
		assembleFlag = 1;
		return;
	}		
	while(1){
		fgets(line, 100, fp1);
		
		for(i = 0 ; i < strlen(line); i++){
			if(line[i] == '\t')	line[i] = ' ';
			else if(line[i] == '\n')	line[i] = '\0';
		}
		
		strcpy(copy, line);
		tok1 = strtok(line, " "); tok2 = strtok(NULL, " "); tok3 = strtok(NULL, " "); 
		
		if(strcmp(tok1, "END") == 0 ){
			for(i = 0; i < strlen(tok1); i++){
				if(tok1[i] == '\t')	tok1[i] = ' ';
			}
			fprintf(fp2, "%d\t    %s\n", lineNumber, copy);
			break;
		}
		else if(strcmp(tok1, "BASE") == 0){
			for(i = 0; i < strlen(tok1); i++){
				if(tok1[i] == '\t')	tok1[i] = ' ';
			}
			base = findSymAddr(tok2);
			fprintf(fp2, "%d\t    %s\n", lineNumber, copy);
			lineNumber += 5;
			continue;
		}

		if(copy[0] == '.'){
			fprintf(fp2, "%s\n", copy);
			memset(line, 256, '\0'); memset(copy, 256, '\0');
			continue;
		}
	
		/* tok2 = symbol tok3 = opcode tok4 = operand */
		/* if tok2 is symbol */
		else if(findSymbol(tok2) == 0){
			tok4 = strtok(NULL, " ");
			
			if(strcmp(tok3, "START") == 0){
				/* list file*/
				fprintf(fp2, "%d\t%s\n", lineNumber, copy);
				
				/* object file */
				Ocode = strtol(tok1, &tok4, 16);	
				faddr = Ocode;
				fprintf(fp4, "H%-6s%.6X%.6X\n", tok2, Ocode, Length);
				lineNumber += 5;
				continue;
			}
			for(i = 0; i < strlen(tok3); i++)	key += tok3[i];

			opcode = findOpcode(tok3, key % 20);

			if(tok3[0] == '+'){
				/*format 4*/
				for(i = 0; i < strlen(tok3); i++)	tok3[i] = tok3[i + 1];
				for(i = 0, key = 0; i < strlen(tok3); i++)	key += tok3[i];

				opcode = findOpcode(tok3, key % 20);
				if(tok4[strlen(tok4) - 1] == ',') xaddr += 8;
				xaddr += 1;

				if(tok4[0] == '#' || tok4[0] == '@'){
					/* list file */
					if(tok4[0] == '#')	opcode += 1;
					else opcode += 2;
					
					for(i = 0; i < strlen(tok4); i++)	tok4[i] = tok4[i + 1];
					taddr = findSymAddr(tok4);
					if(taddr == -1)	taddr = strtol(tok4, &tok1, 10);

					fprintf(fp2, "%d\t%s   \t%.2X%X%.5X\n", lineNumber, copy, opcode, xaddr, taddr);
					
					/* object file */
					Ocode = strtol(tok1, &tok4, 16);
					if(lineLength == 0){
						fprintf(fp4, "T%.6X ", Ocode);
					}
					if(lineLength + 4 <= 30){
						lineLength += 4;
						fprintf(fp4, "%.2X%X%.5X", opcode, xaddr, taddr);
					}
					else if(lineLength + 4 > 30){
						fprintf(fp4, "\nT%.6X %.2X%X%.5X", Ocode, opcode, xaddr, taddr);
						lineLength = 4;
					}
				}
				else{
					opcode += 3;
					taddr =	findSymAddr(tok4);	
					/* list file */
					fprintf(fp2, "%d\t%s   \t%.2X%X%.5X\n", lineNumber, copy,opcode, xaddr, taddr);

					/* object file */
					Ocode = strtol(tok1, &tok4, 16);
					fprintf(fp5, "%X\n", Ocode + 1);
					if(lineLength == 0){
						fprintf(fp4, "T%.6X ", Ocode);
					}
					if(lineLength + 4 <= 30){
						lineLength += 4;
						fprintf(fp4, "%.2X%X%.5X", opcode, xaddr, taddr);
					}
					else if(lineLength + 4 > 30){
						fprintf(fp4, "\nT%.6X %.2X%X%.5X", Ocode, opcode, xaddr, taddr);
						lineLength = 4;
					}
				}
			}
			else if(retFormat(tok3, key % 20) == 1){
				/*format 1*/
				fprintf(fp2, "%d\t%s   \t\t\t%.2X\n", lineNumber, copy, findOpcode(tok3, key % 20));
				
				/* object file */
				Ocode = strtol(tok1, &tok4, 16);
				if(lineLength == 0){
					fprintf(fp4, "T%.6X ", Ocode);
				}
				if(lineLength + 1 <= 30){
					lineLength += 1;
					fprintf(fp4, "%.2X", findOpcode(tok3, key % 20));
				}
				else if(lineLength + 1 > 30){
					fprintf(fp4, "\nT%.6X %.2X", Ocode, findOpcode(tok3, key % 20));
					lineLength = 1;
				}

			}
			else if(retFormat(tok3, key % 20) == 2){
				/*foramt 2*/
				/* check if there is any ',' */
				if(tok4[strlen(tok4) - 1] == ','){
					tok2 = strtok(NULL, " ");
					tok4[strlen(tok4) - 1] = '\0';
				}
				else tok2 = NULL;
				findRegValue(tok4, tok2, &reg1, &reg2);
				fprintf(fp2, "%d\t%s\t\t%.2X%X%X\n", lineNumber, copy, findOpcode(tok3, key % 20), reg1, reg2);
					
				/* object file */
				Ocode = strtol(tok1, &tok4, 16);
				if(lineLength == 0){
					fprintf(fp4, "T%.6X ", Ocode);
				}
				if(lineLength + 2 <= 30){
					lineLength += 2;
					fprintf(fp4, "%.2X%X%X", findOpcode(tok3, key % 20), reg1, reg2);
				}
				else if(lineLength + 2 > 30){
					fprintf(fp4, "\nT%.6X %.2X%X%X", Ocode, findOpcode(tok3, key % 20), reg1, reg2);
					lineLength = 2;
				}
			}
			else if(retFormat(tok3, key % 20) == 3){
				/*format 3*/

				if(tok4[0] == '#'){
					for(i = 0; i < strlen(tok4); i++)	tok4[i] = tok4[i + 1];
					taddr = findSymAddr(tok4);
					if(opcode % 2 == 0)	opcode++;
					/*not symbol*/
					if(taddr == -1){
						taddr = strtol(tok4, &tok1, 10);
						fprintf(fp2, "%d\t%s   \t%.2X0%.3X\n", lineNumber, copy, opcode, taddr);
					
						/* object file */
						Ocode = strtol(tok1, &tok4, 16);
						if(lineLength == 0){
							fprintf(fp4, "T%.6X ", Ocode);
						}
						if(lineLength + 3 <= 30){
							lineLength += 3;
							fprintf(fp4, "%.2X0%.3X", opcode, taddr);
						}
						else if(lineLength + 3 > 30){
							fprintf(fp4, "\nT%.6X %.2X0%.3X", Ocode, opcode, taddr);
							lineLength = 3;
						}
					}
					/*symbol*/
					else {
						laddr = strtol(tok1, &tok2, 16);
						pc = laddr + 3; 
						if(tok4[strlen(tok4) - 1] == ',')	xaddr += 8;
						
						if(-2048 <= taddr - pc && taddr - pc < 2048){
							xaddr += 2;
							if(taddr - pc >= 0){
								fprintf(fp2, "%d\t%s   \t%.2X%X%.3X\n", lineNumber, copy, opcode, xaddr, taddr - pc);
						
								/* object file */
								Ocode = strtol(tok1, &tok4, 16);
								if(lineLength == 0){
									fprintf(fp4, "T%.6X ", Ocode);
								}
								if(lineLength + 3 <= 30){
									lineLength += 3;
									fprintf(fp4, "%.2X%X%.3X", opcode, xaddr, taddr - pc);
								}
								else if(lineLength + 3 > 30){
									fprintf(fp4, "\nT%.6X %.2X%X%.3X", Ocode, opcode, xaddr, taddr - pc);
									lineLength = 3;
								}
							}
							else{
								fprintf(fp2, "%d\t%s   \t%.2X%X", lineNumber, copy, opcode, xaddr);
								sprintf(object, "%.3X", taddr - pc);
								for(i = strlen(object) - 3; i < strlen(object); i++)	fprintf(fp2, "%c", object[i]);
								fprintf(fp2, "\n");

								/* objec file */
								Ocode = strtol(tok1, &tok4, 16);
								if(lineLength == 0){
									fprintf(fp4, "T%.6X ", Ocode);
								}
								if(lineLength + 3 <= 30){
									lineLength += 3;
									fprintf(fp4, "%.2X%X", opcode, xaddr);
									for(i = strlen(object) - 3; i < strlen(object); i++)	fprintf(fp4, "%c", object[i]);
								}
								else if(lineLength + 3 > 30){
									fprintf(fp4, "\nT%.6X %.2X%X", Ocode, opcode, xaddr);
									for(i = strlen(object) - 3; i < strlen(object); i++)	fprintf(fp4, "%c", object[i]);
									lineLength = 3;
								}
							}
						}
						else if(0 <= taddr - base && taddr - base < 4096){
							xaddr += 4;
							fprintf(fp2, "%d\t%s    \t%X%X%.3X\n", lineNumber, copy, opcode, xaddr, taddr - base);

							/* object file */
							Ocode = strtol(tok1, &tok4, 16);
							if(lineLength == 0){
								fprintf(fp4, "T%.6X ", Ocode);
							}
							if(lineLength + 3 <= 30){
								lineLength += 3;
								fprintf(fp4, "%.2X%X%.3X", opcode, xaddr, taddr - base);
							}
							else if( lineLength + 3 > 30){
								fprintf(fp4, "\nT%.6X %.2X%X%.3X", Ocode, opcode, xaddr, taddr - base);
								lineLength = 3;
							}
						}
						else {
							printf("%s\n This line is out of addressing range.\n", copy);
							assembleFlag = 1;
						}
					}
	
				}
				else if(tok4[0] == '@'){
					for(i = 0; i < strlen(tok4); i++)	tok4[i] = tok4[i + 1];

					laddr = strtol(tok1, &tok3, 16);
					pc = laddr + 3;

					taddr = findSymAddr(tok4);
					if(opcode % 4 == 1 || opcode % 4 == 0) opcode += 2;
					if(tok3[strlen(tok3) - 1] == ',')	xaddr += 8;

					if(-2048 <= taddr - pc && taddr - pc < 2048){
						xaddr += 2;
						if(taddr - pc >= 0){
							fprintf(fp2, "%d\t%s   \t%.2X%X%.3X\n", lineNumber, copy, opcode, xaddr, taddr - pc);

							/* object file */
							Ocode = strtol(tok1, &tok4, 16);
							if(lineLength == 0){
								fprintf(fp4, "T%.6X ", Ocode);
							}
							if(lineLength + 3 <= 30){
								lineLength += 3;
								fprintf(fp4, "%.2X%X%.3X", opcode, xaddr, taddr - pc);
							}
							else if(lineLength + 3 > 30){
								fprintf(fp4, "\nT%.6X %.2X%X%.3X", Ocode, opcode, xaddr, taddr - pc);
								lineLength = 3;
							}
						}
						else{
							/* list file */
							fprintf(fp2, "%d\t%s   \t%.2X%X", lineNumber, copy, opcode, xaddr);
							sprintf(object, "%.3X", taddr - pc);
							for(i = strlen(object) - 3; i < strlen(object); i++)	fprintf(fp2, "%c", object[i]);
							fprintf(fp2, "\n");

							/* object file */
							Ocode = strtol(tok1, &tok4, 16);
							if(lineLength == 0){
								fprintf(fp4, "T%.6X ", Ocode);
							}
							if(lineLength + 3 <= 30){
								lineLength += 3;
								fprintf(fp4, "%.2X%X", opcode, xaddr);
								for(i = strlen(object) - 3; i < strlen(object); i++)	fprintf(fp4, "%c", object[i]);
							}
							else if(lineLength + 3 > 30){
								fprintf(fp4, "\nT%.6X %.2X%X", Ocode, opcode, xaddr);
								for(i = strlen(object) - 3; i < strlen(object); i++)	fprintf(fp4, "%c", object[i]);
								lineLength = 3;
							}
						}
					}
					else if(0 <= taddr - base && taddr - base < 4096){
						xaddr += 4;
						fprintf(fp2, "%d\t%s   \t%.2X%X%.3X\n", lineNumber, copy, opcode, xaddr, taddr - base);

						/* object file */
						Ocode = strtol(tok1, &tok4, 16);
						if(lineLength == 0){
							fprintf(fp4, "T%.6X ", Ocode);
						}
						if(lineLength + 3 <= 30){
							lineLength += 3;
							fprintf(fp4, "%.2X%X%.3X", opcode, xaddr, taddr - base);
						}
						else if(lineLength + 3 > 30){
							fprintf(fp4, "\nT%.6X %.2X%X%.3X", Ocode, opcode, xaddr, taddr - base);
							lineLength = 3;
						}
					}
					else {
						printf("%s\n This line is out of addressing range.\n", copy);
						assembleFlag = 1;
					}
				}
				else{
					if(opcode % 2 == 0)	opcode++;
					if(opcode % 4 == 1 || opcode % 4 == 0)	opcode += 2;

					laddr = strtol(tok1, &tok2, 16);
					pc = laddr + 3;

					taddr = findSymAddr(tok4);

					if(tok4[strlen(tok4) - 1] == ',')	xaddr += 8;
				
					if(-2048 <= taddr - pc && taddr - pc < 2048){
						xaddr += 2;
						if(taddr - pc >= 0){
							fprintf(fp2, "%d\t%s   \t%.2X%X%.3X\n", lineNumber, copy, opcode, xaddr, taddr - pc);

							/* object file */
							Ocode = strtol(tok1, &tok4, 16);
							if(lineLength == 0){
								fprintf(fp4, "T%.6X ", Ocode);
							}
							if(lineLength + 3 <= 30){
								lineLength += 3;
								fprintf(fp4, "%.2X%X%.3X", opcode, xaddr, taddr - pc);
							}
							else if(lineLength + 3 > 30){
								fprintf(fp4, "\nT%.6X %.2X%X%.3X", Ocode, opcode, xaddr, taddr - pc);
								lineLength = 3;
							}
						}
						else{
							fprintf(fp2, "%d\t%s   \t%.2X%X", lineNumber, copy, opcode, xaddr);
							sprintf(object, "%.3X", taddr - pc);
							for(i = strlen(object) - 3; i < strlen(object); i++)	fprintf(fp2, "%c", object[i]);
							fprintf(fp2, "\n");

							/* object file */
							Ocode = strtol(tok1, &tok4, 16);
							if(lineLength == 0){
								fprintf(fp4, "T%.6X ", Ocode);
							}
							if(lineLength + 3 <= 30){
								lineLength += 3;
								fprintf(fp4, "%.2X%X", opcode, xaddr);
								for(i = strlen(object) - 3; i < strlen(object); i++)	fprintf(fp4, "%c", object[i]);
							}
							else if(lineLength + 3 > 30){
								fprintf(fp4, "\nT%.6X %.2X%X", Ocode, opcode, xaddr);
								for(i = strlen(object) - 3; i < strlen(object); i++)	fprintf(fp4, "%c", object[i]);
								lineLength = 3;
							}
						}
					}
					else if(0 <= taddr - base && taddr - base < 4096){
						xaddr += 4;
						fprintf(fp2, "%d\t%s   \t%.2X%X%.3X\n", lineNumber, copy, opcode, xaddr, taddr - base);

						/* object file */
						Ocode = strtol(tok1, &tok4, 16);
						if(lineLength == 0){
							fprintf(fp4, "T%.6X ", Ocode);
						}
						if(lineLength + 3 <= 30){
							lineLength += 3;
							fprintf(fp4, "%.2X%X%.3X", opcode, xaddr, taddr - base);
						}
						else if(lineLength + 3 > 30){
							fprintf(fp4, "\nT%.6X %.2X%X%.3X", Ocode, opcode, xaddr, taddr - base);
							lineLength = 3;
						}
					}
					else{
						printf("%s\n This line is out of addressing range.\n", copy);
						assembleFlag = 1;
					}
				}
			}
			else if(strcmp(tok3, "BYTE") == 0){
				if(tok4[0] == 'C'){
					for(i = 0; i < strlen(tok4) - 3; i++)	object[i] = tok4[i + 2];
					fprintf(fp2, "%d\t%s     \t", lineNumber, copy);
					for(i = 0; i < strlen(object); i++)	fprintf(fp2, "%X", object[i]);
					fprintf(fp2, "\n");

					/* object file */
					Ocode = strtol(tok1, &tok3, 16);
					if(lineLength == 0){
						fprintf(fp4, "T%.6X ", Ocode);
					}
					if(lineLength + (strlen(tok4) - 3) <= 30){
						lineLength += (strlen(tok4) - 3);
						for(i = 0; i < strlen(object); i++)	fprintf(fp4, "%X", object[i]);
					}
					else if(lineLength + (strlen(tok4) - 3) > 30){
						fprintf(fp4, "\nT%.6X ", Ocode);
						for(i = 0; i < strlen(object); i++)	fprintf(fp4, "%X", object[i]);
						lineLength = (strlen(tok4) - 3);
					}
				}
				else if(tok4[0] == 'X'){
					for(i = 0; i < strlen(tok4) - 3; i++)	object[i] = tok4[i + 2];
					fprintf(fp2, "%d\t%s     \t", lineNumber, copy);
					for(i = 0; i < strlen(object); i++)	fprintf(fp2, "%c", object[i]);
					fprintf(fp2, "\n");
	
					/* object file */
					Ocode = strtol(tok1, &tok3, 16);
					if(lineLength == 0){
						fprintf(fp4, "T%.6X ", Ocode);
					}
					if(lineLength + (strlen(tok4) - 3) <= 30){
						lineLength += (strlen(tok4) - 3);
						for(i = 0; i < strlen(object); i++)	fprintf(fp4, "%c", object[i]);
					}
					else if(lineLength + (strlen(tok4) - 3) > 30){
						fprintf(fp4, "\nT%.6X ", Ocode);
						for(i = 0; i < strlen(object); i++)	fprintf(fp4, "%c", object[i]);
						lineLength = (strlen(tok4) - 3);
					}
				}
			}

			else if(strcmp(tok3, "RESW") == 0 || strcmp(tok3, "RESB") == 0){
				fprintf(fp2, "%d\t%s\n", lineNumber, copy);
				if(lineLength != 0){
					fprintf(fp4, "\n");
					lineLength = 0;
				}
			}
		}
		/* tok2 = opcode tok3 = operand*/
		/* if tok2 is not a symbol */
		else {
			for(i = 0; i < strlen(tok2); i++)	key += tok2[i];

			if(strcmp(tok2, "RSUB") == 0){
				fprintf(fp2, "%d\t%s    \t\t%X\n", lineNumber, copy, 0x4F0000);	
				
				/* object file */
				Ocode = strtol(tok1, &tok4, 16);
				if(lineLength == 0){
					fprintf(fp4, "T%.6X ", Ocode);
				}
				if(lineLength + 3 <= 30){
					lineLength += 3;
					fprintf(fp4, "%X", 0x4F0000);
				}
				else if(lineLength + 3 > 30){
					fprintf(fp4, "\nT%.6X %X", Ocode, 0x4F0000);
					lineLength = 3;
				}
				lineNumber += 5;
				continue;
			}

			opcode = findOpcode(tok2, key % 20);

			if(tok2[0] == '+'){
				/*format 4*/
				for(i = 0; i < strlen(tok2); i++)	tok2[i] = tok2[i + 1];
				for(i = 0, key = 0; i < strlen(tok2); i++)	key += tok2[i];	
				
				opcode = findOpcode(tok2, key % 20);
				if(tok3[strlen(tok3) - 1] == ',')	xaddr += 8;
				xaddr += 1;

				if(tok3[0] == '#' || tok3[0] == '@'){
					if(tok3[0] == '#')	opcode += 1;
					else opcode += 2;
					for(i = 0; i < strlen(tok3); i++)	tok3[i] = tok3[i + 1];


					taddr = findSymAddr(tok3);
					if(taddr == -1)	taddr = strtol(tok3, &tok4, 10);

					fprintf(fp2, "%d\t%s   \t%.2X%X%.5X\n", lineNumber, copy, opcode, xaddr, taddr);

					/* object file */
					Ocode = strtol(tok1, &tok4, 16);
					if(lineLength == 0){
						fprintf(fp4, "T%.6X ", Ocode);
					}
					if(lineLength + 4 <= 30){
						lineLength += 4;
						fprintf(fp4, "%.2X%X%.5X", opcode, xaddr, taddr);
					}
					else if(lineLength + 4 > 30){
						fprintf(fp4, "\nT%.6X %.2X%X%.5X", Ocode, opcode, xaddr, taddr);
						lineLength = 4;
					}
				}
				else{
					opcode += 3;
					taddr =	findSymAddr(tok3);	
					fprintf(fp2, "%d\t%s   \t%.2X%X%.5X\n", lineNumber, copy,opcode, xaddr, taddr);
					
					/* object file */
					Ocode = strtol(tok1, &tok4, 16);
					fprintf(fp5, "%X\n", Ocode + 1);
					if(lineLength == 0){
						fprintf(fp4, "T%.6X ", Ocode);
					}
					if(lineLength + 4 <= 30){
						lineLength += 4;
						fprintf(fp4, "%.2X%X%.5X", opcode, xaddr, taddr);
					}
					else if(lineLength + 4 > 30){
						fprintf(fp4, "\nT%.6X %.2X%X%.5X", Ocode, opcode, xaddr, taddr);
						lineLength = 4;
					}
				}
			}
			else if(retFormat(tok2, key % 20) == 1){
				/*format 1*/
				fprintf(fp2, "%d\t%s  \t\t\t%.2X\n", lineNumber, copy, findOpcode(tok2, key % 20));
			
				/* object file */
				Ocode = strtol(tok1, &tok4, 16);
				if(lineLength == 0){
					fprintf(fp4, "T%.6X ", Ocode);
				}
				if(lineLength + 1 <= 30){
					lineLength += 1;
					fprintf(fp4, "%.2X", findOpcode(tok2, key % 20));
				}
				else if(lineLength + 1 > 30){
					fprintf(fp4, "\nT%.6X %.2X", Ocode, findOpcode(tok2, key % 20));
					lineLength = 1;
				}	
			}
			else if(retFormat(tok2, key % 20) == 2){
				/*format 2*/
				if(tok3[strlen(tok3) - 1] == ','){
					tok4 = strtok(NULL, " ");
					tok3[strlen(tok3) - 1] = '\0';
				}
				findRegValue(tok3, tok4, &reg1, &reg2);
				fprintf(fp2, "%d\t%s\t\t%.2X%X%X\n", lineNumber, copy, findOpcode(tok2, key % 20), reg1, reg2);
			
				/* object file*/
				Ocode = strtol(tok1, &tok4, 16);
				if(lineLength == 0){
					fprintf(fp4, "T%.6X ", Ocode);
				}
				if(lineLength + 2 <= 30){
					lineLength += 2;
					fprintf(fp4, "%.2X%X%X", findOpcode(tok2, key % 20), reg1, reg2);
				}
				else if(lineLength + 2 > 30){
					fprintf(fp4, "\nT%.6X %.2X%X%X", Ocode, findOpcode(tok2, key % 20), reg1, reg2);
					lineLength = 2;
				}
			}
			else if(retFormat(tok2, key % 20) == 3){
				/*format 3*/
				if(tok3[0] == '#'){
					/*eliminate the '#'*/
					for(i = 0; i < strlen(tok3); i++)	tok3[i] = tok3[i + 1];
					taddr = findSymAddr(tok3);
					if(opcode % 2 == 0)	opcode++;
					/*not symbol*/
					if(taddr == -1){
						taddr = strtol(tok3, &tok4, 10);
						fprintf(fp2, "%d\t%s   \t%.2X0%.3X\n", lineNumber, copy, opcode, taddr);

						/* object file */
						Ocode = strtol(tok1, &tok4, 16);
						if(lineLength == 0){
							fprintf(fp4, "T%.6X ", Ocode);
						}
						if(lineLength + 3 <= 30){
							lineLength += 3;
							fprintf(fp4, "%.2X0%.3X", opcode, taddr);
						}
						else if(lineLength + 3 > 30){
							fprintf(fp4, "\nT%.6X %.2X0%.3X", Ocode, opcode, taddr);
							lineLength = 3;
						}
					}
					/*symbol*/
					else {
						laddr = strtol(tok1, &tok4, 16);
						pc = laddr + 3; 
						if(tok3[strlen(tok3) - 1] == ',')	xaddr += 8;
						
						if(-2048 <= taddr - pc && taddr - pc < 2048){
							xaddr += 2;
							if(taddr - pc >= 0){
								fprintf(fp2, "%d\t%s   \t%X%X%.3X\n", lineNumber, copy, opcode, xaddr, taddr - pc);

								/* object file */
								Ocode = strtol(tok1, &tok4, 16);
								if(lineLength == 0){
									fprintf(fp4, "T%.6X ", Ocode);
								}
								if(lineLength + 3 <= 30){
									lineLength += 3;
									fprintf(fp4, "%.2X%X%.3X", opcode, xaddr, taddr - pc);
								}
								else if(lineLength + 3 > 30){
									fprintf(fp4, "\nT%.6X %.2X%X%.3X", Ocode, opcode, xaddr, taddr - pc);
									lineLength = 3;
								}
							}
							else{
								fprintf(fp2, "%d\t%s   \t%.2X%X", lineNumber, copy, opcode, xaddr);
								sprintf(object, "%.3X", taddr - pc);
								for(i = strlen(object) - 3; i < strlen(object); i++)	fprintf(fp2, "%c", object[i]);
								fprintf(fp2, "\n");

								/* object file */
								Ocode = strtol(tok1, &tok4, 16);
								if(lineLength == 0){
									fprintf(fp4, "T%.6X ", Ocode);
								}
								if(lineLength + 3 <= 30){
									lineLength += 3;
									fprintf(fp4, "%.2X%X", opcode, xaddr);
									for(i = (strlen(object) - 3); i < strlen(object); i++)	fprintf(fp4, "%c", object[i]);
								}
								else if(lineLength + 3 > 30){
									fprintf(fp4, "\nT%.6X %.2X%X", Ocode, opcode, xaddr);
									for(i = (strlen(object) - 3); i < strlen(object); i++)	fprintf(fp4, "%c", object[i]);
									lineLength = 3;
								}
							}
						}
						else if(0 <= taddr - base && taddr - base < 4096){
							xaddr += 4;
							fprintf(fp2, "%d\t%s   \t%X%X%.3X\n", lineNumber, copy, opcode, xaddr, taddr - base);
							
							/* object file */
							Ocode = strtol(tok1, &tok4, 16);
							if(lineLength == 0){
								fprintf(fp4, "T%.6X", Ocode);
							}
							if(lineLength + 3 <= 30){
								lineLength += 3;
								fprintf(fp4, "%.2X%X%.3X", opcode, xaddr, taddr - base);
							}
							else if(lineLength + 3 > 30){
								fprintf(fp4, "\nT%.6X %.2X%X%.3X", Ocode, opcode, xaddr, taddr - base);
								lineLength = 3;
							}
						}
						else{
							printf("%s\n This line is out of addressing range.\n", copy);
							assembleFlag = 1;
						}
					}
				}	
				else if(tok3[0] == '@'){
				//	printf("/*indirect*/ %s\n", tok2);
					for(i = 0; i < strlen(tok3); i++)	tok3[i] = tok3[i + 1];

					laddr = strtol(tok1, &tok4, 16);
					pc = laddr + 3;

					taddr = findSymAddr(tok3);
					
					if(opcode % 4 == 1 || opcode % 4 == 0) opcode += 2;
				
					if(tok3[strlen(tok3) - 1] == ',')	xaddr += 8;

					if(-2048 <= taddr - pc && taddr - pc < 2048){
						xaddr += 2;
						if(taddr - pc >= 0){
							fprintf(fp2, "%d\t%s   \t%.2X%X%.3X\n", lineNumber, copy, opcode, xaddr, taddr - pc);

							/* object file */
							Ocode = strtol(tok1, &tok4, 16);
							if(lineLength == 0){
								fprintf(fp4, "T%.6X", Ocode);
							}
							if(lineLength + 3 <= 30){
								lineLength += 3;
								fprintf(fp4, "%.2X%X%.3X", opcode, xaddr, taddr - pc);
							}
							else if(lineLength + 3 > 30){
								fprintf(fp4, "\nT%.6X %.2X%X%.3X", Ocode, opcode, xaddr, taddr - pc);
								lineLength = 3;
							}
						}
						else{
							fprintf(fp2, "%d\t%s   \t%.2X%X", lineNumber, copy, opcode, xaddr);
							sprintf(object, "%.3X", taddr - pc);
							for(i = strlen(object) - 3; i < strlen(object); i++)	fprintf(fp2, "%c", object[i]);
							fprintf(fp2, "\n");

							/* object file */
							Ocode = strtol(tok1, &tok4, 16);
							if(lineLength == 0){
								fprintf(fp4, "T%.6X ", Ocode);
							}
							if(lineLength + 3 <= 30){
								lineLength += 3;
								fprintf(fp4, "%.2X%X", opcode, xaddr);
								for(i = strlen(object); i < strlen(object); i++)	fprintf(fp4, "%c", object[i]);
							}
							else if(lineLength + 3 > 30){
								fprintf(fp4, "\nT%.6X %.2X%X", Ocode, opcode, xaddr);
								for(i = strlen(object); i < strlen(object); i++)	fprintf(fp4, "%c", object[i]);
								lineLength = 3;
							}
						}
					}
					else if(0 <= taddr - base && taddr - base < 4096){
						xaddr += 4;
						fprintf(fp2, "%d\t%s   \t%.2X%X%.3X\n", lineNumber, copy, opcode, xaddr, taddr - base);
						
						/* object file */
						Ocode = strtol(tok1, &tok4, 16);
						if(lineLength == 0){
							fprintf(fp4, "T%.6X ", Ocode);
						}
						if(lineLength + 3 <= 30){
							lineLength += 3;
							fprintf(fp4, "%.2X%X%.3X", opcode, xaddr, taddr - base);	
						}
						else if(lineLength + 3 > 30){
							fprintf(fp4, "\nT%.6X %.2X%X%.3X", Ocode, opcode, xaddr, taddr - base);
							lineLength = 3;
						}
					}
					else{
						printf("%s\n This line is out of addressing range.\n", copy);
						assembleFlag = 1;
					}
				}
				else{
				//	printf("/*simple*/ %s\n", tok2);
					if(opcode % 2 == 0)	opcode++;
					if(opcode % 4 == 1 || opcode % 4 == 0)	opcode += 2;

					laddr = strtol(tok1, &tok4, 16);
					pc = laddr + 3;

					if(tok3[strlen(tok3) - 1] == ','){
						xaddr += 8;
						tok3[strlen(tok3) - 1] = '\0';
					}
					taddr = findSymAddr(tok3);
					
					if(-2048 <= taddr - pc && taddr - pc < 2048){
						xaddr += 2;
						if(taddr - pc >= 0){
							fprintf(fp2, "%d\t%s     \t%.2X%X%.3X\n", lineNumber, copy, opcode, xaddr, taddr - pc);

							/* object file */
							Ocode = strtol(tok1, &tok4, 16);
							if(lineLength == 0){
								fprintf(fp4, "T%.6X ", Ocode);
							}
							if(lineLength + 3 <= 30){
								lineLength += 3;
								fprintf(fp4, "%.2X%X%.3X", opcode, xaddr, taddr - pc);
							}
							else if(lineLength + 3 > 30){
								fprintf(fp4, "\nT%.6X %.2X%X%.3X", Ocode, opcode, xaddr, taddr - pc);
								lineLength = 3;
							}
						}	
						else{
							fprintf(fp2, "%d\t%s   \t%.2X%X", lineNumber, copy, opcode, xaddr);
							sprintf(object, "%.3X", taddr - pc);
							for(i = strlen(object) - 3; i < strlen(object); i++)	fprintf(fp2, "%c", object[i]);
							fprintf(fp2, "\n");

							/* object file */
							Ocode = strtol(tok1, &tok4, 16);
							if(lineLength == 0){
								fprintf(fp4, "T%.6X ", Ocode);
							}
							if(lineLength + 3 <= 30){
								lineLength += 3;
								fprintf(fp4, "%.2X%X", opcode, xaddr);
								for(i = strlen(object) - 3; i < strlen(object); i++)	fprintf(fp4, "%c", object[i]);
							}
							else if(lineLength + 3 > 30){
								fprintf(fp4, "\nT%.6X %.2X%X", Ocode, opcode, xaddr);
								for(i = strlen(object) - 3; i < strlen(object); i++)	fprintf(fp4, "%c", object[i]);
								lineLength = 3;
							}
						}
					}
					else if(0 <= taddr - base && taddr - base < 4096){
						xaddr += 4;
						fprintf(fp2, "%d\t%s   \t%.2X%X%.3X\n", lineNumber, copy, opcode, xaddr, taddr - base);

						/* object file */
						Ocode = strtol(tok1, &tok4, 16);
						if(lineLength == 0){
							fprintf(fp4, "T%.6X ", Ocode);
						}
						if(lineLength + 3 <= 30){
							lineLength += 3;	
							fprintf(fp4, "%.2X%X%.3X", opcode, xaddr, taddr - base);
						}
						else if(lineLength + 3 > 30){
							fprintf(fp4, "\nT%.6X %.2X%X%.3X", Ocode, opcode, xaddr, taddr - base);		
							lineLength = 3;	
						}
					}
					else{
						printf("%s\n This line is out of addressing range.\n", copy);
						assembleFlag = 1;
					}
				}
			}
		}
		memset(line, 0, 256); memset(copy, 0, 256); memset(object, 0, 50);
		tok1 = NULL; tok2 = NULL; tok3 = NULL; tok4 = NULL; key = 0; 
		taddr = 0; laddr = 0; xaddr = 0; Ocode = 0; opcode = 0; lineNumber += 5;
	}
	fprintf(fp4,"\n");
	fclose(fp1); fclose(fp2); fclose(fp4); fclose(fp5);
}


/*함수 : findRegValue*/
/*목적 : format2의 입력으로 들어온 레지스터의 값을 찾는다. */
/*리턴값 : 없음. */
void findRegValue(char* a, char* b, int* reg1, int* reg2){
	if(a != NULL){
		if(strcmp(a, "A") == 0)	*reg1 = 0;
		else if(strcmp(a, "X") == 0)	*reg1 = 1;
		else if(strcmp(a, "L") == 0)	*reg1 = 2;
		else if(strcmp(a, "PC") == 0) *reg1 = 8;
		else if(strcmp(a, "SW") == 0) *reg1 = 9;
		else if(strcmp(a, "B") == 0)	*reg1 = 3;
		else if(strcmp(a, "S") == 0)	*reg1 = 4;
		else if(strcmp(a, "T") == 0)	*reg1 = 5;
		else if(strcmp(a, "F") == 0)	*reg1 = 6;
		else{
			assembleFlag = 1;
			printf("%s is not supported register.\n", a);
		}
	}
	else *reg1 = 0;

	if(b != NULL){
		if(strcmp(b, "A") == 0)	*reg2 = 0;
		else if(strcmp(b, "X") == 0)	*reg2 = 1;
		else if(strcmp(b, "L") == 0)	*reg2 = 2;
		else if(strcmp(b, "PC") == 0) *reg2 = 8;
		else if(strcmp(b, "SW") == 0) *reg2 = 9;
		else if(strcmp(b, "B") == 0)	*reg2 = 3;
		else if(strcmp(b, "S") == 0)	*reg2 = 4;
		else if(strcmp(b, "T") == 0)	*reg2 = 5;
		else if(strcmp(b, "F") == 0)	*reg2 = 6;
		else{
			assembleFlag = 1;
			printf("%s is not supported register.\n", b);
		}
	}
	else *reg2 = 0;
}

/*함수 : ObjecFile*/
/*목적 : pass2()에서 만들어둔 파일들을 토대로 .obj파일을 만든다. */
/*리턴값 : 없음. */
void ObjectFile(char* input){
	FILE* fp1 = fopen("tempObject.txt", "r"), *fp2 = fopen("format4.txt", "r"), *fp3;
	char name[100] = {0,}, line[256] = {0,};
	char* temp;
	unsigned int addr;

	temp = strtok(input, "."); strcpy(name, temp);
	strcat(name, ".obj");
	fp3 = fopen(name, "w");

	while(1){
		if(fgets(line, 256, fp1) == NULL) break;

		if(line[0] == 'H'){
			fprintf(fp3, "%s", line);
		}
		else if(line[0] == 'T'){
			temp = strtok(line, " "); fprintf(fp3, "%s", temp); 
			temp = strtok(NULL, " "); 
			addr = strlen(temp); fprintf(fp3, "%.2X%s", addr / 2, temp);
		}
	}	
	memset(line, 0, 256); addr = 0;

	while(1){
		if(fgets(line, 256, fp2) == NULL) break;
		
		addr = strtol(line, &temp, 16);
		fprintf(fp3, "M%.6X05\n", addr);
	}	
	fprintf(fp3, "E%.6X\n", faddr);

	fclose(fp1); fclose(fp2); fclose(fp3);
}

/*함수 : freeSymTable */
/*목적 : symbol table의 노드들을 해제시킨다.. */
/*리턴값 : 없음.  */
void freeSymTable(){
	Symbol* temp;
	int i;

	for(i = 0; i < 26; i++){
		if(sym_head[i] == NULL)	continue;
		else{
			while(1){	
				temp = sym_head[i];
				sym_head[i] = sym_head[i]->link;
				if(sym_head[i] == NULL)	break;
				free(temp);
			}
		}
	}

}

/*함수 : fileDelete */
/*목적 : intermediate file을 지운다. */
/*리턴값 : 없음. */
void fileDelete(){
	remove("temp.txt");
	remove("tempObject.txt");
	remove("format4.txt");
}
