require 'mkmf'

$CFLAGS << " -I/usr/local/include #{ `freetype-config --cflags`.chomp }"
$LDFLAGS << "  #{ `freetype-config --libs`.chomp }"

find_library("ggi", "ggiOpen", "/usr/local/lib")
have_header("ggi/ggi.h")

create_makefile('ggi')
