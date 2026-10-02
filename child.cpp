#include <unistd.h>
#include <sys/wait.h>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <string>

size_t recv(int& err, int rd, char* b, size_t k)
{
  size_t r = 0;
  while (r < k) {
    err = read(rd, b + r, k - r);
    if (err < 0)
      break;
    r += err;
  }
  return r;
}

int main(int argc, char** argv)
{
  assert(argc == 2);
  int err = 0;
  int rd = std::atoi(argv[1]);
  assert(rd > 0);

  char p[8]={};
  recv(err, rd, p, 8);
  assert(err > 0);
  int dt = std::atoi(p);
  assert(dt >= 0);

  std::string msg(dt,'\0');
  if (dt>0) {
    recv(err,rd,&msg[0],dt);
    assert(err > 0);
  }


  err = close(rd);
  assert(!err);
  err = printf("%s", msg.c_str());
  assert(err > 0);
}
