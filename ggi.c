// This file was automatically generated from ggi.c.in!

 /*
 * TODO:
 *   visual___free: call to ggiClose failes if ggiExit is called before.
 *
 */


#include <ggi/ggi.h>
#include "ruby.h"


#define VIS_TMPL_BASIC    \
  ggi_visual_t *vis;      \
  int err;                \
  Data_Get_Struct(self, ggi_visual_t, vis);

#define VIS_TMPL_RETURN(funcname) \
  if (err != 0) {                 \
    if (err < 0) {                \
      FAIL_GGI(funcname);         \
    } else {                      \
      return INT2NUM(err);        \
    }                             \
  }                               \
  return Qnil;

#define VIS_TMPL_NO_RETURN(funcname) \
  if (err < 0) {                     \
    FAIL_GGI(funcname);              \
    return Qnil;                     \
  }                                  \


#define FAIL_GGI(funcname) rb_raise(eGGI, funcname " failed (return value: %d)", err)

static VALUE mGGI;
static VALUE cVisual;
static VALUE cColor;
static VALUE cPixelArray;
static VALUE cEvent;
static VALUE cEventCommand;  
static VALUE cEventExpose;
static VALUE cEventValuator;
static VALUE cEventKey;
static VALUE cEventPtrMove;
static VALUE cEventPtrButton;
static VALUE cEventPtrButtonPress;
static VALUE cEventPtrButtonRelease;
static VALUE eGGI;


static VALUE
ggi_init(VALUE self)
{
  int err;

  if ((err = ggiInit()) != 0)
  {
    FAIL_GGI("ggiInit");
  }

  return Qnil;
}

static VALUE
ggi_exit(VALUE self)
{
  int err;
  err = ggiExit();
  VIS_TMPL_RETURN("ggiExit");
}

static VALUE
ggi__gt_construct(VALUE self, VALUE depth, VALUE scheme, VALUE size)
{
  return INT2NUM(GT_CONSTRUCT(NUM2INT(depth), NUM2INT(scheme), NUM2INT(size)));
}

static void visual___free(void *ptr) 
{
  int err;
  ggi_visual_t *vis = (ggi_visual_t*) ptr;


  if (*vis != NULL)
  {
    if ((err = ggiClose(*vis)) != 0)
    {
      rb_warning("ggi: ggiClose during GC free failed (return value: %d)", err);
    }
  }
  free(ptr);
}


static VALUE
visual__open(VALUE self, VALUE display)
{
  char *c_display;
  ggi_visual_t *vis;
  VALUE res;

  switch (TYPE(display))
  {
    case T_NIL:
      c_display = NULL;
      break;
    case T_STRING:
      c_display = STR2CSTR(display);
      break;
    default:
      rb_raise(rb_eTypeError, "invalid type: string or nil expected");
      return Qnil;
  }

  res = Data_Make_Struct(cVisual, ggi_visual_t, NULL, visual___free, vis);
  *vis = ggiOpen(c_display);

  if (NULL == *vis)
  {
    rb_raise(eGGI, "ggiOpen failed");
    return Qnil;
  }

  return res;
}

static VALUE
visual_close(VALUE self)
{
  ggi_visual_t *vis;
  int err;
  
  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiClose(*vis)) != 0)
  {
    FAIL_GGI("ggiClose");
  }

  *vis = NULL; // mark as closed

  return Qnil;
}





static VALUE
visual_setSimpleMode(VALUE self, VALUE xsize, VALUE ysize, VALUE frames, VALUE type)
{
  
  VIS_TMPL_BASIC 

  err = ggiSetSimpleMode(*vis, NUM2INT(xsize), NUM2INT(ysize), NUM2INT(frames), NUM2UINT(type));
  VIS_TMPL_RETURN("ggiSetSimpleMode") 
}

static VALUE
visual_setDisplayFrame(VALUE self, VALUE frameno)
{
  
  VIS_TMPL_BASIC 

  err = ggiSetDisplayFrame(*vis, NUM2INT(frameno));
  VIS_TMPL_RETURN("ggiSetDisplayFrame") 
}

static VALUE
visual_setWriteFrame(VALUE self, VALUE frameno)
{
  
  VIS_TMPL_BASIC 

  err = ggiSetWriteFrame(*vis, NUM2INT(frameno));
  VIS_TMPL_RETURN("ggiSetWriteFrame") 
}

static VALUE
visual_setReadFrame(VALUE self, VALUE frameno)
{
  
  VIS_TMPL_BASIC 

  err = ggiSetReadFrame(*vis, NUM2INT(frameno));
  VIS_TMPL_RETURN("ggiSetReadFrame") 
}
   
static VALUE
visual_getDisplayFrame(VALUE self)
{
  ggi_visual_t *vis;
  Data_Get_Struct(self, ggi_visual_t, vis);
  return INT2NUM(ggiGetDisplayFrame(*vis));
}

static VALUE
visual_getWriteFrame(VALUE self)
{
  ggi_visual_t *vis;
  Data_Get_Struct(self, ggi_visual_t, vis);
  return INT2NUM(ggiGetWriteFrame(*vis));
}

static VALUE
visual_getReadFrame(VALUE self)
{
  ggi_visual_t *vis;
  Data_Get_Struct(self, ggi_visual_t, vis);
  return INT2NUM(ggiGetReadFrame(*vis));
}

// pixel values are represented as Bignums (or Fixnums)
static VALUE
visual_mapColor(VALUE self, VALUE color)
{
  ggi_visual_t *vis;
  ggi_color *col;
  ggi_pixel pixel;

  Data_Get_Struct(color, ggi_color, col); // TODO: check if Color object
  Data_Get_Struct(self, ggi_visual_t, vis);

  pixel = ggiMapColor(*vis, col);
  return ULONG2NUM((unsigned long)pixel);
}

static VALUE
visual_unmapPixel(VALUE self, VALUE pixel)
{
  ggi_color *col;
  VALUE res = Data_Make_Struct(cColor, ggi_color, NULL, free, col);

  
  VIS_TMPL_BASIC 

  err = ggiUnmapPixel(*vis, (ggi_pixel)NUM2ULONG(pixel), col);
  VIS_TMPL_NO_RETURN("ggiUnmapPixel") 

  return res;
}

// colors: [ Color, Color, ... ]
// returns: PixelArray object
// TODO: test
static VALUE
visual_packColors(VALUE self, VALUE colors, VALUE length)
{
  ggi_visual_t *vis;
  void *buf;
  ggi_color *cols, *col;
  const ggi_pixelformat *pixel_format;
  int i, err, len;

  if (TYPE(colors) != T_ARRAY)
  {
    rb_raise(rb_eArgError, "Array required (colors)");
    return Qnil;
  }

  Data_Get_Struct(self, ggi_visual_t, vis);
  pixel_format = ggiGetPixelFormat(*vis);

  len = NUM2INT(length);
  buf = xmalloc(sizeof(int) + len*(pixel_format->size/8)); // sizeof(int) for storing the size 
  cols = ALLOC_N(ggi_color, len); 

  // convert colors to ggi_color array
  for (i=0; i<len; i++) {
    Data_Get_Struct(rb_ary_entry(colors, i), ggi_color, col);
    cols[i] = *col;
  }

  *((int*)buf) = len; // store the size in the first sizeof(int) bytes
  err = ggiPackColors(*vis, (void*)buf+sizeof(int), cols, len); 
  if (err < 0) {
    free(cols);
    free(buf);
    FAIL_GGI("ggiPackColors");
    return Qnil;
  }

  return Data_Wrap_Struct(cPixelArray, NULL, free, buf); 
}

// TODO: test
static VALUE
visual_unpackPixels(VALUE self, VALUE pixelarray, VALUE length)
{
  ggi_visual_t *vis;
  void *buf;
  int err, len, i;
  ggi_color *cols, *col_elem;
  VALUE arr, elem;

  Data_Get_Struct(self, ggi_visual_t, vis);
  Data_Get_Struct(pixelarray, void, buf);

  len = NUM2INT(length);

  if (len > *((int*)buf)) {
    FAIL_GGI("ggiUnpackPixels: pixelarray is smaller than requested length");
    return Qnil;
  }

  cols = ALLOC_N(ggi_color, len);
  
  err = ggiUnpackPixels(*vis, (void*)buf+sizeof(int), cols, len);
  if (err < 0) {
    free(cols);
    FAIL_GGI("ggiUnpackPixels");
    return Qnil;
  }

  // convert cols to Ruby array
  arr = rb_ary_new2(len);
  for (i=0; i<len; i++) {
    elem = Data_Make_Struct(cColor, ggi_color, NULL, free, col_elem);
    cols[i] = *col_elem;
    rb_ary_store(arr, i, elem);
  }

  return arr;
}

// TODO: test
static VALUE
visual_putHLine(VALUE self, VALUE x, VALUE y, VALUE w, VALUE pixelarray)
{
  ggi_visual_t *vis;
  void *buf;
  int err;
  int width;

  Data_Get_Struct(self, ggi_visual_t, vis);
  Data_Get_Struct(pixelarray, void, buf);

  width = NUM2INT(w);

  if (width > *((int*)buf)) {
    FAIL_GGI("ggiPutHLine: size of pixelarray is smaller than width");
    return Qnil;
  }

  err = ggiPutHLine(*vis, NUM2INT(x), NUM2INT(y), width, (void*)buf+sizeof(int));
  VIS_TMPL_RETURN("ggiPutHLine");
}

// TODO: test
static VALUE
visual_putVLine(VALUE self, VALUE x, VALUE y, VALUE h, VALUE pixelarray)
{
  ggi_visual_t *vis;
  void *buf;
  int err;
  int height;

  Data_Get_Struct(self, ggi_visual_t, vis);
  Data_Get_Struct(pixelarray, void, buf);

  height = NUM2INT(h);

  if (height > *((int*)buf)) {
    FAIL_GGI("ggiPutVLine: size of pixelarray is smaller than width");
    return Qnil;
  }

  err = ggiPutVLine(*vis, NUM2INT(x), NUM2INT(y), height, (void*)buf+sizeof(int));
  VIS_TMPL_RETURN("ggiPutVLine");
}

// TODO: test
static VALUE
visual_putBox(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h, VALUE pixelarray)
{
  ggi_visual_t *vis;
  void *buf;
  int err;
  int width, height;

  Data_Get_Struct(self, ggi_visual_t, vis);
  Data_Get_Struct(pixelarray, void, buf);

  width = NUM2INT(w);
  height = NUM2INT(h);

  if ((width*height) > *((int*)buf)) {
    FAIL_GGI("ggiPutBox: size of pixelarray is smaller than width*height");
    return Qnil;
  }

  err = ggiPutBox(*vis, NUM2INT(x), NUM2INT(y), width, height, (void*)buf+sizeof(int));
  VIS_TMPL_RETURN("ggiPutBox");
}




// TODO: test
static VALUE
visual_getHLine(VALUE self, VALUE x, VALUE y, VALUE w)
{
  ggi_visual_t *vis;
  void *buf;
  const ggi_pixelformat *pixel_format;
  int err;
  int width;

  Data_Get_Struct(self, ggi_visual_t, vis);

  pixel_format = ggiGetPixelFormat(*vis);

  width = NUM2INT(w);
  buf = xmalloc(sizeof(int) + width*(pixel_format->size/8)); // sizeof(int) for storing the size 
  *((int*)buf) = width; // store the size in the first sizeof(int) bytes

  err = ggiGetHLine(*vis, NUM2INT(x), NUM2INT(y), width, (void*)buf+sizeof(int));
  if (err < 0) {
    free(buf);
    FAIL_GGI("ggiGetHLine");
    return Qnil;
  }

  return Data_Wrap_Struct(cPixelArray, NULL, free, buf); 
}

// TODO: test
static VALUE
visual_getVLine(VALUE self, VALUE x, VALUE y, VALUE h)
{
  ggi_visual_t *vis;
  void *buf;
  const ggi_pixelformat *pixel_format;
  int err;
  int height;

  Data_Get_Struct(self, ggi_visual_t, vis);

  pixel_format = ggiGetPixelFormat(*vis);

  height = NUM2INT(h);
  buf = xmalloc(sizeof(int) + height*(pixel_format->size/8)); // sizeof(int) for storing the size 
  *((int*)buf) = height; // store the size in the first sizeof(int) bytes

  err = ggiGetVLine(*vis, NUM2INT(x), NUM2INT(y), height, (void*)buf+sizeof(int));
  if (err < 0) {
    free(buf);
    FAIL_GGI("ggiGetVLine");
    return Qnil;
  }

  return Data_Wrap_Struct(cPixelArray, NULL, free, buf); 
}

// TODO: test
static VALUE
visual_getBox(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h)
{
  ggi_visual_t *vis;
  void *buf;
  const ggi_pixelformat *pixel_format;
  int err;
  int width, height;

  Data_Get_Struct(self, ggi_visual_t, vis);

  pixel_format = ggiGetPixelFormat(*vis);

  width = NUM2INT(w);
  height = NUM2INT(h);
  buf = xmalloc(sizeof(int) + width*height*(pixel_format->size/8)); // sizeof(int) for storing the size 
  *((int*)buf) = width*height; // store the size in the first sizeof(int) bytes

  err = ggiGetBox(*vis, NUM2INT(x), NUM2INT(y), width, height, (void*)buf+sizeof(int));
  if (err < 0) {
    free(buf);
    FAIL_GGI("ggiGetBox");
    return Qnil;
  }

  return Data_Wrap_Struct(cPixelArray, NULL, free, buf); 
}


static VALUE
visual_setGCForeground(VALUE self, VALUE pixel)
{
  
  VIS_TMPL_BASIC 

  err = ggiSetGCForeground(*vis, (ggi_pixel)NUM2ULONG(pixel));
  VIS_TMPL_RETURN("ggiSetGCForeground") 
}

static VALUE
visual_setGCBackground(VALUE self, VALUE pixel)
{
  
  VIS_TMPL_BASIC 

  err = ggiSetGCBackground(*vis, (ggi_pixel)NUM2ULONG(pixel));
  VIS_TMPL_RETURN("ggiSetGCBackground") 
}

static VALUE
visual_getGCForeground(VALUE self)
{
  ggi_pixel pixel;
  
  VIS_TMPL_BASIC 

  err = ggiGetGCForeground(*vis, &pixel);
  VIS_TMPL_NO_RETURN("ggiGetGCForeground") 
  return ULONG2NUM((unsigned long) pixel);
}

static VALUE
visual_getGCBackground(VALUE self)
{
  ggi_pixel pixel;
  
  VIS_TMPL_BASIC 

  err = ggiGetGCBackground(*vis, &pixel);
  VIS_TMPL_NO_RETURN("ggiGetGCBackground") 
  return ULONG2NUM((unsigned long) pixel);
}

static VALUE
visual_setGCClipping(VALUE self, VALUE left, VALUE top, VALUE right, VALUE bottom)
{
  
  VIS_TMPL_BASIC 

  err = ggiSetGCClipping(*vis, NUM2INT(left), NUM2INT(top), NUM2INT(right), NUM2INT(bottom));
  VIS_TMPL_RETURN("ggiSetGCClipping") 
}

static VALUE
visual_getGCClipping(VALUE self)
{
  int left, top, right, bottom;
  
  VIS_TMPL_BASIC 

  err = ggiGetGCClipping(*vis, &left, &top, &right, &bottom);
  VIS_TMPL_NO_RETURN("ggiGetGCClipping") 
  return rb_ary_new3(4, INT2NUM(left), INT2NUM(top), INT2NUM(right), INT2NUM(bottom));
}

static VALUE
visual_drawPixel(VALUE self, VALUE x, VALUE y)
{
  
  VIS_TMPL_BASIC 

  err = ggiDrawPixel(*vis, NUM2INT(x), NUM2INT(y));
  VIS_TMPL_RETURN("ggiDrawPixel") 
}

static VALUE
visual_putPixel(VALUE self, VALUE x, VALUE y, VALUE pixel)
{
  
  VIS_TMPL_BASIC 

  err = ggiPutPixel(*vis, NUM2INT(x), NUM2INT(y), (ggi_pixel)NUM2ULONG(pixel));
  VIS_TMPL_RETURN("ggiPutPixel") 
}

static VALUE
visual_getPixel(VALUE self, VALUE x, VALUE y)
{
  ggi_pixel pixel;
  
  VIS_TMPL_BASIC 

  err = ggiGetPixel(*vis, NUM2INT(x), NUM2INT(y), &pixel);
  VIS_TMPL_NO_RETURN("ggiGetPixel") 
  return ULONG2NUM((unsigned long) pixel);
}

static VALUE
visual_drawHLine(VALUE self, VALUE x, VALUE y, VALUE w)
{
  
  VIS_TMPL_BASIC 

  err = ggiDrawHLine(*vis, NUM2INT(x), NUM2INT(y), NUM2INT(w));
  VIS_TMPL_RETURN("ggiDrawHLine") 
}

static VALUE
visual_drawVLine(VALUE self, VALUE x, VALUE y, VALUE h)
{
  
  VIS_TMPL_BASIC 

  err = ggiDrawVLine(*vis, NUM2INT(x), NUM2INT(y), NUM2INT(h));
  VIS_TMPL_RETURN("ggiDrawVLine") 
}

static VALUE
visual_drawLine(VALUE self, VALUE x, VALUE y, VALUE xe, VALUE ye)
{
  
  VIS_TMPL_BASIC 

  err = ggiDrawLine(*vis, NUM2INT(x), NUM2INT(y), NUM2INT(xe), NUM2INT(ye));
  VIS_TMPL_RETURN("ggiDrawLine") 
}

static VALUE
visual_drawBox(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h)
{
  
  VIS_TMPL_BASIC 

  err = ggiDrawBox(*vis, NUM2INT(x), NUM2INT(y), NUM2INT(w), NUM2INT(h));
  VIS_TMPL_RETURN("ggiDrawBox") 
}


/* =================================== My own drawing routines ================================= */

static VALUE
visual_drawRect(VALUE self, VALUE x1, VALUE y1, VALUE width, VALUE height)
{
  int x, y, w, h;
  int err = 0;
  ggi_visual_t *vis;
  Data_Get_Struct(self, ggi_visual_t, vis);

  x = NUM2INT(x1);
  y = NUM2INT(y1);
  w = NUM2INT(width);
  h = NUM2INT(height);

  if (w > 0 && h > 0)
  {
    err = ggiDrawHLine(*vis, x, y, w);
    err |= ggiDrawHLine(*vis, x, y+h-1, w);
    err |= ggiDrawVLine(*vis, x, y, h);
    err |= ggiDrawVLine(*vis, x+w-1, y, h);
  }

  VIS_TMPL_RETURN("drawRect");
}

extern int filledPolygonColor(ggi_visual_t dst, int * vx, int * vy, int n);

static VALUE
visual_filledPoly(VALUE self, VALUE vx, VALUE vy, VALUE n)
{
  int *vertex_x, *vertex_y;
  int elems, i;
  int err;
  ggi_visual_t *vis;
  Data_Get_Struct(self, ggi_visual_t, vis);

  elems = NUM2INT(n);
  vertex_x = ALLOC_N(int, elems); 
  vertex_y = ALLOC_N(int, elems); 

  for (i = 0; i < elems; i++) {
    vertex_x[i] = NUM2INT(rb_ary_entry(vx, i));
    vertex_y[i] = NUM2INT(rb_ary_entry(vy, i));
  }

  err = filledPolygonColor(*vis, vertex_x, vertex_y, elems);

  VIS_TMPL_RETURN("filledPoly");
}


typedef struct _TTF_Font TTF_Font;
extern int TTF_Init( void );
extern TTF_Font* TTF_OpenFontIndex( const char *file, int ptsize, long index );
extern void TTF_CloseFont( TTF_Font* font );
extern ggi_visual_t TTF_RenderText_Solid(TTF_Font *font, const char *text, ggi_color *fg);

static ttf_already_initialized = 0;

static VALUE
visual_renderText(VALUE self, VALUE font_name, VALUE font_size, VALUE xpos, VALUE ypos, VALUE text)
{
  TTF_Font *font;
  ggi_visual_t src, *vis;
  ggi_color col, pixcol;
  ggi_mode mode;
  const ggi_directbuffer *dbuf;
  int x, y, xa, ya;
  ggi_pixel pixel;

  Data_Get_Struct(self, ggi_visual_t, vis);

  xa = NUM2INT(xpos); ya = NUM2INT(ypos);

  ggiGetGCForeground(*vis, &pixel);
  ggiUnmapPixel(*vis, pixel, &col);

  if (ttf_already_initialized == 0) {
    TTF_Init();
    ttf_already_initialized = 1;
  }

  // TODO: very slow to open the font for every renderText call
  font = TTF_OpenFont(STR2CSTR(font_name), NUM2INT(font_size));

  if (font == NULL) {
    rb_raise(eGGI, "renderText: couldn't open font");
    return Qnil;
  }

  src = TTF_RenderText_Solid(font, STR2CSTR(text), &col);
  ggiGetMode(src, &mode);

  // copy text onto destination visual
  // with palette color 0 beeing transparent 
  //ggiCrossBlit(src, 0, 0, mode.visible.x, mode.visible.y, *vis, 0, 0);
  
  // TODO: does the blit extension help to avoid the loops below?
  dbuf = ggiDBGetBuffer(src, 0);
  if(!(dbuf->type & GGI_DB_SIMPLE_PLB))
  {
    rb_raise(eGGI, "renderText/ggiDBGetBuffer: pixel linear buffer required");
  }
  if(dbuf->read == NULL) {
    rb_raise(eGGI, "renderText: visual does not have read buffer");
  } 
  
  for (x=0; x < mode.visible.x; x++) 
  {
    for (y=0; y < mode.visible.y; y++) 
    {
      if (((unsigned char*)dbuf->read)[x+dbuf->buffer.plb.stride*y] == 0)
      {
        continue;
      }
      
      ggiGetPixel(src, x, y, &pixel);
      ggiUnmapPixel(src, pixel, &pixcol);
      ggiPutPixel(*vis, x+xa, y+ya, ggiMapColor(*vis, &pixcol));
    }
  }

  ggiClose(src);
  TTF_CloseFont(font);
  return Qnil;
}


/* ============================================================================================= */


static VALUE
visual_fillscreen(VALUE self)
{
  
  VIS_TMPL_BASIC 

  err = ggiFillscreen(*vis);
  VIS_TMPL_RETURN("ggiFillscreen") 
}

static VALUE
visual_copyBox(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h, VALUE nx, VALUE ny)
{
  
  VIS_TMPL_BASIC 

  err = ggiCopyBox(*vis, NUM2INT(x), NUM2INT(y), NUM2INT(w), NUM2INT(h), NUM2INT(nx), NUM2INT(ny));
  VIS_TMPL_RETURN("ggiCopyBox") 
}

static VALUE
visual_crossBlit(VALUE self, VALUE sx, VALUE sy, VALUE w, VALUE h, VALUE dst, VALUE dx, VALUE dy)
{
  ggi_visual_t *dst_vis;
  Data_Get_Struct(dst, ggi_visual_t, dst_vis);
  
  VIS_TMPL_BASIC 

  err = ggiCrossBlit(*vis, NUM2INT(sx), NUM2INT(sy), NUM2INT(w), NUM2INT(h), *dst_vis, NUM2INT(dx), NUM2INT(dy));
  VIS_TMPL_RETURN("ggiCrossBlit") 
}

static VALUE
visual_setOrigin(VALUE self, VALUE x, VALUE y)
{
  
  VIS_TMPL_BASIC 

  err = ggiSetOrigin(*vis, NUM2INT(x), NUM2INT(y));
  VIS_TMPL_RETURN("ggiSetOrigin") 
}

static VALUE
visual_getOrigin(VALUE self)
{
  int x, y;

  
  VIS_TMPL_BASIC 

  err = ggiGetOrigin(*vis, &x, &y);
  VIS_TMPL_NO_RETURN("ggiGetOrigin") 

  return rb_ary_new3(2, INT2NUM(x), INT2NUM(y));
}

static VALUE
visual_putc(VALUE self, VALUE x, VALUE y, VALUE c)
{
  
  VIS_TMPL_BASIC 

  err = ggiPutc(*vis, NUM2INT(x), NUM2INT(y), NUM2CHR(c));
  VIS_TMPL_RETURN("ggiPutc") 
}

static VALUE
visual_puts(VALUE self, VALUE x, VALUE y, VALUE str)
{
  
  VIS_TMPL_BASIC 

  err = ggiPuts(*vis, NUM2INT(x), NUM2INT(y), STR2CSTR(str));
  VIS_TMPL_RETURN("ggiPuts") 
}

static VALUE
visual_getCharSize(VALUE self)
{
  int w, h;

  
  VIS_TMPL_BASIC 

  err = ggiGetCharSize(*vis, &w, &h);
  VIS_TMPL_NO_RETURN("ggiGetCharSize") 

  return rb_ary_new3(2, INT2NUM(w), INT2NUM(h));
}

static VALUE
visual_flush(VALUE self)
{
  
  VIS_TMPL_BASIC 

  err = ggiFlush(*vis);
  VIS_TMPL_RETURN("ggiFlush") 
}

static VALUE
visual_flushRegion(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h)
{
  
  VIS_TMPL_BASIC 

  err = ggiFlushRegion(*vis, NUM2INT(x), NUM2INT(y), NUM2INT(w), NUM2INT(h));
  VIS_TMPL_RETURN("ggiFlushRegion") 
}

static VALUE
visual_setFlags(VALUE self, VALUE flags)
{
  
  VIS_TMPL_BASIC 

  err = ggiSetFlags(*vis, NUM2INT(flags));
  VIS_TMPL_RETURN("ggiSetFlags") 
}

static VALUE
visual_addFlags(VALUE self, VALUE flags)
{
  
  VIS_TMPL_BASIC 

  err = ggiAddFlags(*vis, NUM2INT(flags));
  VIS_TMPL_RETURN("ggiAddFlags") 
}

static VALUE
visual_removeFlags(VALUE self, VALUE flags)
{
  
  VIS_TMPL_BASIC 

  err = ggiAddFlags(*vis, NUM2INT(flags));
  VIS_TMPL_RETURN("ggiAddFlags") 
}

static VALUE
visual_getFlags(VALUE self)
{
  ggi_visual_t *vis;
  Data_Get_Struct(self, ggi_visual_t, vis);
  return INT2NUM(ggiGetFlags(*vis));
}


/* ======================================== Events ===================================== */

static VALUE
visual_getc(VALUE self)
{
  ggi_visual_t *vis;
  
  Data_Get_Struct(self, ggi_visual_t, vis);
  return INT2NUM(ggiGetc(*vis));
}

static VALUE
visual_kbhit(VALUE self)
{
  ggi_visual_t *vis;
  
  Data_Get_Struct(self, ggi_visual_t, vis);
  return ( (ggiKbhit(*vis) == 0) ? Qfalse : Qtrue ); 
}

static VALUE
visual_eventPoll(VALUE self, VALUE eventMask, VALUE timeout)
{
  ggi_visual_t *vis;
  gii_event_mask em;
  struct timeval tv;
  struct timeval *tv_ptr = &tv;

  Data_Get_Struct(self, ggi_visual_t, vis);

  switch (TYPE(timeout))
  {
    case T_NIL:
      tv_ptr = NULL;
      break;

    case T_FIXNUM:
      tv.tv_sec = FIX2INT(timeout);
      tv.tv_usec = 0;
      break;

    default:
      if (rb_class_of(timeout) == rb_cTime)
      {
        tv.tv_sec = NUM2LONG(rb_funcall(timeout, rb_intern("tv_sec"), 0));
        tv.tv_usec = NUM2LONG(rb_funcall(timeout, rb_intern("tv_usec"), 0));
      }
      else
      {
        rb_raise(rb_eArgError, "nil, Fixnum or Time required");
        return Qnil;
      }
  }

  em = ggiEventPoll(*vis, NUM2INT(eventMask), tv_ptr);

  if ((long)em < 0)
  {
    rb_raise(eGGI, "ggiEventPoll failed (return value: %d)", em);
    return Qnil;
  }

  return LONG2NUM(em);
}

static VALUE
visual_eventsQueued(VALUE self, VALUE eventMask)
{
  ggi_visual_t *vis;

  Data_Get_Struct(self, ggi_visual_t, vis);

  return INT2NUM(ggiEventsQueued(*vis, NUM2INT(eventMask)));
}


/* ======================================== Event ===================================== */

static VALUE
event___create(gii_event *ev)
{
  switch (ev->any.type)
  {
    case evNothing:
      rb_raise(eGGI, "event is not valid");
      return Qnil;
      break;

    case evCommand:
    case evInformation:
      return Data_Wrap_Struct(cEventCommand, NULL, free, ev);
      break;

    case evExpose:
      return Data_Wrap_Struct(cEventExpose, NULL, free, ev);
      break;

    case evKeyPress:
    case evKeyRelease:
    case evKeyRepeat:
      return Data_Wrap_Struct(cEventKey, NULL, free, ev);
      break;

    case evPtrRelative:
    case evPtrAbsolute:
      return Data_Wrap_Struct(cEventPtrMove, NULL, free, ev);
      break;

    case evPtrButtonPress:
      return Data_Wrap_Struct(cEventPtrButtonPress, NULL, free, ev);
      break;
    case evPtrButtonRelease:
      return Data_Wrap_Struct(cEventPtrButtonRelease, NULL, free, ev);
      break;

    case evValRelative:
    case evValAbsolute:
      return Data_Wrap_Struct(cEventValuator, NULL, free, ev);
      break;

    default: 
      rb_raise(eGGI, "invalid/unknown event type");
      return Qnil;
  }
}

static VALUE
event_size(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return INT2FIX(ev->any.size);
}

static VALUE
event_type(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return INT2FIX(ev->any.type);
}

static VALUE
event_error(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return INT2FIX(ev->any.error);
}

static VALUE
event_origin(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return ULONG2NUM(ev->any.origin);
}

static VALUE
event_target(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return ULONG2NUM(ev->any.target);
}

static VALUE
event_time(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return rb_funcall(rb_cTime, rb_intern("at"), 2, ULONG2NUM(ev->any.time.tv_sec), ULONG2NUM(ev->any.time.tv_usec)); 
}

static VALUE
eventCommand_code(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return ULONG2NUM(ev->cmd.code);
}

static VALUE
eventCommand_data(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return rb_str_new((char*)ev->cmd.data, GII_CMD_DATA_MAX); // TODO: use size
}


static VALUE
eventExpose_x(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return ULONG2NUM(ev->expose.x); 
}

static VALUE
eventExpose_y(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return ULONG2NUM(ev->expose.y); 
}

static VALUE
eventExpose_w(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return ULONG2NUM(ev->expose.w); 
}

static VALUE
eventExpose_h(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return ULONG2NUM(ev->expose.h); 
}


static VALUE
eventValuator_first(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return ULONG2NUM(ev->val.first);
}

static VALUE
eventValuator_count(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return ULONG2NUM(ev->val.count);
}

static VALUE
eventValuator_value(VALUE self, VALUE inx)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);

  if (TYPE(inx) != T_FIXNUM || FIX2INT(inx) >= 32 || FIX2INT(inx) < 0)
  {
    rb_raise(rb_eArgError, "Fixnum required (in the range 0..31)");
  }

  return LONG2NUM(ev->val.value[FIX2INT(inx)]);
}


static VALUE
eventKey_modifiers(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return ULONG2NUM(ev->key.modifiers);
}

static VALUE
eventKey_sym(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return ULONG2NUM(ev->key.sym);
}

static VALUE
eventKey_label(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return ULONG2NUM(ev->key.label);
}

static VALUE
eventKey_button(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return ULONG2NUM(ev->key.button);
}



static VALUE
eventPtrMove_x(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return ULONG2NUM(ev->pmove.x);
}

static VALUE
eventPtrMove_y(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return ULONG2NUM(ev->pmove.y);
}

static VALUE
eventPtrMove_z(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return ULONG2NUM(ev->pmove.z);
}

static VALUE
eventPtrMove_wheel(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return ULONG2NUM(ev->pmove.wheel);
}


static VALUE
eventPtrButton_button(VALUE self)
{
  gii_event *ev;
  Data_Get_Struct(self, gii_event, ev);
  return ULONG2NUM(ev->pbutton.button);
}

/* ==================================================================================== */

// returns instance of class Event (or one of it's subclasses)
static VALUE
visual_eventRead(VALUE self, VALUE eventMask)
{
  ggi_visual_t *vis;
  int evSize;
  gii_event *ev;

  Data_Get_Struct(self, ggi_visual_t, vis);

  ev = ALLOC(gii_event);
  evSize = ggiEventRead(*vis, ev, NUM2INT(eventMask));

  if (0 == evSize)
  {
    rb_raise(eGGI, "ggiEventRead failed");
    return Qnil;
  }

  return event___create(ev);  
}



/* ======================================== Color ===================================== */

static VALUE
color__new(VALUE self, VALUE red, VALUE green, VALUE blue, VALUE alpha)
{
  ggi_color *col;
  unsigned int r, g, b, a;
  VALUE res;

  r = NUM2INT(red);
  g = NUM2INT(green);
  b = NUM2INT(blue);
  a = NUM2INT(alpha);

  if (r > 0xFFFF || g > 0xFFFF || b > 0xFFFF || a > 0xFFFF)
  {
    rb_raise(rb_eArgError, "Integer too large: r, g, b, a values must be below 0xFFFF");
    return Qnil;
  }

  res = Data_Make_Struct(cColor, ggi_color, NULL, free, col);

  col->r = r;
  col->g = g;
  col->b = b;
  col->a = a;

  return res;
}


static VALUE
color_r(VALUE self)
{
  ggi_color *col;
  Data_Get_Struct(self, ggi_color, col);
  return INT2FIX(col->r);
}

static VALUE
color_g(VALUE self)
{
  ggi_color *col;
  Data_Get_Struct(self, ggi_color, col);
  return INT2FIX(col->g);
}

static VALUE
color_b(VALUE self)
{
  ggi_color *col;
  Data_Get_Struct(self, ggi_color, col);
  return INT2FIX(col->b);
}

static VALUE
color_a(VALUE self)
{
  ggi_color *col;
  Data_Get_Struct(self, ggi_color, col);
  return INT2FIX(col->a);
}



void Init_ggi()
{
  mGGI = rb_define_module("GGI");
  cVisual = rb_define_class_under(mGGI, "Visual", rb_cObject);
  cColor = rb_define_class_under(mGGI, "Color", rb_cObject);
  cPixelArray = rb_define_class_under(mGGI, "PixelArray", rb_cObject);

  cEvent = rb_define_class_under(mGGI, "Event", rb_cObject);
  cEventCommand = rb_define_class_under(cEvent, "Command", cEvent);
  cEventExpose = rb_define_class_under(cEvent, "Expose", cEvent);
  cEventValuator = rb_define_class_under(cEvent, "Valuator", cEvent);
  cEventKey = rb_define_class_under(cEvent, "Key", cEvent);
  cEventPtrMove = rb_define_class_under(cEvent, "PtrMove", cEvent);
  cEventPtrButton = rb_define_class_under(cEvent, "PtrButton", cEvent);
  cEventPtrButtonPress = rb_define_class_under(cEvent, "PtrButtonPress", cEventPtrButton);
  cEventPtrButtonRelease = rb_define_class_under(cEvent, "PtrButtonRelease", cEventPtrButton);

  eGGI = rb_define_class_under(mGGI, "Error", rb_eRuntimeError);

  // define constants
  rb_define_const(mGGI, "GT_TEXT", INT2NUM(GT_TEXT));
  rb_define_const(mGGI, "GT_TRUECOLOR", INT2NUM(GT_TRUECOLOR));
  rb_define_const(mGGI, "GT_GREYSCALE", INT2NUM(GT_GREYSCALE));
  rb_define_const(mGGI, "GT_PALETTE", INT2NUM(GT_PALETTE));
  rb_define_const(mGGI, "GT_STATIC_PALETTE", INT2NUM(GT_STATIC_PALETTE));
  rb_define_const(mGGI, "GT_SUBSAMPLE_YUV", INT2NUM(GT_SUBSAMPLE_YUV));
  rb_define_const(mGGI, "GT_SUBSAMPLE_U_YCRBR", INT2NUM(GT_SUBSAMPLE_U_YCRBR));
  rb_define_const(mGGI, "GT_SUBSAMPLE_S_YCRBR", INT2NUM(GT_SUBSAMPLE_S_YCRBR));
  rb_define_const(mGGI, "GT_NIL", INT2NUM(GT_NIL));
  rb_define_const(mGGI, "GT_SUB_REVERSE_ENDIAN", INT2NUM(GT_SUB_REVERSE_ENDIAN));
  rb_define_const(mGGI, "GT_SUB_HIGHBIT_RIGHT", INT2NUM(GT_SUB_HIGHBIT_RIGHT));
  rb_define_const(mGGI, "GT_SUB_PACKED_GETPUT", INT2NUM(GT_SUB_PACKED_GETPUT));
  rb_define_const(mGGI, "GT_TEXT16", INT2NUM(GT_TEXT16));
  rb_define_const(mGGI, "GT_TEXT32", INT2NUM(GT_TEXT32));
  rb_define_const(mGGI, "GT_1BIT", INT2NUM(GT_1BIT));
  rb_define_const(mGGI, "GT_2BIT", INT2NUM(GT_2BIT));
  rb_define_const(mGGI, "GT_4BIT", INT2NUM(GT_4BIT));
  rb_define_const(mGGI, "GT_8BIT", INT2NUM(GT_8BIT));
  rb_define_const(mGGI, "GT_15BIT", INT2NUM(GT_15BIT));
  rb_define_const(mGGI, "GT_16BIT", INT2NUM(GT_16BIT));
  rb_define_const(mGGI, "GT_24BIT", INT2NUM(GT_24BIT));
  rb_define_const(mGGI, "GT_32BIT", INT2NUM(GT_32BIT));
  rb_define_const(mGGI, "GT_AUTO", INT2NUM(GT_AUTO));
  rb_define_const(mGGI, "GT_INVALID", INT2NUM(GT_INVALID));
  rb_define_const(mGGI, "GGI_AUTO", INT2NUM(GGI_AUTO));
  rb_define_const(mGGI, "GGIFLAG_ASYNC", INT2NUM(GGIFLAG_ASYNC));
#ifdef GGIFLAG_TIDYBUF
  rb_define_const(mGGI, "GGIFLAG_TIDYBUF", INT2NUM(GGIFLAG_TIDYBUF));
#endif

  // define event constants
  rb_define_const(mGGI, "EmNothing", INT2NUM(emNothing));
  rb_define_const(mGGI, "EmCommand", INT2NUM(emCommand));
  rb_define_const(mGGI, "EmInformation", INT2NUM(emInformation));
  rb_define_const(mGGI, "EmExpose", INT2NUM(emExpose));
  rb_define_const(mGGI, "EmKeyPress", INT2NUM(emKeyPress));
  rb_define_const(mGGI, "EmKeyRelease", INT2NUM(emKeyRelease));
  rb_define_const(mGGI, "EmKeyRepeat", INT2NUM(emKeyRepeat));
  rb_define_const(mGGI, "EmKey", INT2NUM(emKey));
  rb_define_const(mGGI, "EmKeyboard", INT2NUM(emKeyboard));
  rb_define_const(mGGI, "EmPtrRelative", INT2NUM(emPtrRelative));
  rb_define_const(mGGI, "EmPtrAbsolute", INT2NUM(emPtrAbsolute));
  rb_define_const(mGGI, "EmPtrButtonPress", INT2NUM(emPtrButtonPress));
  rb_define_const(mGGI, "EmPtrButtonRelease", INT2NUM(emPtrButtonRelease));
  rb_define_const(mGGI, "EmPtrMove", INT2NUM(emPtrMove));
  rb_define_const(mGGI, "EmPtrButton", INT2NUM(emPtrButton));
  rb_define_const(mGGI, "EmPointer", INT2NUM(emPointer));
  rb_define_const(mGGI, "EmValRelative", INT2NUM(emValRelative));
  rb_define_const(mGGI, "EmValAbsolute", INT2NUM(emValAbsolute));
  rb_define_const(mGGI, "EmValuator", INT2NUM(emValuator));
  rb_define_const(mGGI, "EmZero", INT2NUM(emZero));
  rb_define_const(mGGI, "EmAll", INT2NUM(emAll));
  rb_define_const(mGGI, "EvNothing", INT2NUM(evNothing));
  rb_define_const(mGGI, "EvCommand", INT2NUM(evCommand));
  rb_define_const(mGGI, "EvInformation", INT2NUM(evInformation));
  rb_define_const(mGGI, "EvExpose", INT2NUM(evExpose));
  rb_define_const(mGGI, "EvKeyPress", INT2NUM(evKeyPress));
  rb_define_const(mGGI, "EvKeyRelease", INT2NUM(evKeyRelease));
  rb_define_const(mGGI, "EvKeyRepeat", INT2NUM(evKeyRepeat));
  rb_define_const(mGGI, "EvPtrRelative", INT2NUM(evPtrRelative));
  rb_define_const(mGGI, "EvPtrAbsolute", INT2NUM(evPtrAbsolute));
  rb_define_const(mGGI, "EvPtrButtonPress", INT2NUM(evPtrButtonPress));
  rb_define_const(mGGI, "EvPtrButtonRelease", INT2NUM(evPtrButtonRelease));
  rb_define_const(mGGI, "EvValRelative", INT2NUM(evValRelative));
  rb_define_const(mGGI, "EvValAbsolute", INT2NUM(evValAbsolute));
  rb_define_const(mGGI, "EvLast", INT2NUM(evLast));

  rb_define_module_function(mGGI, "init", ggi_init, 0);
  rb_define_module_function(mGGI, "exit", ggi_exit, 0);

  rb_define_module_function(mGGI, "GT_CONSTRUCT", ggi__gt_construct, 3);

  // class Color
  rb_define_singleton_method(cColor, "new", color__new, 4);
  rb_define_method(cColor, "r", color_r, 0);
  rb_define_method(cColor, "g", color_g, 0);
  rb_define_method(cColor, "b", color_b, 0);
  rb_define_method(cColor, "a", color_a, 0);

  // class Event (and subclasses)

  rb_define_method(cEvent, "size", event_size, 0);
  rb_define_method(cEvent, "type", event_type, 0);
  rb_define_method(cEvent, "error", event_error, 0);
  rb_define_method(cEvent, "origin", event_origin, 0);
  rb_define_method(cEvent, "target", event_target, 0);
  rb_define_method(cEvent, "time", event_time, 0);  rb_define_method(cEventCommand, "code", eventCommand_code, 0);
  rb_define_method(cEventCommand, "data", eventCommand_data, 0);  rb_define_method(cEventExpose, "x", eventExpose_x, 0);
  rb_define_method(cEventExpose, "y", eventExpose_y, 0);
  rb_define_method(cEventExpose, "w", eventExpose_w, 0);
  rb_define_method(cEventExpose, "h", eventExpose_h, 0);  rb_define_method(cEventValuator, "first", eventValuator_first, 0);
  rb_define_method(cEventValuator, "count", eventValuator_count, 0);
  rb_define_method(cEventValuator, "value", eventValuator_value, 1);  rb_define_method(cEventKey, "modifiers", eventKey_modifiers, 0);
  rb_define_method(cEventKey, "sym", eventKey_sym, 0);
  rb_define_method(cEventKey, "label", eventKey_label, 0);
  rb_define_method(cEventKey, "button", eventKey_button, 0);  rb_define_method(cEventPtrMove, "x", eventPtrMove_x, 0);
  rb_define_method(cEventPtrMove, "y", eventPtrMove_y, 0);
  rb_define_method(cEventPtrMove, "z", eventPtrMove_z, 0);
  rb_define_method(cEventPtrMove, "wheel", eventPtrMove_wheel, 0);  rb_define_method(cEventPtrButton, "button", eventPtrButton_button, 0);
  
  // class Visual
  rb_define_singleton_method(cVisual, "open", visual__open, 1);

  rb_define_method(cVisual, "close", visual_close, 0);
  rb_define_method(cVisual, "setSimpleMode", visual_setSimpleMode, 4);
  rb_define_method(cVisual, "setDisplayFrame", visual_setDisplayFrame, 1);
  rb_define_method(cVisual, "setWriteFrame", visual_setWriteFrame, 1);
  rb_define_method(cVisual, "setReadFrame", visual_setReadFrame, 1);
  rb_define_method(cVisual, "getDisplayFrame", visual_getDisplayFrame, 0);
  rb_define_method(cVisual, "getWriteFrame", visual_getWriteFrame, 0);
  rb_define_method(cVisual, "getReadFrame", visual_getReadFrame, 0);
  rb_define_method(cVisual, "mapColor", visual_mapColor, 1);
  rb_define_method(cVisual, "unmapPixel", visual_unmapPixel, 1);
  rb_define_method(cVisual, "setGCForeground", visual_setGCForeground, 1);
  rb_define_method(cVisual, "setGCBackground", visual_setGCBackground, 1);
  rb_define_method(cVisual, "getGCForeground", visual_getGCForeground, 0);
  rb_define_method(cVisual, "getGCBackground", visual_getGCBackground, 0);
  rb_define_method(cVisual, "setGCClipping", visual_setGCClipping, 4);
  rb_define_method(cVisual, "getGCClipping", visual_getGCClipping, 0);
  rb_define_method(cVisual, "drawPixel", visual_drawPixel, 2);
  rb_define_method(cVisual, "putPixel", visual_putPixel, 3);
  rb_define_method(cVisual, "getPixel", visual_getPixel, 2);
  rb_define_method(cVisual, "drawHLine", visual_drawHLine, 3);
  rb_define_method(cVisual, "drawVLine", visual_drawVLine, 3);
  rb_define_method(cVisual, "drawLine", visual_drawLine, 4);
  rb_define_method(cVisual, "drawBox", visual_drawBox, 4);
  rb_define_method(cVisual, "fillscreen", visual_fillscreen, 0);
  rb_define_method(cVisual, "copyBox", visual_copyBox, 6);
  rb_define_method(cVisual, "crossBlit", visual_crossBlit, 7);
  rb_define_method(cVisual, "drawRect", visual_drawRect, 4);
  rb_define_method(cVisual, "filledPoly", visual_filledPoly, 3);
  rb_define_method(cVisual, "renderText", visual_renderText, 5);
  rb_define_method(cVisual, "setOrigin", visual_setOrigin, 2);
  rb_define_method(cVisual, "getOrigin", visual_getOrigin, 0);
  rb_define_method(cVisual, "flush", visual_flush, 0);
  rb_define_method(cVisual, "flushRegion", visual_flushRegion, 4);
  rb_define_method(cVisual, "setFlags", visual_setFlags, 1);
  rb_define_method(cVisual, "addFlags", visual_addFlags, 1);
  rb_define_method(cVisual, "removeFlags", visual_removeFlags, 1);
  rb_define_method(cVisual, "getFlags", visual_getFlags, 0);
  rb_define_method(cVisual, "packColors", visual_packColors, 2);
  rb_define_method(cVisual, "unpackPixels", visual_unpackPixels, 2);
  rb_define_method(cVisual, "putHLine", visual_putHLine, 4);
  rb_define_method(cVisual, "putVLine", visual_putVLine, 4);
  rb_define_method(cVisual, "putBox", visual_putBox, 5);
  rb_define_method(cVisual, "getHLine", visual_getHLine, 3);
  rb_define_method(cVisual, "getVLine", visual_getVLine, 3);
  rb_define_method(cVisual, "getBox", visual_getBox, 4);
  rb_define_method(cVisual, "putc", visual_putc, 3);
  rb_define_method(cVisual, "puts", visual_puts, 3);
  rb_define_method(cVisual, "getCharSize", visual_getCharSize, 0);
  rb_define_method(cVisual, "getc", visual_getc, 0);
  rb_define_method(cVisual, "kbhit", visual_kbhit, 0);
  rb_define_method(cVisual, "eventPoll", visual_eventPoll, 2);
  rb_define_method(cVisual, "eventsQueued", visual_eventsQueued, 1);
  rb_define_method(cVisual, "eventRead", visual_eventRead, 1);
}
 
