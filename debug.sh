#!/bin/bash

valgrind --tool=memcheck --leak-check=full build/api.run

