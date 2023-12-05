#ifndef UTILS_H
#define UTILS_H

/*
 * Module with some helping function(s).
 */

struct rgb_color {
   float r;
   float g;
   float b;
};


int parse_color(char *str, struct rgb_color *c);


#ifdef DEBUG
   #define log(...) printf(__VA_ARGS__)
#else
   #define log(...)
#endif


#endif /* UTILS_H */
