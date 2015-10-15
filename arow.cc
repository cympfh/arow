#include "./arow.hh"

int n;
Real r = 1.0;
Real r0 = 1.0;

vr A(0);

bool
update(vr& mu, vvr& Sigma, int y, const vr& x)
{
  assert(mu.size() == n);
  assert(Sigma.size() == n);
  assert(Sigma[0].size() == n);
  assert(x.size() == n);

  // A = Sigma * x
  if (A.size() < n) { A = vr(n, 0); } // init (once)
  rep (i, n) {
    Real ac = 0;
    rep (j, n) { ac += Sigma[i][j] * x[j]; }
    A[i] = ac;
  }

  // q = x^T A
  Real q;
  { q = 0;
    rep (i, n) q += A[i] * x[i]; }

  // yy = x^T mu
  Real yy;
  { yy = 0;
    rep (i, n) yy += x[i] * mu[i]; }

  bool correct = (0 <= y * yy);

  // update mu
  if (1.0 > y * yy) {
    Real diff = 1.0 - y * yy;
    if (y > 0) {
      rep (i, n) { mu[i] += diff * y / (q + r) * A[i]; }
    } else {
      rep (i, n) { mu[i] += diff * y / (q + r0) * A[i]; }
    }
  }

  // update Sigma
  rep (i, n) {
    rep (j, n) {
      Sigma[i][j] -= A[i] * A[j] / (q + r);
    }
  }

  return correct;
}

bool
update_diag(vr& mu, vr& sigma, int y, const vr& x)
{
  assert(mu.size() == n);
  assert(sigma.size() == n);
  assert(x.size() == n);

  // A = Sigma * x
  if (A.size() < n) { A = vr(n, 0); } // init (once)
  rep (i, n) { A[i] = sigma[i] * x[i]; }

  // q = x^T A
  Real q;
  { q = 0;
    rep (i, n) q += A[i] * x[i]; }

  // yy = x^T mu
  Real yy;
  { yy = 0;
    rep (i, n) yy += x[i] * mu[i]; }

  bool correct = (0 <= y * yy);

  // update mu
  if (1.0 > y * yy) {
    Real diff = 1.0 - y * yy;
    if (y > 0) {
      rep (i, n) { mu[i] += diff * y / (q + r) * A[i]; }
    } else {
      rep (i, n) { mu[i] += diff * y / (q + r0) * A[i]; }
    }
  }

  // update Sigma
  rep (i, n) {
    sigma[i] -= A[i] * A[i] / (q + r);
  }

  return correct;
}

int test(const vr& mu, const vr& x) {
  Real r = 0.0;
  rep (i, mu.size()) r += mu[i] * x[i];
  return r > 0 ? 1 : -1;
}
