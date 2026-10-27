#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Compute failure function f for pattern P
vector<int> compute_f(const string &P) {
  int m = P.length();
  vector<int> f(m, 0);
  int i = 1, j = 0;

  while (i < m) {
    if (P[i] == P[j]) {
      f[i] = j + 1;
      i++;
      j++;
    } else {
      if (j != 0)
        j = f[j - 1];
      else {
        f[i] = 0;
        i++;
      }
    }
  }
  return f;
}

// KMP string matching algorithm
vector<int> kmp_search(const string &T, const string &P, const vector<int> &f) {
  vector<int> matches;
  int n = T.length();
  int m = P.length();
  int x = 0; // pointer to pattern
  int y = 0; // pointer to text

  while (y < n) {
    if (P[x] == T[y]) {
      x++;
      y++;
    } else {
      if (x != 0)
        x = f[x - 1];
      else
        y++;
    }

    if (x == m) {
      matches.push_back(y - m);
      x = f[x - 1];
    }
  }
  return matches;
}

int main() {
  int N, M;
  cin >> N >> M;

  string S, P;
  cin >> S >> P;

  string T = S;
  if (M > 1)
    T += S.substr(0, M - 1);

  vector<int> f = compute_f(P);
  vector<int> matches = kmp_search(T, P, f);

  if (matches.empty())
    cout << 0 << "\n";
  else {
    cout << matches.size() << "\n";
    for (int i = 0; i < (int)matches.size(); ++i)
      cout << matches[i] << (i + 1 == (int)matches.size() ? "" : " ");
    cout << "\n";
  }

  return 0;
}
