#include "./arow.hh"

int n;
Real r;

vr A(0);

bool
update(vr& mu, vvr& Sigma, int y, const vr& x)
{
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
    rep (i, n) { mu[i] += diff * y / (q + r) * A[i]; }
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
    rep (i, n) { mu[i] += diff * y / (q + r) * A[i]; }
  }

  // update Sigma
  rep (i, n) {
    sigma[i] -= A[i] * A[i] / (q + r);
  }

  return correct;
}
