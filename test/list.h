#ifndef LIST_H_
#define LIST_H_

struct node {
	
	
}NODE,*PNODE;

NODE * list_create(int len);
NODE * list_node(NODE * p)
NODE * list_addnode(NODE * p )
NODE * list_deletenode_p(NODE * p)
NODE * list_sort(NODE * p,void func(NODE *,NODE *))
NODE * list_destory(NODE * p)
NODE * list_display(NODE * p)
bool * list_isempty(NODE * p);
NODE * list_search(NODE * p);
#endif  //end  define LIST_H_
