/*
**¿ÎÉè£ºËãÊõ±íŽïÊœµÄÓï·š·ÖÎöŒ°ÓïÒå·ÖÎö³ÌÐòÉèŒÆ----Äæ²šÀŒÊœÊä³ö+µÝ¹éÏòÏÂ
-----------------------------------------------------------------------
**µ¥Î»£ºÎäººÀí¹€ŽóÑ§
**×÷Õß£ºÓÍÖœÉ¡
**Ê±Œä£º2015/1/9-1/11
**ÔËÐÐ»·Ÿ³£ºVS2012
*/

/*
**ÎÄ·šÃèÊö£º
**E -> E+T | E-T | T
**T -> T*F | T/F | F
**F -> (E) | ±êÊ¶·û | ÎÞ·ûºÅÕûÊý
-----------------------------------------------------------------------
**Ïû³ý×óµÝ¹éÈçÏÂ£º
**int E();     // E -> TG ×Óº¯Êý 
**int G();     // G -¡µ+TG | -TG |ŠÅ×Óº¯Êý
**int T();     // T -> FS ×Óº¯Êý
**int S();     // S -> *FS | /FS |ŠÅ ×Óº¯Êý
**int F();     // F -> (E) | ±êÊ¶·û | ÎÞ·ûºÅÕûÊý ×Óº¯Êý
*/
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

#define DEL 0;	//¶šœç·ûµÄÖÖÀà±àÂë£º0-----(,),;
#define OP	1;	//Ëã·ûµÄÖÖ±ð±àÂë£º1-----+,-,*,/
#define CONSTANT 2;	//³£ÊýµÄÖÖ±ð±àÂë£º3
#define	VAR 3;	//±äÁ¿µÄÖÖ±ðÂë

const int NUM = 10;//ŽÊµÄžöÊý?????????
const int OP_NUM = 8;

//Ëã·ûÊý×é
const char OP_token[OP_NUM] = {'+', '-', '*', '/', 
								'(', ')', ';', '#'};

int n = 0;	//ÓÃÓÚ±êŒÇµÝ¹éŽÎÐò¡¢·ÖÎö²ãŽÎ
char OP_sem[2] = {' ', '\0'};	//ÓÃÓÚ±ÈœÏÇ°ºó³öÏÖµÄËÄÔòÔËËã·ûºÅ
int OP_order;
int memory_sem = 0, num_sem = 0;	//ŒÇÂŒÊ¶±ðµÄËÄÔòÔËËã·ûºÅµÄžöÊý

char semTemp[NUM] = "";
ifstream in("1.txt");

//ŽÊ·šœá¹¹Ìå
struct lexical
{
	//enum type t1;
	int type;
	char word[NUM];
	lexical *next;
};

lexical *lhead, *lend;
lexical *p_syntax;	//ÓÃÓÚÓï·šºÍÓïÒå·ÖÎö

void initLexical()
{
	lhead = NULL;
	lend = lhead;
}

//Î²²åÈëÔªËØ
lexical *addElement_l(char *str, int type)
{
	lexical *p;
	p = new lexical();
	strcpy(p->word, str);
	p->type = type;
	if(lhead == NULL)
	{
		lhead = p;
	}
	else 
	{
		lend->next = p;
	}
	lend = p;
	lend->next = NULL;
	return lhead;
}

struct semantic
{
	char word[NUM];
	semantic *next;
};

semantic *p_semantic, *shead, *send;	//ÓÃÓÚÓïÒå·ÖÎö£¬ŽæÈ¡Äæ²šÀŒÊœ

void initSemantic()
{
	shead = NULL;
	send = shead;
}

//Î²²åÈëÔªËØ
semantic *addElement_s(char *str)
{
	semantic *p;
	p = new semantic();
	strcpy(p->word, str);
	if(shead == NULL)
	{
		shead = p;
	}
	else 
	{
		send->next = p;
	}
	send = p;
	send->next = NULL;
	return shead;
}

//ŽŠÀí¿Õžñ
void skipSpace(char &ch)
{
	if(ch == ' ')
	{
		while(ch == ' ')
		{
			in.get(ch);
	}
}
		}

//ÅÐ¶ÏËã·û
int OP_fair(char ch)
{
	for(int i = 0; i < OP_NUM; ++i)
	{
		if(ch == OP_token[i])
			return 1;
	}
		return 0;
}

//ÅÐ¶Ï×Ö·û
int CONST_fair(char ch)
{
	if(ch >= 'a' && ch <= 'z' || ch >= 'A' && ch <= 'Z')
		return 1;
	else 
		return 0;

}

//ÅÐ¶ÏÊý×Ö
int NUM_fair(char ch)
{
	if( ch >= '0' && ch <= '9')
		return 1;
	else 
		return 0;
}

//DFAÅÐ¶ÏÊÇ·ñÎªÊý×Ö
int IF_num(char &ch, int &errorNum)
{

	int type = 2;
	int i = 0;
	char temp[NUM] = "";
	while(NUM_fair(ch))
	{
		temp[i] = ch;
		i++;
		in.get(ch);
	}
	temp[i] = '\0'
;	if(ch == ' ') 
		skipSpace(ch);
	else if(!OP_fair(ch))
	{
		errorNum = 1;	//ŽíÎó±àºÅ1£ººó×ºÁ¬œÓŽíÎó
		return 0;
	}

	//ŽËŽŠ²åÈëœÚµã
	addElement_l(temp, type);
	return 1;
}

//DFAÅÐ¶ÏÊÇ·ñÎª±äÁ¿
int IF_var(char &ch, int &errorNum)
{
	
	int type = 3;
	int i = 0;
	char temp[NUM] = "";
	while(CONST_fair(ch) || NUM_fair(ch))
	{
		temp[i] = ch;
		i++;
		in.get(ch);
	}
	temp[i] = '\0';
	if(ch == ' ')
		skipSpace(ch);//ŽËŽŠŽŠÀí¿Õžñ
	else if(!OP_fair(ch))
	{
		errorNum = 1;	//ŽíÎó±àºÅ£ººó×ºÁ¬œÓŽíÎó
		return 0;
	}

	//ŽËŽŠ²åÈëœÚµã
	addElement_l(temp, type);
	return 1;

}

//DFAÅÐ¶ÏÊÇ·ñÎªËã·û
int IF_OP(char &ch, int &errorNum)
{

	int type;
	char temp[2] = {' ', '\0'};
	int i = 0;
	temp[i] = ch;
	switch(ch)
	{
	case ';':
		type = 0;
		in.get(ch);
		if(ch == ' ')
			skipSpace(ch);
		break;
	case '(':
		type = 0;
		in.get(ch);
		if(ch == ' ')
			skipSpace(ch);//ÕâÀïŽŠÀí¿Õžñ
		else if(!(CONST_fair(ch) || NUM_fair(ch)) && ch != '(')
		{
			if(ch == ')')
				errorNum = 3;	//Ëã·ûÀšºÅÀïÃæÎÞ±äÁ¿ÔËËã
			else errorNum = 2;	//Ëã·ûÖØµþ
			return 0;
		}

		break;
	case ')':
		type = 0;
		in.get(ch);
		if(ch == ' ')
			skipSpace(ch);//ÕâÀïŽŠÀí¿Õžñ
	
		break;
	case '+':
		type = 1;
		in.get(ch);
		if(ch == ' ')
			skipSpace(ch);//ÕâÀïŽŠÀí¿Õžñ
		else if(!(CONST_fair(ch) || NUM_fair(ch)) && ch != '(')
		{
			errorNum = 2;	//Ëã·ûÖØµþ
			return 0;
		}
		break;
	case '-':
		type = 1;
		in.get(ch);
		if(ch == ' ')
			skipSpace(ch);//ÕâÀïŽŠÀí¿Õžñ
		else if(!(CONST_fair(ch) || NUM_fair(ch)) && ch != '(')
		{
			errorNum = 2;	//Ëã·ûÖØµþ
			return 0;
		}
		break;
	case '*':
		type = 1;
		in.get(ch);
		if(ch == ' ')
			skipSpace(ch);//ÕâÀïŽŠÀí¿Õžñ
		/*else if(!(CONST_fair(ch) || NUM_fair(ch)) && ch != '(')
		{
			errorNum = 2;	//Ëã·ûÖØµþ
			return 0;
		}*/
		break;
	case '/':
		type = 1;
		in.get(ch);
		if(ch == ' ')
			skipSpace(ch);//ÕâÀïŽŠÀí¿Õžñ
		else if(!(CONST_fair(ch) || NUM_fair(ch)) && ch != '(')
		{
			errorNum = 2;	//Ëã·ûÖØµþ
			return 0;
		}
		break;
	default:
		errorNum = 4;	//ÎŽÊ¶±ðËã·û
		cout << ch;
		in.get(ch);
		if(ch == ' ')
			skipSpace(ch);//ÕâÀïŽŠÀí¿Õžñ
		return 0;
	}
	//ÌíŒÓœÚµã
	addElement_l(temp, type);
	return 1;
}

//Õ¹ÊŸŽÊ·š·ÖÎöœá¹û
void displayLexi(lexical *lhead)
{
	lexical *ph;
	ph = lhead;
	while(ph != NULL)
	{
		cout << ph->type <<"\t\t" << ph->word<<endl;
		ph = ph->next;
	}
}

//ŽÊ·š·ÖÎöÈë¿Ú³ÌÐò
int lexiMain(char &ch)
{
	int errorNum = 0;
	int sign_lexi = 0;

	initLexical();
	in.get(ch);
	skipSpace(ch);

	cout << "ŽÊ·š·ÖÎö----------\n";
	cout << "ÖÖ±ðÂë£º\t" << "Öµ£º\n";
	while(ch != '#')
	{
		if(CONST_fair(ch))
			sign_lexi = IF_var(ch, errorNum);
		else if(NUM_fair(ch))
			sign_lexi = IF_num(ch, errorNum);	
		else if(OP_fair(ch))
			sign_lexi = IF_OP(ch, errorNum);
		else 
		{
			cout << ch;
			errorNum = 4;	//ÎŽÊ¶±ðµÄ×Ö·û
			sign_lexi = 0;
		}
		if(sign_lexi == 0) return errorNum;
	}
	displayLexi(lhead);
	in.close();
	return errorNum;
}

//ŽòÓ¡ŽÊ·š·ÖÎöŽíÎóÐÅÏ¢
void print_ERROR_lexi(int errorNum)
{
	switch(errorNum)
	{
	case 0:
		cout << "-------ŽÊ·š·ÖÎö³É¹Š\n";break;
	case 1:
		cout << "ŽíÎó±àºÅ£º" << errorNum << "\tÊý×Ö»òÕß±äÁ¿ÊéÐŽÓÐŽí\n";break;
	case 2:
		cout << "ŽíÎó±àºÅ£º" << errorNum <<"\t³öÏÖËã·ûÖØµþ\n";break;
	case 3:
		cout << "ŽíÎó±àºÅ£º" << errorNum << "\t³öÏÖ¿ÕÀšºÅ\n";break;
	case 4:
		cout << "ŽíÎó±àºÅ£º" << errorNum  << "\t³öÏÖÎŽÊ¶±ð×Ö·û\n";break;
	default:
		cout << "ŽíÎó±àºÅ£º" << errorNum << "\tÎŽÖªŽíÎó\n";break;
	}
}

void initSyntax()
{
	p_syntax = lhead;
}

//µ±ÓëSELECTŒ¯ºÏÖÐ·ûºÅÏà»¥Æ¥ÅäÊ±ºò£¬ÖžÕëÓÒÒÆ£¬ÏòÇ°¶ÁÈëÒ»žö·ûºÅ
void readPrev()
{
	p_syntax = p_syntax->next;
}
int E();
/*void print_program()
{
	while(p_syntax!=NULL)
	{
		cout << p_syntax->word;
		p_syntax = p_syntax->next;
	}
	cout << endl;
}*/
 
/*
**FOLLOWŒ¯
**S->ŠÅ:FOLLOW(S)={+,-,#,)}
**G->ŠÅ:FOLLOW(G)={#,)}
*/

int F()
{
	static char op[2];
	static int order;

	//µ±ÓöŒûÀšºÅÊ±ºòŽæÈ¡ÀšºÅÇ°ÃæµÄËã·û
	char temp[2], temp_2[2];
	int to;

	/*
	**-2:ÀšºÅÇ°Ã»Ëã·û
	**0:ÀšºÅÇ°¹¹Ôìºó×ºÊœ£¬Ò»žöËã·û
	**-1£ºÁœžöËã·û,op < OP_order
	**1:ÁœžöËã·û£ºop>=OP_order
	**³öÀšºÅÊ±£º·Ö±ð²Ù×÷£º¶ÔÓÚ-1£¬OP_orderœøÈë¶ÓÁÐ
	*/
	int sign;

	int e;

	if((memory_sem == 1) && (num_sem == 1))
	{
		strcpy(op, OP_sem);
		order = OP_order;
	}
	if(strcmp(p_syntax->word, "(") == 0)
	{
		if(memory_sem == 0)
		{
			sign = -2;
		}
		//ÀšºÅÇ°Ö»ÓÐÒ»žöËã·û
		else if(memory_sem == 1)
		{
			sign = 0;
			strcpy(temp, op);
			to = order;
		}
		
		//ÀšºÅÇ°Ö»ÓÐÒ»žöËã·û
		else if(memory_sem == 2)
		{
			if(order >= OP_order)
			{
				sign = 1;
				addElement_s(op);
				strcpy(temp, OP_sem);
				to = OP_order;
			}
			else
			{
				sign = -1;
				strcpy(temp, op);
				to = order;

				strcpy(temp_2, OP_sem);
			}
		}

		num_sem = 0;
		memory_sem = 0;
	
		cout << '\t' << n++ << '\t' << "F->(E);" << endl;

		//ÏòÇ°¶ÁÈ¡ŽæÈ¡×Ö
		readPrev();
		e = E();
		if(!e) 
			return 0;
		if(strcmp(p_syntax->word, ")") == 0)
		{
		    if(sign == -1)
			{
				addElement_s(temp_2);
				strcpy(op, temp);
				to = order;
				//memory_sem = 1;
				num_sem = 2;
			}
			else if(sign == -2)
			{
				num_sem = 0;
			}
			else
			{
				strcpy(op, temp);
				to = order;
				//memory_sem = 1;
				num_sem = 2;
			}
			

			//ÏòÇ°¶ÁÈ¡ŽæÈ¡×Ö
			readPrev();

			if( strcmp(p_syntax->word, ";") == 0)
			{
				addElement_s(op);
				memory_sem--;
			}
			return 1;
		}
		else 
		{
			cout << "ÀšºÅÆ¥Åä²»³É¹Š" << endl;
			return 0;
		}
	}
	else if(p_syntax->type == 2)
	{
		cout << '\t' << n++ << '\t' << "F->ÕûÊý;" << endl;
		if(memory_sem == 2)
		{
			if(order >= OP_order)
			{
				addElement_s(op);
				strcpy(op, OP_sem);
				order = OP_order;

				strcpy(semTemp,p_syntax->word);
				addElement_s(semTemp);
			}
			else 
			{
				strcpy(semTemp,p_syntax->word);
				addElement_s(semTemp);
				addElement_s(OP_sem);
			}
			memory_sem--;
		}
		else
		{
			strcpy(semTemp,p_syntax->word);
			addElement_s(semTemp);
		}

		readPrev();

		//Óöµœœáœçµ±Ç°×îºóÒ»Ëã·ûÈë¶Ó
		if( strcmp(p_syntax->word, ";") == 0||  strcmp(p_syntax->word, ")") == 0)
		{
			addElement_s(op);
			memory_sem--;
		}
		return 1;
	}
	else if(p_syntax->type == 3)
	{
		cout << '\t' << n++ << '\t' << "F->±êÊŸ·û;" << endl;

		if(memory_sem == 2)
		{
			if(order >= OP_order)
			{
				addElement_s(op);
				strcpy(op, OP_sem);
				order = OP_order;

				strcpy(semTemp,p_syntax->word);
				addElement_s(semTemp);
			}
			else 
			{
				strcpy(semTemp,p_syntax->word);
				addElement_s(semTemp);
				addElement_s(OP_sem);
			}
			memory_sem--;
		}
		else
		{
			strcpy(semTemp,p_syntax->word);
			addElement_s(semTemp);
		}
		readPrev();

		//ŽËŽŠÓŠžÃÓÃ×Ö·û±ÈœÏº¯Êý£¬ÓÃ==Ö±œÓ±ÈœÏ²»³öœá¹û
		if( strcmp(p_syntax->word, ";") == 0||  strcmp(p_syntax->word, ")") == 0)
		{
			addElement_s(op);
			memory_sem--;
		}
		return 1;
	}
	else return 0;
	
}

int S()
{
	int f, s;
	if(strcmp(p_syntax->word, "*") == 0)
	{
		cout << '\t' << n++ << '\t' << "S->*FS;" << endl;

		//ÓÃÓÚÓïÒå·ÖÎö
		OP_sem[0] = '*';
		OP_order = 2;
		memory_sem++;
		num_sem ++;

		//ÏòÇ°¶ÁÈ¡ŽæÈ¡×Ö
		readPrev();
		f = F();
		if(!f) return 0;
		s = S();
		if(!s) return 0;

		return 1;
	}
	else if(strcmp(p_syntax->word, "/") == 0)
	{
		cout << '\t' << n++ << '\t' << "S->/FS;" << endl;

		//ÓÃÓÚÓïÒå·ÖÎö
		OP_sem[0] = '/';
		OP_order = 2;
		memory_sem++;
		num_sem ++;

		//ÏòÇ°¶ÁÈ¡ŽæÈ¡×Ö
		readPrev();
		f = F();
		if(!f) return 0;
		s = S();
		if(!s) return 0;

		return 1;
	}
	//ŽËŽŠÊÇ¹ØÓÚS->¿ÕµÄÇé¿ö
	else if(strcmp(p_syntax->word, ";") == 0 || strcmp(p_syntax->word, ")") == 0 || 
			strcmp(p_syntax->word, "+") == 0 || strcmp(p_syntax->word, "-") == 0)
	{
		cout << '\t' << n++ << '\t' << "S->ŠÅ" << endl;
		return 1;
	}
	else return 0;

}

int T()
{
	int f, s;
	cout << '\t' << n++ << '\t' << "T->FS;" << endl; 
	f = F();
	if(!f)
		return 0;
	s = S();
	if(!s)
		return 0;

	return 1;
}

int G()
{
	int t, g;
	if(strcmp(p_syntax->word, "+") == 0)//ÕâÀïŒÇ×ÅÔÚ¿ŽÒ»¿Ž£¬µ¥ÒýºÅ£¬Ë«ÒýºÅ
	{
		cout << '\t' << n++ << '\t' << "G->+TG;" << endl;

		//ÓÃÓÚÓïÒå·ÖÎö
		OP_sem[0] = '+';
		OP_order = 1;
		memory_sem++;
		num_sem ++;

		//ÏòÇ°¶ÁÈ¡ŽæÈ¡×Ö
		readPrev();
		t = T();
		if(!t) return 0;
		g = G();
		if(!g) return 0;

		return 1;
	}
	else if(strcmp(p_syntax->word, "-") == 0)
	{
		cout << '\t' << n++ << '\t' << "G->-TG;" << endl;

		//ÓÃÓÚÓïÒå·ÖÎö
		OP_sem[0] = '-';
		OP_order = 1;
		memory_sem++;
		num_sem ++;

		//ÏòÇ°¶ÁÈ¡ŽæÈ¡×Ö
		readPrev();
		t = T();
		if(!t) return 0;
		g = G();
		if(!g) return 0;

		return 1;
	}
	//ŽËŽŠÐŽG->¿ÕµÄÇé¿ö
	else if(strcmp(p_syntax->word, ";") == 0 || strcmp(p_syntax->word, ")") == 0)
	{
		cout << '\t' << n++ << '\t' << "G->ŠÅ" << endl;
		return 1;
	}
	else return 0;
}

int E()
{
	int t, g;
	if ((strcmp(p_syntax->word,"+") == 0)|| (strcmp(p_syntax->word,"-") == 0))
		readPrev();
	cout << '\t' << n++ << '\t' << "E->TG;" << endl;
	t = T();
	if(t == 0)
		return 0;
	g = G();
	if(g == 0)
		return 0;

	return 1;
}

//Óï·š·ÖÎöÈë¿Ú
int syntaxMain()
{
	int n = 0;

	initSyntax();
	initSemantic();
	//print_program();
	
	cout << endl;
	
	cout << "----------µÝ¹éÏòÏÂ·ÖÎöÎÄ·š\n";
	cout << "\tÐòºÅ" << "\tÍÆµŒ¹«Êœ\n";
	n = E();
	if(!n)
	{
		cout << "žÃ±íŽïÊœ²»ÊôÓÚžÃÎÄ·š\n";
		cout << "--------Óï·š·ÖÎöÊ§°Ü\n";
		return 0;
	}
	else
	{
		cout << "žÃ±íŽïÊœÊÇ·ûºÏžÃÎÄ·šµÄËãÊõ±íŽïÊœ\n";
		cout << "--------Óï·š·ÖÎö³É¹Š\n";
		return 1;
	}
}

//ŽæÔÚÈ±ÏÝ£º²»ÄÜ×ª»»Ç¶Ì×ÀšºÅµÄÄæ²šÀŒÊœ
void displaySem(semantic *shead)
{
	semantic *ph;
	ph = shead;

	cout << "\n---------Äæ²šÀŒÊœ\n";
	while(ph != NULL)
	{
		cout << ph->word<<" ";
		ph = ph->next;
	}
	cout << "\n---------³É¹ŠÊä³ö\n";
}

int main()
{
	char ch;
	int errorNum;
	int t;

	errorNum = lexiMain(ch);
	print_ERROR_lexi(errorNum);
	if(errorNum == 0)
	{
		if(syntaxMain())
			displaySem(shead);
	}
	system("pause");
	return 0;
}
