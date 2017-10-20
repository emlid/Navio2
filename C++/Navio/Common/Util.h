#pragma once

#define ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])
#define NAVIO2 3
#define NAVIO 1

int write_file(const char *path, const char *fmt, ...);
int read_file(const char *path, const char *fmt, ...);
bool check_apm();
int get_navio_version();
