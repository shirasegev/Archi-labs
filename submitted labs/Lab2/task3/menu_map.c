#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
char censor(char c) {
  if(c == '!')
    return '.';
  else
    return c;
}
 
void map(char *array, int array_length, char (*f) (char)){
  /* TODO: Complete during task 2.a */
  for (int i=0; i< array_length; i++){
      array[i] = f(array[i]);
  }
}

/* Gets a char c and returns its encrypted form by adding 3 to its value. 
If c is not between 0x20 and 0x7E it is returned unchanged */
char encrypt(char c){
  if (c >= 0x20 && c<= 0x7E){
    return c + 3;
  }
  return c;
}

/* Gets a char c and returns its decrypted form by reducing 3 to its value.
If c is not between 0x20 and 0x7E it is returned unchanged */
char decrypt(char c){
  if (c >= 0x20 && c<= 0x7E){
    return c-3;
  }
  return c;
}

/* dprt prints the value of c in a decimal representation followed by a 
new line, and returns c unchanged. */
char dprt(char c){
  printf("%d\n", c);
  return c;
}

/* If c is a number between 0x20 and 0x7E, cprt prints the character of ASCII value c followed 
by a new line. Otherwise, cprt prints the dot ('.') character. After printing, cprt returns 
the value of c unchanged. */
char cprt(char c){
  if (c >= 0x20 && c<= 0x7E){
    printf("%c\n", c);
  }
  else {
    printf(".\n");
  }
  return c;
}

/* Ignores c, reads and returns a character from stdin using fgetc. */
char my_get(char c){
  return (char)fgetc(stdin);
}

/* Gets a char c,  and if the char is 'q' , ends the program with exit code 0. Otherwise returns c. */
char quit(char c){
  if (c == 'q'){
    exit(0);
  }
  return c;
}

struct fun_desc {
  char *name;
  char (*fun)(char);
};
 
int main(int argc, char **argv){

  char* carray = (char*)calloc(5, sizeof(char));
  // char* carray = (char*)(malloc(5 * sizeof(char)));
  struct fun_desc menu[] = { {"Censor", censor}, {"Encrypt", encrypt}, {"Decrypt", decrypt},
  {"Print dec", dprt}, {"Print string", cprt}, {"Get string", my_get}, {"Quit", quit}, {NULL, NULL} };

  int length = 0;
  while (menu[length].name != NULL){
    length++;
  }

  while (1) {
    printf("Please choose a function:\n");
    for (int j = 0; j < length; j++){
      printf("%d) %s\n", j, menu[j].name);
    }
    printf("Opthion: ");
    int choice;
    char tmp;
    scanf("%d%c", &choice, &tmp);
    if (choice >= 0 && choice < length) {
      printf("Within bounds\n");

      map(carray, 5, menu[choice].fun);
      printf("DONE.\n\n");
    }
    else {
      printf("Not within bounds\n");
      exit(0);
    }
  }

  return 0;
}