
#include <iostream>
#include <string>
#include <cstring>
using namespace std;
#define DEBUG 0

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

string tabela_simbolos[1000][7];//[linha da tabela][0-simbolo 1-endereco do simbolo definido 2-definido ou nao 3-ultimo endereco onde o simbolo foi encontrado 4-(TEXT1 DATA0) 5-adicionar no endereco(vetor) 6-CONST ou nao]
int contador_simbolos=0;

string tabela_definicoes[1000][3];//[][0-simbolo 1-n_inst 2-relativo/absoluto]
int contador_def;

string tabela_uso[1000][3];//[][0-simbolo 1-Endereco 2-operacao]
int contador_uso;

int tabela_erro[1000][2]; //[linha da tabela][linha no codigo e tipo do erro]
int contador_erro=0;

string codigo_obj[1000];

string linha_[1000];

int label[100][2];
int contador_label=0;


void add_tabela_erro(int n_linha,int n_erro)
{
	tabela_erro[contador_erro][0] = n_linha;
	tabela_erro[contador_erro][1] = n_erro;
	contador_erro++;
}
void print_tabela_erro()
{
	int i=0;
	string tipo;


	cout<<"----------\tTabela de Erros\t----------"<<endl;

	for(;i<contador_erro;i++)
	{
		cout<<"linha - "<<tabela_erro[i][0]<<"\terro - "<<tabela_erro[i][1];
		if(tabela_erro[i][1]<5 || tabela_erro[i][1]==7 || tabela_erro[i][1]==8 || tabela_erro[i][1] ==12 || tabela_erro[i][1]==15)
		cout<<"\tSEMANTICO"<<endl;
		else if(tabela_erro[i][1]==5 || tabela_erro[i][1]==6 || tabela_erro[i][1]==9 ||tabela_erro[i][1]==11 ||tabela_erro[i][1] == 13||tabela_erro[i][1]==14)
		cout<<"\tSINTATICO"<<endl;
		else if(tabela_erro[i][1]==10)
		cout<<"\tLEXICO"<<endl;
		else cout<<"\tERRO desconhecido"<<endl;
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

	cout<<"----------\tTabela de Simbolos\t----------"<<endl;

	for(;i<contador_simbolos;i++)
	{
		cout<<tabela_simbolos[i][0]<<"\t"<<tabela_simbolos[i][1]<<"\t"<<tabela_simbolos[i][2]<<"\t"<<tabela_simbolos[i][3]<<"\t"<<tabela_simbolos[i][4]<<"\t"<<tabela_simbolos[i][5]<<"\t"<<tabela_simbolos[i][6]<<endl;
	}
	cout<<endl;
}

int corrige_end_vetor(int vetor[100][2],int contador_shift) //LEMBRAR DE CORRIGIR ESSA FUNCAO
{
	int i=0;
	int aux;
	int end;
	char str[100];

	/*Tirar o -1 do (end-1)
	pq tem um \n no codigo objeto*/

	for(i=0;i<contador_shift;i++)
	{
		end = vetor[i][0];
		//cout<<i<<" "<<codigo_obj[end]<<" "<<vetor[i][1]<<endl;
		aux = atoi(codigo_obj[end].c_str());
		aux+= vetor[i][1];
		sprintf(str,"%d",aux);
		codigo_obj[end] = str;
	}
	return 1;

}

void corrige_tabela()
{
	int i=0,j=0;

	string end,end_;
	for(;i<contador_simbolos;i++)
	{
		if(tabela_simbolos[i][2]=="0")
		{
			
			for(j=0;j<contador_label;j++)
			{
				if(i==label[j][0])
				{
					//ERRO 01 - Rotulos ausentes
					add_tabela_erro(label[j][1],1);
				}

			}
			
		}
		end = tabela_simbolos[i][3];
		while(end!="-1")
		{
			end_ = codigo_obj[stoi(end,nullptr,10)];
			codigo_obj[stoi(end,nullptr,10)] = tabela_simbolos[i][1];
			end=end_;
		}
	}

}

void add_tabela_simbolos(string simbolo,int def,int end, int section)
{

	tabela_simbolos[contador_simbolos][0] = simbolo;
	tabela_simbolos[contador_simbolos][2] = to_string(def);	
	tabela_simbolos[contador_simbolos][4] = to_string(section);

	tabela_simbolos[contador_simbolos][6] = "0";
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
		if(!isalnum(word[i])) flag_result=1;
		
		if(word[i]=='_') flag_result=0;	
	}
	
	if(word==",") flag_result=0;
	//cout<<endl;
	return flag_result;
}

void passagem_unica(string arq_in, string arq_out)
{
	int i=0,n_op=0,j=0;
	int n_inst;
	int flag_space=0, flag_const=0, flag_section=0;
	int flag_copy=0;
	int flag_inst=0;
	int section_data=0, section_text=0;
	int pos=1;
	int end=0;
	int end_mem=0;
	int linha=1;
	int ok_section=0;
	int dois_pontos=0;
	int virgula=0;	
	int contador_shift=0;
	int shift_v=0;
	int shift[100][2];
	int flag_inv=0;
	int global_start;
	int flag_p=0;
	int flag_rot=0;
	int flag_def=0;
	int num;




	string word="";
	string hex="";
	string _word="";
	string rot="";
	//string sec_text="";
	//string sec_data="";

	char c;
	char aux[100];

	//ERRO 8
	int flag_div=0;
	int contador_div=0;
	string label_div[100][2];
	int flag_div_zero=0;

	//ERRO 3
	int flag_jump=0;
	int contador_jump=0;
	string label_jump[100][2];

	//ERRO 15
	int flag_mem=0;
	int contador_mem=0;
	string label_mem[100][2];
	//int flag_mod_const=0;



	arq_in = arq_in.replace(arq_in.begin()+arq_in.find(".asm"),arq_in.end(),".inter");
	
	FILE* file = fopen(arq_in.c_str(),"r");
	FILE* file_ = fopen(arq_out.c_str(),"w");


	while(c!=EOF)
	{
		c = fgetc(file);
			
		
		//Fim de token
		if(c==' '|| c=='\n')
		{
			//cout<<word<<endl;
			//Solucao do ultimo token 
			if(flag_const)
			{
				//cout<<"flag_CONST"<<endl;
				flag_const=0;
				
				//sprintf(aux,"%d",stoi(word,nullptr,16));
				
				if((word[0]=='0' && word[1]=='X') || (word[1]=='0' && word[2]=='X') || word[word.length()-1]=='H')
				{
					//cout<<word<<" HEXA"<<endl;
					try
					{
						num = stoi(word,nullptr,16) ;	
					}
					catch(...)
					{
						//Erro 14 - tipo de argumento invalido
						add_tabela_erro(linha,14);
						num = -1;
					}
				}	
				else
				{
					//cout<<word<<" DEC"<<endl;
					try
					{
						num = stoi(word,nullptr,10) ;	
					}
					catch(...)
					{
						//Erro 14 - tipo de argumento invalido
						add_tabela_erro(linha,14);
						num = -1;
					}

				}

				itoa(num,aux,10);
				codigo_obj[end] = aux;
				end++;
				
				word="";
			}
			if(flag_space)
			{
				//cout<<"flag_SPACE"<<endl;
				flag_space=0;
				if(pos!=1)
				{
					try
					{
						num = stoi(word,nullptr,10);
					}
					catch(...)
					{
						//Erro 14 - tipo de argumento invalido
						add_tabela_erro(linha,14);
						num = 1;
					}
					
					for(i=0;i<num;i++)
					{
						codigo_obj[end] = "0";
						end++;
					}	
					word="";
				}
				else
				{
					codigo_obj[end] = "0";
					end++;
					
				}

				n_op=0;
			}
			if(flag_section)
			{
				//cout<<"flag_section"<<endl;
				flag_section=0;
				//Erro 13 - secao invalida
				if(word!="TEXT" && word!="DATA")
				{ 	
					add_tabela_erro(linha,13);
					word="";
				}
			}
			if(flag_p)
			{
				//cout<<"flag_p"<<endl;
				flag_p=0;
				shift_v = atoi(word.c_str());
				shift[contador_shift][0] = end-1;
				shift[contador_shift][1] = shift_v;
				contador_shift++;
				word="";
			}
			if(flag_jump)
			{
				//cout<<"flag_jump"<<endl;
				//cout<<word<<" "<<linha<<endl;
				flag_jump=0;
				label_jump[contador_jump][0] = word;
				label_jump[contador_jump][1] = to_string(linha);
				contador_jump++;
			}
			if(flag_mem ||(flag_copy && (pos==4||pos==6)))
			{
				//cout<<word<<endl;
				flag_mem=0;
				label_mem[contador_mem][0] = word;
				label_mem[contador_mem][1] = to_string(linha);
				contador_mem++;
				if(flag_copy)
				{
					flag_copy=0;
					
				}
			}
			if(flag_copy && word!=",")
			{
				try
				{
					num = stoi(word,nullptr,10);
					//Erro 14 - Tipo de argumento invalido
					add_tabela_erro(linha,14);
				}
				catch(...)
				{
					//cout<<"instrucao com tipo de argumento valido"<<endl;
				}	

			}
			if(flag_div)
			{
				//	cout<<"flag_div"<<endl;
				flag_div=0;

				label_div[contador_div][0] = word;
				label_div[contador_div][1] = to_string(linha);
				contador_div++;
			}
			if(flag_inst)
			{
			//	cout<<"flag_inst"<<endl;
				flag_inst=0;
				try
				{
					num = stoi(word,nullptr,10);
					//Erro 14 - Tipo de argumento invalido
					add_tabela_erro(linha,14);
				}
				catch(...)
				{
					//cout<<"instrucao com tipo de argumento valido"<<endl;
				}				
			}


			//Caracteres Especiais
			if(word==",")
			{
				word="";
				n_op++;
			//	if(pos!=3 && pos!=5) cout<<"Virgula no lugar errado"<<endl;
			}
			if(word=="+")
			{
				flag_p=1;
				word="";
				n_op=n_op+2;
			//	if(pos!=3 && pos!=5) cout<<"+ no lugar errado"<<endl;
			}
			if(word==":")
			{
				word="";
				dois_pontos++;	
				n_op=0;	
				flag_rot=0;
				if(dois_pontos>1) 
				{
					//Erro 11 - Dois rotulos na mesma linha
					add_tabela_erro(linha,11);
				}
				//ERRO 01 = ROTULO AUSENTE
				if(pos==1) 
				{
					flag_def=1;
					add_tabela_erro(linha,1);
				} 
				//else if(pos!=2) cout<<"Dois pontos no lugar errado"<<endl;
			}
			if(flag_rot)
			{
				flag_rot=0;
			
				//Erro 6 - Instrucoes invalidas
				add_tabela_erro(linha,6);
				
				n_op=0;
				word="";

			}

			//Erro 10 - token invalido LEXICO
			if(scanner(word) && word!="")
			{
				//	cout<<word<<endl;
				add_tabela_erro(linha,10);
				word="";
			}

			//cout<<"oi"<<endl;
			//Identificacao de label
			n_inst = busca_tabela_instrucoes(word);
			
			if(n_inst!=-1 && word!="TEXT" && word!="DATA" && (pos!=1 && pos!=3) && !flag_def)
			{
				//ERRO 14 - TIPO de argumento invalido palavra reservada
				add_tabela_erro(linha,14);
			//	cout<<"Palavra reservada "<<word<<endl;
				word="";
			}

			if(n_inst==-1 && flag_space==0 && flag_const==0 && word!="")
			{

				n_inst = busca_tabela_simbolos(word);
				
				if(n_inst==-1)//Primeira ocorrencia de um simbolo
				{
					if(pos==1)//Definicao de simbolo
					{
						flag_rot=1;
						rot = word;

						if(section_text)
						{
							//cout<<word<<" "<<end<<endl;
							add_tabela_simbolos(word,1,end,1);
						}
						else if(section_data)
						{
							add_tabela_simbolos(word,1,end,0);
						}
						else
						{
					//		cout<<"Definicao de simbolo fora de uma secao"<<endl;
							add_tabela_simbolos(word,1,end,-1);
						}
						

					}
					else//Simbolo sendo usado sem ser definido
					{
						//cout<<word<<" "<<end<<endl;
						label[contador_label][0] = contador_simbolos;
						label[contador_label][1] = linha;
						contador_label++;



						add_tabela_simbolos(word,0,end,1);
						codigo_obj[end] = "-1";
						end++;
					}
				}
				else//Depois da primeira ocorrencia
				{

					if(pos==1)//Definicao de simbolo
					{
						flag_rot=1;
						rot = word;

						if(section_text)
						{
							tabela_simbolos[n_inst][4]="1";
						}
						else if(section_data)
						{
							tabela_simbolos[n_inst][4]="0";
						}
					
						//Simbolo ja declarado
						if(tabela_simbolos[n_inst][2] == "1")
						{
							//Erro 02 - declaracao e rotulos repetidos
							add_tabela_erro(linha,2);	
						}
						else//Simbolo ainda nao declarado
						{
							tabela_simbolos[n_inst][2] = "1";
							tabela_simbolos[n_inst][1] = to_string(end);
						}
					}
					else//Uso depois da primeira ocorrencia do simbolo
					{
						label[contador_label][0] = n_inst;
						label[contador_label][1] = linha;
						contador_label++;
												
						if(tabela_simbolos[n_inst][2] == "1")//Se o simbolo ja foi definido
						{
							//[n_inst][1] -> endereco real
							codigo_obj[end] = tabela_simbolos[n_inst][1];
						}
						else//Se o simbolo nao foi definido ainda
						{
							//[n_inst][3] -> endereco lista
							codigo_obj[end] = tabela_simbolos[n_inst][3];
							tabela_simbolos[n_inst][3] = to_string(end);
						}
						end++;
					}	
				}
			}
			else if(n_inst>=0 && n_inst<14)//Achou uma instrucao
			{

				n_op = atoi(tabela_instrucoes[n_inst][1].c_str());
				//cout<<"Numero de operandos de "<<word<<" "<<n_op<<endl;
				
				if(DEBUG)
				{
					codigo_obj[end] = "\n";
					end++;
					codigo_obj[end] = word;
					end++;
				}
				codigo_obj[end] = to_string(n_inst+1);
				end++;
				

				flag_inst=1;

				//Erro 7 - diretivas ou intrucoes na secao errada
				if(section_text!=1) add_tabela_erro(linha,7);

				//Instrucao DIV
				if(n_inst==3) flag_div =1;

				//Instrucoes de pulo
				if(n_inst>=4 && n_inst<=7) flag_jump=1;

				//Instrucoes de modificacao de variavel
				if(n_inst==10||n_inst==11) flag_mem=1;

				//Instrucao COPY
				if(n_inst==8) flag_copy=1;
				
			}
			else if(n_inst==14)//SPACE
			{

				//Erro 7 - diretivas ou intrucoes na secao errada
				if(section_data!=1) add_tabela_erro(linha,7);

				//	cout<<"SPACE"<<endl;
				flag_space=1;

			}
			else if(n_inst==15)//CONST
			{
				for(j=0;j<contador_simbolos;j++)
				{
					if(tabela_simbolos[j][0]==rot) 
					{
						tabela_simbolos[j][6]="1";
					}
				}

				//Erro 7 - diretivas ou intrucoes na secao errada
				if(section_data!=1) add_tabela_erro(linha,7);
				
				flag_const=1;
				n_op=1;			

			}
			else if(n_inst==16)//SECTION
			{
				//cout<<"SECTION"<<endl;
				n_op=1;
				flag_section=1;
			}
			else if(n_inst==17)//TEXT
			{
				global_start = end;
				n_op=0;
				//Erro 13 - duas secao TEXT (SECCAO INVALIDA)
				if(ok_section==1 || section_text==1) add_tabela_erro(linha,13);
				else if(section_data==1)
				{ 
					ok_section=1;
				}
				section_data=0;
				section_text=1;
			}
			else if(n_inst==18)//DATA
			{
				n_op=0;
				//Erro 13 - duas secao DATA (SECCAO INVALIDA)
				if(ok_section==1 || section_data==1) add_tabela_erro(linha,13);
				else if(section_text==1)
				{ 
					ok_section=1;
				}
				section_data=1;
				section_text=0;
			}
			
			//cout<<word<<" "<<end<<endl;
					
			if(c=='\n' && word!=":")
			{
				//cout<<n_op<<endl;
				if(n_op!=0 && !flag_space && n_inst!=-1) 
				{
					//Erro 9 - quantidade de operandos errada
					add_tabela_erro(linha,9);
				}

				//Reset das variavies
				n_op=0;
				pos=1;
				dois_pontos=0;
				flag_p=0;
				flag_jump=0;
				flag_def=0;

				linha++;
			}
			else if(c==' ')
			{
				n_op--;
				pos++;
			}

			linha_[linha]+=" ";
			linha_[linha]+=word;
			word="";
		}///FIM de if
		else word+=c;

	
	}//FIM de while

	//cout<<"fora"<<endl;
	for(i=0;i<contador_simbolos;i++)
	{
		for(j=0;j<contador_jump;j++)
		{
			
			if(label_jump[j][0]==tabela_simbolos[i][0])
			{	
				
				//Erro 3 - pulo para rotulos invalidos
				if(tabela_simbolos[i][1]=="-1")
				{ 
					add_tabela_erro(atoi(label_jump[j][1].c_str()),3);
				}

				//Erro 4 - pulo para secao invalida
				if(tabela_simbolos[i][4] == "0") add_tabela_erro(atoi(label_jump[j][1].c_str()),4);
			}

		}//FIM de while jmp

		for(j=0;j<contador_div;j++)
		{
			
			if(label_div[j][0]==tabela_simbolos[i][0] && codigo_obj[atoi(tabela_simbolos[i][1].c_str())]=="0")
			{
				//Erro 8 - Divisao por zero
				add_tabela_erro(atoi(label_div[j][1].c_str()),8);

			}
		}//FIM de while div

		for(j=0;j<contador_mem;j++)
		{
			
			if(tabela_simbolos[i][0]==label_mem[j][0] && tabela_simbolos[i][6]=="1") 
			{
				//Erro 15 - modificacao de valor constante
				add_tabela_erro(atoi(label_mem[j][1].c_str()),15);
			
			}
		}//FIM de while mem

	}

	//cout<<"ainda"<<endl;
	//Erro 12 - secao TEXT faltante
	if(section_text==0 && ok_section==0) 
	{
		add_tabela_erro(-1,12);
		global_start=0;
	}
	corrige_tabela();
	corrige_end_vetor(shift,contador_shift);

	//cout<<"PQ??"<<endl;
	//Se nao comecar da section text
	for(i=global_start;i<end;i++)
	{
		//cout<<codigo_obj[i]<<" ";
		fputs(codigo_obj[i].c_str(),file_);
		fputs(" ",file_);
	}
	for(i=0;i<global_start;i++)
	{
		//cout<<codigo_obj[i]<<" ";
		fputs(codigo_obj[i].c_str(),file_);
		fputs(" ",file_);
	}

	//cout<<"ultimo"<<endl;
	//DEBUG
	if(DEBUG)
	{
		print_tabela_simbolos();
	}

	print_tabela_erro();

}//FIM da primeira passagem