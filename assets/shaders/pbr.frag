// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#version 450

#include "material.glsl"
#include "camera_data.glsl"

layout(location = 0) out vec4 o_color;

struct VertexOutput {
  Material material;
  vec3 frag_pos;
  vec3 normal;
  vec2 tex_coords;
  float tex_index;
};

layout(location = 0) in VertexOutput v_input;

layout(set = 0, binding = 0) uniform sampler2D u_textures[32];

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness) {
  float a = roughness * roughness;
  float a2 = a * a;
  float NdotH = max(dot(N, H), 0.0);
  float NdotH2 = NdotH * NdotH;

  float nom = a2;
  float denom = (NdotH2 * (a2 - 1.0) + 1.0);
  denom = PI * denom * denom;

  return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
  float r = (roughness + 1.0);
  float k = (r * r) / 8.0;

  float nom = NdotV;
  float denom = NdotV * (1.0 - k) + k;

  return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
  float NdotV = max(dot(N, V), 0.0);
  float NdotL = max(dot(N, L), 0.0);
  float ggx2 = GeometrySchlickGGX(NdotV, roughness);
  float ggx1 = GeometrySchlickGGX(NdotL, roughness);

  return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0) {
  return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main() {

  vec3 lightPositions[] = {
  vec3(-10.0, 10.0, 10.0), vec3(10.0, 10.0, 10.0), vec3(-10.0, -10.0, 10.0), vec3(10.0, -10.0, 10.0) };

vec3 lightColors[] = {
vec3(300.0, 300.0, 300.0), vec3(300.0, 300.0, 300.0), vec3(300.0, 300.0, 300.0), vec3(300.0, 300.0, 300.0) };

vec3 N = normalize(v_input.normal);
vec3 V = normalize(u_camera.position - v_input.frag_pos);

  // int index = int(v_input.tex_index);
  // vec4 sampled = texture(u_textures[index], v_input.tex_coords);

  // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
  // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
vec3 F0 = vec3(0.04);
F0 = mix(F0, v_input.material.albedo, v_input.material.metallic);

    // reflectance equation
vec3 Lo = vec3(0.0);
for(int i = 0;
i < 4;
++ i) {
        // calculate per-light radiance
vec3 L = normalize(lightPositions[i] - v_input.frag_pos);
vec3 H = normalize(V + L);
float distance = length(lightPositions[i] - v_input.frag_pos);
float attenuation = 1.0 / (distance * distance);
vec3 radiance = lightColors[i] * attenuation;

        // Cook-Torrance BRDF
float NDF = DistributionGGX(N, H, v_input.material.roughness);
float G = GeometrySmith(N, V, L, v_input.material.roughness);
vec3 F = FresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

vec3 numerator = NDF * G * F;
float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
vec3 specular = numerator / denominator;

        // kS is equal to Fresnel
vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
kD *= 1.0 - v_input.material.metallic;	  

        // scale light by NdotL
float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
Lo += (kD * v_input.material.albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
}   

    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
vec3 ambient = vec3(0.03) * v_input.material.albedo * v_input.material.ao;

vec3 color = ambient + Lo;

    // HDR tonemapping
color = color / (color + vec3(1.0));
    // gamma correct
color = pow(color, vec3(1.0 / 2.2));

o_color = vec4(color, 1.0);
}
