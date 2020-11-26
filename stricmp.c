static int stricmp(const char* s1, const char* s2) {
  char c1, c2;
  do {
    c1 = *s1++;
    c2 = *s2++;
    if (c1 >= 'A' && c1 <= 'Z') {
      c1 += 0x20;
    }
    if (c2 >= 'A' && c2 <= 'Z') {
      c2 += 0x20;
    }
  } while (c1 && (c1 == c2));
  return (c1 - c2);
}