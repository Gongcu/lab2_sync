/*
*	Operating System Lab
*	    Lab2 (Synchronization)
*	    Student id : 32164172
*	    Student name : JeongYuseok
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
    new->left = new->right = NULL;
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
    if (temp == NULL) //if root is empty, insert node in the root
    {
        tree->root = new_node;
        return LAB2_SUCCESS;
    }
    while (temp != NULL) //Goes down until it meets the downmost tree
    {
        parent_node = temp;
        if (temp->key == new_node->key) // already exists
            return LAB2_ERROR;
        if (temp->key > new_node->key) //Set up proper location of the root
            temp = temp->left;
        else
            temp = temp->right;
    }

    if (parent_node->key > new_node->key) //insert key value
        parent_node->left = new_node;
    else
        parent_node->right = new_node;
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

    lab2_node *temp = tree->root;
    lab2_node *parent_node = NULL;
    pthread_mutex_lock(&tree->mutex);

    if (temp == NULL) //if root is empty, insert node in the root
    {
        tree->root = new_node;
        pthread_mutex_unlock(&(tree->mutex));
        return LAB2_SUCCESS;
    }

    while (temp != NULL) //Goes down until it meets the downmost tree
    {
        parent_node = temp;
        if (temp->key == new_node->key){ // already exists
            pthread_mutex_unlock(&(tree->mutex));
            return LAB2_ERROR;
        }
        if (temp->key > new_node->key) //Set up proper location of the root
            temp = temp->left;
        else
            temp = temp->right;
    }

    if (parent_node->key > new_node->key) //insert key value
        parent_node->left = new_node;
    else
        parent_node->right = new_node;

    pthread_mutex_unlock(&(tree->mutex));
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
    // You need to implement lab2_node_insert function.

    lab2_node *temp = tree->root;
    lab2_node *parent_node = NULL;
    
    if (temp == NULL) //if root is empty, insert node in the root
    {
        pthread_mutex_lock(&(tree->mutex));
        tree->root = new_node;
        pthread_mutex_unlock(&(tree->mutex));
        return LAB2_SUCCESS;
    }
    
    while (temp != NULL) //Goes down until it meets the downmost tree
    {
        parent_node = temp;
        if (temp->key == new_node->key) // already exists
            return LAB2_ERROR;
        
        pthread_mutex_lock(&parent_node->mutex);
        if (temp->key > new_node->key) //Set up proper location of the root
            temp = temp->left;
        else
            temp = temp->right;
        pthread_mutex_unlock(&parent_node->mutex);
    }
    pthread_mutex_lock(&parent_node->mutex);
    if (parent_node->key > new_node->key){ //insert key value
        parent_node->left = new_node;
    }
    else{
        parent_node->right = new_node;
    }
    pthread_mutex_unlock(&parent_node->mutex);
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
    int found = 0;
    if (!tree->root)
        return LAB2_ERROR;
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
        return LAB2_ERROR;

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
        }else
            tree->root = NULL;
        lab2_node_delete(p);
    }
    else
    { // one child
        if (q){
            if (q->left==p) //Parent's left child is to be deleted
            {
                if (p->left)
                    q->left = p->left;
                else
                    q->left = p->right;
            }
            else{ //Parent's right child is to be deleted
                if (p->left)
                    q->right = p->left;

                else
                    q->right = p->right;
            }
        }
        else{ //delete root
            if (p->left)
                tree->root = p->left;
            else if (p->right)
                tree->root = p->right;
            else
                tree->root = NULL;
        }
        lab2_node_delete(p);
    }
    return LAB2_SUCCESS;
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST in fine-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in fine-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_fg(lab2_tree *tree, int key)
{
    // You need to implement lab2_node_remove_fg function.
}

/* 
 * TODO
 *  Implement a function which remove nodes from the BST in coarse-grained manner.
 *
 *  @param lab2_tree *tree  : bst tha you need to remove node in coarse-grained manner from bst which contains key.
 *  @param int key          : key value that you want to delete. 
 *  @return                 : status (success or fail)
 */
int lab2_node_remove_cg(lab2_tree *tree, int key)
{
    // You need to implement lab2_node_remove_cg function.
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