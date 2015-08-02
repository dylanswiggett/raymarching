#version 430 core

layout(location = 0) out vec3 color;

struct Sphere {
  vec3 pos;
  float rad;
  vec3 color;
  float light_emit;
};

uniform spheres
{
  Sphere sphere_list[256];
};

uniform int numspheres;
uniform float whratio;
uniform int t;
uniform float user_seed;
uniform vec3 campos;
uniform vec3 camdir, camup, camright;
in vec2 screenp;

float seed;

float rand(vec3 co) {
  float v1 = fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
  return fract(sin(dot(vec2(co.z, v1) ,vec2(12.9898,78.233))) * 43758.5453);
}

float interpolate(float v1, float v2, float d) {
  float ft = d * 3.141592;
  float f = (1.0 - cos(ft)) * .5;
  return v1 * (1.0-f) + v2 * f;
}

float randAt(vec3 pos) {
  vec3 posSq = floor(pos);
  vec3 disp = pos - posSq;
  float lll = rand(posSq + vec3(0,0,0));
  float llr = rand(posSq + vec3(0,0,1));
  float lrl = rand(posSq + vec3(0,1,0));
  float lrr = rand(posSq + vec3(0,1,1));
  float rll = rand(posSq + vec3(1,0,0));
  float rlr = rand(posSq + vec3(1,0,1));
  float rrl = rand(posSq + vec3(1,1,0));
  float rrr = rand(posSq + vec3(1,1,1));

  float ll = interpolate(lll, llr, disp.z);
  float lr = interpolate(lrl, lrr, disp.z);
  float rl = interpolate(rll, rlr, disp.z);
  float rr = interpolate(rrl, rrr, disp.z);
	
  float l = interpolate(ll, lr, disp.y);
  float r = interpolate(rl, rr, disp.y);

  return interpolate(l, r, disp.x);
}

float perlin(vec3 pos, float scale, float prod, float turb, int levels) {
  float sum = 0.0;
  float product = 1.0;
  float sampleSize = scale;
  for (int lvl = 0; lvl < levels; lvl++) {
    sum += randAt(pos * sampleSize) * product;
    product *= prod;
    sampleSize *= turb;
  }
  return sum;
}

float minDist(vec3 pos) {
  return perlin(pos, .4, .7, 2, 6) - 1;
  /*
    float s = 3;
    float r = 10;
    vec3 q = mod(pos,vec3(r,r,r))-0.5*vec3(r,r,r);

    vec3 t = vec3(1,1,1);
    vec2 v = vec2(length(q.xz)-t.x,q.y);
    return length(v)-t.y;
  */
}

vec3 projectRay(vec3 pos, vec3 dir, int maxDist) {
  int nsteps = 0;
  float raylen = 0;
  float dist = minDist(pos);
  while (raylen < maxDist && dist > .01 && nsteps < 10) {
    raylen += dist;
    dist = minDist(pos + dir * raylen);
    nsteps++;
  }
  float v = 1 - raylen / maxDist;
  return vec3(v,v,v);
}

void main() {
  vec3 pos = campos;
  vec3 dir = camdir;
  dir += screenp.x * whratio * camright;
  dir += screenp.y * camup;
  dir = normalize(dir);
  //vec3 dir = normalize(vec3(screenp.x * whratio, screenp.y, 1));
  pos = pos + dir;
  color = projectRay(pos, dir, 20);
}