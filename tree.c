#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h> 
#pragma warning(disable:4996)

FILE* in, *out, *mem, *help;

int err, mall = 0, fre = 0, curV, resEvalue = 1;
char* lTree;

typedef struct StackStructure
{
	char information;
	struct StackStructure* next;

} forparse;

typedef struct TreeStructure
{
	struct TreeStructure *parent, *left, *right;
	char information;
	int value;
	int kalk;

} Tree;

Tree* root;

void push(forparse** st, char value)
{
	forparse* n = NULL, *top = *st, *kek = *st;

	if (top != NULL)
	{
		n = (forparse*)malloc(sizeof(forparse)); mall++;
		n->information = value;
		n->next = top;
		top = n;
	}
	else
	{
		top = (forparse*)malloc(sizeof(forparse)); mall++;
		top->information = value;
		top->next = NULL;
	}
	*st = top;
}

void pop(forparse** st)
{
	forparse *prevTop, *top = *st;
	prevTop = top;
	top = top->next;
	free(prevTop); fre++;
	prevTop = NULL;
	*st = top;
}

int priority(char c)
{
	int pr;
	if (c == '~')
		pr = 5;
	else
		if (c == '^')
			pr = 4;
		else
			if ((c == '*') || (c == '/') || (c == '%'))
				pr = 3;
			else
				if (c == '+' || c == '-')
					pr = 2;
				else
					if (c == ')' || c == '(')
						pr = 0;
	return pr;
}

void info(char* l, char** resT) // dlya parsinga
{
	forparse* parse = NULL;
	int len = strlen(l), i = 0, j = 0;
	char* res = *resT;
	while (i < len)
	{
		if ((l[i] >= 'a' && l[i] <= 'z') || (l[i] >= '1' && l[i] <= '9'))
		{
			if (l[i] >= '0' && l[i] <= '9')
			{
				while (l[i] >= '0' && l[i] <= '9')
				{
					res[j] = l[i];
					//printf("%c ", res[j]);
					j++; i++;
				}
				i--;
				res[j] = '!';
				//printf("%c ", res[j]);
				j++;
			}
			else
			{
				res[j] = l[i];
				//printf("%c ", res[j]);
				j++;
			}
		}
		else
			if (l[i] == '(')
				push(&parse, '(');
			else
				if (l[i] == ')')
				{
					if (parse != NULL)
					{
						while (parse->information != '(')
						{
							res[j] = parse->information;
							j++;
							pop(&parse);
						}
						pop(&parse);
					}
				}
				else
					if (l[i] == '^' || l[i] == '+' || l[i] == '-' || l[i] == '*' || l[i] == '/' || l[i] == '%' || l[i] == '~')
					{
						if (parse != NULL)
						{
							int pr;
							if (l[i] == '~')
								pr = 5;
							else
								if (l[i] == '^')
									pr = 4;
								else
									if ((l[i] == '*') || (l[i] == '/') || (l[i] == '%'))
										pr = 3;
									else
										if (l[i] == '+' || l[i] == '-')
											pr = 2;
										else
											if (l[i] == ')' || l[i] == '(')
												pr = 0;
							while (parse!= NULL && priority(parse->information) >= pr)
							{
								printf("PARSE -> INFO  %c\n", parse->information);
								if (parse->information == '~' && pr == 5) break;
								res[j] = parse->information;
								j++;
								pop(&parse);
							}
						}
						push(&parse, l[i]);
					}
		i++;
	}
	while (parse != NULL)
	{
		res[j] = parse->information;
		//printf("%c", res[j]);////
		j++;
		pop(&parse);
	}
	res[j] = 0;
	*resT = res;
	printf("\nRESULT IN INFO = %s", *resT);
}

int postfix(char* s)// parsing
{
	int len = strlen(s), count = 1, i;
	for (i = 0; i < len; i++)
	{
		if (s[i] == ')' || s[i] == '(')
		{
			count = 0;
		}
	}
	return count;
}
int  buildtree(Tree** curR)// parsing
{
	Tree* cur = *curR;
	char* value, ch;
	int i;
	int count;
	if (lTree[0] == 0)
		return 0;
	if (cur == NULL)
	{
		cur = (Tree*)malloc(sizeof(Tree)); mall++;
		cur->left = cur->right = cur->parent = NULL;
		*curR = cur;
	}
	ch = lTree[0];
	printf("\n lTree[0]  = %c\n", lTree[0]);
	i = 1;
	if (ch == '^' || ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%' || ch == '~')
	{
		if (ch != '~')
		{
			cur->information = ch;
			cur->right = (Tree*)malloc(sizeof(Tree));
			cur->left = (Tree*)malloc(sizeof(Tree)); mall = mall + 2;
			cur->left->left = cur->left->right = cur->right->right = cur->right->left = NULL;
			cur->right->parent = cur->left->parent = cur;
			*curR = cur;
			lTree++;
			buildtree(&(cur->left));
			lTree++;
			buildtree(&(cur->right));
		}
		else
		{
			cur->information = ch;
			cur->right = (Tree*)malloc(sizeof(Tree)); mall++;
			cur->left = cur->right->left = cur->right->right = NULL;
			cur->right->parent = cur;
			lTree++;
			buildtree(&(cur->right));
		}
	}
	else
	{
		//printf("\nlTREE[0] = %c\n", lTree[0]);
		if (lTree[0] == '!')
		{
			lTree++;
			value = (char*)malloc(10 * sizeof(10)); mall++;
			for (i = 0; lTree[i] != '!' && lTree[i] != 0 && (lTree[i] >= '0' && lTree[i] <= '9'); ++i)
				value[i] = lTree[i];
			value[i] = 0;
			printf("\nVALUE = %s\n", value);
			if (i - 1 > 0)
				lTree += i - 1;
			cur->value = atoi(value); cur->information = -1;
			free(value); fre++;
			value = NULL;
		}
		else
			cur->information = ch;
	}
}
int print(Tree* cur)
{
	int count, i;

	if (cur->left == NULL && cur->right == NULL)
	{
		if (cur->information != -1)
			if (cur->information == '~')
			{
				fprintf(out, "-");
				fprintf(help, "-");
			}
			else
			{
				fprintf(out, "%c", cur->information);
				fprintf(help, "%c", cur->information);
			}
		else
		{
			fprintf(out, "%d", cur->value);
			fprintf(help, "%d", cur->value);
		}
		printf(" VALUE = %d\n", cur->value);
		return 0;
	}
	if (cur->left != NULL && cur->right != NULL)
	{
		fprintf(out, "%c(", cur->information);
		fprintf(help, "%c(", cur->information);
		printf(" TREE = %c \n", cur->information);
		print(cur->right);
		fprintf(out, ",");
		fprintf(help, ",");
		print(cur->left);
		fprintf(out, ")");
		fprintf(help, ")");
	}
	else
		if (cur->left == NULL && cur->right != NULL)
		{
			fprintf(out, "-(");
			fprintf(help, "-(");
			print(cur->right);
			fprintf(out, ")");
			fprintf(help, ")");
		}
		else
			if (cur->left != NULL && cur->right == NULL)
			{
				fprintf(out, "-(");
				fprintf(help, "-(");
				print(cur->left);
				fprintf(out, ")");
				fprintf(help, ")");
			}
}

void delet(Tree* cur)
{
	Tree* temp;
	if (cur->left != NULL)
		delet(cur->left);
	if (cur->right != NULL)
		delet(cur->right);
	if (cur->parent != NULL)
	{
		temp = cur->parent;
		temp->left = NULL;
		temp->right = NULL;
		free(cur); fre++;
		cur = NULL;
	}
	else
	{
		free(cur); fre++;
		cur = NULL;
	}
}
void setValues(Tree* cur)
{
	if (cur->left != NULL)
		setValues(cur->left);
	if (cur->right != NULL)
		setValues(cur->right);
	if (cur->information == -1)
		cur->kalk = cur->value;
}
void checkEvalue(Tree* cur)
{
	if (cur->left != NULL)
		checkEvalue(cur->left);
	if (cur->right != NULL)
		checkEvalue(cur->right);
	if (((cur->information >= 'a' && cur->information <= 'z') || (cur->information >= '1' && cur->information <= '9') || (cur->information == -1)) && cur->information != -1)
		resEvalue = 0;
}

void evalueTree(Tree* cur, char* s)
{
	int i = 0, start, isOtr = 1;
	char* temp = (char*)malloc(10 * sizeof(int)); mall++;
	if (cur->left != NULL)
		evalueTree(cur->left, s);
	if (cur->right != NULL)
		evalueTree(cur->right, s);
	if (cur->information >= 'a' && cur->information <= 'z')
	{
		while (s[i] != 0)
		{
			if (cur->information == s[i])
			{
				start = i + 2;
				if (s[start] == '-')
				{
					start++;
					isOtr = -1;
				}
				else
					isOtr = 1;
				for (i = start; s[i] >= '0' && s[i] <= '9'; ++i)
					temp[i - start] = s[i];
				temp[i - start] = 0;
				cur->value = isOtr*atoi(temp);
				printf("Preobrazhenie %d\n", cur->value);
				cur->information = -1;
				isOtr = 1;
				//printf("Preobrazhenie %d\n", cur->value);
			}
			else
				i++;
		}
		//printf("Preobrazhenie %d\n", cur->value);

	}
	free(temp);  fre++;
	temp = NULL;
}

void eval(Tree* cur)
{
	char ch;
	if (cur->left != NULL)
		eval(cur->left);
	if (cur->right != NULL)
		eval(cur->right);
	ch = cur->information;
	if (ch != '~')
	{
		if (ch == '+')
			cur->kalk = cur->right->kalk + cur->left->kalk;
		if (ch == '-')
			cur->kalk = cur->right->kalk - cur->left->kalk;
		if (ch == '*')
			cur->kalk = cur->right->kalk * cur->left->kalk;
		if (ch == '/')
		{
			if (cur->left->kalk == 0)
			{
				err = 1;
				return;
			}
			cur->kalk = cur->right->kalk / cur->left->kalk;
		}
		if (ch == '^')
		{
			if (cur->right->kalk <= 0 || cur->left->kalk <= 0)
			{
				err = 1;
				return;
			}
			int i = 0, sum = 1;
			while (i < cur->left->kalk)
			{
				sum *= cur->right->kalk;
				i++;
			}
			cur->kalk= sum;
		}
		if (ch == '%')
		{
			if (cur->right->kalk <= 0 || cur->left->kalk <= 0)
			{
				err = 1;
				return;
			}
			cur->kalk = cur->right->kalk % cur->left->kalk;
		}
	}
	else
	{
		if (ch == '~')
		{
			if (cur->left != NULL)
				cur->kalk = -cur->left->kalk;
			else
				cur->kalk =-1*cur->right->kalk;
		}
	}
}
int evalTree()
{
	eval(root);
	return root->kalk;
}

int main()
{
	int i, start, prev, j, revLen, curLen = 100, len = 0, minus, isM;
	char* temp, *com, *expr, *res, *value, *revStr, c;
	root = NULL;
	in = fopen("input.txt", "r");
	out = fopen("output.txt", "w");
	mem = fopen("memstat.txt", "w");
	char checkload[50];
	printf("COMMANDS:\n");
	while (!feof(in))
	{
		temp = (char*)malloc(curLen * sizeof(char)); mall++;
		while ((c = getc(in)) != '\n' && c != EOF)
		{
			temp[len] = c;
			len++;
		}
		temp[len] = 0;
		for (int i = 0; i < len; i++)
		{
			printf("%c ", temp[i]);
		}
		printf("READY \n");
		//printf("SCHITANO\n");
		com = (char*)malloc(curLen * sizeof(char));
		expr = (char*)malloc(curLen * sizeof(char));
		res = (char*)malloc(curLen * sizeof(char));
		value = (char*)malloc(curLen * sizeof(char)); mall = mall + 4;
		len = 0;
		for (i = 0; (temp[i] != ' ') && (temp[i] != 0) && (temp[i] != '\n'); i++)
		{
			com[i] = temp[i];
			printf("%c ", com[i]);
		}
		printf("\n");

		com[i] = 0;

		if (!strcmp(com, "parse"))
		{
			if (root != NULL)
			{
				delet(root);
				root = NULL;
			}
			i++;// schetchik bukv v komande
			start = i;
			prev = -1;
			printf("\ni = %d\n", i);
			printf("TEMP %s\n", temp);
			while (temp[i] != 0)
			{
				if ((prev == -1 || prev == 1) && (temp[i] == '-'))
				{
					expr[i - start] = '~';
					printf("%c ", expr[i - start]);
					i++;
					prev = 1; // esli unarniy minus
				}
				else
					if ((prev == 0) && (temp[i] == '^' || temp[i] == '+' || temp[i] == '-' || temp[i] == '*' || temp[i] == '/' || temp[i] == '%' || temp[i] == '~'))
					{
						prev = 1;
						expr[i - start] = temp[i];
						printf("%c ", expr[i - start]);
						i++;
					}
					else
					{
						if (temp[i] == '(' || (temp[i] == '^' || temp[i] == '+' || temp[i] == '-' || temp[i] == '*' || temp[i] == '/' || temp[i] == '%' || temp[i] == '~'))
						{
							prev = 1;
						}
						else //eslic chislo
							prev = 0;
						expr[i - start] = temp[i];
						printf("%c ", expr[i - start]);
						i++;
					}
				//printf("%c ", expr[i]);
				//printf("\n");
			}
			printf("\nPARSING READY\n");
			expr[i - start] = 0;
			res[0] = 0;
			int len = strlen(expr), count = 0;
			for (int i = 0; i < len; i++)
			{
				if (expr[i] == '(')
					count++;
				else
					if (expr[i] == ')')
					{
						if (i > 0 && (expr[i - 1] == '^' || expr[i - 1] == '+' || expr[i - 1] == '-' || expr[i - 1] == '*' || expr[i - 1] == '/' || expr[i - 1] == '%' || expr[i - 1] == '~'))
						{
							count = -1;
							break;
						}
						count--;
					}
			}
			if (count == 0)
			{
				if (expr[i - start - 1] != '~')
				{
					info(expr, &res);
					printf("\nTOTAL STRING = %s && %s\n", res, expr);
					if (postfix(res))
					{
						printf("\nRESULT V PARSINGE = %s\n", res);
						fprintf(out, "valid\n");
						revLen = strlen(res) + 1;
						revStr = (char*)malloc(revLen * sizeof(char)); mall++;
						int j = strlen(res) - 1;
						for (int i = 0; i < strlen(res); i++)
						{
							revStr[i] = res[j];
							j--;
						}
						revStr[strlen(res)] = 0;
						printf("\nrevStr = %s\n", revStr);
						lTree = revStr;
						printf("\n LTREE = %s", lTree);
						buildtree(&root);
						free(revStr); fre++;
						revStr = NULL;
						while (root->parent != NULL)
						{
							root = root->parent;
						}
					}
					else
					{
						if (root != NULL)
							delet(root);
						fprintf(out, "invalid\n");
					}
				}
				else
				{
					if (root != NULL)
						delet(root);
					fprintf(out, "invalid\n");
				}
			}
			else
			{
				if (root != NULL)
					delet(root);
				fprintf(out, "invalid\n");
			}
		}
		else
			if (!strcmp(com, "save_prf"))
			{
				if (root != NULL)
				{
					help = fopen("help.txt", "w");
					print(root);
					fclose(help);
					help = fopen("help.txt", "r");
					int counter = 0;
					char symbol;
					symbol = fgetc(help);
					while (symbol != EOF)
					{
						checkload[counter] = symbol;
						symbol = fgetc(help);
						counter++;
					}
					checkload[counter] = 0;
					fclose(help);
					printf("CHECK LOAD = %s\n", checkload);
					remove("help.txt");
					fprintf(out, "\n");
				}
				else
					fprintf(out, "not_loaded\n");
			}
			else
				if (!strcmp(com, "load_prf"))
				{
					if (root == NULL)
					{
						fprintf(out, "not_loaded");
					}
					else
					{
						int symbolcounter = 0;
						int mathemcount = 0;
						int leftpos = 0;
						int rightpos = 0;
						for (int i = 0; i < strlen(checkload); i++)
						{
							if ((checkload[i] >= '1' && checkload[i] <= '9') || (checkload[i] >= 'a' && checkload[i] <= 'z'))
							{
								symbolcounter++;
							}
							else if (checkload[i] == '(')
							{
								leftpos++;
							}
							else if (checkload[i] == ')')
							{
								rightpos++;
							}
							else
							{
								if (checkload[i] != ',')
								{
									mathemcount++;
								}
							}
						}
						printf("rightpos = %d\n", rightpos);
						printf("leftpos = %d\n", leftpos);
						printf("symbolcounter= %d\n", symbolcounter);
						printf("mathemcounter = %d\n", mathemcount);
						if (symbolcounter == mathemcount + 1)
						{
							if ((leftpos == rightpos) && (symbolcounter > mathemcount))
							{
								fprintf(out, "valid\n");
								for (int i = 0; i < strlen(checkload); i++)
								{
									checkload[i] = 0;
								}
							}
						}
						else
						{
							fprintf(out, "invalid\n");
							for (int i = 0; i < strlen(checkload); i++)
							{
								checkload[i] = 0;
							}
						}
					}
				}
				else
					if (!strcmp(com, "eval"))
					{
						if (root != NULL)
						{
							start = ++i;
							j = 0;
							while (i < curLen && temp[i])
							{
								value[j++] = temp[i];
								i++;
							}
							value[j] = 0;
							printf("FIRSET VALUE %s\n", value);
							//rememberValues(root);//
							evalueTree(root, value);//vmesto peremennih
							resEvalue = 1;
							checkEvalue(root);//na no var values
							if (resEvalue)
							{
								err = 0;
								setValues(root);// kalkulyator
								resEvalue = evalTree();
								if (err)
								{
									fprintf(out, "error\n");
								}
								else
								{
									fprintf(out, "%d\n", resEvalue);
								}
							}
							else
							{
								fprintf(out, "no_var_values\n");
							}
						}
						else
						{
							fprintf(out, "not_loaded\n");
						}
					
					}
		free(temp);
		free(com);
		free(expr);
		free(res);
		free(value); fre = fre + 5;
		temp = com = expr = res = value = NULL;
	}
	if (root != NULL)
		delet(root);
	fprintf(mem, "malloc : %d\nfree : %d\n", mall, fre);
	fclose(in);
	fclose(out);
	fclose(mem);
	system("pause");
	return 0;
}
