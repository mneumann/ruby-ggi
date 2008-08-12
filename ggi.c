// This file was automatically generated from ggi.c.in!

 /*
 * TODO:
 *   visual___free: call to ggiClose failes if ggiExit is called before.
 *   
 *   putHLine, getHLine, packColors, unpackPixels, putVLine, getVLine, 
 *   ggiPutBox, ggiGetBox
 *
 */


#include <ggi/ggi.h>
#include "ruby.h"

static VALUE mGGI;
static VALUE cVisual;
static VALUE cColor;
static VALUE cEvent;
static VALUE cEventCommand;  
static VALUE cEventExpose;
static VALUE cEventValuator;
static VALUE cEventKey;
static VALUE cEventPtrMove;
static VALUE cEventPtrButton;
static VALUE eGGI;


static VALUE
ggi_init(VALUE self)
{
  int err;

  if ((err = ggiInit()) != 0)
  {
    rb_raise(eGGI, "ggiInit failed (return value: %d)", err);
  }

  return Qnil;
}

static VALUE
ggi_exit(VALUE self)
{
  int err;

  err = ggiExit();

  if (err != 0)
  {
    if (err > 0)
    {
      return INT2NUM(err);
    }
    else if (err < 0)
    {
      rb_raise(eGGI, "ggiExit failed (return value: %d)", err);
    }
  }

  return Qnil;
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
      rb_warn("ggi: ggiClose during GC free failed (return value: %d)", err);
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
    rb_raise(eGGI, "ggiClose failed (return value: %d)", err);
  }

  *vis = NULL; // mark as closed

  return Qnil;
}

static VALUE
visual_setSimpleMode(VALUE self, VALUE xsize, VALUE ysize, VALUE frames, VALUE type)
{
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiSetSimpleMode(*vis, NUM2INT(xsize), NUM2INT(ysize), NUM2INT(frames), NUM2UINT(type))) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiSetSimpleMode failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      return INT2NUM(err);
    }
  }
  return Qnil;

}

static VALUE
visual_setDisplayFrame(VALUE self, VALUE frameno)
{
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiSetDisplayFrame(*vis, NUM2INT(frameno))) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiSetDisplayFrame failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      return INT2NUM(err);
    }
  }
  return Qnil;

}

static VALUE
visual_setWriteFrame(VALUE self, VALUE frameno)
{
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiSetWriteFrame(*vis, NUM2INT(frameno))) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiSetWriteFrame failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      return INT2NUM(err);
    }
  }
  return Qnil;

}

static VALUE
visual_setReadFrame(VALUE self, VALUE frameno)
{
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiSetReadFrame(*vis, NUM2INT(frameno))) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiSetReadFrame failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      return INT2NUM(err);
    }
  }
  return Qnil;

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

  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiUnmapPixel(*vis, (ggi_pixel)NUM2ULONG(pixel), col)) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiUnmapPixel failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      
    }
  }
  


  return res;
}

static VALUE
visual_setGCForeground(VALUE self, VALUE pixel)
{
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiSetGCForeground(*vis, (ggi_pixel)NUM2ULONG(pixel))) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiSetGCForeground failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      return INT2NUM(err);
    }
  }
  return Qnil;

}

static VALUE
visual_setGCBackground(VALUE self, VALUE pixel)
{
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiSetGCBackground(*vis, (ggi_pixel)NUM2ULONG(pixel))) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiSetGCBackground failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      return INT2NUM(err);
    }
  }
  return Qnil;

}

static VALUE
visual_getGCForeground(VALUE self)
{
  ggi_pixel pixel;
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiGetGCForeground(*vis, &pixel)) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiGetGCForeground failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      
    }
  }
  

  return ULONG2NUM((unsigned long) pixel);
}

static VALUE
visual_getGCBackground(VALUE self)
{
  ggi_pixel pixel;
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiGetGCBackground(*vis, &pixel)) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiGetGCBackground failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      
    }
  }
  

  return ULONG2NUM((unsigned long) pixel);
}

static VALUE
visual_setGCClipping(VALUE self, VALUE left, VALUE top, VALUE right, VALUE bottom)
{
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiSetGCClipping(*vis, NUM2INT(left), NUM2INT(top), NUM2INT(right), NUM2INT(bottom))) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiSetGCClipping failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      return INT2NUM(err);
    }
  }
  return Qnil;

}

static VALUE
visual_getGCClipping(VALUE self)
{
  int left, top, right, bottom;
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiGetGCClipping(*vis, &left, &top, &right, &bottom)) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiGetGCClipping failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      
    }
  }
  

  return rb_ary_new3(4, INT2NUM(left), INT2NUM(top), INT2NUM(right), INT2NUM(bottom));
}

static VALUE
visual_drawPixel(VALUE self, VALUE x, VALUE y)
{
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiDrawPixel(*vis, NUM2INT(x), NUM2INT(y))) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiDrawPixel failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      return INT2NUM(err);
    }
  }
  return Qnil;

}

static VALUE
visual_putPixel(VALUE self, VALUE x, VALUE y, VALUE pixel)
{
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiPutPixel(*vis, NUM2INT(x), NUM2INT(y), (ggi_pixel)NUM2ULONG(pixel))) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiPutPixel failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      return INT2NUM(err);
    }
  }
  return Qnil;

}

static VALUE
visual_getPixel(VALUE self, VALUE x, VALUE y)
{
  ggi_pixel pixel;
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiGetPixel(*vis, NUM2INT(x), NUM2INT(y), &pixel)) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiGetPixel failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      
    }
  }
  

  return ULONG2NUM((unsigned long) pixel);
}

static VALUE
visual_drawHLine(VALUE self, VALUE x, VALUE y, VALUE w)
{
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiDrawHLine(*vis, NUM2INT(x), NUM2INT(y), NUM2INT(w))) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiDrawHLine failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      return INT2NUM(err);
    }
  }
  return Qnil;

}

static VALUE
visual_drawVLine(VALUE self, VALUE x, VALUE y, VALUE h)
{
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiDrawVLine(*vis, NUM2INT(x), NUM2INT(y), NUM2INT(h))) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiDrawVLine failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      return INT2NUM(err);
    }
  }
  return Qnil;

}

static VALUE
visual_drawLine(VALUE self, VALUE x, VALUE y, VALUE xe, VALUE ye)
{
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiDrawLine(*vis, NUM2INT(x), NUM2INT(y), NUM2INT(xe), NUM2INT(ye))) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiDrawLine failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      return INT2NUM(err);
    }
  }
  return Qnil;

}

static VALUE
visual_drawBox(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h)
{
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiDrawBox(*vis, NUM2INT(x), NUM2INT(y), NUM2INT(w), NUM2INT(h))) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiDrawBox failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      return INT2NUM(err);
    }
  }
  return Qnil;

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

  if (err != 0) {
    rb_raise(eGGI, "drawRect failed (return value: %d)", err);
  }

  return Qnil;
}

/* ============================================================================================= */


static VALUE
visual_fillscreen(VALUE self)
{
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiFillscreen(*vis)) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiFillscreen failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      return INT2NUM(err);
    }
  }
  return Qnil;

}

static VALUE
visual_copyBox(VALUE self, VALUE x, VALUE y, VALUE w, VALUE h, VALUE nx, VALUE ny)
{
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiCopyBox(*vis, NUM2INT(x), NUM2INT(y), NUM2INT(w), NUM2INT(h), NUM2INT(nx), NUM2INT(ny))) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiCopyBox failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      return INT2NUM(err);
    }
  }
  return Qnil;

}

static VALUE
visual_setOrigin(VALUE self, VALUE x, VALUE y)
{
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiSetOrigin(*vis, NUM2INT(x), NUM2INT(y))) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiSetOrigin failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      return INT2NUM(err);
    }
  }
  return Qnil;

}

static VALUE
visual_getOrigin(VALUE self)
{
  int x, y;

  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiGetOrigin(*vis, &x, &y)) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiGetOrigin failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      
    }
  }
  


  return rb_ary_new3(2, INT2NUM(x), INT2NUM(y));
}

static VALUE
visual_putc(VALUE self, VALUE x, VALUE y, VALUE c)
{
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiPutc(*vis, NUM2INT(x), NUM2INT(y), NUM2CHR(c))) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiPutc failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      return INT2NUM(err);
    }
  }
  return Qnil;

}

static VALUE
visual_puts(VALUE self, VALUE x, VALUE y, VALUE str)
{
  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiPuts(*vis, NUM2INT(x), NUM2INT(y), STR2CSTR(str))) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiPuts failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      return INT2NUM(err);
    }
  }
  return Qnil;

}

static VALUE
visual_getCharSize(VALUE self)
{
  int w, h;

  ggi_visual_t *vis;
  int err;

  Data_Get_Struct(self, ggi_visual_t, vis);

  if ((err = ggiGetCharSize(*vis, &w, &h)) != 0)
  {
    if (err < 0)
    {
      rb_raise(eGGI, "ggiGetCharSize failed (return value: %d)", err);
      return Qnil;
    }
    else
    {
      
    }
  }
  


  return rb_ary_new3(2, INT2NUM(w), INT2NUM(h));
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

  if (em < 0)
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
    case evPtrButtonRelease:
      return Data_Wrap_Struct(cEventPtrButton, NULL, free, ev);
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

  cEvent = rb_define_class_under(mGGI, "Event", rb_cObject);
  cEventCommand = rb_define_class_under(cEvent, "Command", cEvent);
  cEventExpose = rb_define_class_under(cEvent, "Expose", cEvent);
  cEventValuator = rb_define_class_under(cEvent, "Valuator", cEvent);
  cEventKey = rb_define_class_under(cEvent, "Key", cEvent);
  cEventPtrMove = rb_define_class_under(cEvent, "PtrMove", cEvent);
  cEventPtrButton = rb_define_class_under(cEvent, "PtrButton", cEvent);

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
  rb_define_method(cVisual, "drawRect", visual_drawRect, 4);
  rb_define_method(cVisual, "setOrigin", visual_setOrigin, 2);
  rb_define_method(cVisual, "getOrigin", visual_getOrigin, 0);
  rb_define_method(cVisual, "putc", visual_putc, 3);
  rb_define_method(cVisual, "puts", visual_puts, 3);
  rb_define_method(cVisual, "getCharSize", visual_getCharSize, 0);
  rb_define_method(cVisual, "getc", visual_getc, 0);
  rb_define_method(cVisual, "kbhit", visual_kbhit, 0);
  rb_define_method(cVisual, "eventPoll", visual_eventPoll, 2);
  rb_define_method(cVisual, "eventsQueued", visual_eventsQueued, 1);
  rb_define_method(cVisual, "eventRead", visual_eventRead, 1);
}
 
