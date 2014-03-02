
#include "modp_json.h"
#include "modp_messagepack.h"

#include <time.h>
#ifndef CLOCKS_PER_SEC
# ifdef CLK_TCK
#  define CLOCKS_PER_SEC (CLK_TCK)
# endif
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

size_t test_json_encode(char* dest)
{
  modp_json_ctx ctx;
  modp_json_init(&ctx, dest);
  modp_json_map_open(&ctx);

  modp_json_add_cstring(&ctx, "start_ms");
  modp_json_add_uint32(&ctx, 123456789);

  modp_json_add_cstring(&ctx, "remote_ip");
  modp_json_add_cstring(&ctx, "123.123.123.13");

  modp_json_add_cstring(&ctx, "request");
  modp_json_add_cstring(&ctx, "GET /foobar HTTP/1.1");

  modp_json_add_cstring(&ctx, "headers_in");
  modp_json_ary_open(&ctx);

  modp_json_ary_open(&ctx);
  modp_json_add_cstring(&ctx, "Accept");
  modp_json_add_cstring(&ctx, "*/*");
  modp_json_ary_close(&ctx);

  modp_json_ary_open(&ctx);
  modp_json_add_cstring(&ctx, "Content-type");
  modp_json_add_cstring(&ctx, "text/plain");
  modp_json_ary_close(&ctx);

  modp_json_ary_open(&ctx);
  modp_json_add_cstring(&ctx, "Connection");
  modp_json_add_cstring(&ctx, "close");
  modp_json_ary_close(&ctx);

  modp_json_ary_open(&ctx);
  modp_json_add_cstring(&ctx, "User-agent");
  modp_json_add_cstring(&ctx, "Mozilla/5.0 (iPad; U; CPU OS 3_2_1 like Mac OS X; en-us) AppleWebKit/531.21.10 (KHTML, like Gecko) Mobile/7B405");
  modp_json_ary_close(&ctx);

  modp_json_ary_close(&ctx);
  modp_json_map_close(&ctx);
  return modp_json_end(&ctx);
}

size_t test_msgpk_encode(char* dest)
{
  modp_msgpk_ctx ctx;
  modp_msgpk_init(&ctx, dest);
  modp_msgpk_map_open(&ctx, 4);

  modp_msgpk_add_cstring(&ctx, "start_ms");
  modp_msgpk_add_uint32(&ctx, 123456789);

  modp_msgpk_add_cstring(&ctx, "remote_ip");
  modp_msgpk_add_cstring(&ctx, "123.123.123.13");

  modp_msgpk_add_cstring(&ctx, "request");
  modp_msgpk_add_cstring(&ctx, "GET /foobar HTTP/1.1");

  modp_msgpk_add_cstring(&ctx, "headers_in");
  modp_msgpk_ary_open(&ctx, 3);

  modp_msgpk_ary_open(&ctx, 2);
  modp_msgpk_add_cstring(&ctx, "Accept");
  modp_msgpk_add_cstring(&ctx, "*/*");
  modp_msgpk_ary_close(&ctx);

  modp_msgpk_ary_open(&ctx, 2);
  modp_msgpk_add_cstring(&ctx, "Content-type");
  modp_msgpk_add_cstring(&ctx, "text/plain");
  modp_msgpk_ary_close(&ctx);

  modp_msgpk_ary_open(&ctx, 2);
  modp_msgpk_add_cstring(&ctx, "Connection");
  modp_msgpk_add_cstring(&ctx, "close");
  modp_msgpk_ary_close(&ctx);

  modp_msgpk_ary_open(&ctx, 2);
  modp_msgpk_add_cstring(&ctx, "User-agent");
  modp_msgpk_add_cstring(&ctx, "Mozilla/5.0 (iPad; U; CPU OS 3_2_1 like Mac OS X; en-us) AppleWebKit/531.21.10 (KHTML, like Gecko) Mobile/7B405");
  modp_msgpk_ary_close(&ctx);

  modp_msgpk_ary_close(&ctx);
  modp_msgpk_map_close(&ctx);
  return modp_msgpk_end(&ctx);
}

int main(void)
{

  const int imax = 1000000;
  clock_t t0, t1;
  int i;
  double last = 0.0;
  double s1;
  size_t len;
  //  char* buf;
  char buf2[512];

  printf("ALG\tEncodes/Sec\tBYTES\n");
  fflush(stdout);
  t0 = clock();
  for (i = 0; i < imax; ++i) {
    len = test_json_encode(NULL);
    //buf = malloc(len);
    len = test_json_encode(buf2);
    //free(buf);
  }
  t1 = clock();
  last = (double)(t1 -t0);
  s1 = (double)(t1 - t0)*(1.0 / (double)CLOCKS_PER_SEC);
  printf("%s\t%8.0f\t%lu\n", "JSON", imax/s1, len);
  fflush(stdout);

  t0 = clock();
  for (i = 0; i < imax; ++i) {
    len = test_msgpk_encode(NULL);
    //buf = malloc(len);
    len = test_msgpk_encode(buf2);
    //free(buf);
  }
  t1 = clock();
  s1 = (double)(t1 - t0)*(1.0 / (double)CLOCKS_PER_SEC);
  printf("%s\t%8.0f\t%lu\n", "MSGPK", imax/s1, len);
  fflush(stdout);
  return 0;
}
