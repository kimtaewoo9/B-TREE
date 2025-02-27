#include <iostream>
#include <utility> // std::pair

using namespace std;

typedef struct _BTREENODE_{
    int n; // 노드 내 키 값의 개수
    int *data; // 키 값 배열
    struct _BTREENODE_** childPtr; // 서브 트리에 대한 포인터들의 배열
}TREENODE;

pair<bool,stack<TREENODE*>> searchPath(TREENODE* root, int m, int key, stack<TREENODE*> stack){

}

int main(){
    
}