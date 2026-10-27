#include <cmath>
#include <complex>
#include <iostream>
#include <vector>

using namespace std;

using cd = complex<double>;
const double PI = acos(-1.0);

// Recursive-FFT algorithm
vector<cd> recursive_fft(const vector<cd> &a, bool invert) {
  int n = a.size();
  if (n == 1)
    return a;

  vector<cd> ae(n / 2), ao(n / 2);
  for (int i = 0; 2 * i < n; ++i) {
    ae[i] = a[2 * i];
    ao[i] = a[2 * i + 1];
  }

  vector<cd> ye = recursive_fft(ae, invert);
  vector<cd> yo = recursive_fft(ao, invert);

  vector<cd> y(n);
  double ang = 2 * PI / n * (invert ? -1 : 1);
  cd wn(cos(ang), sin(ang));
  cd w(1);

  for (int k = 0; k < n / 2; ++k) {
    y[k] = ye[k] + w * yo[k];
    y[k + n / 2] = ye[k] - w * yo[k];
    w *= wn;
  }
  return y;
}

int main() {
  int n, m;
  cin >> n >> m;

  vector<long long> a_in(n), b_in(m);
  for (int i = 0; i < n; ++i)
    cin >> a_in[i];
  for (int j = 0; j < m; ++j)
    cin >> b_in[j];

  // Smallest power of 2 such that L >= n + m - 1
  int L = 1;
  while (L < n + m - 1)
    L <<= 1;

  // Pad coefficient vectors to length L
  vector<cd> a(L, 0), b(L, 0);
  for (int i = 0; i < n; ++i)
    a[i] = a_in[i];
  for (int j = 0; j < m; ++j)
    b[j] = b_in[j];

  // Evaluation at roots of unity using FFT
  vector<cd> y_a = recursive_fft(a, false);
  vector<cd> y_b = recursive_fft(b, false);

  // Point-wise multiplication in point-value representation
  vector<cd> y_c(L);
  for (int i = 0; i < L; ++i)
    y_c[i] = y_a[i] * y_b[i];

  // Interpolation via inverse FFT
  vector<cd> c = recursive_fft(y_c, true);
  for (int i = 0; i < L; ++i)
    c[i] /= L;

  // Output exact integer coefficients
  for (int i = 0; i < n + m - 1; ++i) {
    long long coeff = llround(c[i].real());
    cout << coeff << (i + 1 == n + m - 1 ? "" : " ");
  }
  cout << "\n";

  return 0;
}
