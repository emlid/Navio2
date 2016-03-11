#pragma once

#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])

int write_file(const char *path, const char *fmt, ...);
int read_file(const char *path, const char *fmt, ...);
bool check_apm();
