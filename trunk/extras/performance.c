
#include "b64.h"
#include "jabber64.h"
#include "apache64.h"

#include <time.h>
#ifndef CLOCKS_PER_SEC
# ifdef CLK_TCK
#  define CLOCKS_PER_SEC (CLK_TCK)
# endif
#endif  
#include <stdio.h>
#include <string.h>

#define SZ 256
int main() {
  int i;
  clock_t c0, c1;
  char teststr1[SZ];
  for (i = 0; i < (int)sizeof(teststr1); ++i) {
    teststr1[i] = (char) i;
  }
  char result[SZ*2];
  char result2[SZ*2];
  
  const int MAX = 1000000;

  c0 = clock();
  for (i = 0; i < MAX; ++i) {
    b64_encode64_1(result, teststr1, sizeof(teststr1));
  }
  c1 = clock();	
  printf("Encode 64-1: TIME: %8.4fs\n",  (c1 - c0)*(1.0 / (double)CLOCKS_PER_SEC));

  c0 = clock();
  for (i = 0; i < MAX; ++i) {
    b64_encode64_2(result, teststr1, sizeof(teststr1));
  }
  c1 = clock();	
  printf("Encode 64-2: TIME: %8.4fs\n",  (c1 - c0)*(1.0 / (double)CLOCKS_PER_SEC));

  c0 = clock();
  for (i = 0; i < MAX; ++i) {
    b64_encode32_0(result, teststr1, sizeof(teststr1));
  }
  c1 = clock();	
  printf("Encode 32-0: TIME: %8.4fs\n",  (c1 - c0)*(1.0 / (double)CLOCKS_PER_SEC));

  c0 = clock();
  for (i = 0; i < MAX; ++i) {
    b64_encode32_1(result, teststr1, sizeof(teststr1));
  }
  c1 = clock();	
  printf("Encode 32-1: TIME: %8.4fs\n",  (c1 - c0)*(1.0 / (double)CLOCKS_PER_SEC));

  c0 = clock();
  for (i = 0; i < MAX; ++i) {
    b64_encode32_2(result, teststr1, sizeof(teststr1));
  }
  c1 = clock();	
  printf("Encode 32-2: TIME: %8.4fs\n",  (c1 - c0)*(1.0 / (double)CLOCKS_PER_SEC));


  c0 = clock();
  for (i = 0; i < MAX; ++i) {
    b64_encode32_3(result, teststr1, sizeof(teststr1));
  }
  c1 = clock();	
  printf("Encode 32-3: TIME: %8.4fs\n",  (c1 - c0)*(1.0 / (double)CLOCKS_PER_SEC));

  printf("DECODE\n");
  int len = strlen(result);
  c0 = clock();
  for (i = 0; i < MAX; ++i) {
    b64_decode64_1(result2, result, len);
  }
  c1 = clock();
  printf("Decode64_1: TIME: %8.4fs\n",  (c1 - c0)*(1.0 / (double)CLOCKS_PER_SEC));

  c0 = clock();
  for (i = 0; i < MAX; ++i) {
    b64_decode64_2(result2, result, len);
  }
  c1 = clock();
  printf("Decode64_2: TIME: %8.4fs\n",  (c1 - c0)*(1.0 / (double)CLOCKS_PER_SEC));

  c0 = clock();
  for (i = 0; i < MAX; ++i) {
    b64_decode64_3(result2, result, len);
  }
  c1 = clock();
  printf("Decode64_3: TIME: %8.4fs\n",  (c1 - c0)*(1.0 / (double)CLOCKS_PER_SEC));

  c0 = clock();
  for (i = 0; i < MAX; ++i) {
    b64_decode64_4(result2, result, len);
  }
  c1 = clock();
  printf("Decode64_4: TIME: %8.4fs\n",  (c1 - c0)*(1.0 / (double)CLOCKS_PER_SEC));


  c0 = clock();
  for (i = 0; i < MAX; ++i) {
    b64_decode64_5(result2, result, len);
  }
  c1 = clock();
  printf("Decode64_5: TIME: %8.4fs\n",  (c1 - c0)*(1.0 / (double)CLOCKS_PER_SEC));

  c0 = clock();
  for (i = 0; i < MAX; ++i) {
    b64_decode32_1(result2, result, len);
  }
  c1 = clock();
  printf("Decode32_1: TIME: %8.4fs\n",  (c1 - c0)*(1.0 / (double)CLOCKS_PER_SEC));

  c0 = clock();
  for (i = 0; i < MAX; ++i) {
    b64_decode32_2(result2, result, len);
  }
  c1 = clock();
  printf("Decode32_2: TIME: %8.4fs\n",  (c1 - c0)*(1.0 / (double)CLOCKS_PER_SEC));

  c0 = clock();
  for (i = 0; i < MAX; ++i) {
    b64_decode32_3(result2, result, len);
  }
  c1 = clock();
  printf("Decode32_3: TIME: %8.4fs\n",  (c1 - c0)*(1.0 / (double)CLOCKS_PER_SEC));


  c0 = clock();
  for (i = 0; i < MAX; ++i) {
    b64_decode32_4(result2, result, len);
  }
  c1 = clock();
  printf("Decode32_4: TIME: %8.4fs\n",  (c1 - c0)*(1.0 / (double)CLOCKS_PER_SEC));

  c0 = clock();
  for (i = 0; i < MAX; ++i) {
    b64_decode32_5(result2, result, len);
  }
  c1 = clock();
  printf("Decode32_5: TIME: %8.4fs\n",  (c1 - c0)*(1.0 / (double)CLOCKS_PER_SEC));

  //------------ Jabber encode
  c0 = clock();
  for (i = 0; i < MAX; ++i) {
    jabber64_encode((unsigned char*) teststr1, sizeof(teststr1), result, sizeof(result));
  }
  c1 = clock();
  printf("jabber Encode: TIME: %8.4fs\n",  (c1 - c0)*(1.0 / (double)CLOCKS_PER_SEC));
  //---------- jabber decode
  c0 = clock();
  for (i = 0; i < MAX; ++i) {
    jabber64_decode(result, (unsigned char*) result2,  sizeof(result2));
  }
  c1 = clock();
  printf("jabber decode: TIME: %8.4fs\n",  (c1 - c0)*(1.0 / (double)CLOCKS_PER_SEC));



  //------------ Apache encode
  c0 = clock();
  for (i = 0; i < MAX; ++i) {
    ap_base64encode(result, teststr1, sizeof(teststr1));
  }
  c1 = clock();
  printf("Apache Encode: TIME: %8.4fs\n",  (c1 - c0)*(1.0 / (double)CLOCKS_PER_SEC));
  //---------- Apache decode
  c0 = clock();
  for (i = 0; i < MAX; ++i) {
    ap_base64decode(result2, result, len);
  }
  c1 = clock();
  printf("apache decode: TIME: %8.4fs\n",  (c1 - c0)*(1.0 / (double)CLOCKS_PER_SEC));


  return 0;
}
