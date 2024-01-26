#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

//Max size of password and username
#define MAX_SIZE 70
#define MAX_USERNAME_SIZE 50
#define MAX_PASSWORD_SIZE 20
#define S_SIZE 10

//User input like this
typedef struct {
    char u_name[MAX_USERNAME_SIZE];
    char u_password[MAX_PASSWORD_SIZE];
}user_t;

//This function determines whether or not the number is prime
unsigned int prime(unsigned int n)
{
    for (unsigned int i = 2; i <= sqrt(n); i++)
    {
        if (n % i == 0)
        {
            return 0;
        }
    }

    return 1;    
}

//The largest common divisor of two numbers is returned by this function
unsigned int gcd(unsigned int a, unsigned int b) 
{ 
    // Find Minimum of a and b 
    unsigned int result = ((a < b) ? a : b); 
    while (result > 0)
    {
        // Check if both a and b are divisible by result 
        if (a % result == 0 && b % result == 0)
        { 
            break; 
        } 
        result--; 
    } 

    // return gcd of a and b 
    return result; 
} 

//The largest common divisor of two numbers is returned by this function
unsigned int totient(unsigned int a, unsigned int b)
{
    return (a - 1) * (b - 1);
}

//This function gives back a random integer that is smaller than the input
unsigned int randome(unsigned int lambda_n)
{
    if (lambda_n <= 2)
    {
        printf("\nInput value must be greater than 2!\n\n");
        exit(EXIT_FAILURE);
    }
    
    unsigned int r_number;

    do
    {
        //Generate random number from current time
        srand((unsigned int)time(NULL));
        r_number = rand() % (lambda_n - 2) + 2;        //Ensure e is greater than 1 and less than lambda_n
    } while (gcd(r_number, lambda_n) != 1);
    

    //Take modulo from that number
    return r_number;
}

//The private key is returned by this function
unsigned int private_key(unsigned int e, unsigned int lambda_n)
{
    if (gcd(e, lambda_n) != 1)
    {
        printf("\nError: e and lambda_n are not coprime.\n");
        exit(EXIT_FAILURE);
    }

    for (unsigned int i = 1; i < lambda_n; i++)
    {
        if ((i * e) % lambda_n == 1)
        {
            printf("\nThus, (i * a) %% b = 1, (%d * %d) %% %d = 1\n", i, e, lambda_n);
            return i;
        }
    }
    
    printf("Somthing went wrong!");
    exit(EXIT_FAILURE);
}

//Gives the number's modular exponentiaton
unsigned long pomod(unsigned int a, unsigned int b, unsigned int m)
{
    unsigned long x = 1, y = a;
    while (b > 0)
    {
        if (b % 2 == 1)
        {
            x = (x * y) % m;
        }
        
        y = (y * y) % m;
        b /= 2;
    }

    return x % m;
}

//Get user input
int get_user_input()
{
    unsigned int number;
    char *endptr;
    char *buffer = (char *)malloc(sizeof(char) * 10);

    if (fgets(buffer, sizeof(buffer), stdin) != NULL)
    {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n')
        {
            buffer[len - 1] = '\0';
        }

        number = strtol(buffer, &endptr, 10);

        if (*endptr != '\0' && *endptr != '\n')
        {
            printf("Invalid input. Please enter a valid integer.");
            return 0;
        }

        return number;
        
    }

    free(buffer);
    return 0;
}

//Check if password have blunks
bool blank_check(const char *data)
{
    while (*data)
    {
        if (isspace(*data))
        {
            return 1;       //Contain blanks
        }
        data++;
    }
    return 0;       //No blanks
}

//Da main squeeze
int main()
{
    //Fore encryption and decryption
    unsigned int p, q, lambda_n;
    unsigned long n, e, d;

    //File declaretion
    FILE *fp;

    //Declaration
    char str_e[MAX_SIZE];
    int long size_of_file = 0;

    //Menu
    printf("\n----------Menu----------\n");
    printf("  1. Enter data to file\n");
    printf("  2. Read data from file\n");
    printf("  3. Encrypt data in file\n");
    printf("  4. Decrypt data from file\n");
    printf("  5. Leave\n\n");

    //Input case to menu
    printf("  Enter number: ");
    if (fgets(str_e, sizeof(MAX_SIZE), stdin) != NULL)
    {
        size_t len = strlen(str_e);
        if (len > 0 && str_e[len - 1] == '\n')
        {
            str_e[len - 1] = '\0';
        }
    }

    switch (*str_e)
    {
    case '1':
        //Initialize users
        int max_number_of_users = 2;
        int number_of_users = 1;

        //Open file for writing
        fp = fopen("Data.txt", "w");
        if (fp == NULL)
        {
            perror("Error opening file");
            return 1;
        }

        //Allocate memory for storing users
        user_t *users = (user_t *)malloc(max_number_of_users * sizeof(MAX_SIZE));
        if (users == NULL)
        {
            fprintf(stderr, "Memory allocation failed!\n");
            return 1;
        }

        //Reading user information
        while (number_of_users < max_number_of_users)
        {
            //Entering username
            printf("\nEnter username (MAXSIZE %d) or type 'exit' to stop: ", MAX_USERNAME_SIZE - 2);
            if (fgets(users[number_of_users].u_name, sizeof(users[number_of_users].u_name), stdin) != NULL)
            {
                //Remove newline character if present
                size_t len = strlen(users[number_of_users].u_name);
                if (len > 0 && users[number_of_users].u_name[len - 1] == '\n')
                {
                    users[number_of_users].u_name[len - 1] = '\0';
                }

                //Check for blanks
                if (blank_check(users[number_of_users].u_name))
                {
                    printf("Blank is invalid!\n");
                    break;
                }
            }
            else
            {
                printf("Somthing went wrong with entering username!");
            }

            //If user want leave
            if (strcmp(users[number_of_users].u_name, "exit") == 0)
            {
                break;
            }

            //Entering password
            printf("Enter password (MAXSIZE %d): ", MAX_PASSWORD_SIZE - 2);
            if (fgets(users[number_of_users].u_password, sizeof(users[number_of_users].u_password), stdin) != NULL)
            {
                //Remove newline character if present
                size_t len = strlen(users[number_of_users].u_password);
                if (len > 0 && users[number_of_users].u_password[len - 1] == '\n')
                {
                    users[number_of_users].u_password[len - 1] = '\0';
                }

                //Check for blanks
                if (blank_check(users[number_of_users].u_password))
                {
                    printf("Blank is invalid!\n");
                    break;
                }
            }
            else
            {
                printf("Somting went wrong with entering password!");
            }


            //Writing users input in file
            fprintf(fp, "%d: %s, %s.\n",
                                   number_of_users,
                                   users[number_of_users].u_name,
                                   users[number_of_users].u_password);

            number_of_users++;

            //Check if more space needed
            if (number_of_users == max_number_of_users)
            {
                //Double current size of the array
                max_number_of_users += 2;

                //Reallocate memory
                users = (user_t *)realloc(users, max_number_of_users * sizeof(MAX_SIZE));
                if (users == NULL)
                {
                    fprintf(stderr, "Memory reallocation is failed!\n");
                    return 1;
                }
            }
        }

        //Free memory
        free(users);

        //Close file
        fclose(fp);
        break;

    case '2':
        //Open file
        fp = fopen("Data.txt", "r");
        if (fp == NULL)
        {
            perror("Error opening file\n");
            return 1;
        }

        //Creating buffer for reading
        fseek(fp, 0L, SEEK_END);                     //Searching for EOF
        size_of_file = ftell(fp);                    //Give size of file
        rewind(fp);                                  //Cursor for beginning
        char *buff_file = malloc(size_of_file);      //Alocate memory
        if (buff_file == NULL)
        {
            perror("Memory allocation in reading from file error!\n");
            return 1;
        }

        //Reading
        fread(buff_file, size_of_file, 1, fp);    //Read file
        printf("\n%s\n", buff_file);              //Print file data

        //Close file
        fclose(fp);

        //Free memory
        free(buff_file);

        break;

    case '3':
        //Get number
        printf("\nEnter value of p: ");
        p = get_user_input();
        if (p == 0)
        {
            printf("Value p error");
            exit(EXIT_FAILURE);
        }

        //Get number
        printf("\nEnter value of q: ");
        q = get_user_input();
        if (q == 0)
        {
            printf("Value q error");
            exit(EXIT_FAILURE);
        }

        //Main thing
        if (prime(p) && prime(q))
        {
            n = p * q;
            lambda_n = totient(p, q);
            e = randome(lambda_n);
            d = private_key(e, lambda_n);
            if (d == 0)
            {
                exit(EXIT_FAILURE);
            }

            printf("\nThe value of n is %ld\n", n);
            //printf("The value of lambda_n is %d\n", lambda_n);
            //printf("The value of e is %ld\n", e);
            printf("The value of d is %ld\n\n", d);

            //Open file for encryption
            fp = fopen("Data.txt", "r");
            if (fp == NULL)
            {
                perror("Error opening file maybe it didn't exist\n");
                return 1;
            }

            //Creating buffer for reading
            fseek(fp, 0, SEEK_END);                     //Searching for EOF
            size_of_file = ftell(fp);                    //Give size of file
            rewind(fp);                         //Rewind to beginning 

            char *file_content = (char *)malloc(size_of_file);        //Alocate memory
            if (file_content == NULL)
            {
                perror("Memory allocation in encryption error!\n");
                fclose(fp);
                return 1;
            }

            fread(file_content, 1, size_of_file, fp);      //Read file

            //Close file for reading
            fclose(fp);

            //Open file for encryption
            fp = fopen("Data.txt", "w");
            if (fp == NULL)
            {
                perror("Error opening file maybe it didn't exist\n");
                free(file_content);
                return 1;
            }

            for (long i = 0; i < size_of_file; i++)
            {
                unsigned long encrypted_value = pomod(file_content[i], e, n);

                //Add delimiter between encrypted value
                fprintf(fp, "%lu ", encrypted_value);
            }

            //File close
            fclose(fp);

            //Free memory
            free(file_content);
        }
        else
        {
            printf("\nThe value of p and q should be prime.\n");
            return 1;
        }
        break;

    case '4':
        //Get user input for d
        printf("Enter the value of d: ");
        d = get_user_input();
        if (d == 0)
        {
            printf("Value d error");
            exit(EXIT_FAILURE);
        }

        //Get user input for n
        printf("Enter the value of n: ");
        n = get_user_input();
        if (n == 0)
        {
            printf("Value n error");
            exit(EXIT_FAILURE);
        }
        
        //Open file for decryption
        fp = fopen("Data.txt", "r");
        if (fp == NULL)
        {
            perror("Error opening file maybe it didn't exist\n");
            return 1;
        }

        //Creating buffer for reading
        fseek(fp, 0, SEEK_END);                     //Searching for EOF
        size_of_file = ftell(fp);                    //Give size of file
        rewind(fp);                         //Rewind to beginning 

        char *file_content = (char *)malloc(size_of_file);        //Alocate memory
        if (file_content == NULL)
        {
            perror("Memory allocation in encryption error!\n");
            fclose(fp);
            return 1;
        }

        fread(file_content, 1, size_of_file, fp);      //Read file

        //Close file for reading
        fclose(fp);

        //Open file for decryption
        fp = fopen("Data.txt", "w");
        if (fp == NULL)
        {
            perror("Error opening file maybe it didn't exist\n");
            free(file_content);
            return 1;
        }
        
        char *token = strtok(file_content, " ");        //Delimiter

        while (token != NULL)
        {
            unsigned long decrypted_value = pomod(atoi(token), d, n);

            //Back to char
            char decrypted_char = (char)decrypted_value;
            fprintf(fp, "%c", decrypted_char);

            //Move on
            token = strtok(NULL, " ");
        }

        //Close files
        fclose(fp);

        //Free memory
        free(file_content);
        break;

    case '5':
        printf("\n  Bye Dude\n\n");
        break;

    default:
        printf("\nSomthing went wrong with!\n\n");
        break;
    }

    return 0;
}