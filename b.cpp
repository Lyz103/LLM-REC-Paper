#include <bits/stdc++.h>

int f1() { return (rand() % 5) + 1; }

int f2() {
  int ans = -1;
  do {
    ans = f1();
  } while (ans == 3);

  return ans <= 2 ? 1 : 0;
}

int f3() {
  std::vector<int> a(3);
  int ans = 0;
  do {
    ans = 0;
    for (int i = 0; i < 3; i++) {
      a[i] = f2();
    }
    for (int i = 0; i < 3; i++) {
      ans += a[i] << i;
    }
  } while (ans > 6);
  return ans + 1;
}

int main() {
  srand(time(0));
  int one = 0, zero = 0;
  std::cout << f3() << "\n";
  return 0;
}


