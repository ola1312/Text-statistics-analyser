#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_SIZE 10
#define MAX_LENGTH 50

struct word
{
  char text[MAX_LENGTH];
  size_t count;
};

void to_lower(char *string)
{
  //zamiana wielkich liter na małe
  for (int i=0; i<strlen(string); i++)
  {
    if(string[i]>='A' && string[i]<='Z')
      string[i] += 32;
  }
}

void trim (char *string)
{
  //Iteruj po napisie od końca i wstawiaj \0 jeśli nie ma litery. Przerwij jeśli napotkasz na pierwszą literę
  for(int i =strlen(string)-1; i>0; i--)
    if( (string[i]<'a' || string[i]>'z') && (string[i]<'A' || string[i]>'Z'))
      string[i] = '\0';
    else break;

  //Iteruj napis od początku i przesuń łańcuch o jeden w lewo
  if( (string[0]<'a' || string[0]>'z') && (string[0]<'A' || string[0]>'Z'))
  {
    for(int i = 0; i<strlen(string); i++)
      string[i] = string[i+1];
  }
}

//funkcja sprawdzająca czy dane słowo jest już w tablicy. Jeśli słowo jest to inkrementuje count i zwraca jego wartość. Jeśli nie ma to zwraca 0.
size_t count_word(struct word *document, size_t n, char *string)
{
  for(int i=0; i<n; i++)
  {
    if( strcmp((document+i)->text, string)==0)
    {
      (document+i)->count +=1;
      return (document+i)->count;
    }
  }
  return 0;
}

struct word *read_file (FILE *stream, size_t *n, size_t *total)
{
  size_t size = INIT_SIZE;
  struct word *document = (struct word*) calloc (sizeof(struct word), size);
  struct word buffer = {"", 1};

  if(!document)
  {
    fputs("Blad alokacji pamieci wewnatrz funkcji read_file", stderr);
    exit (1);
  }
  int i = 0; //licznik zapisanych do tablicy unikalnych słów

  while (!feof (stream)) //pętla wczytująca linia po linii aż do końca pliku
  {
    if(i >= size) //powiększenie tablicy jeśli nie ma miejsca
    {
      size = size + INIT_SIZE;
      document = (struct word*) realloc (document, size * sizeof(struct word));
    }
    fscanf(stream, "%s\n", buffer.text);
    trim(buffer.text);
    to_lower(buffer.text);

    if( count_word(document, size, buffer.text)==0)
    {
      document[i]=buffer;
      i++;
    }
  }

  if(size>i)
  {
    size = i; //zmniejszanie tablicy
    document = (struct word*) realloc (document, size * sizeof(struct word));
  }

  *n = size;

  for(int j=0; j<size; j++) //sumuje po liczbie zliczeń każdego unikalnego słowa
    *total += document[j].count;

  return document;
}

int main(void)
{
  size_t unique = 0;
  size_t total = 0;
  struct word *document = read_file (stdin, &unique, &total);
  fprintf(stdout, "Wczytano łącznie %ld słów, z czego %ld unikalnych\nLista słów:\n", total, unique);

  for(int i=0; i<unique; i++)
    fprintf(stdout, "%s %ld\n", document[i].text, document[i].count);

  free(document);
  return 0;
}