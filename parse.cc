if (s == "--diag" or s == "-D") {
  DIAG=true;
}
else if (s == "-f") {
  loadfile = string(argv[i+1]);
  ++i;
}
else if (s == "-r") {
  stringstream ss(string(argv[i+1]));
  ss >> r;
  r0 = r;
  ++i;
}
else if (s == "-r0") {
  stringstream ss(string(argv[i+1]));
  ss >> r0;
  ++i;
}
else if (s == "--verbose" or s == "-v" or s == "-V") {
  VERBOSE=true;
}
else if (s == "--test") {
  LEARN=false;
}
else {
  cerr << "  --diag or -D       set diagonalize=true (in default: false). ignored when -f" << endl;
  cerr << "  -r <double>        set the parameter `r` (in default: 1.0). ignored when -f" << endl;
  cerr << "  -f <model-path>    load a model file" << endl;
  cerr << "  --test             test (or predict) with a model (requires -f)" << endl;
  cerr << "  --verbose or -V    noisy" << endl;
  return 0;
}
