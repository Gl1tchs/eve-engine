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
  vec3 lightDirection = normalize(vec3(1.0, -1.0, 1.0)); // Direction of the global directional light
  vec3 lightColor = vec3(1.0, 1.0, 1.0); // Color of the global directional light

  vec3 N = normalize(v_input.normal);
  vec3 V = normalize(u_camera.position - v_input.frag_pos);

  vec3 F0 = vec3(0.04);
  F0 = mix(F0, v_input.material.albedo, v_input.material.metallic);

  vec3 Lo = vec3(0.0);

  for(int i = 0; i < 4; ++i) {
    // calculate per-light radiance
    vec3 L = normalize(lightDirection);
    vec3 H = normalize(V + L);
    float distance = length(lightDirection);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = lightColor * attenuation;

    // Cook-Torrance BRDF
    float NDF = DistributionGGX(N, H, v_input.material.roughness);
    float G = GeometrySmith(N, V, L, v_input.material.roughness);
    vec3 F = FresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - v_input.material.metallic;

    float NdotL = max(dot(N, L), 0.0);

    Lo += (kD * v_input.material.albedo / PI + specular) * radiance * NdotL;
  }

  vec3 ambient = vec3(0.03) * v_input.material.albedo * v_input.material.ao;

  vec3 color = ambient + Lo;

  color = color / (color + vec3(1.0));
  color = pow(color, vec3(1.0 / 2.2));

  o_color = vec4(color, 1.0);
}