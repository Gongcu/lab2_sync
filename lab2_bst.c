/*
*	Operating System Lab
*	    Lab2 (Synchronization)
*	    Student id : 
*	    Student name : 
*
*   lab2_bst.c :
*       - thread-safe bst code.
*       - coarse-grained, fine-grained lock code
*
*   Implement thread-safe bst for coarse-grained version and fine-grained version.
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>
#include <string.h>

#include "lab2_sync_types.h"
pthread_mutex_t SUB_MUTEX = PTHREAD_MUTEX_INITIALIZER;
/*
 * TODO
 *  Implement funtction which traverse BST in in-order
 *  
 *  @param lab2_tree *tree  : bst to print in-order. 
 *  @return                 : status (success or fail)
 */
int lab2_node_print_inorder(lab2_tree *tree)
{
    // You need to implement lab2_node_print_inorder function.
    if (!tree)
    {
        return LAB2_ERROR;
    }
    lab2_inorder_driven(tree->root);
    return LAB2_SUCCESS;
}

void lab2_inorder_driven(lab2_node *node)
{
    if (node)
    {
        lab2_inorder_driven(node->left);
        printf("%d ", node->key);
        lab2_inorder_driven(node->right);
    }
}

/*
 * TODO
 *  Implement function which creates struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_tree )
 * 
 *  @return                 : bst which you created in this function.
 */
lab2_tree *lab2_tree_create()
{
    // You need to implement lab2_tree_create function.
    lab2_tree *tree = (lab2_tree *)malloc(sizeof(lab2_tree));
    tree->root = NULL;
    if (pthread_mutex_init(&(tree->mutex), NULL) != LAB2_SUCCESS)
        assert(0);
    return tree;
}

/*
 * TODO
 *  Implement function which creates struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param int key          : bst node's key to creates
 *  @return                 : bst node which you created in this function.
 */
lab2_node *lab2_node_create(int key)
{
    // You need to implement lab2_node_create function.
    lab2_node *new = (lab2_node *)malloc(sizeof(lab2_node));
    new->key = key;
    new->parent =new->left = new->right = NULL;
    pthread_mutex_init(&(new->mutex), NULL);

    return new;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST. 
 *  
 *  @param lab2_tree *tree      : bst which you need to insert new node.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                 : satus (success or fail)
 */
int lab2_node_insert(lab2_tree *tree, lab2_node *new_node)
{
    // You need to implement lab2_node_insert function.
    lab2_node *temp = tree->root;
    lab2_node *parent_node = NULL;
    if (!temp)
    {
        tree->root = new_node;
        return LAB2_SUCCESS;
    }
    while (temp)
    {
        parent_node = temp;
        if (temp->key == new_node->key)
            return LAB2_ERROR;
        if (temp->key > new_node->key){
            temp = temp->left;
        }
        else{
            temp = temp->right;
        }
    }

    if (parent_node->key > new_node->key){
        parent_node->left = new_node;
        new_node->parent = parent_node;
    }
    else{
        parent_node->right = new_node;
        new_node->parent=parent_node;
    }
    return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in fine-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in fine-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_fg(lab2_tree *tree, lab2_node *new_node)
{
    // You need to implement lab2_node_insert_fg function.
     lab2_node *temp = tree->root;
    lab2_node *parent_node = NULL;
    if (!temp)
    {
        tree->root = new_node;
        return LAB2_SUCCESS;
    }
    while (temp)
    {
        parent_node = temp;
        if (temp->key == new_node->key)
            return LAB2_ERROR;
        if (temp->key > new_node->key){
            temp = temp->left;
        }
        else{
            temp = temp->right;
        }
    }

    if (parent_node->key > new_node->key){
        parent_node->left = new_node;
        new_node->parent = parent_node;
    }
    else{
        parent_node->right = new_node;
        new_node->parent=parent_node;
    }
    return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which insert nodes from the BST in coarse-garined manner.
 *
 *  @param lab2_tree *tree      : bst which you need to insert new node in coarse-grained manner.
 *  @param lab2_node *new_node  : bst node which you need to insert. 
 *  @return                     : status (success or fail)
 */
int lab2_node_insert_cg(lab2_tree *tree, lab2_node *new_node)
{
    // You need to implement lab2_node_insert_cg function.
    // You need to implement lab2_node_insert function.
     lab2_node *temp = tree->root;
    lab2_node *parent_node = NULL;
    if (!temp)
    {
        tree->root = new_node;
        return LAB2_SUCCESS;
    }
    while (temp)
    {
        parent_node = temp;
        if (temp->key == new_node->key)
            return LAB2_ERROR;
        if (temp->key > new_node->key){
            temp = temp->left;
        }
        else{
            temp = temp->right;
        }
    }

    if (parent_node->key > new_node->key){
        parent_node->left = new_node;
        new_node->parent = parent_node;
    }
    else{
        parent_node->right = new_node;
        new_node->parent=parent_node;
    }
    return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove(lab2_tree *tree, int key)
{
    // You need to implement lab2_node_remove_fg function.
    int found = 0;
    int status = LAB2_SUCCESS;
    if (!tree->root)
    {
        status = LAB2_NULL_TREE;
    }
    if (status != LAB2_NULL_TREE)
    {
        // You need to implement lab2_node_remove function.
        lab2_node *p;        //to be deleted node
        lab2_node *q = NULL; //deleted node' parent
        p = tree->root;
        while (p)
        {
            if (p->key == key)
            {
                found = 1;
                break;
            }
            q = p;
            if (p->key > key)
                p = p->left;
            else
                p = p->right;
        }

        if (!found)
            goto UNLOCK;

        if ((p->left) && (p->right))
        { //two child
            lab2_node *min = p->right, *min_parent = p;
            while (min->left)
            {
                min_parent = min;
                min = min->left;
            }
            if (min_parent->left == min)
            {
                min_parent->left = min->right;
            }
            else
            {
                min_parent->right = min->right;
            }

            p->key = min->key;
            lab2_node_delete(min);
        }
        else if ((p->left == NULL) && (p->right == NULL))
        {
            if (q)
            {
                if (q->left == p)
                    q->left = NULL;
                else
                    q->right = NULL;
            }
            else
            {
                tree->root = NULL; //tree->root ==p
            }
            lab2_node_delete(p);
        }
        else
        { // one child
            if (q)
            {
                if (q->left == p) //Parent's left child is to be deleted
                {
                    if (p->left)
                        q->left = p->left;
                    else
                        q->left = p->right;
                }
                else
                { //Parent's right child is to be deleted
                    if (p->left)
                        q->right = p->left;

                    else
                        q->right = p->right;
                }
            }
            else
            { //delete root
                if (p->left)
                    tree->root = p->left;
                else if (p->right)
                    tree->root = p->right;
                else
                    tree->root = NULL;
            }
            lab2_node_delete(p);
        }
    }
UNLOCK:
    return LAB2_SUCCESS;
}

lab2_node* search(lab2_node *root, int key)
{
    lab2_node* temp=root;
    while(temp){
        if(temp->key == key)
            return temp;
        if(temp->key > key){
            temp=temp->left;
        }else{
            temp=temp->right;
        }
    }
    return NULL;
}

lab2_node* min_search(lab2_node* root){ //ildan tree lock najunge use stack mutex
    //

}
/* 
 * TODO
 *  Implement a function which remove nodes from the BST in fine-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in fine-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_fg(lab2_tree* tree, int key){
    int status;
    pthread_mutex_lock(&(tree->mutex));
    if(tree->root){
        //pthread_mutex_lock(&(tree->root->mutex));
        pthread_mutex_unlock(&(tree->mutex));
        lab2_node* delete=NULL;
        if(tree->root->key == key){
            puts("root");
            delete=tree->root;
        }
        else if(tree->root->key > key){
            puts("1");
            pthread_mutex_unlock(&(tree->root->mutex));
            if(tree->root->left){
                //pthread_mutex_lock(&(tree->root->left->mutex));
                delete = search(tree->root->left, key);
            }
            puts("dd");
        }else{
            puts("2");
            pthread_mutex_unlock(&(tree->root->mutex));
            if(tree->root->right){
                //pthread_mutex_lock(&(tree->root->right->mutex));
                delete = search(tree->root->right, key);
            }
        }
        
        puts("serach sucess");
        if(delete == NULL){
            return LAB2_ERROR; //NO SAME KEY
        }else{
            pthread_mutex_lock(&(delete->mutex));
            //delete is locked
            
            if(delete->left && delete ->right){
                puts("2child min0");
                pthread_mutex_lock(&(delete->right->mutex));
                pthread_mutex_lock(&(tree->mutex));
                lab2_node* min = delete->right;
                while(min->left){
                    min=min->left;
                }
                puts("2child min1");
                pthread_mutex_lock(&(min->parent->mutex));
                pthread_mutex_lock(&(min->mutex));
                pthread_mutex_unlock(&(tree->mutex));
                puts("2child min2");
                // delete is not deleted, just change key. min is to be deleted;
                delete->key = min->key;
                puts("2child min22222");
                if(min->parent->left==min){
                    puts("-");
                    if(min->right){
                        pthread_mutex_lock(&(min->right->mutex));
                        min->parent->left=min->right;
                        pthread_mutex_unlock(&(min->right->mutex));
                    }else{
                        min->parent->left=NULL;
                    }
                    puts("**");
                    pthread_mutex_unlock(&(min->mutex));
                    puts("***");
                    pthread_mutex_unlock(&(min->parent->mutex));
                    puts("****");
                }else{
                    puts("...");
                    int status = pthread_mutex_lock(&(min->right->mutex));
                    puts("=");
                    if(min->right){
                        pthread_mutex_lock(&(min->right->mutex));
                        min->parent->right=min->right;
                        pthread_mutex_unlock(&(min->right->mutex));
                    }else{
                        min->parent->right=NULL;
                    }
                    pthread_mutex_unlock(&(min->mutex));
                    puts("===");
                    pthread_mutex_unlock(&(min->parent->mutex));
                    puts("====");
                }
                puts("2child min3");
                pthread_mutex_unlock(&(delete->mutex));
                lab2_node_delete(min);
                puts("2child min4");
                return LAB2_SUCCESS;
            }

            if(delete->left && !(delete->right)){
                puts("1child");
                int status = pthread_mutex_lock(&(delete->parent->mutex));
                if(status==LAB2_SUCCESS){//has parent & not root
                    pthread_mutex_lock(&(delete->left->mutex));
                    delete->parent->left = delete->left;
                    pthread_mutex_unlock(&(delete->left->mutex));
                    pthread_mutex_unlock(&(delete->parent->mutex));
                    pthread_mutex_unlock(&(delete->mutex));
                    lab2_node_delete(delete);
                }else{ // no parent case
                    if(delete==tree->root){ //delete node is root node
                        pthread_mutex_lock(&(delete->left->mutex));
                        tree->root=delete->left;
                        pthread_mutex_unlock(&(delete->left->mutex));
                        pthread_mutex_unlock(&(delete->mutex));
                        lab2_node_delete(delete);
                    }else{//THE OTHERS DELETE PARENT
                        return LAB2_ERROR;
                    }
                }
                return LAB2_SUCCESS;
            }

            if(!(delete->left) && delete->right){ // ONLY RIGHT CHILD
                puts("1child");
                int status = pthread_mutex_lock(&(delete->parent->mutex));
                if(status==LAB2_SUCCESS){//has parent & not root
                    pthread_mutex_lock(&(delete->right->mutex));
                    delete->parent->right = delete->right;
                    pthread_mutex_unlock(&(delete->right->mutex));
                    pthread_mutex_unlock(&(delete->parent->mutex));
                    pthread_mutex_unlock(&(delete->mutex));
                    lab2_node_delete(delete);
                }else{ // no parent case
                    if(delete==tree->root){ //delete node is root node
                        pthread_mutex_lock(&(delete->right->mutex));
                        tree->root=delete->right;
                        pthread_mutex_unlock(&(delete->right->mutex));
                        pthread_mutex_unlock(&(delete->mutex));
                        lab2_node_delete(delete);
                    }else{//THE OTHERS DELETE PARENT
                        return LAB2_ERROR;
                    }
                }
                return LAB2_SUCCESS;
            }

            if(!(delete->left) && !(delete->right)){
                puts("0child");
                int status = pthread_mutex_lock(&(delete->parent->mutex));
                if(status==LAB2_SUCCESS){//has parent & not root
                    if(delete->parent->left == delete){
                        delete->parent->left =NULL;
                        pthread_mutex_unlock(&(delete->mutex));
                        lab2_node_delete(delete);
                        pthread_mutex_unlock(&(delete->parent->mutex));
                    }else{
                        delete->parent->right =NULL;
                        pthread_mutex_unlock(&(delete->mutex));
                        lab2_node_delete(delete);
                        pthread_mutex_unlock(&(delete->parent->mutex));
                    }
                }else{ // no parent case
                    if(delete==tree->root){ //delete node is root node
                        pthread_mutex_unlock(&(delete->mutex));
                        lab2_node_delete(delete);
                    }else{//THE OTHERS DELETE PARENT
                        return LAB2_ERROR;
                    }
                }
                return LAB2_SUCCESS;
            }
        }
    }else{
        pthread_mutex_unlock(&(tree->mutex));
        return LAB2_ERROR; //NO TREE
    }
}
/*
int lab2_node_remove_fg(lab2_tree *tree, int key)
{
    int found = 0;
    pthread_mutex_lock(&(tree->mutex));
    if (!tree->root)
    {
        pthread_mutex_unlock(&(tree->mutex));
        //empty
    }
    else
    {
        // You need to implement lab2_node_remove function.
        lab2_node *p = tree->root; //to be deleted node
        lab2_node *q = NULL;       //deleted node' parent

        while (p)
        {
            if (p->key == key)
            {
                found = 1;
                break;
            }
            q = p;
            if (p->key > key)
                p = p->left;
            else
                p = p->right;
        }
        if (!found)
            goto UNLOCK;
        else
        {
            if ((p->left) && (p->right))
            { //two child
                lab2_node *min = p->right, *min_parent = p;
                while (min->left)
                {
                    min_parent = min;
                    min = min->left;
                }
                if (min_parent->left == min)
                {
                    min_parent->left = min->right;
                }
                else
                {
                    min_parent->right = min->right;
                }

                p->key = min->key;
                lab2_node_delete(min);
            }
            else if ((p->left == NULL) && (p->right == NULL))
            {
                if (q)
                {
                    if (q->left == p)
                        q->left = NULL;
                    else
                        q->right = NULL;
                }
                else
                    tree->root = NULL;
                lab2_node_delete(p);
            }
            else
            { // one child
                if (q)
                {
                    if (q->left == p) //Parent's left child is to be deleted
                    {
                        if (p->left)
                            q->left = p->left;
                        else
                            q->left = p->right;
                    }
                    else
                    { //Parent's right child is to be deleted
                        if (p->left)
                            q->right = p->left;

                        else
                            q->right = p->right;
                    }
                }
                else
                { //delete root
                    if (p->left)
                        tree->root = p->left;
                    else if (p->right)
                        tree->root = p->right;
                    else
                        tree->root = NULL;
                }
                lab2_node_delete(p);
            }
        }
    }
UNLOCK:
    pthread_mutex_unlock(&(tree->mutex));
    return LAB2_SUCCESS;
}
*/
/* 
 * TODO
 *  Implement a function which remove nodes from the BST in coarse-grained manner.
 *  @param lab2_tree *tree  : bst tha you need to remove node in coarse-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_cg(lab2_tree *tree, int key)
{
    // You need to implement lab2_node_remove_fg function.
    int found = 0;
    pthread_mutex_lock(&(tree->mutex));
    if (!tree->root)
    {
        //empty
    }
    else
    {
        // You need to implement lab2_node_remove function.
        lab2_node *p = tree->root; //to be deleted node
        lab2_node *q = NULL;       //deleted node' parent

        while (p)
        {
            if (p->key == key)
            {
                found = 1;
                break;
            }
            q = p;
            if (p->key > key)
                p = p->left;
            else
                p = p->right;
        }
        if (!found)
            goto UNLOCK;
        else
        {
            if ((p->left) && (p->right))
            { //two child
                lab2_node *min = p->right, *min_parent = p;
                while (min->left)
                {
                    min_parent = min;
                    min = min->left;
                }
                if (min_parent->left == min)
                {
                    min_parent->left = min->right;
                }
                else
                {
                    min_parent->right = min->right;
                }

                p->key = min->key;
                lab2_node_delete(min);
            }
            else if ((p->left == NULL) && (p->right == NULL))
            {
                if (q)
                {
                    if (q->left == p)
                        q->left = NULL;
                    else
                        q->right = NULL;
                }
                else
                    tree->root = NULL;
                lab2_node_delete(p);
            }
            else
            { // one child
                if (q)
                {
                    if (q->left == p) //Parent's left child is to be deleted
                    {
                        if (p->left)
                            q->left = p->left;
                        else
                            q->left = p->right;
                    }
                    else
                    { //Parent's right child is to be deleted
                        if (p->left)
                            q->right = p->left;

                        else
                            q->right = p->right;
                    }
                }
                else
                { //delete root
                    if (p->left)
                        tree->root = p->left;
                    else if (p->right)
                        tree->root = p->right;
                    else
                        tree->root = NULL;
                }
                lab2_node_delete(p);
            }
        }
    }
UNLOCK:
    pthread_mutex_unlock(&(tree->mutex));
    return LAB2_SUCCESS;
}
/*
 * TODO
 *  Implement function which delete struct lab2_tree
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst which you want to delete. 
 *  @return                 : status(success or fail)
 */
void lab2_tree_delete(lab2_tree *tree)
{
    // You need to implement lab2_tree_delete function.
    pthread_mutex_destroy(&(tree->mutex));
    free(tree);
}

/*
 * TODO
 *  Implement function which delete struct lab2_node
 *  ( refer to the ./include/lab2_sync_types.h for structure lab2_node )
 *
 *  @param lab2_tree *tree  : bst node which you want to remove. 
 *  @return                 : status(success or fail)
 */
void lab2_node_delete(lab2_node *node)
{
    // You need to implement lab2_node_delete function.
    pthread_mutex_destroy(&(node->mutex));
    free(node);
}
