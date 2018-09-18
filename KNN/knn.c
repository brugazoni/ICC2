#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* florteste e flortreino : declarações de structs que serão usadas para 
 * armazenar os dados lidos dos arquivos. Em florteste, há a string guess para armazenar
 * o resultado do knn
*/

typedef struct florteste
{
	double sepallength;
	double sepalwidth;
	double petallength;
	double petalwidth;
	char species[100];
	
	char guess[100];
	
}florteste;

typedef struct flortreino
{
	double sepallength;
	double sepalwidth;
	double petallength;
	double petalwidth;
	char species[100];
}flortreino;

/* florcompara : struct usada para guardar um valor de distância euclidiana ao nome da flor do arquivo treino 
 * a partir da qual foi calculada
*/

typedef struct florcompara
{
	double dist;
	char tipotreino[100];
}florcompara;

/* ordena : função de insertion sort para ordenar structs de dados de flores
 * 
 * @Parâmetros:
 * vetflorcompara: vetor de structs de flores
 * tam: tamanho do vetor
 * 
 * @Retornos esperados:
 * vetor ordenado, a partir das distâncias, em ordem crescente
*/

void ordena (florcompara* vetflorcompara, int tam)
{
	int i = 1;


	while (i < tam)
	{
		int j = i-1;
		florcompara chave = vetflorcompara[i];

		while (j >= 0 && chave.dist < vetflorcompara[j].dist)
		{
			vetflorcompara[j+1] = vetflorcompara[j];
			j--;
		}

		vetflorcompara[j+1] = chave;
		i++;
	}
}

/* deucli: função utilizada para cálculos das distâncias euclidianas.
 * Tendo em vista erros de aproximação, foi relevado o cálculo da raiz quadrada,
 * visto também que os valores das distâncias em si não importam, apenas a relação de
 * ordenação entre eles.
 * 
 * @Parâmetros:
 * linhateste : vetor de informações de uma flor do arquivo teste
 * linhatreino : vetor de informações de uma flor do arquivo treino
 *
 * @Retornos:
 * deucli : distância euclidiana, sem a extração de sua raiz quadrada
*/ 

double deucli (double* linhateste, double* linhatreino)
{
	int i;
	double temp = 0;
	double disteucli;
	
	for (i = 0; i < 4; i++)
	{
		temp += pow ((linhatreino[i] - linhateste[i]), 2);
	}
	
	disteucli = temp;
	return disteucli;
}

/* checak : função para conferir a validez do k informado
 * na entrada e para retornar o número de flores em cada arquivo.
 * 
 * @Parametros:
 * file : ponteiro para arquivo a ser escaneado
 * k : parâmetro do knn
 * 
 * @Retornos:
 * caso bem-sucedido, retorna o número de linhas no arquivo.
 * caso o k seja inválido, retorna 0
 */
 
int checak (FILE* file, int k)
{
	char c;
	int cont = -1;
	
	do
	{
		c = fgetc(file);
		
		if (c == '\n')
		{
			cont++;
		}
		
	} while (!feof(file));

	if (k <= cont) 
	{
		return cont;
	}
	
	else 
	{
		return 0;
	}
}

/* main : na função main, faremos o armazenamento dos dados dos arquivos,
 * assinalaremos os dados das flores a serem tratados em cada laço e executaremos
 * um sistema de pontuação de forma a encontrar o knn.
 * 
 * @Parametros:
 * k, i, j, m, cont: contadores de laços
 * contacertos: quantos knn correspondem à informação do especialista
 * propacertos: proporção de acertos / total de casos
 * 
 * pontsetosa, pontvirginica, pontversicolor: pontuações a serem usadas para
 * determinar quantas vezes uma flor correspondeu ao knn quando suas distâncias euclidianas
 * são avaliadas, pós ordenação
 * 
 * ntestes: número de flores no arquivo teste
 * nexemplos: número de exemplos no arquivo de treino
 * nometreino: nome do arquivo de treino
 * nometeste: nome do arquivo de teste
 * 
 * cabectreino e cabecteste: visto que os cabeçalhos dos arquivos lidos não são relevantes,
 * essas strings guardam sua leitura de forma a proceder com o fscanf a partir dos dados das flores
 * 
 * arquitreino e arquiteste: ponteiros para os arquivos a serem lidos
 */

int main (int argc, char* argv[])
{
	int k, i, j, m;
	
	int cont = 0;
	int contacertos = 0;
	double propacertos;
	
	int pontsetosa = 0;
	int pontvirginica = 0;
	int pontversicolor = 0;
	
	int ntestes = 0;
	int nexemplos = 0;
	
	char* nometreino;
	char* nometeste;
	char* cabectreino;
	char* cabecteste;
	
	FILE* arquitreino;
	FILE* arquiteste;

	scanf("%m[^ ] %m[^ ] %d", &nometreino, &nometeste, &k);

	arquitreino = fopen(nometreino, "r");
	arquiteste = fopen(nometeste, "r");
	
	nexemplos = checak (arquitreino, k);
	ntestes = checak (arquiteste, -1);
	
	
	if (nexemplos == 0)
	{
		printf("k is invalid\n");
		return 0;
	}
	
	florteste vetflorteste[ntestes];
	flortreino vetflortreino[nexemplos];
	florcompara vetflorcompara[nexemplos];
	
	rewind(arquiteste);
	rewind(arquitreino);
	
	fscanf(arquitreino, "%m[^ \n]", &cabectreino);
	fscanf(arquiteste, "%m[^ \n]", &cabecteste);

	for (i = 0; i < ntestes; i++)
	{
			fscanf(arquiteste, "%lf,%lf,%lf,%lf,", &vetflorteste[i].sepallength,
			&vetflorteste[i].sepalwidth, &vetflorteste[i].petallength, &vetflorteste[i].petalwidth);
			
			fscanf(arquiteste, "\"%[^\"]\"", vetflorteste[i].species);
	}
	
	for (i = 0; i < nexemplos; i++)
	{
			fscanf(arquitreino, "%lf,%lf,%lf,%lf,", &vetflortreino[i].sepallength,
			&vetflortreino[i].sepalwidth, &vetflortreino[i].petallength, &vetflortreino[i].petalwidth);
			
			fscanf(arquitreino, "\"%[^\"]\"", vetflortreino[i].species);
	}
	
	fclose(arquitreino);
	fclose(arquiteste);
	
	double linhatreino[4];
	double linhateste[4];
	
	for (i = 0; i < ntestes; i++)
	{
		linhateste[0] = vetflorteste[i].sepallength;
		linhateste[1] = vetflorteste[i].sepalwidth;
		linhateste[2] = vetflorteste[i].petallength;
		linhateste[3] = vetflorteste[i].petalwidth;
		
		for (j = 0; j < nexemplos; j++, cont++)
		{
		linhatreino[0] = vetflortreino[j].sepallength;
		linhatreino[1] = vetflortreino[j].sepalwidth;
		linhatreino[2] = vetflortreino[j].petallength;
		linhatreino[3] = vetflortreino[j].petalwidth;
		
		vetflorcompara[j].dist = deucli(linhateste, linhatreino);
		strcpy (vetflorcompara[j].tipotreino, vetflortreino[j].species);
		}
		
		ordena(vetflorcompara, nexemplos);
		
		for (m = 0; m < k; m++)
		{
			if (strcmp (vetflorcompara[m].tipotreino, "virginica") == 0) pontvirginica++;
			else if (strcmp (vetflorcompara[m].tipotreino, "setosa") == 0) pontsetosa++;
			else if (strcmp (vetflorcompara[m].tipotreino, "versicolor") == 0) pontversicolor++;
		}
		
		if(pontsetosa > pontvirginica && pontsetosa > pontversicolor)
		{
			strcpy(vetflorteste[i].guess,"setosa");
		}
		
		else if(pontvirginica > pontsetosa && pontvirginica > pontversicolor) 
		{
			strcpy(vetflorteste[i].guess,"virginica");
		}
		
		else if (pontversicolor > pontsetosa && pontversicolor > pontvirginica) 
		{
			strcpy(vetflorteste[i].guess,"versicolor");
		}
		
		else 
		{
			strcpy(vetflorteste[i].guess, vetflorcompara[0].tipotreino);
		}
		
		pontsetosa = 0;
		pontvirginica = 0;
		pontversicolor = 0;
	
	}
	
	for (i = 0; i < ntestes; i++)
	{
		printf("%s %s\n", vetflorteste[i].guess, vetflorteste[i].species);
		if (strcmp (vetflorteste[i].guess, vetflorteste[i].species) == 0) contacertos++;
	}
	
	propacertos = ((double) contacertos / (double) ntestes);
	
	printf("%.4lf\n", propacertos);
	
	free (nometreino);
	free (nometeste);
	free (cabectreino);
	free (cabecteste);

	return 0;
}
