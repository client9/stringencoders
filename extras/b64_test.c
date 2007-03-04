#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "b64fast.h"
#include "apr_base64.h"


#define SZ 2560

int main() {
  
  int i,j;
  char result1[SZ * 2];
  char result2[SZ * 2];
  char result3[SZ * 2];
  char result4[SZ * 2];

  char teststr1[SZ];
  for (i =0; i < (int)sizeof(teststr1); ++i) {
    teststr1[i] = (char) i;
    if (teststr1[i] == '\0') {
      teststr1[i] == ' ';
    }
  }
  int x,y;
  int m = 2 > sizeof(teststr1) ?  sizeof(teststr1) : 2;
  for (j = 0; j <= m; ++j) {
    printf("encode32_0: %d\n", j);
    x = apr_base64_encode_binary(result1, teststr1, sizeof(teststr1)-j);
    y = b64_encode32_0(result2, teststr1, sizeof(teststr1)-j);
    /*
    if (x != y) {
      printf("Error %d %d\n", x,y);
      return -1;
    }
    
    if (x != strlen(result1)) {
      printf("Error in apache %d %d\n", x, strlen(result1));
      return -1;
    }
    */
    if (y != strlen(result2)) {
      printf("Error in b64 %d %d\n", x, strlen(result2));
      return -1;
    }

    if (strcmp(result1, result2) != 0) {
      printf("Error %d\n", j);
      printf("r1: %d %s\n", strlen(result1), result1);
      printf("r2: %d %s\n", strlen(result2), result2);
      return -1;
    }
  }

  for (j = 0; j <= m; ++j) {
    printf("encode32_1: %d\n", j);
    ap_base64encode(result1, teststr1, sizeof(teststr1)-j);
    y = b64_encode32_1(result2, teststr1, sizeof(teststr1)-j);
    if (y != strlen(result2)) {
      printf("mismatch\n");
      return -1;
    }
    if (strcmp(result1, result2) != 0) {
      printf("Error %d\n", j);
      printf("r1: %s\n", result1);
      printf("r2: %s\n", result2);
      return -1;
    }
  }

  for (j = 0; j <= m; ++j) {
    printf("encode32_2: %d\n", j);
    ap_base64encode(result1, teststr1, sizeof(teststr1)-j);
    y = b64_encode32_2(result2, teststr1, sizeof(teststr1)-j);
    if (y != strlen(result2)) {
      printf("mismatch\n");
      return -1;
    }
    if (strcmp(result1, result2) != 0) {
      printf("Error %d\n", j);
      printf("r1: %s\n", result1);
      printf("r2: %s\n", result2);
      return -1;
    }
  }
  m = 7 >  sizeof(teststr1) ? sizeof(teststr1) : 7;
  for (j = 0; j <= m; ++j) {
    printf("encode64_1: %d\n", j);
    ap_base64encode(result1, teststr1, sizeof(teststr1)-j);
    y = b64_encode64_1(result2, teststr1, sizeof(teststr1)-j);
    if (y != strlen(result2)) {
      printf("mismatch: %d %d\n", y, strlen(result2));
      return -1;
    }
    if (strcmp(result1, result2) != 0) {
      printf("Error %d\n", j);
      printf("r1: %d %s\n", strlen(result1), result1);
      printf("r2: %d %s\n", strlen(result2), result2);
      return -1;
    }
  }

  for (j = 0; j <= m; ++j) {
    printf("encode64_2: %d\n", j);
    ap_base64encode(result1, teststr1, sizeof(teststr1)-j);
    y = b64_encode64_2(result2, teststr1, sizeof(teststr1)-j);
    if (y != strlen(result2)) {
      printf("mismatch\n");
      return -1;
    }
    if (strcmp(result1, result2) != 0) {
      printf("Error %d\n", j);
      printf("r1: %d: %s\n", strlen(result1), result1);
      printf("r2: %d: %s\n", strlen(result2), result2);
      return -1;
    }
  }

  //
  // DECODE
  //
  for (j = 0; j <= m; ++j) {
    ap_base64encode(result1, teststr1, sizeof(teststr1)-j);
    ap_base64decode(result3, result1, strlen(result1));

    printf("b64decode32_1: %d\n", j);
    y = b64_decode32_1(result4, result1, strlen(result1));
    if (y != sizeof(teststr1) -j) {
      printf("Mismatch %d %d\n", y, sizeof(teststr1)-j);
      return -1;
    }
    if (strcmp(result3, result4) != 0) {
      printf("Error %d\n", j);
      printf("r1: %d %s\n", strlen(result3), result3);
      printf("r2: %d %s\n", strlen(result4), result4);
      return -1;
    }

    printf("b64decode32_2: %d\n", j);
    y = b64_decode32_2(result4, result1, strlen(result1));
    if (y != sizeof(teststr1) -j) {
      printf("Mismatch %d %d\n", y, sizeof(teststr1)-j);
      return -1;
    }
    if (strcmp(result3, result4) != 0) {
      printf("Error %d\n", j);
      printf("r1: %s\n", result3);
      printf("r2: %s\n", result4);
      return -1;
    }


    printf("b64decode32_3: %d\n", j);
    y = b64_decode32_3(result4, result1, strlen(result1));
    if (y != sizeof(teststr1) -j) {
      printf("Mismatch %d %d\n", y, sizeof(teststr1)-j);
      return -1;
    }
    if (strcmp(result3, result4) != 0) {
      printf("Error %d\n", j);
      printf("r1: %s\n", result3);
      printf("r2: %s\n", result4);
      return -1;
    }

    printf("b64decode32_4: %d\n", j);
    y = b64_decode32_4(result4, result1, strlen(result1));
    if (y != sizeof(teststr1) -j) {
      printf("Mismatch %d %d\n", y, sizeof(teststr1)-j);
      return -1;
    }
    if (strcmp(result3, result4) != 0) {
      printf("Error %d\n", j);
      printf("r1: %s\n", result3);
      printf("r2: %s\n", result4);
      return -1;
    }
    
    //
    // 64-bit decode versions
    //
    //
    printf("b64decode64_1: %d\n", j);
    y = b64_decode64_1(result4, result1, strlen(result1));
    if (y != sizeof(teststr1) -j) {
      printf("Mismatch %d %d\n", y, sizeof(teststr1)-j);
      return -1;
    }
    if (strcmp(result3, result4) != 0) {
      printf("Error %d\n", j);
      printf("r1: %d %s\n", strlen(result3), result3);
      printf("r2: %d %s\n", strlen(result4), result4);
      return -1;
    }

    printf("b64decode64_2: %d\n", j);
    y = b64_decode64_2(result4, result1, strlen(result1));
    if (y != sizeof(teststr1) -j) {
      printf("Mismatch %d %d\n", y, sizeof(teststr1)-j);
      return -1;
    }
    if (strcmp(result3, result4) != 0) {
      printf("Error %d\n", j);
      printf("r1: %s\n", result1);
      printf("r2: %s\n", result2);
      return -1;
    }

    printf("b64decode64_3: %d\n", j);
    y = b64_decode64_3(result4, result1, strlen(result1));
    if (y != sizeof(teststr1) -j) {
      printf("Mismatch %d %d\n", y, sizeof(teststr1)-j);
      return -1;
    }
    if (strcmp(result3, result4) != 0) {
      printf("Error %d\n", j);
      printf("r1: %s\n", result1);
      printf("r2: %s\n", result2);
      return -1;
    }

    printf("b64decode64_4: %d\n", j);
    y = b64_decode64_4(result4, result1, strlen(result1));
    if (y != sizeof(teststr1) -j) {
      printf("Mismatch %d %d\n", y, sizeof(teststr1)-j);
      return -1;
    }
    if (strcmp(result3, result4) != 0) {
      printf("Error %d\n", j);
      printf("r1: %d %s\n", strlen(result3), result3);
      printf("r2: %d %s\n", strlen(result4), result4);
    return -1;
    }
  }
}

	       
