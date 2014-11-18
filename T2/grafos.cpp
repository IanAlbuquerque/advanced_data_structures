#include <stdio.h>
#include <vector>

#define MAX_VERTICES 50

using namespace std;

vector<int> lista_adj[MAX_VERTICES];
vector<int> clique;
int N;

/*
(a) Inicie com S, um conjunto vazio OK
(b) Seleciona um v´ertice de V de grau m´aximo em G = (V, E) OK
(c) Adicione a S um v´ertice que seja vizinho a todos os v´ertices j´a em S dentre estes, escolhao v´ertice de maior grau.
(d) Se nenhum v´ertice puder ser adicionado a S, Pare! S ´e uma clique.
*/
int grau(int v)
{
	return lista_adj[v].size();
}

int vGrauMax()
{
	int max=0;
	int v_max;
	int size;
	for(int i=0;i<N;i++)
	{
		size = lista_adj[i].size();
		if(size > max)
		{
 			max = size;
 			v_max = i;
		}
	}
	return v_max;
}

bool existeAresta(int v, int w)
{
	int num_arestas = lista_adj[v].size();
	for(int i=0;i<num_arestas;i++)
	{
		if(w == lista_adj[v][i])
			return true;
	}
	return false;
}

bool ehVizinhoClique(int v)
{
	int clique_size = clique.size();
	for(int i=0;i<clique_size;i++)
	{
		if(!existeAresta(v,clique[i]))
			return false;
	}
	return true;
}

bool ehDoClique(int v)
{
	for(int i=0;i<clique.size();i++)
	{
		if(v==clique[i])
			return true;
	}
	return false;
}

void encontraClique()
{
	clique.clear(); //a

	int v = vGrauMax(); // b
	clique.push_back(v);

	vector<int> v_vizinhos;
	bool eh_vizinho;
	int vizinhos_size;
	int viz_max_grau;
	int max_grau;

	while(1)
	{
		v_vizinhos.clear(); // c
		for(int i=0;i<N;i++)
		{
			if(!ehDoClique(i))
			{
				if(ehVizinhoClique(i))
					v_vizinhos.push_back(i);
			}
		}

		vizinhos_size = v_vizinhos.size();
		if(vizinhos_size == 0) break;

		viz_max_grau = -1;
		max_grau = 0;

		for(int i=0;i<vizinhos_size;i++)
		{ 
			if(grau(v_vizinhos[i]) > max_grau)
			{
				max_grau = grau(v_vizinhos[i]);
				viz_max_grau = v_vizinhos[i];
			}
		}

		clique.push_back(viz_max_grau);
	}
}

int main(int argn, char* argc[])
{
	N = 6;
	lista_adj[0].push_back(1);
	lista_adj[0].push_back(2);
	lista_adj[0].push_back(3);
	lista_adj[0].push_back(4);
	lista_adj[0].push_back(5);

	lista_adj[1].push_back(0);
	lista_adj[1].push_back(3);
	lista_adj[1].push_back(4);
	lista_adj[1].push_back(5);

	lista_adj[2].push_back(0);

	lista_adj[3].push_back(0);
	lista_adj[3].push_back(1);

	lista_adj[4].push_back(1);
	lista_adj[4].push_back(0);
	lista_adj[4].push_back(5);

	lista_adj[5].push_back(1);
	lista_adj[5].push_back(0);
	lista_adj[5].push_back(4);
	encontraClique();

	for(int i=0;i<clique.size();i++)
	{
		printf("%d\n",clique[i]);
	}

	return 0;
}