#include "btree.h"
#include <stdlib.h>
#include <stdio.h>

#define N 3 /* Ordem (precisa ser impar) */
struct btree {
  int n; /* Numero de chaves */
  int k[N];
  BTree *p[N+1];
};


static int isleaf (BTree* a) {
  return (a->p[0] == NULL);
}

static int findpos (BTree* a, int x, int* pos) {
  for( (*pos) = 0; (*pos) < a->n; ++(*pos))
    if (x == a->k[*pos])
      return 1;
    else if (x < a->k[*pos])
      break ;
  return 0;
}


BTree* bt_search(BTree* a, int x, int* pos) {
  int found = findpos(a,x,pos);
  if (found)
    return a;
  else if (isleaf(a))
    return NULL;
  else
    return bt_search(a->p[*pos], x, pos);
}

int bt_key (BTree* a, int pos) {
  return a->k[pos];
}

BTree* bt_create (void) {
  BTree* a = (BTree*) malloc(sizeof(BTree));
  a->n = 0;
  a->p[0] = NULL;
  return a;
}


void bt_destroy (BTree* a) {
  int i;
  if (!isleaf(a)) {
    for (i = 0; i <= a->n; ++i)
    bt_destroy(a->p[i]);
  }
  free(a);
}


static int overflow (BTree* a) {
  return (a->n == N);
}

static BTree* split (BTree* a, int* m) {
  int i;
  BTree* b = bt_create();

  int q = a->n/2;
  b->n = a->n - q - 1;
  a->n = q;
  *m = a->k[q];
  b->p[0] = a->p[q+1];

  for (i=0; i < b->n; ++i) {
    b -> k[i] = a->k[q + 1 + i] ;
    b->p[i+1] = a->p[q+1+i+1];
  }
  return b;
}


static void addright (BTree* a , int pos, int k, BTree* p) {
  int j;
  for (j = a->n; j>pos; --j) {
    a->k[j] = a->k[j-1];
    a -> p [j + 1] = a->p[j] ;
  }
  a->k[pos] = k;
  a->p[pos+1] = p;
  a->n++;
}


static void insert (BTree* a, int x) {
  int pos;
  findpos(a,x,&pos);
  /* insert even if already exists */
  if (isleaf(a)) {
    addright(a,pos,x,NULL);
  }
  else {
    insert( a->p[pos], x );
    if (overflow(a->p[pos])) {
      int m;
      BTree* b = split(a->p[pos],&m);
      addright(a,pos,m,b);
    }
  }
}


BTree* bt_insert (BTree* a, int x) {

  insert(a,x);
  if(overflow(a)) {
    int m;
    BTree* b = split(a,&m);
    BTree* r = bt_create(); r->k[0] = m;
    r->p[0] = a;
    r->p[1] = b;
    r->n = 1;
    return r;
  }
  return a;
}






#define INDENT(x) for (int j=0; j<x; ++j) printf(" ");
void bt_print (BTree* a, int indent)
{
  int i;
  if (isleaf(a)) {
    for (i = a->n - 1; i >= 0; --i) {
      INDENT(indent);
      printf( "%d \n" , a->k[i]);
    }
  }
  else {
    bt_print(a->p[a->n],indent+2);
    for (i = a->n - 1; i>=0; --i) {
      INDENT(indent);
      printf("%d \n" , a->k[i] ) ;
      bt_print(a->p[i],indent+2);
    }
  }
}
