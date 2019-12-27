#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/queue.h>
#include <pthread.h>


FILE* fp;
static LIST_HEAD(listhead, entry) head;
const int k_num_thread = 4;
struct listhead *headp = NULL;
int num_entries = 0;
int max_frequency = 0;
struct entry {
  char name[BUFSIZ];
  int frequency;
  LIST_ENTRY(entry) entries;
};
char buf[4096];

pthread_mutex_t mutex_lock;

void *tolower_funtion(void *argv){
  int line_length = strlen(buf);
    for (int it = 0; it < line_length; ++it) {
      if (!isalnum(buf[it])) {
        buf[it] = ' ';
      } else {
        buf[it] = tolower(buf[it]);
      }
    }
    return NULL;
}
//Tokenization
void *Tokenization(void * argv)
{
  
  pthread_mutex_lock(&mutex_lock);
  while (fgets(buf, 4096, fp)){

    tolower_funtion(buf);
    
    const char* WHITE_SPACE =" \t\n";
    char* tok = strtok(buf, WHITE_SPACE);
    
    if(tok == NULL || strcmp(tok, "") == 0){
        continue;
      }
    
    do {

      if (num_entries == 0) {
        struct entry* e = malloc(sizeof(struct entry));

        strncpy(e->name, tok, strlen(tok));
        e->frequency = 1;

        LIST_INSERT_HEAD(&head, e, entries);
        num_entries++;
        continue;
      } else if (num_entries == 1) {
        int cmp = strcmp(tok, head.lh_first->name);

        if (cmp == 0) {
          head.lh_first->frequency++;
        } else if (cmp > 0) {
          struct entry* e = malloc(sizeof(struct entry));

          strncpy(e->name, tok, strlen(tok));
          e->frequency = 1;


          LIST_INSERT_AFTER(head.lh_first, e, entries);
          num_entries++;
        } else if (cmp < 0) {
          struct entry* e = malloc(sizeof(struct entry));

          strncpy(e->name, tok, strlen(tok));
          e->frequency = 1;

          LIST_INSERT_HEAD(&head, e, entries);
          num_entries++;
        }

        continue;
      }
      // Reduce: actual word-counting
      struct entry* np = head.lh_first;
      struct entry* final_np = NULL;

      int last_cmp = strcmp(tok, np->name);

      if (last_cmp < 0) {
        struct entry* e = malloc(sizeof(struct entry));

        strncpy(e->name, tok, strlen(tok));
        e->frequency = 1;

        LIST_INSERT_HEAD(&head, e, entries);
        num_entries++;

        continue;

      } else if (last_cmp == 0) {
        np->frequency++;

        continue;
      }

      for (np = np->entries.le_next; np != NULL; np = np->entries.le_next) {
        int cmp = strcmp(tok, np->name);

        if (cmp == 0) {
          np->frequency++;
          break;
        } else if (last_cmp * cmp < 0) { // sign-crossing occurred
          struct entry* e = malloc(sizeof(struct entry));

          strncpy(e->name, tok, strlen(tok));
          e->frequency = 1;

          LIST_INSERT_BEFORE(np, e, entries);
          num_entries++;

          break;
        }

        if (np->entries.le_next == NULL) {
          final_np = np;
        } else {
          last_cmp = cmp;
        }
      }

      if (!np && final_np) {
        struct entry* e = malloc(sizeof(struct entry));

        strncpy(e->name, tok, strlen(tok));
        e->frequency = 1;

        LIST_INSERT_AFTER(final_np, e, entries);
        num_entries++;
      }

    } while (tok = strtok(NULL, WHITE_SPACE));
  }
    pthread_mutex_unlock(&mutex_lock);
    
    return NULL;
}


void *count(void *buf)
{
 // pthread_mutex_lock(&mutex_lock);
  for (struct entry* np = head.lh_first; np != NULL; np = np->entries.le_next) {
    if (max_frequency < np->frequency) {
      max_frequency = np->frequency;
    }
  }
 // pthread_mutex_unlock(&mutex_lock);
  return NULL;
}

//단순출력
void Print(void *buf)
{
  for (int it = max_frequency; it > 0; --it) {
    for (struct entry* np = head.lh_first; np != NULL; np = np->entries.le_next) {
      if (np->frequency == it) {
        printf("%s %d\n", np->name, np->frequency);
      }
    }
  }
}


int main(int argc, char** argv)
{
  pthread_t thread1[3],thread2[3],thread3[3];
  pthread_mutex_init(&mutex_lock, NULL);
  int status;
  if (argc != 2) {
    fprintf(stderr, "%s: not enough input\n", argv[0]);
    exit(1);
  }

   fp = fopen(argv[1], "r");
   

   LIST_INIT(&head);

   for(int i=0;i<3;++i){
 pthread_create(&thread1[i], NULL, tolower_funtion, (void *)buf);
  }
  for(int i=0;i<3;++i){
  pthread_join(thread1[i], (void *)&status);
  }
   for(int i=0;i<3;++i){
   pthread_create(&thread2[i], NULL, Tokenization, (void *)buf);
  }

    for(int i=0;i<3;++i){
    pthread_join(thread2[i], (void *)&status);
  }

  for(int i=0; i<3; ++i){
   pthread_create(&thread3[i], NULL, count, (void *)buf);
  }

  for(int i=0; i <3; ++i){
   pthread_join(thread3[i], (void *)&status);
  }
  
  Print(buf);

  // Release
  while (head.lh_first != NULL) {
    LIST_REMOVE(head.lh_first, entries);
  }

  fclose(fp);
  pthread_exit(NULL);
  return 0;
}
