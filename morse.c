#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include<ctype.h>
/*this function is used to allow the user to enter an length of input he wants.
*/
// this function is similar to readlongline in practice 8.  
char *readuserinput()
{
    int length = 0;
    char *file = (char*) malloc(sizeof(char)*1);
    file[0] = '\0';
    char newchar;
    while (scanf("%c", &newchar) == 1 && newchar != '\n')
    {
        char *newfilename = (char*) malloc(sizeof(char) * (length+1+1));
        for(int i = 0; i < length; i++)
        {
            newfilename[i] = file[i];
        }
        newfilename[length] = newchar;
        newfilename[length + 1] = '\0';
        free(file);
        file = newfilename;
        length++;
    }
    return file;
}
// the node structure : the building unit of the binary tree. two char one for data and one for morse and two pointers.
typedef struct node{
    char data;
    char morse; // this will only contain dots and dashs so we could move in the binary tree
    struct node *left, *right;
} node;
// the structure we use in the array containing the Morse and Text pairs
typedef struct{
    char text;
    char morse[8];
} letter;
// the building function of the binary tree. creates a node pointer, allocate memory for it,
//nulls its pointers, assign char to its data or it's morse or both and return that pointer
node* crtnode(char c,char morse){
    node *n = (node*)malloc(sizeof(node));
    n->data = c;
    n->morse = morse;
    n->left = n->right = NULL;
    return(n);
}
//this function checks if the user input is morse code
int checkifmorse(char *userinput)
{
    for(int i = 0; i < strlen(userinput);i++) 
    {
        if(userinput[i] != '.' && userinput[i] != '/' &&  userinput[i] != '-' && userinput[i] != '\n' && userinput[i] != ' ')
        {
            return 1; // not morse
        }
    } 
    return 0; // this is vaild morse input
}
//function to switch from normal to morse 
void encode(char *input,FILE *fp, letter *pairs)
{
    // get the input and loops untill the length of it
    int limit = strlen(input);
    // to keep track of dots and dashs and slashes
    int numofdots = 0;
    int numofdashs = 0;
    int numofslashes = 0;
    for(int i = 0; i < limit; i++)
    {
        // if the loop is an upper char we have to turn it to lower to fit the pairs struct.
        if(isupper(input[i]))
        {
            input[i] = tolower(input[i]);
        }
        // loop the pairs struct and try to find the letter eq to it.
        for(int j = 0; j < 53; j++)
        {
            if(input[i] == pairs[j].text)
            {
                int k = 0;
                // loop to see how many dots and dashs
                while(pairs[j].morse[k] != '\0')
                {
                    if(pairs[j].morse[k] == '.')
                    {
                        numofdots++;
                    }
                    else if(pairs[j].morse[k] == '-')
                    {
                        numofdashs++;
                    }
                    else
                    {
                        numofslashes++;
                    }
                    k++;
                }
                // if the user enter a file name then we will print to the file else he will give us NULL.
                if(fp)
                {
                    fprintf(fp,"%s ",pairs[j].morse);
                }
                else
                {
                printf("%s ",pairs[j].morse);
                }
            }
        }
    }
    if(fp)
    {
    fprintf(fp,"\n");
    fprintf(fp,"Number of dots is: %i\n",numofdots);
    fprintf(fp,"Number of dash is: %i\n",numofdashs);
    fprintf(fp,"Number of slashs is: %i\n",numofslashes); 
    }
    if(!fp)
    {
        printf("\n");
        printf("Number of dots is: %i\n",numofdots);
        printf("Number of dash is: %i\n",numofdashs);
        printf("Number of slashs is: %i\n",numofslashes);
    }
}

void insertleft(node* r) // this inserts a node that contains a . on the left
{
    node *new = crtnode('\0','.');
    r->left = new;
}
void inserttight(node* r) // this inserts a node that contains a - on the right
{
    node *new = crtnode('\0','-');
    r->right = new;
}
// form morse to normal
void decode(char *input,node *root,FILE *fp)
{
    if(checkifmorse(input))
    {
        printf("The text entered is incorrect\n");
        return;
    }
    int arrcountchars[127]; // contains an element to count every Ascii value.
    for(int i = 0; i < 127;i++)
    {
        arrcountchars[i] = 0;
    } 
    // temp to move in the binary tree.
    node *temp;
    temp = root;
    if(checkifmorse(input))
    {
        printf("The text entered is incorrect\n");
        return;
    }
    int limit = strlen(input);
    int i = 0;
    while(i < limit) // loops over every char in input
    {
        node *temp = root;
        int j = i;
        while(input[j] != '\0' && input[j] != ' ' && input[j] != '/') // we stop at space because it saprates the letters and / because it sprates the words.
        {
            if(input[j] == '.')
            {
                temp = temp->left; // moves to the left 
            }
            else
            {
                temp = temp->right; // moves to the right
            }
            j++;
        }
        if(fp)
        {
            if(input[j] == '/')
            {
            fprintf(fp," ");// print a space in the file
            j++; // move to the next space after  
            i = ++j; // skip it
            continue;
            }
            arrcountchars[temp->data]++;
            fprintf(fp,"%c",temp->data);
        }
        else
        {
            if(input[j] == '/')
            {

                //reach we print a space
                printf(" ");
                j++;
                i = j++;
                continue;
            }
            // print where the temp moved last.
            arrcountchars[temp->data]++;
            printf("%c",temp->data);
        }
    i = ++j; // move to next letter
    }
    if(fp)
    {
        fprintf(fp,"\n");
    }
    else
    {
        printf("\n");
    }
    for(int i = 1; i < 127; i++) // to not include the Null char
    {
        if(arrcountchars[i] != 0)
        {
            if(fp)
            {
                fprintf(fp,"The number of %c is %i\n",i,arrcountchars[i]);
            }
            else
            {
                printf("The number of %c is %i\n",i,arrcountchars[i]);
            }
        }
    }
}
// code from lecture 13 binary trees
//function to free the binary tree
void deleteTree(node* root)
{
    if (root == NULL) return;
 
    /* first delete both subtrees */
    deleteTree(root->left);
    deleteTree(root->right);
   
    /* then delete the node */
    free(root);
}

int main()
{
    // normal letter and their morse equivlanet
    letter pairs[53]={
    {'a', ".-\0"},{'b', "-...\0"},{'c', "-.-.\0"},{'d', "-..\0"},{'e', ".\0"},{'f', "..-.\0"}, {'g', "--.\0"},
    {'h', "....\0"},{'i', "..\0"}, {'j', ".---\0"},{'k', "-.-\0"},{'l', ".-..\0"},{'m', "--\0"},{'n', "-.\0"},
    {'o', "---\0"},{'p', ".--.\0"},{'q', "--.-\0"},{'r', ".-.\0"},{'s', "...\0"},{'t', "-\0"},{'u', "..-\0"},
    {'v', "...-\0"},{'w', ".--\0"},{'x', "-..-\0"},{'y', "-.--\0"},{'z', "--..\0"},{'0', "-----\0"}, {'1', ".----\0"},
    {'2', "..---\0"},{'3', "...--\0"},{'4', "....-\0"},{'5', ".....\0"},{'6', "-....\0"},{'7', "--...\0"},{'8', "---..\0"},
    {'9', "----.\0"},{'/', "-..-.\0"},{'\n', ".-.-\0"},{'.', ".-.-.-\0"},{'-', "-....-\0"},{'&', ".-...\0"},{'\'', ".----.\0"},
    {'@', ".--.-.\0"},{')', "-.--.-\0"},{'(', "-.--.\0"},{':', "---...\0"},{',', "--..--\0"},{'=', "-...-\0"},{'!', "-.-.--\0"},
    {'+', ".-.-.\0"},{'\"', ".-..-.\0"},{'?', "..--..\0"},{' ', "/\0"}
    };
    node *root = crtnode('\0','\0'); // the root of the binary tree.
    // loop to crt the binary tree
    for(int i = 0; i < 52; i++) // loop over every char except space because it does not have a morse equivalent
    {
        node *temp = root;// temp node to move in the for loop and create nodes.
        int limit = strlen(pairs[i].morse);
        int j = 0;
        for(j; j < limit - 1; j++) // loops over every dot and dash expect the last one because we want to put the value of the char in the last one
        {
            if(pairs[i].morse[j] == '.')
            {
                if(temp->left != NULL)// check if there is already a node.
                {
                    temp = temp->left; // if there is we move to it.
                }
                else
                {
                   insertleft(temp); // we inserted a node that contains a dot on
                   temp = temp->left; // we move to it.
                }
            }
            else if(pairs[i].morse[j] == '-')
            {
                if(temp->right != NULL)
                {
                    temp = temp->right;
                }
                else
                {
                    inserttight(temp);
                    temp = temp->right;
                }
            }
        }
        // this is for the last char
        if(pairs[i].morse[j] == '.')
        {
            if(temp->left !=NULL)
            {
                temp->left->data = pairs[i].text;
            }
            else
            {
                temp->left = crtnode(pairs[i].text,'.');
            }
        }
        else if(pairs[i].morse[j] == '-')
        {
            if(temp->right !=NULL)
            {
                temp->right->data = pairs[i].text;
            }
            else
            {
                temp->right = crtnode(pairs[i].text,'-');
            }            
        }
    }
    int choice = 0;
    do{
    printf("Enter 1: If you want to encode input and print on the screen\n");
    printf("Enter 2: If you want to encode from file and print on the screen\n");
    printf("Enter 3: If you want to encode input and print output to file\n");
    printf("Enter 4: If you want to decode input and print on the screen\n");
    printf("Enter 5: If you want to decode from file and print on the screen\n");
    printf("Enter 6: If you want to decode form file and print output to file\n");
    printf("Enter 9: If want to exit the code \n");
    scanf("%i",&choice);
    if(((choice < 1 || choice > 6)) && choice != 9)
    {
        printf("Wrong input please enter again!\n");
    }
    if(choice == 1)
    {
        printf("Enter text: \n");
        // because the printf ends with a new line and that does not allow the function to work
        getchar();
        // works as read a long line
        char *input = readuserinput();
        // gets input encodes and prints output on screen
        encode(input,NULL,pairs);
        // free the input from stack
        free(input);
    }
    if(choice == 2)
    {
    FILE *fp; printf("Enter the file path: Example: (C:\\Documents\\)\n");getchar();
    // static file name for the user to enter the path.
    char fileN[300];
    gets(fileN);
    // want to get data from file so we use r.
    fp = fopen(fileN,"r");
    if(fp == NULL)
        {
            // check if the file handling worked.
            printf("Uncorrect file name");
        }
    // scan a word from the file the max size is 12.
    char input[20];
    fgets(input,20,fp);
    // get the input prints it on the screen using encode we have null because we do not want to print to file.
    encode(input,NULL,pairs);
    fclose(fp);
    }
    if(choice == 3)
    {
    FILE *fp; printf("Enter the file path: Example: (C:\\Documents\\)\n");getchar();
    // static file name for the user to enter the path.
    char fileN[300];
    gets(fileN);
    // we do not check if we entered the right path because if we did not the fopen will create a file by itself
    fp = fopen(fileN,"w");
    printf("Enter text: \n");
    char *input = readuserinput();
    encode(input,fp,pairs);
    // free input and close the file.
    free(input);
    fclose(fp);
    }
    if(choice == 4)
    {   printf("Enter text: \n");
        // because the printf ends with a new line and that does not allow the function to work
        getchar();
        // works as read a long line
        char *input = readuserinput();
        decode(input,root,NULL);
        free(input);       
    }
    if(choice == 5)
    {    
    FILE *fp; printf("Enter the file path: Example: (C:\\Documents\\)\n");getchar();
    // static file name for the user to enter the path.
    char fileN[300];
    gets(fileN);
    // want to get data from file so we use r.
    fp = fopen(fileN,"r");
    if(fp == NULL)
        {
            // check if the file handling worked.
            printf("Uncorrect file name\n");
        }
    // scan a morse code of max size of 50.
    char input[50];
    fgets(input,50,fp);
    decode(input,root,NULL);
    fclose(fp);
    }
    if(choice == 6)
    {
    FILE *fp; printf("Enter the file path: Example: (C:\\Documents\\)\n");getchar();
    // static file name for the user to enter the path.
    char fileN[300];
    gets(fileN);
    // we do not check if we entered the right path because if we did not the fopen will create a file by itself
    fp = fopen(fileN,"w");
    printf("Enter text: \n");
    char *input = readuserinput();
    decode(input,root,fp);
    // free input and close the file.
    free(input);
    fclose(fp);
    }
    }while(choice != 9);
    deleteTree(root);
}