    if (s == "--help" or s == "-?") {
      cerr << "--help or -?" << endl;
      cerr << "--diag or -D: diagonalize (in default: no). ignored when -f" << endl;
      cerr << "-r <long double>: configure the parameter `r` (in default: 1.0). ignored when -f" << endl;
      cerr << "-f <file>: load a model file" << endl;
      cerr << "--verbose or -V" << endl;
      return 0;
    }
    if (s == "--diag" or s == "-D") {
      DIAG=true;
    }
    if (s == "-f") {
      loadfile = string(argv[i+1]);
      ++i;
    }
    if (s == "-r") {
      stringstream ss(string(argv[i+1]));
      ss >> r;
      ++i;
    }
    if (s == "--verbose" or s == "-v" or s == "-V") {
      VERBOSE=true;
    }

