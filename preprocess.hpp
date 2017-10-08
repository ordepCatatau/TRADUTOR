
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
#include <cstring>
using namespace std;

/*Variaveis globais*/
string tabela_mnt[100][2]; //contem os nomes das macros definidas no programa [nome da macro][numero da linha que comeca a macro na mdt]
int contador_mnt=0;

string tabela_mdt[100]; //contem os corpos das macros [numero da linha][corpo da linha]
int contador_mdt=0;

string tabela_equ[100][2]; //contem os label e seus valores [contador de equ][0-label 1-valor][tamanho max da label]
int contador_equ=0;

string token[1000];

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
void add_tabela_mnt(string nome_macro)
{
	//cout<<"add_tabela_mnt"<<endl;
	tabela_mnt[contador_mnt][0] = nome_macro;
	tabela_mnt[contador_mnt][1] = to_string(contador_mdt); //linha da mdt onde comeca o corpo da macro
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
	unsigned int i=0;
	//cout<<"add_tabela_mdt"<<endl;
	//corpo_macro[corpo_macro.length()-3]='\0';
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

int separa_tokens(string arq_in,string comp_mode)
{
	int i=0,pv=0;

	char c;

	string word;

	FILE*  file = fopen(arq_in.c_str(),"r");

	//Separa  tokens
	while(1)
	{
		c = toupper(fgetc(file));
		//cout<<c;
		if(pv==1 && c!='\n') continue;
		else pv=0;

		if(c==EOF) 
		{
			token[i] = word;
			i++;
			token[i] = "\n";
			break;
		}

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
				else if(token[i-1]==" ")
				{
					token[i-1]="\n";
				}
				else if(comp_mode=="-o") 
				{
					token[i] = '\n';
					i++;
				}
				word = "";
				break;
			}
			case(';'):
			{	
				pv = 1;
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
			case(','):
			{
				if(word!="")
				{ 
					token[i] = word;
					i++;
					token[i] = " ";
					i++;
				}
				token[i] = ",";
				i++;	
				token[i] = " ";
				i++;
				word = "";
				break;
			}
			case('+'):
			{
				if(word!="")
				{ 
					token[i] = word;
					i++;
					token[i] = " ";
					i++;
				}
				token[i] = "+";
				i++;	
				token[i] = " ";
				i++;
				word = "";
				break;
			}
			default:
			{
				word+=c;
			}
		}		
	}//FIM de while

	fclose(file);
	return i;
}

void prep(string comp_mode,string arq_in)
{
	
	int i=0,tam=0;
	int flag_diretiva=0;
	int flag_macro=0;
	int flag_text=0;
	int flag_data=0;
	int flag_p=0, flag_m=0, flag_o=0;
	int flag_salva_macro=0;
	int n_linha=0;

	//char aux[100];

	string valor_equ;
	string linha_mnt;
	string corpo_macro;
	string macro;
	string nome_macro;
	string linha;
	string sec_data="";
	string sec_text="";
	string sec_pretext = "";
	string arq_out;

	FILE* file;

	tam = separa_tokens(arq_in,comp_mode);

//	for(i=0;i<tam;i++)
//		cout<<token[i];

	//Escrever no arquivo limpo
	if(comp_mode=="-p")
	{ 
		flag_p=1;
		arq_out = arq_in.replace(arq_in.begin()+arq_in.find(".asm"),arq_in.end(),".pre");
	}
	else if(comp_mode=="-m")
	{ 
		flag_m=1;
		arq_out = arq_in.replace(arq_in.begin()+arq_in.find(".asm"),arq_in.end(),".mcr");	
	}
	else if(comp_mode=="-o")
	{
		//flag_o=1;
		flag_p=1;
		flag_m=1;
		arq_out = arq_in.replace(arq_in.begin()+arq_in.find(".asm"),arq_in.end(),".inter");
	}
	file = fopen(arq_out.c_str(),"w");
	

	i=0;
	while(i<tam)
	{

		if(token[i]=="EQU" && flag_p)
		{
			add_tabela_equ(token[i+2],token[i+4]);
			flag_diretiva=1;
		}
		else if(token[i]=="IF" && flag_p)
		{	
			flag_diretiva = (busca_tabela_equ(token[i+2])=="0") ? 2:1;
		}
		else if(token[i] == "TEXT")
		{
			flag_data=0;
			flag_text=1;
		}
		else if(token[i] == "DATA")
		{
			flag_data=1;
			flag_text=0;
		}
		else if(token[i]=="MACRO" && flag_m)
		{
			flag_macro=1;
			flag_diretiva=1;
			nome_macro = token[i-4];
			add_tabela_mnt(nome_macro);
		}
		else if(token[i]=="END" && flag_m)
		{	
			flag_macro=0;
			flag_diretiva=1;
			add_tabela_mdt(corpo_macro);
			corpo_macro = "";
		}

		//Carrega o corpo da macro
		if(flag_macro && !flag_diretiva) corpo_macro += token[i];

		//Busca o nome da macro
		linha_mnt = busca_tabela_mnt(token[i]);
		if(linha_mnt!="")
		{
			flag_diretiva=1;
			//Resgata a macro da tabela
			macro = busca_tabela_mdt(stoi(linha_mnt,nullptr,10));
			flag_salva_macro=1;
		}

		//Fim de linha
		linha +=token[i]; 
		if(token[i]=="\n")
		{
			n_linha++;
			if(!flag_diretiva && !flag_macro) //So escrever se habilitado
			{
				if(flag_text)
				{	
					//sec_text +=itoa(n_linha,aux,10);
					sec_text +=linha;
				}
				else if(flag_data)
				{
					//sec_data +=itoa(n_linha,aux,10);
					sec_data +=linha;
				}
				else
				{ 
					//sec_pretext +=itoa(n_linha,aux,10);
					sec_pretext	+=linha;
				}
			}
			
			else
			{
				if(flag_text)
				{ 
					//sec_text +=itoa(n_linha,aux,10);
					sec_text+="\n";
				}
				else if(flag_data) 
				{
					//sec_data +=itoa(n_linha,aux,10);
					sec_data+="\n";
				}
				else
				{
					//sec_pretext +=itoa(n_linha,aux,10);
					sec_pretext+="\n";
				}
			}
			
			if(flag_salva_macro)
			{ 
				flag_salva_macro=0;
				sec_text+=macro;
			}
			linha = "";
			if(flag_diretiva>0)
				flag_diretiva--; //Habilita a escrita no arquivo limpo
		}


		i++;
	}

	fputs(sec_pretext.c_str(),file);
	//cout<<sec_pretext<<endl;
	fputs(sec_text.c_str(),file);
	//cout<<sec_text<<endl;
	fputs(sec_data.c_str(),file);
	//cout<<sec_data<<endl;
	fclose(file);
	
	
}//FIM do preprocessamento