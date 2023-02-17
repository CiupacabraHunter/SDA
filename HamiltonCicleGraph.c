#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define OBIECTIVE_MAXIME 20 //numarul de obiective maxime din graf
#define MAX_NAME_LENGTH 20 //lungimea maxima a numelui unui obiectiv

//muchia dintre doua noduri
typedef struct Edge {
  int node1;//nodul de start
  int node2;//nodul de sfarsit
  int weight;//inaltimea(distanta dintre noduri)
}Edge;

typedef struct Graph {
  char obiective[OBIECTIVE_MAXIME][MAX_NAME_LENGTH];
  int nrObiective;//numarul de obiective (noduri)
  int nrEdges;//numarul de muchii
  Edge edges[OBIECTIVE_MAXIME * OBIECTIVE_MAXIME];//vectorul de muchii de tip structura
}Graph;

//cautam obiectivele si returnam indexul acestora
int cautaObiectiv(Graph *graph, char *obiectiv) {
  //Parcurgem vectorul de obiective
  for (int i = 0; i < graph->nrObiective; i++) {
    // Verificam daca obiectivul cautat se gaseste in graf
    if (strcmp(graph->obiective[i], obiectiv) == 0) {
      // Daca da, returnam pozitia (indexul) obiectivului in graf
      return i;
    }
  }
  // Daca nu gasim obiectivul, il adaugam in graf si returnam pozitia (indexul) acestuia
  strcpy(graph->obiective[graph->nrObiective], obiectiv);
  return graph->nrObiective++;
}

//citim din fisier
// Fiecare nod este adăugat în graf sau găsit prin funcția cautaObiectiv.
void citesteDinFisier(Graph *graph, char *filename) {
  FILE *file = fopen(filename, "r");

  // Verificare daca fisierul a putut fi deschis
  if (file == NULL) {
    printf("Failed to open file\n");
    return;
  }

  // Initializarea numarului de obiective si muchii
  graph->nrObiective = 0;
  graph->nrEdges = 0;

  char obiectiv1[MAX_NAME_LENGTH];
  char obiectiv2[MAX_NAME_LENGTH];
  int weight;

  //citim datele din fisier pana la finalul acestuia si le atribuim corespunzator
  while (fscanf(file, "%s %s %d", obiectiv1, obiectiv2, &weight) == 3) {
    int obiectiv1_index = cautaObiectiv(graph, obiectiv1);
    int obiectiv2_index = cautaObiectiv(graph, obiectiv2);

    graph->edges[graph->nrEdges].node1 = obiectiv1_index;
    graph->edges[graph->nrEdges].node2 = obiectiv2_index;
    graph->edges[graph->nrEdges].weight = weight;
    graph->nrEdges++;
  }

  fclose(file);
}

//afisam continutul grafului
void printGraph(Graph *graph) {
  for (int i = 0; i < graph->nrEdges; i++) {
    printf("%s  %s  %d\n",
           graph->obiective[graph->edges[i].node1],
           graph->obiective[graph->edges[i].node2],
           graph->edges[i].weight);
  }
}

//parcurgere in adancime a grafului
void DFS(Graph *graph, int current_node, bool visited[], int path[], int path_index, int *min_length, int current_length) {
  visited[current_node] = true;
  path[path_index] = current_node;
  path_index++;

  // parcurge graful, marcând nodurile vizitate, adăugând nodurile la traseu și calculând lungimea traseului curent
  if (path_index == graph->nrObiective) {
    // Am vizitat toate nodurile, verific dacă traseul are întoarcere în punctul de plecare
    for (int i = 0; i < graph->nrEdges; i++) {
      if (graph->edges[i].node1 == path[path_index - 1] && graph->edges[i].node2 == path[0]) {
        if (current_length + graph->edges[i].weight < *min_length) {
          *min_length = current_length + graph->edges[i].weight;
        }
      }
    }
  } else {
    // Continui parcurgerea
    for (int i = 0; i < graph->nrEdges; i++) {
      if (graph->edges[i].node1 == current_node) {
        int next_node = graph->edges[i].node2;
        if (!visited[next_node]) {
          DFS(graph, next_node, visited, path, path_index, min_length, current_length + graph->edges[i].weight);
        }
      }
    }
  }

  // Setam valorile initiale inapoi
  path_index--;
  visited[current_node] = false;
}

int main(void) {

  Graph graph;

  citesteDinFisier(&graph, "data.txt");//citim din fisier
  printGraph(&graph);//afisam din fisier

  int min_length = 1000000;//lungimea minima a drumului
  int path[OBIECTIVE_MAXIME];//drumul

  bool visited[OBIECTIVE_MAXIME];//aici o sa retinem daca un nod(obiectiv) a fost viztat

  //memset practic copiaza "0" la primele n caractere din sriu la care este trimis la care este
  memset(visited, 0, sizeof(visited));
  //sizeof(visited) reprezinta numarul de biti pe care il setam la valoarea 0
  //functia returneaza in pointer la locatia din memorie a lui visited


  for (int i = 0; i < graph.nrObiective; i++) {
    DFS(&graph, i, visited, path, 0, &min_length, 0);
  }

  printf("\nLungimea minima care sa includa toate obiectivele(nodurile) si sa se intoarca la inceput este: %d\n", min_length);

  return 0;

}
