/*
For a group of students, a text file with the following format is given:

The first line contains the name and surname.
The second line contains the date of birth in the format day/month/year.
Using a binary search tree structure (ordered by date of birth), implement the following operations:

Display students born on a specific date.

Display students over 20 years old.

Display the youngest student.

Display the height of the tree.

Display a text menu in the console that allows selecting any of the above operations.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Student{
    int year;
    int month;
    int day;
    char last_name[30];
    char first_name[30];
    struct Student *left, *right;
}Student;

int count=0;

Student* newNode(int day,int month,int year,char* last_name,char* first_name)
{
    Student* e=(Student*)malloc(sizeof(Student));
    if(NULL==e)
    {
        printf("Unsuccesful allocation!");
        return NULL;
    }
    e->day=day;
    e->month=month;
    e->year=year;
    e->left=NULL;
    e->right=NULL;
    strncpy(e->last_name,last_name,29);
    strncpy(e->first_name,first_name,29);
    return e;
}

void insert(Student** root,int day,int month, int year,char* last_name,char* first_name)
{
    if(!(*root))
    {
        *root=newNode(day,month,year,last_name,first_name);
        return;
    }
    if((*root)->year>year)
        insert(&((*root)->left),day,month,year,last_name,first_name);
    else if ((*root)->year<year)
        insert(&((*root)->right),day,month,year,last_name,first_name);
    else
    {
        if((*root)->month>month)
            insert(&((*root)->left),day,month,year,last_name,first_name);
        else if ((*root)->month<month)
            insert(&((*root)->right),day,month,year,last_name,first_name);
        else
        {
            if((*root)->day>day)
                insert(&((*root)->left),day,month,year,last_name,first_name);
            else if ((*root)->day<=day)
                insert(&((*root)->right),day,month,year,last_name,first_name);
        }
    }
}

void inordine(Student* root)
{
    if(!root)
        return;
    inordine(root->left);
    printf("%s %s born on :%d %d %d \n",root->last_name,root->first_name,root->day,root->month,root->year);
    inordine(root->right);
}

void youngest(Student* root)
{
    if(root==NULL)
    {
        printf("No student found!");
        return;
    }
    while(root->right!=NULL)
    {
        root=root->right;
    }
    printf("The youngest student is: %s %s born on :%d %d %d \n",root->last_name,root->first_name,root->day,root->month,root->year);
}

void search_by_date(Student* root,int day,int month,int year)
{
    if(!root)
        return;
    search_by_date(root->left,day,month,year);
    if(root->day==day && root->month==month && root->year==year)
        printf("Nascut pe %d/%d/%d este : %s %s \n",root->day,root->month,root->year,root->last_name,root->first_name);
    search_by_date(root->right,day,month,year);
}

void peste_20(Student* root)
{
    if(!root)
        return;
    peste_20(root->left);
    if((2017-root->year)>=20)// 2017 is the date of last version of FisierText.txt file
        printf("%s %s born on :%d %d %d \n",root->last_name,root->first_name,root->day,root->month,root->year);
    peste_20(root->right);
}

int max(int x,int y)
{
    if(x>y)
        return x;
    return y;
}

int tree_height(Student* root)
{
    if(root==NULL)
        return 0;
    int height_left=tree_height(root->left);
    int height_right=tree_height(root->right);
    return max(height_left,height_right)+1;
}


void eliberare(Student* rad)
{
    if(rad==NULL)
        return;
    eliberare(rad->left);
    eliberare(rad->right);
    free(rad);
}

int main()
{
    int n = 0;
    int day,month,year;
    int height=0;
    int chunk=10;
    
    Student* rad=NULL;
    
    Student *students=(Student*)malloc(chunk*sizeof(Student));
    
    if(NULL==students)
    {
        printf("Memory has not been allocated!");
        return 0;
    }
    FILE* fisier=fopen("FisierTest.txt","r");
    if(NULL==fisier)
    {
        printf("Couldn't open the file");
        return 0;
    }
    
    while(!feof(fisier))
    {
        if(n==chunk)
        {
            chunk=chunk*2;
            students=realloc(students,chunk*sizeof(Student));
            if(students==NULL)
            {
                printf("Memory has not been allocated!");
                return 0;
            }
        }
        fscanf(fisier,"%s %s",students[n].last_name,students[n].first_name);
        fscanf(fisier,"%d/%d/%d",&students[n].day,&students[n].month,&students[n].year);
        
        insert(&rad,students[n].day,students[n].month,students[n].year,students[n].last_name,students[n].first_name);
        
        n++;
    }
    
    free(students);
    fclose(fisier);
    
    for(;;)
    {
        int opt;
        printf("Choose option:\n");
        printf("0: Close program\n");
        printf("1: Print student\n");
        printf("2: Print youngest student\n");
        printf("3: Print students who are above 20 years\n");
        printf("4: Print tree's height\n");
        printf("5: Search student by date\n");
        scanf("%d",&opt);
        switch(opt)
        {
            case 0:
                printf("The program is closing...\n");
                eliberare(rad);
                return 0;
            case 1:
                inordine(rad);
                break;
            case 2:
                youngest(rad);
                break;
            case 3:
                peste_20(rad);
                break;
            case 4:
                height=tree_height(rad);
                printf("Height is : %d\n",height);
                break;
            case 5:
                printf("Choose day:\n");
                scanf("%d",&day);
                printf("Choose month:\n");
                scanf("%d",&month);
                printf("Choose year:\n");
                scanf("%d",&year);
                search_by_date(rad,day,month,year);
                break;
            default:
                printf("Instruction is not available!\nPlease try again!\n");
                break;
        }
    }

    return 0;
}
