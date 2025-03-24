#include <iostream>
#include <utility> // std::pair
#include <stack>

using namespace std;

// 출력 수정하기
// 노드가 삭제되는 경우 1. 합병할때 오른쪽 노드 삭제, 2. 부모에게 물려받고 부모 삭제되는 경우 .

typedef struct __BTREENODE__{
    int n; // 한 노드 내의 키 값의 수
    struct __BTREENODE__** p; // 서브트리에 대한 포인터의 배열
    int* k; // 키값 배열
}TREENODE;

TREENODE* getNodeBT();

pair<bool,stack<TREENODE*>> searchPath(TREENODE* root, int key);

bool insertBT(TREENODE** root, int m, int newKey);
bool deleteBT(TREENODE** root, int m, int deleteKey);

bool insertKey(TREENODE** root, int m, TREENODE* x, TREENODE* y, int newKey);
bool deleteKey(TREENODE** root, int m, TREENODE* x, int oldKey);

void inorder(TREENODE* node);
void clearTree(TREENODE* node);

bool isLeaf(TREENODE* node);
bool isRoot(TREENODE* node, TREENODE* root);

int bestSibling(TREENODE* root, int m, TREENODE* x, TREENODE* y);
void redistributeKey(TREENODE** root, int m, TREENODE* x, TREENODE* y, int bestSiblingIndex);
void mergeNode(TREENODE** root, int m, TREENODE* x, TREENODE* y, int bestSiblingIndex);

TREENODE* getNodeBT(){
    TREENODE* newNode = (TREENODE*)calloc(1,sizeof(TREENODE));
    newNode->n = 0;
    newNode->k = (int*)calloc(100,sizeof(int));
    newNode->p = (TREENODE**)calloc(100,sizeof(TREENODE*));

    return newNode;
}

// x-> 삽입 해야할 노드
bool insertKey(TREENODE** root, int m, TREENODE* x, TREENODE* y, int newKey){

    // newKey보다 큰 키들을 오른쪽으로 한칸 씩 이동 ..
    int i = (x->n)-1;
    while(i>=0 && newKey < x->k[i]){
        x->k[i+1] = x->k[i];
        x->p[i+2] = x->p[i+1]; // 포인터들도 하나씩 밀어줌 .
        i -= 1; // i를 빼면서 밀어서 자리 만들기.
    }

    // newKey를 자리에 삽입 ..
    x->k[i+1] = newKey;
    x->p[i+2] = y; // y-> 새로운 키와 연결된 자식 노드 ..
    x->n++; // x노드 키값의 수 1증가
    
    return true;
}

pair<bool, stack<TREENODE*>> searchPath(TREENODE* root, int key){
    stack<TREENODE*> st;
    
    TREENODE* x = root;
    int i=0;

    while(x != NULL){
        i=0;

        while(i < x->n && key > x->k[i]){
            i++;
        }
        
        st.push(x);

        //////////////////////////////////////
        // for(int cnt_i=0;cnt_i<x->n;cnt_i++){
        //     cout << x->k[cnt_i] << " ";
        // }
        // cout << "\n";
        //////////////////////////////////////

        if(i < x->n && key == x->k[i]){
            return {true, st};
        }

        x = x->p[i]; // 못찾았으면 자식으로 내려감
    }
    // 자식이 NULL이면 탐색 종료 .. 찾았는데 키값이 없으면 NULL임.
    return {false, st};
}

// x -> 오버플로우 발생한 노드, y-> 옮기려고 새로 만든 노드 ..
pair<int, TREENODE*> splitNode(TREENODE** root, int m, TREENODE* x, TREENODE* y, int newKey){
    insertKey(root, m, x, y, newKey);
    TREENODE tempNode = *x; // 구조체 복사.

    int centerKey = 0;
    int index=(x->n)/2; // 중앙값을 찾기위해 index를 정의함 .
    
    centerKey = x->k[index];

    x->n = 0; // 일단 n을 0으로 만들고 .
    int i=0;
    while(tempNode.k[i] < centerKey){
        x->k[i] = tempNode.k[i];
        x->p[i] = tempNode.p[i];
        i += 1;
        x->n += 1;
    }

    x->p[i] = tempNode.p[i];

    TREENODE* newNode = getNodeBT();
    i += 1; // i는 center 키를 가리키고 있는데, i+1하면 분할된 노드를 가리키게됨.
    
    while(i < tempNode.n){
        newNode->k[newNode->n] = tempNode.k[i]; // 우리는 인덱스를 0부터 시작함.
        newNode->p[newNode->n] = tempNode.p[i]; // newNode->n 을 갱신하면서 인덱스로 사용함 .ㄴ
        
        i += 1;
        newNode->n += 1; // 
    }

    newNode->p[newNode->n] = tempNode.p[i]; // 마지막 끝에 포인터는 노드의 n보다 1 크다. 인덱스 0부터 시작

    return {centerKey, newNode}; // centerKey를 반환함 .
}

bool insertBT(TREENODE** root, int m, int newKey){
    // 트리가 비어 있는 경우 ..
    if(*root == NULL){
        *root = getNodeBT();
        (*root)->k[0] = newKey;
        (*root)->n = 1;
        return true;
    }

    // newKey를 삽입할 노드를 탐색함 .
    
    stack<TREENODE*> st;
    bool found = false;

    // searchPath()
    TREENODE* temp = *root;
    int i=0;
    do{
        i=0;

        while(i < temp->n && newKey > temp->k[i]){
            i++;
        }

        st.push(temp);

        if(i < temp->n && newKey == temp->k[i]){
            found = true;
            break;
        }

    }while((temp = temp->p[i]) != NULL);

    if(found == true) return false; //if(found == true) then return;

    // newKey가 없으므로 T에 삽입 .. 경우의 수는 두가지
    bool finished = false;
    TREENODE* x = st.top(); st.pop();
    TREENODE* y = NULL; // 오버 플로우로 인한 노드 분할로 새로 생성되는 노드 ..

    do{
        if(x->n < m-1){ // 노드에 2개 있으면 삽입이 가능함.
            // overflow 아닌 경우 (노드가 가질수있는 최대 키 개수는 m-1개이다.)
            insertKey(root,m,x,y,newKey); // y-> 새로운 키와 연결된 자식노드 ..
            finished = true;
        }
        else{
            // overflow 발생.
            // x를 newKey기준으로 분할함 . 분할된 노드와 새로운 newKey 반환.
            pair<int, TREENODE*> result = splitNode(root,m,x,y,newKey);
            // x는 포인터를 통해 직접 바꾸고, y는 반환값으로 받음 .
            newKey = result.first; // 첫번째 반환값: 새로운 중앙값.
            y = result.second; // 두번째 반환값: 오른쪽 노드

            if(!st.empty()){
                x = st.top(); st.pop(); // 부모노드가 있는 경우 부모 노드를 확인 .
                // result.first -> 중앙값 ..
                // 부모노드를 x로 하고 다시 루프를 돈다 .
                // y = result.second; y가 필요 없음 ..
            }
            else{ // stack이 empty인 경우 -> 루트노드인 경우
                *root = getNodeBT(); // 
                (*root)->k[0] = newKey;
                (*root)->p[0] = x; // 왼쪽 자식은 x
                (*root)->p[1] = y; // 오른쪽 자식은 y
                (*root)->n = 1;
                finished = true;
            }
        }
    }while(!finished);

    return true;
}

// 키 재분배가 불가능한 경우 .. -> 좌/우 형제 노드 모두 최솟값인 경우 ..
// 좌/우 형제노드 모두 underflow일때 . 왼쪽 노드와 합병함 !.
// 부모에게 받은 다음에 형제와 합병한다 !.
// 합병을 해야 .. 루트노드가 하나일 수 있구나 .

bool isLeaf(TREENODE* node){
    for(int cnt_i=0;cnt_i<=node->n;cnt_i++){
        if(node->p[cnt_i] != NULL){
            return false; // 자식이 하나라도 있으면 리프가 아님 .
        }
    }

    return true;
}

bool isRoot(TREENODE* node, TREENODE* root){
    return node == root;
}

bool deleteKey(TREENODE** root, int m, TREENODE* x, int oldKey){
    int i=0;
    while(oldKey > x->k[i]){
        i += 1;
    }

    while(i<x->n){
        x->k[i] = x->k[i+1];
        x->p[i+1] = x->p[i+2];
        i += 1;
    }
    x->n -= 1;

    return true;
}

int bestSibling(TREENODE* root, int m, TREENODE* x, TREENODE* y){
    // y에서 x의 위치 i를 탐색함 .
    int bestSiblingIndex = 0;
    int i=0;
    while(y->p[i] != x){
        i += 1;
    }

    if(i == 0){ // 우측 형제 밖에 없는 경우
        bestSiblingIndex = i+1;
    }
    else if(i == y->n){ // 죄측 형제 밖에 없는 경우 ..
        bestSiblingIndex = i-1; // 좌측이 bestSiblingIndex..
    }
    else if(y->p[i-1]->n >= y->p[i+1]->n){ // 둘이 같은 경우 왼쪽이 bestSiblingIndex임
        bestSiblingIndex = i-1;
    }
    else{
        bestSiblingIndex = i+1;
    }

    return bestSiblingIndex;
}

// 노드 y는 x의 부모노드 !.
void redistributeKey(TREENODE** root, int m, TREENODE* x, TREENODE* y, int bestSiblingIndex){
    // 키를 재분배 하는 함수
    int i=0;
    // y에서 x의 위치를 탐색함 .
    while(y->p[i] != x){
        i += 1;
    }
    
    // 형제 노드에서 가져와서 부모로 옮기고 부모의 키 값을 내린다. 
    TREENODE* bestNode = y->p[bestSiblingIndex];
    if(bestSiblingIndex < i){ // 좌측 노드인 경우(bestSiblingIndex가 i-1인 경우)
        int lastKey = bestNode->k[bestNode->n-1]; // 좌측 노드에서 가장 큰 값을 가져옴.
        insertKey(root, m, x, NULL, y->k[i-1]); // 부모의 키를 x에 삽입한다 .
        
        // 근데 리프노드인데 이거 왜해주는거지 ?
        x->p[1] = x->p[0]; // x의 자식 포인터 재조정 ..
        x->p[0] = bestNode->p[bestNode->n]; // 형제 노드의 마지막 포인터를 가져옴 ..
        bestNode->p[bestNode->n] = NULL; // 형제 노드의 마지막 포인터를 삭제해줌 ..
        deleteKey(root, m, bestNode, lastKey); // bestNode에서 가져온거 삭제해줌 ..
        y->k[i-1] = lastKey; // 부모로 올려줌 .
    }
    else{ // 우측 노드인 경우 (bestSiblingIndex가 i+1인 경우)
        int firstKey = bestNode->k[0];
        insertKey(root, m, x, NULL, y->k[i]); // 부모노드를 x로 내려줌 .
        x->p[x->n] = bestNode->p[0];
        bestNode->p[0] = bestNode->p[1];
        deleteKey(root, m, bestNode, firstKey);
        y->k[i] = firstKey; // 부모 노드에는 .. 오른쪽 자식의 가장 왼쪽 값을 올려줌 .
    }
}

void swapIndex(int* a, int* b){
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void swapNode(TREENODE* a, TREENODE* b){
    TREENODE temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

// y -> 부모노드
void mergeNode(TREENODE** root, int m, TREENODE* x, TREENODE* y, int bestSiblingIndex){
    int i=0;
    
    // y에서 x를 가리키는 포인터의 위치를 찾음 .
    while(y->p[i] != x){
        i += 1;
    }

    TREENODE* bestNode = y->p[bestSiblingIndex];

    if(bestSiblingIndex > i){ // 이 경우가 안나올거 같은데 .
        // swap
        int temp = i;
        i = bestSiblingIndex;
        bestSiblingIndex = temp;

        // swap
        TREENODE* tempNode = x;
        x = bestNode;
        bestNode = tempNode;
    }

    // bestNode는 일단 부모에게 하나 물려받음.
    bestNode->k[bestNode->n] = y->k[i-1];
    bestNode->n += 1;

    // bestNode와의 합병 .
    int j=0;

    while(j < x->n){
        bestNode->k[bestNode->n] = x->k[j];
        bestNode->p[bestNode->n] = x->p[j];
        bestNode->n += 1;
        j += 1;
    }

    bestNode->p[bestNode->n] = x->p[x->n];
    deleteKey(root, m, y, y->k[i-1]); // 부모노드에서 물려준 건 삭제 함 .. // 부모노드를 땡겨주는건 deleteKey에서 해준다 .

    // 병합된 후 오른쪽 노드는 메모리 해제해줘야함 ..
    x = NULL;
    delete x;
}

bool deleteBT(TREENODE** root, int m, int oldKey){
    bool found;
    stack<TREENODE*> st = {};

    pair<bool, stack<TREENODE*>> path = searchPath(*root, oldKey);

    found = path.first;
    st = path.second;

    if(found == false) return false; // delete하지 못함.

    TREENODE* x = st.top(); st.pop();
    TREENODE* y = NULL;

    // oldKey가 내부 노드에 있는 경우 ..
    if(!isLeaf(x)){
        TREENODE* internalNode = x;
        int i=0;

        // index of oldKey in internalNode
        while(x->k[i] != oldKey){
            i += 1;
        }

        st.push(x); // 일단 다시 집어넣어 ..
        pair<bool,stack<TREENODE*>> path = searchPath(x->p[i+1], x->k[i]); // 후행키의 위치를 탐색 ..

        bool found2 = path.first;
        stack<TREENODE*> st2 = path.second; // st2 : 후행키의 위치 탐색하면서 노드를 저장한 스택.
        stack<TREENODE*> st3;

        // 빈스택(st3)에 반환값 st2를 넣었다가 다시 꺼내면서 st에 넣어주는 식으로 구현하였습니다.
        while(!st2.empty()){
            TREENODE* cur = st2.top(); st2.pop();
            st3.push(cur);
        }

        while(!st3.empty()){
            TREENODE* cur = st3.top(); st3.pop();
            st.push(cur);
        }

        x = st.top(); st.pop();

        int temp = internalNode->k[i];
        internalNode->k[i] = x->k[0]; // 가장 왼쪽에 있는거 선택 .
        x->k[0] = temp;
    }
    bool finished = false;

    deleteKey(root,m,x,oldKey); // nodeX에서 oldKey삭제함 .

    if(!st.empty()){
        y = st.top(); st.pop();
    }

    int underFlow= (m-1)/2;

    while(!finished){
        if(*root == x || x->n >= underFlow){
            finished = true;
        }
        else{
            int bestSiblingIndex = bestSibling(*root, m, x, y);

            // 빌려줄 수 있는 경우 .
            if(y->p[bestSiblingIndex]->n > underFlow){
                redistributeKey(root, m, x, y, bestSiblingIndex);
                finished = true;
            }
            else{ // 빌려줄 수 없는 경우. 부모한테 받고 합병해야함.
                mergeNode(root,m,x,y,bestSiblingIndex);
                x = y; // y는 부모노드 ..
                
                if(!st.empty()){
                    y = st.top(); st.pop(); // stack이 비어있지 않으면 계속 진행함
                }
                else{
                    finished = true;
                }
            }
        }
    }

    // y에 키가 없는 경우 부모가 더이상 물려주지 못하는 경우(메모리 해제하는 경우)
    // 부모가 root노드이고 비어 있으면 부모 노드 삭제해주고 직전에 합쳐진 노드가 root 노드가됨 .
    if(y != NULL && y->n == 0){
        *root = y->p[0]; // 직전에 합쳐진 노드가 root 노드가 된다.
        y = NULL;   
        delete y; // 트리 높이가 줄어드는 경우 ..
    }

    return true;
}

void inorder(TREENODE* node){
    if(node == NULL || node->n == 0) return;

    cout << "<";
    for(int cnt_i=0;cnt_i<node->n;cnt_i++){
        if(node->p[cnt_i] != NULL){
            inorder(node->p[cnt_i]);
        }
        cout << " " << node->k[cnt_i];
        if(node->p[cnt_i+1] != NULL){
            cout << " ";
        }
    }
    if(node->p[node->n] != NULL){
        inorder(node->p[node->n]);
    }
    else{
        cout << " ";
    }
    cout << ">";
}

void clearTree(TREENODE* node){
    if(node == NULL) return;
    
    for(int cnt_i=0;cnt_i<=node->n;cnt_i++){
        clearTree(node->p[cnt_i]);
    }

    delete node;
    node = NULL;
}

int main(){
    char command;
    int key;

    TREENODE* root = NULL;

    while(cin >> command >> key){
        if(command == 'i'){
            if(insertBT(&root,4,int(key)) == false){
                printf("i %d: The key already exists\n", key);
                continue;
            }
        }

        else if(command == 'd'){
            if(deleteBT(&root,4,int(key)) == false){
                printf("d %d: The key does not exist\n", key);
                continue;
            }
        }

        else{
            cout << "Invalid command: " << command << "\n";
            return (1);
        }

        inorder(root);
        cout << "\n";
    }

    // 프로그램 종료 전, 메모리 누수가 발생하지 않도록 할당받은 메모리를 반드시 해제해야 합니다.
    // clearTree(root);

    return (0);
}
