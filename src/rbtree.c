#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  // yongjae is big man
  
  if (p != NULL) {
    p->root = NULL; // root node 초기화

    p->nil = NULL; // nill node 초기화
  }
  return p;
}

void delete_rbtree(rbtree *t) {
  if (t == NULL) {
    return; // 트리가 이미 NULL인 경우 아무것도 하지 않음
  }

  node_t *nil = t->nil;
  node_t *cur = t->root; // 현재 커서
  node_t *prev = nil; // 현재 커서의 직전

  while (cur != nil || prev != nil) { // 현재 노드가 nil이 아니거나, 직전 노드가 nil이 아닌 경우
    if (cur != nil) {
      // 왼쪽 자식으로 이동
      if (cur->left != nil && cur->left != prev) {
        prev = cur; // 지금 보고 있던 커서는 이전에 본 커서가 됨
        cur = cur->left; // 현재 커서는 왼쪽 자식으로 이동
      } else if (cur->right != nil && cur->right != prev) {
        // 오른쪽 자식으로 이동
        prev = cur;
        cur = cur->right;
      } else { // 현재 노드의 양 자식이 nill일 때
        // 현재 노드를 처리하고 이전 노드로 이동
        node_t *temp = cur; // 삭제를 위해 현재 노드 임시 저장
        cur = prev; // 뒤로가기
        prev = cur->parent; // 뒤로가기

        // 현재 노드를 할당 해제
        free(temp);
        temp = NULL; //dangling pointer 방지
      }
    } else {
      // 현재 노드가 nill인 경우, 이전 노드로 이동
      cur = prev;
      prev = cur->parent;
    }
  }

  // 트리 자체를 할당 해제
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  // 새로 삽입한 노드의 메모리 할당
  node_t *new = (node_t*)malloc(sizeof(node_t)); 
  node_t *cur = t->root;

  if (cur == NULL) { // 빈 트리의 경우, root에 추가
    t->root = new;
    new->color = RBTREE_BLACK;
    new->key = key;
    new->left = NULL;
    new->parent = NULL;
    new->right = NULL;
  } else if (cur->key < key) {
    while (cur->right != NULL && cur->right->key < key) { // 더 큰 key가 오른쪽 자식으로 나올 때까지 이동
      cur = cur->right;
    }

    cur = cur->right; // 처음 만난 더 큰 오른쪽 자식노드로 이동

    while (cur->left != NULL && cur->left->key > key) { // 더 작은 key가 왼쪽 자식으로 나올 때까지 이동
      cur = cur->left;
    }

    cur = cur->left; // 처음 마주친 더 작은 왼쪽 자식노드로 이동
    
  }

  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  return 0;
}
