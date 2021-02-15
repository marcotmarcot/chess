#include "color.h"

Color Other(Color color) {
  if (color == kWhite) {
    return kBlack;
  }
  return kWhite;
}
