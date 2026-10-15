/* Portrait render surface presented on the handheld's landscape SDL window.
 * Unity sees the logical dimensions. Only its default FBO is redirected;
 * game-created render targets retain their native GL semantics. */
#include "threes_display.h"
#include <SDL2/SDL.h>
#include <GLES2/gl2.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

struct threes_target { GLuint fbo, color, depth, program, vao, vbo; int w, h; };
static _Thread_local struct threes_target *active;
static int panel_w, panel_h;
static atomic_int tate, resized;
static void (*gen_vaos)(GLsizei, GLuint *);
static void (*bind_vao)(GLuint);

void threes_display_init(int w, int h, int portrait) {
  panel_w=w; panel_h=h; atomic_store(&tate,portrait);
}
int threes_display_tate(void) { return atomic_load(&tate); }
int threes_display_width(void) { return panel_h; }
int threes_display_height(void) { return panel_w; }
void threes_display_toggle(void) { atomic_store(&tate,!atomic_load(&tate)); atomic_store(&resized,1); }
int threes_display_take_resize(void) { return atomic_exchange(&resized,0); }

static void fail(const char *what) {
  fprintf(stderr,"[THREES-DISPLAY] %s (GL=%x)\n",what,glGetError()); exit(23);
}
static GLuint shader(GLenum type, const char *source) {
  GLuint id=glCreateShader(type); glShaderSource(id,1,&source,NULL); glCompileShader(id);
  GLint ok=0; glGetShaderiv(id,GL_COMPILE_STATUS,&ok);
  if(!ok){char log[1024];glGetShaderInfoLog(id,sizeof(log),NULL,log);fprintf(stderr,"%s\n",log);fail("compositor shader");}
  return id;
}
static void create_program(struct threes_target *s) {
  gen_vaos=(void *)SDL_GL_GetProcAddress("glGenVertexArrays");
  bind_vao=(void *)SDL_GL_GetProcAddress("glBindVertexArray");
  if(!gen_vaos||!bind_vao)fail("GLES3 vertex arrays unavailable");
  const char *vert="attribute vec2 position; varying vec2 uv; uniform int rotated; void main(){ vec2 p=position*.5+.5; uv=rotated!=0?vec2(p.y,1.-p.x):p; gl_Position=vec4(position,0.,1.); }";
  const char *frag="precision mediump float; varying vec2 uv; uniform sampler2D image; uniform float fit; void main(){vec2 q=vec2((uv.x-.5)/fit+.5,uv.y); gl_FragColor=q.x<0.||q.x>1.?texture2D(image,vec2(.005,.995)):texture2D(image,q);}";
  GLuint vs=shader(GL_VERTEX_SHADER,vert),fs=shader(GL_FRAGMENT_SHADER,frag);
  s->program=glCreateProgram();glAttachShader(s->program,vs);glAttachShader(s->program,fs);
  glBindAttribLocation(s->program,0,"position");glLinkProgram(s->program);
  GLint ok=0;glGetProgramiv(s->program,GL_LINK_STATUS,&ok);if(!ok)fail("compositor link");
  glDeleteShader(vs);glDeleteShader(fs);
  GLint old_vao,old_buffer;glGetIntegerv(0x85b5,&old_vao);glGetIntegerv(GL_ARRAY_BUFFER_BINDING,&old_buffer);
  static const GLfloat quad[]={-1,-1,1,-1,-1,1,1,1};
  gen_vaos(1,&s->vao);bind_vao(s->vao);glGenBuffers(1,&s->vbo);glBindBuffer(GL_ARRAY_BUFFER,s->vbo);
  glBufferData(GL_ARRAY_BUFFER,sizeof(quad),quad,GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,0,0);
  bind_vao(old_vao);glBindBuffer(GL_ARRAY_BUFFER,old_buffer);
}
static void ensure_target(void) {
  struct threes_target *s=active;if(!s)return;
  int w=threes_display_width(),h=threes_display_height();
  if(s->fbo&&s->w==w&&s->h==h)return;
  GLint texture,renderbuffer,fb;
  glGetIntegerv(GL_TEXTURE_BINDING_2D,&texture);glGetIntegerv(GL_RENDERBUFFER_BINDING,&renderbuffer);
  glGetIntegerv(GL_FRAMEBUFFER_BINDING,&fb);
  GLuint previous=s->fbo;
  if(s->fbo){glDeleteFramebuffers(1,&s->fbo);glDeleteTextures(1,&s->color);glDeleteRenderbuffers(1,&s->depth);}
  glGenTextures(1,&s->color);glBindTexture(GL_TEXTURE_2D,s->color);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
  glGenRenderbuffers(1,&s->depth);glBindRenderbuffer(GL_RENDERBUFFER,s->depth);
  glRenderbufferStorage(GL_RENDERBUFFER,0x88f0,w,h); /* DEPTH24_STENCIL8 */
  glGenFramebuffers(1,&s->fbo);glBindFramebuffer(GL_FRAMEBUFFER,s->fbo);
  glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,s->color,0);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,s->depth);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_STENCIL_ATTACHMENT,GL_RENDERBUFFER,s->depth);
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE)fail("portrait framebuffer incomplete");
  s->w=w;s->h=h;if(!s->program)create_program(s);
  glBindTexture(GL_TEXTURE_2D,texture);glBindRenderbuffer(GL_RENDERBUFFER,renderbuffer);
  glBindFramebuffer(GL_FRAMEBUFFER,fb==(GLint)previous?s->fbo:(GLuint)fb);
  fprintf(stderr,"[THREES-DISPLAY] logical=%dx%d panel=%dx%d rotation=%d\n",w,h,panel_w,panel_h,threes_display_tate()?90:0);
}
void threes_display_context(struct threes_target **slot,int is_window) {
  if(!is_window){active=NULL;return;}
  if(!*slot)*slot=calloc(1,sizeof(**slot));
  active=*slot;ensure_target();glBindFramebuffer(GL_FRAMEBUFFER,active->fbo);
}
void threes_display_bind_framebuffer(unsigned target,unsigned framebuffer) {
  if(active&&framebuffer==0){ensure_target();framebuffer=active->fbo;}
  glBindFramebuffer(target,framebuffer);
}
void threes_display_present(void) {
  struct threes_target *s=active;if(!s||!s->fbo)return;
  GLint program,vao,buffer,viewport[4],fb,unit,texture;
  GLboolean mask[4];
  GLenum caps[]={GL_BLEND,GL_DEPTH_TEST,GL_STENCIL_TEST,GL_SCISSOR_TEST,GL_CULL_FACE};
  GLboolean enabled[5];
  glGetIntegerv(GL_CURRENT_PROGRAM,&program);glGetIntegerv(0x85b5,&vao);
  glGetIntegerv(GL_ARRAY_BUFFER_BINDING,&buffer);glGetIntegerv(GL_VIEWPORT,viewport);
  glGetIntegerv(GL_FRAMEBUFFER_BINDING,&fb);glGetIntegerv(GL_ACTIVE_TEXTURE,&unit);
  glActiveTexture(GL_TEXTURE0);glGetIntegerv(GL_TEXTURE_BINDING_2D,&texture);
  glGetBooleanv(GL_COLOR_WRITEMASK,mask);
  for(int i=0;i<5;i++){enabled[i]=glIsEnabled(caps[i]);glDisable(caps[i]);}
  glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
  glBindFramebuffer(GL_FRAMEBUFFER,0);
  /* Keep Unity's portrait backbuffer stable. Landscape presents the entire
   * portrait surface fitted to panel height, rather than cropping a viewport
   * whose dimensions Unity still caches from boot. */
  GLfloat clear[4];glGetFloatv(GL_COLOR_CLEAR_VALUE,clear);
  glViewport(0,0,panel_w,panel_h);glClearColor(0,0,0,1);glClear(GL_COLOR_BUFFER_BIT);
  glClearColor(clear[0],clear[1],clear[2],clear[3]);
  glUseProgram(s->program);glUniform1i(glGetUniformLocation(s->program,"image"),0);
  glUniform1i(glGetUniformLocation(s->program,"rotated"),threes_display_tate());
  glUniform1f(glGetUniformLocation(s->program,"fit"),threes_display_tate()?1.f:(float)(panel_h*s->w)/(s->h*panel_w));
  glBindTexture(GL_TEXTURE_2D,s->color);bind_vao(s->vao);glDrawArrays(GL_TRIANGLE_STRIP,0,4);
  bind_vao(vao);glBindBuffer(GL_ARRAY_BUFFER,buffer);glUseProgram(program);
  glBindTexture(GL_TEXTURE_2D,texture);glActiveTexture(unit);
  glColorMask(mask[0],mask[1],mask[2],mask[3]);
  for(int i=0;i<5;i++)if(enabled[i])glEnable(caps[i]);
  glViewport(viewport[0],viewport[1],viewport[2],viewport[3]);glBindFramebuffer(GL_FRAMEBUFFER,fb);
}
