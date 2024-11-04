#pragma once

#include <cinttypes>
#include <fstream>
#include <iostream>
#include <stdio.h>

#include "parser.h"
#include "strBuf.h"
#include "sand.h"

void WriteBMPbyGrid(const sandState& st, const char*);