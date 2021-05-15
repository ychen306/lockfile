#include <fcntl.h>  // open
#include <unistd.h> // close
#include <sys/file.h> // flock
#include <stdio.h>
#include <stdlib.h>
#include <string>

class LockFile {
  int FD;

public:
  LockFile(std::string FName, bool &Ok) {
    FD = open(FName.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
    Ok = FD != -1;
    if (!Ok) {
      perror("failed to open lock file");
      abort();
      return;
    }
    if (flock(FD, LOCK_EX)) {
      perror("failed to flock lock file");
      close(FD);
      FD = -1;
      Ok = false;
    }
  }

  ~LockFile() {
    if (FD != -1) {
      flock(FD, LOCK_UN);
      close(FD);
    }
  }
};
