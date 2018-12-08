#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN (1000)
#define SIZE (128)
#define KW_SIZE (100)
#define TXT_SIZE (5000)

typedef struct keyword {
  char at[KW_SIZE];
} keyword;

typedef struct trie {
  int is_word;
  int kw_idx;
  struct trie *ch[SIZE];
  struct trie *fail;
} node;

int n;
keyword kws_list[MAXN + 1];
node root_ins = {.is_word = 0, .kw_idx = -1, .ch = {NULL}, .fail = NULL};

int comparator(const void *a, const void *b) {
  keyword *ia = (keyword *)a;
  keyword *ib = (keyword *)b;
  return (strcmp(ia->at, ib->at));
}

void init_node(node *node_ptr) {
  int i;
  node_ptr->is_word = 0;
  node_ptr->kw_idx = -1;
  for (i = 0; i < SIZE; i++)
    node_ptr->ch[i] = NULL;
  node_ptr->fail = NULL;
}

void trie_insert(node *root, char *word, int kw_idx) {
  int i;
  node *p = root;
  for (i = 0; word[i]; i++) {
    char cur = word[i];
    if (!p->ch[cur]) {
      node *new_node = (node *)malloc(sizeof(node));
      init_node(new_node);
      p->ch[cur] = new_node;
    }
    p = p->ch[cur];
  }
  p->is_word = 1;
  p->kw_idx = kw_idx;
}

void fail_gen(node *root) {
  int h = 0, t = 1;
  node *q[MAXN * KW_SIZE];
  q[h] = root;
  while (h < t) {
    int i;
    node *cur = q[h++];
    for (i = 0; i < SIZE; i++) {
      if (!cur->ch[i]) continue;
      if (cur == root) cur->ch[i]->fail = root;
      else {
        node *p;
        for (p = cur->fail; p; p = p->fail) {
          if (p->ch[i]) {
            cur->ch[i]->fail = p->ch[i];
            break;
          }
        }
        if (!p) cur->ch[i]->fail = root;
      }
      q[t++] = cur->ch[i];
    }
  }
}

keyword *ac_match(int line_no, node *root, char *s, int *keyword_num) {
  int i, kw_num = 0;
  keyword *kws = (keyword *)malloc(sizeof(keyword) * TXT_SIZE);
  node *p = root;
  for (i = 0; s[i]; i++) {
    while (!p->ch[s[i]] && p != root) p = p->fail;
    if (!p->ch[s[i]]) continue;
    if (p->ch[s[i]]->is_word) {
      kws[kw_num++] = kws_list[p->ch[s[i]]->kw_idx];
      p = root; continue;
    }
    p = p->ch[s[i]];
  }
  qsort(kws, kw_num, sizeof(keyword), comparator);
  *keyword_num = kw_num;
  return kws;
}

void init() {
  int i;
  scanf("%d\n", &n);
  for (i = 0; i < n; i++) {
    scanf("%s\n", kws_list[i].at);
    trie_insert(&root_ins, kws_list[i].at, i);
  }
  fail_gen(&root_ins);
}

void solve() {
  int line_no = 0;
  char text[TXT_SIZE];
  while (fgets(text, TXT_SIZE, stdin) != NULL) {
    line_no++;
    int keyword_num, i;
    keyword *kws = ac_match(line_no, &root_ins, text, &keyword_num);
    for (i = 0; i < keyword_num; i++)
      printf("(%d, %s)\n", line_no, kws[i].at);
    free(kws);
  }
}

int main(void) {

  init();
  solve();

  return 0;
}
