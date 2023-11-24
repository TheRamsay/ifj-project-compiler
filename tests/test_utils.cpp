#include <unistd.h>

#include <array>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <utility>

std::pair<char *, int> run_interpreter(const char *filePath) {
  std::array<char, 128> buffer;
  std::string result;

  std::string cmd = "../interpreter/ic23int " + std::string(filePath) + " 2>&1";

  FILE *pipe = popen(cmd.c_str(), "r");

  while (fgets(buffer.data(), 128, pipe) != NULL) {
    result += buffer.data();
  }

  int returnCode = pclose(pipe);

  char *output = (char *)malloc(result.length() + 1);
  strcpy(output, result.c_str());

  return std::make_pair(output, returnCode);
}