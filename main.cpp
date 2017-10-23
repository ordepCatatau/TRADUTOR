/*
Software Básico - 2/2017 - Turma B
Trabalho 1 - Programa Montador de Assembly inventado para código máquina
Alunos: Saulo Cardoso - xx/xxxxx
        Pedro Henrique V. Berto - 12/0020432
*/



/*-----------Function main------------*/

/* 
	Essa funcao chama a funcao de preprocessamento e de montagem

	preprocess.hpp
	prep - Remove macros, equ e if de acordo com os argumentos de montagem

	montador.hpp
	passagem unica - Indica erros e traduz codigo assembly para codigo maquina

	Compilar:

	WINDOWS

	entrar no prompt: 
	g++ -o main main.cpp
	
	main -o arquivo_in.asm arquivo_out.o
	ou
	main -p arquivo_in.asm arquivo_out.o
	ou
	main -m arquivo_in.asm arquivo_out.o
	
	dois arquivos sao gerados
	arquivo intermediario (preprocessado)
	arquivo objeto

	LINUX
	

*/


#include "preprocess.hpp"

int main(int argc,char* argv[])
{

	//cout<<"Argc = "<<argc<<" Argv = "<<argv[1]<<endl;

	prep(argv[1],argv[2]);
	
	//if(argv[1]=="-o") 
	passagem_unica(argv[2],argv[3]);

	if(DEBUG)
	{
		print_tabela_mnt();
		print_tabela_mdt();
	}
return 0;
}
