#include "./arow.hh"
#include "./util.cc"
#include "./arow.cc"

bool DIAG=false;
bool VERBOSE=false;

int main(int argc, char*argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(0);
  cout.setf(ios::fixed);
  cout.precision(10);

  string loadfile = "";
  // parse
  for (int i = 1; i < argc; ++i) {
    string s = string(argv[i]);
#   include "./parse.cc"
  }

  cin >> n; ++n;

  vr mu(n, 0);
  vvr Sigma;
  vr sigma; // this for diagonalization of `Sigma`

  if (loadfile == "") {
    if (not DIAG) {
      Sigma = vvr(n, vr(n, 0));
      rep (i, n) Sigma[i][i] = 1;
    }
    else {
      sigma = vr(n, 1);
    }
  } else { // load
    stringstream ss;
    int i=0;
    ifstream sin(loadfile);
    while (cin) {
      string s;
      getline(sin, s); if (not sin) break;
      if (s == "" or s[0] == '#') continue;
      ss << s << endl;
    }
    ss >> DIAG;
    ss >> r;
    rep (i, n) ss >> mu[i];
    if (not DIAG) {
      Sigma = vvr(n, vr(n));
      rep (i, n) rep (j, n) ss >> Sigma[i][j];
    }
    else {
      sigma = vr(n);
      rep (i, n) ss >> sigma[i];
    }
  }

  // receive datum;
  loop {
    int y; vr x(n - 1);
    cin >> y; if (y == 0) break;
    cin >> x; if (not cin) break;
    x.push_back(1);

    bool res;
    if (not DIAG) {
      res = update(mu, Sigma, y, x);
    } else {
      res = update_diag(mu, sigma, y, x);
    }

    if (VERBOSE) { cerr << res << endl; }
  }

  // output the result model
  cout << "# DIAG" << endl;
  cout << DIAG << endl;
  cout << "# r" << endl;
  cout << r << endl;
  cout << "# mu" << endl;
  cout << mu << endl;
  if (not DIAG) {
    cout << "# Sigma" << endl;
    rep (i, n) cout << Sigma[i] << endl;
  } else {
    cout << "# diag of Sigma" << endl;
    cout << sigma << endl;
  }

  return 0;
}
