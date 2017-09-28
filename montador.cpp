/*
Software Básico - 2/2017 - Turma B
Trabalho 1 - Programa Montador de Assembly inventado para código máquina
Alunos: Saulo Cardoso - xx/xxxxx
        Pedro Henrique V. Berto - 12/0020432
*/

/* 
	DUVIDAS

1	Como faz pra identificar um erro dentro da macro(marcar a linha)?
2	Qual linha apontar para rotulos ausentes?
3	Dois pontos define uma declaracao?

*/


#include "preprocess.hpp"
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

string tabela_instrucoes[19][2]={"ADD","1", //[numero da instrucao][0->nome 1->end depois]
                                "SUB","1",
                                "MULT","1",
                                "DIV","1",
                                "JMP","1",
                                "JMPN","1",
                                "JMPP","1",
                                "JMPZ","1",
                                "COPY","2",
                                "LOAD","1",
                                "STORE","1",
                                "INPUT","1",
                                "OUTPUT","1",
                                "STOP","0",
                                "SPACE","0",
                                "CONST","1",
                                "SECTION","1",
                            	"TEXT","0",
                            	"DATA","0"
                                };


string tabela_simbolos[1000][4];//[linha da tabela][0-simbolo 1-endereco do simbolo definido 2-definido ou nao 3-ultimo endereco onde o simbolo foi encontrado]
int contador_simbolos=0;

string tabela_definicoes[1000][3];//[][0-simbolo 1-Valor 2-relativo/absoluto]
int contador_def;

string tabela_uso[1000][3];//[][0-simbolo 1-Endereco 2-operacao]
int contador_uso;

int tabela_erro[1000][2]; //[linha da tabela][linha no codigo e tipo do erro]
int contador_erro=0;

string codigo_obj[1000];

string linha_[1000];


void add_tabela_erro(int n_linha,int n_erro)
{
	tabela_erro[contador_erro][0] = n_linha;
	tabela_erro[contador_erro][1] = n_erro;
	contador_erro++;
}
void print_tabela_erro()
{
	int i=0;

	cout<<"Tabela de erro"<<endl;
	for(;i<contador_erro;i++)
	{
		cout<<i<<" "<<tabela_erro[i][0]<<" "<<tabela_erro[i][1]<<endl;
		cout<<"linha - "<<tabela_erro[i][0]<<" "<< linha_[tabela_erro[i][0]]<<endl;
	}
	cout<<endl;

}

int busca_tabela_simbolos(string token)
{
	int i=0;
	//cout<<"busca_tabela_mnt"<<endl;
	while(i<=contador_simbolos)
	{
		//cout<<token<<" "<<tabela_instrucoes[i][0]<<endl;
		if(token== tabela_simbolos[i][0])
		{
			return i;
		}
		i++;
	}
	return -1;
}

void print_tabela_simbolos()
{
	int i=0;

	cout<<"Tabela Simbolos"<<endl;
	for(;i<contador_simbolos;i++)
	{
		cout<<i<<" "<<tabela_simbolos[i][0]<<" "<<tabela_simbolos[i][1]<<" "<<tabela_simbolos[i][2]<<" "<<tabela_simbolos[i][3]<<endl;
	}
	cout<<endl;

}

void corrige_tabela()
{
	int i=0;

	string end,end_;
	for(;i<contador_simbolos;i++)
	{
		end = tabela_simbolos[i][3];
		while(end!="-1")
		{
			end_ = codigo_obj[stoi(end,nullptr,10)];
			codigo_obj[stoi(end,nullptr,10)] = tabela_simbolos[i][1];
			end=end_;
		}

	} 
	//Erro 1 - DECLARACAO e rotulos ausentes (qual linha apontar?)
	for(i=0;i<contador_simbolos;i++)
		if(tabela_simbolos[i][2]=="0") add_tabela_erro(-1,1);
}

void add_tabela_simbolos(string simbolo,int def,int end)
{

	tabela_simbolos[contador_simbolos][0] = simbolo;
	tabela_simbolos[contador_simbolos][2] = to_string(def);	
	if(def==1)
	{
		tabela_simbolos[contador_simbolos][1] = to_string(end);
		tabela_simbolos[contador_simbolos][3] = "-1";	
	}
	else
	{
		tabela_simbolos[contador_simbolos][1] = "-1";
		tabela_simbolos[contador_simbolos][3] = to_string(end);	
	}
	contador_simbolos++;
}


int busca_tabela_instrucoes(string token)
{
	int i=0;
	//cout<<"busca_tabela_mnt"<<endl;
	while(i<=18)
	{
		//cout<<token<<" "<<tabela_instrucoes[i][0]<<endl;
		if(token== tabela_instrucoes[i][0])
		{
			
			return i;
		}
		
		i++;
	}
	return -1;
}

int scanner(string word)
{
	unsigned int i=0;
	int flag_result=0;

	if(isalpha(word[0])||word[0]=='_'||word[0]==':')
	{
		//cout<<word[i]<<" ";		
		flag_result=0;
	}
	else flag_result=1;
	//cout<<word[i]<<" ";

	for(i=1;i<word.length();i++)
	{
	//	cout<<word[i]<<" ";
		if(!isalnum(word[i])) 
			flag_result=1;
	}
	//cout<<endl;
	return flag_result;
	

}

void passagem_unica(string arq_in, string arq_out)
{
	int i=0,n_op=0;
	int valor;
	int flag_space=0, flag_const=0, flag_section=0;
	int section_data=0, section_text=0;
	int pos=1;
	int end=0;
	int linha=1;
	int ok_section=0;
	int dois_pontos=0;

	string word="";
	string hex="";
	string _word="";
	
	char c;
	char aux[100];
	
	arq_in = arq_in.replace(arq_in.begin()+arq_in.find(".asm"),arq_in.end(),".inter");
	
	FILE* file = fopen(arq_in.c_str(),"r");
	FILE* file_ = fopen(arq_out.c_str(),"w");

	//cout<<"IN - "<<arq_in<<endl;
	//cout<<"OUT - "<<arq_out<<endl;

	while(c!=EOF)
	{
		c = fgetc(file);
		//cout<<c<<endl;

		if(c==' '|| c=='\n')
		{

			cout<<word<<endl;
			//print_tabela_simbolos();
			//analisar o label depois do const

			if(flag_const==1)
			{
				flag_const=0;
				itoa(stoi(word,nullptr,16),aux,10);
				codigo_obj[end] = aux;
				end++;

				linha++;
				word="";
				pos=1;
				dois_pontos=0;
				
				continue;
			}
			else if(flag_space==1)
			{
				flag_space=0;
				if(pos!=1)
				{
					for(i=0;i<stoi(word,nullptr,16);i++)
					{
						codigo_obj[end] = "0";
						end++;
					}		
					linha++;			
					word="";
					pos=1;
					n_op=0;
					dois_pontos=0;
					
					continue;
				}
				else
				{
					codigo_obj[end] = "0";
					end++;
				}
				
			}
			else if(flag_section==1)
			{
				flag_section=0;

				//Erro 13 - secao invalida
				if(word!="TEXT" && word!="DATA")
				{ 	
					add_tabela_erro(linha,13);
				}

				//linha++;			
				//word="";
				//pos=1;
				//n_op=0;
				//continue;
			}

			
			if(word==":")
			{
				dois_pontos++;
				//Erro 1 - declaracao e ROTULOS ausentes
				if(pos==1) add_tabela_erro(linha,1);
			}

			if(dois_pontos>1) 
			{
				dois_pontos=0;
				//Erro 11 - dois rotulos na mesma linha	
				add_tabela_erro(linha,11);
			}

			if(word=="")
			{ 	
				linha++; 
				dois_pontos=0;
				
				continue;
			}

			//Erro 10 - token invalido
			if(scanner(word)) add_tabela_erro(linha,10);

			valor = busca_tabela_instrucoes(word);
			//cout<<valor<<" ";

			if(valor==-1 && flag_space==0 && flag_const==0)
			{
				
				valor = busca_tabela_simbolos(word);
				
				if(valor==-1)//Primeira ocorrencia de um simbolo
				{

					//cout<<word<<" "<<pos<<endl;
					if(pos==1)//Definicao de simbolo
					{
						add_tabela_simbolos(word,1,end);
					}
					else if(word!=":")//Simbolo sendo usado sem ser definido
					{
						add_tabela_simbolos(word,0,end);
						codigo_obj[end] = "-1";
						end++;
					}
					//print_tabela_simbolos();
			
				}
				else//Depois da primeira ocorrencia
				{
					if(pos==1)//Definicao de simbolo
					{
						//Erro 2 - declaracao e rotulos repetidos
						if(tabela_simbolos[valor][2] == "1") add_tabela_erro(linha,2);
						else
						{
							tabela_simbolos[valor][2] = "1";
							tabela_simbolos[valor][1] = to_string(end);
						}
						//print_tabela_simbolos();	
					}
					else
					{					
						if(tabela_simbolos[valor][2] == "1")//Se o simbolo ja foi definido
						{
							//[valor][1] -> endereco real
							tabela_simbolos[valor][1] = to_string(end);
							codigo_obj[end] = tabela_simbolos[valor][1];
						}
						else//Se o simbolo nao afoi definidp ainda
						{
							//[valor][3] -> endereco lista
							codigo_obj[end] = tabela_simbolos[valor][3];
							tabela_simbolos[valor][3] = to_string(end);
						}

						end++;
					}
					
				}
				
			}
			else if(valor>-1 && valor<14)//Achou uma instrucao
			{
				n_op = atoi(tabela_instrucoes[valor][1].c_str());
				//cout<<"Numero de operandos de "<<word<<" "<<n_op<<endl;
				codigo_obj[end] = to_string(valor+1);
				end++;

				//Erro 7 - diretivas ou intrucoes na secao errada
				if(section_data==1) add_tabela_erro(linha,7);

			}
			else if(valor==14)
			{
				//Erro 7 - diretivas ou intrucoes na secao errada
				if(section_text==1) add_tabela_erro(linha,7);

			//	cout<<"SPACE"<<endl;
				flag_space=1;
			}
			else if(valor==15)//CONST
			{
				//Erro 7 - diretivas ou intrucoes na secao errada
				if(section_text==1) add_tabela_erro(linha,7);

			//	cout<<"CONST"<<endl;
				n_op=1;
				flag_const=1;
			}
			else if(valor==16)//SECTION
			{
				//cout<<"SECTION"<<endl;
				n_op=1;
				flag_section=1;
			}
			else if(valor==17)//TEXT
			{
				n_op=0;

				//cout<<"TEXT"<<endl;
				//Erro 16 - duas secao TEXT
				if(ok_section==1 || section_text==1) add_tabela_erro(linha,16);
				else if(section_data==1)
				{ 
					ok_section=1;
				}
				section_data=0;
				section_text=1;
			}
			else if(valor==18)//DATA
			{	
				//cout<<"DATA"<<endl;
				n_op=0;

				//Erro 17 - duas secao DATA
				if(ok_section==1 || section_data==1) add_tabela_erro(linha,17);
				else if(section_text==1)
				{ 
					ok_section=1;
				}
				section_data=1;
				section_text=0;
			}

			//cout<<word<<" "<<valor<<endl;

			linha_[linha]+=" ";
			linha_[linha]+=word;
			word="";

		}
		else word+=c;
		 	
	
		if(c=='\n')
		{
			//Erro 9 - quantidade de operandos errada
			if(n_op!=0 && !flag_space) add_tabela_erro(linha,9);
			
			n_op=0;
			linha++;
			pos=1;
			dois_pontos=0;
			//linha_="";
		}
		else if(c==' ')
		{
			n_op--;
			pos++;
		}

	}

	//Erro 12 - secao TEXT faltante
	if(ok_section==0 && section_text==0) add_tabela_erro(-1,12);

	/*if(!ok_section)
	{
		if(section_text==0)
		{ 
			add_tabela_erro(-1,7);
			cout<<"Secao TEXT faltando"<<endl;
		}
		if(section_data==0)
		{ 
			add_tabela_erro(-1,7);
			cout<<"Secao DATA faltando"<<endl;
		}
	}*/

	corrige_tabela();
	//cout<<endl;
	for(i=0;i<end;i++)
	{
		//cout<<codigo_obj[i]<<" ";
		fputs(codigo_obj[i].c_str(),file_);
		fputs(" ",file_);
	}

	print_tabela_simbolos();
	print_tabela_erro();

	fclose(file_);
	fclose(file);
}

int main(int argc,char* argv[])
{

	//cout<<"Argc = "<<argc<<" Argv = "<<argv[1]<<endl;
	prep(argv[1],argv[2]);
	passagem_unica(argv[2],argv[3]);

	//print_tabela_mnt();
	//print_tabela_mdt();

return 0;
}

/*

string erro[100] = 

Erro 1 check - declaracao e rotulos ausentes
Erro 2 check - declaracao e rotulos repetidos
Erro 3 - pulo para rotulos invalidos
Erro 4 - pulo para secao invalida
Erro 5 - diretiva invalida
Erro 6 - instrucao invalida
Erro 7 check - diretivas ou intrucoes na secao errada
Erro 8 - divisao por zero
Erro 9 check - quantidade de operandos errada
Erro 10 check - token invalido (lexico)
Erro 11 ???- dois rotulos na mesma linha
Erro 12 check - secao TEXT faltante
Erro 13 check(da o erro 12 junto)- secao invalida
Erro 14 - tipo de argumento invalido
Erro 15 - modificacao de um valor constante

Meus erros
Erro 16 check - duas secao TEXT
Erro 17 check - duas secao DATA

3 -> Operaçao/diretiva nao identificada -> sintatico
6 -> diretiva na seção errada -> semantico
check 7 -> seção faltando -> semantico
check 8 -> seção inválida -> sintatico
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
