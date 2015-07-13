
template<class S, class T> inline
ostream& operator<<(ostream&os, pair<S,T> p) {
  os << '(' << p.first << ", " << p.second << ')';
  return os;
}

template<class T> inline
ostream& operator<<(ostream&os, vector<T> v) {
  if (v.size() == 0) {
    os << "(empty)";
    return os;
  }
  os << v[0];
  for (int i=1, len=v.size(); i<len; ++i) os << ' ' << v[i];
  return os;
}

template<class T> inline
istream& operator>>(istream&is, vector<T>&v) {
  rep (i, v.size()) is >> v[i];
  return is;
}

template<class S, class T, class U> inline
ostream& operator<<(ostream&os, tuple<S, T, U> t) {
  return os << '(' << get<0>(t) << ", " << get<1>(t)
    << ", " << get<2>(t) << ')';
}
