#pragma once

/**
 * n dimension
 * feature its size must be n*2
 * return 1-success, 0-fail
 */
int extractFourier(const char* imagenamefile, int n, double* feature);