#include <stdio.h>
#include <vector>
#include <set>
#include "CPUTimer/CPUTimer.h"

#define MAX_VERTICES 2100

using namespace std;

vector<int> adjacency_list[MAX_VERTICES];
set<int> clique; 
int vertex_color[MAX_VERTICES]; 
int NUM_VERTICES; 
int vertex_degree[MAX_VERTICES];
set<int> clique_components[MAX_VERTICES];


void calculateDegrees(set<int>* vertices)
{
	int num_adjacent_vertices;
	for(int vertex=0; vertex<NUM_VERTICES; vertex++)
	{
		num_adjacent_vertices = adjacency_list[vertex].size();
		vertex_degree[vertex] = 0;
		for(int i=0; i<num_adjacent_vertices; i++)
		{
			if(vertices->count(adjacency_list[vertex][i]))
			{
				vertex_degree[vertex]++;
			}
		}
	}
}

void calculateDegreesWholeGraph()
{
	for(int i=0;i<NUM_VERTICES;i++)
	{
		vertex_degree[i] = adjacency_list[i].size();
	}
}

int vertexWithMaximumDegree(set<int>* vertices)
{
	int max_degree = -1;
	int vertex_with_max_degree; 
	
	for (set<int>::iterator it=vertices->begin(); it!=vertices->end(); it++)
	{
		if(vertex_degree[*it] > max_degree)
		{
 			max_degree = vertex_degree[*it];
 			vertex_with_max_degree = *it;
		}
	}
	return vertex_with_max_degree;
}


bool isAdjacent(int v, int w)
{
	int num_adjacent_vertices = vertex_degree[v];
	
	for(int i=0; i<num_adjacent_vertices; i++)
	{
		if(w == adjacency_list[v][i])
		{
			return true;
		}
	}

	return false;
}


bool hasEdgesToEveryoneInClique(int vertex, set<int>* clique)
{
	int click_size = clique->size();
	
	for (set<int>::iterator it=clique->begin(); it!=clique->end(); it++)
	{
		if(!isAdjacent(vertex,*it))
		{
			return false;
		}
	}

	return true;
}


bool isVertexInTheClique(int vertex, set<int>* clique)
{
	for (set<int>::iterator it=clique->begin(); it!=clique->end(); it++)
	{
		if(vertex==*it)
			return true;
	}
	
	return false;
}

void findCliqueIncludingSoloVertex(set<int>* vertices)
{
	int vertex_with_max_degree;
	int max_degree;

	vector<int> adjacent_vertices;
	int num_adjacent_vertices;

	calculateDegrees(vertices);
	clique.clear(); 

	vertex_with_max_degree = vertexWithMaximumDegree(vertices); 
	clique.insert(vertex_with_max_degree);
	
	while(true)
	{
		adjacent_vertices.clear(); 
		for (set<int>::iterator i=vertices->begin(); i!=vertices->end(); i++)
		{
			if(!isVertexInTheClique(*i,&clique))
			{
				if(hasEdgesToEveryoneInClique(*i,&clique))
				{
					adjacent_vertices.push_back(*i);
				}
			}
		}
		
		num_adjacent_vertices = adjacent_vertices.size();
		
		if(!num_adjacent_vertices)
		{
			break;	
		}

		max_degree = -1;
		vertex_with_max_degree = -1;
		
		for(int i=0; i<num_adjacent_vertices; i++)
		{
			if(vertex_degree[adjacent_vertices[i]] > max_degree)
			{
				max_degree = vertex_degree[adjacent_vertices[i]];
				vertex_with_max_degree = adjacent_vertices[i];
			}
		}
		
		clique.insert(vertex_with_max_degree);
	}
}

void findClique(set<int>* vertices)
{
	findCliqueIncludingSoloVertex(vertices);

	if(clique.size() == 1)
	{
		clique.clear();
	}
}

void findClickWholeGraph()
{
	set<int> vertices;

	for(int i=0; i<NUM_VERTICES; i++)
	{
		vertices.insert(i);
	}

	findClique(&vertices);
}


void clearUsedColors(bool* adjacent_used_colors)
{
	for(int i=0; i<MAX_VERTICES; i++)
	{
		adjacent_used_colors[i] = false;
	}
}

void clearVerticesColor()
{
	for(int i=0; i<NUM_VERTICES; i++)
	{
		vertex_color[i] = -1;
	}
}

void setAdjacentUsedColors(int vertex, int num_adjacent_vertices, bool* adjacent_used_colors)
{
	for(int i=0;i<num_adjacent_vertices;i++)
	{
		if(vertex_color[adjacency_list[vertex][i]] != -1)
		{
			adjacent_used_colors[vertex_color[adjacency_list[vertex][i]]] = true;
		}
	}
}

void colorVertex(int vertex)
{

	bool adjacent_used_colors[MAX_VERTICES];
	int num_adjacent_vertices;

	calculateDegreesWholeGraph();

	num_adjacent_vertices = vertex_degree[vertex];

	clearUsedColors(adjacent_used_colors);
	
	setAdjacentUsedColors(vertex, num_adjacent_vertices, adjacent_used_colors);
	
	for(int i=0;i<MAX_VERTICES;i++)
	{
		if(!adjacent_used_colors[i])
		{
			vertex_color[vertex] = i;
			break;
		}
	}

	for(int i=0;i<num_adjacent_vertices;i++)
	{
		if(vertex_color[adjacency_list[vertex][i]] == -1)
		{
			colorVertex(adjacency_list[vertex][i]);
		}
	}
}

void initAndColor(int vertex)
{
	clearVerticesColor();
	colorVertex(vertex);
}

int cobertura_minimal()
{
	int q;
	set<int> v_linha;
	set<int> candidates;
	int max_degree = 0;
	int vertex_with_max_degree = -1;
	int numero_de_coberturas;

	for(int i=0;i<=NUM_VERTICES;i++)
	{
		clique_components[i].clear();
	}

	for(int i=0;i<NUM_VERTICES;i++)
	{
		v_linha.insert(i);
	}
	
	q = 0;
	do
	{
		q++;

		findCliqueIncludingSoloVertex(&v_linha);
		clique_components[q] = clique;

		for ( set<int>::iterator i = clique_components[q].begin(); i != clique_components[q].end(); i++ )
		{
			v_linha.erase(*i);
		}

	}while(!v_linha.empty());

	numero_de_coberturas = q;


	for(int i=0;i<NUM_VERTICES;i++)
	{
		vertex_degree[i] = adjacency_list[i].size();
	}

	for(int p=1;p<=q;p++)
	{
		candidates.clear();

		for(int i=0;i<NUM_VERTICES;i++)
		{
			if(hasEdgesToEveryoneInClique(i,&clique_components[p]))
			{
				candidates.insert(i);
			}	
		}
		while(!candidates.empty())
		{

			max_degree = 0;
			vertex_with_max_degree = -1;
			for ( set<int>::iterator i = candidates.begin(); i != candidates.end(); i++ )
			{
				if( vertex_degree[*i] > max_degree )
				{
					max_degree = vertex_degree[*i];
					vertex_with_max_degree = *i;
				}
			}

			clique_components[p].insert(vertex_with_max_degree);
			candidates.erase(vertex_with_max_degree);
		}

	}
	

	return numero_de_coberturas;
}

void run_tudo(bool should_print)
{
	findClickWholeGraph();
	if(should_print) 
	{
		for ( set<int>::iterator i = clique.begin(); i != clique.end(); i++ )
			printf("%d\n",*i);
		printf("Temanho do clique encontrado = %d\n",clique.size());
	}
	

	if(should_print) printf("---------------------\n");

	initAndColor(1);
	
	if(should_print) 
	{
		for(int i=0;i<NUM_VERTICES;i++)
		{
			printf("cor do %d = %d\n",i,vertex_color[i]);
		}
	}
	

	int max_color = -1;
	for(int i=0;i<NUM_VERTICES;i++)
	{
		if(vertex_color[i] > max_color)
		{
			max_color = vertex_color[i];
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
	int min_size_clique = NUM_VERTICES+1;
	int soma=0;
	for(int i=1;i<=numero_de_coberturas;i++)
	{
		soma += (int) clique_components[i].size();
		if( (int) clique_components[i].size() > max_size_clique)
		{
			if(clique_components[i].size() > 1) max_size_clique = clique_components[i].size();
		}
		if( (int) clique_components[i].size() < min_size_clique)
		{
			if(clique_components[i].size() > 1) min_size_clique = clique_components[i].size();
		}
		num_clicks[clique_components[i].size()] = num_clicks[clique_components[i].size()] + 1;
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

	scanf(" edge %d %d",&NUM_VERTICES,&M);
	printf("NUM_VERTICES=%d M=%d\n",NUM_VERTICES,M);

	for(int i=0;i<M;i++)
	{
		scanf(" e %d %d ",&v1, &v2);
		adjacency_list[v1].push_back(v2);
		adjacency_list[v2].push_back(v1);
		
	}

	

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

	printf("Executou %d execucoes em %fs (media: %f exec/clique_components)\n",runs,timer_total.getCPUTotalSecs(),timer_total.getCPUTotalSecs()/runs);

	return 0;
}
