//CS 4504 - Section 03
//Fall 2022
//Name: Khoa Ho
//KSU id: dho5

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 10240
#define NUM_THREADS 10

int n1, n2;
char *s1, *s2;
FILE *fp;
int countArray[NUM_THREADS] = {0};
int total_substring = 0;
static pthread_mutex_t mlock;

// read input file and generate string s1/s2 and length n1/n2
int readf(FILE *fp) {
  if ((fp = fopen("strings.txt", "r")) == NULL) {
    printf("ERROR: can't open string.txt!\n");
    return 0;
  }
  s1 = (char *)malloc(sizeof(char) * MAX);
  if (s1 == NULL) {
    printf("ERROR: Out of memory!\n");
    return -1;
  }
  s2 = (char *)malloc(sizeof(char) * MAX);
  if (s1 == NULL) {
    printf("ERROR: Out of memory\n");
    return -1;
  }
  /*read s1 s2 from the file*/
  s1 = fgets(s1, MAX, fp);
  s2 = fgets(s2, MAX, fp);
  n1 = strlen(s1);     /*length of s1*/
  n2 = strlen(s2) - 1; /*length of s2*/

  if (s1 == NULL || s2 == NULL || n1 < n2) /*when error exit*/
    return -1;
  return 0;
}

int num_substring(int t) {
  // add your logic here
  // 1, how to distribute different parts of string s1 into different threads
  int i, j, k, count;
  int total = 0;
  char ss[n1 / NUM_THREADS];
  strncpy(ss, s1 + t * (n1 / NUM_THREADS), n1 / NUM_THREADS);
  for (i = 0; i <= (strlen(ss) - n2); i++) {
    count = 0;
    for (j = i, k = 0; k < n2; j++, k++) {
      if (*(ss + j) != *(s2 + k)) {
        break;
      } else {
        count++;
      }
      if (count == n2) {
        total++;
      }
    }
  }

  return total;
}

void *calSubStringThread(void *threadid) {
  long tid = (long)threadid;
  pthread_mutex_lock(&mlock);
  printf("This is thread %ld, ", tid);
  // 2, how to sum up the total number of substring from all threads
  int num = num_substring(tid);
  total_substring += num;
  printf("find num of is: %d\n", num);
  pthread_mutex_unlock(&mlock);
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t threads[NUM_THREADS];
  int t, rc;
  int totalNum = 0;

  readf(fp);
  if (pthread_mutex_init(&mlock, NULL) != 0) {
    printf("Mutex init failed\n");
    return 1;
  }

  for (t = 0; t < NUM_THREADS; t++) {
    rc = pthread_create(&threads[t], NULL, calSubStringThread,
                        (void *)(size_t)t);
    if (rc) {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }

  for (t = 0; t < NUM_THREADS; t++) {
    pthread_join(threads[t], NULL);
  }
  totalNum = total_substring;

  printf("The number of substrings is: %d\n", totalNum);
  pthread_mutex_destroy(&mlock);
  return 1;
}
