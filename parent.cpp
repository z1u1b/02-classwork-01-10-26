#include <unistd.h>
#include <sys/wait.h>
#include <cassert>
#include <cstdio>

const char msg[256] = "user data\n";

size_t send(int& err, int wr, const char* b, size_t k)
{
  size_t r = 0;
  while (r < k) {
    err = write(wr, b + r, k - r);
    if (err < 0) break;
    r += err;
  }
  return r;
}

int main() { // handle SIGPIPE...
  int pps[2] = {}, err = pipe(pps);              assert(!err);
  int rd = pps[0], wr = pps[1];
  pid_t pid = fork();                            assert(pid >= 0);
  if (!pid) {
    err = close(wr);                             assert(!err);
    char p[100] = {};
    err = sprintf(p, "%d", rd);                  assert(err > 0);
    execl("child", "child", p, NULL);            assert(0);
  }
  err = close(rd);                               assert(!err);
  send(err, wr, msg, 255);                       assert(err > 0);
  err = close(wr);                               assert(!err);
  err = waitpid(pid, 0, 0);                      assert(err == pid);
}
