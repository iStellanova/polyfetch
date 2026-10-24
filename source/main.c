#include "fetch.h"

int main(void) {
  int cOS = 1; //Config OS
  int cOSPretty = 1; //Config OS Pretty Name
  int nClosure = 1; //Nix closure size.
  if (cOS) os();
  if (cOSPretty) pretty();
  if (nClosure) nixClosureSize();
  return 0;
}
