#!/bin/sh
PATH=/home/uahcls19/local/bin:$PATH
cmake . && make && make test
