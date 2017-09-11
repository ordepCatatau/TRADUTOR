/*
Software Básico - 2/2017 - Turma B
Trabalho 1 - Programa Montador de Assembly inventado para código máquina
Alunos: Saulo Cardoso - xx/xxxxx
        Pedro Henrique V. Berto - 12/0020432
*/

/* 
	Preprocessamentto 

	entrada: arquivo de codigo bruto
	saida: arquivo de codigo limpo com as diretivas resolvidas

	Retirar espacos, \n e tabs desnecessarios  

	Resolver diretivas:
	-EQU
	-IF
	-MACRO 
*/

#include <iostream>
#include <string>
using namespace std;

/*Variaveis globais*/
string tabela_mnt[100][2]; //contem os nomes das macros definidas no programa [nome da macro][numero da linha que comeca a macro na mdt]
int contador_mnt=0;

string tabela_mdt[100]; //contem os corpos das macros [numero da linha][corpo da linha]
int contador_mdt=0;

string tabela_equ[100][2]; //contem os label e seus valores [contador de equ][0-label 1-valor][tamanho max da label]
int contador_equ=0;

int contador_linha=0;


//Retorna o valor do equ
string busca_tabela_equ(string label)
{
	int i=0;
	
	while(i<contador_equ)
	{
		if(label == tabela_equ[i][0])
		{ 
			return tabela_equ[i][1];
		}
		i++;
	}
	return"X";
}
//Adiciona definicoes do EQU
void add_tabela_equ(string label, string valor)
{
	if(busca_tabela_equ(label)=="X")//Previne dupla entrada na tabela
	{
		tabela_equ[contador_equ][0] = label;
		tabela_equ[contador_equ][1] = valor;
		//cout<<label;
		//cout<<valor;
		contador_equ++;
	}
}
//retorna a posicao
string busca_tabela_mnt(string nome_macro)
{
	int i=0;
	//cout<<"busca_tabela_mnt"<<endl;
	while(i<=contador_mnt)
	{
		if(nome_macro== tabela_mnt[i][0])
		{ 
			
			return tabela_mnt[i][1];
		}
		i++;
	}
	return"";
}
//Adiciona na tabela mnt
void add_tabela_mnt(string nome_macro,int linha)
{
	//cout<<"add_tabela_mnt"<<endl;
	tabela_mnt[contador_mnt][0] = nome_macro;
	tabela_mnt[contador_mnt][1] = to_string(linha); //linha da mdt onde comeca o corpo da macro
	contador_mnt++;
}
void print_tabela_mnt()
{
	int i;
	cout<<"Tabela MNT"<<endl;
	for(i=0;i<=contador_mnt;i++)
		cout<<tabela_mnt[i][0]<<" "<<tabela_mnt[i][1]<<endl;	
}
//retorna o corpo da macro
string busca_tabela_mdt(int linha)
{
	string corpo_macro;
	//cout<<"busca_tabela_mdt"<<endl;
	corpo_macro += tabela_mdt[linha];
	return corpo_macro;
}
//Adiciona na tabela mdt
void add_tabela_mdt(string corpo_macro)
{
	int i=1;
	//cout<<"add_tabela_mdt"<<endl;
	corpo_macro[corpo_macro.length()-3]='\0';
	for(;i<corpo_macro.length();i++)
		tabela_mdt[contador_mdt] += corpo_macro[i];

	contador_mdt++;
}
void print_tabela_mdt()
{
	int i;
	cout<<"Tabela MDT"<<endl;
	for(i=0;i<=contador_mdt;i++)
		cout<<tabela_mdt[i]<<endl;	
}

void cleaner(string arq)
{
	int i=0,k=0;
	int flag_diretiva=0;
	int flag_macro=0;
	int cont_macro=0;
	char c;
	string valor_equ;
	string linha_mnt;
	string word;
	string token[1000];
	string corpo_macro;
	string macro;
	string nome_macro;
	string linha;
	FILE*  

	file = fopen(arq.c_str(),"r");

	//Separa  tokens
	while(1)
	{
		c = toupper(fgetc(file));
		if(c==EOF) break;
		//cout<<c<<endl;
		switch(c)
		{
			case(' '):
			{
				if(word!="")
				{ 
					token[i] = word;
					i++;
					token[i] = " ";
					i++;
				}
				word = "";
				break;
			}
			case('\t'):
			{
				if(word!="")
				{ 
					token[i] = word;
					i++;
					token[i] = " ";
					i++;
				}
				word = "";
				break;
			}
			case('\n'):
			{

				if(word!="")
				{ 
					token[i] = word;
					i++;
					token[i] = "\n";
					i++;
				}
				word = "";
				break;
			}
			case(';'):
			{
				if(word!="")
				{ 
					token[i] = word;
					i++;
					token[i] = "\n";
					i++;
					
				}
				token[i] = ";";
				i++;
				token[i] = " ";
				i++;

				word = "";
				break;
			}
			case(':'):
			{
				if(word!="")
				{ 
					token[i] = word;
					i++;
					token[i] = " ";
					i++;
				}
				token[i] = ":";
				i++;	
				token[i] = " ";
				i++;
				word = "";
				break;
			}

			default:
				word+=c;			
			
		}	
	}//FIM de while
	fclose(file);

	//Escrever no arquivo limpo
	file = fopen("intermediario.txt","w");
	i=0;
	while(token[i]!="")
	{
		//cout<<token[i];

		//Substitui as definicoes do equ
		valor_equ = busca_tabela_equ(token[i]);			
		if(valor_equ!="X") token[i] = valor_equ;
		
		//Carrega o corpo da macro
		if(flag_macro) 
		{	
			//cout<<token[i];
			corpo_macro += token[i];
		}
		//Expande a macro
		linha_mnt = busca_tabela_mnt(token[i]);
		if(linha_mnt!="")
		{
			flag_diretiva=1;
			//cout<<"linha_mnt "<<linha_mnt<<endl;
			macro = busca_tabela_mdt(stoi(linha_mnt,nullptr,10));
			//cout<<macro<<endl;
			fputs(macro.c_str(),file);
		}

		if(token[i]=="EQU")
		{
			//cout<<"Equ detected"<<endl;
			add_tabela_equ(token[i-4],token[i+2]);
			flag_diretiva=1;
		}
		else if(token[i]==";")
		{
			fputs((linha).c_str(),file);
			flag_diretiva=1;
		}
		else if(token[i]=="IF")
		{
			if(stoi(busca_tabela_equ(token[i+2]),nullptr,10)!=1)
			{	
				flag_diretiva=2;
			}
			else
			{
				flag_diretiva=1;
			}
		}
		else if(token[i]=="MACRO")
		{
			flag_macro=1;
			nome_macro = token[i-4];
			add_tabela_mnt(nome_macro,contador_mdt);
		}
		else if(token[i]=="END")
		{	
			flag_macro=0;
			flag_diretiva=1;
			add_tabela_mdt(corpo_macro);
			corpo_macro = "";
		}

		linha +=token[i]; 

		if(token[i]=="\n")
		{
			if(!flag_diretiva && !flag_macro) //So escrever se habilitado
				fputs(linha.c_str(),file);
			
			linha = "";

			flag_diretiva--; //Habilita a escrita no arquivo limpo
			if(flag_diretiva<0)
				flag_diretiva=0;
		}

		i++;
	}
	fclose(file);
}//FIM de cleaner

string tabela_instrucoes[14][3]={"ADD","1","2", //[numero da instrucao][0->nome 1-> opcode 2-> tamanho][nome]
                                "SUB","2","2",
                                "MULT","3","2",
                                "DIV","4","2",
                                "JMP","5","2",
                                "JMPN","6","2",
                                "JMPP","7","2",
                                "JMPZ","8","2",
                                "COPY","9","3",
                                "LOAD","10","2",
                                "STORE","11","2",
                                "INPUT","12","2",
                                "OUTPUT","13","2",
                                "STOP","14","1"};

string tabela_diretivas[3]   = { "SPACE",//[numero da diretiva][tamanho max da diretiva]
                                "CONST",
                                "SECTION"};

string tabela_simbolos[1000][4];//[linha da tabela][0-simbolo 1-valor 2-relativo/absoluto 3-extreno/publico]
int contador_simbolos=0;

string tabela_definicoes[1000][3];//[][0-simbolo 1-Valor 2-relativo/absoluto]

string tabela_uso[1000][3];//[][0-simbolo 1-Endereco 2-operacao]

string tabela_erro[1000][2]; //[][numero da linha e tipo do erro]
int contador_erro=0;

string codigo_obj[1000];


void add_tabela_erro(int n_linha,int n_erro)
{
	tabela_erro[contador_erro][0] = n_linha;
	tabela_erro[contador_erro][1] = n_erro;
	contador_erro++;
}

int busca_tabela_diretivas(string token)
{
	int i=0;
	//cout<<"busca_tabela_mnt"<<endl;
	while(i<=10)
	{
		if(token== tabela_diretivas[i])
			return i;
		i++;
	}
	return -1;
}

int busca_tabela_instrucoes(string token)
{
	int i=0;
	//cout<<"busca_tabela_mnt"<<endl;
	while(i<=10)
	{
		if(token== tabela_instrucoes[i][0])
			return i;
		i++;
	}
	return -1;
}
/*
int scanner(string token)
{
	int i=0;
	if(isdig(token[0])) return 1;
	add_tabela_erro(contador_linha,10) //label invalido (vide tabela de erro)

	while(i<=token.length())
	{

	}

	return 0;
}
void parser(string linha[10])
{
	int i;
	int diretiva;
	int instrucao;


	while(linha[i]!="\n")
	{
		
		else
		{
			scanner(linha[i]);
		}



	}

}*/

void passagem_unica()
{
	FILE* file = fopen("intermediario.txt","r");
	
	int i=0,k;
	int flag_text;
	int flag_data;
	int flag_const=0;
	int flag_space=1;
	int diretiva;
	int instrucao;
	int aux;
	int end=0;

	char word[100];
	string token[100];

	while(1)
	{
		fscanf(file,"%s",word);
		//cout<<word<<endl;

		token[i] = word;
		if(token[i] == "FIM") break;

		diretiva 	= busca_tabela_diretivas(token[i]);
		instrucao 	= busca_tabela_instrucoes(token[i]);
		
		if(flag_const==1) codigo_obj[end] = token[i];
			flag_const=0;

		if(flag_space==1) 
		{
			aux = atoi(word);
			if(aux)
			{
				for(k=0;k<aux;k++)
				{
					codigo_obj[end]="00";
					end++;
				}
			}
			else
			{
				codigo_obj[end] = "00";
			}
			flag_space=0;
		} 

		if(diretiva!=-1)
		{
			switch(diretiva)
			{
				case(0)://SPACE
				{
					codigo_obj[end] = "00";
					break;
				}
				case(1)://CONST
				{
					flag_const=1;
					break;
				}
			}
				
		}
		else if(instrucao!=-1)
		{
			codigo_obj[end] = to_string(instrucao);
		}
		end++;
		

		i++;

	}
	fclose(file);
	for(k=0;k<end;k++)
	{
		cout<<codigo_obj[k]<<endl;
	}

}

int main()
{
	string file_name = "SOMA.txt";
	cleaner(file_name);

	passagem_unica();

	//print_tabela_mnt();
	//print_tabela_mdt();

return 0;
}

/*
{
Tabela de erros:
1 -> numero de operandos errado -> Sintatico
2 -> simbolo redefinido -> Semantico
3 -> Operaçao/diretiva nao identificada -> sintatico
4 -> duas seções data ou data antes de texto -> semantico
5 -> duas secoes texto ou data antes do texto -> semantico
6 -> diretiva na seção errada -> semantico
7 -> seção faltando -> semantico
8 -> seção inválida -> sintatico
9 -> Simbolo nao declarado -> semantico
10-> Label com simbolos nao validos -> lexico
11-> Divisão por 0 -> semantico
12-> Modificar const -> semantico
13-> Valor negativo de SPACE -> semantico
14-> Label nao válido(label==diretiva) -> semantico
15-> Acesso de vetor maior do que declarado -> semantico
16-> Pulo para seção inválida -> semantico
17-> Seção data com instrucoes ou diretivas -> semantico
18-> Const sem valor ->sintatico
19-> section depois de label
20->Space com tamanho errado ->semantico
21-> Caso const nao seja numero ->sintatico
22-> Vetor sem valor A+ ->Sintático
23-> public vetor ->semantico
24-> extern mal declarado ->sintatico
25-> argumento invalido -> semantico
26-> Hexadecimal com valores errados ex: G
27-> mais de uma seção data semantico
28-> declaraçao equ errado ->sintatico
29-> declaraçao if errado ->sintatico
30-> dois equ iguais->semantico
31-> if com label errada ->semantico
32-> Equ posição errada ->semantico
33->equ sem numero ->sintatico
34->label de equ reservada->semantico
35->label equ errada ->lexico
36-> jump longo - >nao suportado
37-> equ e label repetida
38-> Copy com 3 operandos ->sintatico
39-> operação com instrucao
40->Vetor acessa memoria invalida
41->label com escreverinteiro ou lerinteiro->semantico
}
*/