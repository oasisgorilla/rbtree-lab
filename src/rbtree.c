#include "rbtree.h"
#include <stdio.h>
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  // yongjae is big man
  
    node_t *nilNode = (node_t *)calloc(1, sizeof(node_t)); // nil node 할당 및 초기화

    nilNode->color = RBTREE_BLACK; // nil node의 색상은 항상 검정으로 설정
    p->nil = nilNode;
    p->root = nilNode; // root node 초기화

  return p;
}

// void delete_rbtree(rbtree *t) {
//   if (t == NULL) {
//     return; // 트리가 이미 NULL인 경우 아무것도 하지 않음
//   }

//   node_t *nil = t->nil;
//   node_t *cur = t->root; // 현재 커서
//   node_t *prev = nil; // 현재 커서의 직전

//   while (cur != nil || prev != nil) { // 현재 노드가 nil이 아니거나, 직전 노드가 nil이 아닌 경우
//     if (cur != nil) {
//       // 왼쪽 자식으로 이동
//       if (cur->left != nil && cur->left != prev) {
//         prev = cur; // 지금 보고 있던 커서는 이전에 본 커서가 됨
//         cur = cur->left; // 현재 커서는 왼쪽 자식으로 이동
//       } else if (cur->right != nil && cur->right != prev) {
//         // 오른쪽 자식으로 이동
//         prev = cur;
//         cur = cur->right;
//       } else { // 현재 노드의 양 자식이 nill일 때
//         // 현재 노드를 처리하고 이전 노드로 이동
//         node_t *temp = cur; // 삭제를 위해 현재 노드 임시 저장
//         cur = prev; // 뒤로가기
//         prev = cur->parent; // 뒤로가기

//         // 현재 노드를 할당 해제
//         free(temp);
//         temp = NULL; //dangling pointer 방지
//       }
//     } else {
//       // 현재 노드가 nill인 경우, 이전 노드로 이동
//       cur = prev;
//       prev = cur->parent;
//     }
//   }

//   // 트리 자체를 할당 해제
//   free(t);
// }

void free_node(rbtree *t, node_t *x) {
  // 후위 순회 방식으로 RB Tree 내의 노드 메모리 반환
  if (x->left != t->nil) 
    free_node(t, x->left);
  if (x->right != t->nil)
    free_node(t, x->right);
  free(x);
  x = NULL;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t->root != t->nil)
    free_node(t, t->root);
  free(t->nil);
  free(t);
}


/////////////////////////////////////회전 함수//////////////////////////////////////
void rbtree_left_rotate(rbtree *t, node_t *datum) {
  node_t *rChild = datum->right;
  datum->right = rChild->left; // 기준점의 오른쪽 자식으로 rChild의 왼쪽 자식을 붙인다.
  if (rChild->left != t->nil) { // rChild의 왼쪽 자식이 nil이면 부모 설정이 필요 없다.
    rChild->left->parent = datum; // rChild의 왼쪽 자식의 부모 설정
  }
  rChild->parent = datum->parent; // 기준점의 부모를 rChild에 연결
  if (datum->parent == t->nil) { // 기준점이 root였을 경우
    t->root = rChild; // rChild가 root    
  } else if (datum->parent->left == datum) { // 기준점이 left 자식이었을 경우
    datum->parent->left = rChild; // 기준점 부모의 left에 rChild 연결
  } else { // 기준점이 right 자식이었을 경우
    datum->parent->right = rChild; // 기준점 부모의 right에 rChild 연결
  }

  rChild->left = datum; // rChild의 왼쪽에 기준점 연결
  datum->parent = rChild; // 기준점의 부모 설정

}

void rbtree_right_rotate(rbtree *t, node_t *datum) {
  node_t *lChild = datum->left;
  datum->left = lChild->right; // 기준점의 왼쪽 자식으로 lChild의 오른쪽 자식을 붙인다.
  if (lChild->right != t->nil) { // lChild의 오른쪽 자식이 nil이면 부모 설정이 필요 없다.
    lChild->right->parent = datum; // lChild의 왼쪽 자식의 부모 설정
  }
  lChild->parent = datum->parent; // 기준점의 부모를 lChild에 연결
  if (datum->parent == t->nil) { // 기준점이 root였을 경우
    t->root = lChild; // rChild가 root    
  } else if (datum->parent->left == datum) { // 기준점이 left 자식이었을 경우
    datum->parent->left = lChild; // 기준점 부모의 left에 lChild 연결
  } else { // 기준점이 right 자식이었을 경우
    datum->parent->right = lChild; // 기준점 부모의 right에 lChild 연결
  }

  lChild->right = datum; // lChild의 오른쪽에 기준점 연결
  datum->parent = lChild; // 기준점의 부모 설정
}

/////////////////////////////////////노드 삽입 후처리 함수/////////////////////////////////////
void rbtree_insert_fixup(rbtree *t, node_t *new) {
  
  node_t *uncle;
  while (new->parent->color == RBTREE_RED) { // 부모가 RED면 4번 특성을 맞출 때 까지 계속
    if (new->parent == new->parent->parent->left) { // 조상의 왼쪽 가지에 추가된 경우
      uncle = new->parent->parent->right; // 삼촌노드 설정
      if (uncle->color == RBTREE_RED) { // 삼촌노드가 red일 경우 (case1)
        new->parent->color = RBTREE_BLACK; // 삼촌노드, 부모노드의 색과 조상노드의 색 교환
        uncle->color = RBTREE_BLACK; // ''
        new->parent->parent->color = RBTREE_RED; // ''
        new = new->parent->parent; // 조상노드에서 다시 4번 조건 확인
      }
      else { // 삼촌노드가 black이거나 없는 경우 (case2, 3)
        if (new == new->parent->right) { // 조상까지 경로가 꺾인 경우
          new = new->parent; // 부모노드와 자식노드의 입지 교환
          rbtree_left_rotate(t, new); // 좌회전
        }
        new->parent->color = RBTREE_BLACK; // 부모와 조상의 색 교환
        new->parent->parent->color = RBTREE_RED; // ''
        rbtree_right_rotate(t, new->parent->parent);
      }
    } else { // 조상의 오른쪽 가지에 추가된 경우
      uncle = new->parent->parent->left; // 삼촌노드 설정
      if (uncle->color == RBTREE_RED) { // 삼촌노드가 red일 경우 (case1)
        new->parent->color = RBTREE_BLACK; // 삼촌노드, 부모노드의 색과 조상노드의 색 교환
        uncle->color = RBTREE_BLACK; // ''
        new->parent->parent->color = RBTREE_RED; // ''
        new = new->parent->parent; // 조상노드에서 다시 4번 조건 확인
      }
      else { // 삼촌노드가 black이거나 없는 경우 (case2, 3)
        if (new == new->parent->left) { // 조상까지 경로가 꺾인 경우
          new = new->parent; // 부모와 자식의 입지 교환
          rbtree_right_rotate(t, new); // 우회전
        }
        new->parent->color = RBTREE_BLACK; // 부모와 조상의 색 교환
        new->parent->parent->color = RBTREE_RED; // ''
        rbtree_left_rotate(t, new->parent->parent);
      }
    }
  } // while
  t->root->color = RBTREE_BLACK;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert

  node_t *curParent = t->nil; // 현재 커서의 부모
  node_t *cur = t->root; // 노드 삽입에 사용되는 커서
  node_t *new = (node_t*)calloc(1, sizeof(node_t)); // new 노드 공간 할당

  new->key = key;

  while (cur != t->nil) { // 루트가 빈 값이 아닐 경우
    curParent = cur; // 현재 커서 이동 전 부모노드로 저장
    if (new->key < cur->key) { // 새 노드의 키가 현재 노드의 키보다 작으면
      cur = cur->left; // 왼쪽 자식으로 이동
    } else {
      cur = cur->right; // 오른쪽으로 이동
    }
  } // while

  new->parent = curParent;

  if (curParent == t->nil) { // while문에 들어가지 않았으면
    t->root = new; // 신규 노드의 포인터를 root에 대입
  } else if (new->key < curParent->key) {
    curParent->left = new;
  } else {
    curParent->right = new;
  }
  // new 노드 정보 초기화
  new->color = RBTREE_RED;
  new->left = t->nil;
  new->right = t->nil;

  rbtree_insert_fixup(t, new); // 후처리 함수로 보낸다.

  return new;
}


node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  if (t->root == NULL) {
    return t->root;
  }
  node_t *cur = t->root;

  while (cur != t->nil && key != cur->key) {
    if (key < cur->key) {
      cur = cur->left;
    } else {
      cur = cur->right;
    }
  }
  if (cur == t->nil) {
    return NULL;
  }
  return cur; // 찾은 노드 return, 없으면 NULL 반환
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t->root;
  while(cur->left != t->nil){
    cur = cur->left;
  }
  return cur;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *cur = t->root;
  while(cur->right != t->nil){
    cur = cur->right;
  }
  return cur;
}
/////////////////////////////////////transplant 함수/////////////////////////////////////
void transplant(rbtree *t, node_t *u, node_t *v) { // 트리의 root인 u를 v로 교체 
  if (u->parent == t->nil) { // 피교체대상 u가 root일 때
    t->root = v; // 교체대상 v는 교체 후 해당 트리의 root가 된다.
  } else if (u == u->parent->left) { // u가 왼쪽 자식일 때
    u->parent->left = v; // v가 u.parent의 왼쪽 자식이 된다.
  } else { // u가 오른쪽 자식일 때
    u->parent->right = v; // v가 u.parent의 오른쪽 자식이 된다.
  }
  v->parent = u->parent; // v의 parent를 u의 parent로 바꾼다. 
}
/////////////////////////////////////subtree_minimum 함수/////////////////////////////////////
node_t *subtree_minimum(rbtree *t, node_t *x) { // 서브트리의 루트노드를 받아 최솟값 반환
  while (x->left != t->nil) {
    x = x->left;
  }
  return x;
}
/////////////////////////////////////노드 삭제 후처리 함수/////////////////////////////////////
void rbtree_erase_fixup(rbtree *t, node_t *x) {
  while (x != t->root && x->color == RBTREE_BLACK) {
    node_t *sibling;
    if (x == x->parent->left) {
      sibling = x->parent->right;
      if (sibling->color == RBTREE_RED) {
        sibling->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        rbtree_left_rotate(t, x->parent);
        sibling = x->parent->right;
      }
      if (sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK) {
        sibling->color = RBTREE_RED;
        x = x->parent;
      } else if (sibling->right->color == RBTREE_BLACK) {
        sibling->left->color = RBTREE_BLACK;
        sibling->color = RBTREE_RED;
        rbtree_right_rotate(t, sibling);
        sibling = x->parent->right;
      } else {
        sibling->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        sibling->right->color = RBTREE_BLACK;
        rbtree_left_rotate(t, x->parent);
        x = t->root;
      }
    } else {
      sibling = x->parent->left;
      if (sibling->color == RBTREE_RED) {
        sibling->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        rbtree_right_rotate(t, x->parent);
        sibling = x->parent->left;
      }
      if (sibling->right->color == RBTREE_BLACK && sibling->left->color == RBTREE_BLACK) {
        sibling->color = RBTREE_RED;
        x = x->parent;
      } else if (sibling->left->color == RBTREE_BLACK) {
        sibling->right->color = RBTREE_BLACK;
        sibling->color = RBTREE_RED;
        rbtree_left_rotate(t, sibling);
        sibling = x->parent->left;
      } else {
        sibling->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        sibling->left->color = RBTREE_BLACK;
        rbtree_right_rotate(t, x->parent);
        x = t->root;
      }
    }
  } // while
  x->color = RBTREE_BLACK;
}
/////////////////////////////////////노드 삭제 함수/////////////////////////////////////
int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *replacement = p; // y = z
  node_t *child; // x 선언

  color_t original_color = replacement->color; // 삭제한 노드의 색상 기억(추후 fixup필요여부 판단시 사용)

  if (p->left == t->nil) { // 삭제하려는 대상의 자식이 하나인 경우(오른쪽)
    child = p->right;
    transplant(t, p, p->right); // 함수 구현 필요
  } else if (p->right == t->nil) { // 삭제하려는 대상의 자식이 하나인 경우(왼쪽)
    child = p->left;
    transplant(t, p, p->left);
  } else { // 삭제하려는 대상의 자식이 둘인 경우
    replacement = subtree_minimum(t, p->right); // 삭제 대상의 successor 찾기(함수 구현 필요)
    original_color = replacement->color; // succossor의 색상 기억(추후 fixup필요여부 판단시 사용)
    child = replacement->right; // 바로 다음 라인, 삭제대상의 successor가 삭제대상의 right일 때 사용
    if (replacement->parent == p) { // successor가 삭제 대상의 right일 때
      child->parent = replacement; // transplant 관련 처리 필요
    } else { // successor가 삭제대상의 손자~ 일 경우
      transplant(t, replacement, replacement->right); // 대체노드를 원래 자리에서 빼내고 메꾸기
      replacement->right = p->right; // 대체노드에 삭제노드의 right연결
      replacement->right->parent = replacement; // ''
    }
    transplant(t, p, replacement); // 삭제노드를 빼내고 대체노드를 집어넣기
    replacement->left = p->left; // 대체노드에 삭제노드의 left연결
    replacement->left->parent = replacement; // ''
    replacement->color = p->color; // 삭제노드의 색 계승
  }
  if (original_color == RBTREE_BLACK) { // 삭제된 노드의 색상이 black일 경우
    rbtree_erase_fixup(t, child); // fixup 호출
  }
  free(p); // 노드 메모리 할당해제
  return 0;
}


void inorder_to_arr(const rbtree *t, node_t *n, key_t *arr, size_t *idx, size_t size) {
  if (n == t->nil){
    return;
  } else {
    inorder_to_arr(t, n->left, arr, idx, size);
    arr[(*idx)] = n->key;
    (*idx)++;
    if ((*idx) == size) {
      return;
    }
    inorder_to_arr(t, n->right, arr, idx, size);
  }

}


int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  // 중위순회하며 삽입해야 함
  if(t == NULL || arr == NULL || n == 0){
    return -1;
  }

  size_t idx = 0;
  inorder_to_arr(t, t->root, arr, &idx, n);

  return 0;
}


// 트리를 출력하는 함수
// void print_rbtree(rbtree *t, node_t *node, int space)
// {
//   if (node == t->nil)
//     return;

//   space += 10;
//   print_rbtree(t, node->right, space);

//   printf("\n");
//   for (int i = 10; i < space; i++)
//     printf(" ");
//   printf("%d(%s)\n", node->key, node->color == RBTREE_RED ? "R" : "B");

//   print_rbtree(t, node->left, space);
// }

// int main()
// {
//   rbtree *t = new_rbtree(); // 레드-블랙 트리 생성 함수
//   int key;

//   printf("노드를 삽입하려면 키 값을 입력하세요 (음수를 입력하면 종료):\n");
//   while (scanf("%d", &key) && key >= 0)
//   {
//     rbtree_insert(t, key);
//     print_rbtree(t, t->root, 0);
//   }

//   // 트리 메모리 해제
//   delete_rbtree(t);

//   return 0;
// }

// erase to array 테스트
// size_t rbtree_size_helper(node_t *node, node_t *sentinel_node) {
//     if (node == sentinel_node) // 노드가 더 이상 없으면 0 반환
//         return 0;
//     else
//         return 1 + rbtree_size_helper(node->left, sentinel_node) + rbtree_size_helper(node->right, sentinel_node);
// }

// size_t rbtree_size(const rbtree *t) {
//     // 빈 트리인 경우 크기는 0
//     if (t == NULL || t->root == t->nil)
//         return 0;
//     // 루트 노드부터 시작하여 트리의 크기 계산
//     return rbtree_size_helper(t->root, t->nil);
// }

// int main() {
//     // 새로운 레드-블랙 트리 생성
//     rbtree *t = new_rbtree();
    
//     // 트리에 임의의 값들을 삽입
//     rbtree_insert(t, 10);
//     rbtree_insert(t, 5);
//     rbtree_insert(t, 15);
//     rbtree_insert(t, 3);
//     rbtree_insert(t, 7);
//     rbtree_insert(t, 12);
//     rbtree_insert(t, 20);

//     // 중위 순회를 통해 배열로 변환
//     key_t *arr = (key_t*)malloc(rbtree_size(t) * sizeof(key_t)); // 트리의 노드 수만큼 배열 할당
//     int result = rbtree_to_array(t, arr, rbtree_size(t));
    
//     if (result == -1) {
//         printf("Error: Invalid input or empty tree.\n");
//         return 1;
//     }
    
//     // 배열 출력
//     printf("Array from inorder traversal of the red-black tree:\n");
//     for (size_t i = 0; i < rbtree_size(t); i++) {
//         printf("%d ", arr[i]);
//     }
//     printf("\n");

//     // 메모리 해제
//     free(arr);
//     delete_rbtree(t);

//     return 0;
// }