#include <stdio.h>
#include <string.h>

/* compara
   * Funcao que compara o início de um bloco de 512 bytes com o início 
   * esperado de um jpeg e retorna 1 caso sejam iguais ou 0 caso não sejam.
   * @Parametros
   *  char* inicio : vetor que guarda um bloco de 512 bytes recebido na main
   * @Retorno
   *  int : 0 caso não haja correspondência, 1 caso haja */

int compara (char* inicio, char* jpeginicio)
{
	int i;
	
	for (i = 0; i < 3; i++)
	{
		if (inicio[i] != jpeginicio[i]) return 0;
	}
	
	return ((unsigned char)inicio[3])/16 == jpeginicio[3]; /* dividimos o último byte
	por 16 de forma a relevarmos o valor de seu último bit, imprevisível.*/
}

/* main
  * a função main esta preparada para receber, atraves de redirecionamento do stdin,
  * uma sequência de bytes de um arquivo, guardados sucessivamente em um bloco de 512. 
  * Temos por objetivo processar este bloco e analisá-lo de forma a identificar uma ou mais imagens
  * jpeg nele, e salvá-las em arquivos separados ao fim.
  * 
  * @Parametros:
  * i : contador de laço
  * match : retorno da função compara
  * fim : retorno da função scanf, a ser utilizado para detectar o fim do arquivo
  * cont : variavel crescente utilizada para nomear os jpegs sucessivamente encontrados
  * bloco : vetor de chars que guarda um bloco de 512 bytes
  * FILE* jpegatual : ponteiro para o arquivo a ser construído a partir de um jpeg atualmente
  * encontrado
  * nome : vetor a ser utilizado para escrever os nomes dos arquivos armazenando jpegs
  * 
  * @Retornos esperados:
  * Uma sequência de arquivos jpeg, caso sejam encontrados na stream, ou um aviso sobre sua
  * ausência.
  * */
  
  
int main (int argc, char* argv[])
{
	int i, match, fim;
	int cont = 0;
	char bloco[512];
	
	char jpeginicio[4] = {0xff, 0xd8, 0xff, 0x0e}; /* declaramos o último byte de comparação
	como 0x0e de forma a preparar sua comparação com o último bit do bloco dividido por 16 (ver
	função compara).*/
	
	FILE* jpegatual = NULL;
	char nome[8];
	
	do //loop de armazenamento e analise de blocos sucessivos
	{
		for (i = 0; i < 512; i++)
		{
			fim = scanf("%c", &bloco[i]);
			if (fim == EOF) break;
		}
		
		if (fim == EOF) break;
		
		// processo de identificação de cabeçalhos de jpeg no bloco:
		match = compara (bloco, jpeginicio);
		
		if (match == 1)
		{
			if (jpegatual != NULL) fclose (jpegatual);
			sprintf (nome, "%03d.jpg", cont);
			jpegatual = fopen (nome, "wb");
			cont++;
		}
		
		//processo de armazenamento das imagens encontradas em arquivos:
		if (jpegatual != NULL) fwrite (bloco, 1, 512, jpegatual); 					 
	}
	while (fim != EOF);
	
	if (jpegatual == NULL) printf("Could not find pictures\n");
	else fclose (jpegatual);
	
return 0;
}
