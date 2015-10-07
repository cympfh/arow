#include "./arow.hh"
#include "./util.cc"
#include "./arow.cc"

bool DIAG=false;
bool VERBOSE=false;
bool LEARN=true;

int main(int argc, char*argv[]) {
  cin.tie(0);
  ios::sync_with_stdio(0);
  cout.setf(ios::fixed);
  cout.precision(3);

  string loadfile = "";
  // parse
  for (int i = 1; i < argc; ++i) {
    string s = string(argv[i]);
#   include "./parse.cc"
  }

  // arg assertion
  {
    if (not LEARN and loadfile == "") {
      cerr << "--test requires -f <model-path>" << endl;
      return 0;
    }
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
    string s;
    while (getline(sin, s)) {
      if (s == "" or s[0] == '#') continue;
      ss << s << endl;
    }
    ss >> DIAG;
    ss >> r;
    rep (i, n) ss >> mu[i];
    if (not DIAG) {
      Sigma = vvr(n, vr(n));
      rep (i, n) rep (j, n) ss >> Sigma[i][j];
    } else {
      sigma = vr(n);
      rep (i, n) ss >> sigma[i];
    }
  }

  using vi = vector<int>;
  vi ans;
  vi predicts;

  // receive datum;
  loop {
    int y; vr x(n - 1);
    cin >> y; if (y == 0) break;
    cin >> x; if (not cin) break;
    x.push_back(1);

    bool res;
    if (LEARN) {
      if (not DIAG) {
        res = update(mu, Sigma, y, x);
      } else {
        res = update_diag(mu, sigma, y, x);
      }
      if (VERBOSE) { cerr << res << endl; }
    } else {
      int z = test(mu, x);
      ans.push_back(y);
      predicts.push_back(z);
    }
  }

  // output the result model
  if (LEARN) {
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
  }

  // output the predicts and accuracy
  if (not LEARN) {
    for (int z: predicts) cout << z << endl;
    int a = 0,
        b = 0,
        c = 0,
        d = 0;
    rep (i, predicts.size()) {
      if (ans[i] == 1 and predicts[i] == 1) {
        ++a;
      } else if (ans[i] == 1) {
        ++b;
      } else if (ans[i] == 0 and predicts[i] == 1) {
        ++c;
      } else {
        ++d;
      }
    }
    cerr << "Acc " << (double(a+d) / double(a+b+c+d)) << endl;
    cerr << "Rec " << (double(a) / double(a+b)) << endl;
    cerr << "Prec " << (double(a) / double(a+c)) << endl;
    cerr << "F1 " << (double(2*a) / double(a+a+b+c)) << endl;

  }

  return 0;
}
