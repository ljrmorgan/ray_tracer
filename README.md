# Ray tracing in one weekend

Working through ["Ray tracing in one weekend"](http://in1weekend.blogspot.co.uk/2016/01/ray-tracing-in-one-weekend.html), though not in one weekend. Code is largely lifted from the book with fairly minimal changes.

Building:
```
cmake -H. -Bbuild
cmake --build build -- -j3
```

Running:
```
./bin/ray_tracer > bin/ray.ppm
```

Opening the output file:
```
# Linux
feh bin/ray.ppm

# OS X
open bin/ray.ppm
```
