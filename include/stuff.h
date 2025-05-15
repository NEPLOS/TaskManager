

#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <cstring>
#include <vector>
#include <signal.h>
#include <stdio.h>
#include <sys/types.h>


extern "C"
{
    #include "leif/leif.h"
    #include <GLFW/glfw3.h>
}

static LfInputField inputSearchProcesses;
static char inputSearchProcessesString[512] = {0};

