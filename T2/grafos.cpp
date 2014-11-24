#include <stdio.h>
#include <vector>
#include <set>
#include "CPUTimer/CPUTimer.h"

#define MAX_VERTICES 2100

using namespace std;

vector<int> graph[MAX_VERTICES];
set<int> click; 
int color_array[MAX_VERTICES]; //array that has the color of each vertex
int N; //number of current vertex
int degree[MAX_VERTICES];
set<int> S[MAX_VERTICES];

//returns the degree of the vertex
int calculateDegrees(set<int>* vertices)
{
	int num_vizinhos;
	for(int v=0;v<N;v++)
	{
		num_vizinhos = graph[v].size();
		degree[v] = 0;
		for(int i=0;i<num_vizinhos;i++)
		{
			if(vertices->count(graph[v][i]))
			{
				degree[v]++;
			}
		}
	}
}

//finds the vertex with the biggest degree
int vMaxDegree(set<int>* vertices)
{
	int max=-1;
	int v_max; //value to be returned
	int size; //degree of the vertex
	//loop while there's non visited vertex
	for ( set<int>::iterator i = vertices->begin(); i != vertices->end(); i++ )
	{
		if(degree[*i] > max)
		{
 			max = degree[*i];
 			v_max = *i;
		}
	}
	return v_max;
}

//looks for a edge between v and w vertex
bool isAdjacent(int v, int w)
{

	int num_arestas = degree[v];
	//while there's edges checks for the w variable
	for(int i=0;i<num_arestas;i++)
		if(w == graph[v][i])
			return true;
	
	return false;
}

//checks if v is adjacent to the click
bool isAdjacentToClick(int v, set<int>* click)
{
	int click_size = click->size();
	//while theres vertex in click
	for ( set<int>::iterator i = click->begin(); i != click->end(); i++ )
		if(!isAdjacent(v,*i))
			return false;
	
	return true;
}

//finds if v is part of the click
bool isClick(int v)
{
	//check every vertex of the click and compare to v
	for (set<int>::iterator i = click.begin(); i != click.end(); i++)
		if(v==*i)
			return true;
	
	return false;
}

void findClickPodeSolitario(set<int>* vertices)
{
	calculateDegrees(vertices);
	//clear click cache
	click.clear(); //a
	int v = vMaxDegree(vertices); // b
	click.insert(v);

	vector<int> v_adj;
	//size of the adjacent vertex
	int adj_size;
	//max degree of the adjacent vertex
	int adj_max_degree, max_degree;
	//loop while there's adjacents vertex
	while(1)
	{
		v_adj.clear(); // c
		for ( set<int>::iterator i = vertices->begin(); i != vertices->end(); i++ )
		{
			//if the vertex is not in the click
			if(!isClick(*i))
			{
				if(isAdjacentToClick(*i,&click))
					//add i to the v_adj if its adjacent and is not part of the click
					v_adj.push_back(*i);
			}
		}
		

		adj_size = v_adj.size();
		//end loop if tehres no v_adj
		if(!adj_size) break;

		adj_max_degree = -1;
		max_degree = 0;
		//checks for v_adj	 vertex with the biggest degree
		for(int i=0;i<adj_size;i++)
			if(degree[v_adj[i]] > max_degree)
			{
				max_degree = degree[v_adj[i]];
				adj_max_degree = v_adj[i];
			}
		
		//add to click the biggest degree v_adj with an adjacent edge
		click.insert(adj_max_degree);
	}
}

void findClick()
{

	set<int> V;

	for(int i=0;i<N;i++)
	{
		V.insert(i);
	}

	findClickPodeSolitario(&V);

	if(click.size() == 1)
	{
		click.clear();
	}
}

//color the graph's vertex
void color(int v)
{
	for(int i=0;i<N;i++)
	{
		degree[i] = graph[i].size();
	}

	//array of already used colors in the adjacents' vertex
	bool used_colors[MAX_VERTICES];
	int num_adj = degree[v];

	//initiate the value as false
	for(int i=0;i<MAX_VERTICES;i++)
		used_colors[i] = false;
	
	//checks for already used colors
	for(int i=0;i<num_adj;i++)
	{
		if(color_array[graph[v][i]] != -1)
		{
			used_colors[color_array[graph[v][i]]] = true;
		}

	}
	//checks for the lowest not used color
	//eg: in this array [wite][grey][blue][][red] it would use the used_colors[3]
	for(int i=0;i<MAX_VERTICES;i++)
	{
		if(!used_colors[i])
		{
			color_array[v] = i;
			break;
		}
	}
	//color every adjacents' vertex if its not colord
	for(int i=0;i<num_adj;i++)
	{
		if(color_array[graph[v][i]] == -1)
		{
			color(graph[v][i]);
		}
	}
}
//init every color and start coloring
void initAndColor(int v)
{
	for(int i=0;i<N;i++)
		color_array[i] = -1;
	
	color(v);
}

//==============================================

/*
2. encontra cobertura minimal :
(a) Inicie com V' = V . Faça q = 1.
(b) Encontre uma clique em G(V') correspondente ao conjunto S'q .
(c) Faça V' ← V' \ S'q .
(d) Se V' != ∅. Faça q ←− q + 1 e volte ao passo (b)

(e) Caso contrário, para p de 1 a q Faça:
i. Adicione a S'p um vértice que seja vizinho a todos os vértices já em S'p dentre estes,
escolha vértice de maior grau.
ii. Se nenhum vértice puder ser adicionado a S p , Pare! S p  ́e uma clique.
iii. Faça S p = S p .
(f) O conjunto S1 , . . . , S'q  ́e uma cobertura minimal em cliques.
*/


int cobertura_minimal()
{
	int q;
	set<int> v_linha;
	set<int> candidates;
	int max_degree = 0;
	int adj_max_degree = -1;
	int numero_de_coberturas;

	for(int i=0;i<=N;i++)
	{
		S[i].clear();
	}

	for(int i=0;i<N;i++)
	{
		v_linha.insert(i);
	}
	
	q = 0;
	do
	{
		q++;

		findClickPodeSolitario(&v_linha);
		S[q] = click;

		for ( set<int>::iterator i = S[q].begin(); i != S[q].end(); i++ )
		{
			v_linha.erase(*i);
		}

	}while(!v_linha.empty());

	numero_de_coberturas = q;


	for(int i=0;i<N;i++)
	{
		degree[i] = graph[i].size();
	}

	for(int p=1;p<=q;p++)
	{
		candidates.clear();

		for(int i=0;i<N;i++)
		{
			if(isAdjacentToClick(i,&S[p]))
			{
				candidates.insert(i);
			}	
		}
		while(!candidates.empty())
		{

			max_degree = 0;
			adj_max_degree = -1;
			for ( set<int>::iterator i = candidates.begin(); i != candidates.end(); i++ )
			{
				if( degree[*i] > max_degree )
				{
					max_degree = degree[*i];
					adj_max_degree = *i;
				}
			}

			S[p].insert(adj_max_degree);
			candidates.erase(adj_max_degree);
		}

	}
	

	return numero_de_coberturas;
}

void run_tudo(bool should_print)
{
	findClick();
	if(should_print) 
	{
		for ( set<int>::iterator i = click.begin(); i != click.end(); i++ )
			printf("%d\n",*i);
		printf("Temanho do clique encontrado = %d\n",click.size());
	}
	

	if(should_print) printf("---------------------\n");

	initAndColor(1);
	
	if(should_print) 
	{
		for(int i=0;i<N;i++)
		{
			printf("cor do %d = %d\n",i,color_array[i]);
		}
	}
	

	int max_color = -1;
	for(int i=0;i<N;i++)
	{
		if(color_array[i] > max_color)
		{
			max_color = color_array[i];
		}
	}
	if(should_print) printf("Numero de cores usados = %d\n",max_color+1);


	if(should_print) printf("---------------------\n");

	int numero_de_coberturas = cobertura_minimal();

	int num_clicks[MAX_VERTICES];
	for(int i=0;i<MAX_VERTICES;i++)
	{
		num_clicks[i]=0;
	}
	int max_size_clique = -1;
	int min_size_clique = N+1;
	int soma=0;
	for(int i=1;i<=numero_de_coberturas;i++)
	{
		soma += (int) S[i].size();
		if( (int) S[i].size() > max_size_clique)
		{
			if(S[i].size() > 1) max_size_clique = S[i].size();
		}
		if( (int) S[i].size() < min_size_clique)
		{
			if(S[i].size() > 1) min_size_clique = S[i].size();
		}
		num_clicks[S[i].size()] = num_clicks[S[i].size()] + 1;
	}

	float media = ((float)soma)/numero_de_coberturas;

	if(should_print) 
	{
		for(int i=2;i<MAX_VERTICES;i++)
		{
			if(num_clicks[i]) printf("Numero de cliques de tamanho %d = %d\n",i,num_clicks[i]);
		}
		printf("Maior tamanho de clique = %d\n",max_size_clique);
		printf("Menor tamanho de clique = %d\n",min_size_clique);
		printf("Media tamanho de clique = %.02f\n",media);
	}
}


int main(int argn, char* argc[])
{

/*
	N = 6;
	graph[0].push_back(1);
	graph[0].push_back(2);
	graph[0].push_back(3);
	graph[0].push_back(4);
	graph[0].push_back(5);

	graph[1].push_back(0);
	graph[1].push_back(3);
	graph[1].push_back(4);
	graph[1].push_back(5);

	graph[2].push_back(0);

	graph[3].push_back(0);
	graph[3].push_back(1);

	graph[4].push_back(0);
	graph[4].push_back(1);
	graph[4].push_back(5);

	graph[5].push_back(0);
	graph[5].push_back(1);
	graph[5].push_back(4);
*/

	char c_lido;
	int M;
	int v1,v2;

	while(1)
	{
		scanf(" %c",&c_lido);
		if(c_lido != 'c')
		{
			break;
		}
		else
		{
			scanf("%*[^\n]");
		}
	}

	scanf(" %*s %d %d",&N,&M);
	printf("N=%d M=%d\n",N,M);

	for(int i=0;i<M;i++)
	{
		scanf(" e %d %d ",&v1, &v2);
		graph[v1].push_back(v2);
		graph[v2].push_back(v1);
		//printf("%d %d\n",v1,v2);
	}

	//====================================================

	CPUTimer timer_total;
	int runs = 0;

	do
	{
		runs++;

		printf("#Run %d\n",runs);
		timer_total.start();
		run_tudo(false);
		timer_total.stop();
	}
	while(timer_total.getCPUTotalSecs() < 5.0f);

	printf("Executou %d execucoes em %fs (media: %f exec/s)\n",runs,timer_total.getCPUTotalSecs(),timer_total.getCPUTotalSecs()/runs);

	return 0;
}
