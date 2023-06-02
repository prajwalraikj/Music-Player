#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include<stdbool.h>
#define MAX 100

typedef struct node 
{
    int id; //ID of user
    int numfren; //number of friends of user
    char name[MAX]; //name of user
    int* friends; //friends of user as an array
    struct node* right;  //user to the right
    struct node* left; //user to the left
} node;

struct node* retUser(char str[MAX])
{
    char name[MAX];
    char ID[MAX];
    char strfriends[MAX];

    //copied ID
    char*token=strtok(str,",");
    strcpy(ID,token);

    //copied Name
    token=strtok(NULL,",");
    strcpy(name,token);

    //copied friends' ID
    token=strtok(NULL,",");
    strcpy(strfriends,token);
    //printf("%s\n",(strfriends));

    //creating user nodes
    int id=atoi(ID);
    struct node *user = malloc(sizeof(struct node));
    user->id=id;
    user->friends=(int*)malloc(MAX * sizeof(int));
    strcpy(user->name,name);


    //adding user's friend's IDs
    token = strtok(strfriends,"|");  
    int i=0;
    while( token != NULL ) 
    {

        int temp=atoi(token);
        user->friends[i]=temp;
        i++;
        token = strtok(NULL,"|");
    }
    user->numfren=i;
    if(i==0){user->friends[i]=-1;}
    return user;
}


//search for user with id=key
struct node* search(int key, struct node *users)
{
    //CODE HERE

    if(users==NULL)
        return users;

    else if(key==users->id)
        return users;

    else if(users->id>key)
        return search(key,users->left);
    
    else
        return search(key,users->right);
    
}    

//see document for explanattion
struct node*refineUser(struct node*user, struct node *users)
{
    //CODE HERE
    user->left=user->right=NULL;

    if(user==NULL)
    {
        return user;
    }
    else
    {
        while(search(user->id,users)!=NULL)
        {
            user->id++;
        }

        for(int i=0;i<user->numfren;i++)
        {   
            struct node *var;
            var=search(user->friends[i],users);
            if(var==NULL)
            {
                for (int j=i;j<user->numfren-1;j++)
                {
                    user->friends[j]=user->friends[j+1];
                }
                user->numfren--;
                i--;
            }
            
            else
            {
                var->friends[var->numfren]=user->id;
                var->numfren++; 
            }
        }
    


        if(user->numfren==0)
        {
            user->friends[user->numfren]=-1;
        }

        return user;
    }
}

//insert user with id
struct node* insertUser(struct node*root,int id,struct node*user)
{   
   //CODE HERE

    node *p=root;
	node *q=NULL;
	
	if(root==NULL)
		root=user;
	else
	{
		while(p!=NULL)
		{
			q=p;
		
			if(user->id < p->id)
				p=p->left;
			else
				p=p->right;
		}
		if(user->id < q->id)
			q->left=user;
		else
			q->right=user;
	}	    
    return root;
}

//prints friends list
void friends(int id, struct node *users) 
{
   //CODE HERE

    struct node *var=search(id,users);

    if(users==NULL)
    {
        return;
    }

    else
    {
        if(var->numfren==0)
        {
            printf("-1\n");
        }
        else
        {
            for(int i=0;i<var->numfren;i++)
            {
                printf("%d\n",var->friends[i]);
            }
        }
    }

}

//find child node with minimum value (inorder successor) - helper to delete node
struct node *minValueNode(struct node *node) 
{
  //CODE HERE

    while(node->left!=NULL)
    {
        node=node->left;
    }
    return node;
}

//deletes itself from its friend's nodes
struct node*deleteFriends(int key, struct node*users)
{
    //CODE HERE
    struct node *var=search(key,users);
    struct node* friend;
    if(var==NULL)
    {
        return users;
    }
    
    for(int i=0;i<var->numfren;i++)
    {
        friend=search(var->friends[i],users);
        
        if(friend==NULL)
        {
            return NULL;
        }
        for(int j=0; j<friend->numfren;j++)
        {
            if(friend->friends[j]==key)
            {
                for(int k=j;k<friend->numfren-1;k++)
                {
                    friend->friends[k]=friend->friends[k+1];
                }

                friend->numfren--;
                j--;
            }
        }

    }
}

// Deleting a node
struct node *deleteNode(struct node *root, int key)
{
  //CODE HERE
    node *q;
    if(root==NULL)
        return root;
    
    else if(key>root->id)
        root->right=deleteNode(root->right,key);

    else if(key<root->id)
        root->left= deleteNode(root->left,key);

    else
    {
        if(root->left==NULL)		
		{
			q=root->right;
			free(root);
			return q;
		}
		else if(root->right==NULL)			
		{
			q=root->left;
			free(root);
			return q;
		}
        else
        {   q=minValueNode(root->right);
            root->id=q->id;
            strcpy(root->name,q->name);
            root->friends=q->friends;
            root->numfren=q->numfren;
            root->right=deleteNode(root->right,q->id);
        }
    }
    return root;
}

//Print USER's IDs in ascending order
void printInOrder(node* myusers) 
{
    if(myusers!=NULL)
    {
        printInOrder(myusers->left);
        printf("%d %s\n",myusers->id,myusers->name);
        printInOrder(myusers->right);
    }
}

int main(int argc, char **argv)
{
    node *users=NULL;   
    
    while(1)
    {
        int opt, id;
        fflush(stdin);
        scanf("%d",&opt);
        char str[MAX];
        switch (opt)
        {
            case 1:
      
                scanf("%s",str);
                struct node*tbins=retUser(str);
                tbins=refineUser(tbins, users);
                users=insertUser(users,tbins->id,tbins);
                break;

            case 2:
           
                scanf("%d", &id);
                deleteFriends(id, users);
                users=deleteNode(users, id);
                break;

            case 3:
        
                scanf("%d", &id);
                node* result=search(id,users);
                if(result==NULL) 
                    printf("USER NOT IN TREE.\n");
                else{
                    printf("%d\n",result->id);
                }
                break;

            case 4:
                scanf("%d", &id);
                friends(id, users);
                break;

            case 5:
                printInOrder(users);
                break;

            case 6:
                exit(0);
                break;

            default: 
                printf("Wrong input! \n");
                break;
        }

    }
    return 0;
}