#include "./arow.hh"
#include "./util.cc"
#include "./arow.cc"

bool DIAG=false;
bool VERBOSE=false;
bool LEARN=true;

vr mu;
vvr Sigma;
vr sigma; // this for diagonalization of `Sigma`

bool COLON_MODE = false;
void setcolon(const string& line) {
  bool has_space = false;
  bool has_colon = false;
  for (char c: line) {
    if (c == ' ') has_space = true;
    else if (c == ':') has_colon = true;
  }
  COLON_MODE = (has_colon or (not has_space));
}

int count_space(const string& line) {
  int ret = 0;
  for (char c: line) { if (c == ' ') ++ret; }
  return ret;
}

// returns <answer, predict>
pair<int, int>
receive(istream&sin) {
  string line; getline(sin, line);
  stringstream ss(line);
  int y; ss >> y;
  vr xs(n);
  xs[0] = 1.0;

  if (COLON_MODE) {
    int idx; char colon; Real x;
    while (ss >> idx >> colon >> x) {
      assert(idx >= 1);
      if (idx >= n) { // enlarge range
        mu.resize(idx + 1);
        xs.resize(idx + 1);
        if (not DIAG) {
          rep (i, n) Sigma[i].resize(idx + 1);
          Sigma.resize(idx + 1);
          for (int i = n; i <= idx; ++i) {
            Sigma[i].resize(idx + 1);
            Sigma[i][i] = 1.0;
          }
        } else {
          sigma.resize(idx + 1);
        }
        n = idx + 1;
      }
      xs[idx] = x;
    }
  } else {
    rep (i, n-1) ss >> xs[i+1];
  }

  bool res;
  if (LEARN) {
    if (not DIAG) {
      res = update(mu, Sigma, y, xs);
    } else {
      res = update_diag(mu, sigma, y, xs);
    }
    if (VERBOSE) { cerr << res << endl; }
  }

  int z = test(mu, xs);
  return make_pair(y, z);
}

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
  if (not LEARN and loadfile == "") {
    cerr << "--test requires -f <model-path>" << endl;
    return 0;
  }

  if (loadfile != "") {
    stringstream ss;
    ifstream sin(loadfile);
    string s;
    while (getline(sin, s)) {
      if (s == "" or s[0] == '#') continue;
      ss << s << endl;
    }
    ss >> DIAG;
    ss >> n;
    ss >> r;
    mu = vr(n); ss >> mu;
    if (not DIAG) {
      Sigma = vvr(n, vr(n));
      rep (i, n) rep (j, n) ss >> Sigma[i][j];
    } else {
      sigma = vr(n);
      rep (i, n) ss >> sigma[i];
    }
  }

  // read first line
  // and determine n if not loadmodel
  string firstline;
  getline(cin, firstline);
  setcolon(firstline);
  if (loadfile == "") {
    if (not COLON_MODE) {
      n = count_space(firstline) + 1;
    } else {
      n = 20; // reservation
    }
    mu = vr(n, 0);
    if (not DIAG) {
      Sigma = vvr(n, vr(n, 0));
      rep (i, n) Sigma[i][i] = 1;
    } else {
      sigma = vr(n, 1);
    }
  }

  using vi = vector<int>;
  vi ans;
  vi predicts;

  /*
   * receive datum and update
   */
  {
    stringstream ss(firstline);
    auto res = receive(ss);
    ans.push_back(res.first);
    predicts.push_back(res.second);
  }
  while (cin) {
    auto res = receive(cin);
    ans.push_back(res.first);
    predicts.push_back(res.second);
  }

  // output the result model
  if (LEARN) {
    cout << "# DIAG" << endl;
    cout << DIAG << endl;
    cout << "# N" << endl;
    cout << n << endl;
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
      } else if (predicts[i] == 1) {
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
