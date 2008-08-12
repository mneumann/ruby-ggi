require 'mkmf'

$CFLAGS << " -I/usr/local/include"

find_library("ggi", "ggiOpen", "/usr/local/lib")
have_header("ggi/ggi.h")

create_makefile('ggi')
