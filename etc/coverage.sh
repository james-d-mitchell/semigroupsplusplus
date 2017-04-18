#!/bin/bash
set -e
if [ ! -f .coverage_build ]; then
  make clean
  touch .coverage_build
  ./configure CPPFLAGS='-DDEBUG -UNDEBUG' CXXFLAGS='-O0 -g --coverage' LDFLAGS='-O0 -g --coverage'
fi
make check-standard -j
TODAY=`date "+%Y-%m-%d-%H-%M-%S"`
lcov --capture --directory . --output-file log/$TODAY.info
genhtml log/$TODAY.info --output-directory log/$TODAY-coverage/
rm -f log/$TODAY.info
echo "See: " log/$TODAY-coverage/index.html