#include <stdio.h>
#include <vector>
#include <set>
#include "CPUTimer/CPUTimer.h"

#define MAX_VERTICES 2100

using namespace std;

int NUM_VERTICES;
int NUM_EDGES; 

vector<int> adjacency_list[MAX_VERTICES];
set<int> clique; 
int vertex_color[MAX_VERTICES]; 
int vertex_degree[MAX_VERTICES];
set<int> clique_cover_components[MAX_VERTICES];


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
	int clique_size = clique->size();
	
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

void fillVerticesWithWholeGraph(set<int>* vertices)
{
	for(int i=0; i<NUM_VERTICES; i++)
	{
		vertices->insert(i);
	}	
}

void findCliqueWholeGraph()
{
	set<int> vertices;
	fillVerticesWithWholeGraph(&vertices);
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

int smallestColorNotUsed(bool* adjacent_used_colors)
{
	for(int i=0; i<MAX_VERTICES; i++)
	{
		if(!adjacent_used_colors[i])
		{
			return i;
		}
	}
	return MAX_VERTICES;	
}

void colorVertex(int vertex)
{

	bool adjacent_used_colors[MAX_VERTICES];
	int num_adjacent_vertices;

	calculateDegreesWholeGraph();
	num_adjacent_vertices = vertex_degree[vertex];
	clearUsedColors(adjacent_used_colors);
	setAdjacentUsedColors(vertex, num_adjacent_vertices, adjacent_used_colors);
	vertex_color[vertex] = smallestColorNotUsed(adjacent_used_colors);

	for(int i=0; i<num_adjacent_vertices; i++)
	{
		if(vertex_color[adjacency_list[vertex][i]] == -1)
		{
			colorVertex(adjacency_list[vertex][i]);
		}
	}
}

void colorGraphStartingOn(int vertex)
{
	clearVerticesColor();
	colorVertex(vertex);
}

void clearCover()
{
	for(int i=0;i<=NUM_VERTICES;i++)
	{
		clique_cover_components[i].clear();
	}	
}

int findCover()
{
	int component_index;
	set<int> vertices;
	set<int> candidates;
	int max_degree = 0;
	int vertex_with_max_degree = -1;
	int number_of_cover_components;

	calculateDegreesWholeGraph();
	clearCover();

	fillVerticesWithWholeGraph(&vertices);
	
	component_index = 0;
	do
	{
		component_index++;

		findCliqueIncludingSoloVertex(&vertices);
		clique_cover_components[component_index] = clique;

		for(set<int>::iterator it=clique_cover_components[component_index].begin(); it!=clique_cover_components[component_index].end(); it++)
		{
			vertices.erase(*it);
		}

	}
	while(!vertices.empty());

	number_of_cover_components = component_index;

	for(int component_index=1; component_index<=number_of_cover_components; component_index++)
	{
		while(!candidates.empty())
		{
			candidates.clear();
			for(int i=0;i<NUM_VERTICES;i++)
			{
				if(!isVertexInTheClique(i,&clique_cover_components[component_index]))
				{
					if(hasEdgesToEveryoneInClique(i,&clique_cover_components[component_index]))
					{
						candidates.insert(i);
					}
				}	
			}

			max_degree = 0;
			vertex_with_max_degree = -1;
			for(set<int>::iterator it = candidates.begin(); it != candidates.end(); it++)
			{
				if( vertex_degree[*it] > max_degree )
				{
					max_degree = vertex_degree[*it];
					vertex_with_max_degree = *it;
				}
			}
			clique_cover_components[component_index].insert(vertex_with_max_degree);
			candidates.erase(vertex_with_max_degree);
		}
	}
	
	return number_of_cover_components;
}

void runFindClique(bool should_print_results)
{
	findCliqueWholeGraph();

	if(should_print_results) 
	{
		printf("---------------------\n");
		printf("Find Clique Results\n");
		printf("---------------------\n");
		/*
		for (set<int>::iterator it = clique.begin(); it != clique.end(); it++)
		{
			printf("%d\n",*it);
		}
		*/
		printf("Clique size found = %d\n",clique.size());
		printf("---------------------\n");
	}	
}

void runColoring(bool should_print_results)
{
	colorGraphStartingOn(1);
	
	if(should_print_results) 
	{
		printf("---------------------\n");
		printf("Coloring Results\n");
		printf("---------------------\n");
		/*
			for(int i=0;i<NUM_VERTICES;i++)
			{
				printf("Vertex #%d was colored with = %d\n",i,vertex_color[i]);
			}
		*/
		int max_color_used = -1;
		for(int i=0;i<NUM_VERTICES;i++)
		{
			if(vertex_color[i] > max_color_used)
			{
				max_color_used = vertex_color[i];
			}
		}
		printf("Amount of colors used = %d\n",max_color_used+1);
		printf("---------------------\n");
	}	
}

void runFindCover(bool should_print_results)
{
	int number_of_cover_components = findCover();

	if(should_print_results)
	{
		printf("---------------------\n");
		printf("Clique Cover Results\n");
		printf("---------------------\n");
		int num_cliques_with_size[MAX_VERTICES];

		for(int i=0;i<MAX_VERTICES;i++)
		{
			num_cliques_with_size[i]=0;
		}

		int max_clique_size = -1;
		int min_clique_size = NUM_VERTICES+1;
		int sum=0;

		for(int i=1; i<=number_of_cover_components; i++)
		{
			sum += (int) clique_cover_components[i].size();

			if( (int) clique_cover_components[i].size() > max_clique_size)
			{
				if(clique_cover_components[i].size() > 1) max_clique_size = clique_cover_components[i].size();
			}
			if( (int) clique_cover_components[i].size() < min_clique_size)
			{
				if(clique_cover_components[i].size() > 1) min_clique_size = clique_cover_components[i].size();
			}

			num_cliques_with_size[clique_cover_components[i].size()]++;
		}

		float average = ((float)sum) / number_of_cover_components;

		for(int i=2;i<MAX_VERTICES;i++)
		{
			if(num_cliques_with_size[i])
			{
				printf("Number of cliques with size %d = %d\n",i,num_cliques_with_size[i]);
			}
		}
		printf("Largest clique = %d\n",max_clique_size);
		printf("Smallest clique= %d\n",min_clique_size);
		printf("Average clique size = %.02f\n",average);
		printf("---------------------\n");
	}
}


void readGraph()
{
	char input_char_read;
	int vertex_1;
	int vertex_2;

	while(true)
	{
		scanf(" %c",&input_char_read);

		if(input_char_read != 'c')
		{
			break;
		}
		else
		{
			scanf("%*[^\n]");
		}
	}

	scanf(" %*s %d %d",&NUM_VERTICES,&NUM_EDGES);

	for(int i=0;i<NUM_EDGES;i++)
	{
		scanf(" e %d %d ",&vertex_1, &vertex_2);
		adjacency_list[vertex_1].push_back(vertex_2);
		adjacency_list[vertex_2].push_back(vertex_1);
		
	}	
}

int main(int argn, char* argc[])
{
	CPUTimer timer_total;
	int num_runs;

	readGraph();

	num_runs = 0;
	printf("========================================================\n");
	printf("Running Find Clique Algorithm...\n");
	printf("========================================================\n");
	do
	{
		num_runs++;
		timer_total.start();
		runFindClique(false);
		timer_total.stop();
	}
	while(timer_total.getCPUTotalSecs() < 5.0f);
	timer_total.start();
	runFindClique(true);
	timer_total.stop();
	printf("The 'find clique' algorithm ran %d times in %.02f seconds (average: %f seconds/run)\n",num_runs,
																								timer_total.getCPUTotalSecs(),
																								timer_total.getCPUTotalSecs()/num_runs);

	timer_total.reset();

	num_runs = 0;
	printf("========================================================\n");
	printf("Running Find Cover Algorithm...\n");
	printf("========================================================\n");
	do
	{
		num_runs++;
		timer_total.start();
		runFindCover(false);
		timer_total.stop();
	}
	while(timer_total.getCPUTotalSecs() < 5.0f);
	timer_total.start();
	runFindCover(true);
	timer_total.stop();
	printf("The 'find cover' algorithm ran %d times in %.02f seconds (average: %f seconds/run)\n",num_runs,
																								timer_total.getCPUTotalSecs(),
																								timer_total.getCPUTotalSecs()/num_runs);

	timer_total.reset();

	num_runs = 0;
	printf("========================================================\n");
	printf("Running Coloring Algorithm...\n");
	printf("========================================================\n");
	do
	{
		num_runs++;
		timer_total.start();
		runColoring(false);
		timer_total.stop();
	}
	while(timer_total.getCPUTotalSecs() < 5.0f);
	timer_total.start();
	runColoring(true);
	timer_total.stop();
	printf("The 'coloring' algorithm ran %d times in %.02f seconds (average: %f seconds/run)\n",num_runs,
																								timer_total.getCPUTotalSecs(),
																								timer_total.getCPUTotalSecs()/num_runs);
	printf("========================================================\n");

	return 0;
}
