/*=================================================================================================================*/
/* Andre Costa Castilhano - RA: 2659280, Rafael Goulart Mendes - RA: 2684152 e Thierre Trannin - RA: 2619741       */
/*=================================================================================================================*/
/** JUNHO - 2024
Universidade Tecnológica Federal do Paraná
Departamento Acadêmico de Informática
ICSF13 – Fundamentos de Programação 1
Prof. Bogdan Tomoyuki Nassu
Profa. Leyza Baldo Dorini
Prof. Daniel Fernando Pigatto */
/*=================================================================================================================*/

#include "trabalho3.h"
#include <stdio.h>
#include <stdlib.h>

#define PAREDE -2
#define CAMINHO -1
#define PARAMETRO 1.25

/** Gera uma matriz com as propoções fornecidas e a preenche com valor -2 em todos os índices.
 *
 * Parâmetros: unsigned long altura: altura da imagem.
 *             unsigned long largura: largura da imagem.
 *
 * Valor de retorno: a matriz alocada e preenchida. A responsabilidade por desalocá-la é do
 *                   chamador. */

int** geraMatriz(unsigned long altura, unsigned long largura)
{
    int i, j;
    int** matriz_auxiliar;

    matriz_auxiliar=(int**)malloc(sizeof(int*)*altura); // Alocação da primeira coluna da matriz

    for(i=0; i<altura; i++)
        matriz_auxiliar[i]=(int*)malloc(sizeof(int)*largura); // Alocação das linhas da matriz

    for(i=0; i<largura; i++)
    {
        for(j=0; j<altura; j++)
            matriz_auxiliar[j][i]=PAREDE; // Preenche a matriz com -2 em todos os índices
    }

    return matriz_auxiliar; // Retorna o endereço da matriz
}

/** Recebe a matriz auxiliar, aloca a matriz de custo e retorna o número da saída com o menor caminho
 *
 * Parâmetros: int** matriz_custo: matriz auxiliar.
 *             unsigned long altura: altura da matriz.
 *             unsigned long largura: largura da matriz.
 *
 * Valor de retorno: número da saída com o menor caminho */

unsigned long matrizDeCusto(int** matriz_custo, unsigned long altura, unsigned long largura)
{
    int tem_maior=1, x=0;
    unsigned long menor; // Menor número do pixel preto na última coluna da matriz
    unsigned long i, j;

    while(tem_maior) // Enquanto houver um caminho livre
    {
        tem_maior=0;

        for(i=0; i<largura; i++) // Verifica todas as colunas
        {
            for(j=0; j<altura-1; j++) // Verifica todas as linhas
            {
                if(matriz_custo[j][i]==x) // Se x for encontrado
                {
                    if(matriz_custo[j][(i+1)]==CAMINHO) // Se for caminho livre
                    {
                        matriz_custo[j][(i+1)]=x+1; // Recebe a distância até a saída

                        tem_maior=1;
                    }

                    if(matriz_custo[(j+1)][i]==CAMINHO) // Se for caminho livre
                    {
                        matriz_custo[(j+1)][i]=x+1; // Recebe a distância até a saída

                        tem_maior=1;
                    }

                    if(i>0 && matriz_custo[j][(i-1)]==CAMINHO) // Se for caminho livre
                    {
                        matriz_custo[j][(i-1)]=x+1; // Recebe a distância até a saída

                        tem_maior=1;
                    }

                    if(j>0 && matriz_custo[(j-1)][i]==CAMINHO) // Se for caminho livre
                    {
                        matriz_custo[(j-1)][i]=x+1; // Recebe a distância até a saída

                        tem_maior=1;
                    }

                }
            }
        }

        x++; // Incrementa a distância até a saída

        menor=x; // A variável recebe o maior valor possível
    }

    // Encontra a saída do labirinto com o menor caminho
    for(j=0; j<altura-1; j++) // Verifica cada linha da última coluna
    {
        if(matriz_custo[j][largura-1]>0 && matriz_custo[j][largura-1]<menor) // Se o caminho for menor
            menor=matriz_custo[j][largura-1]; // Registra o caminho
    }

    return menor; // Retorna o menor caminho
}

/** Recebe a matriz de custo e faz o calculo para preencher o vetor pontos_vermelhos.
 *
 * Parâmetros: int** matriz: matriz auxiliar.
 *             unsigned long altura: altura da matriz.
 *             unsigned long largura: largura da matriz.
 *             unsigned long numero: menor caminho possível.
 *             Coordenada* pontos_vermelhos: vetor do caminho. */

void calculaPontos(int** matriz, unsigned long altura, unsigned long largura, unsigned long numero, Coordenada* pontos_vermelhos)
{
    int tem_menor=1, contagem=numero, i, j;

    for(j=1; j<altura-1; j++)
    {
        if(matriz[j][largura-1]==numero) // Procura a saída com o menor caminho
        {
            i=largura-1;

            pontos_vermelhos[contagem].x=largura-1;

            pontos_vermelhos[contagem].y=j;

            contagem--;

            break;
        }
    }

    while(tem_menor) // Cálculo para verificação da posição do número
    {
        tem_menor=0; // Inicializa a flag com 0

        if(i>0 && matriz[(j)][(i-1)]==numero-1 ) // Verifica se o número está embaixo
        {
            pontos_vermelhos[contagem].x=i-1;

            pontos_vermelhos[contagem].y=j;

            tem_menor=1;

            i--;
        }
        else if(i<largura-1 && matriz[(j)][(i+1)]==numero-1) // Verifica se o número está em cima
        {
            pontos_vermelhos[contagem].x=i+1;

            pontos_vermelhos[contagem].y=j;

            tem_menor=1;

            i++;
        }
        if(j>0 && matriz[(j-1)][(i)]==numero-1) // Verifica se o número está à esquerda
        {
            pontos_vermelhos[contagem].x=i;

            pontos_vermelhos[contagem].y=j-1;

            tem_menor=1;

            j--;
        }
        else if(j<altura-1 && matriz[(j+1)][(i)]==numero-1) // Verifica se o número está à direita
        {
            pontos_vermelhos[contagem].x=i;

            pontos_vermelhos[contagem].y=j+1;

            tem_menor=1;

            j++;
        }

        numero--; // Decrementa o número procurado

        contagem--; // Decrementa o índice do vetor
    }
}

/** Faz o cálculo para marcas os pontos pretos e por onde começar a encontrar o caminho.
 *
 * Parâmetros: Imagem1C* img: imagem de entrada.
 *             Coordenada** caminho: o caminho descoberto.
 *
 * Valor de retorno: tamanho do menor caminho. */

int encontraCaminho(Imagem1C* img, Coordenada** caminho)
{
    unsigned long i,j;
    unsigned long largura=img->largura;
    unsigned long altura=img->altura;
    int media=0;
    unsigned long tamanho=0;
    Coordenada* pontos_vermelhos;
    int** matriz_auxiliar=geraMatriz(altura,largura); // Criação da matriz auxiliar

    for(i=0; i<largura-1; i++) //
    {
        media=0; // Inicializa a média de cada coluna

        for(j=0; j<altura-1; j++)
            media+=img->dados[j][i]; // Soma todos os valores da altura

        media=media/(altura); // Faz a divisão de media

        for(j=0; j<altura-1; j++)
        {
            if(img->dados[j][i]<(media/PARAMETRO)) // se for menor que a media/PARAMETRO ele marca na matriz auxiliar como caminho
            {
                matriz_auxiliar[j][i]=CAMINHO;

                matriz_auxiliar[j][i+1]=CAMINHO; // Aumenta a linha

                matriz_auxiliar[j+1][i]=CAMINHO; // Aumenta a linha
            }
        }
    }

    for(j=0; j<altura; j++)
    {
        if(matriz_auxiliar[j][0]==CAMINHO) // marca na primeira coluna o primeiro ponto preto como zero
        {
            matriz_auxiliar[j][0]=0;

            break;
        }
    }

    tamanho=matrizDeCusto(matriz_auxiliar, altura,largura);

    pontos_vermelhos=(Coordenada*)malloc(sizeof(Coordenada)*(tamanho+1)); // Aloca um vetor do tamanho do menor caminho +1

    calculaPontos(matriz_auxiliar, altura, largura, tamanho, pontos_vermelhos);

    *caminho=pontos_vermelhos;

    for(i=0; i<altura; i++) // Desaloca a matriz auxiliar
        free(matriz_auxiliar[i]);

    free(matriz_auxiliar); // Desaloca a matriz auxiliar

    return tamanho+1;
}
