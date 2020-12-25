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
  // char* mapped_array = (char*)(malloc(array_length*sizeof(char)));
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
  return (char)(fgetc(stdin));
}

/* Gets a char c,  and if the char is 'q' , ends the program with exit code 0. Otherwise returns c. */
char quit(char c){
  if (c == 'q'){
    exit(0);
  }
  return c;
}
 
int main(int argc, char **argv){

  /* TODO: Test your code */

  int base_len = 5;
  char arr1[base_len];
  map(arr1, base_len, my_get);
  map(arr1, base_len, dprt);
  map(arr1, base_len, encrypt);
  map(arr1, base_len, dprt);
  map(arr1, base_len, decrypt);
  map(arr1, base_len, cprt);

  return 0;
}