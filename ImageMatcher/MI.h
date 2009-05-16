#pragma once

// 7-dimension, isCanny=false: Hu MI; isCanny=true: Canny MI
int extractMI(const char* imagenamefile, bool isCanny, double* feature);