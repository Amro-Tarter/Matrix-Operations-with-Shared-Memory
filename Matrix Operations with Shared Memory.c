#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <complex.h>
#define MAX_CHAR 128
#define MAX_STRUCTS 16


void add(double complex **arr1, double complex **arr2, double complex **arr3);
void sub(double complex **arr1, double complex **arr2, double complex **arr3);
void multiply(double complex **arr1, double complex **arr2, double complex **arr3, int len);
void ANDFunction(double complex **mat1, double complex **mat2, double complex **result, int rows);
void ORFunction(double complex **mat1, double complex **mat2, double complex **result, int rows);
void transpose(double complex **matrix, double complex **result, int rows);

void roundMatrix(double complex ** arr);
double roundNum(double num);
int isBinaryMatrix(double complex **matrix, int rows);
int matrixType(char *string);
char *matrixToString(double complex **matrix, int rows,int flag);
void swapToMatrix(const char *input, double complex ***matrix, int *rows, int type) ;
void printMatrix(double complex **matrix, int rows);
void freeMatrix(double complex **matrix, int rows);
double complex **allocateMatrix(int rows);
int len;
int flag;

typedef struct sharedData {
    char data[3][MAX_CHAR];
} sharedData;

int main(void) {

    key_t key;
    int shm_id;

    if ((key = ftok("/tmp", 'A')) == -1) {
        perror("ftok() failed");
        exit(EXIT_FAILURE);
    }

    if ((shm_id = shmget(key, 0, 0600)) == -1) {
        perror("shmget() failed");
        exit(EXIT_FAILURE);
    }

    sharedData *temp = (sharedData *)shmat(shm_id, NULL, 0);
    if (temp == (sharedData *)-1) {
        perror("shmat() failed");
        exit(EXIT_FAILURE);
    }

    sem_t *sem_writer = sem_open("/writer", 0);
    sem_t *sem_reader = sem_open("/reader", 0);

    if (sem_writer == SEM_FAILED || sem_reader == SEM_FAILED) {
        perror("sem_open() failed");
        exit(EXIT_FAILURE);
    }
    sem_wait(sem_reader);

    double complex **arr1 = allocateMatrix(len);
    double complex **arr2 = allocateMatrix(len);
    double complex **arr3 = allocateMatrix(len);



    for (int i = 0; i < MAX_STRUCTS; ++i) {
        if (strcmp(temp[i].data[0], "no input") == 0) {
            continue;
        }


        len = temp[i].data[0][1] - 48;
        flag= matrixType(temp[i].data[0]);

        swapToMatrix(temp[i].data[0], &arr1, &len, flag);
        swapToMatrix(temp[i].data[0], &arr2, &len, flag);
        swapToMatrix(temp[i].data[0], &arr3, &len, flag);




      if (strcmp(temp[i].data[1], "NOT") != 0 && strcmp(temp[i].data[1], "TRANSPOSE") != 0) {
          if (flag == 0) {
              flag = matrixType(temp[i].data[1]);
          }


          swapToMatrix(temp[i].data[1], &arr2, &len, flag);

          if (flag == 1) {       // we round the numbers in the matrix
              roundMatrix(arr1);
              roundMatrix(arr2);

          }

          if (strcmp(temp[i].data[2], "ADD") == 0) {
              add(arr1, arr2, arr3);



               if (flag == 1) {
                   roundMatrix(arr3);
               }

                strcpy(temp[i].data[2], matrixToString(arr3, len, flag));
               printf("%s\n", temp[i].data[2]);

           }else if (strcmp(temp[i].data[2], "SUB") == 0) {

               sub(arr1, arr2, arr3);
               if (flag == 1) {
                   roundMatrix(arr3);
               }

               strcpy(temp[i].data[2], matrixToString(arr3, len, flag));
               printf("%s\n", temp[i].data[2]);

           } else if (strcmp(temp[i].data[2], "MUL") == 0) {

               multiply(arr1, arr2, arr3, len);

               if (flag == 1) {
                   roundMatrix(arr3);
               }

               strcpy(temp[i].data[2], matrixToString(arr3, len, flag));
               printf("%s\n", temp[i].data[2]);

           }else if (strcmp(temp[i].data[2], "AND") == 0) {

               if (isBinaryMatrix(arr1, len) == 1 & isBinaryMatrix(arr2, len) == 1) {
                   ANDFunction(arr2, arr1, arr3, len);
                   if (flag == 1) {
                       roundMatrix(arr3);
                   }
                   strcpy(temp[i].data[2] , matrixToString(arr3, len, flag));
                   printf("%s\n", temp[i].data[2]);
               } else{
                   printf("ERR\n");
               }
           }else if (strcmp(temp[i].data[2], "OR") == 0) {
               if (isBinaryMatrix(arr1, len) == 1 & isBinaryMatrix(arr2, len) == 1) {
                   ORFunction(arr1, arr2, arr3, len);
                   if (flag == 1) {
                       roundMatrix(arr3);
                   }
                   strcpy(temp[i].data[2] , matrixToString(arr3, len, flag));
                   printf("%s\n", temp[i].data[2]);
               }else{
                   printf("ERR\n");
               }
           }


       }else {
           if (flag == 1) {
               roundMatrix(arr1);
           }
           if (strcmp(temp[i].data[1], "NOT") == 0) {
               // to the not method
               if (isBinaryMatrix(arr1, len)) {
                   for (int k = 0; k < len; ++k) {
                       for (int j = 0; j < len; ++j) {
                           if (arr1[k][j] == 1) {
                               arr1[k][j] = 0;
                           } else
                               arr1[k][j] = 1;
                       }
                   }
                   if (flag == 1) {
                       roundMatrix(arr1);
                   }
                   strcpy(temp[i].data[2] , matrixToString(arr1, len, flag));
                   printf("%s\n", temp[i].data[2]);
               } else{
                   printf("ERR\n");
               }
           } else if (strcmp(temp[i].data[1], "TRANSPOSE") == 0) {
               // to the transpose method
               transpose(arr1, arr2, len);
               if (flag == 1) {
                   roundMatrix(arr2);
               }
               strcpy(temp[i].data[2] , matrixToString(arr2, len, flag));
               printf("%s\n", temp[i].data[2]);
           }
       }





       strcpy(temp[i].data[0],"no input");

   freeMatrix(arr1, len);
   freeMatrix(arr2, len);
   freeMatrix(arr3, len);

    }

    sem_post(sem_writer);

    sem_close(sem_writer);
    sem_close(sem_reader);
    shmdt(temp);

    return 0;
}

char *matrixToString(double complex **matrix, int rows, int flag) {
int size = MAX_CHAR;
char *result = (char *)malloc(size * sizeof(char));
if (result == NULL) {
printf("Memory allocation failed\n");
exit(1);
}

// Start with the opening parenthesis and row, column values
sprintf(result, "(%d,%d:", rows, rows);

char buffer[MAX_CHAR];

for (int i = 0; i < rows; i++) {
for (int j = 0; j < rows; j++) {
if (flag == 0) { // Integer values
sprintf(buffer, "%d", (int)creal(matrix[i][j]));
} else if (flag == 1) { // Double values
sprintf(buffer, "%.1f", creal(matrix[i][j]));
} else if (flag == 2) { // Complex values
sprintf(buffer, "%d%+di", (int)creal(matrix[i][j]), (int)cimag(matrix[i][j]));
}
strcat(result, buffer);
if (i != rows - 1 || j != rows - 1) {
strcat(result, ",");
}
}
}

// Add the closing parenthesis
strcat(result, ")");

return result;
}


void swapToMatrix(const char *input, double complex ***matrix, int *rows, int type) {
int i, j;
char *token;
char *inputCopy = strdup(input); // Make a copy of the input string to avoid modifying the original
char *endPtr; // Pointer for strtol and strtod

// Remove the parentheses from the input string
if (inputCopy[0] == '(') {
memmove(inputCopy, inputCopy + 1, strlen(inputCopy));
}
if (inputCopy[strlen(inputCopy) - 1] == ')') {
inputCopy[strlen(inputCopy) - 1] = '\0';
}

// Extract the number of rows and columns
token = strtok(inputCopy, ",:");
*rows = (int)strtol(token, &endPtr, 10);
if (*endPtr != '\0') {
fprintf(stderr, "ERR\n");
free(inputCopy);
return;
}

token = strtok(NULL, ",:");
int columns = (int)strtol(token, &endPtr, 10);
if (*endPtr != '\0') {
fprintf(stderr, "ERR\n");
free(inputCopy);
return;
}

// Allocate memory for the matrix
*matrix = (double complex **)malloc(*rows * sizeof(double complex *));
for (i = 0; i < *rows; i++) {
(*matrix)[i] = (double complex *)malloc(columns * sizeof(double complex));
}

// Parse the matrix values
for (i = 0; i < *rows; i++) {
for (j = 0; j < columns; j++) {
token = strtok(NULL, ",:");
if (token == NULL) {
fprintf(stderr, "ERR\n");
free(inputCopy);
return;
}

if (type == 0) { // Integer values
(*matrix)[i][j] = (double complex)strtol(token, &endPtr, 10);
} else if (type == 1) { // Double values
(*matrix)[i][j] = (double complex)strtod(token, &endPtr);
} else if (type == 2) { // Complex values
double realPart = 0.0;
double imagPart = 0.0;

// Check if the value contains 'i'
char *imagPtr = strchr(token, 'i');
if (imagPtr != NULL) {
// Parse imaginary part
if (!strchr(token,'+')&&!strchr(token,'-')) {
// Handle cases like "1i"
imagPart = strtod(token, &endPtr);
} else {
// Handle cases like "1+1i" or "1-1i"
realPart = strtod(token, &endPtr);
if (*endPtr == '+' || *endPtr == '-') {
imagPart = strtod(endPtr, &endPtr);
}
}
(*matrix)[i][j] = realPart + imagPart * I;
} else {
// Parse real part
(*matrix)[i][j] = strtod(token, &endPtr);
}

if (*endPtr != '\0' && *endPtr != 'i' && *endPtr != ',' && *endPtr != ':') {
fprintf(stderr, "ERR\n");
free(inputCopy);
return;
}
}
}
}

free(inputCopy);
}


/*void printMatrix(double complex **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            if (cimag(matrix[i][j]) == 0) {
                printf("%.0f ", creal(matrix[i][j]));
            } else {
                printf("%.0f%+.0fi ", creal(matrix[i][j]), cimag(matrix[i][j]));
            }
        }
        printf("\n");
    }
}*/

void freeMatrix(double complex **matrix, int rows) {
for (int i = 0; i < rows; i++) {
free(matrix[i]);
}
free(matrix);
}

void add(double complex **arr1, double complex **arr2, double complex **arr3) {
for (int i = 0; i < len; ++i)
for (int j = 0; j < len; ++j)
arr3[i][j] =  arr2[i][j]+arr1[i][j] ;
}

void sub(double complex **arr1, double complex **arr2, double complex **arr3) {
for (int i = 0; i < len; ++i)
for (int j = 0; j < len; ++j)
arr3[i][j] = arr1[i][j] - arr2[i][j];
}

void multiply(double complex **arr1, double complex **arr2, double complex **arr3, int len) {
for (int i = 0; i < len; i++) {
for (int j = 0; j < len; j++) {
arr3[i][j] = 0;
for (int k = 0; k < len; k++) {
arr3[i][j] += arr1[i][k] * arr2[k][j];
}
}
}
}

double complex **allocateMatrix(int rows) {
    double complex **matrix = (double complex **)malloc(rows * sizeof(double complex *));
    if (matrix == NULL) {
        fprintf(stderr, "Memory allocation failed for matrix rows\n");
        exit(1);
    }
    for (int i = 0; i < rows; i++) {
        matrix[i] = (double complex *)malloc(rows * sizeof(double complex));
        if (matrix[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for matrix columns\n");
            for (int j = 0; j < i; j++) {
                free(matrix[j]);
            }
            free(matrix);
            exit(1);
        }
    }
    return matrix;
}

void ANDFunction(double complex **mat1, double complex **mat2, double complex **result, int rows) {
for (int i = 0; i < rows; i++) {
for (int j = 0; j < rows; j++) {
result[i][j] = mat1[i][j] && mat2[i][j];
}
}
}

void ORFunction(double complex **mat1, double complex **mat2, double complex **result, int rows) {
for (int i = 0; i < rows; i++) {
for (int j = 0; j < rows; j++) {
result[i][j] = mat1[i][j] || mat2[i][j];
}
}
}

void transpose(double complex **matrix, double complex **result, int rows) {
for (int i = 0; i < rows; i++) {
for (int j = 0; j < rows; j++) {
result[j][i] = matrix[i][j];
}
}
}

int isBinaryMatrix(double complex **matrix, int rows) {
for (int i = 0; i < rows; i++) {
for (int j = 0; j < rows; j++) {
if (matrix[i][j] != 0 && matrix[i][j] != 1) {
//printf("ERR\n");
return 0;
}
}
}
return 1;
}

int matrixType(char *string) {
    for (int i = 0; i < MAX_CHAR; ++i) {
        if (string[i] == '.') {
            return 1;
        } else if (string[i] == 'i') {
            return 2;
        }
    }
    return 0;
}
double roundNum(double num){
    return ((int )(num *10+.5))/10.0;
}

void roundMatrix(double complex ** arr){
for (int i = 0; i < len; ++i) {
for (int j = 0; j < len; ++j) {
arr[i][j] = roundNum(arr[i][j]);
}
}

}
void printMatrix(double complex **matrix, int rows) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < rows; j++) {
            if (cimag(matrix[i][j]) == 0) {
                printf("%.0f ", creal(matrix[i][j]));
            } else {
                printf("%.0f%+.0fi ", creal(matrix[i][j]), cimag(matrix[i][j]));
            }
        }
        printf("\n");
    }
}

