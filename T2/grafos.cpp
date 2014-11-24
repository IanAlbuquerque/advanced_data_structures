

1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
18
19
20
21
22
23
24
25
26
27
28
29
30
31
32
33
34
35
36
37
38
39
40
41
42
43
44
45
46
47
48
49
50
51
52
53
54
55
56
57
58
59
60
61
62
63
64
65
66
67
68
69
70
71
72
73
74
75
76
77
78
79
80
81
82
83
84
85
86
87
88
89
90
91
92
93
94
95
96
97
98
99
100
101
102
103
104
105
106
107
108
109
110
111
112
113
114
115
116
117
118
119
120
121
122
123
124
125
126
127
128
129
130
131
132
133
134
135
136
137
138
139
140
141
142
143
144
145
146
147
148
149
150
151
152
153
154
155
156
157
158
159
160
161
162
163
164
165
166
167
168
169
170
171
172
173
174
175
176
177
178
179
180
181
182
183
184
185
186
187
188
189
190
191
192
193
194
195
196
197
198
199
200
201
202
203
204
205
206
207
208
209
210
211
212

   /* ********************************************* */
  /*             TRABALHO 2 DE EDA                 */
 /*Ian Albuquerque, Gustavo Marques, Victor Sabino*/
/* ********************************************* */

#include <stdio.h>
#include <vector>

#define MAX_VERTICES 50

using namespace std;


vector<int> graph[MAX_VERTICES];
vector<int> click; 
int color[MAX_VERTICES]; //array that has the color of each vertex
int N; //number of current vertex

//returns the degree of the vertex
int degree(int v)
{
	return graph[v].size();
}

//finds the vertex with the biggest degree
int vMaxDegree()
{
	int max=0;
	int v_max; //value to be returned
	int size; //degree of the vertex
	//loop while there's non visited vertex
	for(int i=0;i<N;i++)
	{
		size = degree(i);
		if(size > max)
		{
 			max = size;
 			v_max = i;
		}
	}
	return v_max;
}

//looks for a edge between v and w vertex
bool isAdjacent(int v, int w)
{

	int num_arestas = degree(v);
	//while there's edges checks for the w variable
	for(int i=0;i<num_arestas;i++)
		if(w == graph[v][i])
			return true;
	
	return false;
}

//checks if v is adjacent to the click
bool isAdjacentToClick(int v)
{
	int click_size = click.size();
	//while theres vertex in click
	for(int i=0;i<click_size;i++)
		if(!isAdjacent(v,click[i]))
			return false;
	
	return true;
}

//finds if v is part of the click
bool isClick(int v)
{
	//check every vertex of the click and compare to v
	for(int i=0;i<click.size();i++)
		if(v==click[i])
			return true;
	
	return false;
}

void findClick()
{
	//clear click cache
	click.clear(); //a
	int v = vMaxDegree(); // b
	click.push_back(v);

	vector<int> v_adj;
	//size of the adjacent vertex
	int adj_size;
	//max degree of the adjacent vertex
	int adj_max_degree, max_degree;
	//loop while there's adjacents vertex
	while(1)
	{
		v_adj.clear(); // c
		for(int i=0;i<N;i++)
			//if the vertex is not in the click
			if(!isClick(i))
			{
				if(isAdjacentToClick(i))
					//add i to the v_adj if its adjacent and is not part of the click
					v_adj.push_back(i);
			}
		

		adj_size = v_adj.size();
		//end loop if tehres no v_adj
		if(!adj_size) break;

		adj_max_degree = -1;
		max_degree = 0;
		//checks for v_adj	 vertex with the biggest degree
		for(int i=0;i<adj_size;i++)
			if(degree(v_adj[i]) > max_degree)
			{
				max_degree = degree(v_adj[i]);
				adj_max_degree = v_adj[i];
			}
		
		//add to click the biggest degree v_adj with an adjacent edge
		click.push_back(adj_max_degree);
	}
}
//color the graph's vertex
void color(int v)
{
	//array of already used colors in the adjacents' vertex
	bool used_colors[MAX_VERTICES];
	int num_adj = degree(v);

	//initiate the value as false
	for(int i=0;i<MAX_VERTICES;i++)
		used_colors[i] = false;
	
	//checks for already used colors
	for(int i=0;i<num_adj;i++)
	{
		if(color[graph[v][i]] != -1)
		{
			used_colors[color[graph[v][i]]] = true;
		}
		else break;
	}
	//checks for the lowest not used color
	//eg: in this array [wite][grey][blue][][red] it would use the used_colors[3]
	for(int i=0;i<MAX_VERTICES;i++)
	{
		if(!used_colors[i])
		{
			color[v] = i;
			break;
		}
	}
	//color every adjacents' vertex if its not colord
	for(int i=0;i<num_adj;i++)
	{
		if(color[graph[v][i]] == -1)
		{
			color(graph[v][i]);
		}
	}
}
//init every color and start coloring
void initAndColor(int v)
{
	for(int i=0;i<N;i++)
		color[i] = -1;
	
	color(v);
}

int main(int argn, char* argc[])
{

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

	graph[4].push_back(1);
	graph[4].push_back(0);
	graph[4].push_back(5);

	graph[5].push_back(1);
	graph[5].push_back(0);
	graph[5].push_back(4);

	findClick();
	for(int i=0;i<click.size();i++)
		printf("%d\n",click[i]);
	

	printf("---------------------\n");

	initAndColor(0);
	for(int i=0;i<N;i++)
		printf("cor do %d = %d\n",i,color[i]);

	return 0;
}

//==============================================

/*
2. encontra cobertura minimal :
(a) Inicie com V' = V . Faça q = 1.
(b) Encontre uma clique em G(V') correspondente ao conjunto S'q .
(c) Faça V' ← V' \ S'q .
(d) Se V = ∅. Faça q ←− q + 1 e volte ao passo (b)
(e) Caso contrário, para p de 1 a q Faça:
i. Adicione a S p um vértice que seja vizinho a todos os vértices j ́a em S p dentre estes,
escolha vértice de maior grau.
ii. Se nenhum vértice puder ser adicionado a S p , Pare! S p  ́e uma clique.
iii. Faça S p = S p .
(f) O conjunto S1 , . . . , S'q  ́e uma cobertura minimal em cliques.
*/